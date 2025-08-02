# C Toolkit
![](https://gist.githubusercontent.com/nate-craft/648bbda6337b503a5d703f86757e4647/raw/144cf1f5f80e9c5ac6b5efde45869d01feb2ccd9/brainmade.png)

The C Toolkit is a C standard library implemented with type and memory safety in mind. This includes:
- Stronger data types (e.g., `i8`, `i16`, `i32`, `i64`)
- Explicit string types (e.g., `Str`, `StrSlice`, `String`, `StringMut`)
- Strongly typed data structures via macros (e.g., `VecString` instead of `Vec`)
- C STD compatibility (through structs like `CString` and `CStrSlice`)
- Type generic `display` function (e.g., `display("Format: {} + {} = {}\n{}{}\n", 1, 2, 3, str_static("It is "), true)`)
    - Prints `1 + 2 = 3\nIt is true\n`
- Null avoidance in favor of macro created Option types

## Installing

CTK can most simply be added to a project with the use of [cproject](https://github.com/nate-craft/cproject)

### Manual Installation
```sh
1. git clone https://github.com/nate-craft/ctk.git
2. cd ctk
3. ./build.sh --local 
# Both headers and a static library can be found in the out/ directory
```

## Usage

- Headers are set into directories pseudo name-spacing (e.g., `#include /ctk/string/string.h` 
- Functions, important macros, and structs are marked with inline documentation
- See [GUIDELINES](GUIDELINES.md) to understand code structure 

## Explicit Code 

CTK is designed to be as explicit as possible. As an example, CTK's string library features 8 string types
to fit all applications it may require without sacrificing performance or safety. See the following for clarity:

```c
// Macro call to create a static immutable indexed string
const Str* example= str_static("Example");

// Stack allocated immutable indexed string
const Str example = str_init("Example");

// Heap allocated immutable indexed string
String* example = string_new("Example");

// Heap allocated mutable indexed string
StringMut* example = string_mut_new("Example");

// Macro call to create a static immutable null-terminated string (c interoperability) 
const CStr* example = cstr_static("Example");

// Stack allocated immutable null-terminated string (c interoperability)
const CStr example = cstr_init("Example");

// Heap allocated immutable null-terminated string (c interoperability) 
CString* heap_immutable_cstr = cstring_new("Example");

// Heap allocated mutable null-terminated string (c interoperability)
CStringMut* heap_mut_cstr = cstring_mut_new("Example");
```

Many of these string types will have specified accompanying functions.
E.g., appending to a string requires a mutable string:

```c
StringMut* string = string_mut_new("Hello ");
string_mut_push(string, str_static("World"));
display("My String: {}\n", string_mut_as_ref(string));
string_mut_free(&string);
```

But many functions share the same type as many of the strings can be safely casted to a 
common type for read-only situations, including automatic casting for types that
are identical in said read-only situations:

```c
StringMut* string_mut = string_mut_new("Hello World");
String* string = string_new("Hello World");
Str str_stack = str_init("Hello World");
Str* str_static = str_static("Hello World");

println(&str_stack);                    // Auto casting
println(str_static);                    // Auto casting
println(string_as_ref(string));         // Requires explicit casting
println(string_mut_as_ref(string_mut)); // Requires explicit casting

string_mut_free(&string_mut);
string_free(&string);
```


