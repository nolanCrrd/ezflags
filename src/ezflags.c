#include "ezflags.h"
#include "ezflags_internal.h"
#include <stdbool.h>
#include <strings.h>

ezflag_status
ezflags (char **args, flag_t flag_array[], char ***still_argv)
{
    int still_args_found = false;

    while (args[0])
	{
	    arg_result_t arg_status = parse_next_arg (args, flag_array);

	    if (is_important_error (arg_status.status))
		{
		    return (arg_status.status);
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
		    args = &args[arg_status.information + 1];
		}
	}

    if (check_all_required (flag_array))
	{
	    return (MISSING_FLAG);
	}
    return (SUCCESS);
}
