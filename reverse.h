#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define INC_LINES 1024
#define INC_CHARS 1024

int
reverse (char *log, int verbose, char *pkg_name, int hist_all)
{
  FILE *file = fopen (log, "r");

  if (file == NULL)
    {
      printf ("error opening file");
      return 1;
    }

  char **lines;
  lines = malloc (sizeof (char *) * INC_CHARS);

  size_t tot_lines = 0;
  size_t tot_chars = 0;
  char c;

  do
    {
      c = fgetc (file);

      if (ferror (file))
	{
	  printf ("error reading from file\n");
	  return 1;
	}

      if (feof (file))
	{
	  if (tot_chars != 0)
	    {
	      lines[tot_lines] = realloc (lines[tot_lines], tot_chars + 1);
	      lines[tot_lines][tot_chars] = '\0';

	      tot_lines++;
	    }

	  break;
	}

      if (tot_chars == 0)
	lines[tot_lines] = malloc (INC_CHARS);

      lines[tot_lines][tot_chars] = c;

      tot_chars++;

      if (c == '\n')
	{
	  lines[tot_lines] = realloc (lines[tot_lines], tot_chars + 1);
	  lines[tot_lines][tot_chars] = '\0';

	  tot_lines++;
	  tot_chars = 0;

	  if (tot_lines % INC_LINES == 0)
	    {
	      size_t new_sz = tot_lines + INC_LINES;
	      lines = realloc (lines, sizeof (char *) * new_sz);
	    }
	}
      else if (tot_chars % INC_CHARS == 0)
	{
	  size_t new_sz = tot_chars + INC_CHARS;
	  lines[tot_lines] = realloc (lines[tot_lines], new_sz);
	}
    }
  while (true);

  lines = realloc (lines, sizeof (char *) * tot_lines);


  /*** ARG DEPENDTEND ***/

  static char *pat_run = ">>> emerge";
  static char *pat_end = "*** terminating";
  static char delim_start[] = "(";
  static char delim_counter[] = ")";
  static char delim_pkg[] = " ";
  static char delim_colon[] = ":";
  static char delim_nl[] = "\n";
  size_t i = tot_lines - 1;

  if (hist_all == 1)
  {
    for (i = 0; i < tot_lines; i++)
    {
      if (strstr (lines[i], pat_run) != NULL)
      {
        char *buf1 = strtok (lines[i - 2], delim_pkg);
        buf1[0] = '\0';
        char *buf2 = strtok (NULL, delim_colon);
        buf2[0] = '\0';
        char *time = strtok (NULL, delim_nl);

        char *buf3 = strtok (lines[i], delim_counter);
        buf3[0] = '\0';
        char *pkg = strtok (NULL, delim_pkg);

        printf ("%s : %s\n", time, pkg);

      }
    }

    for (size_t cnt = 0; cnt < tot_lines; cnt++)
      free (lines[cnt]);

    free (lines);
    fclose (file);
  }

  else if (pkg_name[0] != '\0')
  {
    for (i = 0; i < tot_lines - 1; i++)
    {
      
      if (strstr (lines[i], pkg_name) != NULL && strstr (lines[i], pat_run) != NULL)
      {
        char *buf1 = strtok (lines[i - 2], delim_pkg);
        buf1[0] = '\0';
        char *buf2 = strtok (NULL, delim_colon);
        buf2[0] = '\0';
        char *time = strtok (NULL, delim_nl);

        char *buf3 = strtok (lines[i], delim_counter);
        buf3[0] = '\0';
        char *pkg = strtok (NULL, delim_pkg);

        printf ("%s : %s\n", time, pkg);

      }
    }

    for (size_t cnt = 0; cnt < tot_lines; cnt++)
      free (lines[cnt]);

    free (lines);
    fclose (file);
  }

  else if (strstr (lines[i], pat_end) != NULL)
  {
    printf ("emwa - [em]erge [wa]tchtower\n\n");
    printf ("no running build process found.\n");

    for (size_t cnt = 0; cnt < tot_lines; cnt++)
      free (lines[cnt]);

    free (lines);
    fclose (file);

  } else
  {
    for (i; i > 0; i--)
    {
      if (strstr (lines[i], pat_run) != NULL)
      {
        
        char *extract_buf = strtok (lines[i], delim_start);
        extract_buf[0] = '\0';

        char *counter = strtok (NULL, delim_counter);
        char *pkg = strtok (NULL, delim_pkg);

        if (verbose == 1)
        {
          printf ("emwa - [em]erge [wa]tchtower\n\n");
          printf (" - emerging: %s\n\n", counter);
          printf (" - package: %s\n\n", pkg);
          printf (" - reading from: %s", log);
        } else
        {
          printf ("emwa - [em]erge [wa]tchtower\n\n");
          printf (" - emerging: %s\n\n", counter);
          printf (" - package: %s", pkg);
        }

        break;
      }
    }
    for (size_t cnt = 0; cnt < tot_lines; cnt++)
      free (lines[cnt]);

    free (lines);
    fclose (file);
  }

  return 0;
}
