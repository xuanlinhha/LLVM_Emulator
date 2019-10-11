/*
 * array.c
 *
 *  Created on: 2 Sep 2019
 *      Author: xuanlinhha
 */

#include "../src/include/sym.h"
#include <stdio.h>

int main() {
  int arr[2][3] = {{1, 3, 4}, {3, 5, 6}};
  int i = 3, j = 4;
  if (arr[i][j] > 0) {
    printf("i>0\n");
  } else {
    printf("i<=0\n");
  }
  return 0;
}
