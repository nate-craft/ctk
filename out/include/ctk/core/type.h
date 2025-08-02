#ifndef CTK_TYPE_H
#define CTK_TYPE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

typedef char c8;
typedef size_t usize;

typedef void (*Destroy)(void*);
typedef void* (*Clone)(void*);
typedef bool (*Equals)(void*, void*);
typedef i32 (*Compare)(const void*, const void*);

/**
 * @brief Option type for the given type with attached helper functions
 *
 * @param type: inner type for the option
 * @param type_name: upper case name of the type (e.g., Int, Double, String)
 * @param func_name: lower case name of the type (e.g., int, double, string)
 * @param default_value: default value for empty options
 */
#define DEFINE_OPTION(type, type_name, func_name, default_value)                          \
    typedef struct {                                                                      \
        type value;                                                                       \
        bool present;                                                                     \
    } Option##type_name;                                                                  \
    static inline Option##type_name option_##func_name(type value) {                      \
        return (Option##type_name) {.value = value, .present = true};                     \
    }                                                                                     \
    static inline Option##type_name option_##func_name##_empty() {                        \
        return (Option##type_name) {.value = default_value, .present = false};            \
    }                                                                                     \
    static inline type option_##func_name##_get_or(Option##type_name value, type other) { \
        return value.present ? value.value : other;                                       \
    }                                                                                     \
    static inline type option_##func_name##_get_or_default(Option##type_name value) {     \
        return value.present ? value.value : default_value;                               \
    }                                                                                     \
    static inline type option_##func_name##_get(Option##type_name value) {                \
        if (!value.present) {                                                             \
            exit(1);                                                                      \
        }                                                                                 \
        return value.value;                                                               \
    }

#endif
