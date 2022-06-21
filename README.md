# `fsmlib`
Finite state machines library in C++

## The `moore_fsm` class
The `moore_fsm` class implements a [Moore machine](https://en.wikipedia.org/wiki/Moore_machine), that is, a finite state machine whose outputs depend only on its current state.  
The inputs and the outputs of this fsm are stored in two `std::vector`s of `size_t` and the transition functions are `std::function`s.

Below is a list of public methods available from the class and a small explanation.  
Under the list there are further explanations on how to use the library and how it internally works.
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
Adding and removing states <br />Temporarily unavailable | `int remove_state(size_t state_id)` | Removes the state specified by `state_id`. <br />Returns `0` on success, `1` if `state_id` is invalid.
Adding and removing states <br />Temporarily unavailable | `int remove_state(std::string name)` | Removes the state with the associated name `name`. <br />Returns `0` on success, `1` if no state has `name` associated to it.
Associating names to states | `int set_state_name(size_t state_id, std::string name)` | Sets the state `state_id`' name to `name`. <br />Returns `0` on success, `1` if `state_id` is invalid.
Associating names to states | `std::string get_state_name(size_t state_id)` | Returns the name associated to the state `state_id`. <br />Returns an empty string if `state_id` is invalid.
Associating names to states | `size_t get_state_id(std::string name)` | Returns the id of the state whose associated name is `name`. <br />Returns `-1` if no state has `name` associated to it.
Simulation of the machine | `int set_current_state(size_t state_id)` | Sets the current state of the machine to `state_id` and updates the outputs correspondingly. <br />Returns `0` on success, `1` if `state_id` is invalid.
Simulation of the machine | `int set_current_state(std::string name)` | Sets the current state of the machine to a state with the associated name `name` and updates the outputs correspondingly. <br />Returns `0` on success, `1` if no state has `name` associated to it.
Simulation of the machine | `size_t get_current_state_id()` | Returns the current machine state's id.
Simulation of the machine | `std::string get_current_state_name()` | Returns the name associated to the current machine state.
Simulation of the machine | `size_t step_machine()` | Steps the machine for a single step. <br />Returns the machine state after the transition has completed.
Simulation of the machine | `size_t step_machine(size_t num_steps)` | Steps the machine for `num_steps` steps. <br />Returns the machine state after all the transitions have completed.

## Usage
### Naming the inputs, the outputs and the states
The `moore_fsm` class allows to associate names to inputs, outputs and states.  
By default, the name associated to a state corresponds to its id. The id-s, for the inputs and the outputs just go from `0` to `num_inputs-1` and from `0` to `num_outputs - 1` respectively. For the states, they first added state has id `0` and then the id increases as more state are added.  
Keep these simple rules in mind when associating names.

The names are kept into 3 `std::map`s: one for the inputs, one for the outputs and one for the states. They all map an `std::string` containing the name to a `size_t` containing the id.

### Transition functions
The transition functions specified when adding a state are of the type `state_transition_fn`, which is an alias defined in the header of the library to:
```
std::function<size_t(const std::vector<size_t>& inputs, const std::map<std::string, size_t>& name_to_input_id, const std::map<std::string, size_t>& name_to_state_id)>
```
This is a function that returns a `size_t` and takes as arguments:
* the vector of inputs;
* the map containing the names of the inputs;
* the map containing the names of the states.

For convenience, in the header of the library, another alias is defined, called `tr_lambda`, which is the abbreviation of "transition lambda function". It takes the same input arguments as `state_transition_fn`, all passed as const references and, to access them, their names are:
* `inputs`, which is of type `std::vector<size_t>`;
* `name_to_input_id`, which is of type `std::map<std::string, size_t>`;
* `name_to_state_id`, which is of type `std::map<std::string, size_t>`.

An example usage of `tr_lambda` is the following:
```
[...]
fsm.add_state(  "meaningful_name",      //Name of the state
                {0, 1, 3, 7},           //Associated outputs
                                        //Transition function
                []tr_lamba -> size_t{
                    if(inputs[name_to_input_id.at("foo")] == 0)
                        return name_to_state_id.at("bar");
                    else
                        return name_to_state_id.at("baz");
                });
[...]
```

## Examples
Some examples are provided in the `examples` folder.