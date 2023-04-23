#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>

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

void
changemode (int dir)
{
  static struct termios oldt, newt;

  if (dir == 1)
    {
      tcgetattr (STDIN_FILENO, &oldt);
      newt = oldt;
      newt.c_lflag &= ~(ICANON | ECHO);
      tcsetattr (STDIN_FILENO, TCSANOW, &newt);
    }
  else
    tcsetattr (STDIN_FILENO, TCSANOW, &oldt);
}

int
kbhit (void)
{
  struct timeval tv;
  fd_set rdfs;

  tv.tv_sec = 0;
  tv.tv_usec = 0;

  FD_ZERO (&rdfs);
  FD_SET (STDIN_FILENO, &rdfs);

  select (STDIN_FILENO + 1, &rdfs, NULL, NULL, &tv);
  return FD_ISSET (STDIN_FILENO, &rdfs);

}
