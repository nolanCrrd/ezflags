#include "ezflags.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int
main (int argc, char **argv)
{
    (void)argc;

    flag_t flags[] = {
	// 1. Le switch classique (booléen) - Pas d'argument
	{
	    .short_name = 'v',
	    .long_name = "verbose",
	    .required = false,
	    .min_args = 0,
	    .max_args = 0,
	    .args = NULL,
	    .description = "test descriptiontest descriptiontest "
	                   "descriptiontest descriptiontest descriptiontest "
	                   "descriptiontest description",
	    .help_category = "Test",
	},

	// 2. Obligatoire avec exactement 1 argument (--output file.txt)
	{
	    .short_name = 'o',
	    .long_name = "output",
	    .required = true,
	    .min_args = 1,
	    .max_args = 1,
	    .args = NULL,
	    .description = "test description",
	    .help_category = "Test",
	},

	// 3. Glued possible (-p8080 ou --port=8080)
	{
	    .short_name = 'p',
	    .long_name = "port",
	    .required = false,
	    .min_args = 1,
	    .max_args = 1,
	    .glued_arg = true,
	    .args = NULL,
	    .description = "test description",
	    .help_category = "TEST2",
	    .help_args = (char *[]){ "NUMBER", NULL },
	},

	// 4. Multi-arguments infinis ET Glued (-fmain.c util.c ...)
	// Prend 'main.c' en collé, puis tout ce qui suit jusqu'au prochain
	// flag.
	{
	    .short_name = 'f',
	    .long_name = "files",
	    .required = false,
	    .min_args = 1,
	    .max_args = -1,
	    .glued_arg = true,
	    .args = NULL,
	    .description = "test description",
	    .help_category = "Other",
	},

	// 5. Uniquement un Long Flag (pas de version courte possible)
	{
	    .short_name = 0,
	    .long_name = "license-key",
	    .required = false,
	    .min_args = 1,
	    .max_args = 1,
	    .args = NULL,
	    .description = "test description",
	},

	// 6. Uniquement un Short Flag (pas de version longue)
	{
	    .short_name = 'X',
	    .long_name = NULL,
	    .required = false,
	    .min_args = 0,
	    .max_args = 0,
	    .args = NULL,
	    .description = "test description",
	},

	// 7. Glued OBLIGATOIRE avec DEUX arguments (-c10 20)
	// Ici, le '10' est collé et le '20' est le prochain argv.
	{
	    .short_name = 'c',
	    .long_name = "coord",
	    .required = true,
	    .min_args = 2,
	    .max_args = 2,
	    .glued_arg = true,
	    .args = NULL,
	    .description = "test description",
	},

	// 8. Optionnel gourmand ET Glued (1 à 3 arguments)
	// Exemple : -s800 600 32 (800 est collé, les autres suivent)
	{
	    .short_name = 's',
	    .long_name = "sizes",
	    .required = false,
	    .min_args = 1,
	    .max_args = 3,
	    .glued_arg = true,
	    .args = NULL,
	    .description = "test description",
	},

	// 9. Le flag de récursion (Switch classique)
	{
	    .short_name = 'R',
	    .long_name = "Recursive",
	    .required = false,
	    .min_args = 0,
	    .max_args = 0,
	    .args = NULL,
	    .description = "test description",
	},

	// 10. Test de Glued Multi-args "Lourd" (-rgb255 128 64)
	{
	    .short_name = 'c',
	    .long_name = "color",
	    .required = false,
	    .min_args = 3,
	    .max_args = 3,
	    .glued_arg = true,
	    .args = NULL,
	    .description = "test description",
	},

	{ 0 } // Sentinelle
    };

    if (argc == 2 && strcmp (argv[1], "--help") == 0)
	{
	    print_help (flags);
	    return (0);
	}

    char **still_argv = NULL;

    if (ezflags (argv, (flag_t *)&flags, &still_argv))
	{
	    printf ("Error\n");
	}

    print_ezflags_result (flags, still_argv);

    if (still_argv)
	{
	    free (still_argv);
	}

    for (int i = 0; flags[i].short_name || flags[i].long_name; ++i)
	{
	    free (flags[i].args);
	}

    return (0);
}
