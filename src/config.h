#ifndef CONFIG_H
# define CONFIG_H

# define SERVER_STRING "mbhttpd/0.1"

/*
  Buffer Size	|	Mean Performance
=========================================
  128 (witness)	|	  39116.01
  8192		|	 802289.36
  8192 * 4	|	1144879.54
  8192 * 8	|	1116878.95
  8192 * 24	|	1215090.32
  8192 * 512	|	 722482.57
*/

# define CLIENT_BUFFER_SIZE	32768
# define MAX_CLIENTS		250
# define MAX_FILENAME		1024
# define CLIENT_TIMEOUT		60 /* in seconds */

#endif
