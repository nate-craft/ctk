#ifndef CTK_STRING_H
#define CTK_STRING_H

#include "../core/memory.h"
#include "../core/type.h"

// MARK: Definition

/**
 * @brief Heap allocated, mutable, non-null terminated string
 * @note Shares duplicate struct definition with CStringMut, but does not store null terminator in buffer
 */
typedef struct {
    c8* buffer;
    usize length;
    usize capacity;
} StringMut;

/**
 * @brief Heap allocated, mutable, null terminated string
 * @note Shares duplicate struct definition with StringMut, but stores null terminator in buffer
 */
typedef struct {
    c8* buffer;
    usize length;
    usize capacity;
} CStringMut;

/**
 * @brief Heap allocated, non-null terminated, immutable string
 * @note Shares duplicate struct definition with CString, but does not store null terminator in buffer
 * @note Despite being immutable, String is not constant as it must be freed
 */
typedef struct {
    c8* buffer;
    usize length;
} String;

/**
 * @brief Heap allocated, immutable, null terminated string
 * @note Shares duplicate struct definition with String, but stores null terminator in buffer
 * @note Despite being immutable, CString is not constant as it must be freed
 */
typedef struct {
    c8* buffer;
    usize length;
} CString;

/**
 * @brief Stack allocated, immutable, non-null terminated string
 * @note Shares duplicate struct definition with String, and keeps same buffer layout, but is stack allocated
 */
typedef struct {
    const c8* buffer;
    usize length;
} Str;

/**
 * @brief Stack allocated, immutable, null terminated string
 * @note Shares duplicate struct definition with CString, and keeps same buffer layout, but is stack allocated
 */
typedef struct {
    const c8* buffer;
    usize length;
} CStr;

/**
 * @brief Stack allocated, immutable, non-null terminated slice to existing stack allocated string
 * @note aliases with Str as a slice is defined as a pointer to a place in a string, plus some length
 */
typedef Str StrSlice;

/**
 * @brief Stack allocated, immutable, null terminated slice to existing stack allocated cstring
 * @note aliases with CStr as a slice is defined as a pointer to a place in a cstring, plus some length
 */
typedef CStr CStrSlice;

/**
 * @brief Heap allocated struct with reference to existing slices of an existing String or CString
 * @note because this refrences an existing string/cstring, the lifetime of these slices is equal to the lifetime of the original
 * @note CString reference splitting returns StrSlices as it does not guarantee null termination in sub slices
 */
typedef struct {
    StrSlice* slices;
    usize count;
} StrSlices;

// MARK: Preprocessor Type Defines

DEFINE_OPTION(usize, Index, index, 0)

// MARK: Lifecycle

/**
 * @param cstr_literal as a null terminated raw c string
 * @return heap allocated owned String pointer
 * @note must be freed
 */
String* string_new(const c8* cstr_literal)
    __attribute__((warn_unused_result))
    __attribute__((nonnull(1)));

/**
 * @brief performs a deep copy of the given string
 */
String* string_clone(const String* string)
    __attribute__((warn_unused_result))
    __attribute__((nonnull(1)));

/**
 * @brief deep free of String* and sets pointer to NULL
 */
void string_free(String** string) __attribute__((nonnull(1)));

/**
 * @param cstr_literal as a null terminated raw c string
 * @return heap allocated owned CString pointer
 * @note must be freed
 */
CString* cstring_new(const c8* cstr_literal)
    __attribute__((warn_unused_result))
    __attribute__((nonnull(1)));

/**
 * @brief performs a deep copy of the given cstring
 */
CString* cstring_clone(const CString* string)
    __attribute__((warn_unused_result))
    __attribute__((nonnull(1)));

/**
 * @brief deep free of CString* and sets pointer to NULL
 */
void cstring_free(CString** cstring) __attribute__((nonnull(1)));

/**
 * @param cstr_literal as a null terminated raw c string
 * @return heap allocated, mutable, owned StringMut pointer
 * @note must be freed
 */
StringMut* string_mut_new(const c8* cstr_literal)
    __attribute__((warn_unused_result))
    __attribute__((nonnull(1)));

/**
 * @return heap alocated, non-null terminated, owned, mutable string pointer with buffer allocated to the initial capacity
 * @note if initial_capacity = 0, initial_capacity will be set to 1
 */
StringMut* string_mut_new_sized(usize initial_capacity) __attribute__((warn_unused_result));

/**
 * @brief performs a deep copy of the given string_mut
 */
StringMut* string_mut_clone(const StringMut* string_mut)
    __attribute__((warn_unused_result))
    __attribute__((nonnull(1)));

/**
 * @brief deep free of StringMut* and sets pointer to NULL
 */
void string_mut_free(StringMut** string_mut) __attribute__((nonnull(1)));

