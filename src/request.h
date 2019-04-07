#ifndef REQUEST_H
# define REQUEST_H


typedef struct {
  char method;
  char *pathname;
  char *request_str;
} request_t;

int free_request(request_t *);
int parse_request(request_t *request);

#endif
