#include "../src/include/sym.h"
#include <stdio.h>

int main() {
  int i;
  symbolic_int("i");
  if (i > 0) {
    printf("i>0\n");
  } else {
    printf("i<=0\n");
  }
  return 0;
}
