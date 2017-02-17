#include "statemachine.h"
#include "elev.h"
#include <stdio.h>

/* For descriptions see statemachine.h */

int statemachine_init_floor(void)
{
	elev_set_motor_direction(DIRN_UP);
	printf(" -DETERMINING POSITION\n");

	while (1)
	{
		int floor;
		floor = elev_get_floor_sensor_signal();
		
		if (floor != -1)
		{
			elev_set_motor_direction(DIRN_STOP);
			printf("%s %d %s"," -FLOOR:" ,floor+1, "\n");
			return floor;
		}
	}
}
