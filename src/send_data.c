#include <fcntl.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "utils.h"
#include "config.h"
#include "send_data.h"
#include "error_utils.h"
#include "response.h"
#include "core.h"
#include "methods.h"
#include "xmalloc.h"
#include "filetype.h"

static char *open_file_to_send(client_t *client)
{
  struct stat fstatus;

  if (!client->request.pathname)
    return HTTP_NOT_FOUND;
  if (stat(client->request.pathname, &fstatus) < 0) {
    my_perror("fstat failed", 0);
    if (client->response.fd > 0)
      close(client->response.fd);
    return HTTP_NOT_FOUND;
  }
  if (!S_ISREG(fstatus.st_mode)) {
    my_error("not a regular file", 0);
    if (client->response.fd > 0)
      close(client->response.fd);
    return HTTP_NOT_FOUND;
  }
  if ((client->response.fd = open(client->request.pathname, 0)) < 0) {
    my_perror("open failed", 0);
    if (client->response.fd > 0)
      close(client->response.fd);
    return HTTP_NOT_FOUND;
  }
  memset(client->buffer, 0, CLIENT_BUFFER_SIZE);
  if ((client->response.read_bytes = read(client->response.fd, client->buffer,
					  CLIENT_BUFFER_SIZE)) < 0) {
    my_perror("read error", 0);
    return HTTP_NOT_FOUND;
  }
  return HTTP_OK;
}

static int send_headers(client_t *client) {
  size_t len = client->response.headers_len;
  ssize_t sent;

  if (client->response.sent_bytes < len) {
    sent = send(client->fd,
		client->response.headers + client->response.sent_bytes,
		len - client->response.sent_bytes, 0);
    if (sent < 0)
      return my_perror("send failed", SYS_ERROR);
    client->response.sent_bytes += sent;
    if (client->response.sent_bytes == len) {
      client->response.sent_bytes = 0;
      client->state = client->next_state;
    }
  }
  return 0;
}

static int add_header(client_t *client, const char *name, const char *data)
{
  size_t len = client->response.headers_len;

  client->response.headers = realloc(client->response.headers,
				     len + strlen(name) + strlen(data) + 1);
  strncpy(client->response.headers + len, name, strlen(name));
  strncpy(client->response.headers + len + strlen(name), data,
	  strlen(data) + 1);
  client->response.headers_len += strlen(name) + strlen(data);
  return 0;
}

static int build_headers(client_t *client, const char *status)
{
  /* FIXME: define size instead of strlens */
  client->response.headers = (char *) xmalloc(sizeof(char)		\
		     * (strlen(HTTP_STATUS) + strlen(status) + 1));
  memset(client->response.headers, 0, sizeof(char)	\
	 * (strlen(HTTP_STATUS) + strlen(status) + 1));
  strncpy(client->response.headers, HTTP_STATUS, strlen(HTTP_STATUS));
  strncpy(client->response.headers + strlen(HTTP_STATUS), status,
	  strlen(status));
  client->response.headers_len = strlen(HTTP_STATUS) + strlen(status);
  if (strcmp(status, HTTP_OK) == 0) {
    client->next_state = WRITE_FILE;
    add_header(client, "Content-type: ", detect_filetype(client));
  } else {
    client->next_state = CLOSE;
  }
  add_header(client, CRLF, CRLF);
  return 0;
}

int send_file(client_t *client) {
  ssize_t ssent = 1;

  ssent = send(client->fd, client->buffer + client->response.sent_bytes \
	       % CLIENT_BUFFER_SIZE, client->response.read_bytes - \
	       client->response.sent_bytes % CLIENT_BUFFER_SIZE, 0);
  if (ssent < 0) {
    my_perror("sent failed", 0);
    close(client->response.fd);
    client->state = CLOSE;
    return -1;
  }
  client->response.sent_bytes += ssent;
  if (ssent == (ssize_t) (client->response.read_bytes -			\
			  client->response.sent_bytes % CLIENT_BUFFER_SIZE)) {
    client->response.read_bytes = read(client->response.fd, client->buffer, \
				       CLIENT_BUFFER_SIZE);
    if (!client->response.read_bytes) {
      close(client->response.fd);
      remove_client(client);
    }
  }
  return 0;
}



int send_response(client_t *client)
{
  const char *open_retval;

  switch (client->state) {
  case WRITE:
    client->state = WRITE_HEADERS;
    open_retval = open_file_to_send(client);
    build_headers(client, open_retval);
    return send_headers(client);
  case WRITE_HEADERS:
    return send_headers(client);
  case WRITE_FILE:
    return dispatch_method(client);
  case RECEIVE:
    return SYS_ERROR;
  default:
    break ;
  }
  return 0;
}
