
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    
    MPI_Init(&argc, &argv); 

    int rank, size;    
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
    MPI_Comm_size(MPI_COMM_WORLD, &size); 

    if (rank == 0) {  // Process 0
        int data = 200;
        MPI_Send(&data, 1, MPI_INT, 1, 1, MPI_COMM_WORLD);
        printf("Process 0 sent value %d to Process 1\n", data);
    } 
    else if (rank == 1) {  // Process 1
        int received_data;
        MPI_Recv(&received_data, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 1 received value %d from Process 0\n", received_data);
    }

    MPI_Finalize();
    return 0;
}
