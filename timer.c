#include <stdio.h>
#include "timer.h"

void timehandler_delay(struct Timehandler *timehandler, int delay)
{
	time_t current_time;
	current_time = time(NULL);
	timehandler->timeout = current_time + delay;

}

void timehandler_is_time_up(struct Timehandler *timehandler) {
	return (time(NULL) >= timehandler->timeout);
}