#ifndef EZFLAG_INTERNAL_H
#define EZFLAG_INTERNAL_H

#include "ezflags.h"

/*========================
 * Typedef
 * ======================== */

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

/*========================
 * Utils functions functions
 * ======================== */

/**
 * @brief Append a string to a given array (allocate the necessary space alone)
 *
 * @param str string to append
 * @param array_ptr array where to append
 * @param array_size current size of the array
 * @return
 */
int str_array_append (char *str, char ***array_ptr, int array_size);

/*========================
 * Implementation functions
 * ======================== */

/**
 * @brief Return the type of the given arg string
 *
 * @param arg string
 * @return Kind of flag or argument
 */
arg_type get_arg_type (char *arg);

/**
 * @brief Find the index that match the flag string in the array
 *
 * @param flag string of the flag
 * @param flag_type kin of flag (short, long)
 * @param flag_array the array of flag defined by the user
 * @param equal_position the index of the equal sign in the flag string
 * @return index of the flag / -1 if not found
 */
int get_flag_index (char *flag, arg_type flag_type, flag_t flag_array[],
                    int equal_position);

/**
 * @brief Fill all the arg of the flag with other args
 *
 * @param flag flag to fill
 * @param args args that come after the flag
 * @param size size of flag's params (if already filled with things)
 * @return number of filled args
 */
int fill_spaced_arg (flag_t *flag, char **args, int size);

/**
 * @brief Fill argument starting by the glued one (-Iinclude or
 * --Include=include)
 *
 * @param flag flag to fill
 * @param flag_type kind of flag (short, long)
 * @param flag_str string of the flag (to get the glued arg)
 * @param equal_position index of the equal in the string
 * @return number of filled args (0 for one and -1 for 0)
 */
int fill_glued_arg (flag_t *flag, arg_type flag_type, char *flag_str,
                    int equal_position);

/**
 * @brief Fill all argument of the flag independant of glued or not
 *
 * @param flag flag to fill
 * @param flag_str string of the flag
 * @param flag_type kind of flag (short, long)
 * @param equal_position index of equal sign in the string
 * @param args args that come after the flag
 * @return number of filled args (sum of glued and spaced)
 */
int fill_flag_args (flag_t *flag, char *flag_str, arg_type flag_type,
                    int equal_position, char **args);

/**
 * @brief Find and fill the correct flag according to the given string
 *
 * @param flag string that contain the flag
 * @param flag_type kind of flag identified
 * @param args args that come after the flag
 * @param flag_array all flag defined by the user
 * @return Result status and information
 */
arg_result_t fill_flag (char *flag, arg_type flag_type, char **args,
                        flag_t flag_array[]);

/**
 * @brief Make all action to treat the current arg (flag or not)
 *
 * @param args args to analyse (always start from index 0)
 * @param flag_array Flag defined by the user
 * @return Result status and information
 */
arg_result_t parse_next_arg (char **args, flag_t flag_array[]);

/**
 * @brief Check if no arg required is not found
 *
 * @param flag_array FLags defined by the user
 * @return 0 if ok and 1 if missing at least one flag
 */
int check_all_required (flag_t flag_array[]);

/**
 * @brief tell if the error of this arg need to stop the parsing
 *
 * @param status the status to check
 * @return 0 if not and 1 if yes
 */
int is_important_error (ezflag_status status);

#endif // EZFLAG_INTERNAL_H
