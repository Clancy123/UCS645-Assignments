
#include <mpi.h>
#include <stdio.h>

static long num_steps = 100000;  // Number of iterations for accuracy
double step;

int main(int argc, char **argv) {
    int rank, size, i;
    double x, sum = 0.0, pi, local_sum = 0.0;
    double start_time, end_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Process 0 broadcasts num_steps to all other processes
    MPI_Bcast(&num_steps, 1, MPI_LONG, 0, MPI_COMM_WORLD);

    step = 1.0 / (double)num_steps;

    // Calculate the range for each process
    long start = rank * (num_steps / size);
    long end = (rank + 1) * (num_steps / size);

    start_time = MPI_Wtime();

    // Each process calculates its portion of the sum
    for (i = start; i < end; i++) {
        x = (i + 0.5) * step;
        local_sum += 4.0 / (1.0 + x * x);
    }

    // Reduce all local sums to a global sum at process 0
    MPI_Reduce(&local_sum, &sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        pi = step * sum;
        end_time = MPI_Wtime();
        printf("Approximated π: %.15f\n", pi);
        printf("Execution Time: %f seconds\n", end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}
