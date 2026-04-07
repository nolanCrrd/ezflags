#ifndef EZFLAGS_H
#define EZFLAGS_H

#include <stdbool.h>

/*========================
 * Typedef
 * ======================== */

/**
 * @brief Type that store the found result of an option of a command line
 */
typedef struct s_flag
{
    char *name;     /**< Nom du flag (-v -> "v",  --verbose -> "verbose") */
    bool found;     /**< Adress where store found status */
    int min_params; /**< Number of parameter that option can have (-1 if
                         until another param) */
    int max_params;
    char **params; /**< Array where store params of the option */
    char required;
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
