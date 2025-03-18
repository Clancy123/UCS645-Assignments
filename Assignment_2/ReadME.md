# Assignment 2 - UCS645 (Parallel Computing with MPI)

## Introduction
This assignment demonstrates various parallel computing concepts using MPI. The programs showcase different distributed computing techniques such as Monte Carlo approximation, matrix multiplication, sorting, heat distribution, and reduction operations.

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

### 1. Monte Carlo Approximation of Pi (Pi_Monte.c)
**Description:**
Uses Monte Carlo method to estimate the value of Pi using parallel execution and demonstrates basic MPI functions.

**Compile and Run:**
```sh
mpicc -o pi_monte Pi_Monte.c
mpirun -np 4 ./pi_monte
```

---

### 2. Matrix Multiplication using MPI (Matrix_Mul.c)
**Description:**
Performs parallel matrix multiplication on a 70x70 matrix using MPI and OpenMP. Also compares execution time with sequential computation.

**Compile and Run:**
```sh
mpicc -o matrix_mul Matrix_Mul.c -fopenmp
mpirun -np 4 ./matrix_mul
```

---

### 3. Parallel Sorting using MPI (OddEvenSort.c)
**Description:**
Performs an odd-even transposition sort using MPI.

**Compile and Run:**
```sh
mpicc -o odd_even_sort OddEvenSort.c
mpirun -np 4 ./odd_even_sort
```

---

### 4. Heat Distribution Simulation using MPI (HeatDistribution.c)
**Description:**
Simulates heat diffusion over a 2D grid using MPI parallelization.

**Compile and Run:**
```sh
mpicc -o heat_distribution HeatDistribution.c
mpirun -np 4 ./heat_distribution
```

---

### 5. Parallel Reduction using MPI (Reduce.c)
**Description:**
Uses MPI_Reduce to perform a sum operation on distributed data.

**Compile and Run:**
```sh
mpicc -o reduce Reduce.c
mpirun -np 4 ./reduce
```

---

### 6. Parallel Dot Product using MPI (DotProduct.c)
**Description:**
Computes the dot product of two large vectors using MPI parallelism.

**Compile and Run:**
```sh
mpicc -o dot_product DotProduct.c
mpirun -np 4 ./dot_product
```

---

### 7. Parallel Prefix Sum (Scan) using MPI (PrefixScan.c)
**Description:**
Implements a parallel prefix sum (scan) using MPI.

**Compile and Run:**
```sh
mpicc -o prefix_scan PrefixScan.c
mpirun -np 4 ./prefix_scan
```

---

### 8. Parallel Matrix Transposition using MPI (MatrixTranspose.c)
**Description:**
Implements parallel matrix transposition using MPI.

**Compile and Run:**
```sh
mpicc -o matrix_transpose MatrixTranspose.c
mpirun -np 4 ./matrix_transpose
```

---
