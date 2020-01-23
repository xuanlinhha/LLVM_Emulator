#include "../src/include/sym.h"

int main() {
  int i;
  int j = symbolic_int("j");
  int n = symbolic_int("n");
  tool_assume(n < 100000);
  int a[n];

  tool_assume(j > 0 && j < 10000);

  for (i = 1; i < n; i++) {
    int k = symbolic_int("k");
    tool_assume(k > 0 && k < 10000);
    a[i] = i + j + k;
  }

  for (i = 1; i < n; i++)
    tool_assert(a[i] >= (i + 2));
  return 0;
}
