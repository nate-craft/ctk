#include "../string/string.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "../core/memory.h"

// MARK: Lifecycle

String* string_new(const c8* cstr_literal) {
    ASSERT_NONNULL(cstr_literal);

    String* string = heap_one(sizeof(String));
    string->length = strlen(cstr_literal);

    if (string->length > 0) {
        string->buffer = heap_many(sizeof(c8), string->length);
        memcpy(string->buffer, cstr_literal, string->length);
    } else {
        string->buffer = heap_many(sizeof(c8), 1);
        string->buffer[0] = '\0';
    }

    return string;
}

CString* cstring_new(const c8* cstr_literal) {
    ASSERT_NONNULL(cstr_literal);

    CString* cstring = heap_one(sizeof(CString));
    cstring->length = strlen(cstr_literal);
    cstring->buffer = heap_many(sizeof(c8), cstring->length + 1);
    memcpy(cstring->buffer, cstr_literal, cstring->length + 1);

    return cstring;
}

StringMut* string_mut_new_sized(usize initial_capacity) {
    StringMut* string_mut = heap_one(sizeof(StringMut));
    string_mut->length = 0;

    if (initial_capacity > 0) {
        string_mut->capacity = initial_capacity;
    } else {
        string_mut->capacity = 1;
    }

    string_mut->buffer = heap_many(sizeof(c8), string_mut->capacity);

    return string_mut;
}

CStringMut* cstring_mut_new_sized(usize initial_capacity) {
    CStringMut* cstring_mut = heap_one(sizeof(CStringMut));
    cstring_mut->length = 0;

    if (initial_capacity > 0) {
        cstring_mut->capacity = initial_capacity;
    } else {
        cstring_mut->capacity = 1;
    }

    cstring_mut->buffer = heap_many(sizeof(c8), cstring_mut->capacity);
    cstring_mut->buffer[0] = '\0';

    return cstring_mut;
}

StringMut* string_mut_new(const c8* cstr_literal) {
    ASSERT_NONNULL(cstr_literal);

    StringMut* string_mut = heap_one(sizeof(StringMut));
    string_mut->length = strlen(cstr_literal);

    if (string_mut->length > 0) {
        string_mut->buffer = heap_many(sizeof(c8), string_mut->length);
        string_mut->capacity = string_mut->length;
        memcpy(string_mut->buffer, cstr_literal, string_mut->length);
    } else {
        string_mut->buffer = heap_many(sizeof(c8), 1);
        string_mut->capacity = 1;
    }

    return string_mut;
}

CStringMut* cstring_mut_new(const c8* cstr_literal) {
    ASSERT_NONNULL(cstr_literal);

    CStringMut* cstring_mut = heap_one(sizeof(CStringMut));
    cstring_mut->length = strlen(cstr_literal);
    cstring_mut->capacity = cstring_mut->length + 1;
    cstring_mut->buffer = heap_many(sizeof(c8), cstring_mut->capacity);
    memcpy(cstring_mut->buffer, cstr_literal, cstring_mut->capacity);

    return cstring_mut;
}

String* string_clone(const String* string) {
    ASSERT_NONNULL(string);

    String* duplicate = heap_one(sizeof(String));
    duplicate->length = string->length;

    if (string->length > 0) {
        duplicate->buffer = heap_many(sizeof(c8), string->length);
        memcpy(duplicate->buffer, string->buffer, string->length);
    } else {
        duplicate->buffer = heap_many(sizeof(c8), 1);
    }

    return duplicate;
}

CString* cstring_clone(const CString* cstring) {
    ASSERT_NONNULL(cstring);

    CString* duplicate = heap_one(sizeof(CString));
    duplicate->length = cstring->length;
    duplicate->buffer = heap_many(sizeof(c8), cstring->length + 1);
    memcpy(duplicate->buffer, cstring->buffer, cstring->length + 1);

    return duplicate;
}

StringMut* string_mut_clone(const StringMut* string_mut) {
    ASSERT_NONNULL(string_mut);

    StringMut* duplicate = heap_one(sizeof(StringMut));
    duplicate->length = string_mut->length;
    duplicate->capacity = string_mut->capacity;

    if (string_mut->capacity > 0) {
        duplicate->buffer = heap_many(sizeof(c8), string_mut->capacity);
        memcpy(duplicate->buffer, string_mut->buffer, string_mut->capacity);
    } else {
        duplicate->buffer = heap_many(sizeof(c8), 1);
        duplicate->buffer[0] = '\0';
    }

    return duplicate;
}

