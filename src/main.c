#include <stdlib.h>
#include <stdio.h>
#include "parser.h"


int main() {
  int i;

  ir_ts_t ts = lex("const int main auto /* blabla */ <= >= << >> <<= >>= == != || [ |= ? ?? ???", NULL);

  for (i = 0; i < deque_len(&ts); ++i) {
    printf("token: %s\n", str_raw(deque_get(&ts, i).d.string));
  }

  return EXIT_SUCCESS;
}