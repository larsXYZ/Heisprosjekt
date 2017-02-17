#include "elev.h"
#include "statemachine.h"
#include <stdio.h>



int main() {
   
	printf(" -INITIALIZING SYSTEMS- \n");
	// Initialize hardware
    if (!elev_init()) {
        printf("Unable to initialize elevator hardware!\n");
        return 1;
    }
	else{ printf("   -initializing hardware\n"); }

	//Initialize statemachine
	struct Statemachine statemachine;
	statemachine.state = INIT;
	statemachine.current_floor = statemachine_init_floor();
	statemachine.state = NORM;

    return 0;
}
