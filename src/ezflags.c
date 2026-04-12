#include "ezflags.h"
#include "ezflags_internal.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

void
print_help (flag_t flag_array[])
{
    size_t first_col_size = get_longest_first_colums (flag_array);
    const char **categories = get_categories (flag_array);

    // Print per category
    for (int i = 0; categories[i]; ++i)
	{
	    // Empty line if not first category (better lisibility)
	    if (i != 0)
		{
		    printf ("\n");
		}
	    printf ("%s\n", categories[i]);

	    for (int j = 0;
	         flag_array[j].short_name || flag_array[j].long_name; ++j)
		{
		    // To avoid segfault need to treat NULL in another way
		    if (flag_array[j].help_category == NULL
		        && strcmp (categories[i], "Other") != 0)
			{
			    continue;
			}

		    if ((flag_array[j].help_category == NULL
		         && strcmp (categories[i], "Other") == 0)
		        || strcmp (categories[i], flag_array[j].help_category)
		               == 0)
			{
			    char column[first_col_size];

			    fill_first_help_column (flag_array[j], column);

			    // Print first column
			    printf ("%-*s\t", (int)first_col_size, column);

			    if (flag_array[j].description)
				{
				    size_t max_desc_width = (size_t)MAX_WIDTH
				                            - first_col_size
				                            - 1;

				    char *desc = flag_array[j].description;

				    while (desc && *desc)
					{
					    size_t len = strlen (desc);

					    if (len < max_desc_width)
						{
						    printf ("%s\n", desc);
						    break;
						}

					    // Find the break point to match
					    // max_width
					    size_t break_at = max_desc_width;
					    while (break_at > 0
					           && desc[break_at] != ' ')
						{
						    --break_at;
						}

					    if (break_at == 0)
						{
						    break_at = max_desc_width;
						}

					    printf ("%.*s\n", (int)break_at,
					            desc);

					    printf ("%*s\t ",
					            (int)first_col_size, "");

					    // Skip whitespaces at beginning
					    desc += break_at;
					    while (desc[0] == ' ')
						{
						    desc += 1;
						}
					}
				}
			}
		}
	}
    free (categories);
}

ezflag_status
ezflags (char **args, flag_t flag_array[], char ***still_argv)
{
    size_t still_args_found = 0;

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
