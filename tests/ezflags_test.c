#include "ezflags.h"
#include "ezflags_internal.h"
#include <stdio.h>
#include <unistd.h>

int
main (void)
{
    // print_test ();

    const char *args[] = { "--verbose", "--help", 0 };

    flag_t flags[] = { (flag_t){
	                   .required = 0,
	                   .params_count = 0,
	                   .name = "R",
	                   .params = NULL,
	               },
	               (flag_t){
	                   .required = 0,
	                   .params_count = 1,
	                   .name = "r",
	                   .params = NULL,
	               },

	               (flag_t){
	                   .required = 0,
	                   .params_count = 0,
	                   .name = "verbose",
	                   .params = NULL,
	               },

	               (flag_t){
	                   .required = 0,
	                   .params_count = -1,
	                   .name = "o",
	                   .params = NULL,
	               },
	               { 0 } };

    char **still_argv = NULL;

    if (ezflags ((char **)&args, (flag_t *)&flags, &still_argv))
	{
	    printf ("Error\n");
	}

    return (0);
}
