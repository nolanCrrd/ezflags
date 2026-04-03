#include "ezflags.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Return the type of the given argument of argv
 *
 * @param arg argument to check
 * @return arg_typ enum that contain the response
 */
arg_type
get_arg_type (char *arg)
{
    switch (arg[0])
	{
	case '-':
	    if (arg[1] == '-')
		{
		    if (!arg[2])
			{
			    return END_FLAG;
			}
		    return STRING_FLAG;
		}
	    if (arg[1] && arg[2])
		{
		    return SINGLE_GROUP_FLAG;
		}
	    return SINGLE_CHAR_FLAG;
	default:
	    return ARGUMENT;
	}
}

/**
 * @brief Append a string to an array based on array_size
 *
 * @param str string to append
 * @param array_ptr pointer to the array that need to be updated
 * @param array_size initial size of the array
 * @return 0 if success / 1 if realloc error
 */
int
str_array_append (char *str, char ***array_ptr, int array_size)
{
    *array_ptr = realloc (*array_ptr, sizeof (char *) * (array_size + 2));
    if (!(*array_ptr))
	{
	    return (1);
	}

    (*array_ptr)[array_size] = str;
    (*array_ptr)[array_size + 1] = NULL;

    return (0);
}

arg_result_t
fill_flag (char *flag, char **args, flag_t flag_array[])
{
    int i = 0;
    while (flag_array[i].name && strcmp (flag, flag_array[i].name) != 0)
	{
	    ++i;
	}
    if (!flag_array[i].name)
	{
	    return (
	        (arg_result_t){ .status = FLAG_NOT_FOUND, .information = 0 });
	}

    flag_array[i].found = 1;
    int j = 0;
    if (flag_array[i].params_count > 0)
	{
	    while (args[j] && j < flag_array[i].params_count)
		{
		    if (get_arg_type (args[j]) != ARGUMENT)
			{
			    break;
			}
		    str_array_append (args[j], &flag_array[i].params, j);
		    ++j;
		}

	    if (j < flag_array[i].params_count)
		{
		    return ((arg_result_t){ .status = FLAG_ARGUMENT_NOT_FOUND,
		                            .information = j });
		}
	}

    return ((arg_result_t){ .status = SUCCESS, .information = j + 1 });
}

arg_result_t
parse_next_arg (char **args, flag_t flag_array[])
{
    arg_type type = get_arg_type (args[0]);

    if (type == ARGUMENT)
	{
	    return ((arg_result_t){ .status = NOT_A_FLAG, .information = 0 });
	}

    if (type == END_FLAG)
	{
	    return (
	        (arg_result_t){ .status = NO_MORE_FLAG, .information = 0 });
	}

    char *flag;
    if (type == STRING_FLAG)
	{
	    flag = args[0] + 2;
	}
    else
	{
	    flag = args[0] + 1;
	}

    arg_result_t fill_flag_result = fill_flag (flag, args + 1, flag_array);

    return (fill_flag_result);
}

int
check_all_required (flag_t flag_array[])
{
    for (int i = 0; flag_array[i].name; ++i)
	{
	    if (!flag_array[i].found && flag_array[i].required)
		{
		    return (1);
		}
	}
    return (0);
}

int
ezflags (char **args, flag_t flag_array[], char ***still_argv)
{
    int still_args_found = 0;

    while (args[0])
	{
	    arg_result_t arg_status = parse_next_arg (args, flag_array);

	    if (arg_status.status == FLAG_NOT_FOUND
	        || arg_status.status == NOT_A_FLAG)
		{
		    if (still_argv)
			{
			    str_array_append (args[0], still_argv,
			                      still_args_found);
			}
		    args = &args[1];
		    still_args_found += 1;
		}
	    else if (arg_status.status == SYSTEM_ERROR
	             || arg_status.status == FLAG_ARGUMENT_NOT_FOUND)
		{
		    return (1);
		}
	    else if (arg_status.status == NO_MORE_FLAG)
		{
		    if (still_argv)
			{
			    int i = 0;
			    while (args[i])
				{
				    str_array_append (args[i], still_argv,
				                      still_args_found);
				    still_args_found += 1;
				}

			    args = &args[i];
			}
		}
	    else
		{
		    args = &args[arg_status.information];
		}
	}

    return (check_all_required (flag_array));
}
