/*
 * test.cpp
 *
 *  Created on: 23 Jan 2020
 *      Author: xuanlinhha
 */

#include <iostream>
#include "basic/Ref.h"

class A {
  unsigned refCount;
  A() : refCount(0){};
  ~A(){
	  printf("");
  };
};
int main(int argc, char **argv) {
	Ref<A> a;
}
