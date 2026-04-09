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
	// 1. Switch classique (BOOL)
	EZ_FLAG_BOOL ('v', "verbose",
	              "test descriptiontest descriptiontest descriptiontest "
	              "description",
	              "Test"),

	// 2. Obligatoire avec 1 argument (FULL car requis)
	EZ_FLAG_FULL ('o', "output", 1, 1, false, true, "test description",
	              "Test", NULL),

	// 3. Glued possible 1 argument (OPT)
	EZ_FLAG_OPT ('p', "port", "test description", "TEST2", false,
	             "NUMBER"),

	// 4. Multi-arguments infinis (FULL)
	EZ_FLAG_FULL ('f', "files", 1, -1, true, false, "test description",
	              "Other", NULL),

	// 5. Uniquement Long Flag (OPT)
	EZ_FLAG_OPT (0, "license-key", "test description", NULL, false, "KEY"),

	// 6. Uniquement Short Flag (BOOL)
	EZ_FLAG_BOOL ('X', NULL, "test description", NULL),

	// 7. Glued OBLIGATOIRE avec DEUX arguments (FULL)
	EZ_FLAG_FULL ('c', "coord", 2, 2, true, true, "test description", NULL,
	              EZ_HELP_ARGS ("X", "Y")),

	// 8. Optionnel gourmand 1 à 3 arguments (FULL)
	EZ_FLAG_FULL ('s', "sizes", 1, 4, true, false, "test description",
	              NULL, EZ_HELP_ARGS ("WIDTH", "HEIGHT", "DEPTH")),

	// 9. Flag de récursion (BOOL)
	EZ_FLAG_BOOL ('R', "Recursive", "test description", NULL),

	// 10. Glued Multi-args 3 paramètres (FULL)
	EZ_FLAG_FULL ('C', "color", 3, 3, true, false, "test description",
	              NULL, EZ_HELP_ARGS ("R", "G", "B")),

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
