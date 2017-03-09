#include "timer.h"
#include <stdio.h>

void timehandler_init(struct Timehandler *timehandler)
{
	timehandler->timeout = -1;
}

void timehandler_delay(struct Timehandler *timehandler, double delay)
{	 
	clock_t t;
	t = clock();
	timehandler->timeout = t + CLOCKS_PER_SEC*delay;

}

int timehandler_is_time_up(struct Timehandler *timehandler)
{
	if (timehandler->timeout == -1) return -1;
	else return (clock() >= timehandler->timeout);
}
