#include "statemachine.h"
#include "orderhandler.h"
#include "timer.h"
#include "elev.h"
#include <stdio.h>

/* For descriptions see statemachine.h */

void statemachine_init(struct Statemachine* target)
{
	target->current_floor = statemachine_init_floor();
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

void statemachine_print_state(struct Statemachine* statemachine)
{
	printf( "%s %u %s", "current state: ",statemachine->state, " | ");
}

void statemachine_run(struct Statemachine* statemachine, struct Orderhandler* orderhandler, struct Timehandler* timehandler)
{
	int i = 0;
	while(1)
	{
		//THINGS THAT NEED TO BE DONE EVERYTIME, UPDATING LISTS, CHECKING SENSORS, ETC
		i++;
		orderhandler_update_wait_list(orderhandler);
		orderhandler_update_lights(orderhandler);
		orderhandler_update_target_list(orderhandler);
		statemachine_update_current_floor(statemachine);
		statemachine_update_current_floor_light(statemachine);
		if (i % 20000 == 0) { statemachine_print_state(statemachine); orderhandler_print_lists(orderhandler);}
	
		switch (statemachine->state)
		{
	
			case IDLE:	//CHECKS IF ANY BUTTON IS PRESSED, GOES TO DESIGNATED FLOOR IF CORRESPONDING BUTTON IS PRESSED
			{
				int button_pressed = 0;
				for (int i = 0; i < 4; i++) if (orderhandler->wait_list[i] != NO_PASSENGER) {button_pressed = 1; orderhandler_add_target(orderhandler,i); }
				if (orderhandler->target_list[0] != -1) button_pressed = 1;

				if (button_pressed) statemachine->state = NORM;
				break;
			}
			case NORM:	//GOING TO TARGET IN TARGETLIST
			{
				if (statemachine->current_floor == orderhandler->target_list[0])
				{ 
					orderhandler_remove_target(orderhandler,0);
					orderhandler->wait_list[elev_get_floor_sensor_signal()] = NO_PASSENGER;
					statemachine->state = STOP;					
					elev_set_door_open_lamp(1);
					elev_set_motor_direction(DIRN_STOP);
					timehandler_delay(timehandler,3);
					break;
				}
				
				//OPERATES MOTOR CORRESPONDING TO TARGET_LIST
				if(statemachine->current_floor > orderhandler->target_list[0] && orderhandler->target_list[0] != -1) elev_set_motor_direction(DIRN_DOWN);
				else if (statemachine->current_floor < orderhandler->target_list[0] && orderhandler->target_list[0] != -1) elev_set_motor_direction(DIRN_UP);
				else elev_set_motor_direction(DIRN_STOP);
				
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
			
			
			
				break;
			}
			default:
			{
				printf("EXTREME SYSTEM ERROR, RUN FOR YOUR LIFE");
			}
		
	
		}
	}	
}
