
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 1000  // Number of elements

int main(int argc, char** argv) {
    int rank, size;
    int local_n;
    double *vector = NULL;
    double *local_vector, *prefix_sum;
    double start_time, run_time;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    local_n = SIZE / size;
    local_vector = (double*)malloc(local_n * sizeof(double));
    prefix_sum = (double*)malloc(local_n * sizeof(double));
    
    if (rank == 0) {
        vector = (double*)malloc(SIZE * sizeof(double));
        srand(time(NULL));
        for (int i = 0; i < SIZE; i++) {
            vector[i] = rand() % 100;
        }
        start_time = MPI_Wtime();
    }
    
    MPI_Scatter(vector, local_n, MPI_DOUBLE, local_vector, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    // Compute local prefix sum
    prefix_sum[0] = local_vector[0];
    for (int i = 1; i < local_n; i++) {
        prefix_sum[i] = prefix_sum[i - 1] + local_vector[i];
    }
    
    double local_offset = 0.0;
    MPI_Scan(&prefix_sum[local_n - 1], &local_offset, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
    
    if (rank > 0) {
        for (int i = 0; i < local_n; i++) {
            prefix_sum[i] += local_offset - prefix_sum[local_n - 1];
        }
    }
    
    MPI_Gather(prefix_sum, local_n, MPI_DOUBLE, vector, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        run_time = MPI_Wtime() - start_time;
        printf("Parallel Prefix Sum completed, Execution Time: %f seconds\n", run_time);
        free(vector);
    }
    
    free(local_vector);
    free(prefix_sum);
    MPI_Finalize();
    return 0;
}