#include "ezflags.h"
#include "ezflags_internal.h"

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
		    if (tmp == 0 && filled_args == 0)
			{
			    filled_args = -1;
			}
		}
	}
    else
	{
	    filled_args = fill_spaced_arg (flag, args, 0);
	}

    return (filled_args);
}
