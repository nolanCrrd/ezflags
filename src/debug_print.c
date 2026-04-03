#include "ezflags.h"
#include <stdio.h>

void
print_flag (flag_t flag)
{
    printf ("{\n \tname: %s\n", flag.name);

    printf ("\tfound: %i\n", flag.found);

    printf ("\tparams_count: %i\n", flag.params_count);

    if (flag.params)
	{
	    printf ("\tparams: [\n");
	    for (int i = 0; i < flag.params_count; ++i)
		{
		    printf ("\t\t%s,\n", flag.params[i]);
		}
	    printf ("\t]\n");
	}

    printf ("}\n");
}
