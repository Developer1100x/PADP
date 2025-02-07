#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <float.h>
#define MAX_N 10  // Default max matrix size

double max(double a, double b) {
    return (a > b) ? a : b;
}
void print_matrix(double *A, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%.2f\t", A[n * i + j]);
        printf("\n");
    }
}
int main(int argc, char *argv[]) {
    int n = (argc > 1) ? atoi(argv[1]) : 5;
    int max_iter = (argc > 2) ? atoi(argv[2]) : 10;
    double tol = (argc > 3) ? atof(argv[3]) : 0.01;
    int n2 = n + 2;
    double *T = (double *)calloc(n2 * n2, sizeof(double));
    double *T_new = (double *)calloc(n2 * n2, sizeof(double));

    if (!T || !T_new) {
        printf("Memory allocation failed!\n");
        return 1;
    }
    for (int i = 1; i <= n; i++) {
        T[n * n2 + i] = T[n2 * i + n] = (i * 100.0) / (n + 1);
    }
    double start_time = omp_get_wtime();
    double max_diff = DBL_MAX;
    for (int iter = 1; iter <= max_iter && max_diff > tol; iter++) {
        max_diff = 0;
        // OpenMP Parallelized Loop
        #pragma omp parallel for reduction(max:max_diff) collapse(2)
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n; j++) {
                T_new[n2 * i + j] = 0.25 * (T[n2 * i + j - 1] + T[n2 * i + j + 1] +
                                            T[n2 * (i - 1) + j] + T[n2 * (i + 1) + j]);
                max_diff = max(max_diff, fabs(T_new[n2 * i + j] - T[n2 * i + j]));
            }
        }
        double *temp = T;
        T = T_new;
        T_new = temp;
    }
    double end_time = omp_get_wtime();
    printf("Time taken: %.4f seconds\n", end_time - start_time);
    free(T);
    free(T_new);
    return 0;
}

/*

gcc -fopenmp -o heat_equation 7.c -lm
./heat_equation 5 10 0.01


*/
