#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include "xmalloc.h"
#include "methods.h"
#include "core.h"
#include "utils.h"
#include "error_utils.h"

static char *parse_pathname(char *request_str) {
  char *p;

  p = strtok(request_str," \r\n");
  return p;
}

int read_request(client_t *client) {
  static char buffer[CLIENT_BUFFER_SIZE];
  unsigned strsize = 0, size;

  memset(buffer, 0, CLIENT_BUFFER_SIZE);
  if ((size = recv(client->fd, buffer, CLIENT_BUFFER_SIZE, MSG_DONTWAIT)) > 0) {
    if (client->request.request_str)
      strsize = strlen(client->request.request_str);
    /* We don't want people grow our memory as they want */
    if (strsize + size > MAX_FILENAME)
      return 3;
    client->request.request_str = realloc(client->request.request_str,
					  strsize + size + 1);
    strncpy(client->request.request_str + strsize, buffer, size + 1);
  }
  if (client->request.request_str)
    if (strstr(client->request.request_str, CRLF) != NULL)
      return 2;
  if (size == 0) {
    return 2;
  }
  return 0;
}

int parse_request(request_t *request) {
  int i;
  unsigned method_len;

  if (!request->request_str)
    return SYS_ERROR;
  for (i = 0; methods[i].code != UNKNOWN_METHOD; ++i)
    if (strncmp(methods[i].str, request->request_str,
		(method_len = strlen(methods[i].str))) == 0)
      break ;
  request->method = methods[i].code;
  if (request->method != UNKNOWN_METHOD)
    request->pathname = parse_pathname(request->request_str + method_len);
  return 0;
}
