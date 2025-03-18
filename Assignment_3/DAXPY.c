#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N (1 << 16)  // 2^16 elements
#define A 1.5        

// Function to perform sequential DAXPY
void sequentialDAXPY(double* X, const double* Y, double a) {
    for (int i = 0; i < N; i++) {
        X[i] = a * X[i] + Y[i];
    }
}

int main(int argc, char** argv) {
    int rank, num_procs;
    double sequential_time = 0.0;  // Declare sequential_time outside

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    int local_size = N / num_procs;
    double* X = (double*)malloc(local_size * sizeof(double));
    double* Y = (double*)malloc(local_size * sizeof(double));

    if (rank == 0) {
        double* full_X = (double*)malloc(N * sizeof(double));
        double* full_Y = (double*)malloc(N * sizeof(double));

        // Initialize full_X and full_Y
        for (int i = 0; i < N; i++) {
            full_X[i] = 1.0;
            full_Y[i] = 2.0;
        }

        // Measure sequential execution time
        double seq_start = MPI_Wtime();
        sequentialDAXPY(full_X, full_Y, A);
        double seq_end = MPI_Wtime();
        sequential_time = seq_end - seq_start;  // Assign value

        printf("Sequential Execution Time: %f seconds\n", sequential_time);

        // Distribute data to processes
        MPI_Scatter(full_X, local_size, MPI_DOUBLE, X, local_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Scatter(full_Y, local_size, MPI_DOUBLE, Y, local_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        free(full_X);
        free(full_Y);
    } else {
        MPI_Scatter(NULL, local_size, MPI_DOUBLE, X, local_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
        MPI_Scatter(NULL, local_size, MPI_DOUBLE, Y, local_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }

    // Measure parallel execution time
    double start_time = MPI_Wtime();

    // Perform DAXPY on local portion
    for (int i = 0; i < local_size; i++) {
        X[i] = A * X[i] + Y[i];
    }

    double end_time = MPI_Wtime();
    double local_time = end_time - start_time;

    // Gather results at rank 0
    if (rank == 0) {
        double* result = (double*)malloc(N * sizeof(double));
        MPI_Gather(X, local_size, MPI_DOUBLE, result, local_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);

        // Optionally print the first few elements of the result
        printf("First 10 elements of X after DAXPY:\n");
        for (int i = 0; i < 10; i++) {
            printf("%f ", result[i]);
        }
        printf("\n");

        free(result);
    } else {
        MPI_Gather(X, local_size, MPI_DOUBLE, NULL, local_size, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    }

    // Measure max execution time across all processes
    double max_time;
    MPI_Reduce(&local_time, &max_time, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double speedup = sequential_time / max_time;  // Now it has a valid value
        printf("Parallel Execution Time: %f seconds\n", max_time);
        printf("Speedup: %fx\n", speedup);
    }

    free(X);
    free(Y);
    MPI_Finalize();
    return 0;
}