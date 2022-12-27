#include "photoalbum.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Photo *create_photo(int id, const char *description) {
  Photo photo;
  Photo *photo_ptr;
  char *str;

  photo_ptr = malloc(sizeof(photo));
  if (!photo_ptr) {
    return NULL;
  }

  *photo_ptr = photo;
  photo_ptr->id = id;

  if (description) {
    str = malloc(strlen(description) + 1);
    if (!str) {
      return NULL;
    }

    strcpy(str, description);
    photo_ptr->description = str;
  } else {
    photo_ptr->description = NULL;
  }

  return photo_ptr;
}
void print_photo(Photo *photo) {
  if (photo) {
    if (photo->description) {
      printf("Photo Id: %d, Description: %s\n", photo->id, photo->description);
    } else {
      printf("Photo Id: %d, Description: None\n", photo->id);
    }
  }
}
void destroy_photo(Photo *photo) {
  if (photo) {
    if (photo->description) {
      free(photo->description);
      photo->description = NULL;
    }
    free(photo);
    photo = NULL;
  }
}
void initialize_album(Album *album) {
  if (album) {
    album->size = 0;
  }
}
void print_album(const Album *album) {
  int i = 0;

  if (album) {
    if (album->size == 0) {
      printf("Album has no photos.\n");
    } else {
      for (i = 0; i < album->size; i++) {
        print_photo(album->all_photos[i]);
      }
    }
  }
}
void destroy_album(Album *album) {
  int i = 0;

  for (i = 0; i < album->size; i++) {
    destroy_photo(album->all_photos[i]);
  }
  album->size = 0;
}
void add_photo_to_album(Album *album, int id, const char *description) {
  Photo *new_photo;

  if (album) {
    if (album->size < MAX_ALBUM_SIZE) {
      new_photo = create_photo(id, description);
      if (new_photo) {
        album->all_photos[album->size] = new_photo;
        album->size++;
      }
    }
  }
}