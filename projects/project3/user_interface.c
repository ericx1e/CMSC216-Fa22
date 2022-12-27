/*
Eric Xie
118862019
exie
Project 3
*/

#include "document.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sysexits.h>

#define MAX_LINE_LEN 1024 /* Valid command lines are up to 1024 characters */

/* Prototypes */
void run_exit(char *line);
void run_print_document(Document *doc, char *line);
void run_add_paragraph_after(Document *doc, char *line);
void run_add_line_after(Document *doc, char *line);
void run_append_line(Document *doc, char *line);
void run_remove_line(Document *doc, char *line);
void run_load_file(Document *doc, char *line);
void run_replace_text(Document *doc, char *line);
void run_highlight_text(Document *doc, char *line);
void run_remove_text(Document *doc, char *line);
void run_save_document(Document *doc, char *line);
void run_reset_document(Document *doc, char *line);

int main(int argc, char *argv[]) {
  char line[MAX_LINE_LEN + 1];
  char command[MAX_LINE_LEN + 1];
  char first_char;
  int n_read;
  FILE *input;

  Document doc;

  init_document(&doc, "main_document");

  if (argc == 1) { /* No filename arg passed, use stdin */
    input = stdin;
  } else if (argc == 2) { /* Use file input */
    input = fopen(argv[1], "r");

    if (input == NULL) {
      fprintf(stderr, "%s cannot be opened.\n", argv[1]);
      exit(EX_OSERR);
    }

  } else { /* Too many arguments */
    fprintf(stderr,
            "Usage: user_interface\nUsage: user_interface <filename>\n");
    exit(EX_USAGE);
  }

  /* If no file is provided and we are using stdin, print the > prompt */
  if (input == stdin) {
    printf("> ");
  }

  /* Reads line until end of input. Exit may be called and exit the program */
  while (fgets(line, MAX_LINE_LEN, input) != NULL) {
    n_read = sscanf(line, " %c", &first_char);
    if (n_read == 1 && !isspace(first_char)) {
      if (first_char !=
          '#') { /* If the first character is a #, ignore the line */
        sscanf(line, " %s", command);
        if (strcmp(command, "quit") == 0 || strcmp(command, "exit") == 0) {
          run_exit(line);
        } else if (strcmp(command, "print_document") == 0) {
          run_print_document(&doc, line);
        } else if (strcmp(command, "add_paragraph_after") == 0) {
          run_add_paragraph_after(&doc, line);
        } else if (strcmp(command, "add_line_after") == 0) {
          run_add_line_after(&doc, line);
        } else if (strcmp(command, "append_line") == 0) {
          run_append_line(&doc, line);
        } else if (strcmp(command, "remove_line") == 0) {
          run_remove_line(&doc, line);
        } else if (strcmp(command, "load_file") == 0) {
          run_load_file(&doc, line);
        } else if (strcmp(command, "replace_text") == 0) {
          run_replace_text(&doc, line);
        } else if (strcmp(command, "highlight_text") == 0) {
          run_highlight_text(&doc, line);
        } else if (strcmp(command, "remove_text") == 0) {
          run_remove_text(&doc, line);
        } else if (strcmp(command, "save_document") == 0) {
          run_save_document(&doc, line);
        } else if (strcmp(command, "reset_document") == 0) {
          run_reset_document(&doc, line);
        } else {
          printf("Invalid Command\n");
        }
      }
    }
    /* If no file is provided and we are using stdin, print > */
    if (input == stdin) {
      printf("> ");
    }
  }

  if (input != stdin) {
    fclose(input);
  }

  return 0;
}

/* Runs when exit or quit is the first string read.
 * Checks there is no data following the command. */
void run_exit(char *line) {
  int n_read;
  char command[MAX_LINE_LEN + 1];
  char extra_inputs[MAX_LINE_LEN + 1]; /* Stores extra inputs */

  n_read = sscanf(line, " %s %s", command, extra_inputs);
  if (n_read != 1) {
    printf("Invalid command\n");
    return;
  }

  /* If exit command is valid, exit the program */
  exit(SUCCESS);
}

/* Checks the command line inputs and runs the
 * add_paragraph_after function if everything is valid */
void run_add_paragraph_after(Document *doc, char *line) {
  int n_read, paragraph_number;
  char command[MAX_LINE_LEN + 1];
  char extra_inputs[MAX_LINE_LEN + 1];

  n_read = sscanf(line, " %s %d %s", command, &paragraph_number, extra_inputs);

  if (n_read != 2 || paragraph_number < 0) {
    printf("Invalid Command\n");
    return;
  }

  if (add_paragraph_after(doc, paragraph_number) == FAILURE) {
    printf("add_paragraph_after failed\n");
  }
}

