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

void statemachine_enter_stop_state(struct Statemachine* statemachine, struct Orderhandler* orderhandler, struct Timehandler* timehandler)
{
	elev_set_door_open_lamp(1);
	statemachine->state = STOP;
	elev_set_motor_direction(DIRN_STOP);
	timehandler_delay(timehandler,3);
}

void statemachine_print_motor_dir(struct Statemachine* target)
{
	printf( "%s %u %s", " Motor dir(Up: 1, Stop: 0, Down: -1): ",target->current_motor_dir, " | ");
}

void statemachine_run(struct Statemachine* statemachine, struct Orderhandler* orderhandler, struct Timehandler* timehandler)
{
	int i = 0;
	while(1)
	{
	
		int floor_sensor_value = elev_get_floor_sensor_signal(); //Now we dont have to check hardware multiple times each cycle
		i++;
		

		if (elev_get_stop_signal())
		{
			if (floor_sensor_value != -1) elev_set_door_open_lamp(1); //Opens door if elevator is at floor
			elev_set_stop_lamp(1);			
			statemachine->state = ESTOP;
		}
		else
		{
			orderhandler_update_target_list(orderhandler);
			orderhandler_update_outside_lists(orderhandler);
			statemachine_update_current_floor(statemachine);	
		}
		
		//Updating lights
		statemachine_update_current_floor_light(statemachine);	
		orderhandler_update_lights(orderhandler);
		
		//Prints info, for debugging
		if (i % 5000 == 0) { statemachine_print_state(statemachine); orderhandler_print_lists(orderhandler); statemachine_print_motor_dir(statemachine);}
	
		switch (statemachine->state)
		{
	
			case IDLE:	//When the elevator has nothing to do it checks if anyone wants into or out of the elevator.
			{
				
				//Checks if we should enter stop state
				if (floor_sensor_value == statemachine->current_floor) 
				{ 	
					int enter_stop_mode = 0;
					if (floor_sensor_value != 0) enter_stop_mode = elev_get_button_signal(BUTTON_CALL_DOWN, floor_sensor_value); 
					if (floor_sensor_value != 3 && !enter_stop_mode) enter_stop_mode = elev_get_button_signal(BUTTON_CALL_UP, floor_sensor_value); 
					if (!enter_stop_mode) enter_stop_mode = elev_get_button_signal(BUTTON_COMMAND, floor_sensor_value);
					
					if (enter_stop_mode)
					{
						orderhandler->outside_going_up[floor_sensor_value] = 0;
						orderhandler->outside_going_down[floor_sensor_value] = 0;
						if (orderhandler->target_list[0] == floor_sensor_value) orderhandler_remove_target_index(orderhandler, 0);
						statemachine_enter_stop_state(statemachine, orderhandler, timehandler);
						break;
					}
				}

				//Checks if we should go to other floor
				int has_destination = 0;
				for (int floor = 0; floor < 4; floor++)
				{
					if (orderhandler->outside_going_up[floor]||orderhandler->outside_going_down[floor])
					{
						has_destination = 1;
						orderhandler_add_target(orderhandler,floor);
					}
				}
				if (orderhandler->target_list[0] != -1) has_destination = 1;
				if (has_destination) statemachine->state = NORM;

				
				break;
			}
			case NORM:	//Going to target in target list
			{
				//Stops if we have arrived at target, or if we can pick up a passenger along the way
				if (orderhandler_stop_at_floor(orderhandler, statemachine, floor_sensor_value))
				{ 	
					if (statemachine->current_motor_dir == DIRN_UP || orderhandler->target_list[0] == floor_sensor_value) orderhandler->outside_going_up[floor_sensor_value] = 0;
					if (statemachine->current_motor_dir == DIRN_DOWN || orderhandler->target_list[0] == floor_sensor_value) orderhandler->outside_going_down[floor_sensor_value] = 0;
					
					orderhandler_remove_target_floor(orderhandler, floor_sensor_value);
					
					statemachine_enter_stop_state(statemachine, orderhandler, timehandler);
					break;
				}
				
				//Controls motor
				statemachine_motor_control(statemachine, orderhandler);
				
				
				break;
			}
			case STOP:	//Opens door for 3 seconds and lets passengers in
			{
				
				//If first element of target list is the floor we are at we double check that we remove order. This killed a nasty bug. Victory!
				if (floor_sensor_value == orderhandler->target_list[0]) orderhandler_remove_target_index(orderhandler,0);
				
				//Checks if time is up, if it is returns to normal behavior
				if(timehandler_is_time_up(timehandler))
				{
					elev_set_door_open_lamp(0);
					
					if (orderhandler->target_list[0] == -1) statemachine->state = IDLE;
					else statemachine->state = NORM;
				}
				
				
				break;
			}
			case ESTOP:	//EMERGENCY STOP
			{
				elev_set_motor_direction(DIRN_STOP);
				orderhandler_init(orderhandler);
				
				if (!elev_get_stop_signal())
				{
					statemachine->state = IDLE;
					elev_set_door_open_lamp(0);
					elev_set_stop_lamp(0);
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


