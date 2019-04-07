#ifndef FILETYPE_H
# define FILETYPE_H

# include "core.h"

char *detect_filetype(const client_t *);

# define BINARY_TEST_N 50
# define ASCII_THRESHOLD 30
# define MAX_EXT_SIZE 6
# define MAX_MIME_SIZE 17

typedef struct {
  char ext[MAX_EXT_SIZE];
  char mime[MAX_MIME_SIZE];
} filetype_t;

#endif