/**
 * @param cstr_literal as a null terminated raw c string
 * @return heap allocated, mutable, owned CStringMut pointer
 * @note must be freed
 */
CStringMut* cstring_mut_new(const c8* cstr_literal)
    __attribute__((warn_unused_result))
    __attribute__((nonnull(1)));

/**
 * @return heap alocated, null-terminated, owned, mutable cstring pointer with buffer allocated to the initial capacity
 * @note if initial_capacity = 0, initial_capacity will be set to 1
 */
CStringMut* cstring_mut_new_sized(usize initial_capacity) __attribute__((warn_unused_result));

/**
 * @brief performs a deep copy of the given cstring_mut
 */
CStringMut* cstring_mut_clone(const CStringMut* cstring_mut)
    __attribute__((warn_unused_result))
    __attribute__((nonnull(1)));

/**
 * @brief deep free of CStringMut* and sets pointer to NULL
 */
void cstring_mut_free(CStringMut** cstring_mut) __attribute__((nonnull(1)));

/**
 * @param cstr_literal as a null terminated raw c string
 * @return stack allocated, not null terminated, wrapper around a raw c string with an attached length
 * @note does not require freeing
 */
Str str_init(const c8* cstr_literal) __attribute__((nonnull(1)));

/**
 * @param cstr_literal as a null terminated literal static c string
 * @return stack allocated non-null terminated wrapper around a raw c string with an attached length
 * @note does not require freeing
 * @note unlike str_init(cstr_literal), this can be used as an rvalue
 */
#define str_static(cstr_literal) ({                                                            \
    ASSERT_NONNULL_STATIC(cstr_literal);                                                       \
    static const Str str = (Str) {.buffer = cstr_literal, .length = sizeof(cstr_literal) - 1}; \
    &str;                                                                                      \
})

/**
 * @param cstr_literal as a null terminated literal c string
 * @return stack allocated null-terminated wrapper around a raw c string with an attached length
 * @note does not require freeing
 */
CStr cstr_init(const c8* cstr_literal) __attribute__((nonnull(1)));

/**
 * @param cstr_literal as a null terminated literal static c string
 * @return stack allocated null-terminated wrapper around a raw c string with an attached length
 * @note does not require freeing
 * @note unlike cstr_init(cstr_literal), this can be used as an rvalue
 */
#define cstr_static(cstr_literal) ({                                                              \
    ASSERT_NONNULL_STATIC(cstr_literal);                                                          \
    static const CStr cstr = (CStr) {.buffer = cstr_literal, .length = sizeof(cstr_literal) - 1}; \
    &cstr;                                                                                        \
})

/**
 * @brief shallow free of StrSlices and sets pointer to NULL
 * @note this does not free the strng these slices reference
 */
void str_slices_free(StrSlices** slices) __attribute__((nonnull(1)));

// MARK: Conversions

/**
 * @brief heap allocates a new string from the given str
 */
__attribute__((warn_unused_result))
__attribute__((nonnull(1))) static inline String*
str_to_owned(const StrSlice* str) {
    return string_clone((String*) str);
}

/**
 * @brief heap allocates a new cstring from the given cstr
 */
__attribute__((warn_unused_result))
__attribute__((nonnull(1))) static inline CString*
cstr_to_owned(const CStrSlice* cstr) {
    return cstring_clone((CString*) cstr);
}

/**
 * @brief heap allocates a new mutable cstring from the given cstr
 */
StringMut* str_to_owned_mut(const StrSlice* str)
    __attribute__((warn_unused_result))
    __attribute__((nonnull(1)));

/**
 * @brief heap allocates a new mutable cstring from the given cstr
 */
CStringMut* cstr_to_owned_mut(const CStrSlice* cstr)
    __attribute__((warn_unused_result))
    __attribute__((nonnull(1)));

// MARK: Shallow conversions

/**
 * @brief takes a reference to the given owned string without cloning
 * @note the lifetime of this returned ref is the same as the given mutable CString
 * @note because the referenced string is mutable, the references slice may change
 */
static inline StrSlice* string_mut_as_ref(const StringMut* string) {
    return ((StrSlice*) string);
}

/**
 * @brief takes a reference to the given owned cstring without cloning
 * @note the lifetime of this returned ref is the same as the given mutable String
 * @note because the referenced cstring is mutable, the references slice may change
 */
static inline CStrSlice* cstring_mut_as_ref(const CStringMut* cstring) {
    return ((CStrSlice*) cstring);
}

/**
 * @brief takes a reference to the given owned string without cloning
 * @note the lifetime of this returned ref is the same as the given String
 */
__attribute__((nonnull(1))) static inline StrSlice* string_as_ref(const String* string) {
    return (StrSlice*) string;
}

