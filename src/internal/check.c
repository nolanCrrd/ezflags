#include "ezflags.h"
#include "ezflags_internal.h"

int
check_all_required (flag_t flag_array[])
{
    for (int i = 0; flag_array[i].short_name; ++i)
	{
	    if (!flag_array[i].found && flag_array[i].required)
		{
		    return (1);
		}
	}
    return (0);
}
