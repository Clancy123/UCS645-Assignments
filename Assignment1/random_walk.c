
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int location;
    int num_steps_left_in_walk;
} Walker;

void decompose_domain(int domain_size, int world_rank, int world_size, int* subdomain_start, int* subdomain_size) {
    *subdomain_start = (domain_size / world_size) * world_rank;
    *subdomain_size = domain_size / world_size;
    if (world_rank == world_size - 1) {
        *subdomain_size += domain_size % world_size;
    }
}

void initialize_walkers(int num_walkers_per_proc, int max_walk_size, int subdomain_start, Walker* walkers) {
    for (int i = 0; i < num_walkers_per_proc; i++) {
        walkers[i].location = subdomain_start;
        walkers[i].num_steps_left_in_walk = rand() % (max_walk_size + 1);
    }
}

void walk(Walker* walker, int subdomain_start, int subdomain_size, int domain_size, Walker* outgoing_walkers, int* out_count) {
    while (walker->num_steps_left_in_walk > 0) {
        if (walker->location >= subdomain_start + subdomain_size) {
            if (walker->location >= domain_size) {
                walker->location = 0;
            }
            outgoing_walkers[*out_count] = *walker;
            (*out_count)++;
            break;
        } else {
            walker->num_steps_left_in_walk--;
            walker->location++;
        }
    }
}

void send_outgoing_walkers(Walker* outgoing_walkers, int out_count, int world_rank, int world_size) {
    MPI_Send(outgoing_walkers, out_count * sizeof(Walker), MPI_BYTE, (world_rank + 1) % world_size, 0, MPI_COMM_WORLD);
}

void receive_incoming_walkers(Walker* incoming_walkers, int* in_count, int world_rank, int world_size) {
    MPI_Status status;
    int incoming_rank = (world_rank == 0) ? world_size - 1 : world_rank - 1;
    MPI_Probe(incoming_rank, 0, MPI_COMM_WORLD, &status);
    MPI_Get_count(&status, MPI_BYTE, in_count);
    *in_count /= sizeof(Walker);
    MPI_Recv(incoming_walkers, *in_count * sizeof(Walker), MPI_BYTE, incoming_rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
}

int main(int argc, char** argv) {
    if (argc < 4) {
        printf("Usage: %s domain_size max_walk_size num_walkers_per_proc\n", argv[0]);
        return 1;
    }

    int domain_size = atoi(argv[1]);
    int max_walk_size = atoi(argv[2]);
    int num_walkers_per_proc = atoi(argv[3]);

    MPI_Init(&argc, &argv);
    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    srand(time(NULL) * world_rank);
    int subdomain_start, subdomain_size;
    decompose_domain(domain_size, world_rank, world_size, &subdomain_start, &subdomain_size);

    Walker incoming_walkers[num_walkers_per_proc];
    Walker outgoing_walkers[num_walkers_per_proc];
    int out_count = 0;

    initialize_walkers(num_walkers_per_proc, max_walk_size, subdomain_start, incoming_walkers);
    printf("Process %d initialized %d walkers in subdomain %d - %d\n", world_rank, num_walkers_per_proc, subdomain_start, subdomain_start + subdomain_size - 1);

    int max_sends_recvs = max_walk_size / (domain_size / world_size) + 1;
    for (int m = 0; m < max_sends_recvs; m++) {
        out_count = 0;
        for (int i = 0; i < num_walkers_per_proc; i++) {
            walk(&incoming_walkers[i], subdomain_start, subdomain_size, domain_size, outgoing_walkers, &out_count);
        }
        printf("Process %d sending %d outgoing walkers\n", world_rank, out_count);
        send_outgoing_walkers(outgoing_walkers, out_count, world_rank, world_size);
        int in_count = 0;
        receive_incoming_walkers(incoming_walkers, &in_count, world_rank, world_size);
        printf("Process %d received %d incoming walkers\n", world_rank, in_count);
    }
    printf("Process %d done\n", world_rank);
    MPI_Finalize();
    return 0;
}
