#ifndef EZFLAGS_H
#define EZFLAGS_H

#include <stdbool.h>

/*========================
 * Typedef
 * ======================== */

typedef struct s_flag
{
    char *short_name;
    char *long_name;

    int min_args;
    int max_args;
    char **args;

    bool glued_arg;
    bool required;
    bool found;
} flag_t;

typedef enum e_status
{
    SUCCESS,

    // internal
    NOT_A_FLAG,
    NO_MORE_FLAG,

    // returnable
    MISSING_FLAG,
    FLAG_NOT_FOUND,
    FLAG_GROUP_NOT_FOUND,
    FLAG_GROUP_INVALID_ARGUMENT,
    REPTITIVE_FLAG,
    FLAG_ARGUMENT_NOT_FOUND,
    CANNOT_BE_GLUED,
    IMPOSSIBLE_FLAG,
    SYSTEM_ERROR,
    ERROR,

} ezflag_status;

/*========================
 * Public functions
 * ======================== */

/**
 * @brief Parse all argv and help you to get all flags and thair params
 *
 * @param flags flags to parse (argv + 1)
 * @param flag_array array that contain all the option define with opt_t type
 * (need a sentinelle)
 * @param still_argv pointer to a variable that will be allocated to store all
 * argv that are not specified flags
 * @return -1 in error case | number of flags not found | or 0 in total
 * success
 */
ezflag_status ezflags (char **args, flag_t flag_array[], char **still_argv[]);

void print_flag (flag_t flag);
void print_ezflags_result (flag_t flags[], char **still_argv);

// TODO: Make constructor helper

#endif // EZFLAGS_H
