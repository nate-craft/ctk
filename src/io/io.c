#include "io.h"
#include <stdarg.h>
#include <stdio.h>

void println(const StrSlice* msg) {
	fwrite(msg->buffer, 1, msg->length, stdout);
	putchar('\n');
}

void print(const StrSlice* msg) {
	fwrite(msg->buffer, 1, msg->length, stdout);
}

