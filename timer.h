#pragma once
#include <time.h>
struct Timehandler 
{
	int timeout;
};

void timehandler_delay(struct Timehandler *timehandler, int delay);

void timehandler_is_time_up(struct Timehandler *timehandler);