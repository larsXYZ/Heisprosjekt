#include <stdio.h>
#include "timer.h"


void timehandler_init(struct Timehandler *timehandler)
{
	timehandler->timeout = -1;
}

void timehandler_delay(struct Timehandler *timehandler, int delay)
{
	time_t current_time;
	current_time = time(NULL);
	timehandler->timeout = current_time + delay;

}

int timehandler_is_time_up(struct Timehandler *timehandler)
{
	if (timehandler->timeout == -1) return -1;
	else return (time(NULL) >= timehandler->timeout);
}
