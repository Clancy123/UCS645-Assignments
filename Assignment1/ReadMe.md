# Assignment 1 - UCS645 (Parallel Computing with MPI)

## Introduction
This assignment demonstrates various MPI (Message Passing Interface) concepts using C programs. The codes provided illustrate fundamental MPI operations such as sending/receiving messages, probing for message sizes, and implementing a random walk simulation using multiple processes.

## Prerequisites
To run these programs, you need:
- A system with MPI installed (e.g., OpenMPI, MPICH)
- A C compiler (e.g., `gcc`)
- A multi-core system or access to a distributed computing environment

## Compiling and Running MPI Programs
Each program can be compiled and executed using the following general commands:

### Compilation
```sh
mpicc -o <output_executable> <source_file.c>
```

### Execution
```sh
mpirun -np <number_of_processes> ./<output_executable>
```

---

## MPI Programs

### 1. Hello World (mpi_helloworld.c)
**Description:**
This program initializes MPI, retrieves the rank and total number of processes, and prints a "Hello World" message from each process.

**Compile and Run:**
```sh
mpicc -o mpi_helloworld mpi_helloworld.c
mpirun -np 4 ./mpi_helloworld
```

---

### 2. MPI Probe (mpi_probe.c)
**Description:**
Demonstrates the use of `MPI_Probe` to determine the amount of incoming data before receiving it dynamically.

**Compile and Run:**
```sh
mpicc -o mpi_probe mpi_probe.c
mpirun -np 2 ./mpi_probe
```

---

### 3. MPI Send and Receive (mpi_sendrecv.c)
**Description:**
Illustrates basic `MPI_Send` and `MPI_Recv` operations between two processes.

**Compile and Run:**
```sh
mpicc -o mpi_sendrecv mpi_sendrecv.c
mpirun -np 2 ./mpi_sendrecv
```

---

### 4. MPI Status (mpi_status.c)
**Description:**
Shows how to use `MPI_Status` to check metadata about received messages.

**Compile and Run:**
```sh
mpicc -o mpi_status mpi_status.c
mpirun -np 2 ./mpi_status
```

---

### 5. Random Walk Simulation (random_walk.c)
**Description:**
Implements a random walk model using MPI, where walkers move through a distributed domain.

**Compile and Run:**
```sh
mpicc -o random_walk random_walk.c
mpirun -np 4 ./random_walk 100 50 10
```
- `100`: Domain size
- `50`: Maximum walk size
- `10`: Number of walkers per process

---
