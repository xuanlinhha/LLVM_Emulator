/*
 * sym.h
 *
 *  Created on: 3 Aug 2019
 *      Author: xuanlinhha
 */

#ifndef SRC_TOOL_SYM_H_
#define SRC_TOOL_SYM_H_

#include <stdbool.h>

char symbolic_char(const char *name);     // i8
int symbolic_int(const char *name);       // i32
float symbolic_float(const char *name);   // float
void *symbolic_pointer(const char *name); // i8*

#endif /* SRC_TOOL_SYM_H_ */
