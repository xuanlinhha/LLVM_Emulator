#include <stdio.h>
int gcd(int n1, int n2) {
  int i, g;
  for (i = 1; i <= n1 && i <= n2; ++i) {
    // Checks if i is factor of both integers
    if (n1 % i == 0 && n2 % i == 0)
      g = i;
  }
  return g;
}
int main() {
  int g = gcd(342, 99);
  printf("G.C.D of %d and %d is %d\n", 342, 99, g);
  return 0;
}