/* Checks the command line inputs and runs the
 * add_line_after function if everything is valid */
void run_add_line_after(Document *doc, char *line) {
  int n_read, paragraph_number, line_number, i = 0, ptr_index = 0;
  char command[MAX_LINE_LEN + 1];
  char new_line[MAX_STR_SIZE + 1];
  char *line_ptr;

  n_read = sscanf(line, " %s %d %d %s", command, &paragraph_number,
                  &line_number, new_line);
  if (n_read != 4 || paragraph_number <= 0 || line_number < 0 ||
      new_line[0] != '*') {
    printf("Invalid command\n");
    return;
  }
  line_ptr = line;
  /* Increments pointer until * is found */
  while (*line_ptr != '*') {
    line_ptr++;
    /* If * is missing, break out of the loop */
    if (ptr_index++ >= strlen(line)) {
      printf("Invalid command\n");
      return;
    }
  }
  /* Found *, skip over it and start reading the new_line */
  line_ptr++;
  while (*line_ptr != '\n') {
    new_line[i++] = *line_ptr;
    line_ptr++;
    if (ptr_index++ >= strlen(line)) {
      printf("Invalid command\n");
      return;
    }
  }
  new_line[i] = '\0';

  if (add_line_after(doc, paragraph_number, line_number, new_line) == FAILURE) {
    printf("add_line_after failed\n");
  }
}

/* Checks the command line inputs and runs the
 * print_document function if everything is valid */
void run_print_document(Document *doc, char *line) {
  int n_read;
  char command[MAX_LINE_LEN];
  char extra_inputs[MAX_LINE_LEN];

  n_read = sscanf(line, " %s %s", command, extra_inputs);
  if (n_read != 1) {
    printf("Invalid Command\n");
    return;
  }

  print_document(doc);
}

/* Checks the command line inputs and runs the
 * append_line function if everything is valid */
void run_append_line(Document *doc, char *line) {
  int n_read, paragraph_number, i = 0, ptr_index = 0;
  char command[MAX_LINE_LEN];
  char new_line[MAX_STR_SIZE];
  char *line_ptr;

  n_read = sscanf(line, " %s %d %s", command, &paragraph_number, new_line);
  if (n_read != 3 || paragraph_number <= 0 || new_line[0] != '*') {
    printf("Invalid command\n");
    return;
  }
  line_ptr = line;

  while (*line_ptr != '*') {
    line_ptr++;
    if (ptr_index++ >= strlen(line)) {
      printf("Invalid command\n");
      return;
    }
  }
  line_ptr++;
  while (*line_ptr != '\n') {
    new_line[i++] = *line_ptr;
    line_ptr++;
    if (ptr_index++ >= strlen(line)) {
      printf("Invalid command\n");
      return;
    }
  }
  new_line[i] = '\0';

  if (append_line(doc, paragraph_number, new_line) == FAILURE) {
    printf("append_line failed\n");
  }
}

/* Checks the command line inputs and runs the
 * remove_line function if everything is valid */
void run_remove_line(Document *doc, char *line) {
  int n_read, paragraph_number, line_number;
  char command[MAX_LINE_LEN];
  char extra_inputs[MAX_LINE_LEN];

  n_read = sscanf(line, " %s %d %d %s", command, &paragraph_number,
                  &line_number, extra_inputs);

  if (n_read != 3 || paragraph_number <= 0 || line_number <= 0) {
    printf("Invalid Command\n");
    return;
  }

  if (remove_line(doc, paragraph_number, line_number) == FAILURE) {
    printf("remove_line failed\n");
  }
}

/* Checks the command line inputs and runs the
 * load_file function if everything is valid */
void run_load_file(Document *doc, char *line) {
  int n_read;
  char command[MAX_LINE_LEN];
  char file_name[MAX_LINE_LEN];
  char extra_inputs[MAX_LINE_LEN];

  n_read = sscanf(line, " %s %s %s", command, file_name, extra_inputs);
  if (n_read != 2) {
    printf("Invalid Command\n");
    return;
  }

  if (load_file(doc, file_name) == FAILURE) {
    printf("load_file failed\n");
  }
}

/* Checks the command line inputs and runs the
 * replace_text function if everything is valid */
