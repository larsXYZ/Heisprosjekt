#include "orderhandler.h"
#include "elev.h"

#include <stdio.h>

/* For descriptions see orderhandler.h */

void orderhandler_init(struct Orderhandler* target)
{
	printf("   -initializing orderhandler\n");
	target->wait_list[0] = NO_PASSENGER;
	target->wait_list[1] = NO_PASSENGER;
	target->wait_list[2] = NO_PASSENGER;
	target->wait_list[3] = NO_PASSENGER;
}

void orderhandler_print_wait_list(struct Orderhandler *target)
{
	for (int i = 0; i < 4; i++)
	{
		printf("%d %s", target->wait_list[i] , " ");
	}
	printf("\n");
}

void orderhandler_update_wait_list(struct Orderhandler *target)
{
	for (int i = 0; i < 4; i++)
	{
		int button_state_up = 0;
		int button_state_down = 0;	

		if (i != 3) button_state_up = elev_get_button_signal(BUTTON_CALL_UP, i);
		if (i != 0) button_state_down = elev_get_button_signal(BUTTON_CALL_DOWN, i);

		int current_state_up = (target->wait_list[i] == GOING_UP);
		int current_state_down = (target->wait_list[i] == GOING_DOWN);
		if (target->wait_list[i] == BOTH_WAYS) { current_state_up = 1; current_state_down = 1; }
		
		int next_state_up = button_state_up || current_state_up;
		int next_state_down = button_state_down || current_state_down;

		if (next_state_up && next_state_down) target->wait_list[i] = BOTH_WAYS;
		else if (next_state_up) target->wait_list[i] = GOING_UP;
		else if (next_state_down) target->wait_list[i] = GOING_DOWN;
		else if (next_state_up) target->wait_list[i] = NO_PASSENGER;
	}

}

void orderhandler_update_lights(struct Orderhandler *target)
{
	for (int floor = 0; floor < 4; floor++) //Etasjebestillingsknapper
	{
		int LIGHT_UP;
		int LIGHT_DOWN;
		
		if (floor != 3) LIGHT_UP = (target->wait_list[floor] == GOING_UP || target->wait_list[floor] == BOTH_WAYS);
		else LIGHT_UP = 0;
		
		if (floor != 0) LIGHT_DOWN = (target->wait_list[floor] == GOING_DOWN || target->wait_list[floor] == BOTH_WAYS);
		else LIGHT_DOWN = 0;
		
		if (floor != 3) elev_set_button_lamp(BUTTON_CALL_UP, floor, LIGHT_UP);
		if (floor != 0) elev_set_button_lamp(BUTTON_CALL_DOWN, floor, LIGHT_DOWN);
	}
}
