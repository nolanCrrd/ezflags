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

    int min_params;
    int max_params;
    char **params;

    bool glued_arg;
    bool required;
    bool found;
} flag_t;

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
int ezflags (char **args, flag_t flag_array[], char **still_argv[]);

// TODO: Make constructor helper

#endif // EZFLAGS_H
