#ifndef UTILS_H
# define UTILS_H

/* UNUSED tip: http://sourcefrog.net/weblog/software/languages/C/unused.html */
# ifdef UNUSED
# elif defined(__GNUC__)
#  define UNUSED(x) UNUSED_ ## x __attribute__((unused))
# elif defined(__LCLINT__)
#  define UNUSED(x) /*@unused@*/ x
# else
#  define UNUSED(x) x
# endif

# define MYMAX(a, b) ((a) < (b) ? (b) : (a))
# define MYMIN(a, b) ((a) > (b) ? (b) : (a))

# ifdef __DARWIN_UNIX03
#  define FILE_FD _file
# else
#  define FILE_FD _fileno
# endif

# define CRLF "\r\n"

#endif
