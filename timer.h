#pragma once
#include <time.h>
struct Timehandler 
{
	int timeout;
};

void timehandler_init(struct Timehandler *timehandler);

void timehandler_delay(struct Timehandler *timehandler, int delay);

int timehandler_is_time_up(struct Timehandler *timehandler);

