#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define INC_LINES 1024
#define INC_CHARS 1024

static char *pat_run = ">>> emerge";
static char *pat_end = "*** terminating";
static char *pat_unmerge = "unmerge success";
static char delim_start[] = "(";
static char delim_counter[] = ")";
static char delim_pkg[] = " ";
static char delim_colon[] = ":";
static char delim_nl[] = "\n";
static char *time1;
char buffer2[80];
char *pkg;

void freelines (size_t total, char *lines[], FILE *file)
{
  for (size_t i = 0; i < total; i++)
  free (lines[i]);
  free (lines);
  fclose (file);
}

void timestamp (size_t i, char *pat_time, char delim_1[],
  char delim_2[], char delim_3[], char *lines[])
{
  for (size_t h = i; h > 0; h--)
  {
    if (strstr (lines[h], pat_time) != NULL)
    {
      char *buf1 = strtok (lines[h], delim_1);
      buf1[0] = '\0';
      char *buf2 = strtok (NULL, delim_2);
      buf2[0] = '\0';
      time1 = strtok (NULL, delim_3);
      break;
    }
  }
}

void printer (char *outfile, char *time, char *pkg)
{
  if (outfile[0] != '\0')
  {
    FILE *fp;
    fp = fopen (outfile, "a");
    fprintf (fp, "%s >>> %s\n", time, pkg);
    fclose (fp);
  } else
  {
    printf (" %s >>> %s\n", time, pkg);
  }
}

void epoch (int i, char *lines[])
{
  time1 = strtok (lines[i], delim_colon);
  time_t rawtime = strtoul (time1, NULL, 0);
  struct tm ts;

  ts = *localtime (&rawtime);
  strftime (buffer2, sizeof (buffer2), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
}


int
reverse (char *log, int verbose, char *pkg_name, int hist_all, char *outfile,
    int unmerge)
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
  } while (true);

  lines = realloc (lines, sizeof (char *) * tot_lines);
  size_t i = tot_lines - 1;

  if (unmerge == 1)
  {
    for (i = 0; i < tot_lines - 1; i++)
    {
      if (strstr (lines[i], pat_unmerge) != NULL)
      {
        epoch (i, lines);

        char *buf = strtok (NULL, delim_colon);
        buf[0] = '\0';
        pkg = strtok (NULL, delim_nl);

        printer (outfile, buffer2, pkg);
      }
    }
    freelines (tot_lines, lines, file);
  }

  else if (pkg_name[0] != '\0')
  {
    for (i = 0; i < tot_lines - 1; i++)
    { 
      if (strstr (lines[i], pkg_name) != NULL && strstr (lines[i], pat_run) != NULL)
      {
        epoch (i, lines);

        char *buf3 = strtok (NULL, delim_counter);
        buf3[0] = '\0';
        char *pkg = strtok (NULL, delim_pkg);

        printer (outfile, buffer2, pkg);
      }
    }
    freelines (tot_lines, lines, file);
  }
  else if (hist_all == 1)
  {
    for (i = 0; i < tot_lines; i++)
    {
      if (strstr (lines[i], pat_run) != NULL)
      {
        epoch (i, lines);

        char *buf3 = strtok (NULL, delim_counter);
        buf3[0] = '\0';
        char *pkg = strtok (NULL, delim_pkg);

        printer (outfile, buffer2, pkg);
      }
    }
    freelines (tot_lines, lines, file);
  }
  else if (strstr (lines[i], pat_end) != NULL)
  {
    printf ("emwa - [em]erge [wa]tchtower\n\n");
    printf (" - no running build process found.\n");

    if (verbose == 1)
      printf ("\n - reading from: %s\n", log);

    freelines (tot_lines, lines, file);
  } 
  else
  {
    for (i; i > 0; i--)
    {
      if (strstr (lines[i], pat_run) != NULL)
      {
        
        char *extract_buf = strtok (lines[i], delim_start);
        extract_buf[0] = '\0';
        char *counter = strtok (NULL, delim_counter);
        char *pkg = strtok (NULL, delim_pkg);

        printf ("emwa - [em]erge [wa]tchtower\n\n");
        printf (" - emerging: %s\n\n", counter);
        printf (" - package: %s\n", pkg);

        if (verbose == 1)
          printf ("\n - reading from: %s\n", log);
        break;
      }
    }
    freelines (tot_lines, lines, file);
  }

  return 0;
}
