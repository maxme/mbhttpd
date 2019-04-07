#include <ctype.h>
#include <string.h>
#include "filetype.h"
#include "response.h"
#include "utils.h"


/* FIXME: import full array from another project ? */
/* Warning: check MAX_*_SIZE macro and filetype_t before modifying the array */
static const filetype_t filetypes[] =
  {
    {".html", "text/html"},
    {".htm", "text/html"},
    {".txt", "text/plain"},
    {".pdf", "application/pdf"},
    {"", ""}
  };

static char *detect_binary(const char *buffer, unsigned buffer_size)
{
  int i, res = 0;

  for (i = 0; i < BINARY_TEST_N; ++i) {
    res += isprint(buffer[(297 * i) % buffer_size]);
  }
  return (res < ASCII_THRESHOLD) ? MIME_BIN_DEFAULT : MIME_ASCII_DEFAULT;
}

char *detect_filetype(const client_t *client)
{
  int i;

  for (i = 0; *filetypes[i].ext &&			    \
	 strncmp(filetypes[i].ext, client->request.pathname \
		 + strlen(client->request.pathname)	    \
		 - MYMIN(strlen(client->request.pathname),
		       strlen(filetypes[i].ext)),
		 strlen(filetypes[i].ext)) != 0; ++i) ;
  if (!*filetypes[i].ext)
    return detect_binary(client->buffer, CLIENT_BUFFER_SIZE);
  return (char *) filetypes[i].mime;
}