CStringMut* cstring_mut_clone(const CStringMut* cstring_mut) {
    ASSERT_NONNULL(cstring_mut);

    CStringMut* duplicate = heap_one(sizeof(CStringMut));
    duplicate->length = cstring_mut->length;
    duplicate->capacity = cstring_mut->capacity;
    duplicate->buffer = heap_many(sizeof(c8), cstring_mut->capacity);
    memcpy(duplicate->buffer, cstring_mut->buffer, cstring_mut->capacity);
    duplicate->buffer[duplicate->length] = '\0';

    return duplicate;
}

StringMut* str_to_owned_mut(const StrSlice* str) {
    ASSERT_NONNULL(str);

    StringMut* string_mut = heap_one(sizeof(StringMut));
    string_mut->length = str->length;

    if (string_mut->length > 0) {
        string_mut->buffer = heap_many(sizeof(c8), string_mut->length);
        string_mut->capacity = string_mut->length;
        memcpy(string_mut->buffer, str->buffer, string_mut->length);
    } else {
        string_mut->buffer = heap_many(sizeof(c8), 1);
        string_mut->capacity = 1;
        string_mut->buffer[0] = '\0';
    }

    return string_mut;
}

CStringMut* cstr_to_owned_mut(const CStrSlice* cstr) {
    ASSERT_NONNULL(cstr);

    CStringMut* cstring_mut = heap_one(sizeof(CStringMut));
    cstring_mut->length = cstr->length;
    cstring_mut->capacity = cstring_mut->length + 1;
    cstring_mut->buffer = heap_many(sizeof(c8), cstring_mut->capacity);
    memcpy(cstring_mut->buffer, cstr->buffer, cstring_mut->capacity);

    return cstring_mut;
}

void string_free(String** string) {
    ASSERT_NONNULL(string);
    ASSERT_NONNULL(*string);

    free((*string)->buffer);
    free(*string);
    *string = NULL;
}

void cstring_free(CString** cstring) {
    ASSERT_NONNULL(cstring);
    ASSERT_NONNULL(*cstring);

    free((*cstring)->buffer);
    free(*cstring);
    *cstring = NULL;
}

void string_mut_free(StringMut** string_mut) {
    ASSERT_NONNULL(string_mut);
    ASSERT_NONNULL(*string_mut);

    free((*string_mut)->buffer);
    free(*string_mut);
    *string_mut = NULL;
}

void cstring_mut_free(CStringMut** cstring_mut) {
    ASSERT_NONNULL(cstring_mut);
    ASSERT_NONNULL(*cstring_mut);

    free((*cstring_mut)->buffer);
    free(*cstring_mut);
    *cstring_mut = NULL;
}

Str str_init(const c8* cstr_literal) {
    ASSERT_NONNULL(cstr_literal);

    return (Str) {.buffer = cstr_literal, .length = strlen(cstr_literal)};
}

CStr cstr_init(const c8* cstr_literal) {
    ASSERT_NONNULL(cstr_literal);

    return (CStr) {.buffer = cstr_literal, .length = strlen(cstr_literal) + 1};
}

// MARK: Comparison

bool str_equals(const StrSlice* one, const StrSlice* two) {
    ASSERT_NONNULL(one);
    ASSERT_NONNULL(two);

    if (one->length != two->length) {
        return false;
    }

    return memcmp(one->buffer, two->buffer, one->length) == 0;
}

i32 str_compare(const StrSlice* one, const StrSlice* two) {
    ASSERT_NONNULL(one);
    ASSERT_NONNULL(two);

    usize length = one->length < two->length ? one->length : two->length;
    i32 compare = memcmp(one->buffer, two->buffer, length);

    if (compare != 0 || one->length == two->length) {
        return compare;
    }

    if (one->length > two->length) {
        return 1;
    }

    return -1;
}

// MARK: Query

String* str_replace(const StrSlice* str, const StrSlice* query, const StrSlice* replacement) {
    ASSERT_NONNULL(str);
    ASSERT_NONNULL(query);
    ASSERT_NONNULL(replacement);

    StringMut* string_mut = string_mut_new_sized(str->length);

    for (usize i = 0; i < str->length; i++) {
        for (usize j = 0; j < query->length; j++) {
            if (str->buffer[i + j] != query->buffer[j]) {
                string_mut_push_char(string_mut, str->buffer[i]);
                break;
            }

            if (j >= query->length - 1) {
                (void) replacement;
                string_mut_push(string_mut, replacement);
                i += query->length - 1;
            }
        }
    }

    String* moved = heap_one(sizeof(String));
    moved->length = string_mut->length;
    moved->buffer = string_mut->buffer;
    free(string_mut);

    return moved;
}

