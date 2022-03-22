#include <stdio.h>
#include <stdlib.h>

#include "port_utils.h"

int
main (void)
{
  char *port = get_port ();
  printf ("%s\n", port);

  return EXIT_SUCCESS;
}
