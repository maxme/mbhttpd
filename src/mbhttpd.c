#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include "xmalloc.h"
#include "mbhttpd.h"
#include "core.h"

int main(int argc, char **argv)
{
  mbhttpd_config_t mbhttpd_config;
  int c;
  extern char *optarg;
  extern int optopt;

  memset(&mbhttpd_config, 0, sizeof(mbhttpd_config));
  for (; (c = getopt(argc, argv, ":p:")) != -1;) {
    switch (c) {
    case 'p':
      mbhttpd_config.port = atoi(optarg);
      break;
    case ':':
      fprintf(stderr, "Option -%c requires an argument.\n", optopt);
      return EXIT_ARG_FAILURE;
    default:
      abort();
    }
  }
  if (!mbhttpd_config.port)
    mbhttpd_config.port = 8080;
  if (accept_loop(&mbhttpd_config) < 0)
    return EXIT_INIT_FAILURE;
  return 0;
}
