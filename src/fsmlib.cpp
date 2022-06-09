#include "fsmlib.hpp"
#include <algorithm>
#include <sstream>

#include <iostream>

//------------------------------------------------------------------------------------------------------------------------------------------
//Constructor
moore_fsm::moore_fsm(const size_t& _num_inputs, const size_t& _num_outputs, const size_t& _num_states) :
num_inputs(_num_inputs), num_outputs(_num_outputs)
{
    //Configure inputs and their names
    current_inputs = std::vector<size_t>(num_inputs, 0);
    for(size_t i = 0; i < _num_inputs; ++i)
        name_input_id_map[std::to_string(i)] = i;

    //Configure outputs and their names
    current_outputs = std::vector<size_t>(num_outputs, 0);
    for(size_t i = 0; i < _num_outputs; ++i)
        name_output_id_map[std::to_string(i)] = i;

    //Pre allocate space for the states
    machine_states.reserve(_num_states);

    current_state_id = 0;
}
//Destructor is default type

//------------------------------------------------------------------------------------------------------------------------------------------
//Getter/setter of I/O
int moore_fsm::set_input(const size_t& id, const size_t& value){
    if(id >= num_inputs)
        return 1;
    
    current_inputs[id] = value;
    return 0;
}
int moore_fsm::set_input(const std::string& name, const size_t& value){
    if(name_input_id_map.contains(name)){
        current_inputs[name_input_id_map.at(name)] = value;
        return 0;
    } else
        return 1;
}
int moore_fsm::set_inputs(const std::vector<size_t>& in){
    if(in.size() < current_inputs.size()){
        std::copy(in.begin(), in.end(), current_inputs.begin());
        return 1;
    }
    else if(in.size() > current_inputs.size()){
        std::copy_n(in.begin(), current_inputs.size(), current_inputs.begin());
        return 2;
    }
    else {
        current_inputs = in;
        return 0;
    }
}
size_t moore_fsm::get_output(const size_t& id) const{
    if(id >= num_outputs)
        return -1;

    return current_outputs[id];
}
size_t moore_fsm::get_output(const std::string& name) const{
    if(name_output_id_map.contains(name))
        return current_outputs[name_output_id_map.at(name)];
    else
        return -1;
}
const std::vector<size_t>& moore_fsm::get_outputs() const{
    return current_outputs;
}

//------------------------------------------------------------------------------------------------------------------------------------------
//Associating names to inputs
int moore_fsm::set_input_name(const size_t& input_id, const std::string& name){
    if(input_id >= num_inputs)
        return 1;

    std::erase_if(name_input_id_map, [&](const auto& e) -> bool{return e.second == input_id;});
    name_input_id_map[name] = input_id;
    return 0;
}
std::string moore_fsm::get_input_name(const size_t& input_id) const {
    const auto it = std::find_if(name_input_id_map.begin(), name_input_id_map.end(), [&](const auto& e) -> bool{return e.second == input_id;});
    if(it != std::end(name_input_id_map))
        return it->first;
    else
        return "";
}
size_t moore_fsm::get_input_id(const std::string& name) const {
    if(name_input_id_map.contains(name))
        return name_input_id_map.at(name);
    else
        return -1;
}

//------------------------------------------------------------------------------------------------------------------------------------------
//Associating names to outpus
int moore_fsm::set_output_name(const size_t& output_id, const std::string& name){
    if(output_id >= num_outputs)
        return 1;

    std::erase_if(name_output_id_map, [&](const auto& e) -> bool{return e.second == output_id;});
    name_output_id_map[name] = output_id;
    return 0;
}
std::string moore_fsm::get_output_name(const size_t& output_id){
    const auto it = std::find_if(name_output_id_map.begin(), name_output_id_map.end(), [&](const auto& e) -> bool{return e.second == output_id;});
    if(it != std::end(name_output_id_map))
        return it->first;
    else
        return "";
}
size_t moore_fsm::get_output_id(const std::string& name){
    if(name_output_id_map.contains(name))
        return name_output_id_map.at(name);
    else
        return -1;
}

