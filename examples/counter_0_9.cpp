/*
In this example, a simple up counter class is implemented and a fsm
is programmed to:
- reset the counter after it reaches the number 9
- stop the counter and wait for 3 clock cycles
- repeat

This programming style is common in VHDL.
*/

#include <iostream>
#include "fsmlib.hpp"

using namespace std;

struct counter{
    //Members
    size_t count;

    //Methods
    counter() : count(0) {}
    void tick(const bool& reset, const bool& count_enable) {
        if(reset)
            count = 0;
        else if(count_enable)
            ++count;
    }
};

int main(){
    moore_fsm fsm{1, 2};
    counter counter09;

    //Signals
    size_t counter_count;
    bool counter_res;
    bool counter_ce;

    //Assign name aliases to inputs and outputs
    fsm.set_input_name(0, "count");
    fsm.set_output_name(0, "res");
    fsm.set_output_name(1, "ce");

    fsm.add_state(  "run",                              //name of the state
                    {0, 1},                             //state outputs (first output is reset signal, second outputs is count enable signal)
                    []tr_lamba -> size_t{  //lambda to calculate the next state
                        //If count input is 8, go to state 1, else stay in this state
                        //We have to check if the input is 8 because of how synchronous state machines work. The timer is allowed to do one more step
                        //before it is reset
                        if(inputs[name_to_input_id.at("count")] == 8)
                            return name_to_state_id.at("res");
                        else
                            return name_to_state_id.at("run");
                    });

    fsm.add_state(  "res",
                    {1, 0},
                    []tr_lamba -> size_t{
                        //go directly to next state
                        return name_to_state_id.at("wait2");
                    });
    fsm.add_state(  "wait2",    //wait for another clock cycle
                    {0, 0},
                    []tr_lamba -> size_t{
                        //go to next state
                        return name_to_state_id.at("wait3");
                    });
    fsm.add_state(  "wait3",    //wait for another clock cycle
                    {0, 0},
                    []tr_lamba -> size_t{
                        //go to "run" state again
                        return name_to_state_id.at("run");
                    });

    //Initial conditions
    //Setting the initial state
    fsm.set_current_state("run");
    cout << "Initial conditions: " << endl;
	cout << "- FSM state  : " << fsm.get_current_state_name() << " (" << fsm.get_current_state_id() << ")" << endl;
	cout << "- FSM outputs: " << fsm.get_output(0) << ", " << fsm.get_output(1) << endl;
	cout << "- Counter cnt: " << counter09.count << endl;
    counter_count = 0;
    counter_res = fsm.get_output(0);
    counter_ce = fsm.get_output(1);
    cout << "- signal \"counter_count\" : " << counter_count << endl;
    cout << "- signal \"counter_res\"   : " << counter_res << endl;
    cout << "- signal \"counter_ce\"    : " << counter_ce << endl;

    //Simulation of the circuit for 30 clock cycles
    for(int i = 0; i < 30; ++i){
        //The simulation is divided into 3 phases:
        //- simulation of the fsm and of the datapath (in this case the counter)
        //- update of the signals based on the output of the devices
        //- update of the inputs of the devices based on the signals

        //Step 1: simulation
        fsm.step_machine();
        counter09.tick(counter_res, counter_ce);

        //Step 2: update the signals
        counter_count = counter09.count;
        counter_res = fsm.get_output(0);
        counter_ce = fsm.get_output(1);

        //Step 3: update the inputs of the devices, in this case only of the fsm
        fsm.set_input(0, counter_count);

        //Display info
        cout << "-------------------------------------------------------------------------------" << endl;
        cout << "Step " << i << endl;
	    cout << "- FSM state  : " << fsm.get_current_state_name() << " (" << fsm.get_current_state_id() << ")" << endl;
	    cout << "- FSM outputs: " << fsm.get_output(0) << ", " << fsm.get_output(1) << endl;
	    cout << "- Counter cnt: " << counter09.count << endl;
        counter_count = 0;
        counter_res = fsm.get_output(0);
        counter_ce = fsm.get_output(1);
        cout << "- signal \"counter_count\" : " << counter_count << endl;
        cout << "- signal \"counter_res\"   : " << counter_res << endl;
        cout << "- signal \"counter_ce\"    : " << counter_ce << endl;
    }

    return 0;
}