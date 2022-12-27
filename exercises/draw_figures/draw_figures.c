/*
Eric Xie
ID: 118862019
CMSC216 E1
Section 0203
*/

#include <stdio.h>

/*Prototypes*/
int draw_rectangle(char character, int width, int length);
int draw_triangle(char character, int size);
int valid_character(char character);

int main() {
  /*Variables*/
  char input = 'a'; /*initalize with char, updated later with scanf*/
  char input_character = 'a';
  int input_width = 0; /*used as size for triangle*/
  int input_length = 0;
  int quit = 0; /*Set to 1 when the user enters 0, keeps the loop running*/

  /*Main loop to keep requesting input until the quit command*/
  while (!quit) {
    printf("Enter 1(rectangle), 2(triangle), 3(other), 0(quit): ");
    scanf(" %c", &input);
    
    if (input == '1') { /*Rectangle*/
      printf("Enter character, width and length: ");
      scanf(" %c %d %d", &input_character, &input_width, &input_length);

      if (!draw_rectangle(input_character, input_width, input_length)) {
	  printf("Invalid data provided.\n");
      }
    } else if (input == '2') { /*Triangle*/
      printf("Enter character and size: ");
      scanf(" %c %d", &input_character, &input_width);

      if (!draw_triangle(input_character, input_width)) {
	  printf("Invalid data provided.\n");
      }
    } else if (input == '3') {
    } else if (input == '0') {
      /*Ends loop next iteration*/
      quit = 1;
    } else {
      printf("Invalid choice.\n");
    }
  }

  printf("Bye Bye.\n");
  return 0;
}

/*Returns 1 if character is *, %, or #; otherwise return 0*/
int valid_character(char character) {
  return character == '*' || character == '%' || character == '#';
}

/*Attempts to print a rectangle with the given inputs */
/*Returns 1 if successful and 0 if an input is invalid*/
int draw_rectangle(char character, int width, int length) {
  int i = 0; /*initialize i and j for loop*/
  int j = 0;

  if (!valid_character(character) || width <= 0 || length <= 0) {
    return 0;
  }
  
  for (i = 0; i < width; i++) {
    for (j = 0; j < length; j++) {
      printf("%c", character);
    }
    printf("\n");
  }

  return 1;
}

/*Same as draw_rect but attempts to draw triangle instead*/
int draw_triangle(char character, int size) {
  int i = 1;
  int j = 0;

  if (!valid_character(character) || size <= 0) {
    return 0;
  }

  for (i = 1; i <= size; i++) {
    for (j = 0; j < 2*i-1; j++) {
      printf("%c", character);
    }
    printf("\n");
  }

  return 1;
}
