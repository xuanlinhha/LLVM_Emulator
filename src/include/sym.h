/*
 * sym.h
 *
 *  Created on: 3 Aug 2019
 *      Author: xuanlinhha
 */

#ifndef SRC_TOOL_SYM_H_
#define SRC_TOOL_SYM_H_

#include <stdbool.h>
// symbolic
char symbolic_char(const char *name);     // i8
int symbolic_int(const char *name);       // i32
float symbolic_float(const char *name);   // float
void *symbolic_pointer(const char *name); // i8*

//
void tool_assume(bool cond);
void tool_assert(bool cond);

#endif /* SRC_TOOL_SYM_H_ */
