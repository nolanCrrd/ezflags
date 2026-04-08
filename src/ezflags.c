#include "ezflags.h"
#include "ezflags_internal.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>

void
print_help (flag_t flag_array[])
{
    int first_col_size = get_longest_first_colums (flag_array) + 2;

    for (int i = 0; flag_array[i].short_name || flag_array[i].long_name; ++i)
	{
	    char column[first_col_size];

	    fill_first_help_column (flag_array[i], column);

	    printf ("%-*s\t", first_col_size, column);

	    if (flag_array[i].description)
		{
		    int max_desc_width = MAX_WIDTH - first_col_size - 1;

		    char *desc = flag_array[i].description;

		    while (desc && *desc)
			{
			    int len = strlen (desc);

			    if (len < max_desc_width)
				{
				    printf ("%s\n", desc);
				    break;
				}

			    int break_at = max_desc_width;
			    while (break_at > 0 && desc[break_at] != ' ')
				{
				    --break_at;
				}

			    if (break_at == 0)
				{
				    break_at = max_desc_width;
				}

			    printf ("%.*s\n", break_at, desc);

			    printf ("%*s\t ", first_col_size, "");

			    desc += break_at;
			    while (desc[0] == ' ')
				{
				    desc += 1;
				}
			}
		}
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