/**
 * @brief takes a reference to the given owned string without cloning
 * @note the lifetime of this returned ref is the same as the given CString
 */
__attribute__((nonnull(1))) static inline CStrSlice* cstring_as_ref(const CString* cstring) {
    return (CStrSlice*) cstring;
}

/**
 * @brief takes a StrSlice reference to the given owned cstring without cloning
 * @note the lifetime of this returned ref is the same as the given String
 * @note the casting of CString to StrSlice is possible as the null terminator is ignored
 * @note the casting of String to CStrSlice is faliable as the null terminator may be necessary
 */
__attribute__((nonnull(1))) static inline StrSlice* cstring_as_str_ref(const CString* cstring) {
    return (StrSlice*) cstring;
}

/**
 * @brief takes a StrSlice reference to the given owned cstring without cloning
 * @note the lifetime of this returned ref is the same as the given String
 * @note the casting of CString to StrSlice is possible as the null terminator is ignored
 * @note the casting of String to CStrSlice is faliable as the null terminator may be necessary
 */
__attribute__((nonnull(1))) static inline StrSlice* cstring_mut_as_str_ref(const CStringMut* cstring_mut) {
    return (StrSlice*) cstring_mut;
}

/**
 * @brief takes a StrSlice reference to the given owned cstring without cloning
 * @note the lifetime of this returned ref is the same as the given String
 * @note the casting of CStrSlice to StrSlice is possible as the null terminator is ignored
 * @note the casting of StrSlice to CStrSlice is faliable as the null terminator may be necessary
 */
__attribute__((nonnull(1))) static inline StrSlice* cstr_mut_as_str_ref(const CStrSlice* cstr) {
    return (StrSlice*) cstr;
}

// MARK: Comparison

/**
 * @return true if each string have the same length and same buffer contents
 */
bool str_equals(const StrSlice* one, const StrSlice* two) __attribute__((nonnull(1, 2)));

/**
 * @return positive if one > two, negative if two > one, or 0 if one == two
 */
i32 str_compare(const StrSlice* one, const StrSlice* two) __attribute__((nonnull(1, 2)));

/**
 * @return true if each cstring have the same length and same buffer contents
 * @note this is the same as str_equals(cstring_as_str_ref(cstring), cstring_as_str_ref(cstring))
 */
__attribute__((nonnull(1, 2))) static inline bool cstr_equals(const CStrSlice* one, const CStrSlice* two) {
    return str_equals((const StrSlice*) one, (const StrSlice*) two);
}

/**
 * @return positive if one > two, negative if two > one, or 0 if one == two
 * @note this is a helper for str_compare(cstring_as_str_ref(cstring), cstring_as_str_ref(cstring))
 */
__attribute__((nonnull(1, 2))) static inline i32 cstr_compare(const CStrSlice* one, const CStrSlice* two) {
    return str_compare((const StrSlice*) one, (const StrSlice*) two);
}

// MARK: Query

/**
 * @return a new mutable string with every instance of the given query replaced with given replacement
 * @note if a mutable, in-place, replacement is necessary, see string_mut_replace()
 */
String* str_replace(const StrSlice* str, const StrSlice* query, const StrSlice* replacement)
    __attribute__((nonnull(1, 2, 3)))
    __attribute__((warn_unused_result));

/**
 * @return a new mutable cstring with every instance of the given query replaced with given replacement
 * @note if a mutable, in-place, replacement is necessary, see cstring_mut_replace()
 */
CString* cstr_replace(const CStrSlice* cstr, const CStrSlice* query, const CStrSlice* replacement)
    __attribute__((nonnull(1, 2, 3)))
    __attribute__((warn_unused_result));

/**
 * @param str: existing string
 * @param start: start inclusive
 * @param length: length from start, inclusive
 * @return a reference slice to an existing string
 * @note this has the same lifetime as the given str
 * @note this does not need to be freed
 */
StrSlice str_sub_slice(const StrSlice* str, usize start, usize length) __attribute__((nonnull(1)));

/**
 * @param cstr: existing string
 * @param start: start inclusive
 * @param length: length from start, inclusive
 * @return a reference slice to an existing cstring
 * @note this has the same lifetime as the given cstr
 * @note this does not need to be freed
 * @note because a CStrSlice guarantees a null terminator, this returns a StrSlice to avoid new memory allocation
 */
__attribute__((nonnull(1))) static inline StrSlice cstr_sub_slice(const CStrSlice* cstr, usize start, usize length) {
    return str_sub_slice((const StrSlice*) cstr, start, length);
}

/**
 * @return a heap allocated struct wrapper around 1 or more slice references to a given str as split by the given delimiter
 * @note this does need to be freed, but freeing will not free the referenced string
 * @note if the delimiter is not found, the return value will be a single element consisting of the given str
 * @note all slices returned are guaranteed to not be empty
 */
