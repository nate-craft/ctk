#ifndef CTK_MEMORY_H
#define CTK_MEMORY_H

#include <assert.h>
#include "../core/type.h"

void* heap_one(usize size);
void* heap_many(usize size, usize count);
void* heap_clear(usize size, usize count);
void* heap_renew(void* existing, usize size, usize count);

#define ASSERT_NONNULL_STATIC(value) _Static_assert(value != NULL, "Value cannot be null!")
#define ASSERT_NONNULL(value) assert(value != NULL)

#endif
