#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

void init(int *X, int N) {
    for (int i = 0; i < N * N; i++) X[i] = rand() % 100;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <size> <threads>\n", argv[0]);
        return 1;
    }
    
    int N = atoi(argv[1]), tnum = atoi(argv[2]);
    int *A = malloc(N * N * sizeof(int)), *B = malloc(N * N * sizeof(int)), *C = calloc(N * N, sizeof(int));
    
    init(A, N);
    init(B, N);
    double start = omp_get_wtime();
    
    #pragma omp parallel for num_threads(tnum) collapse(2)
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            for (int k = 0; k < N; k++)
                C[i * N + j] += A[i * N + k] * B[k * N + j];
    
    double end = omp_get_wtime();
    printf("Time taken: %.4f seconds\n", end - start);
    
    free(A), free(B), free(C);
    return 0;
}


/*gcc -fopenmp -o matrix_mul 2.c
./matrix_mul 500 4
*/
