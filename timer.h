#pragma once
#include <time.h>
struct Timehandler 
{
	int timeout;
};

void timehandler_delay(struct Timehandler *next_timeout, int delay);