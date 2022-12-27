#include "document.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

/* Checks if a string is all spaces */
int is_blank_line(char *line) {
  int i;

  for (i = 0; i < strlen(line); i++) {
    if (!isspace(line[i])) {
      return 0;
    }
  }

  return 1;
}

/* Initializes an empty (0 paragraphs) document with the given name */
int init_document(Document *doc, const char *name) {
  if (!doc || !name || strlen(name) >= MAX_STR_SIZE) {
    return FAILURE;
  }
  strcpy(doc->name, name);
  doc->number_of_paragraphs = 0;
  return SUCCESS;
}

/* Sets the document's number of paragraphs to 0 */
int reset_document(Document *doc) {
  if (!doc) {
    return FAILURE;
  }
  doc->number_of_paragraphs = 0;
  return SUCCESS;
}

/* Prints the document's name and number of paragraphs,
 * followed by each paragraph separated by a new line */
int print_document(Document *doc) {
  int i;
  int k;
  Paragraph paragraph;

  if (!doc) {
    return FAILURE;
  }
  printf("Document name: \"%s\"\n", doc->name);
  printf("Number of Paragraphs: %d\n", doc->number_of_paragraphs);

  /* Loops over every paragraph, and loop and print every line in each */
  for (i = 0; i < doc->number_of_paragraphs; i++) {
    paragraph = doc->paragraphs[i];
    for (k = 0; k < paragraph.number_of_lines; k++) {
      printf("%s\n", paragraph.lines[k]);
    }
    if (i != doc->number_of_paragraphs - 1) {
      printf("\n");
    }
  }
  return SUCCESS;
}

/* Adds a paragraph to the document after the specified paragraph number */
int add_paragraph_after(Document *doc, int paragraph_number) {
  int i;
  Paragraph new_paragraph;

  /* Initialize a new paragraph to add */
  new_paragraph.number_of_lines = 0;
  if (!doc || doc->number_of_paragraphs >= MAX_PARAGRAPHS ||
      paragraph_number > doc->number_of_paragraphs) {
    return FAILURE;
  }
  /* Shift all paragraphs in the array to the right
   of the insertion point one over, and no data is lost
   after inserting the new paragraph */
  for (i = doc->number_of_paragraphs; i > paragraph_number; i--) {
    doc->paragraphs[i] = doc->paragraphs[i - 1];
  }
  doc->paragraphs[paragraph_number] = new_paragraph;
  doc->number_of_paragraphs++;
  return SUCCESS;
}

/* Adds a line after the line number in a paragraph */
int add_line_after(Document *doc, int paragraph_number, int line_number,
                   const char *new_line) {
  Paragraph *paragraph;
  int i;

  if (!doc || paragraph_number > doc->number_of_paragraphs ||
      doc->paragraphs[paragraph_number - 1].number_of_lines >=
          MAX_PARAGRAPH_LINES ||
      !new_line) {
    return FAILURE;
  }
  paragraph = &doc->paragraphs[paragraph_number - 1];

  /* Shift all lines after the insertion point
   * one to the right then insert the new line. */
  for (i = paragraph->number_of_lines; i > line_number; i--) {
    strcpy(paragraph->lines[line_number - 1] + i,
           paragraph->lines[line_number - 1] + i - 1);
  }

  strcpy(paragraph->lines[line_number], new_line);
  paragraph->number_of_lines++;
  return SUCCESS;
}

/* Returns the number of lines in the specified paragraph number */
int get_number_lines_paragraph(Document *doc, int paragraph_number,
                               int *number_of_lines) {
  if (!doc || paragraph_number > MAX_PARAGRAPHS || !number_of_lines) {
    return FAILURE;
  }
  *number_of_lines = doc->paragraphs[paragraph_number - 1].number_of_lines;
  return SUCCESS;
}

/* Adds a line to the end of a paragraph */
int append_line(Document *doc, int paragraph_number, const char *new_line) {
  int num_lines;

  if (!doc || paragraph_number > doc->number_of_paragraphs ||
      doc->paragraphs[paragraph_number - 1].number_of_lines >=
          MAX_PARAGRAPH_LINES ||
      !new_line) {
    return FAILURE;
  }
  /* Gets the number of lines in the paragraph, then calls
   * add_line_after on the last line, effectively appending a line */
  if (get_number_lines_paragraph(doc, paragraph_number, &num_lines) ==
      FAILURE) {
    return FAILURE;
  }
  return add_line_after(doc, paragraph_number, num_lines, new_line);
}

/* Deletes a line in a paragraph */
int remove_line(Document *doc, int paragraph_number, int line_number) {
  Paragraph *paragraph;
  int i;

  if (!doc || paragraph_number > doc->number_of_paragraphs) {
    return FAILURE;
  }
  paragraph = &doc->paragraphs[paragraph_number - 1];
  if (line_number > paragraph->number_of_lines) {
    return FAILURE;
  }
  for (i = line_number - 1; i < paragraph->number_of_lines - 1; i++) {
    strcpy(paragraph->lines[i], paragraph->lines[i + 1]);
  }
  paragraph->number_of_lines--;
  return SUCCESS;
}

