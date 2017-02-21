#include "statemachine.h"
#include "elev.h"
#include <stdio.h>

/* For descriptions see statemachine.h */

void statemachine_init(struct Statemachine* target)
{
	target->state = INIT;
	target->current_floor = statemachine_init_floor();
	target->state = NORM;
}

int statemachine_init_floor(void)
{
	elev_set_motor_direction(DIRN_UP);
	printf("   -determining position\n");

	while (1)
	{
		int floor;
		floor = elev_get_floor_sensor_signal();
		
		if (floor != -1)
		{
			elev_set_motor_direction(DIRN_STOP);
			printf("%s %d %s","   -floor found :" ,floor+1, "\n");
			return floor;
		}
	}
}

void statemachine_update_current_floor(struct Statemachine* target)
{
	int floor_status = elev_get_floor_sensor_signal();
	if (floor_status != -1) target->current_floor = floor_status;
}

void statemachine_update_current_floor_light(struct Statemachine* target)
{
	elev_set_floor_indicator(target->current_floor);
}
