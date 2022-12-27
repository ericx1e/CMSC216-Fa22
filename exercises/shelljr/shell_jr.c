#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sysexits.h>
#include <unistd.h>

#define MAX_LEN 1024

int main() {
  char line[MAX_LEN + 1];
  char command[MAX_LEN + 1];
  char input[MAX_LEN + 1];
  char *argv[3];
  pid_t pid;

  printf("shell_jr: ");
  fflush(stdout);
  while (fgets(line, MAX_LEN, stdin)) {
    sscanf(line, " %s %s", command, input);
    if (strcmp(command, "exit") == 0 || strcmp(command, "hastalavista") == 0) {
      printf("See you\n");
      exit(EXIT_SUCCESS);
    } else {

      argv[0] = command;
      argv[1] = input;
      argv[2] = NULL;

      if (strcmp(command, "cd") == 0) {
        if (chdir(input) == -1) {
          err(EX_OSERR, "Cannot change to directory %s", input);
          fflush(stdout);
        }
      } else {
        pid = fork();
        if (pid == 0) {
          fflush(stdout);
          execvp(command, argv);
          printf("Failed to execute %s\n", command);
          exit(EX_OSERR);
        } else {
          wait(NULL);
        }
      }
    }
    printf("shell_jr: ");
    fflush(stdout);
  }
  return 0;
}