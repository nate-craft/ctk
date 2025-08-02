#include "ctk/io/io.h"

int main() {
    const Str* stack_static = str_static("Stack Static Allocated");
    const Str stack = str_init("Stack Allocated");
    String* heap_immutable = string_new("Heap Allocated");
    String* heap_immutable_clone = string_clone(heap_immutable);
    CString* heap_immutable_cstr = cstring_new("Heap Allocated CStr");
    CString* heap_immutable_cstr_clone = cstring_clone(heap_immutable_cstr);
    StringMut* heap_mut = string_mut_new("Heap Allocated + Mut");
    StringMut* heap_mut_clone = string_mut_clone(heap_mut);
    CStringMut* heap_mut_cstr = cstring_mut_new("Heap Allocated + Mut + CString");
    CStringMut* heap_mut_cstr_clone = cstring_mut_clone(heap_mut_cstr);

    String* replaced = str_replace(string_as_ref(heap_immutable), str_static("Heap"), str_static("Heap (Modified!)"));
    assert(str_equals(string_as_ref(replaced), str_static("Heap (Modified!) Allocated")));

    CString* replaced_cstr = cstr_replace(cstring_as_ref(heap_immutable_cstr), cstr_static("Heap"), cstr_static("Heap (Modified!)"));
    assert(cstr_equals(cstring_as_ref(replaced_cstr), cstr_static("Heap (Modified!) Allocated CStr")));

    StrSlices* slices_space = str_split_slices(string_mut_as_ref(heap_mut), ' ');
    StrSlices* slices_last = str_split_slices(string_mut_as_ref(heap_mut), 't');
    StrSlices* slices_first = str_split_slices(string_mut_as_ref(heap_mut), 'H');
    StrSlices* slices_missing = str_split_slices(string_mut_as_ref(heap_mut), 'Z');

    assert(slices_space->count == 4);
    assert(slices_last->count == 2);
    assert(slices_first->count == 1);
    assert(slices_missing->count == 1);

    assert(str_equals(&stack, str_static("Stack Allocated")));
    assert(!str_equals(&stack, str_static("Heap Allocated")));
    assert(str_equals(stack_static, str_static("Stack Static Allocated")));
    assert(!str_equals(&stack, stack_static));

    assert(!str_equals(string_as_ref(heap_immutable), str_static("Stack Allocated")));
    assert(str_equals(string_as_ref(heap_immutable), str_static("Heap Allocated")));
    assert(str_equals(string_as_ref(heap_immutable), string_as_ref(heap_immutable_clone)));

    assert(!str_equals(string_mut_as_ref(heap_mut), str_static("Heap Allocated")));
    assert(str_equals(string_mut_as_ref(heap_mut), str_static("Heap Allocated + Mut")));
    assert(str_equals(string_mut_as_ref(heap_mut), string_mut_as_ref(heap_mut_clone)));

    assert(!cstr_equals(cstring_mut_as_ref(heap_mut_cstr), cstr_static("Heap Allocated + Mut")));
    assert(cstr_equals(cstring_mut_as_ref(heap_mut_cstr), cstr_static("Heap Allocated + Mut + CString")));
    assert(cstr_equals(cstring_mut_as_ref(heap_mut_cstr), cstring_mut_as_ref(heap_mut_cstr_clone)));

    assert(str_compare(&stack, stack_static) < 1);
    assert(str_compare(stack_static, &stack) > 1);
    assert(str_compare(string_as_ref(heap_immutable), string_as_ref(heap_immutable_clone)) == 0);

    string_mut_push(heap_mut, str_static(" + Pushed"));
    string_mut_push(heap_mut, str_static(""));
    assert(str_equals(string_mut_as_ref(heap_mut), str_static("Heap Allocated + Mut + Pushed")));

    cstring_mut_push(heap_mut_cstr, cstr_static(" + Pushed"));
    cstring_mut_push(heap_mut_cstr, cstr_static(""));
    assert(cstr_equals(cstring_mut_as_ref(heap_mut_cstr), cstr_static("Heap Allocated + Mut + CString + Pushed")));

    string_mut_replace_char(heap_mut, ' ', '_');
    string_mut_replace_char(heap_mut, 'd', '&');
    string_mut_replace_char(heap_mut, 'H', '{');
    string_mut_replace(heap_mut, str_static("Mut"), str_static("Modifiable"));
    assert(str_equals(string_mut_as_ref(heap_mut), str_static("{eap_Allocate&_+_Modifiable_+_Pushe&")));

    string_mut_clear(heap_mut);
    string_mut_clear_to(heap_mut_clone, '\0');
    assert(str_equals(string_mut_as_ref(heap_mut), str_static("")));
    assert(str_equals(string_mut_as_ref(heap_mut_clone), str_static("")));

    OptionIndex index = str_find(&stack, 'd');
    assert(index.present);
    assert(option_index_get(index) == stack.length - 1);

    OptionIndex last = str_find_last(&stack, 't');
    assert(last.present);
    assert(option_index_get(last) == stack.length - 3);

    OptionIndex second = str_find_nth(&stack, 'a', 2);
    assert(second.present);
    assert(option_index_get(second) == 11);

    OptionIndex second_last = str_find_last_nth(&stack, 'a', 2);
    assert(second_last.present);
    assert(option_index_get(second_last) == 2);

    string_free(&heap_immutable);
    string_free(&heap_immutable_clone);
    string_free(&replaced);
    string_mut_free(&heap_mut);
    string_mut_free(&heap_mut_clone);
    cstring_mut_free(&heap_mut_cstr);
    cstring_mut_free(&heap_mut_cstr_clone);
    cstring_free(&replaced_cstr);
    cstring_free(&heap_immutable_cstr);
    cstring_free(&heap_immutable_cstr_clone);
    str_slices_free(&slices_space);
    str_slices_free(&slices_last);
    str_slices_free(&slices_first);
    str_slices_free(&slices_missing);

    assert(heap_immutable == NULL);
    assert(heap_immutable_clone == NULL);
    assert(heap_immutable_cstr == NULL);
    assert(heap_immutable_cstr_clone == NULL);
    assert(heap_mut == NULL);
    assert(heap_mut_clone == NULL);
    assert(replaced == NULL);
    assert(heap_mut_cstr == NULL);
    assert(heap_mut_cstr_clone == NULL);
    assert(replaced_cstr == NULL);
    assert(slices_space == NULL);
    assert(slices_last == NULL);
    assert(slices_first == NULL);
    assert(slices_missing == NULL);

    println(str_static("\nAll string tests passed!\n"));

    return 0;
}
