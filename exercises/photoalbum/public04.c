#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "photoalbum.h"
#include "my_memory_checker_216.h"

int main() {
   Album album, second_album;
   
   start_memory_check(); /* Start memory check */

   initialize_album(&album);
   add_photo_to_album(&album, -1, "Computer"); 
   add_photo_to_album(&album, 40, "Car"); 
   add_photo_to_album(&album, 8, "TV"); 
   print_album(&album);
   destroy_album(&album);

   initialize_album(&second_album);
   add_photo_to_album(&second_album, 100, "Shirt"); 
   add_photo_to_album(&second_album, 200, "Pants"); 
   add_photo_to_album(&second_album, 300, "Ties"); 
   print_album(&second_album);
   destroy_album(&second_album);

   stop_memory_check(); /* End memory check */

   return EXIT_SUCCESS;
}
