#ifndef XMALLOC_H
# define XMALLOC_H

# include <unistd.h>

void *xmalloc(size_t size);
void xfree(void *p);

#endif
