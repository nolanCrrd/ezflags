#include "ezflags.h"
#include <stdio.h>
#include <unistd.h>

int
main (void)
{
    // print_test ();

    const char *args[]
        = { "-r", "-R", "1", "hello-world.txt", "c-aca.txt", 0 };

    flag_t flags[] = { (flag_t){
	                   .required = 0,
	                   .params_count = 1,
	                   .name = "R",
	                   .params = NULL,
	               },
	               (flag_t){
	                   .required = 1,
	                   .params_count = 0,
	                   .name = "r",
	                   .params = NULL,
	               },
	               { 0 } };

    char **still_argv = NULL;

    ezflags ((char **)&args, (flag_t *)&flags, &still_argv);

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

    return (0);
}
