# MPI

## Introduction

🔘 **What is MPI?**

Programs pass messages through some processes in order to finish parallel tasks. In practice, it's easy to implement parallel tasks by MPI. For example manager process can send a message to describe a new job, and assign the job to the latter.

🔘 **Concepts in MPI**

- **communicator**: define a set of processes which can send message to each other
- **rank**: inside a communicator, each of the process has a number(called rank). Processes communicate **explicitly** by specifying ranks.
- **tag**: each message has its unique tag.

## Tasks

### Introduction to MPI

- [x] [Hello world](./1.introduction.md#11-hello-world)

### Point-to-point communication

- [x] [Message exchange](./2.point_to_point.md#21-message-exchange)
- [x] [Parallel pi with two processes](./2.point_to_point.md#22-parallel-pi-with-two-processes-program)

### Special MPI variables and communication patterns

- [x] [Message chain](./3.special_mpi_varables.md#31-message-chain)
- [x] [Parallel pi revisited](./3.special_mpi_varables.md#32-parallel-pi-general)
- [x] [(Bonus) Broadcast and scatter](./3.special_mpi_varables.md#33-broadcast-and-scatter)

### Collective operations

- [x] [Collective operations](./4.collectives.md)

### Debugging

- [x] [Debugging](./5.debugging.md)

### Heat equation

- [x] [Parallel heat equation solver](./6.heat_equation.md): First three tasks

### Custom communicators

- [x] [Communicators and collectives](./7.communicator.md)

### Non-blocking communication and persistent communication

- [x] [Message chain revisited](./8.message_chain_non_blocking.md)

### Process topologies

- [x] [Cartesian grid process topology](./9.process_topologies.md#91-cartesian-grid)
- [x] [Message chain with Cartesian communicator](./9.process_topologies.md#92-chain-cartesian)

### User-defined datatypes

- [ ] [Custom datatypes](./10.user_defined_types.md#101-custom-datatypes)
- [ ] [Modifying extent](./10.user_defined_types.md#102-modifying-extent)
- [ ] [Communicating struct](./10.user_defined_types.md#103-communicating-struct)

### Heat equation cont'd

- [ ] [Parallel heat equation solver](./6.heat_equation.md): Remaining tasks
