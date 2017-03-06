#include "elev.h"

struct Orderhandler;
struct Timehandler;

enum machine_state{IDLE, NORM, STOP, ESTOP}; //The possible states of the statemachine

struct Statemachine
{
	enum machine_state state; //State of statemachine
	int current_floor; //0 - first, 1 - second , 2 - third, 3 - fourth
	elev_motor_direction_t current_motor_dir; //Current direction of motor, used when recovering from emergency stop and for controlling motor.
};

void statemachine_init(struct Statemachine* target); //Initializes statemachine

int statemachine_init_floor(void); //Determines where the elevator is by moving to a floor and registering its number

void statemachine_update_current_floor(struct Statemachine* target); //Updates the current_floor value

void statemachine_update_current_floor_light(struct Statemachine* target);//Updates the current_floor lights

void statemachine_run(struct Statemachine* statemachine, struct Orderhandler* orderhandler, struct Timehandler* timehandler); //Main operating function

void statemachine_print_state(struct Statemachine* target); //Prints state, for debugging

void statemachine_motor_control(struct Statemachine* statemachine, struct Orderhandler* orderhandler); //Controls motor

void statemachine_enter_stop_state(struct Statemachine* statemachine, struct Orderhandler* orderhandler, struct Timehandler* timehandler); //Function for entering stop state

void statemachine_print_motor_dir(struct Statemachine* target); //Prints motor dir, for debugging
