# Assignment 3 - UCS645 (Parallel Computing with MPI)

## Introduction
This assignment explores different parallel computing concepts using MPI. The programs cover numerical approximation, linear algebra operations, and parallel searching techniques.

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

### 1. Estimating Pi Using Numerical Integration (Pi_Value.c)
**Description:**
Uses numerical integration to approximate the value of Pi using parallel execution.

**Compile and Run:**
```sh
mpicc -o pi_value Pi_Value.c
mpirun -np 4 ./pi_value
```

---

### 2. Parallel DAXPY Computation (DAXPY.c)
**Description:**
Computes the DAXPY operation (Y = A * X + Y) using MPI and compares execution time with a sequential implementation.

**Compile and Run:**
```sh
mpicc -o daxpy DAXPY.c
mpirun -np 4 ./daxpy
```

---

### 3. Finding Positive Prime Numbers (PositivePrimes.c)
**Description:**
Distributes numbers across processes to check for prime numbers in parallel.

**Compile and Run:**
```sh
mpicc -o positive_primes PositivePrimes.c
mpirun -np 4 ./positive_primes
```

---

## Conclusion
This assignment demonstrates key concepts in numerical methods and parallel execution using MPI. Experiment with different problem sizes and numbers of processes to analyze performance improvements.

For any issues, refer to the MPI documentation or contact the instructor.

