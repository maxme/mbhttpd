#include <stdlib.h>
#include <stdio.h>
#include "xmalloc.h"

void *xmalloc(size_t size)
{
  void *p = malloc(size);

  if (!p)
    perror("malloc failed");
  return p;
}

void xfree(void *p)
{
  free(p);
  p = NULL;
}
