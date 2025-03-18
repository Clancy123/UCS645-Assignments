#include <mpi.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

// Function to check if a number is prime
bool is_prime(int num) {
    if (num <= 1) return false;
    if (num <= 3) return true;
    if (num % 2 == 0 || num % 3 == 0) return false;
    for (int i = 5; i * i <= num; i += 6) {
        if (num % i == 0 || num % (i + 2) == 0) return false;
    }
    return true;
}

int main(int argc, char* argv[]) {
    int rank, size;
    MPI_Init(&argc, &argv);  // Initialize MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int max_value = 100;  // Maximum value to test for primes
    int num_primes = 0;   // Counter for the number of primes found

    if (rank == 0) {
        // Master Process
        int next_num = 2;  // Start testing from 2
        int result;
        MPI_Status status;

        // Loop until all numbers up to max_value are tested
        while (next_num <= max_value) {
            // Receive a request from any slave
            MPI_Recv(&result, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);

            if (result == 0) {
                // Slave is just starting, send the next number to test
                MPI_Send(&next_num, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD);
                next_num++;
            } else if (result > 0) {
                // Positive result: the number is prime
                printf("%d is prime.\n", result);
                num_primes++;
            } else {
                // Negative result: the number is not prime
                printf("%d is not prime.\n", -result);
            }
        }

        // Send termination signal to all slaves
        int terminate = -1;
        for (int i = 1; i < size; i++) {
            MPI_Send(&terminate, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        printf("Total primes found: %d\n", num_primes);

    } else {
        // Slave Process
        int num;
        int result;

        while (1) {
            // Send a request to the master for a number to test
            result = 0;
            MPI_Send(&result, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

            // Receive a number to test from the master
            MPI_Recv(&num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (num == -1) {
                // Termination signal received
                break;
            }

            // Test if the number is prime
            if (is_prime(num)) {
                result = num;  // Send the number if it is prime
            } else {
                result = -num; // Send the negative of the number if it is not prime
            }

            // Send the result back to the master
            MPI_Send(&result, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }

    MPI_Finalize(); // Finalize MPI
    return 0;
}