//------------------------------------------------------------------------------------------------------------------------------------------
//Adding and removing states
size_t moore_fsm::add_state(const std::vector<size_t>& outputs, const state_transition_fn& transition_fn){
    machine_states.emplace_back(outputs, transition_fn);

    const auto last_state_id = machine_states.size() - 1;
    set_state_name(last_state_id, std::to_string(last_state_id));

    return last_state_id;
}
size_t moore_fsm::add_state(const std::string& name, const std::vector<size_t>& outputs, const state_transition_fn& transition_fn){
    machine_states.emplace_back(outputs, transition_fn);

    const auto last_state_id = machine_states.size() - 1;
    if(name.empty())
        set_state_name(last_state_id, std::to_string(last_state_id));
    else
        set_state_name(last_state_id, name);

    return last_state_id;
}
int moore_fsm::remove_state(const size_t& state_id){
    if(state_id >= machine_states.size())
        return 1;

    machine_states.erase(machine_states.begin() + state_id);
    std::erase_if(name_state_id_map, [&](const auto& e) -> bool{return e.second == state_id;});

    return 0;
}
int moore_fsm::remove_state(const std::string& name){
    if(!name_state_id_map.contains(name))
        return 1;
    else {
        const auto id = name_state_id_map.at(name);

        return remove_state(id);
    }
}
size_t moore_fsm::get_current_state_id() const {
    return current_state_id;
}
std::string moore_fsm::get_current_state_name() const {
    const auto it = std::find_if(name_state_id_map.begin(), name_state_id_map.end(), [&](const auto& e) -> bool{return e.second == current_state_id;});
    if(it != std::end(name_state_id_map))
        return it->first;
    else
        return "";
}

//------------------------------------------------------------------------------------------------------------------------------------------
//Associating names to states
int moore_fsm::set_state_name(const size_t& state_id, const std::string& name){
    if(state_id >= machine_states.size())
    return 1;

    std::erase_if(name_state_id_map, [&](const auto& e) -> bool{return e.second == state_id;});
    name_state_id_map[name] = state_id;
    return 0;
}
std::string moore_fsm::get_state_name(const size_t& state_id){
    const auto it = std::find_if(name_state_id_map.begin(), name_state_id_map.end(), [&](const auto& e) -> bool{return e.second == state_id;});
    if(it != std::end(name_state_id_map))
        return it->first;
    else
        return "";
}
size_t moore_fsm::get_state_id(const std::string& name){
    if(name_state_id_map.contains(name))
        return name_state_id_map.at(name);
    else
        return -1;
}

//------------------------------------------------------------------------------------------------------------------------------------------
//Simulation of the machine
int moore_fsm::set_current_state(const size_t& state_id){
    if(state_id >= machine_states.size())
        return 1;

    current_state_id = state_id;
    current_outputs = machine_states[current_state_id].state_outputs;
    return 0;
}
int moore_fsm::set_current_state(const std::string& name){
    if(!name_state_id_map.contains(name))
        return 1;
    else {
        const auto id = name_state_id_map.at(name);

        return set_current_state(id);
    }
}
size_t moore_fsm::step_machine(){
    const auto next_state_id = machine_states[current_state_id].transition_fn(current_inputs, name_input_id_map, name_state_id_map);

    if(next_state_id >= machine_states.size())
        return -1;

    current_state_id = next_state_id;
    current_outputs = machine_states[current_state_id].state_outputs;

    return current_state_id;
}
int moore_fsm::step_machine(const size_t& num_steps){
    int ret_val = 0;

    for(size_t i = 0; i < num_steps; ++i)
        step_machine();

    return ret_val;
}

//------------------------------------------------------------------------------------------------------------------------------------------
//Saving/loading the machine
/*
std::string to_string(const moore_fsm& mfsm){
    std::stringstream ret;
    ret << "{";

    //Load the number of inputs, outputs and states
    ret << "\"" << "num_inputs" << "\":" << mfsm.num_inputs << ",";
    ret << "\"" << "num_outputs" << "\":" << mfsm.num_outputs << ",";
    ret << "\"" << "num_states" << "\":" << mfsm.machine_states.size() << ",";

    //Load the aliases of the inputs
    ret << "\"input_names\":[";
    for(auto it = mfsm.name_input_id_map.begin(); it != mfsm.name_input_id_map.end(); ++it)
        ret << "\"" << it->first << "\",\"" << it->second << "\"" << ",";
    ret.seekp(-1, ret.cur);
    ret << "],";

    //Load the aliases of the outputs
    ret << "\"output_names\":[";
    for(auto it = mfsm.name_output_id_map.begin(); it != mfsm.name_output_id_map.end(); ++it)
        ret << "\"" << it->first << "\",\"" << it->second << "\"" << ",";
    ret.seekp(-1, ret.cur);
    ret << "],";

    //Load the aliases of the states
    ret << "\"state_names\":[";
    for(auto it = mfsm.name_state_id_map.begin(); it != mfsm.name_state_id_map.end(); ++it)
        ret << "\"" << it->first << "\",\"" << it->second << "\"" << ",";
    ret.seekp(-1, ret.cur);
    ret << "]";

    //how do I serialize std::function-s?

    ret << "}";
    return ret.str();
}
*/