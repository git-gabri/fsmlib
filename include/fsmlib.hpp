#ifndef FSM_INCLUDED
#define FSM_INCLUDED

#include <string>
#include <vector>
#include <map>
#include <functional>

using state_transition_fn = std::function<size_t(const std::vector<size_t>& inputs, const std::map<std::string, size_t>& name_to_input_id)>;

class moore_fsm {
    private:
        size_t num_inputs;
        size_t num_outputs;

        size_t current_state_id;
        std::vector<size_t> current_inputs;
        std::vector<size_t> current_outputs;

        struct state{
            std::vector<size_t> state_outputs;
            state_transition_fn transition_fn;
            
            state(const std::vector<size_t>& out, const state_transition_fn& fn) :
            state_outputs(out), transition_fn(fn) {}
        };
        std::vector<state> machine_states;

        std::map<std::string, size_t> name_input_id_map;
        std::map<std::string, size_t> name_output_id_map;
        std::map<std::string, size_t> name_state_id_map;

    public:
        //---------------------------------------------------------------------------------------
        //Costructors and destructor
        moore_fsm(const size_t& num_inputs, const size_t& num_outputs);
        moore_fsm(const size_t& num_inputs, const size_t& num_outputs, const size_t& num_states);
        ~moore_fsm() = default;

        //---------------------------------------------------------------------------------------
        //Getters of general machine info
        size_t get_num_inputs() const {return num_inputs;}
        size_t get_num_outputs() const {return num_outputs;}
        size_t get_num_states() const {return machine_states.size();}

        //---------------------------------------------------------------------------------------
        //Getter/setter of I/O
        int set_input(const size_t& id, const size_t& value);
        int set_input(const std::string& name, const size_t& value);
        int set_inputs(const std::vector<size_t>& in);
        const std::vector<size_t>& get_output(const size_t& id) const;
        const std::vector<size_t>& get_output(const std::string& name) const;
        const std::vector<size_t>& get_outputs() const;

        //---------------------------------------------------------------------------------------
        //Associating names to inputs
        int set_input_name(const size_t& input_id, const std::string& name);
        std::string get_input_name(const size_t& input_id) const;
        size_t get_input_id(const std::string& name) const;

        //---------------------------------------------------------------------------------------
        //Associating names to outpus
        int set_output_name(const size_t& output_id, const std::string& name);
        std::string get_output_name(const size_t& output_id);
        size_t get_output_id(const std::string& name);

        //---------------------------------------------------------------------------------------
        //Adding and removing states
        size_t add_state(const std::vector<size_t>& outputs, const state_transition_fn& transition_fn);
        size_t add_state(const std::string& name, const std::vector<size_t>& outputs, const state_transition_fn& transition_fn);
        int remove_state(const size_t& state_id);
        int remove_state(const std::string& name);
        size_t get_current_state_id() const;
        std::string get_current_state_name() const;

        //---------------------------------------------------------------------------------------
        //Associating names to states
        int set_state_name(const size_t& state_id, const std::string& name);
        std::string get_state_name(const size_t& state_id);
        size_t get_state_id(const std::string& name);

        //---------------------------------------------------------------------------------------
        //Simulation of the machine
        int set_current_state(const size_t& state_id);
        int set_current_state(const std::string& name);
        size_t step_machine();
        int step_machine(const size_t& num_steps);

        //---------------------------------------------------------------------------------------
        //Saving/loading the machine
        //std::vector<size_t> vec_ify();
        //int devec_ify(std::vector<size_t>& v);
};

#endif