#include "ezflags.h"
#include <stdio.h>

void
print_flag (flag_t flag)
{
    printf ("{\n \tshort_name: %s\n", flag.short_name);
    printf ("\tlong_name: %s\n", flag.long_name);

    printf ("\tfound: %i\n", flag.found);

    printf ("\tmin_params: %i\n", flag.min_params);
    printf ("\tmax_params: %i\n", flag.max_params);

    if (flag.params)
	{
	    printf ("\tparams: [\n");
	    for (int i = 0; flag.params[i]; ++i)
		{
		    printf ("\t\t%s,\n", flag.params[i]);
		}
	    printf ("\t]\n");
	}

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
