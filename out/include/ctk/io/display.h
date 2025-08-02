#ifndef CTK_DISPLAY_H
#define CTK_DISPLAY_H

#include "../string/string.h"
#include "../core/memory.h"

#define display(fmt, ...) \
    do { \
		ASSERT_NONNULL_STATIC(fmt); \
		__auto_type fmt_typed = fmt; \
        StrSlice fmt_slice = _Generic((fmt_typed), \
            const char*: str_init, \
            char*: str_init, \
            const StrSlice*: _identity_str \
        )(fmt_typed); \
        _DISPLAY_WRAPPER(fmt_slice, _COUNT_ARGS(__VA_ARGS__), __VA_ARGS__); \
    } while (0)

// INTERNAL

typedef enum {
    _TYPE_INT_32,
    _TYPE_INT_64,
    _TYPE_UINT,
    _TYPE_FLOAT_32,
    _TYPE_FLOAT_64,
    _TYPE_CHAR_ARR,
    _TYPE_CHAR,
    _TYPE_BOOL,
    _TYPE_STR,
    _TYPE_CSTR,
	_TYPE_STRING,
    _TYPE_CSTRING
} _PrintType;

typedef struct {
    _PrintType type;
    void* param;
} _Display;

void _print(_Display display);
void _display(StrSlice format, _Display* formatters, usize formatter_count);
static inline StrSlice _identity_str(const StrSlice* s) { return *s; }

#define FUNC_FOR_TYPE(input) _Generic((input), \
    usize: (_Display) { .type = _TYPE_UINT, .param = (void*) &input }, \
    i32: (_Display) { .type = _TYPE_INT_32, .param = (void*) &input }, \
    i64: (_Display) { .type = _TYPE_INT_64, .param = (void*) &input }, \
    f32: (_Display) { .type = _TYPE_FLOAT_32, .param = (void*) &input }, \
    f64: (_Display) { .type = _TYPE_FLOAT_64, .param = (void*) &input }, \
    bool: (_Display) { .type = _TYPE_BOOL, .param = (void*) &input }, \
	c8: (_Display) { .type = _TYPE_CHAR, .param = (void*) &input }, \
	const StrSlice*: (_Display) { .type = _TYPE_STR, .param = *(void**) &input }, \
	StrSlice*: (_Display) { .type = _TYPE_STR, .param = *(void**) &input }, \
    const CStrSlice*: (_Display) { .type = _TYPE_CSTR, .param = *(void**) &input }, \
    CStrSlice*: (_Display) { .type = _TYPE_CSTR, .param = *(void**) &input }, \
	String*: (_Display) { .type = _TYPE_STRING, .param = *(void**) &input }, \
    CString*: (_Display) { .type = _TYPE_CSTRING, .param = *(void**) &input }, \
    c8*: (_Display) { .type = _TYPE_CHAR_ARR, .param = *(void**) &input } \
)

#define _LOCAL(num, val) __auto_type arg_##num = (val)\

#define _DISPLAY_1(fmt_slice, a1) \
    do { \
		_LOCAL(1, a1); \
        _Display formatters[] = { FUNC_FOR_TYPE(arg_1) }; \
        _display(fmt_slice, formatters, 1); \
    } while (0)

#define _DISPLAY_2(fmt_slice, a1, a2) \
    do { \
		_LOCAL(1, a1); _LOCAL(2, a2); \
        _Display formatters[] = { FUNC_FOR_TYPE(arg_1), FUNC_FOR_TYPE(arg_2) }; \
        _display(fmt_slice, formatters, 2); \
    } while (0)

#define _DISPLAY_3(fmt_slice, a1, a2, a3) \
    do { \
		_LOCAL(1, a1); _LOCAL(2, a2); _LOCAL(3, a3); \
        _Display formatters[] = { FUNC_FOR_TYPE(arg_1), FUNC_FOR_TYPE(arg_2), FUNC_FOR_TYPE(arg_3) }; \
        _display(fmt_slice, formatters, 3); \
    } while (0)

#define _DISPLAY_4(fmt_slice, a1, a2, a3, a4) \
    do { \
		_LOCAL(1, a1); _LOCAL(2, a2); _LOCAL(3, a3); _LOCAL(4, a4); \
        _Display formatters[] = { \
			FUNC_FOR_TYPE(arg_1), FUNC_FOR_TYPE(arg_2), FUNC_FOR_TYPE(arg_3), \
			FUNC_FOR_TYPE(arg_4) \
		}; \
        _display(fmt_slice, formatters, 4); \
    } while (0)

#define _DISPLAY_5(fmt_slice, a1, a2, a3, a4, a5) \
    do { \
		_LOCAL(1, a1); _LOCAL(2, a2); _LOCAL(3, a3); _LOCAL(4, a4); _LOCAL(5, a5); \
        _Display formatters[] = { \
			FUNC_FOR_TYPE(arg_1), FUNC_FOR_TYPE(arg_2), FUNC_FOR_TYPE(arg_3), \
			FUNC_FOR_TYPE(arg_4), FUNC_FOR_TYPE(arg_5) \
		}; \
        _display(fmt_slice, formatters, 5); \
    } while (0)

#define _DISPLAY_6(fmt_slice, a1, a2, a3, a4, a5, a6) \
    do { \
		_LOCAL(1, a1); _LOCAL(2, a2); _LOCAL(3, a3); _LOCAL(4, a4); _LOCAL(5, a5); _LOCAL(6, a6); \
        _Display formatters[] = { \
			FUNC_FOR_TYPE(arg_1), FUNC_FOR_TYPE(arg_2), FUNC_FOR_TYPE(arg_3), \
			FUNC_FOR_TYPE(arg_4), FUNC_FOR_TYPE(arg_5), FUNC_FOR_TYPE(arg_6)  \
		}; \
        _display(fmt_slice, formatters, 6); \
    } while (0)


#define _CHOOSE_DISPLAY_MACRO(_1,_2,_3,_4,_5,_6,NAME,...) NAME
#define _EXPAND_DISPLAY(...) \
    _CHOOSE_DISPLAY_MACRO(__VA_ARGS__, _DISPLAY_6, _DISPLAY_5, _DISPLAY_4, _DISPLAY_3, _DISPLAY_2, _DISPLAY_1)

#define _EXPAND_LOCALS_IMPL(a1, a2, a3, a4, a5, a6, _NUM, ...) \
    _LOCAL(1, a1); \
    _LOCAL(2, a2); \
    _LOCAL(3, a3); \
    _LOCAL(4, a4); \
    _LOCAL(5, a5); \
    _LOCAL(6, a6);
#define _EXPAND_LOCALS(...) _EXPAND_LOCALS_IMPL(__VA_ARGS__, 6, 5, 4, 3, 2, 1)

#define _COUNT_ARGS_IMPL(_1, _2, _3, _4, _5, _6, N, ...) N
#define _COUNT_ARGS(...) _COUNT_ARGS_IMPL(__VA_ARGS__, 6, 5, 4, 3, 2, 1)

#define _DISPLAY_WRAPPER(fmt_slice, num_args, ...) \
    do { \
        _EXPAND_LOCALS(__VA_ARGS__); \
        _Display formatters[] = { FUNC_FOR_TYPE(arg_1), FUNC_FOR_TYPE(arg_2), FUNC_FOR_TYPE(arg_3), \
                                 FUNC_FOR_TYPE(arg_4), FUNC_FOR_TYPE(arg_5), FUNC_FOR_TYPE(arg_6) }; \
        _display(fmt_slice, formatters, num_args); \
    } while (0)

#endif
