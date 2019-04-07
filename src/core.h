#ifndef CORE_H
# define CORE_H

# include <sys/types.h>
# include <time.h>
# include "config.h"
# include "request.h"
# include "mbhttpd.h"

typedef enum {IDLE, RECEIVE, WRITE, WRITE_HEADERS, WRITE_FILE, CLOSE} state_e;

typedef struct {
  char *headers;
  ssize_t headers_len;
  size_t sent_bytes;
  ssize_t read_bytes;
  int fd;
} response_t;

typedef struct {
  int fd;
  state_e state;
  state_e next_state;
  request_t request;
  response_t response;
  char buffer[CLIENT_BUFFER_SIZE];
  time_t last_activity;
} client_t;

int accept_loop(const mbhttpd_config_t *);
int read_request(client_t *);
int remove_client(client_t *);

#endif