void run_replace_text(Document *doc, char *line) {
  int n_read;
  char command[MAX_LINE_LEN];
  char target[MAX_LINE_LEN];
  char replacement[MAX_LINE_LEN];
  char *line_ptr;
  int i = 0, ptr_index = 0;

  n_read = sscanf(line, " %s %s %s", command, target, replacement);
  if (n_read != 3) {
    printf("Invalid Command\n");
    return;
  }

  line_ptr = line;

  /* Finds the first opening quote */
  while (*line_ptr != '"') {
    line_ptr++;
    /* If the pointer reaches the end of the line without finding another
     * quote, this means quotes are mismatched and the command is invalid */
    if (ptr_index++ >= strlen(line)) {
      printf("Invalid Command\n");
      return;
    }
  }
  line_ptr++; /* Skips the quote */
  i = 0;
  /* Reads data into target until closing quote */
  while (*line_ptr != '"') {
    target[i++] = *line_ptr;
    line_ptr++;
    if (ptr_index++ >= strlen(line)) {
      printf("Invalid Command\n");
      return;
    }
  }
  target[i] = '\0';

  line_ptr++; /* Skips closing quote, looks for next quote */
  i = 0;
  while (*line_ptr != '"') {
    line_ptr++;
    if (ptr_index++ >= strlen(line)) {
      printf("Invalid Command\n");
      return;
    }
  }
  line_ptr++;
  i = 0;
  /* Reads data into replacement until closing quote */
  while (*line_ptr != '"') {
    replacement[i++] = *line_ptr;
    line_ptr++;
    if (ptr_index++ >= strlen(line)) {
      printf("Invalid Command\n");
      return;
    }
  }
  replacement[i] = '\0';

  if (replace_text(doc, target, replacement) == FAILURE) {
    printf("replace_text failed\n");
  }
}

/* Checks the command line inputs and runs the
 * highlight_text function if everything is valid */
void run_highlight_text(Document *doc, char *line) {
  int n_read;
  char command[MAX_LINE_LEN];
  char target[MAX_LINE_LEN];
  char *line_ptr;
  int i = 0, ptr_index = 0;

  n_read = sscanf(line, " %s %s", command, target);
  if (n_read != 2) {
    printf("Invalid Command\n");
    return;
  }

  line_ptr = line;

  /* Finds the first opening quote */
  while (*line_ptr != '"') {
    line_ptr++;
    /* If the pointer reaches the end of the line without finding another
     * quote, this means quotes are mismatched and the command is invalid */
    if (ptr_index++ >= strlen(line)) {
      printf("Invalid Command\n");
      return;
    }
  }
  line_ptr++; /* Skips the quote */
  i = 0;
  /* Reads data into target until closing quote */
  while (*line_ptr != '"') {
    target[i++] = *line_ptr;
    line_ptr++;
    if (ptr_index++ >= strlen(line)) {
      printf("Invalid Command\n");
      return;
    }
  }
  target[i] = '\0';

  /* No failure text; either text is highlighted or not */
  highlight_text(doc, target);
}

/* Checks the command line inputs and runs the
 * remove_text function if everything is valid */
void run_remove_text(Document *doc, char *line) {
  int n_read;
  char command[MAX_LINE_LEN];
  char target[MAX_LINE_LEN];
  char *line_ptr;
  int i = 0, ptr_index = 0;

  n_read = sscanf(line, " %s %s", command, target);
  if (n_read != 2) {
    printf("Invalid Command\n");
    return;
  }

  line_ptr = line;

  /* Finds the first opening quote */
  while (*line_ptr != '"') {
    line_ptr++;
    /* If the pointer reaches the end of the line without finding another
     * quote, this means quotes are mismatched and the command is invalid */
    if (ptr_index++ >= strlen(line)) {
      printf("Invalid Command\n");
      return;
    }
  }
  line_ptr++; /* Skips the quote */
  i = 0;
  /* Reads data into target until closing quote */
  while (*line_ptr != '"') {
    target[i++] = *line_ptr;
    line_ptr++;
    if (ptr_index++ >= strlen(line)) {
      printf("Invalid Command\n");
      return;
    }
  }
  target[i] = '\0';

  /* No failure text, either text is removed or not */
  remove_text(doc, target);
}

/* Checks the command line inputs and runs the
 * save_document function if everything is valid */
void run_save_document(Document *doc, char *line) {
  int n_read;
  char command[MAX_LINE_LEN];
  char file_name[MAX_LINE_LEN];
  char extra_inputs[MAX_LINE_LEN];

  n_read = sscanf(line, " %s %s %s", command, file_name, extra_inputs);
  if (n_read != 2) {
    printf("Invalid Command\n");
    return;
  }

  if (save_document(doc, file_name) == FAILURE) {
    printf("save_document failed\n");
  }
}

/* Checks the command line inputs and runs the
 * reset_document function if everything is valid */
void run_reset_document(Document *doc, char *line) {
  int n_read;
  char command[MAX_LINE_LEN];
  char extra_inputs[MAX_LINE_LEN];

  n_read = sscanf(line, " %s %s", command, extra_inputs);
  if (n_read != 1) {
    printf("Invalid Command\n");
    return;
  }

  /* No failure text */
  reset_document(doc);
}