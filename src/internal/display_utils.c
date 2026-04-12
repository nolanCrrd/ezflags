#include "ezflags.h"
#include "ezflags_internal.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t
get_longest_first_colums (flag_t flag_array[])
{
    size_t max_size = 0;
    size_t size;

    for (int i = 0; flag_array[i].short_name || flag_array[i].long_name; ++i)
	{
	    size = 2;

	    if (flag_array[i].short_name)
		{
		    // counting " -v,"
		    size += 4;
		}
	    if (flag_array[i].long_name)
		{
		    // counting " --verbose"
		    size += 3 + strlen (flag_array[i].long_name);
		}

	    if (flag_array[i].help_args)
		{
		    for (int j = 0; flag_array[i].help_args[j]; j++)
			{
			    // counting " <arg_type>" or " [arg_type]"
			    size += 3 + strlen (flag_array[i].help_args[j]);
			}
		}

	    if (size > max_size)
		{
		    max_size = size;
		}
	}
    return (max_size);
}

void
fill_first_help_column (flag_t flag, char *buf)
{
    int n = 0;

    if (flag.short_name)
	{
	    n += sprintf (buf + n, " -%c,", flag.short_name);
	}
    else
	{
	    n += sprintf (buf + n, "    ");
	}

    if (flag.long_name)
	{
	    n += sprintf (buf + n, " --%s", flag.long_name);
	}

    if (flag.help_args)
	{
	    for (size_t j = 0; flag.help_args[j]; ++j)
		{
		    if (j < flag.min_args)
			{
			    n += sprintf (buf + n, " <%s>", flag.help_args[j]);
			}
		    else
			{
			    n += sprintf (buf + n, " [%s]", flag.help_args[j]);
			}
		}
	}

    buf[n] = 0;
}

const char **
get_categories (flag_t flag_array[])
{
    const char **categories;
    size_t nb_cat = 1;

    // init categories
    categories = malloc (sizeof (char *) * 2);
    categories[0] = "Generals Options";
    categories[1] = 0;

    for (int i = 0; flag_array[i].short_name || flag_array[i].long_name; ++i)
	{
	    int j = 0;
	    while (categories[j])
		{
		    if (flag_array[i].help_category
		        && strcmp (categories[j], flag_array[i].help_category)
		               == 0)
			{
			    break;
			}
		    ++j;
		}

	    // Append help_category if not found
	    if (categories[j] != NULL)
		{
		    continue;
		}

	    categories = realloc (categories, sizeof (char *) * (nb_cat + 2));

	    categories[nb_cat] = flag_array[i].help_category;
	    ++nb_cat;
	    categories[nb_cat] = NULL;
	}

    return (categories);
}
