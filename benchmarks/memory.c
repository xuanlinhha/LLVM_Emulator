/*
 * memory.c
 *
 *  Created on: 1 Sep 2019
 *      Author: xuanlinhha
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  char *str;

  /* Initial memory allocation */
  str = (char *)malloc(15);
  strcpy(str, "tutorialspoint");
  printf("String = %s,  Address = %u\n", str, str);

  /* Reallocating memory */
  str = (char *)realloc(str, 25);
  strcat(str, ".com");
  printf("String = %s,  Address = %u\n", str, str);

  /* Deallocate allocated memory */
  free(str);

  return (0);
}
