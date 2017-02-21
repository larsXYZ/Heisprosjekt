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
		if (i % 100000 == 0) orderhandler_print_wait_list(&orderhandler);
	}

    return 0;
}