CString* cstr_replace(const CStrSlice* cstr, const CStrSlice* query, const CStrSlice* replacement) {
    ASSERT_NONNULL(cstr);
    ASSERT_NONNULL(query);
    ASSERT_NONNULL(replacement);

    CStringMut* cstring_mut = cstring_mut_new_sized(cstr->length);

    for (usize i = 0; i < cstr->length; i++) {
        for (usize j = 0; j < query->length; j++) {
            if (cstr->buffer[i + j] != query->buffer[j]) {
                cstring_mut_push_char(cstring_mut, cstr->buffer[i]);
                break;
            }

            if (j >= query->length - 1) {
                (void) replacement;
                cstring_mut_push(cstring_mut, replacement);
                i += query->length - 1;
            }
        }
    }

    cstring_mut->buffer[cstring_mut->length] = '\0';

    CString* moved = heap_one(sizeof(CString));
    moved->length = cstring_mut->length;
    moved->buffer = cstring_mut->buffer;
    free(cstring_mut);

    return moved;
}

StrSlice str_sub_slice(const StrSlice* str, usize start, usize length) {
    ASSERT_NONNULL(str);
    assert(start + length <= length);

    return (StrSlice) {
        str->buffer + start,
        length};
}

StrSlices* str_split_slices(const StrSlice* str, c8 delimiter) {
    ASSERT_NONNULL(str);

    usize capacity = 2;
    StrSlices* slices = heap_one(sizeof(StrSlices));
    slices->slices = heap_many(sizeof(StrSlice), capacity);

    if (str->length == 0) {
        slices->slices[0] = (StrSlice) {str->buffer, (usize) str->length};
        slices->count = 1;
        return slices;
    }

    usize slice_index = 0;
    usize current = 0;

    for (usize i = 0; i < str->length + 1; i++) {
        if (i < str->length && str->buffer[i] != delimiter) {
            continue;
        }
        if (i > 0 && i - current > 0) {
            if (slice_index > capacity - 1) {
                slices->slices = heap_renew(slices->slices, sizeof(StrSlice), capacity * 2);
                capacity *= 2;
            }
            slices->slices[slice_index] = (StrSlice) {str->buffer + current, i - current};
            slice_index++;
        }
        current = i + 1;
    }

    slices->count = slice_index;

    return slices;
}

void str_slices_free(StrSlices** slices) {
    ASSERT_NONNULL(slices);
    ASSERT_NONNULL(*slices);

    free((StrSlices*) (*slices)->slices);
    free(*slices);
    *slices = NULL;
}

bool str_contains(const StrSlice* str, const StrSlice* query) {
    ASSERT_NONNULL(str);
    ASSERT_NONNULL(query);

    if (str->length < query->length || query->length == 0) {
        return false;
    }

    for (usize i = 0; i < str->length - query->length + 1; i++) {
        for (usize j = 0; j < query->length; j++) {
            if (str->buffer[i + j] != query->buffer[j]) {
                break;
            }
            if (j == query->length - 1) {
                return true;
            }
        }
    }

    return false;
}

bool str_contains_char(const StrSlice* str, c8 query) {
    ASSERT_NONNULL(str);

    for (usize i = 0; i < str->length; i++) {
        if (str->buffer[i] == query) {
            return true;
        }
    }

    return false;
}

OptionIndex str_find(const StrSlice* str, c8 query) {
    ASSERT_NONNULL(str);

    for (usize i = 0; i < str->length; i++) {
        if (str->buffer[i] == query) {
            return option_index(i);
        }
    }

    return option_index_empty();
}

OptionIndex str_find_last(const StrSlice* str, c8 query) {
    ASSERT_NONNULL(str);

    for (i32 i = (i32) str->length - 1; i >= 0; i--) {
        if (str->buffer[i] == query) {
            return option_index((usize) i);
        }
    }

    return option_index_empty();
}

OptionIndex str_find_nth(const StrSlice* str, c8 query, usize n) {
    ASSERT_NONNULL(str);
    usize current = 0;

    for (usize i = 0; i < str->length; i++) {
        if (str->buffer[i] == query) {
            if (++current >= n) {
                return option_index(i);
            }
        }
    }

    return option_index_empty();
}

OptionIndex str_find_last_nth(const StrSlice* str, c8 query, usize n) {
    ASSERT_NONNULL(str);
    usize current = 0;

    for (i32 i = (i32) str->length - 1; i >= 0; i--) {
        if (str->buffer[i] == query) {
            if (++current >= n) {
                return option_index((usize) i);
            }
        }
    }

    return option_index_empty();
}

// MARK: Mutation

