struct Statemachine;

struct Orderhandler
{
	int outside_going_up[4]; //1 if person at floor (index 0-3) is going up, 0 if not
	int outside_going_down[4]; //1 if person at floor (index 0-3) is going down, 0 if not
	int target_list[4]; //List holding next elevator targets, gotten from passengers
};

void orderhandler_init(struct Orderhandler* target); //Initializes orderhandler with clear lists

void orderhandler_print_lists(struct Orderhandler *target); //Printing function for debugging, prints state of wait_list

void orderhandler_update_outside_lists(struct Orderhandler *target); //Checks with sensors to update outside lists

void orderhandler_update_lights(struct Orderhandler *target); //Updates lights on controlpanel

void orderhandler_update_target_list(struct Orderhandler *target); //Updates target list

void orderhandler_add_target(struct Orderhandler *target, int floor); //Adds new target to the target list if it fits, target = floornumber

void orderhandler_remove_target_index(struct Orderhandler *target, int start_floor); //Removes element with given index from target list

void orderhandler_remove_target_floor(struct Orderhandler *target, int floor); //Searches for target floor then removes it from target list

int orderhandler_stop_at_floor(struct Orderhandler *orderhandler, struct Statemachine *statemachine, int floor_sensor_value); //Decides if elevator should stop and pick up passenger at floor.
