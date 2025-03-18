
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 1000000

int main(int argc, char** argv) 
{    
    int rank, size;
    int count = 0, total_count = 0;
    int points;
    double x, y,z;

    //Init MPI
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    points = N / size;
    
    srand(time(NULL) + rank);
    
    for (int i = 0; i < points; i++) {
        x = (double)rand() / RAND_MAX;
        y = (double)rand() / RAND_MAX;
        z = x * x + y * y;
        if (z <= 1) {
            count++;
        }
    }

    MPI_Reduce(&count, &total_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        double pi = 4.0 * total_count / N;
        printf("Estimated Pi = %f\n", pi);
    }

    // Finalize MPI
    MPI_Finalize();
    return 0;
}
