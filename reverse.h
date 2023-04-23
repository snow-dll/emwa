#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define INC_LINES 1024
#define INC_CHARS 1024

int
reverse (char *log, int verbose		/* int hist_all,
				   char *pkg_name, int verbose, int logvar */ )
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

  /** temporary, will be reworked in future updates **/

  static char *pat_run = ">>> emerge";
  static char *pat_end = "*** terminating";
  static char delim_start[] = "(";
  static char delim_counter[] = ")";
  static char delim_pkg[] = " ";
  int i = tot_lines - 1;

  if (strstr (lines[i], pat_end) != NULL)
  {
    printf ("emwa - [em]erge [wa]tchtower\n\n");
    printf ("no running build process found.");

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

/*
  if (strstr (lines[i], pat_end) != NULL)
  {
    printf ("no running build process found.\n\n(reading from %s)\n\npress [q] to exit...)", log);

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
          printf ("emerging: %s\n\npackage: %s\n\n(reading from %s)\n\npress [q] to exit..", counter, pkg, log);
        else 
          printf ("emerging: %s\n\npackage: %s\n\npress [q] to exit..", counter, pkg);

        for (size_t cnt = 0; cnt < tot_lines; cnt++)
          free (lines[cnt]);

        free (lines);
        fclose (file);
        
	      break;
      }
    }
  }

*/
  return 0;
}
