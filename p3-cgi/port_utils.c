#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "port_utils.h"

#define BASE_PORT 4000

char *
get_port (void)
{
  static char portno[6];
  const char *users[] = {
    "bergermj", "chaudhhx",  "chen6wx",  "clevelcd", "david2ja", "dimma",
    "dolansdg", "egannt",    "fryerak",  "hinemp",   "hopkincm", "jones2nb",
    "kangay",   "lanetb",    "lewisjw",  "lipsih",   "lucas4ac", "lucassr",
    "ludwigad", "morellcw",  "newsommr", "nguye4lt", "peter2js", "pham2jt",
    "smithhz",  "teshomat",  "tjadenbc", "wistjp",   NULL
  };

  char *user = getlogin ();
  size_t i = 0;
  while (users[i] != NULL)
    {
      if (!strncmp (user, users[i], 8))
        {
          memset (portno, 0, sizeof (portno));
          short int port = (short int)BASE_PORT + i;
          snprintf (portno, sizeof (portno), "%hu", port);
          return portno;
        }
      i++;
    }
  return NULL;
}
