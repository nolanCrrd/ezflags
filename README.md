# ezflags
🚧 Simple C library to simplify argv parsing separating options and args

> ⚠️ WIP
```
I just started the project so there may be some bugs and function that i've not implemented yet:
- May be some bug
```

## Overview
Ezflag is here to help you parse argv in your projects. If you make a CLI app you may need to parse some
options given by the user.

Like ls or other command you can have:
```bash
command -l                          # short flag
command -lR                         # short flag group
command -l -R                       # spaced short flag
command --recurse                   # long flag
command -n 5 file                   # argument for flag
command -c10 20                     # glued short flag
command --files=file1               # glued long flag
command --files=file1 file2 file3   # glued arg list
```

That can be annoying to parse all of this, so use ezflags to enjoy it.

## Usage
### Example
Yiu can see en example of simple usage in my repository [github.com/nolancrrd/ft_ls](ft_ls).

### Flag structure
```c
typedef struct s_flag
{
    char *short_name;
    char *long_name;

    int min_args;
    int max_args;
    char **args;

    bool glued_arg;
    bool required;
    bool found;

    char *description;
    char *help_category;
    char **help_args;
} flag_t;
```
This struct help you build a list of argument that your app will need. Here some explaination on each data:
- short_name: the short name of the flag of the argument `-v -> "v"
- long_name: the long name of the flag of the argument `--verbose -> "verbose"
- min_args: number of minimum args required -> error if not respected
- max_args: number of max args required
- args: Array that will be allocated to store args (args are take from given argv so don't free the content of both)
- glued_arg: boolean to know if the first arg can be glued to the flag
- required: boolean that tell if you want to get the error when not found (stop the execution here)
- found: boolean that will be set to true if the arg is found
- description: description of the given flag (not needed if you not use the print help function)
- help_category: help you to organise --help by categories
- help_args: help you to specify information about flag's args like `--color <NUMBER> <NUMBER> <NUMBER>` or `--size <NUMBER> [HEIGHT] [DEPTH]`

### Helper macro
It can be annoying to build the flag array from scratch by hand (it good to understand what you'r doing but in big project do not make this mistake). I created
Macro to help you here is how to use it:

#### EZ_HELP_ARGS(...)
This help you to build the `help_args` array without aving a weird cast and it place the NULL end for you
```c
char *[] array = EZ_HELP_ARGS("NUMBER", "NUMBER");
// array = (char *[]){"NUMBER", "NUMBER", NULL}
```

#### EZ_FLAG_BOOL
The most used flag in cli application is just a boolean to tell to active one thing or not here how to simply build a flag
like this:
```c
flag_t flag = EZ_FLAG_BOOL('v', "verbose", "some description", "VIEW");
//Equivalent to:
flag_t flag2 = {
    .short_name = 'v',
    .long_name = "verbose",
    .description = "some description",
    .min_args = 0,
    .max_args = 0,
    .help_args = NULL,
    .help_category = "VIEW",
    .glued_arg = false,
    .required = false,
};
```
If you will not use help function you can give NULL 3 and 4 arg

#### EZ_FLAG_OPT
The other case that you will have often is the case where you want to give an argument to your flag:
```c
flag_t = EZ_FLAG_OPT('v', "variant", false, NULL, NULL, "TEXT")
// Equivalent to:
flag_t flag2 = {
    .short_name = 'v',
    .long_name = "variant",
    .description = NULL,
    .min_args = 1,
    .max_args = 1,
    .help_args = EZ_HELP_ARGS("TEXT"),
    .help_category = NULL,
    .glued_arg = true,
    .required = false,
};
```
Note that glued is true by default to handle most of user action.

#### EZ_FLAG_FULL
This macro is for specific case to mke your parser act as you want:
```c
flag_t = EZ_FLAG_FULL('s', "size", 1, 3, true, true, NULL, NULL, false, EZ_HELP_ARGS("NUMBER", "NUMBER", "NUMBER"));
// Equivalent to:
flag_t flag2 = {
    .short_name = 's',
    .long_name = "size",
    .min_args = 1,
    .max_args = 3,
    .required = true,
    .glued_arg = true,
    .description = NULL,
    .help_category = NULL,
    .help_args = EZ_HELP_ARGS("NUMBER", "NUMBER", "NUMBER"),
};
```
Here you have to specify all aspect of the flag (often used is )

### Functions

#### ezflags
Important informations before use:
- the flag array must be terminated b y a sentinel `{ 0 }`

Here an example of use:

```c
#include "ezflags.h"
#include "ezflags_internal.h"
#include <stdio.h>
#include <unistd.h>

int
main (int argc, char **argv)
{
    (void)argc;

    flag_t flags[] = {
        ...,
        {0}
    };

    char **still_argv = NULL;

    if (ezflags (argv, (flag_t *)&flags, &still_argv))
	{
	    printf ("Error\n");
	}

    print_ezflags_result (flags, still_argv);

    return (0);
}
```
I suggest you to use the macro to build the array that allow you to go from 150 lines of code to 40/50.

### Error handling
You can know what kind of error make the parsing stop with this structure (some error are not extremly precise but at least you can find why by yourself ;) ):

```typedef enum e_status
{
    SUCCESS,
    MISSING_FLAG,
    FLAG_GROUP_NOT_FOUND,
    FLAG_GROUP_INVALID_ARGUMENT,
    REPTITIVE_FLAG,
    FLAG_ARGUMENT_NOT_FOUND,
    CANNOT_BE_GLUED,
    IMPOSSIBLE_FLAG,
    SYSTEM_ERROR,
    ERROR,

} ezflag_status;
c
```

Here why all error can be returned:

- `SUCCESS` : It talk by itself
- `MISSING_FLAG` : If at least on required flag is not found
- `FLAG_GROUP_NOT_FOUND` : Considering "-r -z" valid even if z is not found but with "-rz" the error is return
- `FLAG_GOUP_INVALID_ARGUMENT` : Considering -a and -b with a wating args "-ab" is not valid flag with argument need to be at the end of a group
- `REPETITIVE_FLAG` : If a flag is found multiple times this error is returned
- `FLAG_ARGUMENT_NOT_FOUND` : If the minimum argument number of a flag is not meet
- `CANNOT_BE_GLUED` : If the arg is a flag with an equal then this error is returned
- `IMPOSSIBLE_FLAG` : If the arg start with equal "-=" or "--="
- `SYSTEM_ERROR` : If an allocation fail
- `ERROR` : Other kind of errors

### Warning
All allocated memory is at your charge to be free, even in error case (all data are NULL terminated)
