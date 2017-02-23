#include "elev.h"
#include "statemachine.h"
#include "orderhandler.h"

#include <stdio.h>



int main()
{
   	
	//Initialize hardware
	printf(" -INITIALIZING SYSTEMS- \n");
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }
	else{ printf("   -initializing hardware\n"); }

	//Initialize statemachine
	struct Statemachine statemachine;
	statemachine_init(&statemachine);

	//Initialize orderhandler
	struct Orderhandler orderhandler;
	orderhandler_init(&orderhandler);

	int i = 0;
	while (1)
	{
		i++;
		orderhandler_update_wait_list(&orderhandler);
		orderhandler_update_lights(&orderhandler);
		orderhandler_update_target_list(&orderhandler);
		
		statemachine_update_current_floor(&statemachine);
		statemachine_update_current_floor_light(&statemachine);
		
		//TARGETLIST MANIPULASJON
		if (i % 50000 == 0) orderhandler_print_lists(&orderhandler);
		if (elev_get_floor_sensor_signal() == orderhandler.target_list[0]) orderhandler_remove_target(&orderhandler,0);
		
		//TESTKJØRING
		if(statemachine.current_floor > orderhandler.target_list[0] && orderhandler.target_list[0] != -1) elev_set_motor_direction(DIRN_DOWN);
		else if (statemachine.current_floor < orderhandler.target_list[0] && orderhandler.target_list[0] != -1) elev_set_motor_direction(DIRN_UP);
		else elev_set_motor_direction(DIRN_STOP);
		
	}

    return 0;
}
