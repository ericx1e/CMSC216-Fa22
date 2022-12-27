#include "text_manipulation.h"
#include <stdio.h>
#include <string.h>

int remove_spaces(const char *source, char *result, int *num_spaces_removed) {
  int i = 0;
  int left;
  int right;

  if (!source || strlen(source) == 0 || !result) {
    return FAILURE;
  }

  if (num_spaces_removed) {
    *num_spaces_removed = 0;
  }

  while (source[i] == ' ') {
    if (num_spaces_removed) {
      (*num_spaces_removed)++;
    }
    i++;
  }
  left = i;

  i = strlen(source) - 1;
  while (source[i] == ' ') {
    if (num_spaces_removed) {
      (*num_spaces_removed)++;
    }
    i--;
  }
  right = i;

  for (i = left; i <= right; i++) {
    *result = source[i];
    result++;
    if (!result) {
      return FAILURE;
    }
  }
  *result = '\0';

  return SUCCESS;
}

int center(const char *source, int width, char *result) {
  int i;
  int n;
  int source_length;

  if (!source) {
    return FAILURE;
  }
  source_length = strlen(source);
  if (source_length == 0 || width < source_length || !result) {
    return FAILURE;
  }

  n = (width - source_length) / 2;

  for (i = 0; i < n; i++) {
    *result = ' ';
    result++;
    if (!result) {
      return FAILURE;
    }
  }
  for (i = 0; i < source_length; i++) {
    *result = source[i];
    result++;
    if (!result) {
      return FAILURE;
    }
  }
  for (i = 0; i < n; i++) {
    *result = ' ';
    result++;
    if (!result) {
      return FAILURE;
    }
  }
  *result = '\0';

  return SUCCESS;
}
