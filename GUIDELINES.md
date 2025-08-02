# Code Guidelines

## Syntax 
- Use typedef structs and enums (e.g., `typedef struct { fields } MyStruct`)
- Use lower snakecase for functions and variables (e.g., `foo_bar()`, `i32 my_foo`)
- Use lower snakecase for public macros that imitate functions (e.g, `display()`) 
- Use upper snake case for define, assert, and constant value macros (e.g, `DEFINE_MY_TYPE()`, `ASSERT_FOO()`, `MATH_PI`) 
- Use upper snake case prefixed with an underscore for private implementation macros (e.g., `_NOT_FOR_PUBLIC_DEF`)
- Use upper snake case for enum variants (e.g., `MY_TYPE_VARIANT`)
- Use full words instead of single letter prefixes (e.g., `str`/`string` instead of `s`)

## Practices
- Duplicate code twice before outsourcing responsibility to a new function
- Always mark pointer arguments as nonnull
- Always check pointer arguments for nullability
- Only declare one variable per line
- Use asserts and panic to avoid undefined behavior

## Naming Patterns
- Functions with `new` in their name heap allocate objects
- Functions with `init` in their name initialize fields in a stack allocated object
- Functions with `owned` in their name take ownership of any input
- Functions with `from` in their name do not take ownership of any input
- Functions with `ref` in their name cast to an analogous type without cloning
- Functions with `free` in their name free the given object
    - For reference types, `free` and `deep` naming should clean up the object entirely
    - For non-reference types, functions marked `free` are expected to do the same as `deep`
- Any functions without the above terms may provide information via documentation 
    - E.g., `string_split_slices()` is marked as returning heap data in its docs 

