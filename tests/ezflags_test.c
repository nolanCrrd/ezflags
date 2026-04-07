#include "ezflags.h"
#include "ezflags_internal.h"
#include <stdio.h>
#include <unistd.h>

int
main (int argc, char **argv)
{
    (void)argc;

    flag_t flags[] = {
	// 1. Le switch classique (booléen) - Pas d'argument
	{
	    .short_name = "v",
	    .long_name = "verbose",
	    .required = false,
	    .min_params = 0,
	    .max_params = 0,
	    .params = NULL,
	},

	// 2. Obligatoire avec exactement 1 argument (--output file.txt)
	{
	    .short_name = "o",
	    .long_name = "output",
	    .required = true,
	    .min_params = 1,
	    .max_params = 1,
	    .params = NULL,
	},

	// 3. Glued possible (-p8080 ou --port=8080)
	{
	    .short_name = "p",
	    .long_name = "port",
	    .required = false,
	    .min_params = 1,
	    .max_params = 1,
	    .glued_arg = true,
	    .params = NULL,
	},

	// 4. Multi-arguments infinis ET Glued (-fmain.c util.c ...)
	// Prend 'main.c' en collé, puis tout ce qui suit jusqu'au prochain
	// flag.
	{
	    .short_name = "f",
	    .long_name = "files",
	    .required = false,
	    .min_params = 1,
	    .max_params = -1,
	    .glued_arg = true,
	    .params = NULL,
	},

	// 5. Uniquement un Long Flag (pas de version courte possible)
	{
	    .short_name = NULL,
	    .long_name = "license-key",
	    .required = false,
	    .min_params = 1,
	    .max_params = 1,
	    .params = NULL,
	},

	// 6. Uniquement un Short Flag (pas de version longue)
	{
	    .short_name = "X",
	    .long_name = NULL,
	    .required = false,
	    .min_params = 0,
	    .max_params = 0,
	    .params = NULL,
	},

	// 7. Glued OBLIGATOIRE avec DEUX arguments (-c10 20)
	// Ici, le '10' est collé et le '20' est le prochain argv.
	{
	    .short_name = "c",
	    .long_name = "coord",
	    .required = true,
	    .min_params = 2,
	    .max_params = 2,
	    .glued_arg = true,
	    .params = NULL,
	},

	// 8. Optionnel gourmand ET Glued (1 à 3 arguments)
	// Exemple : -s800 600 32 (800 est collé, les autres suivent)
	{
	    .short_name = "s",
	    .long_name = "sizes",
	    .required = false,
	    .min_params = 1,
	    .max_params = 3,
	    .glued_arg = true,
	    .params = NULL,
	},

	// 9. Le flag de récursion (Switch classique)
	{
	    .short_name = "R",
	    .long_name = "Recursive",
	    .required = false,
	    .min_params = 0,
	    .max_params = 0,
	    .params = NULL,
	},

	// 10. Test de Glued Multi-Params "Lourd" (-rgb255 128 64)
	{
	    .short_name = "rgb",
	    .long_name = "color",
	    .required = false,
	    .min_params = 3,
	    .max_params = 3,
	    .glued_arg = true,
	    .params = NULL,
	},

	{ 0 } // Sentinelle
    };

    char **still_argv = NULL;

    if (ezflags (argv, (flag_t *)&flags, &still_argv))
	{
	    printf ("Error\n");
	}

    print_ezflags_result (flags, still_argv);

    return (0);
}
