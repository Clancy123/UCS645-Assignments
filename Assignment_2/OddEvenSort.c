#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function to compare and split two arrays
void compareSplit(int* local_data, int* recv_data, int local_n, int keep_small) {
    int* merged = (int*)malloc(2 * local_n * sizeof(int));
    int i = 0, j = 0, k = 0;

    // Merge the two arrays
    while (i < local_n && j < local_n) {
        if (local_data[i] < recv_data[j]) {
            merged[k++] = local_data[i++];
        } else {
            merged[k++] = recv_data[j++];
        }
    }
    while (i < local_n) merged[k++] = local_data[i++];
    while (j < local_n) merged[k++] = recv_data[j++];

    // Keep the smaller or larger half
    if (keep_small) {
        for (i = 0; i < local_n; i++) local_data[i] = merged[i];
    } else {
        for (i = 0; i < local_n; i++) local_data[i] = merged[local_n + i];
    }

    free(merged);
}

int main(int argc, char* argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 16;  // Total number of elements (should be divisible by number of processes)
    int local_n = n / size;
    int* local_data = (int*)malloc(local_n * sizeof(int));

    srand(rank + time(0));  // Seed for randomness
    for (int i = 0; i < local_n; i++)
        local_data[i] = rand() % 100;  // Random values between 0 and 99

    // Local sorting
    for (int i = 0; i < local_n - 1; i++) {
        for (int j = 0; j < local_n - i - 1; j++) {
            if (local_data[j] > local_data[j + 1]) {
                int temp = local_data[j];
                local_data[j] = local_data[j + 1];
                local_data[j + 1] = temp;
            }
        }
    }

    printf("Process %d initial array: ", rank);
    for (int i = 0; i < local_n; i++) printf("%d ", local_data[i]);
    printf("\n");

    // Odd-Even Sorting
    for (int phase = 0; phase < size; phase++) {
        int partner = (phase % 2 == 0) ? (rank % 2 == 0 ? rank + 1 : rank - 1)
                                      : (rank % 2 == 0 ? rank - 1 : rank + 1);

        if (partner >= 0 && partner < size) {
            int* recv_data = (int*)malloc(local_n * sizeof(int));
            MPI_Sendrecv(local_data, local_n, MPI_INT, partner, 0,
                         recv_data, local_n, MPI_INT, partner, 0,
                         MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            compareSplit(local_data, recv_data, local_n, rank < partner);
            free(recv_data);
        }
    }

    // Gather the sorted subarrays back to rank 0
    int* sorted_data = NULL;
    if (rank == 0) sorted_data = (int*)malloc(n * sizeof(int));
    MPI_Gather(local_data, local_n, MPI_INT,
               sorted_data, local_n, MPI_INT, 0, MPI_COMM_WORLD);

    // Rank 0 prints the sorted array
    if (rank == 0) {
        printf("\nSorted Array: ");
        for (int i = 0; i < n; i++) printf("%d ", sorted_data[i]);
        printf("\n");
        free(sorted_data);
    }

    free(local_data);
    MPI_Finalize();
    return 0;
}