/* Creates a document from an array of strings.
 * An empty string creates a new paragraph */
int load_document(Document *doc, char data[][MAX_STR_SIZE + 1],
                  int data_lines) {
  int i, paragraph_num;

  if (!doc || !data || data_lines <= 0) {
    return FAILURE;
  }
  add_paragraph_after(doc, doc->number_of_paragraphs);
  paragraph_num = doc->number_of_paragraphs;
  for (i = 0; i < data_lines; i++) {
    /* Empty string creates a new paragraph.
    Otherwise, append to the current paragraph */
    if (strlen(data[i]) == 0) {
      add_paragraph_after(doc, paragraph_num);
      paragraph_num++;
    } else {
      append_line(doc, paragraph_num, data[i]);
    }
  }
  return SUCCESS;
}

/* Iterates over a string, replacing any instance of the
 * target with the replacement, updating the line field. */
void replace_text_in_line(char *line, const char *target,
                          const char *replacement) {
  /* i: index for the original line */
  /* k: index for the new line */
  int i, k = 0;
  char result[MAX_STR_SIZE] = {'\0'};

  for (i = 0; i < (int)strlen(line); i++) {
    /* If the current string is target, append replacement,
     * otherwise, append line's current character. */
    if (i <= (int)(strlen(line) - strlen(target)) &&
        strncmp(line + i, target, strlen(target)) == 0) {
      strcpy(result + k, replacement);
      /* Update indices to always point to the end of the string.
       * Need to subtract 1 from i to counteract the for loop. */
      k += strlen(replacement);
      i += strlen(target) - 1;
    } else {
      result[k++] = line[i];
    }
  }
  /* Replace line with the result */
  strcpy(line, result);
}

int replace_text(Document *doc, const char *target, const char *replacement) {
  Paragraph *paragraph;
  int i, j;

  if (!doc || !target || !replacement) {
    return FAILURE;
  }
  /* Iterates over every line of every paragraph
   * calling replace_text_in_line on each */
  for (i = 0; i < doc->number_of_paragraphs; i++) {
    paragraph = &doc->paragraphs[i];
    for (j = 0; j < paragraph->number_of_lines; j++) {
      replace_text_in_line(paragraph->lines[j], target, replacement);
    }
  }
  return SUCCESS;
}
/* Surrounds every instance of target with highlight strings */
int highlight_text(Document *doc, const char *target) {
  char highlight_str[MAX_STR_SIZE] = {'\0'};

  if (!doc || !target) {
    return FAILURE;
  }
  /* Surrounds the target string with the highlight
   * string, then calls replace_text to replace every
   * instance of target with the highlighted version */
  strcat(highlight_str, HIGHLIGHT_START_STR);
  strcat(highlight_str, target);
  strcat(highlight_str, HIGHLIGHT_END_STR);
  return replace_text(doc, target, highlight_str);
}

/* Removes every instance of target from the document */
int remove_text(Document *doc, const char *target) {
  if (!doc || !target) {
    return FAILURE;
  }
  /* Replaces target with an empty string, effectively deleting target */
  return replace_text(doc, target, "");
}

int load_file(Document *doc, const char *filename) {
  FILE *input;
  char line[MAX_STR_SIZE];
  int paragraph_number;

  if (!doc || !filename) {
    return FAILURE;
  }

  input = fopen(filename, "r");
  if (input == NULL) {
    return FAILURE;
  }

  if (add_paragraph_after(doc, 0) == FAILURE) {
    return FAILURE;
  }
  paragraph_number = 1;

  while (fgets(line, MAX_STR_SIZE, input) != NULL) {
    if (is_blank_line(line)) {
      if (add_paragraph_after(doc, paragraph_number) == FAILURE) {
        fclose(input);
        return FAILURE;
      }
      paragraph_number++;
    } else {
      line[strlen(line) - 1] = '\0';
      append_line(doc, paragraph_number, line);
    }
  }
  fclose(input);

  return SUCCESS;
}

int save_document(Document *doc, const char *filename) {
  FILE *output;
  Paragraph paragraph;
  int i, k;

  if (!doc || !filename) {
    return FAILURE;
  }

  output = fopen(filename, "w");
  if (output == NULL) {
    return FAILURE;
  }

  for (i = 0; i < doc->number_of_paragraphs; i++) {
    paragraph = doc->paragraphs[i];
    for (k = 0; k < paragraph.number_of_lines; k++) {
      fprintf(output, "%s\n", paragraph.lines[k]);
    }
    if (i != doc->number_of_paragraphs - 1) {
      fprintf(output, "\n");
    }
  }
  return SUCCESS;
}