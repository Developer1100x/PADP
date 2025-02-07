#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <omp.h>

void print_primes(bool *X, long N) {
    for (long i = 2; i < N; i++)
        if (X[i]) printf("%ld ", i);
    printf("\n");
}
void sieve_unfriendly(long N) {
    bool *X = malloc(N * sizeof(bool));
    for (long i = 2; i < N; i++) X[i] = 1;
    double t = omp_get_wtime();
    for (long i = 2; i < N; i++) {
        if (!X[i]) continue;
        for (long j = i * i; j < N; j += i) X[j] = 0;  // Marks multiples
    }
    t = omp_get_wtime() - t;
    int count = 0;
    for (long i = 2; i < N; i++) if (X[i]) count++;
    printf("Cache-unfriendly: Time = %.6f sec, Primes = %d\n", t, count);
    free(X);
}
void sieve_friendly(long N) {
    bool *X = malloc(N * sizeof(bool));
    int root_N = sqrt(N), count = 0;
    int primes[root_N], p_count = 0;
    for (long i = 2; i < N; i++) X[i] = 1;
    double t = omp_get_wtime();
    for (int i = 2; i <= root_N; i++) {
        if (X[i]) {
            primes[p_count++] = i;
            for (long j = i * i; j <= root_N; j += i) X[j] = 0;
        }
    }
    for (long i = root_N + 1; i < N; i += root_N) {
        long end = (i + root_N - 1 < N) ? (i + root_N - 1) : N;
        for (int j = 0; j < p_count; j++)
            for (long k = (i % primes[j] == 0) ? i : i + (primes[j] - i % primes[j]); k <= end; k += primes[j])
                X[k] = 0;

        for (long k = i; k <= end; k++) if (X[k]) count++;
    }
    t = omp_get_wtime() - t;
    printf("Cache-friendly: Time = %.6f sec, Primes = %d\n", t, count + p_count);
    free(X);
}
void sieve_parallel_friendly(long N) {
    bool *X = malloc(N * sizeof(bool));
    int root_N = sqrt(N), count = 0;
    int primes[root_N], p_count = 0;
    for (long i = 2; i < N; i++) X[i] = 1;
    double t = omp_get_wtime();
    for (int i = 2; i <= root_N; i++) {
        if (X[i]) {
            primes[p_count++] = i;
            for (long j = i * i; j <= root_N; j += i) X[j] = 0;
        }
    }
    #pragma omp parallel for reduction(+:count)
    for (long i = root_N + 1; i < N; i += root_N) {
        long end = (i + root_N - 1 < N) ? (i + root_N - 1) : N;
        for (int j = 0; j < p_count; j++)
            for (long k = (i % primes[j] == 0) ? i : i + (primes[j] - i % primes[j]); k <= end; k += primes[j])
                X[k] = 0;

        for (long k = i; k <= end; k++) if (X[k]) count++;
    }
    t = omp_get_wtime() - t;
    printf("Parallel Cache-friendly: Time = %.6f sec, Primes = %d\n", t, count + p_count);
    free(X);
}
int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <N>\n", argv[0]);
        return 1;
    }
    long N = atol(argv[1]) + 1;

    sieve_unfriendly(N);
    sieve_friendly(N);
    sieve_parallel_friendly(N);
    return 0;
}



/*
gcc -o sieve 3.c -fopenmp -lm

./sieve 1000000
*/
