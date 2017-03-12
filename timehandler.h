//Handles time criterias using processor clock

#include <time.h>
struct Timehandler
{
	int timeout;
};

void timehandler_init(struct Timehandler *target); //Initializes the timehandler object

void timehandler_delay(struct Timehandler *target, int delay); //Sets a delay time

int timehandler_is_time_up(struct Timehandler *target); //Checks if delay time is reached

