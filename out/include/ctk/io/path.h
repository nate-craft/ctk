#ifndef CTK_PATH_H
#define CTK_PATH_H

#include "../collection/vector.h"
#include "../string/string.h"

#ifdef __APPLE__
#define HOME_LITERAL "/Users"
#else
#define HOME_LITERAL "/home"
#endif

DEFINE_VEC(StringMut, PathNode, path_node, string_mut_clone, string_mut_free, str_compare)
DEFINE_OPTION(StrSlice*, PathNodeRef, path_node_ref, NULL)

// MARK: Definition

typedef struct {
    VecPathNode* nodes;
} PathMut;

typedef struct {
    CString* uri;
} Path;

// MARK: Lifecycle

PathMut* path_mut_new(const StrSlice* str)
    __attribute__((warn_unused_result))
    __attribute__((nonnull(1)));

void path_mut_free(PathMut** path_mut) __attribute__((nonnull(1)));

void path_mut_normalize(PathMut* path_mut) __attribute__((nonnull(1)));
Path* path_mut_to_path(PathMut* path_mut)
    __attribute__((warn_unused_result))
    __attribute__((nonnull(1)));

void path_free(Path** path) __attribute__((nonnull(1)));

PathMut* path_mut_users() __attribute__((warn_unused_result));
PathMut* path_mut_user() __attribute__((warn_unused_result));
PathMut* path_mut_current() __attribute__((warn_unused_result));

// MARK: Query

bool path_mut_is_absolute(PathMut* path_mut) __attribute__((nonnull(1)));
bool path_mut_is_root(PathMut* path_mut) __attribute__((nonnull(1)));
bool path_mut_is_empty(PathMut* path_mut) __attribute__((nonnull(1)));

OptionPathNodeRef path_mut_node(PathMut* path_mut, usize index) __attribute__((nonnull(1)));

// MARK: Display

void path_mut_display(PathMut* path_mut) __attribute__((nonnull(1)));

// MARK: Mutation

void path_mut_append(PathMut* path_mut, const StrSlice* added) __attribute__((nonnull(1, 2)));

#endif
