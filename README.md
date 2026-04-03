# ezflags
🚧 Simple C library to simplify argv parsing separating flags and args

> ⚠️ WIP
```
I just started the project so there is some bugs and things that are not implemented:
- glued arg like `tail -n5`
- equal arg like `--std=c++98`
- some helper function (to help build array / struct)
```

## Overview
Ezflag is here to help you parse argv in your projects. If you make a CLI app you may need to parse some
options given by the user.

Like ls or other command you can have:
```bash
ls -l           # short flag
ls -lR          # short flag group
ls -l -R        # spaced short flag
ls -recurse     # long flag
tail -n 5 file  # argument for flag
```

That can be annoying to parse all of this, so use ezflags.

## Usage
### Flag structure
```c
typedef struct s_flag
{
    char *name;
    char found;
    int params_count;
    char **params;
    char required;
} flag_t;
```
This struct help you build a list of argument that your app will need. Here some explaination on each data:
- name: the name of the argument `-v -> "v"` or `--verbose -> "verbose"` ezflags see automatically if it is a short or a long flag
- found: boolean that will be set to true if the arg is found
- params_count: maximun number of params for this flag `-n 5 -> 1` or `-o file1 file2 fileN -c ... -> -1` put -1 if you want undefined number (that stop at the next arg or the end of argv)
- params: Array that will be allocated to store params (params are take from given argv so don't free the content of both)
- required: boolean that tell if you want to get the error when not found (stop the execution here)

### Functions

#### ezflags
Important informations before use:
- the flag array must be terminated b y a sentinel `{ 0 }`

Here an example of use:
```c
#include <stdio.h>
#include "ezflags.h"

int main(int argc, char **argv)
{
    flag_t flags[] = {
        { .name = "verbose", .required = 0, .params_count = 0, .params = NULL },
        { .name = "o",       .required = 0, .params_count = -1, .params = NULL },
        { .name = "r",       .required = 0, .params_count = 1, .params = NULL },
        { 0 }
    };

    char **still_argv = NULL;

    if (ezflags(argv + 1, flags, &still_argv))
    {
        fprintf(stderr, "Error while parsing flags\n");
        return 1;
    }

    if (flags[0].found)
        printf("Verbose mode enabled\n");

    if (flags[1].found)
        printf("Output file: %s\n", flags[1].params[0]);

    return 0;
}
```

### Warning
All allocated memory is at your charge to be free, even in error case (all data are NULL terminated)
