#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*** global vars ***/

static char *log = "/var/log/portage/emerge.log";
static char *logname = "/emerge.log";
static char *cmd = "portageq envvar EMERGE_LOG_DIR";
static char l_dir[100];

static void
usage ()
{
  printf ("usage: test\n");
}

static void
graphic ()
{
  printf ("\x1b[1;H\x1b[2J");
  printf ("emwa - [em]erge [wa]tchtower\n\n");
}

static int
arg_val (int hist_all, char *logdir, char *pkg_name, int logvar)
{
  if (hist_all == 1 && pkg_name[0] != '\0')
    {
      usage ();
      exit (1);
    }
  else if (logvar == 1 && logdir[0] != '\0')
    {
      usage ();
      exit (1);
    }

  if (logvar == 1)
    {
      FILE *fp;
      char c;
      fp = popen (cmd, "r");

      while ((c = fgetc (fp)) != EOF)
	if (!isspace (c))
	  {
	    strncat (l_dir, &c, 1);
	  }

      int sz = strlen (logname);
      strncat (l_dir, logname, sz);

      pclose (fp);
      return EXIT_SUCCESS;
    }
  else
    {
      int sz = strlen (log);
      strncpy (l_dir, log, sz);
    }

  return 0;
}
