#ifndef METHODS_H
# define METHODS_H

# define METHOD_STRING_MAXSIZE 5

# define UNKNOWN_METHOD -1
# define GET_METHOD 0
# define HEAD_METHOD 1
# define POST_METHOD 2

# include <stdio.h>
# include "request.h"
# include "core.h"

typedef struct {
  char str[METHOD_STRING_MAXSIZE];
  int code;
  int (*func)(client_t *);
} method_t;

int get_method(client_t *client);
int unknown_method(client_t *client);
int not_implemented(client_t *client);
int dispatch_method(client_t *client);

/*
  Static declaration in the .h because the array is used in parsing and
  dispatching functions
*/
static const method_t methods[] =
  {
    {"GET ", GET_METHOD, get_method},
    {"HEAD ", HEAD_METHOD, not_implemented},
    {"POST ", POST_METHOD, not_implemented},
    {"", UNKNOWN_METHOD, unknown_method} /* Keep UNKNOWN_METHOD the last */
  };

#endif
