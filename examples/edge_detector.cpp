/*
In this example, the fsm outputs a 1 whenever its input changes from 0 to 1 or viceversa.

The input is a random boolean.
*/

#include <iostream>
#include <cstdlib>
#include <vector>
#include "fsmlib.hpp"

using namespace std;

int main(){
    srand(time(NULL));
    moore_fsm fsm{1, 1, 4};

    //Signals
    bool boolean_in;
    bool edge_detected;

    //Assign name aliases to inputs and outputs
    fsm.set_input_name(0, "input");
    fsm.set_output_name(0, "edge");

    fsm.add_state(  "low_in_low_out",                   //name of the state
                    {0},                                //state outputs (in this case there's only one)
                    []tr_lamba -> size_t{  //lambda to calculate the next state
                        if(inputs[name_to_input_id.at("input")] != 0)
                            return name_to_state_id.at("high_in_high_out");
                        else
                            return name_to_state_id.at("low_in_low_out");
                    });

    fsm.add_state(  "high_in_high_out",
                    {1},
                    []tr_lamba -> size_t{
                        if(inputs[name_to_input_id.at("input")] != 0)
                            return name_to_state_id.at("high_in_low_out");
                        else
                            return name_to_state_id.at("low_in_high_out");
                    });
    fsm.add_state(  "high_in_low_out",
                    {0},
                    []tr_lamba -> size_t{
                        if(inputs[name_to_input_id.at("input")] != 0)
                            return name_to_state_id.at("high_in_low_out");
                        else
                            return name_to_state_id.at("low_in_high_out");
                    });
    fsm.add_state(  "low_in_high_out",
                    {1},
                    []tr_lamba -> size_t{
                        if(inputs[name_to_input_id.at("input")] != 0)
                            return name_to_state_id.at("high_in_high_out");
                        else
                            return name_to_state_id.at("low_in_low_out");
                    });

    //Initial conditions
    //Setting the initial state
    fsm.set_current_state("low_in_low_out");
    cout << "Initial conditions: " << endl;
	cout << "- FSM state  : " << fsm.get_current_state_name() << " (" << fsm.get_current_state_id() << ")" << endl;
	cout << "- FSM output : " << fsm.get_output(0) << endl;
    boolean_in = false;
    edge_detected = fsm.get_output(0);
    cout << "- signal \"boolean_in\"    : " << boolean_in << endl;
    cout << "- signal \"edge_detected\" : " << edge_detected << endl;

    //Simulation of the circuit for 30 clock cycles
    vector<bool> boolean_inputs;
    boolean_inputs.reserve(100);
    vector<bool> edges;
    edges.reserve(100);
    boolean_inputs.push_back(boolean_in);
    edges.push_back(edge_detected);
    for(int i = 0; i < 30; ++i){
        //The simulation is divided into 3 phases:
        //- simulation of the fsm and of the datapath (in this case the counter)
        //- update of the signals based on the output of the devices
        //- update of the inputs of the devices based on the signals

        //Step 1: simulation
        fsm.step_machine();
        boolean_in = (rand() % 2) && (rand() % 2);

        //Step 2: update the signals
        edge_detected = fsm.get_output("edge");

        //Step 3: update the inputs of the devices, in this case only of the fsm
        fsm.set_input(0, boolean_in);

        //Display info
        //cout << "-------------------------------------------------------------------------------" << endl;
        //cout << "Step " << i << endl;
	    //cout << "- FSM state  : " << fsm.get_current_state_name() << " (" << fsm.get_current_state_id() << ")" << endl;
	    //cout << "- FSM output : " << fsm.get_output(0) << endl;
        //cout << "- signal \"boolean_in\"    : " << boolean_in << endl;
        //cout << "- signal \"edge_detected\" : " << edge_detected << endl;

        //Collect info in vector
        boolean_inputs.push_back(boolean_in);
        edges.push_back(edge_detected);
    }

    //Display info collected in vectors
    cout << "                 if these two bits are different..." << endl;
    cout << "                 ||" << endl;
    cout << "                 VV" << endl;
    cout << "Boolean inputs : "; for(auto it = boolean_inputs.begin(); it != boolean_inputs.end(); ++it) cout << (*it ? "1" : "0"); cout << endl;
    cout << "Edges detected : "; for(auto it = edges.begin(); it != edges.end(); ++it) cout << (*it ? "1" : "0"); cout << endl;
    cout << "Predicted      : 00"; for(auto it = boolean_inputs.begin() + 2; it != boolean_inputs.end(); ++it) cout << (*(it-1) != *(it-2) ? "1" : "0"); cout << endl;
    cout << "                   ^" << endl;
    cout << "                   |" << endl;
    cout << "              ...then this bit is 1, otherwise it's 0" << endl;

    return 0;
}