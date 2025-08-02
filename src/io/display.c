#include "display.h"
#include "../core/error.h"
#include "../core/memory.h"
#include <stdio.h>

void _print(_Display display) {
	ASSERT_NONNULL(display.param);
    switch (display.type) {
        case _TYPE_INT_32: printf("%d", *(i32*) display.param); break;
        case _TYPE_INT_64: printf("%ld", *(i64*) display.param); break;
        case _TYPE_UINT: printf("%zu", *(usize*) display.param); break;
        case _TYPE_FLOAT_32: printf("%.2f", *(f32*) display.param); break;
        case _TYPE_FLOAT_64: printf("%.2f", *(f64*) display.param); break;
        case _TYPE_CHAR_ARR: puts((char*) display.param); break;
        case _TYPE_CHAR: putchar(*(char*) display.param); break;
        case _TYPE_BOOL: puts(*(bool*) display.param ? "true" : "false"); break;
        case _TYPE_STR: fwrite(((StrSlice*) display.param)->buffer, 1, ((StrSlice*) display.param)->length, stdout); break;
        case _TYPE_CSTR: fwrite(((CStrSlice*) display.param)->buffer, 1, ((CStrSlice*) display.param)->length, stdout); break;
        case _TYPE_STRING: fwrite(((String*) display.param)->buffer, 1, ((String*) display.param)->length, stdout); break;
        case _TYPE_CSTRING: fwrite(((CString*) display.param)->buffer, 1, ((CString*) display.param)->length, stdout); break;
    }
}

void _display(StrSlice format, _Display* formatters, usize formatter_count) {
	usize format_index = 0;
    const c8* fmt_ptr_current = format.buffer;
    const c8* fmt_ptr_end = fmt_ptr_current + format.length;

    while (fmt_ptr_current < fmt_ptr_end) {
        c8 current_char = *fmt_ptr_current;
        c8 next_char = *(fmt_ptr_current + 1); 

        if (current_char == '{' && next_char == '}') {
            if (format_index < formatter_count) {
				_print(formatters[format_index]);
                format_index++;
            } else {
                panic(str_static("Invalid number of args for 'display()'"));
            }
            fmt_ptr_current += 2; 
            continue;
        }

        if (current_char == '\\') {
            if (next_char == '\0') {
				puts("\\0");
            } else if (fmt_ptr_current + 1 < fmt_ptr_end) {
                putchar(next_char);
            } else {
                putchar(current_char);
            }
            fmt_ptr_current += 2;
            continue;
        }

        putchar(current_char);
        fmt_ptr_current++;
    }
}

