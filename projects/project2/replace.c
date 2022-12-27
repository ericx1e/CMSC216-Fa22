#include "document.h"
#include <stdio.h>
#include <string.h>

int main() {
  char str[6] = {'a', 'p', 'p', 'l', 'e', '\0'};
  printf("%s\n", &str);
  replace_text_in_line(str, "e", "eee");
  printf("%s\n", &str);
  return 0;
}
