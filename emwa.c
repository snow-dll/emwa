#include "lib.h"
#include "reverse.h"
#include <stdio.h>
#include <argp.h>
#include <unistd.h>
#include <stdlib.h>

const char *argp_program_version = "emwa 1.0.0";
const char *argp_program_bug_address = "k3nny.wx@mailfence.com";

static char args_doc[] = "[LOGDIR] / [PKG]";
static char doc[] = "emwa -- [em]erge [wa]tchtower";

struct arguments
{
  int verbose;
  int hist_all;
  char *logdir;
  char *pkg_name;
  int logvar;
  char *outfile;
  int eprefix;
  int unmerge;
};

static struct argp_option options[] = {
  {"verbose", 'v', 0, 0, "Produce verbose output", 0},
  {"hist-pkg", 'p', "PKG", 0, "Print build history for PKG", 0},
  {"hist-all", 'a', 0, 0, "Print full build history", 0},
  {"logdir", 'd', "DIR", 0, "Specify emerge log directory", 0},
  {"logvar", 'l', 0, 0, "Read from EMERGE_DIR_LOG", 0},
  {"outfile", 'o', "FILE", 0, "Write to file instead of standard output", 0},
  {"unmerge", 'u', 0, 0, "Show unmerge history", 0},
  {"eprefix", 'e', 0, 0, "Use the Gentoo Prefix offset", 0},
  {0}
};

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
    case 'o':
      arguments->outfile = arg;
      break;
    case 'e':
      arguments->eprefix = 1;
      break;
    case 'u':
      arguments->unmerge = 1;
      break;
    default:
      return ARGP_ERR_UNKNOWN;
    }

  return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc };

int
main (int argc, char **argv)
{
  struct arguments arguments;

  arguments.verbose = 0;
  arguments.hist_all = 0;
  arguments.logdir = "";
  arguments.pkg_name = "";
  arguments.logvar = 0;
  arguments.outfile = "";
  arguments.eprefix = 0;
  arguments.unmerge = 0;

  argp_parse (&argp, argc, argv, 0, 0, &arguments);

  arg_val (arguments.hist_all, arguments.logdir,arguments.pkg_name,
      arguments.logvar, arguments.outfile, arguments.eprefix);

  if (arguments.outfile[0] != '\0' || arguments.unmerge == 1)
    reverse (l_dir, arguments.verbose, arguments.pkg_name, arguments.hist_all,
        arguments.outfile, arguments.unmerge);

  else if (arguments.pkg_name[0] != '\0' || arguments.hist_all == 1)
  {
    reverse (l_dir, arguments.verbose, arguments.pkg_name, arguments.hist_all,
        arguments.outfile, arguments.unmerge);
  }
  else
    while (1)
    {
      printf ("\x1b[H\x1b[J");

      reverse (l_dir, arguments.verbose, arguments.pkg_name, arguments.hist_all,
          arguments.outfile, arguments.unmerge);
 
      fflush (stdout);
      sleep (1);
    }

return 0;
}
