#include "ezflags.h"
#include "ezflags_internal.h"
#include <stdlib.h>
#include <string.h>
#include <strings.h>

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
			    // -- then all args after are not flags
			    return END_FLAG;
			}
		    // --... its a long flag
		    return LONG_FLAG;
		}
	    if (arg[1] && arg[2])
		{
		    // -lR = -l -R
		    return SHORT_FLAG_GROUP;
		}
	    // -l juste short flag
	    return SHORT_FLAG;
	default:
	    // if not flag make it just an arg
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

/**
 * @brief Return the index of the corresponding flag in the flag array
 *
 * @param flag flag to find
 * @param flag_type kind of flag
 * @param flag_array array of flag that the user want
 * @return index where the flag was found / -1 if not found
 */
int
get_flag_index (char *flag, arg_type flag_type, flag_t flag_array[])
{
    if (flag_type == SHORT_FLAG || flag_type == SHORT_FLAG_GROUP)
	{
	    for (int i = 0;
	         flag_array[i].short_name || flag_array[i].long_name; ++i)
		{
		    if (flag_array[i].short_name
		        && strncmp (flag, flag_array[i].short_name, 1) == 0)
			{
			    return (i);
			}
		}
	}
    else
	{
	    for (int i = 0;
	         flag_array[i].short_name || flag_array[i].long_name; ++i)
		{
		    if (flag_array[i].long_name
		        && strcmp (flag, flag_array[i].long_name) == 0)
			{
			    return (i);
			}
		}
	}
    return (-1);
}

/**
 * @brief Fill the given flal with arguments that are in next args
 *
 * @param flag flag to fill
 * @param args next args
 * @return number of filled args
 */
int
fill_spaced_arg (flag_t *flag, char **args)
{
    int j = 0;
    while (args[j] && (flag->max_params == -1 || j < flag->max_params))
	{
	    // if other args are a flag then stop
	    if (get_arg_type (args[j]) != ARGUMENT)
		{
		    break;
		}

	    str_array_append (args[j], &flag->params, j);
	    ++j;
	}

    return (j);
}

arg_result_t
fill_flag (char *flag, arg_type flag_type, char **args, flag_t flag_array[])
{
    int index = get_flag_index (flag, flag_type, flag_array);

    if (index == -1 || flag_array[index].found)
	{
	    return (
	        (arg_result_t){ .status = FLAG_NOT_FOUND, .information = 1 });
	}

    flag_array[index].found = true;
    int filled_args = fill_spaced_arg (&flag_array[index], args);

    if (filled_args < flag_array[index].min_params)
	{
	    return ((arg_result_t){ .status = FLAG_ARGUMENT_NOT_FOUND,
	                            .information = filled_args });
	}

    return (
        (arg_result_t){ .status = SUCCESS, .information = filled_args + 1 });
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

    arg_result_t fill_flag_result;
    if (type == LONG_FLAG)
	{
	    fill_flag_result
	        = fill_flag (args[0] + 2, type, args + 1, flag_array);
	}
    else
	{
	    int to_skip = 0;
	    int i = 0;
	    while (args[0][i + 1])
		{
		    fill_flag_result = fill_flag (&args[0][i + 1], type,
		                                  args + 1, flag_array);

		    if (fill_flag_result.status == SYSTEM_ERROR
		        || fill_flag_result.status == FLAG_ARGUMENT_NOT_FOUND
		        || (type == SHORT_FLAG_GROUP
		            && fill_flag_result.status == FLAG_NOT_FOUND)
		        || (args[0][i + 2]
		            && fill_flag_result.information > 1))
			{
			    return ((arg_result_t){
			        .status = ERROR,
			        .information = fill_flag_result.information });
			}

		    to_skip += fill_flag_result.information;

		    ++i;
		}
	    return ((arg_result_t){ .status = fill_flag_result.status,
	                            .information = to_skip });
	}

    return (fill_flag_result);
}

int
check_all_required (flag_t flag_array[])
{
    for (int i = 0; flag_array[i].short_name; ++i)
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
    int still_args_found = false;

    while (args[0])
	{
	    arg_result_t arg_status = parse_next_arg (args, flag_array);

	    if (arg_status.status == ERROR || arg_status.status == SYSTEM_ERROR
	        || (get_arg_type (args[0]) == SHORT_FLAG_GROUP
	            && arg_status.status == FLAG_NOT_FOUND))
		{
		    return (1);
		}

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
	    else if (arg_status.status == NO_MORE_FLAG)
		{
		    if (still_argv)
			{
			    int i = 1;
			    while (args[i])
				{
				    str_array_append (args[i], still_argv,
				                      still_args_found);
				    still_args_found += 1;
				    ++i;
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
