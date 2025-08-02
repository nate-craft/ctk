#ifndef CTK_VECTOR_H
#define CTK_VECTOR_H

#include "../core/memory.h"

#define DEFINE_VEC(type, type_name, func_name, clone, destroy, compare)                                        \
    typedef struct {                                                                                           \
        type** elements;                                                                                       \
        usize count;                                                                                           \
        usize capacity;                                                                                        \
    } Vec##type_name;                                                                                          \
                                                                                                               \
    typedef type* (*_clone_##func_name)(type*);                                                                \
    typedef void(_destroy_##func_name)(type**);                                                                \
    typedef i32(_compare_##func_name)(const type**, const type**);                                             \
    DEFINE_OPTION(type*, type_name, func_name, NULL)                                                           \
                                                                                                               \
    static inline Vec##type_name* vec_##func_name##_new(usize initial_capacity) {                              \
        Vec##type_name* vec = (Vec##type_name*) heap_many(sizeof(Vec##type_name), initial_capacity);           \
        vec->elements = (type**) heap_many(sizeof(type*), initial_capacity);                                   \
        vec->count = 0;                                                                                        \
        vec->capacity = initial_capacity;                                                                      \
        return vec;                                                                                            \
    }                                                                                                          \
    static inline void vec_##func_name##_resize(Vec##type_name* vec, usize new_capacity) {                     \
        ASSERT_NONNULL(vec);                                                                                   \
        vec->elements = (type**) heap_renew(vec->elements, sizeof(type*), new_capacity);                       \
        vec->capacity = new_capacity;                                                                          \
    }                                                                                                          \
    static inline Vec##type_name* vec_##func_name##_clone(Vec##type_name* vec) {                               \
        ASSERT_NONNULL(vec);                                                                                   \
        Vec##type_name* cloned = vec_##func_name##_new(vec->capacity);                                         \
        cloned->count = vec->count;                                                                            \
        for (usize i = 0; i < vec->count; i++) {                                                               \
            if (cloned->elements[i] != NULL) {                                                                 \
                cloned->elements[i] = clone(vec->elements[i]);                                                 \
            } else {                                                                                           \
                cloned->elements[i] = NULL;                                                                    \
            }                                                                                                  \
        }                                                                                                      \
        return cloned;                                                                                         \
    }                                                                                                          \
    static inline void vec_##func_name##_free(Vec##type_name** vec) {                                          \
        ASSERT_NONNULL(vec);                                                                                   \
        ASSERT_NONNULL(*vec);                                                                                  \
        for (usize i = 0; i < (*vec)->count; i++) {                                                            \
            if ((*vec)->elements[i] != NULL) {                                                                 \
                destroy(&(*vec)->elements[i]);                                                                 \
            }                                                                                                  \
        }                                                                                                      \
        free((*vec)->elements);                                                                                \
        free(*vec);                                                                                            \
        *vec = NULL;                                                                                           \
    }                                                                                                          \
    static inline void vec_##func_name##_push_back_owned(Vec##type_name* vec, type* element) {                 \
        ASSERT_NONNULL(vec);                                                                                   \
        ASSERT_NONNULL(element);                                                                               \
        if (vec->count + 1 > vec->capacity) {                                                                  \
            vec_##func_name##_resize(vec, vec->capacity * 2);                                                  \
        }                                                                                                      \
        vec->elements[vec->count] = element;                                                                   \
        vec->count++;                                                                                          \
    }                                                                                                          \
    static inline void vec_##func_name##_push_back(Vec##type_name* vec, type element) {                        \
        ASSERT_NONNULL(vec);                                                                                   \
        vec_##func_name##_push_back_owned(vec, clone(&element));                                               \
    }                                                                                                          \
    static inline Option##type_name vec_##func_name##_get(Vec##type_name* vec, usize index) {                  \
        ASSERT_NONNULL(vec);                                                                                   \
        if (index >= vec->count) {                                                                             \
            return option_##func_name##_empty();                                                               \
        }                                                                                                      \
        return option_##func_name(vec->elements[index]);                                                       \
    }                                                                                                          \
    static inline Option##type_name vec_##func_name##_last(Vec##type_name* vec) {                              \
        ASSERT_NONNULL(vec);                                                                                   \
        if (vec->count <= 0) {                                                                                 \
            return option_##func_name##_empty();                                                               \
        }                                                                                                      \
        return option_##func_name(vec->elements[vec->count - 1]);                                              \
    }                                                                                                          \
    static inline Option##type_name vec_##func_name##_first(Vec##type_name* vec) {                             \
        ASSERT_NONNULL(vec);                                                                                   \
        if (vec->count <= 0) {                                                                                 \
            return option_##func_name##_empty();                                                               \
        }                                                                                                      \
        return option_##func_name(vec->elements[0]);                                                           \
    }                                                                                                          \
    static inline Option##type_name vec_##func_name##_remove(Vec##type_name* vec, usize index) {               \
        ASSERT_NONNULL(vec);                                                                                   \
        if (index >= vec->count) {                                                                             \
            return option_##func_name##_empty();                                                               \
        }                                                                                                      \
        type* value = vec->elements[index];                                                                    \
        type* cloned = clone(value);                                                                           \
        destroy(&value);                                                                                       \
        for (usize i = index; i < vec->count - 1; i++) {                                                       \
            vec->elements[i] = vec->elements[i + 1];                                                           \
        }                                                                                                      \
        vec->count--;                                                                                          \
        return option_##func_name(cloned);                                                                     \
    }                                                                                                          \
    static inline Option##type_name vec_##func_name##_pop_back(Vec##type_name* vec) {                          \
        return vec_##func_name##_remove(vec, vec->count - 1);                                                  \
    }                                                                                                          \
    static inline Option##type_name vec_##func_name##_pop_first(Vec##type_name* vec) {                         \
        return vec_##func_name##_remove(vec, 0);                                                               \
    }                                                                                                          \
    static inline void vec_##func_name##_clear(Vec##type_name* vec) {                                          \
        for (i32 i = vec->count - 1; i >= 0; i--) {                                                            \
            destroy(&vec->elements[i]);                                                                        \
        }                                                                                                      \
        vec->count = 0;                                                                                        \
    }                                                                                                          \
    static inline void vec_##func_name##_delete(Vec##type_name* vec, usize index) {                            \
        ASSERT_NONNULL(vec);                                                                                   \
        destroy(&(vec->elements[index]));                                                                      \
        for (usize i = index; i < vec->count - 1; i++) {                                                       \
            vec->elements[i] = vec->elements[i + 1];                                                           \
        }                                                                                                      \
        vec->count--;                                                                                          \
    }                                                                                                          \
    static inline void vec_##func_name##_sort(Vec##type_name* vec) {                                           \
        ASSERT_NONNULL(vec);                                                                                   \
        qsort(vec->elements, vec->count, sizeof(type*), (Compare) compare);                                    \
    }                                                                                                          \
    static inline void vec_##func_name##_sort_custom(Vec##type_name* vec, _compare_##func_name compare_func) { \
        ASSERT_NONNULL(vec);                                                                                   \
        qsort(vec->elements, vec->count, sizeof(type*), (Compare) compare_func);                               \
    }

#define vec_for_each(declaration, vector, body)                                                  \
    do {                                                                                         \
        for (usize _i_##__COUNTER__ = 0; _i_##__COUNTER__ < vector->count; _i_##__COUNTER__++) { \
            declaration = *vector->elements[_i_##__COUNTER__];                                   \
            body                                                                                 \
        }                                                                                        \
    } while (0);

#endif
