#include "ezflags.h"
#include <stdio.h>

void
print_flag (flag_t flag)
{
    printf ("{\n \tshort_name: %c\n", flag.short_name);
    printf ("\tlong_name: %s\n", flag.long_name);

    printf ("\tfound: %i\n", flag.found);
    printf ("\trequired: %i\n", flag.required);

    printf ("\tmin_args: %i\n", flag.min_args);
    printf ("\tmax_args: %i\n", flag.max_args);
    printf ("\tglued_param: %i\n", flag.glued_arg);

    if (flag.args)
	{
	    printf ("\targs: [\n");
	    for (int i = 0; flag.args[i]; ++i)
		{
		    printf ("\t\t%s,\n", flag.args[i]);
		}
	    printf ("\t]\n");
	}

    if (flag.help_args)
	{
	    printf ("\targs_help: [\n");
	    for (int i = 0; flag.help_args[i]; ++i)
		{
		    printf ("\t\t%s,\n", flag.help_args[i]);
		}
	    printf ("\t]\n");
	}

    printf ("\tdescription: %s\n", flag.description);
    printf ("\thelp_category: %s\n", flag.help_category);

    printf ("}\n");
}

void
print_ezflags_result (flag_t flags[], char **still_argv)
{
    for (int i = 0; flags[i].short_name || flags[i].long_name; ++i)
	{
	    print_flag (flags[i]);
	}

    printf ("[");
    if (still_argv)
	{

	    for (int i = 0; still_argv[i]; ++i)
		{
		    printf ("%s", still_argv[i]);
		    if (still_argv[i + 1])
			{
			    printf (",");
			}
		}
	}
    printf ("]");
}
