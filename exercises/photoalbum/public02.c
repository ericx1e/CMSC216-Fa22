#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "photoalbum.h"
#include "my_memory_checker_216.h"

int main() {
   Album album;

   start_memory_check(); /* Start memory check */

   initialize_album(&album);
   print_album(&album);
   destroy_album(&album);

   stop_memory_check(); /* End memory check */

   return EXIT_SUCCESS;
}
