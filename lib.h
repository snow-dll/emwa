#include <stdio.h>


/*** global vars ***/

// static char *log_file = "/var/log/portage/emerge.log";

static char *cmd = "portageq envvar EMERGE_DIR_LOG";
static char *logname = "emerge.log";


/** TEMPORARY DECLARATION FOR TESTING PURPOSES **/

static char *fulllog = "/var/log/portage/emerge.log";


static int merge_act;
static int merge_tot;
static char *build_flags;

static char *std_tmp_dir = "/tmp";


/*** function prototypes ***/

static void parse_log (char *cmd, char *l_dir, char *l_name,
    int hist_all, char *pkg_name, int verbose, int logvar)
{
  if (logvar == 1)
    // read from cmd
    printf ("logvar = 1\n");


  // read from file, parse logs into global vars
  // merge-act / merge_tot
  

  if (verbose == 1)
    // get str "*** emerge" to fetch build flags
    // parse into var build_flags
    printf ("verbose = 1\n");

  if (hist_all == 1)
    printf ("hist_all = 1\n");
}
