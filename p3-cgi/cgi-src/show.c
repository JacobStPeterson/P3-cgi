#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>

int
main ()
{
  // Starter HTML (to avoid copy-and-paste annoynances):
  printf ("<html lang=\"en\">\n");
  printf ("  <head>\n");
  printf ("    <title>File Hash Database</title>\n");
  printf ("    <link rel=\"stylesheet\" href=\"https://stackpath.bootstrapcdn."
          "com/bootstrap/4.3.1/css/bootstrap.min.css\" integrity=\""
          "sha384-ggOyR0iXCbMQv3Xipma34MD+dH/1fQ784/j6cY/"
          "iJTQUOhcWr7x9JvoRxT2MZw1T\" crossorigin=\"anonymous\" />\n");
  printf ("  </head>\n\n");

  // retrieve file
  FILE *fp = fopen ("./data/data.txt", "r");

  if (fp == NULL)
    {
      return 0;
    }

  char *line = (char *) calloc (70, sizeof (char));
  char *firstWord = (char *) calloc (70,  sizeof (char));
  char *secondWord = (char *) calloc (70,  sizeof (char));

  char *db = getenv ("db");
  char *record = getenv ("record");
  char *hash = NULL;
  char *query = getenv ("QUERY_STRING");

  // This is an HTML comment. It's useful for debugging to see if your
  // environment variables got through.
  printf ("  <!-- Environment variables:\n");
  printf ("       db: %s\n", db);
  printf ("       record: %s\n", record);
  printf ("       hash: %s\n", hash);
  printf ("       query: %s\n", query);
  printf ("    -->\n\n");

  // TODO [PART]: Read the data/data.txt file and produce the HTML table
  // to match the output in the cgi-src/tests/expected/PART_show_all.txt
  // file. Note that each line in the data.txt file should correspond to
  // two "col" divs, and all but the last line should be followed by a
  // "w-100" div. The overall framework of the body should look like this
  // (with exactly 2 spaces before the opening < character of each line):
  //    <body>
  //      <div class="container">
  //        <br />
  //        <h2 class="mb-0">Database Records</h2>
  //        <div class="row">
  //          <div class="col py-md-2 border bg-light">PART_hello.txt</div>
  //          <div class="col py-md-2 border bg-light">94079f...</div>
  //          <div class="w-100"></div>
  //          <div class="col py-md-2 border bg-light">PART_show_all.txt</div>
  //          <div class="col py-md-2 border bg-light">9e5543...</div>
  //        </div>
  //      </div>
  //    </body>
  printf ("  <body>\n");
  printf ("    <div class=\"container\">\n");
  printf ("      <br />\n");
  printf ("      <h2 class=\"mb-0\">Database Records</h2>\n");
  printf ("      <div class=\"row\">\n");


  if (db != NULL)
    {
      char *buffer = (char *) calloc (40, sizeof(char));
      strncat (buffer, "./data/", 40);
      strncat (buffer, db, 40);
      fp = fopen (buffer, "r");

      if (fp == NULL)
        {
          printf ("lmao retard.");
          free (buffer);
          return 0;
        }
      free (buffer);
    }

  bool a = false;
  while ((fgets (line, 70, fp) != NULL))
    {
      if (a)
        printf ("        <div class=\"w-100\"></div>\n");

      // takes the two words (sperated by a space) in the the file line
      // and prints them
      char *linecpy = strdup (line);
      char *token = strtok (linecpy, " ");
      firstWord = strdup(token);
      token = strtok(NULL, " ");
      secondWord = strdup(token);
      
      // remove the \n from the second word
      secondWord[strcspn(secondWord, "\n")] = 0;
      printf ("        <div class=\"col py-md-2 border bg-light\">%s</div>\n", secondWord);
      printf ("        <div class=\"col py-md-2 border bg-light\">%s</div>\n", firstWord);
      a = true;
    }

  printf ("      </div>\n");
  printf ("    </div>\n");
  printf ("  </body>\n");
  // TODO [MIN]: Once you have the basic structure working, extend this to
  // read in environment variables (db, record, hash, and QUERY_STRING).
  // From a logic standpoint, if QUERY_STRING is set, use that and split it
  // apart at the & character. For example, the QUERY_STRING might look like:
  //   db=foo.txt&record=2&hash=9e5543354d4592db8272b3c3e14953770df88ba3
  // If the QUERY_STRING is not set, look for the db, record, and hash
  // environment variables independently. If the hash variable is set,
  // compare its value with the hash value for the specified record. (If
  // record is not set, then ignore the hash variable.) If the hash does not
  // match, add this code just after the hash value from the database (put
  // a space before the <span and no space between </span></div>):
  //    <span class="badge badge-danger">MISMATCH</span>

  printf ("\n</html>\n");

  return 0;
}
