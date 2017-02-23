#include <stdio.h>
#include "timer.h"

void timehandler_delay(struct Timehandler *next_timeout, int delay)
{
	time_t current_time;
	current_time = time(NULL);
	next_timeout->timeout = current_time + delay;

}