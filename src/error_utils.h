#ifndef ERROR_UTILS_H
# define ERROR_UTILS_H

# define SYS_ERROR -1

int my_perror(const char *err_string, int ret_code);
int my_error(const char *err_string, int ret_code);

#endif
