#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

/*** global vars ***/

static char *log = "/var/log/portage/emerge.log";
static char *logname = "emerge.log";
static char *cmd = "portageq envvar EMERGE_DIR_LOG";

static void
usage ()
{
  printf ("usage: test");
}

static int
arg_val (int hist_all, char *logdir, char *pkg_name, int logvar)
{
  if (hist_all == 1 && pkg_name != "")
  {
    usage ();
    return 1;
  } else if (logdir != "" && logvar == 1)
  {
    usage ();
    return 1;
  }
  
  if (logvar == 1)
  {
    if (logdir != "")
    {
      usage ();
      return 1;
    }

    FILE *fp;
    char *cmd = "portageq envvar EMERGE_LOG_DIR";
    char *ch[100];
    char c;

    fp = popen (cmd, "r");

    while ((c = fgetc(fp)) != EOF)
      if (!isspace(c))
      {
        strncat (ch, &c, 1);
      }

    puts (ch); 
    pclose(fp);

    return EXIT_SUCCESS;
  }

  return 0;
}
