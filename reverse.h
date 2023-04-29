#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <zlib.h>

#define INC_LINES 1024
#define INC_CHARS 1024

char **lines;

static char *pat_run = ">>> emerge";
static char *pat_end = "*** terminating";
static char *pat_unmerge = "unmerge success";
static char *pat_start = "Started emerge on:";
static char *pat_success = "::: completed emerge";
static char *merged = ">>>";
static char *unmerged = "<<<";
static char delim_start[] = "(";
static char delim_counter[] = ")";
static char delim_space[] = " ";
static char delim_colon[] = ":";
static char delim_nl[] = "\n";
static char *rawtime_str;
char pkgtime[80];
char *pkgname;

void freelines (size_t total, char *lines[], gzFile *file);
void printer (char *outfile, char *time, char *op, char *pkg);
void epoch (int i, char *lines[]);
void hist_delim (void);










static size_t eta_i = 0;

static char *rawtime_start;
static char *rawtime_end;

static size_t diff = 0;

static size_t start_int = 0;
static size_t end_int = 0;

static size_t h, m, s = 0;


int calc_eta (char *lines[], size_t tot_lines)
{
  for (size_t i = tot_lines - 1; i > eta_i; i--)
  {
    if (strstr (lines[i], pat_success) != NULL && strstr (lines[i], pkgname) != NULL)
    {
      rawtime_end = strtok (lines[i], delim_colon);
      sscanf (rawtime_end, "%zu", &end_int);


      for (size_t h = i; h > 0; h--)
      {
        if (strstr (lines[h], pat_run) != NULL && strstr (lines[h], pkgname) != NULL)
        {
          rawtime_start = strtok (lines[h], delim_colon);
          sscanf (rawtime_start, "%zu", &start_int);

          diff = end_int - start_int;

          h = (diff/3600); 
	        m = (diff -(3600*h))/60;
	        s = (diff -(3600*h)-(m*60));

          return 0;
        }
      }
    } else
    {
      h = 0;
      m = 0;
      s = 0;
    }
  }
}

void freelines (size_t total, char *lines[], gzFile *file)
{
  for (size_t i = 0; i < total; i++)
  free (lines[i]);
  free (lines);
  gzclose (file);
}

void printer (char *outfile, char *time, char *op, char *pkg)
{
  if (outfile[0] != '\0')
  {
    FILE *fp = fopen (outfile, "a");
    fprintf (fp, "%s %s %s\n", time, op, pkg);
    fclose (fp);
  } else
  {
    printf (" %s %s %s\n", time, op, pkg);
  }
}

void epoch (int i, char *lines[])
{
  rawtime_str = strtok (lines[i], delim_colon);
  time_t rawtime = strtoul (rawtime_str, NULL, 0);
  struct tm ts;

  ts = *localtime (&rawtime);
  strftime (pkgtime, sizeof (pkgtime), "%Y-%m-%d %H:%M:%S", &ts);
}

void hist_delim (void)
{
  char *buf3 = strtok (NULL, delim_counter);
  buf3[0] = '\0';
  pkgname = strtok (NULL, delim_space);
}

int
reverse (char *log, int verbose, char *pkg_name, int hist_all, char *outfile,
    int unmerge)
{
  gzFile *file = gzopen (log, "r");

  if (file == NULL)
    {
      printf ("%s", log);
      printf ("!!! Error\nUnable to open file\n");
      return 1;
    }

  lines = malloc (sizeof (char *) * INC_CHARS);

  size_t tot_lines = 0;
  size_t tot_chars = 0;
  char c;

  do
  {
    c = gzgetc ((gzFile) file);

    if (gzeof (file))
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
        pkgname = strtok (NULL, delim_nl);

        printer (outfile, pkgtime, unmerged, pkgname);
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

        hist_delim ();

        printer (outfile, pkgtime, merged, pkgname);
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
        hist_delim ();

        printer (outfile, pkgtime, merged, pkgname);
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
    for (i = tot_lines - 1; i > 0; i--)
    {
      if (strstr (lines[i], pat_run) != NULL)
      { 
        char *extract_buf = strtok (lines[i], delim_start);
        extract_buf[0] = '\0';
        char *counter = strtok (NULL, delim_counter);
        pkgname = strtok (NULL, delim_space);

        calc_eta (lines, tot_lines);

        printf ("emwa - [em]erge [wa]tchtower\n\n");
        printf (" - emerging: %s\n\n", counter);
        printf (" - package: %s\n\n", pkgname);
        if (h == 0 && m == 0 && s == 0)
          printf (" - No ETA available\n");
        else
          printf (" - ETA: %zuh %zum %zus\n", h, m, s);

        if (verbose == 1)
          printf ("\n - reading from: %s\n", log);
        break;
      }
    }
    freelines (tot_lines, lines, file);
  }

  return 0;
}
