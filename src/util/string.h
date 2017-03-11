#include <string.h>
#include <malloc.h>

#if !defined(__GNUC__)
char * strndup (const char *s, size_t n) {
  char *result;
  size_t len = strnlen (s, n);

  result = (char *) malloc (len + 1);
  if (!result)
    return 0;

  result[len] = '\0';
  return (char *) memcpy (result, s, len);
}
#endif
