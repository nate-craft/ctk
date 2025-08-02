#include "error.h"
#include <stdlib.h>

#include "../io/io.h"

void panic(const StrSlice* msg) {
	println(msg);
	exit(1);
}
