#ifndef EZFLAG_INTERNAL_H
#define EZFLAG_INTERNAL_H

#include "ezflags.h"

typedef enum e_arg_type
{
    SHORT_FLAG,
    SHORT_FLAG_GROUP,
    LONG_FLAG,
    ARGUMENT,
    END_FLAG,
} arg_type;

typedef struct s_arg_result
{
    ezflag_status status;
    int information;
} arg_result_t;

void print_flag (flag_t flag);
void print_ezflags_result (flag_t flags[], char **still_argv);

#endif // EZFLAG_INTERNAL_H
