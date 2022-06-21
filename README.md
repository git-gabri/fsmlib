# `fsmlib`
Finite state machines library in C++

## The `moore_fsm` class
The `moore_fsm` class implements a [Moore machine](https://en.wikipedia.org/wiki/Moore_machine), that is, a finite state machine whose outputs depend only on its current state.  
The inputs and the outputs of this fsm are stored in two `std::vector`s of `size_t` and the transition functions are `std::function`s.

Below is a list of public methods available from the class and a small explanation.  
Under the list there are further explanations on how to use the library.
| Category | Method | Purpose |
|-----|-----|-----|
Constructor | `moore_fsm(num_inputs, num_outputs, num_states = 0)` | Constructor. Specify the number of inputs, outputs and optionally the number of states of the machine. <br />States can be added and removed later, but if the last parameter is specified, a preallocation with `std::reserve` is performed.
Destructor | `~moore_fsm() = default` | Destructor. Default.
Getter general machine info | `size_t get_num_inputs()` | Returns the number of inputs of the fsm.
Getter general machine info | `size_t get_num_outputs()` | Returns the number of outputs of the fsm.
Getter general machine info | `size_t get_num_states()` | Returns the number of states of the fsm.
Getter/setter of I/O | `int set_input(size_t id, size_t value)` | Sets the input specified by `id` to `value`. <br />Returns `0` on success, `1` if `id` is invalid.
Getter/setter of I/O | `int set_input(std::string name, size_t value)` | Sets the input named `name` to `value`. <br />Returns `0` on success, `1` if `name` is invalid.
Getter/setter of I/O | `int set_inputs(std::vector<size_t> in)` | Sets all the inputs of the fsm to `in`. <br />Returns:<br />`0` on success (`in.size() == num_inputs`), <br />`1` if `in.size() < num_inputs` after having copied `in` into the first elements of the fsm inputs, <br />`2` if `in.size() > num_inputs` after having copied the first elements of `in` into the fsm inputs.
Getter/setter of I/O | `size_t get_output(const size_t id)` | Returns the value of the output specified by `id`. <br />Returns `-1` if `id` is invalid.
Getter/setter of I/O | `size_t get_output(std::string name)` | Returns the value of the output specified by `name`. <br />Returns `-1` if `name` is invalid.
Getter/setter of I/O | `const std::vector<size_t> get_outputs()` | Returns the vector of outputs of the fsm.
Associating names to inputs | `int set_input_name(size_t input_id, std::string name)` | Sets the input `input_id`' name to `name`. <br />Returns `0` on success, `1` if `input_id` is invalid.
Associating names to inputs | `std::string get_input_name(size_t input_id)` | Returns the name associated to the input `input_id`. <br />Returns an empty string if `input_id` is invalid.
Associating names to inputs | `size_t get_input_id(std::string name)` | Returns the id of the input whose associated name is `name`. <br />Returns `-1` if no input has `name` associated to it.
Associating names to outputs | `int set_output_name(size_t output_id, std::string name)` | Sets the output `output_id`' name to `name`. <br />Returns `0` on success, `1` if `output_id` is invalid.
Associating names to outputs | `std::string get_output_name(size_t output_id)` | Returns the name associated to the output `output_id`. <br />Returns an empty string if `output_id` is invalid.
Associating names to outputs | `size_t get_output_id(std::string name)` | Returns the id of the output whose associated name is `name`. <br />Returns `-1` if no output has `name` associated to it.
Adding and removing states | `size_t add_state(std::vector<size_t> outputs, state_transition_fn transition_fn)` | Adds a state at which the outputs of the fsm will be set to `outputs` and from which the next state is computed calling the transition function `transition_fn`. <br />Returns the id of the newly added state.
Adding and removing states | `size_t add_state(std::string name, std::vector<size_t> outputs, state_transition_fn transition_fn)` | Adds a state in the same way as the above function, but associates the name `name` to it. If `name` is an empty string, it uses the default naming. <br />Returns the id of the newly added state.
Adding and removing states | `int remove_state(size_t state_id)` | Removes the state specified by `state_id`. <br />Returns `0` on success, `1` if `state_id` is invalid.
Adding and removing states | `int remove_state(std::string name)` | Removes the state with the associated name `name`. <br />Returns `0` on success, `1` if no state has `name` associated to it.
Associating names to states | `int set_state_name(size_t state_id, std::string name)` | Sets the state `state_id`' name to `name`. <br />Returns `0` on success, `1` if `state_id` is invalid.
Associating names to states | `std::string get_state_name(size_t state_id)` | Returns the name associated to the state `state_id`. <br />Returns an empty string if `state_id` is invalid.
Associating names to states | `size_t get_state_id(std::string name)` | Returns the id of the state whose associated name is `name`. <br />Returns `-1` if no state has `name` associated to it.
TO FINISH | TO FINISH | TO FINISH