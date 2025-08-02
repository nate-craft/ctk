#include "ctk/io/path.h"
#include "ctk/os/env.h"
#include "ctk/string/string.h"

int main() {
    const OptionEnvVar env_home = env_var(cstr_static("HOME"));
    const OptionEnvVar env_pwd = env_var(cstr_static("PWD"));
    const StrSlice* env_users = str_static(HOME_LITERAL);

    PathMut* users = path_mut_users();
    PathMut* user = path_mut_user();
    PathMut* current = path_mut_current();
    PathMut* relative_blank = path_mut_new(str_static(""));
    PathMut* home_mod = path_mut_new(str_static("~/Forward/../Forward/../"));
    PathMut* relative_mod = path_mut_new(str_static("Forward/../Forward/../"));

    Path* users_norm = path_mut_to_path(users);
    Path* user_norm = path_mut_to_path(user);
    Path* current_norm = path_mut_to_path(current);
    Path* relative_blank_norm = path_mut_to_path(relative_blank);
    Path* home_mod_norm = path_mut_to_path(home_mod);
    Path* relative_mod_norm = path_mut_to_path(relative_mod);

    assert(str_equals(cstring_as_str_ref(user_norm->uri), &env_home.value));
    assert(str_equals(cstring_as_str_ref(current_norm->uri), &env_pwd.value));
    assert(str_equals(cstring_as_str_ref(users_norm->uri), env_users));
    assert(str_equals(cstring_as_str_ref(relative_blank_norm->uri), cstring_as_str_ref(current_norm->uri)));
    assert(str_equals(cstring_as_str_ref(user_norm->uri), cstring_as_str_ref(home_mod_norm->uri)));
    assert(str_equals(cstring_as_str_ref(current_norm->uri), cstring_as_str_ref(relative_mod_norm->uri)));

    path_mut_free(&users);
    path_mut_free(&user);
    path_mut_free(&current);
    path_mut_free(&relative_blank);
    path_mut_free(&home_mod);
    path_mut_free(&relative_mod);
    path_free(&users_norm);
    path_free(&user_norm);
    path_free(&current_norm);
    path_free(&relative_blank_norm);
    path_free(&home_mod_norm);
    path_free(&relative_mod_norm);

    assert(users == NULL);
    assert(users_norm == NULL);
}
