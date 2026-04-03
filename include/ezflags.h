#ifndef EZFLAGS_H
#define EZFLAGS_H

/*========================
 * Defines
 * ======================== */

/*========================
 * Typedef
 * ======================== */

/**
 * @brief Type that store the found result of an option of a command line
 */
typedef struct s_flag {
  char *name;       /**< Nom du flag (-v -> "v",  --verbose -> "verbose") */
  char *found;      /**< Adress where store found status */
  int params_count; /**< Number of parameter that option can have (-1 if until
                       another param) */
  char **params;    /**< Array where store params of the option */
} flag_t;

/*========================
 * Public functions
 * ======================== */

/**
 * @brief Parse all argv and help you to get all flags and thair params
 *
 * @param flags flags to parse (argv + 1)
 * @param opt_array array that contain all the option define with opt_t type
 * (need a sentinelle)
 * @param still_argv pointer to a variable that will be allocated to store all
 * argv that are not specified flags
 * @return -1 in error case | number of flags not found | or 0 in total
 * success
 */
int ezflags(char **flags, flag_t *opt_array, char **still_argv[]);

/*========================
 * Private functions
 * ======================== */
void print_flag(flag_t flag);

#endif // EZFLAGS_H
