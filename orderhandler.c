#include "orderhandler.h"
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

void orderhandler_print_wait_list(struct Orderhandler target)
{
	for (int i = 0; i < 4; i++)
	{
		printf("%d %s", target.wait_list[i] , " ");
	}
	printf("\n");
}
