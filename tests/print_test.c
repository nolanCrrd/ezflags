#include "ezflags.h"
#include "ezflags_internal.h"
#include "test.h"
#include <stdio.h>

void
print_flag_test ()
{
    flag_t flag = { .name = "r", .found = 0, .params_count = 1, NULL };

    print_flag (flag);
}

void
print_test ()
{
    print_flag_test ();
}
