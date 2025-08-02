#ifndef CTK_OS_H
#define CTK_OS_H

#include "../string/string.h"

DEFINE_OPTION(Str, EnvVar, env_var, ((Str) {"", 0}))

OptionEnvVar env_var(const CStrSlice* var_name) __attribute__((nonnull(1)));

#endif
