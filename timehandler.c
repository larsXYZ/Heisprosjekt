#include "timehandler.h"
#include <stdio.h>

/* For function descriptions see timehandler.h */

void timehandler_init(struct Timehandler *target)
{
	target->timeout = -1;
}

void timehandler_delay(struct Timehandler *target, int delay)
{	 
	target->timeout = (int)clock() + delay*CLOCKS_PER_SEC;
}

int timehandler_is_time_up(struct Timehandler *target)
{
	return ((int)clock() >= target->timeout);
}
