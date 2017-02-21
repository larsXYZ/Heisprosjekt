
enum Passenger_outside{NO_PASSENGER =0, GOING_UP, GOING_DOWN, BOTH_WAYS}; //Possible wait list conditions, where does a passenger want to go?

struct Orderhandler
{
	enum Passenger_outside wait_list[4]; //Vector holding information about passenger outside the elevator
};

void orderhandler_init(struct Orderhandler* target); //Initializes orderhandler

void orderhandler_print_wait_list(struct Orderhandler *target); //Printing function for debugging, prints state of wait_list

void orderhandler_update_wait_list(struct Orderhandler *target); //Checks with sensors to update waitlist

void orderhandler_update_lights(struct Orderhandler *target);
