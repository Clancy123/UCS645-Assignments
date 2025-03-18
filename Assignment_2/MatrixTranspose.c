#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 4  // Matrix size (N x N)

// Function to print the matrix
void printMatrix(int* mat) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%2d ", mat[i * N + j]);
        }
        printf("\n");
    }
}

int main(int argc, char** argv) {
    int rank, num_procs;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    // Allocate memory for the matrix and transposed matrix
    int* matrix = (int*)malloc(N * N * sizeof(int));
    int* transposed = (int*)malloc(N * N * sizeof(int));

    // Initialize matrices to 0
    for (int i = 0; i < N * N; i++) {
        matrix[i] = 0;
        transposed[i] = 0;
    }

    // Rank 0 initializes the matrix
    if (rank == 0) {
        printf("Original Matrix:\n");
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                matrix[i * N + j] = i * N + j + 1;
            }
        }
        printMatrix(matrix);
    }

    // Broadcast the matrix to all processes
    MPI_Bcast(matrix, N * N, MPI_INT, 0, MPI_COMM_WORLD);

    // Each process transposes a part of the matrix
    for (int i = rank; i < N; i += num_procs) {
        for (int j = 0; j < N; j++) {
            transposed[j * N + i] = matrix[i * N + j];
        }
    }

    // Gather the transposed parts from all processes
    MPI_Reduce(rank == 0 ? MPI_IN_PLACE : transposed, transposed,
               N * N, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Rank 0 prints the transposed matrix
    if (rank == 0) {
        printf("\nTransposed Matrix:\n");
        printMatrix(transposed);
    }

    // Free memory
    free(matrix);
    free(transposed);

    MPI_Finalize();
    return 0;
}