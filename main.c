#include "elev.h"
#include "statemachine.h"
#include <stdio.h>



int main() {
   
	// Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }
	else
	{
		printf(" -INITIALIZING HARDWARE\n");
	}

	//Initialize statemachine
	struct Statemachine statemachine;
	statemachine.state = INIT;
	statemachine.current_floor = statemachine_init_floor();

    return 0;
}
