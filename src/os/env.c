#include "env.h"
#include <stdlib.h>

OptionEnvVar env_var(const CStrSlice* var_name) {
    return option_env_var(str_init(getenv(var_name->buffer)));
}
