#include <stdio.h>

int my_perror(const char *err_string, int ret_code)
{
  perror(err_string);
  return ret_code;
}

int my_error(const char *err_string, int ret_code)
{
  fprintf(stderr, "Error: %s\n", err_string);
  return ret_code;
}
