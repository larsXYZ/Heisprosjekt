struct Orderhandler;

enum machine_state{IDLE, NORM, STOP, ESTOP};

struct Statemachine
{
	enum machine_state state; //State of statemachine
	int current_floor; //0 - first, 1 - second , 2 - third, 3 - fourth
};

void statemachine_init(struct Statemachine* target); //Initializes statemachine

int statemachine_init_floor(void); //Determines where the elevator is by moving to a floor and registering its number

void statemachine_update_current_floor(struct Statemachine* target); //Updates the current_floor value

void statemachine_update_current_floor_light(struct Statemachine* target);//Updates the current_floor lights

void statemachine_run(struct Statemachine* statemachine, struct Orderhandler* orderhandler);

void statemachine_print_state(struct Statemachine* statemachine);
