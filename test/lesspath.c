/*
 * lesspath.c
 *
 *  Created on: 11 Sep 2019
 *      Author: xuanlinhha
 */

#include "../src/include/sym.h"
int main() {
  int x = symbolic_int("x");
  int y = symbolic_int("y");
  int z = symbolic_int("z");
  int total = 0;
  if (x > 0) {
    total = total + 1;
  } else {
    total = total + 2;
  }
  if (y > 0) {
    if (z > 1) {
      total = total + 10;
    }
  }
  return 0;
}

/*
#include <klee/klee.h>
int main() {
  int x = klee_make_symbolic(&x, sizeof(int), "x");
  int y = klee_make_symbolic(&y, sizeof(int), "y");
  int z = klee_make_symbolic(&z, sizeof(int), "z");
  int total = 0;
  if (x > 0) {
    total = total + 1;
  } else {
    total = total + 2;
  }
  if (y > 0) {
    if (z > 1) {
      total = total + 10;
    }
  }
  return 0;
}
*/
