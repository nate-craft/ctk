#include "path.h"
#include <assert.h>
#include "../core/memory.h"
#include "../os/env.h"
#include "io.h"
#include "unistd.h"

// MARK: Lifecycle

PathMut* path_mut_new(const StrSlice* str) {
    ASSERT_NONNULL(str);

    PathMut* path_mut = heap_one(sizeof(PathMut));
    StrSlices* split = str_split_slices(str, '/');
    path_mut->nodes = vec_path_node_new(split->count);

    if (str->buffer[0] == '/') {
        vec_path_node_push_back_owned(path_mut->nodes, string_mut_new("/"));
    }

    for (usize i = 0; i < split->count; i++) {
        vec_path_node_push_back_owned(path_mut->nodes, str_to_owned_mut(&split->slices[i]));
    }

    str_slices_free(&split);

    return path_mut;
}

PathMut* path_mut_users() {
    return path_mut_new(str_static(HOME_LITERAL));
}

PathMut* path_mut_current() {
    c8* raw_current = getcwd(NULL, 0);
    Str raw_as_str = str_init(raw_current);
    PathMut* path_mut = path_mut_new(&raw_as_str);

    free(raw_current);

    return path_mut;
}

PathMut* path_mut_user() {
    PathMut* path_mut = path_mut_users();
    OptionEnvVar user = env_var(cstr_static("USER"));
    assert(user.present);

    vec_path_node_push_back_owned(path_mut->nodes, str_to_owned_mut(&user.value));

    return path_mut;
}

void path_mut_normalize(PathMut* path_mut) {
    ASSERT_NONNULL(path_mut);

    if (path_mut_is_empty(path_mut)) {
        vec_path_node_clear(path_mut->nodes);

        PathMut* current = path_mut_current();

        for (usize j = 0; j < current->nodes->count; j++) {
            StringMut* node_current = current->nodes->elements[j];
            vec_path_node_push_back_owned(path_mut->nodes, node_current);
        }

        free(current->nodes->elements);
        free(current->nodes);
        free(current);
        return;
    }

    VecPathNode* normalized = vec_path_node_new(path_mut->nodes->count);

    for (usize i = 0; i < path_mut->nodes->count; i++) {
        OptionPathNode node_opt = vec_path_node_get(path_mut->nodes, i);

        if (!node_opt.present) {
            continue;
        }

        StringMut* node = node_opt.value;
        bool is_slash = str_equals(string_mut_as_ref(node), str_static("/"));
        bool is_back = str_equals(string_mut_as_ref(node), str_static(".."));
        bool is_user = str_equals(string_mut_as_ref(node), str_static("~"));

        // "/" -> Add slash
        if (is_slash) {
            vec_path_node_push_back_owned(normalized, string_mut_clone(node));
            continue;
        }

        // "~" -> Add user
        if (is_user && i == 0) {
            PathMut* home = path_mut_user();

            for (usize j = 0; j < home->nodes->count; j++) {
                StringMut* node_current = home->nodes->elements[j];
                vec_path_node_push_back_owned(normalized, string_mut_clone(node_current));
            }

            path_mut_free(&home);
            continue;
        }

        // "~" after first entry -> Do not add
        if (is_user) {
            continue;
        }

        // ".." -> Push previous directory from cwd
        if (is_back && i == 0) {
            PathMut* current = path_mut_current();

            if (path_mut_is_root(current)) {
                vec_path_node_push_back_owned(normalized, current->nodes->elements[0]);
            } else {
                for (usize j = 0; j < current->nodes->count - 1; j++) {
                    StringMut* node_current = current->nodes->elements[j];
                    vec_path_node_push_back_owned(normalized, node_current);
                }
                string_mut_free(&current->nodes->elements[current->nodes->count - 1]);
            }

            free(current->nodes->elements);
            free(current->nodes);
            free(current);

            continue;
        }

        // ".." within path -> Remove previous if not at root
        if (is_back) {
            bool only_root = normalized->count == 1 && str_equals(string_mut_as_ref(normalized->elements[0]), str_static("/"));
            if (normalized->count >= 1 && !only_root) {
                vec_path_node_delete(normalized, normalized->count - 1);
            }
            continue;
        }

        vec_path_node_push_back_owned(normalized, string_mut_clone(node));
    }

    // Nothing in normalized after normalization, user cwd
    if (normalized->count == 0) {
        PathMut* current = path_mut_current();

        vec_path_node_clear(path_mut->nodes);

        for (usize j = 0; j < current->nodes->count; j++) {
            StringMut* node_current = current->nodes->elements[j];
            vec_path_node_push_back_owned(path_mut->nodes, node_current);
        }

        free(current->nodes->elements);
        free(current->nodes);
        free(current);
        vec_path_node_free(&normalized);

        return;
    }

    vec_path_node_free(&path_mut->nodes);
    path_mut->nodes = normalized;
}

