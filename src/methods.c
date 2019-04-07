#include "utils.h"
#include "methods.h"
#include "send_data.h"

/* we don't want gcc complains about unused variables */

int get_method(client_t *client)
{
  return send_file(client);
}

int not_implemented(client_t *UNUSED(client))
{
  /* FIXME: todo */
  return 0;
}

int unknown_method(client_t *UNUSED(client))
{
  /* FIXME: todo */
  return 0;
}


int dispatch_method(client_t *client) {
  int i;

  for (i = 0; (methods[i].code != UNKNOWN_METHOD) \
	 && (methods[i].code != client->request.method); ++i) ;
  return methods[i].func(client);
}
