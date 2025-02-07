#include <stdio.h>
#include <string.h>
#include <mpi.h>
#define SIZE 4
#define BUFSIZE 32
#define ROOT 0
#define TAG 0
char* messages[SIZE] = { "", "HELLO", "CSE", "RVCE" };
char msg[BUFSIZE];
int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if (size != SIZE) {
        printf("Error: Requires exactly 4 processes\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }
    if (rank != ROOT) {
        strcpy(msg, messages[rank]);
        MPI_Send(msg, BUFSIZE, MPI_CHAR, ROOT, TAG, MPI_COMM_WORLD);
    } else {
        for (int i = 1; i < size; i++) {
            MPI_Recv(msg, BUFSIZE, MPI_CHAR, i, TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("Received: %s\n", msg);
        }
    }
    MPI_Finalize();
    return 0;
}
/*
mpicc mpi_msg.c -o mpi_msg
mpirun --oversubscribe -np 4 ./mpi_msg
*/

