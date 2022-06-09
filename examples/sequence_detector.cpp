/*
In this example, the fsm outputs a 1 whenever the input follows the sequence 10010.

The input is a random boolean.
*/

#include <iostream>
#include <cstdlib>
#include <vector>
#include "fsmlib.hpp"

using namespace std;

int main(){
    srand(time(NULL));
    moore_fsm fsm{1, 1};

    //Signals
    bool boolean_in;
    bool sequence_detected;

    //Assign name aliases to inputs and outputs
    fsm.set_input_name(0, "input");
    fsm.set_output_name(0, "seq");

    fsm.add_state(  "bit0",                             //name of the state
                    {0},                                //state outputs (in this case there's only one)
                    []tr_lamba -> size_t{  //lambda to calculate the next state
                        if(inputs[name_to_input_id.at("input")] != 0)
                            return name_to_state_id.at("bit1");
                        else
                            return name_to_state_id.at("bit0");
                    });
    fsm.add_state(  "bit1",
                    {0},
                    []tr_lamba -> size_t{
                        if(inputs[name_to_input_id.at("input")] == 0)
                            return name_to_state_id.at("bit2");
                        else
                            return name_to_state_id.at("bit1");
                    });
    fsm.add_state(  "bit2",
                    {0},
                    []tr_lamba -> size_t{
                        if(inputs[name_to_input_id.at("input")] == 0)
                            return name_to_state_id.at("bit3");
                        else
                            return name_to_state_id.at("bit1");
                    });
    fsm.add_state(  "bit3",
                    {0},
                    []tr_lamba -> size_t{
                        if(inputs[name_to_input_id.at("input")] != 0)
                            return name_to_state_id.at("bit4");
                        else
                            return name_to_state_id.at("bit0");
                    });
    fsm.add_state(  "bit4",
                    {0},
                    []tr_lamba -> size_t{
                        if(inputs[name_to_input_id.at("input")] == 0)
                            return name_to_state_id.at("seq");
                        else
                            return name_to_state_id.at("bit1");
                    });
    fsm.add_state(  "seq",
                    {1},
                    []tr_lamba -> size_t{
                        if(inputs[name_to_input_id.at("input")] != 0)
                            return name_to_state_id.at("bit1");
                        else
                            return name_to_state_id.at("bit0");
                    });

    //Initial conditions
    //Setting the initial state
    fsm.set_current_state("low_in_low_out");
    cout << "Initial conditions: " << endl;
	cout << "- FSM state  : " << fsm.get_current_state_name() << " (" << fsm.get_current_state_id() << ")" << endl;
	cout << "- FSM output : " << fsm.get_output(0) << endl;
    boolean_in = false;
    sequence_detected = fsm.get_output(0);
    cout << "- signal \"boolean_in\"    : " << boolean_in << endl;
    cout << "- signal \"sequence_detected\" : " << sequence_detected << endl;

    //Simulation of the circuit until the sequence is detected 2 times
    vector<bool> boolean_inputs;
    boolean_inputs.reserve(100);
    vector<bool> edges;
    edges.reserve(100);
    boolean_inputs.push_back(boolean_in);
    edges.push_back(sequence_detected);
    int num_detects = 0;
    while(num_detects < 2){
        //The simulation is divided into 3 phases:
        //- simulation of the fsm and of the datapath
        //- update of the signals based on the output of the devices
        //- update of the inputs of the devices based on the signals

        //Step 1: simulation
        fsm.step_machine();
        boolean_in = (rand() % 2) && (rand() % 2);

        //Step 2: update the signals
        sequence_detected = fsm.get_output("seq");
        if(sequence_detected)
            ++num_detects;

        //Step 3: update the inputs of the devices, in this case only of the fsm
        fsm.set_input(0, boolean_in);

        //Display info
        //cout << "-------------------------------------------------------------------------------" << endl;
        //cout << "Step " << i << endl;
	    //cout << "- FSM state  : " << fsm.get_current_state_name() << " (" << fsm.get_current_state_id() << ")" << endl;
	    //cout << "- FSM output : " << fsm.get_output(0) << endl;
        //cout << "- signal \"boolean_in\"    : " << boolean_in << endl;
        //cout << "- signal \"sequence_detected\" : " << sequence_detected << endl;

        //Collect info in vector
        boolean_inputs.push_back(boolean_in);
        edges.push_back(sequence_detected);
    }

    //Display info collected in vectors
    cout << "Boolean inputs     : "; for(auto it = boolean_inputs.begin(); it != boolean_inputs.end(); ++it) cout << (*it ? "1" : "0"); cout << endl;
    cout << "Sequences detected : "; for(auto it = edges.begin(); it != edges.end(); ++it) cout << (*it ? "1" : "0"); cout << endl;

    return 0;
}