#include "orderhandler.h"
#include "elev.h"
#include "statemachine.h"

#include <stdio.h>

/* For descriptions see orderhandler.h */

void orderhandler_init(struct Orderhandler* target)
{
	target->outside_going_up[0] = 0;
	target->outside_going_up[1] = 0;
	target->outside_going_up[2] = 0;
	target->outside_going_up[3] = 0;
	
	target->outside_going_down[0] = 0;
	target->outside_going_down[1] = 0;
	target->outside_going_down[2] = 0;
	target->outside_going_down[3] = 0; 
	
	target->target_list[0] = -1;
	target->target_list[1] = -1;
	target->target_list[2] = -1;
	target->target_list[3] = -1;
}

void orderhandler_print_lists(struct Orderhandler *target)
{
	for (int i = 0; i < 4; i++)
	{
		printf("%d %d %s", target->outside_going_up[i],target->outside_going_down[i]," ");
	}
	
	printf("%s", "| ");
	
	for (int i = 0; i < 4; i++)
	{
		printf("%d %s", target->target_list[i] , " ");
	}
	printf("\n");
}

void orderhandler_update_outside_lists(struct Orderhandler *target)
{
	for (int floor = 0; floor < 4; floor++) //Går gjennom bestillingsknappene og oppdaterer minne
	{
		int button_state_up = 0;
		int button_state_down = 0;	

		if (floor != 3) button_state_up = elev_get_button_signal(BUTTON_CALL_UP, floor);
		if (floor != 0) button_state_down = elev_get_button_signal(BUTTON_CALL_DOWN, floor);
		
		target->outside_going_up[floor] = button_state_up; 
		target->outside_going_down[floor] = button_state_down;
	}

}

void orderhandler_update_lights(struct Orderhandler *target)
{
	for (int floor = 0; floor < 4; floor++)
	{
		int LIGHT_UP;
		int LIGHT_DOWN;
		
		if (floor != 3) LIGHT_UP = target->outside_going_up[floor];
		else LIGHT_UP = 0;
		
		if (floor != 0) LIGHT_DOWN = target->outside_going_down[floor];
		else LIGHT_DOWN = 0;
		
		if (floor != 3) elev_set_button_lamp(BUTTON_CALL_UP, floor, LIGHT_UP);
		if (floor != 0) elev_set_button_lamp(BUTTON_CALL_DOWN, floor, LIGHT_DOWN);
		
		elev_set_button_lamp(BUTTON_COMMAND, floor, 0);
		for (int q = 0; q < 4; q++) if (target->target_list[q] == floor) {elev_set_button_lamp(BUTTON_COMMAND, floor, 1); break;}
	}
}

void orderhandler_update_target_list(struct Orderhandler *target)
{
	for (int floor = 0; floor < 4; floor++)
	{
		if (elev_get_button_signal(BUTTON_COMMAND, floor)) orderhandler_add_target(target,floor);
	}	
}

void orderhandler_add_target(struct Orderhandler *target, int floor)
{
	for (int i = 0; i < 4; i++) if(target->target_list[i] == floor) return;
	for (int i = 0; i < 4; i++) if(target->target_list[i] == -1) {target->target_list[i] = floor; return; }
}

void orderhandler_remove_target_index(struct Orderhandler *target, int start_floor)
{
	target->target_list[3] = -1;
	for (int i = start_floor; i < 3; i++)
	{
		if (target->target_list[i+1] == -1) {target->target_list[i] = -1; break;}
		target->target_list[i] = target->target_list[i+1];
	}
	
}

void orderhandler_remove_target_floor(struct Orderhandler *target, int floor)
{
	for (int i = 0; i < 3; i++)
	{
		if (target->target_list[i] == floor)
		{
			orderhandler_target_clear(target, i);
			break;
		}
	}
}

int orderhandler_stop_at_floor(struct Orderhandler *target, struct Statemachine *statemachine, int floor_sensor_value)
{
	if (floor_sensor_value == -1) return 0; //Does not stop if elevator is not at a floor
	
	if (statemachine->current_floor == target->target_list[0] && floor_sensor_value == statemachine->current_floor) return 1; //Stops if floor is next target
	
	return 0;
}


