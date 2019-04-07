#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>
#include "config.h"
#include "core.h"
#include "error_utils.h"
#include "utils.h"
#include "send_data.h"
#include "xmalloc.h"
#include "response.h"

/* SIGPIPE may happen if the connection is closed by the other side */
static void sigpipe_handler(int UNUSED(sig))
{
  my_error("SIGPIPE received", 0);
  signal(SIGPIPE, sigpipe_handler);
}

static int init(const mbhttpd_config_t *mbhttpd_config)
{
  int sock_fd;
  struct sockaddr_in sockaddr;

  sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_port = htons(mbhttpd_config->port);
  sockaddr.sin_addr.s_addr = INADDR_ANY;
  if (bind(sock_fd, (struct sockaddr*) &sockaddr, sizeof(sockaddr)) < 0)
    return my_perror("bind failed", SYS_ERROR);
  if (listen(sock_fd, MAX_CLIENTS)  < 0)
    return my_perror("listen failed", SYS_ERROR);
  printf("Listening on port %d\n", mbhttpd_config->port);
  return sock_fd;
}

static int fill_fd_sets(fd_set *rfds, fd_set *wfds, fd_set *efds,
			client_t *clients, int sock_fd) {
  int i, max_fd = sock_fd;

  FD_ZERO(rfds);
  FD_ZERO(wfds);
  FD_ZERO(efds);
  FD_SET(sock_fd, rfds);
  for (i = 0; i < MAX_CLIENTS; ++i) {
    if (clients[i].fd != 0) {
      FD_SET(clients[i].fd, efds);
      switch (clients[i].state) {
      case RECEIVE:
	FD_SET(clients[i].fd, rfds);
	break ;
      case WRITE:
      case WRITE_HEADERS:
      case WRITE_FILE:
	FD_SET(clients[i].fd, wfds);
	break ;
      case CLOSE:
	remove_client(&clients[i]);
	break ;
      default:
	break ;
      }
      if (clients[i].fd > max_fd)
	max_fd = clients[i].fd;
    }
  }
  return max_fd;
}

static int set_nonblocking_fd(int client_sockfd) {
  int flags;

  if ((flags = fcntl(client_sockfd, F_GETFL, 0)) < 0)
    return my_perror("fcntl failed", SYS_ERROR);
  if (fcntl(client_sockfd, F_SETFL, flags | O_NONBLOCK) < 0)
    return my_perror("fcntl failed", SYS_ERROR);
  return 0;
}

static int new_connection(int sock_fd, client_t *clients) {
  int client_sock_fd, i;

  if ((client_sock_fd = accept(sock_fd, NULL, NULL)) < 0)
    return my_perror("accept failed", SYS_ERROR);
  set_nonblocking_fd(client_sock_fd);
  for (i = 0; i < MAX_CLIENTS; ++i) {
    if (clients[i].fd == 0) {
      clients[i].last_activity = time(NULL);
      clients[i].fd = client_sock_fd;
      clients[i].state = RECEIVE;
      break ;
    }
  }
  if (i == MAX_CLIENTS) {
    close(client_sock_fd);
    return my_error("too many clients connected", SYS_ERROR);
  }
  return client_sock_fd;
}

static int test_fd_sets(fd_set *rfds, fd_set *wfds, fd_set *efds,
			client_t *clients, int sock_fd) {
  int i;
  time_t cur_time = time(NULL);

  if (FD_ISSET(sock_fd, rfds))
    new_connection(sock_fd, clients);

  for (i = 0; i < MAX_CLIENTS; ++i) {
    if (clients[i].fd != 0) {
      if (FD_ISSET(clients[i].fd, efds)) {
 	/* next read or write will return an error, do i need to do
	   something there ? */
      }
      if (FD_ISSET(clients[i].fd, rfds)) {
	clients[i].last_activity = cur_time;
	switch (read_request(&clients[i])) {
	case 2:
	  parse_request(&clients[i].request);
	  clients[i].state = WRITE;
	  break ;
	case 3:
	  remove_client(&clients[i]);
	}
      }
      if (FD_ISSET(clients[i].fd, wfds)) {
	clients[i].last_activity = cur_time;
	send_response(&clients[i]);
      }
      if (clients[i].last_activity &&					\
	  ((cur_time - clients[i].last_activity) > CLIENT_TIMEOUT)) {
	remove_client(&clients[i]);
	my_error("connection timed out", 0);
      }
    }
  }
  return 0;
}

int remove_client(client_t *client)
{
  xfree(client->request.request_str);
  xfree(client->response.headers);
  close(client->fd);
  /* memset(0) required to set the element free */
  memset(client, 0, sizeof(client_t));
  return 0;
}

int accept_loop(const mbhttpd_config_t *mbhttpd_config)
{
  struct timeval timeval;
  fd_set rfds, wfds, efds;
  static client_t clients[MAX_CLIENTS];
  int sret, sock_fd, max_fd;

  memset(clients, 0, sizeof(client_t) * MAX_CLIENTS);
  memset(&rfds, 0, sizeof(fd_set));
  signal(SIGPIPE, sigpipe_handler);
  if ((sock_fd = init(mbhttpd_config)) < 0)
    return SYS_ERROR;
  set_nonblocking_fd(sock_fd);
  while (1) {
    timeval.tv_sec = 5;
    timeval.tv_usec = 0;
    max_fd = fill_fd_sets(&rfds, &wfds, &efds, clients, sock_fd);
    if ((sret = select(max_fd + 1, &rfds, &wfds, &efds, &timeval) == -1))
      my_perror("select failed", 0);
    test_fd_sets(&rfds, &wfds, &efds, clients, sock_fd);
  }
  return 0;
}
