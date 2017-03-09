#include <time.h>
struct Timehandler
{
	int timeout;
};

void timehandler_init(struct Timehandler *target);

void timehandler_delay(struct Timehandler *target, int delay);

int timehandler_is_time_up(struct Timehandler *target);