StrSlices* str_split_slices(const StrSlice* str, c8 delimiter)
    __attribute__((nonnull(1)))
    __attribute__((warn_unused_result));

/**
 * @return a heap allocated struct wrapper around 1 or more slice references to a given cstr as split by the given delimiter
 * @note this does need to be freed, but freeing will not free the referenced string
 * @note if the delimiter is not found, the return value will be a single element consisting of the given cstr
 * @note all slices returned are guaranteed to not be empty
 * @note because CStrSlice must have a null terminator, this returns StrSlices as it does not modify the existing cstr
 */
__attribute__((nonnull(1)))
__attribute__((warn_unused_result)) static inline StrSlices*
cstr_split_slices(const CStrSlice* cstr, c8 delimiter) {
    return str_split_slices((const StrSlice*) cstr, delimiter);
}

/**
 * @return true if the given source str contains the given query str
 */
bool str_contains(const StrSlice* str, const StrSlice* query) __attribute__((nonnull(1, 2)));

/**
 * @return true if the given str contains the given query character
 */
bool str_contains_char(const StrSlice* str, c8 query) __attribute__((nonnull(1)));

/**
 * @return the first instance of the given query in the given str, or an empty optional if not found
 */
OptionIndex str_find(const StrSlice* str, c8 query) __attribute__((nonnull(1)));

/**
 * @return the last instance of the given query in the given str, or an empty optional if not found
 */
OptionIndex str_find_last(const StrSlice* str, c8 query) __attribute__((nonnull(1)));

/**
 * @return the nth instance from the front of the given query in the given str, or an empty optional if not found
 * @note n is index starting at 1
 */
OptionIndex str_find_nth(const StrSlice* str, c8 query, usize n) __attribute__((nonnull(1)));

/**
 * @return the nth instance from the back of the given query in the given str, or an empty optional if not found
 * @note n is index starting at 1
 */
OptionIndex str_find_last_nth(const StrSlice* str, c8 query, usize n) __attribute__((nonnull(1)));

// MARK: Mutation

/**
 * @brief replaces all instances of the given query string with the given replacement string
 * @note the size of the query and replacer do not need to match
 */
void string_mut_replace(StringMut* string_mut, const StrSlice* query, const StrSlice* replacer)
    __attribute__((nonnull(1, 2, 3)));

/**
 * @brief replaces all instances of the given query with the given replacement in place
 */
void string_mut_replace_char(StringMut* string_mut, c8 query, c8 replacement) __attribute__((nonnull(1)));

/**
 * @brief replaces all instances of the given query with the given replacement in place
 */
void cstring_mut_replace_char(CStringMut* cstring_mut, c8 query, c8 replacement) __attribute__((nonnull(1)));

/**
 * @brief pushes a given slice onto the end of mutable string
 */
void string_mut_push(StringMut* string_mut, const StrSlice* added) __attribute__((nonnull(1, 2)));

/**
 * @brief pushes a given character onto the end of mutable string
 */
void string_mut_push_char(StringMut* string_mut, c8 added) __attribute__((nonnull(1)));

/**
 * @brief changes the length of the given string without modifying the underlying data or capacity
 * @note to perform a deep clear, string_mut_clear_to() can be used
 */
void string_mut_clear(StringMut* string_mut) __attribute__((nonnull(1)));

/**
 * @brief sets all characters in the given string to the given value from 0 to string_mut->capacity inclusive
 * @note to perform a shallow clear string_mut_clear() can be used
 */
void string_mut_clear_to(StringMut* string_mut, c8 value) __attribute__((nonnull(1)));

/**
 * @brief pushes a given slice onto the end of mutable cstring
 * @note this functionmoves the existing null terminator from cstring to the end after the appended value
 */
void cstring_mut_push(CStringMut* cstring_mut, const CStrSlice* added) __attribute__((nonnull(1, 2)));

/**
 * @brief pushes a given character onto the end of mutable string
 */
void cstring_mut_push_char(CStringMut* string_mut, c8 added) __attribute__((nonnull(1)));

/**
 * @brief changes the length of the given cstring and sets the first character to \0
 * @note to perform a deep clear, cstring_mut_clear_to() can be used
 */
void cstring_mut_clear(CStringMut* cstring_mut) __attribute__((nonnull(1)));

/**
 * @brief sets all characters in the given string to the given value from 0 to string_mut->capacity exclusive
 * @note the last character at string_mut->buffer[capacity - 1] is now \0 regardless of what the value argument is
 * @note to perform a shallow clear cstring_mut_clear() can be used
 */
void cstring_mut_clear_to(CStringMut* cstring_mut, c8 value) __attribute__((nonnull(1)));

#endif
