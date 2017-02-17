enum machine_state{INIT, IDLE, NORM, STOP, ESTOP};

struct Statemachine
{
enum machine_state state; //State of statemachine
int current_floor; //0 - first, 1 - second , 2 - third, 3 - fourth
};

int Statemachine_init(void); //Determines where the elevator is by moving to a floor and registering it
