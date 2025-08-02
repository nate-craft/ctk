#include "memory.h"
#include "../core/error.h"
#include "../string/string.h"
#include <stdlib.h>

void* heap_one(usize size) {
	void* result = malloc(size);
	if (result == NULL) {
		panic(str_static("[CTK ERROR]: Could not allocate memory for function 'heap_one()'"));
	}
	return result;
}

void* heap_many(usize size, usize count) {
	void* result = malloc(size * count);
	if (result == NULL) {
		panic(str_static("[CTK ERROR]: Could not allocate memory for function 'heap_many()'"));
	}
	return result;
}

void* heap_clear(usize size, usize count) {
	void* result = calloc(count, size);
	if (result == NULL) {
		panic(str_static("[CTK ERROR]: Could not allocate memory for function 'heap_clear()'"));
	}
	return result;
}

void* heap_renew(void* existing, usize size, usize count) {
	void* result = realloc(existing, count * size);
	if (result == NULL) {
		panic(str_static("[CTK ERROR]: Could not allocate memory for function 'heap_rewew()'"));
	}
	return result;
}