Path* path_mut_to_path(PathMut* path_mut) {
    path_mut_normalize(path_mut);

    Path* path = heap_one(sizeof(Path));
    StringMut* uri_mut = string_mut_new_sized(path_mut->nodes->count * 4);

    for (usize i = 0; i < path_mut->nodes->count; i++) {
        StringMut* current = path_mut->nodes->elements[i];
        string_mut_push(uri_mut, string_mut_as_ref(current));
        if (i < path_mut->nodes->count - 1 && !str_equals(string_mut_as_ref(current), str_static("/"))) {
            string_mut_push_char(uri_mut, '/');
        }
    }

    if (uri_mut->capacity < uri_mut->length + 1) {
        uri_mut->buffer = heap_renew(uri_mut->buffer, sizeof(c8), ++uri_mut->capacity);
    }

    uri_mut->buffer[uri_mut->length] = '\0';
    path->uri = heap_one(sizeof(CString));
    path->uri->buffer = uri_mut->buffer;
    path->uri->length = uri_mut->length;
    free(uri_mut);

    return path;
}

void path_mut_free(PathMut** path_mut) {
    ASSERT_NONNULL(path_mut);
    ASSERT_NONNULL(*path_mut);

    vec_path_node_free(&(*path_mut)->nodes);
    free(*path_mut);
    *path_mut = NULL;
}

void path_free(Path** path) {
    ASSERT_NONNULL(path);
    ASSERT_NONNULL(*path);

    cstring_free(&(*path)->uri);
    free(*path);
    *path = NULL;
}

// MARK: Query

bool path_mut_is_absolute(PathMut* path_mut) {
    ASSERT_NONNULL(path_mut);

    return path_mut->nodes->count > 0 && path_mut->nodes->elements[0]->buffer[0] == '/';
}

bool path_mut_is_root(PathMut* path_mut) {
    ASSERT_NONNULL(path_mut);

    return path_mut->nodes->count == 1 && str_equals(string_mut_as_ref(path_mut->nodes->elements[0]), str_static("/"));
}

bool path_mut_is_empty(PathMut* path_mut) {
    ASSERT_NONNULL(path_mut);

    vec_for_each(StringMut node, path_mut->nodes, {
        if (node.length != 0) {
            return false;
        }
    });

    return true;
}

OptionPathNodeRef path_mut_node(PathMut* path_mut, usize index) {
    ASSERT_NONNULL(path_mut);

    if (index >= path_mut->nodes->count) {
        return option_path_node_ref_empty();
    }

    return option_path_node_ref(string_mut_as_ref(path_mut->nodes->elements[index]));
}

// MARK: Display

void path_mut_display(PathMut* path_mut) {
    print(str_static("["));

    for (usize i = 0; i < path_mut->nodes->count; i++) {
        print(str_static("\""));
        print(string_mut_as_ref(path_mut->nodes->elements[i]));
        print(str_static("\""));

        if (i < path_mut->nodes->count - 1) {
            print(str_static(", "));
        }
    }

    print(str_static("]\n"));
}

// MARK: Mutation

void path_mut_append(PathMut* path_mut, const StrSlice* added) {
    ASSERT_NONNULL(path_mut);
    ASSERT_NONNULL(added);

    vec_path_node_push_back_owned(path_mut->nodes, str_to_owned_mut(added));
}