void string_mut_replace(StringMut* string_mut, const StrSlice* query, const StrSlice* replacer) {
    ASSERT_NONNULL(string_mut);
    ASSERT_NONNULL(query);
    ASSERT_NONNULL(replacer);

    if (query->length <= 0 || string_mut->length < query->length || string_mut->length == 0 || query->length == 0) {
        return;
    }

    StringMut* new_string = string_mut_new_sized(string_mut->length);

    for (usize i = 0; i < string_mut->length; i++) {
        usize last_match = 0;

        for (usize j = 0; j < query->length; j++) {
            if (i + j >= string_mut->length || (string_mut->buffer[i + j] != query->buffer[j])) {
                // no match found at index -> push existing string segment
                usize added_index = 0;
                do {
                    string_mut_push_char(new_string, string_mut->buffer[i + added_index]);
                    added_index++;
                } while (added_index < last_match);

                break;
            }

            if (j >= query->length - 1) {
                // full match -> push replacer
                string_mut_push(new_string, replacer);
                i += query->length - 1;
            }

            // character matched -> not necessarily full match
            last_match++;
        }
    }

    // Put new onto old
    free(string_mut->buffer);
    string_mut->buffer = new_string->buffer;
    string_mut->length = new_string->length;
    string_mut->capacity = new_string->capacity;
    free(new_string);
}

void string_mut_replace_char(StringMut* string_mut, c8 query, c8 replacement) {
    for (usize i = 0; i < string_mut->length; i++) {
        if (string_mut->buffer[i] == query) {
            string_mut->buffer[i] = replacement;
        }
    }
}

void cstring_mut_replace_char(CStringMut* cstring_mut, c8 query, c8 replacement) {
    for (usize i = 0; i < cstring_mut->length; i++) {
        if (cstring_mut->buffer[i] == query) {
            cstring_mut->buffer[i] = replacement;
        }
    }
}

void string_mut_push(StringMut* string, const StrSlice* added) {
    ASSERT_NONNULL(string);
    ASSERT_NONNULL(added);

    if (added->length == 0) {
        return;
    }

    if (string->capacity < (string->length + added->length)) {
        if ((string->capacity * 2) < (string->capacity + added->length)) {
            string->capacity = string->capacity + added->length;
        } else {
            string->capacity *= 2;
        }

        string->buffer = heap_renew(string->buffer, sizeof(c8), string->capacity);
    }

    memcpy(string->buffer + string->length, added->buffer, added->length);
    string->length += added->length;
}

void cstring_mut_push(CStringMut* cstring, const CStrSlice* added) {
    ASSERT_NONNULL(cstring);
    ASSERT_NONNULL(added);

    if (added->length == 0) {
        return;
    }

    if (cstring->capacity < (cstring->length + added->length + 1)) {
        if ((cstring->capacity * 2) < (cstring->capacity + added->length)) {
            cstring->capacity = cstring->capacity + added->length;
        } else {
            cstring->capacity *= 2;
        }

        cstring->buffer = heap_renew(cstring->buffer, sizeof(c8), cstring->capacity);
    }

    memcpy(cstring->buffer + cstring->length, added->buffer, added->length);
    cstring->length += added->length;
    cstring->buffer[cstring->length] = '\0';
}

void string_mut_push_char(StringMut* string_mut, c8 added) {
    ASSERT_NONNULL(string_mut);

    if (string_mut->capacity < string_mut->length + 2) {
        string_mut->capacity *= 2;
        string_mut->buffer = heap_renew(string_mut->buffer, sizeof(c8), string_mut->capacity);
    }

    string_mut->buffer[string_mut->length] = added;
    string_mut->length++;
}

void cstring_mut_push_char(CStringMut* cstring_mut, c8 added) {
    ASSERT_NONNULL(cstring_mut);

    if (cstring_mut->capacity < cstring_mut->length + 2) {
        cstring_mut->capacity *= 2;
        cstring_mut->buffer = heap_renew(cstring_mut->buffer, sizeof(c8), cstring_mut->capacity);
    }

    cstring_mut->buffer[cstring_mut->length] = added;
    cstring_mut->length++;
    cstring_mut->buffer[cstring_mut->length] = '\0';
}

void string_mut_clear(StringMut* string_mut) {
    string_mut->length = 0;
}

void cstring_mut_clear(CStringMut* cstring_mut) {
    cstring_mut->length = 0;
    cstring_mut->buffer[0] = '\0';
}

void string_mut_clear_to(StringMut* string_mut, c8 value) {
    string_mut->length = 0;
    memset(string_mut->buffer, value, string_mut->capacity * sizeof(c8));
}

void cstring_mut_clear_to(CStringMut* cstring_mut, c8 value) {
    cstring_mut->length = 0;
    memset(cstring_mut->buffer, value, (cstring_mut->capacity - 1) * sizeof(c8));
    cstring_mut->buffer[cstring_mut->capacity - 1] = '\0';
}
