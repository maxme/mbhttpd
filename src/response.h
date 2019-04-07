#ifndef RESPONSE_H
# define RESPONSE_H

# define HTTP_OK                        "200 OK\r\n"
# define HTTP_BAD_REQUEST               "400 Bad request\r\n"
# define HTTP_NOT_FOUND                 "404 Not found\r\n"
# define HTTP_REQUEST_URI_TOO_LARGE     "414 Request URI too large\r\n"

# define HTTP_STATUS			"HTTP/1.0 "
# define HTTP_STATUS_S			10

# define MIME_ASCII_DEFAULT		"text/plain"
# define MIME_BIN_DEFAULT		"application/octet-stream"

# define MAX_HEADER_NAME                12

# include "core.h"

int send_response(client_t *);

#endif
