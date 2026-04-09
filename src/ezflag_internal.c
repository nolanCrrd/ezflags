#include "ezflags.h"
#include "ezflags_internal.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

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

int
get_flag_index (char *flag, arg_type flag_type, flag_t flag_array[],
                int equal_position)
{
    if (flag_type == SHORT_FLAG || flag_type == SHORT_FLAG_GROUP)
	{
	    // If short flag ignor equal just compare first flag's letter
	    for (int i = 0;
	         flag_array[i].short_name || flag_array[i].long_name; ++i)
		{
		    if (flag_array[i].short_name
		        && flag[0] == flag_array[i].short_name)
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
		    if (flag_array[i].long_name)
			{
			    // If equal found then compare without it
			    if (equal_position != -1
			        && strncmp (flag, flag_array[i].long_name,
			                    equal_position)
			               == 0)
				{
				    return (i);
				}
			    else if (strcmp (flag, flag_array[i].long_name)
			             == 0)
				{
				    return (i);
				}
			}
		}
	}
    return (-1);
}

int
fill_spaced_arg (flag_t *flag, char **args, int size)
{
    int j = 0;
    while (args[j] && (flag->max_args == -1 || size + j < flag->max_args))
	{
	    // if other args are a flag then stop
	    if (get_arg_type (args[j]) != ARGUMENT)
		{
		    break;
		}

	    str_array_append (args[j], &flag->args, size + j);
	    ++j;
	}
    return (size + j);
}

int
fill_glued_arg (flag_t *flag, arg_type flag_type, char *flag_str,
                int equal_position)
{
    if (flag_type == LONG_FLAG)
	{
	    if (equal_position > 0)
		{
		    char *arg = &flag_str[equal_position];
		    str_array_append (arg, &flag->args, 0);
		    return (0);
		}
	}
    else
	{
	    char *arg = &flag_str[1];
	    if (arg[0])
		{
		    str_array_append (arg, &flag->args, 0);
		    return (0);
		}
	}
    return (-1);
}

int
fill_flag_args (flag_t *flag, char *flag_str, arg_type flag_type,
                int equal_position, char **args)
{
    int filled_args = 0;
    if (flag->glued_arg)
	{
	    filled_args = fill_glued_arg (flag, flag_type, flag_str,
	                                  equal_position + 1);
	    if (filled_args + 1 < flag->max_args || flag->max_args == -1)
		{
		    int tmp = filled_args + 1;
		    filled_args = fill_spaced_arg (flag, args, tmp);
		    filled_args -= (tmp == 1);
		}
	}
    else
	{
	    filled_args = fill_spaced_arg (flag, args, 0);
	}

    return (filled_args);
}

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
		    to_skip = -1;
		}
	}

    if (filled_args < flag_array[index].min_args)
	{
	    return ((arg_result_t){ .status = FLAG_ARGUMENT_NOT_FOUND,
	                            .information = filled_args });
	}

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

		    if (fill_flag_result.information == -1)
			{
			    to_skip = 1;
			    break;
			}

		    // Basic error that stop
		    if (fill_flag_result.status == SYSTEM_ERROR
		        || fill_flag_result.status == FLAG_ARGUMENT_NOT_FOUND
		        || fill_flag_result.status
		               == FLAG_GROUP_INVALID_ARGUMENT)
			{
			    return (fill_flag_result);
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
is_important_error (ezflag_status status)
{
    const ezflag_status important_error[] = { ERROR,
	                                      SYSTEM_ERROR,
	                                      REPTITIVE_FLAG,
	                                      CANNOT_BE_GLUED,
	                                      FLAG_GROUP_INVALID_ARGUMENT,
	                                      FLAG_ARGUMENT_NOT_FOUND,
	                                      FLAG_GROUP_NOT_FOUND,
	                                      IMPOSSIBLE_FLAG };
    for (int i = 0; i < 7; ++i)
	{
	    if (status == important_error[i])
		{
		    return (1);
		}
	}
    return (0);
}

int
get_longest_first_colums (flag_t flag_array[])
{
    int max_size = 0;
    int size;

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
	    for (int j = 0; flag.help_args[j]; ++j)
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

char **
get_categories (flag_t flag_array[])
{
    char **categories;
    int nb_cat = 1;

    // init categories
    categories = malloc (sizeof (char *) * 2);
    categories[0] = "Other";
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
