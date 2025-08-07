# LSTM: A C++ Implementation of Long Short-Term Memory

This project is a C++ implementation of a Long Short-Term Memory (LSTM) neural network, based on the work of Jurgen Schmidhuber. It provides a simple framework for defining, training, and running LSTM networks from configuration files.

## Building the Project

The project uses a standard `Makefile` for building.

### Dependencies

*   A C++ compiler that supports C++98, such as `g++`.

### Build Commands

To build the project, run the `make` command in the root directory:

```bash
make
```

This will compile the source code and create an executable file named `tester` in the `src/` directory.

To clean up the build files, run:

```bash
make clean
```

## Usage

The primary executable of this project is `src/tester`. It is hardcoded to load two configuration files from the working directory:

*   `simple.cfg`: Defines the architecture of the LSTM network.
*   `train.cfg`: Defines the training data and target outputs, using the same network definition syntax.

To run the simulation, first ensure you have created the necessary configuration files, then execute the `tester`:

```bash
./src/tester
```

The program will then:
1.  Build the network specified in `simple.cfg`.
2.  Load the training data from `train.cfg`.
3.  Run a training loop, performing forward and backward passes.
4.  Save the trained network state to `here.dat`.

## Configuration Files

The network structure and training data are defined in `.cfg` files. These files have a simple, custom syntax for defining nodes and their connections.

### 1. Node Definitions

Nodes are the basic building blocks of the network. They are defined at the beginning of the configuration file.

**Syntax:**
`<NODE_TYPE> <node_name>`

**Node Types:**
*   `IE`: Input Edge - Represents an input to the network.
*   `MB`: Memory Block - The core LSTM cell.
*   `OE`: Output Edge - Represents an output from the network.

**Example:**
```
IE input_1
MB lstm_cell_1
OE output_1
```

### 2. Connection Definitions

After defining all nodes, you must define their connections under a `CONNECTIONS` header.

**Syntax for `IE` and `OE`:**
`CONNECTIONS`
`<target_type> <target_name> <source_name> <weight>`

**Syntax for `MB` (Memory Block):**
`CONNECTIONS`
`<target_type> <target_name> <gate_type> <source_name> <weight>`

*   **`<gate_type>`** specifies which gate of the memory block to connect to:
    *   `IP`: Input gate
    *   `IG`: Input gate (alternative)
    *   `FG`: Forget gate
    *   `OG`: Output gate

### Example Configuration (`simple.cfg`)

Here is an example of a simple network configuration:

```
# Define the nodes
IE input_data
IE input_gate_control
IE forget_gate_control
IE output_gate_control
MB lstm_block
OE network_output

# Define the connections
CONNECTIONS
# Connect inputs to the LSTM block's gates
MB lstm_block IP input_data 1.0
MB lstm_block IG input_gate_control -1.0
MB lstm_block FG forget_gate_control 2.0
MB lstm_block OG output_gate_control -2.0

# Connect the LSTM block to the output
OE network_output lstm_block 1.0
```

This configuration defines a single LSTM block with four separate inputs controlling its data and gates, and one output. The `.dat` files referenced in the original `simple.cfg` should contain the initial weight values for the corresponding nodes.
