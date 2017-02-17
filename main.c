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

	orderhandler_print_wait_list(orderhandler);
	printf("%d", statemachine.current_floor);

    return 0;
}
