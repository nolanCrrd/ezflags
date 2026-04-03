#include "ezflags.h"
#include "test.h"
#include <stdio.h>

void
print_flag_test ()
{
    flag_t flag = { .name = "r", .found = 0, .params_count = 1, NULL };

    print_flag (flag);
}

void
print_ezflags_result (flag_t flags[], char **still_argv)
{
    for (int i = 0; flags[i].name; ++i)
	{
	    print_flag (flags[i]);
	}

    printf ("[");
    for (int i = 0; still_argv[i]; ++i)
	{
	    printf ("%s", still_argv[i]);
	    if (still_argv[i + 1])
		{
		    printf (",");
		}
	}
    printf ("]");
}

void
print_test ()
{
    print_flag_test ();
}
