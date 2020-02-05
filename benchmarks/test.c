#include "../src/include/sym.h"

int N = 100000;
int main() {
  int a[N];
  int b[N];
  int i;
  int sum = 0;

  for (i = 0; i < N; i++) {
    a[i] = i;
  }

  for (i = 0; i < N; i++) {
    b[N - i - 1] = i;
  }

  for (i = 0; i < N; i++) {
    sum = sum + (a[i] - b[N - i - 1]);
  }

  tool_assert(sum == 0);
  return 0;
}
