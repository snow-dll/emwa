#include "lib.h"
#include "reverse.h"
#include <stdio.h>
#include <argp.h>


const char *argp_program_version = "emwa 1.0.0";
const char *argp_program_bug_address = "k3nny.wx@mailfence.com";

static char args_doc[] = "[LOGDIR] / [PKG]";
static char doc[] = "emwa -- [em]erge [wa]tchtower";


/*** data structs ***/

struct arguments
{
  int verbose;
  int hist_all;
  char *logdir;
  char *pkg_name;
  int logvar;
};


static struct argp_option options[] = {
  {"verbose", 'v', 0, 0, "Produce verbose output", 0},
  {"hist-pkg", 'p', "PKG", 0, "Print build history for PKG", 0},
  {"hist-all", 'a', 0, 0, "Print global build history", 0},
  {"logdir", 'd', "DIR", 0, "Specify emerge log directory", 0},
  {"logvar", 'l', 0, 0, "Read from EMERGE_DIR_LOG", 0},
  {0}
};


/*** parser ***/

static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
  struct arguments *arguments = state->input;

  switch (key)
    {
    case 'v':
      arguments->verbose = 1;
      break;
    case 'p':
      arguments->pkg_name = arg;
      break;
    case 'a':
      arguments->hist_all = 1;
      break;
    case 'd':
      arguments->logdir = arg;
      break;
    case 'l':
      arguments->logvar = 1;
      break;
    default:
      return ARGP_ERR_UNKNOWN;
    }
  return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc };

/*** main ***/

int
main (int argc, char **argv)
{
  struct arguments arguments;

  arguments.verbose = 0;
  arguments.hist_all = 0;
  arguments.logdir = "";
  arguments.pkg_name = "";
  arguments.logvar = 0;

  argp_parse (&argp, argc, argv, 0, 0, &arguments);

  arg_val (arguments.hist_all, arguments.logdir,
	   arguments.pkg_name, arguments.logvar);

  // implement while loop

  reverse (l_dir);

  return 0;
}
