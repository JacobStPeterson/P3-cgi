#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#define BUFFER_LENGTH 8192

#define CRLF "\r\n"

/* Used to execute a given CGI program in a separate process. Uses the
   version parameter to build the HTTP header. Other parameters are used
   for the FULL implementation only as follows:

     if method is "GET":
       Use query as the value of the QUERY_STRING environment variable.

     if method is "POST":
       Use boundary to split the body of the request (see the project
       page for an example). The size parameter is only used if the POST
       request is for uploading a file, and is the size of the data to
       be uploaded.
 */
char *
cgi_response (char *uri, char *version, char *method, char *query,
              ssize_t size, char *boundary, char *body)
{
  // TODO [PART]: If the URI exists and is executable, run it as a separate
  // process, redirecting its STDOUT back to this process. You can then use
  // that resulting string to determine the Content-Length to send back. As
  // an example, here would be the full response to return for
  // cgi-bin/hello.cgi (assuming HTTP/1.0 and CRLF means "\r\n"; don't print
  // the quotes):
  //   "HTTP/1.0 200 OK" CRLF
  //   "Content-Type: text/html; charset: UTF-8" CRLF
  //   "Content-Length: 95" CRLF
  //   CRLF
  //   "<html>\n"
  //   "<head>\n"
  //   "  <title>Hello world demo</title>\n"
  //   "</head>\n"
  //   "\n"
  //   "<h2>Hello world!</h2>\n"
  //   "</body>\n"
  //   "</html>\n"
  if (uri != NULL && strncmp (method, "GET", strlen(method)) == 0)
    {
      int pipefd[2];
      pipe (pipefd);
      pid_t pid = fork ();

      if (pid == 0)
        {
          //child code
          close (pipefd[0]);
          dup2 (pipefd[1], STDOUT_FILENO);
          close (pipefd[1]);
          char *const argv[] = {NULL};
          execvp (uri, argv);
        }
      // parent code
      close (pipefd[1]);
      char *tmp = (char *)calloc (BUFFER_LENGTH, sizeof (char));
      wait (NULL);

      // format response
      char *buffer = (char *)calloc (BUFFER_LENGTH, sizeof (char));
      strncat (buffer, version, BUFFER_LENGTH);
      strncat (buffer, " 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\nContent-Length: ", BUFFER_LENGTH);
      
      // read message sent by the child process
      if (read (pipefd[0], tmp, BUFFER_LENGTH) <= 0)
        return NULL;

      // add content length to the buffer
      int content_len = strlen (tmp);
      char content_len_str[6];
      snprintf (content_len_str, 6, "%d", content_len);
      strncat (buffer, content_len_str, BUFFER_LENGTH);

      // add "Connection: close" if uri points to shutdown
      if (strstr (uri, "cgi-bin/shutdown.cgi"))
        strncat (buffer, "\r\nConnection: close", BUFFER_LENGTH);

      strncat (buffer, "\r\n\r\n", BUFFER_LENGTH);
      // add the html string retrieved from the child process
      // to the buffer
      strncat (buffer, tmp, BUFFER_LENGTH);
      return buffer;
    }
  // TODO [FULL]: Set the environment variables needed for the CGI programs
  // located in cgi-bin. To do this, you will need to use either execve()
  // or execle() when running the CGI program, using an array of string
  // pairs. For example, the following array would set the db and record
  // environment variables:
  //
  //   char *env[] = { "db=foo.txt", "record=2", NULL };
  //
  // If the request is a GET request, you should only set the QUERY_STRING
  // variable to be the query parameter. For POST requests, you will need
  // to look through the body of the HTTP request, splitting based on the
  // boundary values (see the project description for an example)

  return strdup ("HTTP/1.0 404 Not Found" CRLF CRLF);
}
