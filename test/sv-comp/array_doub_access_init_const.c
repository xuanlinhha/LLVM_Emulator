#include "../src/include/sym.h"

int main() {
  int i;
  int N = 10;
  int a[2 * N + 2];

  for (i = 0; i <= N; i++) {
    a[2 * i] = 0;
    a[2 * i + 1] = 0;
  }

  for (i = 0; i <= 2 * N; i++)
    tool_assert(a[i] >= 0);
  return 0;
}
