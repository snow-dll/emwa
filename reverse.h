#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define INC_LINES 1024
#define INC_CHARS 1024

//static char *fulllog = "/var/log/portage/emerge.log";
//static char *logname = "emerge.log";

int
reverse(char *log/*, char *l_dir, char *l_name, int hist_all,
    char *pkg_name, int verbose, int logvar*/) 
{
  FILE * file = fopen(log, "r");

  if (file == NULL) {
    printf("error opening file");
    return 1;
  }

  char ** lines;
  lines = malloc(sizeof(char * ) * INC_CHARS);

  size_t tot_lines = 0;
  size_t tot_chars = 0;
  char c;

  do {
    c = fgetc(file);

    if (ferror(file)) {
      printf("error reading from file\n");
      return 1;
    }

    if (feof(file)) {
      if (tot_chars != 0) {
        lines[tot_lines] = realloc(lines[tot_lines], tot_chars + 1);
        lines[tot_lines][tot_chars] = '\0';

        tot_lines++;
      }

      break;
    }

    if (tot_chars == 0)
      lines[tot_lines] = malloc(INC_CHARS);

    lines[tot_lines][tot_chars] = c;

    tot_chars++;

    if (c == '\n') {
      lines[tot_lines] = realloc(lines[tot_lines], tot_chars + 1);
      lines[tot_lines][tot_chars] = '\0';

      tot_lines++;
      tot_chars = 0;

      if (tot_lines % INC_LINES == 0) {
        size_t new_sz = tot_lines + INC_LINES;
        lines = realloc(lines, sizeof(char * ) * new_sz);
      }
    } else if (tot_chars % INC_CHARS == 0) {
      size_t new_sz = tot_chars + INC_CHARS;
      lines[tot_lines] = realloc(lines[tot_lines], new_sz);
    }
  }
  while (true);

  lines = realloc(lines, sizeof(char * ) * tot_lines);

  static char * pattern = ">>> emerge";

  for (size_t i = tot_lines - 1; i > 0; i--) {
    if (strstr(lines[i], pattern) != NULL)
      printf("%s", lines[i]);
  }

  for (size_t i = 0; i < tot_lines; i++)
    free(lines[i]);

  free(lines);
  fclose(file);

  return 0;
}