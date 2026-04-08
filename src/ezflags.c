#include "ezflags.h"
#include "ezflags_internal.h"
#include <stdbool.h>
#include <stdio.h>
#include <strings.h>

void
print_help (flag_t flag_array[])
{
    int size = get_longest_first_colums (flag_array);

    for (int i = 0; flag_array[i].short_name || flag_array[i].long_name; ++i)
	{
	    int n = 0;
	    char column[size];

	    if (flag_array[i].short_name)
		{
		    n += sprintf (column + n, "-%c,",
		                  flag_array[i].short_name);
		}
	    else
		{
		    n += sprintf (column + n, "   ");
		}

	    if (flag_array[i].long_name)
		{
		    n += sprintf (column + n, " --%s",
		                  flag_array[i].long_name);
		}

	    if (flag_array[i].args_help)
		{
		    for (int j = 0; flag_array[i].args_help[j]; ++j)
			{
			    if (j < flag_array[i].min_args)
				{
				    n += sprintf (column + n, " <%s>",
				                  flag_array[i].args_help[j]);
				}
			    else
				{
				    n += sprintf (column + n, " [%s]",
				                  flag_array[i].args_help[j]);
				}
			}
		}

	    column[n] = 0;

	    printf ("%-*s\t%s\n", size + 2, column,
	            flag_array[i].description ? flag_array[i].description
	                                      : "");
	}
}

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
