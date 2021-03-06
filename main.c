#include "elev.h"
#include "statemachine.h"
#include "orderhandler.h"
#include "timehandler.h"

#include <stdio.h>



int main()
{
   	
	//Initialize hardware
	printf(" -INITIALIZING SYSTEMS- \n");
	if (!elev_init()) {printf("Unable to initialize elevator hardware!\n"); return 1;}
	else printf("   -initializing hardware\n");

	//Initialize orderhandler
	struct Orderhandler orderhandler;
	orderhandler_init(&orderhandler);
	printf("   -initializing orderhandler\n");
	
	//Initialize timehandler
	struct Timehandler timehandler;
	timehandler_init(&timehandler);
	printf("   -initializing timehandler\n");
	
	//Initialize statemachine
	struct Statemachine statemachine;
	statemachine_init(&statemachine);

	statemachine_run(&statemachine, &orderhandler, &timehandler);

    	return 0;
}
