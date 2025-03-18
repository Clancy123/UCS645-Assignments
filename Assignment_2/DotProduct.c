
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 1000  // Number of elements

int main(int argc, char** argv) {
    int rank, size;
    int local_n;
    double *vectorA = NULL, *vectorB = NULL;
    double local_dot = 0.0, global_dot = 0.0;
    double start_time, run_time;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    local_n = SIZE / size;
    double *local_A = (double*)malloc(local_n * sizeof(double));
    double *local_B = (double*)malloc(local_n * sizeof(double));
    
    if (rank == 0) {
        vectorA = (double*)malloc(SIZE * sizeof(double));
        vectorB = (double*)malloc(SIZE * sizeof(double));
        srand(time(NULL));
        for (int i = 0; i < SIZE; i++) {
            vectorA[i] = rand() % 100;
            vectorB[i] = rand() % 100;
        }
        start_time = MPI_Wtime();
    }
    
    MPI_Scatter(vectorA, local_n, MPI_DOUBLE, local_A, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(vectorB, local_n, MPI_DOUBLE, local_B, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    for (int i = 0; i < local_n; i++) {
        local_dot += local_A[i] * local_B[i];
    }
    
    MPI_Reduce(&local_dot, &global_dot, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        run_time = MPI_Wtime() - start_time;
        printf("Parallel Dot Product: %f, Execution Time: %f seconds\n", global_dot, run_time);
        free(vectorA);
        free(vectorB);
    }
    
    free(local_A);
    free(local_B);
    MPI_Finalize();
    return 0;
}
