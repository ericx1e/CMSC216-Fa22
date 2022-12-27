/* Eric Xie
 * 118862019
 * exie
 * Project 6 */

#include "executor.h"
#include "command.h"
#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sysexits.h>
#include <unistd.h>

#define MAX_LEN 1024
#define FILE_PERMISSIONS 0664

static int execute_aux(struct tree *t, int input_fd, int output_fd);
static void print_tree(struct tree *t);

int execute(struct tree *t) {

  if (t != NULL) {
    return execute_aux(t, -1, -1);
  }

  /* print_tree(t); */

  return 0;
}

int execute_aux(struct tree *t, int input_fd, int output_fd) {
  pid_t pid;
  int status, pipe_fd[2];

  if (t == NULL) {
    return 0;
  }

  if (t->conjunction == NONE) {
    if (strcmp(t->argv[0], "exit") == 0) {
      exit(0);
    } else if (strcmp(t->argv[0], "cd") == 0) {
      chdir(t->argv[1]);
    } else {
      if ((pid = fork()) < 0) {
        perror("Fork error");
        exit(EX_OSERR);
      }

      if (pid == 0) { /* Child process */
        /* If input file */
        if (t->input) {
          /* Opens input file with read only */
          if ((input_fd = open(t->input, O_RDONLY)) < 0) {
            perror("Opening input file error");
            exit(EX_OSERR);
          }
          /* Redirect input to stdin */
          if (dup2(input_fd, STDIN_FILENO) < 0) {
            perror("Input dup2 error");
            exit(EX_OSERR);
          }
          close(input_fd);
        }

        /* If output file */
        if (t->output) {
          /* Creates an output file if it doesn't exists or opens
          the file to writes only with specified file permissions */
          if ((output_fd =
                   open(t->output, O_CREAT | O_WRONLY, FILE_PERMISSIONS)) < 0) {
            perror("Opening output file error");
            exit(EX_OSERR);
          }
          /* Redirect output to stdout */
          if (dup2(output_fd, STDOUT_FILENO) < 0) {
            perror("Output dup2 error\n");
            exit(EX_OSERR);
          }
          close(output_fd);
        }

        /* Execute command */
        execvp(t->argv[0], t->argv);
        /* Code here will only be reached if execvp fails */
        fprintf(stderr, "Failed to execute %s\n", t->argv[0]);
        exit(EXIT_FAILURE);
      } else { /* Parent proc∂ess */
               /* Wait for child and return its status */
        wait(&status);
        return status;
      }
    }
  } else if (t->conjunction == AND) {
    /* Read input and output then execute children */
    if (t->input) {
      if ((input_fd = open(t->input, O_RDONLY) < 0)) {
        perror("Opening input file error");
        exit(EX_OSERR);
      }
    }

    if (t->output) {
      if ((output_fd = open(t->output, O_WRONLY | O_CREAT | O_TRUNC,
                            FILE_PERMISSIONS)) < 0) {
        perror("Opening output file error");
        exit(EX_OSERR);
      }
    }

    /* Execute left and if successful (status 0), then execute right */
    if (execute_aux(t->left, input_fd, output_fd) == 0) {
      execute_aux(t->right, input_fd, output_fd);
    }
  } else if (t->conjunction == SUBSHELL) {
    if (t->input) {
      if ((input_fd = open(t->input, O_RDONLY) < 0)) {
        perror("Opening input file error\n");
        exit(EX_OSERR);
      }
    }
    if (t->output) {
      if ((output_fd = open(t->output, O_WRONLY | O_CREAT | O_TRUNC,
                            FILE_PERMISSIONS)) < 0) {
        perror("Opening output file error");
        exit(EX_OSERR);
      }
    }

    if ((pid = fork()) < 0) {
      perror("Fork error");
    }

    /* Child process becomes the subshell */
    if (pid == 0) {
      /* Recurse on only the left */
      execute_aux(t->left, input_fd, output_fd);
      exit(EXIT_SUCCESS);
    } else {
      wait(NULL);
    }
  } else if (t->conjunction == PIPE) {
    /* a < x | y > b */
    /* Ambiguous output if left child has output redirect */
    /* Ambiguous input input if right child has input redirect */

    if (t->left->output) {
      printf("Ambiguous output redirect.\n");
      return 0;
    }
    if (t->right->input) {
      printf("Ambiguous input redirect.\n");
      return 0;
    }

    if (t->input) {
      if ((input_fd = open(t->input, O_RDONLY) < 0)) {
        perror("Opening input file error\n");
        exit(EX_OSERR);
      }
    }
    if (t->output) {
      if ((output_fd = open(t->output, O_WRONLY | O_CREAT | O_TRUNC,
                            FILE_PERMISSIONS)) < 0) {
        perror("Opening output file error");
        exit(EX_OSERR);
      }
    }

    if (pipe(pipe_fd) < 0) {
      perror("Pipe error");
      exit(EX_OSERR);
    }
    if ((pid = fork()) < 0) {
      perror("Fork error");
      exit(EX_OSERR);
    }

    /* After forking, both parent and need to close the pipe */
    if (pid == 0) {      /* Child */
      close(pipe_fd[0]); /* Close read end */
      /* Redirect write end to stdout */
      if (dup2(pipe_fd[1], STDOUT_FILENO) < 0) {
        perror("Pipe write dup2 error");
        exit(EX_OSERR);
      }
      execute_aux(t->left, input_fd, pipe_fd[1]);
      close(pipe_fd[1]); /* Close write end */
      exit(EXIT_SUCCESS);
    } else {             /* Parent */
      close(pipe_fd[1]); /* Close write end */
      if (dup2(pipe_fd[0], STDIN_FILENO) < 0) {
        perror("Pipe read dup2 error");
        exit(EX_OSERR);
      }
      execute_aux(t->right, pipe_fd[0], output_fd);
      close(pipe_fd[0]); /* Close read end */
      wait(NULL);
    }
  }
  return 0;
}

static void print_tree(struct tree *t) {
  if (t != NULL) {
    print_tree(t->left);

    if (t->conjunction == NONE) {
      printf("NONE: %s, ", t->argv[0]);
    } else {
      printf("%s, ", conj[t->conjunction]);
    }
    printf("IR: %s, ", t->input);
    printf("OR: %s\n", t->output);

    print_tree(t->right);
  }
}
