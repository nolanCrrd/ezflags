#include "ezflags.h"
#include "ezflags_internal.h"
#include <stdlib.h>
#include <string.h>

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
