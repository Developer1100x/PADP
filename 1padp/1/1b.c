#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <time.h>

int process(int iter) {
    int count = 0;
    for (int i = 0; i < iter; i++) {
        float x = (float)rand() / RAND_MAX;
        float y = (float)rand() / RAND_MAX;
        if (x * x + y * y <= 1) count++;
    }
    return count;
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int rank, size, N, local_count, total_count;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    if (argc != 2) {
        if (rank == 0) printf("Usage: %s <iterations_per_process>\n", argv[0]);
        MPI_Finalize();
        return 1;
    }
    
    N = atoi(argv[1]);
    srand(time(NULL) + rank);
    local_count = process(N);
    
    MPI_Reduce(&local_count, &total_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    
    if (rank == 0) printf("PI = %.4f\n", 4.0 * total_count / (N * size));
    
    MPI_Finalize();
    return 0;
}


/*
mpicc -o mpi_pi 1b.c


mpiexec --oversubscribe -np 4 ./mpi_pi 1000000

*/
