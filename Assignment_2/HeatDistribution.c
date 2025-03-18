#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define SIZE 10         // Grid size (10x10)
#define ITERATIONS 100  // Number of time steps

// Function to print the grid
void printGrid(double* grid) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%6.2f ", grid[i * SIZE + j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main(int argc, char** argv) {
    int rank, num_procs;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    // Define the full grid as a 1D array
    double* grid = (double*)malloc(SIZE * SIZE * sizeof(double));
    double* newGrid = (double*)malloc(SIZE * SIZE * sizeof(double));

    // Initialize grids to 0.0
    for (int i = 0; i < SIZE * SIZE; i++) {
        grid[i] = 0.0;
        newGrid[i] = 0.0;
    }

    // Rank 0 initializes the heat source
    if (rank == 0) {
        grid[4 * SIZE + 4] = 100.0;  // Heat source at (4,4)
        printf("Initial Grid (Rank 0 before Bcast):\n");
        printGrid(grid);
    }

    // Broadcast the initial grid to all processes
    MPI_Bcast(grid, SIZE * SIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Divide work among processes
    int rows_per_proc = SIZE / num_procs;
    int start_row = rank * rows_per_proc;
    int end_row = (rank == num_procs - 1) ? SIZE - 1 : (rank + 1) * rows_per_proc - 1;

    for (int iter = 0; iter < ITERATIONS; iter++) {
        // Copy old values before updating
        for (int i = 0; i < SIZE * SIZE; i++) {
            newGrid[i] = grid[i];
        }

        // Compute heat diffusion for assigned rows (excluding boundaries)
        for (int i = start_row; i <= end_row; i++) {
            for (int j = 1; j < SIZE - 1; j++) {
                if (i > 0 && i < SIZE - 1) {  // Avoid out-of-bounds
                    if (!(i == 4 && j == 4)) {  // Keep heat source constant
                        newGrid[i * SIZE + j] = 0.25 * (
                            grid[(i - 1) * SIZE + j] + grid[(i + 1) * SIZE + j] +
                            grid[i * SIZE + (j - 1)] + grid[i * SIZE + (j + 1)]);
                    }
                }
            }
        }

        // Gather updated data from all processes
        MPI_Allgather(&newGrid[start_row * SIZE], rows_per_proc * SIZE, MPI_DOUBLE,
                      grid, rows_per_proc * SIZE, MPI_DOUBLE, MPI_COMM_WORLD);
    }

    // Final output (only rank 0)
    if (rank == 0) {
        printf("Final Heat Distribution:\n");
        printGrid(grid);
    }

    // Free memory
    free(grid);
    free(newGrid);

    MPI_Finalize();
    return 0;
}