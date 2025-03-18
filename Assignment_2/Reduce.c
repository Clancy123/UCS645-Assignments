
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 1000  // Number of elements

int main(int argc, char** argv) {
    int rank, size;
    int local_n;
    double *data = NULL;
    double local_sum = 0.0, global_sum = 0.0;
    double start_time, run_time;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    local_n = SIZE / size;
    double *local_data = (double*)malloc(local_n * sizeof(double));
    
    if (rank == 0) {
        data = (double*)malloc(SIZE * sizeof(double));
        srand(time(NULL));
        for (int i = 0; i < SIZE; i++) {
            data[i] = rand() % 100;
        }
        start_time = MPI_Wtime();
    }
    
    MPI_Scatter(data, local_n, MPI_DOUBLE, local_data, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    
    for (int i = 0; i < local_n; i++) {
        local_sum += local_data[i];
    }
    
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if (rank == 0) {
        run_time = MPI_Wtime() - start_time;
        printf("Parallel Reduction Sum: %f, Execution Time: %f seconds\n", global_sum, run_time);
        free(data);
    }
    
    free(local_data);
    MPI_Finalize();
    return 0;
}