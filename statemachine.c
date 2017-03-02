#include "statemachine.h"
#include "orderhandler.h"
#include "timer.h"
#include <stdio.h>

/* For descriptions see statemachine.h */

void statemachine_init(struct Statemachine* target)
{
	target->current_floor = statemachine_init_floor();
	target->current_motor_dir = DIRN_STOP;
	target->state = IDLE;
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

void statemachine_print_state(struct Statemachine* target)
{
	printf( "%s %u %s", "current state: ",target->state, " | ");
}

void statemachine_motor_control(struct Statemachine* statemachine, struct Orderhandler* orderhandler)
{
	if (orderhandler->target_list[0] > statemachine->current_floor && orderhandler->target_list[0] != -1) statemachine->current_motor_dir = DIRN_UP;
	if (orderhandler->target_list[0] < statemachine->current_floor && orderhandler->target_list[0] != -1) statemachine->current_motor_dir = DIRN_DOWN;
	elev_set_motor_direction(statemachine->current_motor_dir);
}

void statemachine_run(struct Statemachine* statemachine, struct Orderhandler* orderhandler, struct Timehandler* timehandler)
{
	int i = 0;
	while(1)
	{
		//THINGS THAT NEED TO BE DONE EVERYTIME, UPDATING LISTS, CHECKING SENSORS, ETC
		if (elev_get_stop_signal())
		{
			if (statemachine->state == STOP) elev_set_door_open_lamp(1); //OPENS DOOR IF ELEVATOR IS AT FLOOR
			statemachine->state = ESTOP;
		}
			
		i++;
		orderhandler_update_outside_lists(orderhandler);
		orderhandler_update_lights(orderhandler);
		orderhandler_update_target_list(orderhandler);
		statemachine_update_current_floor(statemachine);
		statemachine_update_current_floor_light(statemachine);
		if (i % 15000 == 0) { statemachine_print_state(statemachine); orderhandler_print_lists(orderhandler);}
	
		switch (statemachine->state)
		{
	
			case IDLE:	//CHECKS IF ANY BUTTON IS PRESSED, GOES TO DESIGNATED FLOOR IF CORRESPONDING BUTTON IS PRESSED
			{
				int has_destination = 0;
				for (int floor = 0; floor < 4; floor++) if (orderhandler->outside_going_up[floor]||orderhandler->outside_going_down[floor])
				{
					has_destination = 1;
					orderhandler_add_target(orderhandler,floor);
				}
				if (orderhandler->target_list[0] != -1) has_destination = 1;
				if (has_destination) statemachine->state = NORM;
				break;
			}
			case NORM:	//GOING TO TARGET IN TARGETLIST
			{
				int floor_sensor_value = elev_get_floor_sensor_signal();
				
				if (orderhandler_stop_at_floor(orderhandler, statemachine, floor_sensor_value))
				{ 	
					if (statemachine->current_motor_dir == DIRN_UP || orderhandler->target_list[0] == floor_sensor_value) orderhandler->outside_going_up[floor_sensor_value] = 0;
					if (statemachine->current_motor_dir == DIRN_DOWN || orderhandler->target_list[0] == floor_sensor_value) orderhandler->outside_going_down[floor_sensor_value] = 0;
					
					orderhandler_remove_target_floor(orderhandler, floor_sensor_value);
					
					elev_set_door_open_lamp(1);
					statemachine->state = STOP;					
					elev_set_motor_direction(DIRN_STOP);
					timehandler_delay(timehandler,3);
					break;
				}
				
				statemachine_motor_control(statemachine, orderhandler);
				break;
			}
			case STOP:	//OPENS DOOR FOR 3 SECONDS AND LETS PASSENGERS IN
			{
				if(timehandler_is_time_up(timehandler))
				{
					elev_set_door_open_lamp(0);
					
					if (orderhandler->target_list[0] == -1) statemachine->state = IDLE;
					else statemachine->state = NORM;
				}
				
				break;
			}
			case ESTOP:
			{
				elev_set_motor_direction(DIRN_STOP);
				orderhandler_init(orderhandler);
				if (!elev_get_stop_signal())
				{
					statemachine->state = IDLE;
					elev_set_door_open_lamp(0);
				}
				break;
			}
			default:
			{
				printf("EXTREME SYSTEM ERROR, RUN FOR YOUR LIFE");
			}
		
	
		}
	}	
}
