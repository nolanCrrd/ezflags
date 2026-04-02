#ifndef EZFLAGS_H
#define EZFLAGS_H

#include <stdbool.h>

/*========================
 * Defines
 * ======================== */

/*========================
 * Typedef
 * ======================== */

/**
 * @brief Type that store the found result of an option of a command line
 */
typedef struct s_opt {
  char *name;       /**< Nom du flag (-v -> "v",  --verbose -> "verbose") */
  bool *found;      /**< Adress where store found status */
  int params_count; /**< Number of parameter that option can have (-1 if until
                       another param) */
  char **params;    /**< Array where store params of the option */
} opt_t;

/*========================
 * Public functions
 * ======================== */

/**
 * @brief Parse all argv and help you to get all options and thair params
 *
 * @param options options to parse (argv + 1)
 * @param opt_array array that contain all the option define with opt_t type
 * @param still_argv pointer to a variable that will be allocated to store all
 * argv that are not specified options
 * @return -1 in error case | number of options not found | or 0 in total
 * success
 */
int ezflags(char **options, opt_t *opt_array, char **still_argv[]);

/*========================
 * Private functions
 * ======================== */

#endif // EZFLAGS_H
