#if !defined(TEXTMANIPULATION_H)

#define MAX_STR_LEN 80
#define SUCCESS 0
#define FAILURE -1

int remove_spaces(const char *source, char *result, int *num_spaces_removed);
int center(const char *source, int width, char *result);

#endif
