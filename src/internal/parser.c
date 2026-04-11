#include "ezflags.h"
#include "ezflags_internal.h"
#include <stdbool.h>
#include <string.h>
#include <strings.h>

arg_result_t
fill_flag (char *flag, arg_type flag_type, char **args, flag_t flag_array[])
{
    // find the equal index
    char *equal = strchr (flag, '=');
    int equal_position = -1;

    if (equal != NULL)
	{
	    equal_position = equal - flag;
	    // equal cannot be the fist char of a flag
	    if (equal_position == 0)
		{
		    return ((arg_result_t){ .status = IMPOSSIBLE_FLAG,
		                            .information = 1 });
		}
	}

    int index = get_flag_index (flag, flag_type, flag_array, equal_position);

    // Error handeling
    if (index == -1)
	{
	    return (
	        (arg_result_t){ .status = FLAG_NOT_FOUND, .information = 1 });
	}
    if (flag_array[index].found)
	{
	    return (
	        (arg_result_t){ .status = REPTITIVE_FLAG, .information = 1 });
	}
    if (equal_position > 0 && flag_array[index].glued_arg == false)
	{
	    return (
	        (arg_result_t){ .status = CANNOT_BE_GLUED, .information = 1 });
	}

    flag_array[index].found = true;
    int filled_args = fill_flag_args (&flag_array[index], flag, flag_type,
                                      equal_position, args);
    int to_skip = filled_args;

    if (flag_array[index].glued_arg && filled_args >= 0)
	{
	    filled_args += 1;
	    if (flag_type == SHORT_FLAG_GROUP)
		{
		    // If its a group flag -s255 but 255 s is glued then all
		    // flag afeter become args
		    to_skip = -filled_args;
		    // Notify the function before to skip other by passing the
		    // filled arg as negative value
		}
	}

    if (filled_args < flag_array[index].min_args)
	{
	    return ((arg_result_t){ .status = FLAG_ARGUMENT_NOT_FOUND,
	                            .information = filled_args });
	}

    // flag is in middle of a short group and the flag wait an arg -> error
    if (flag_type == SHORT_FLAG_GROUP && flag_array[index].glued_arg == false
        && (flag_array[index].min_args > 0 || filled_args > 0))
	{
	    return ((arg_result_t){ .status = FLAG_GROUP_INVALID_ARGUMENT,
	                            .information = filled_args });
	}

    return ((arg_result_t){ .status = SUCCESS, .information = to_skip });
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

		    // Basic error that stop
		    if (fill_flag_result.status == SYSTEM_ERROR
		        || fill_flag_result.status == FLAG_ARGUMENT_NOT_FOUND
		        || fill_flag_result.status
		               == FLAG_GROUP_INVALID_ARGUMENT)
			{
			    return (fill_flag_result);
			}

		    if (fill_flag_result.information < 0)
			{
			    // If filled_args negative then that a glued arg
			    // that take all remaining char of the group
			    to_skip = -fill_flag_result.information - 1;
			    break;
			}

		    // Composed error that stop (ex: -Rz and z not found)
		    if (type == SHORT_FLAG_GROUP
		        && fill_flag_result.status == FLAG_NOT_FOUND)
			{
			    return ((arg_result_t){
			        .status = FLAG_GROUP_NOT_FOUND,
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
