/*** global vars ***/

static char *cmd = "portageq envvar EMERGE_DIR_LOG";

/*** function prototypes ***/

static void parse_log (char *cmd, char *l_dir, char *l_name,
    int hist_all, char *pkg_name, int verbose);
