#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <iterations> <threads>\n", argv[0]);
        return 1;
    }
    
    int iter = atoi(argv[1]), tnum = atoi(argv[2]), N = 0;
    double t = omp_get_wtime();
    
    #pragma omp parallel for num_threads(tnum) reduction(+:N)
    for (int i = 0; i < iter; i++) {
        int x = rand() % 100 - 50, y = rand() % 100 - 50;
        if (x * x + y * y <= 2500) N++;
    }
    
    t = omp_get_wtime() - t;
    printf("Time: %.4f\nPI: %.4f\n", t, 4.0 * N / iter);
    return 0;
}


/*

gcc -fopenmp -o pi_estimation 1.c

./pi_estimation 1000000 4
*/

