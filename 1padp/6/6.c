#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <omp.h>

#define COUNT 7
#define WORD_SIZE 20

char *FILE_NAME;
char words[COUNT][WORD_SIZE] = {"Hi", "the", "Originium", "here", "mount", "inference", "Ursus"};
int word_count[COUNT];

void read_word(char *temp, FILE *fp) {
    int i = 0;
    char ch;
    while ((ch = fgetc(fp)) != EOF && !isalpha(ch));
    while (ch != EOF && isalpha(ch)) {
        temp[i++] = ch;
        ch = fgetc(fp);
    }
    temp[i] = '\0';
}

int get_word_count(int num) {
    FILE *fp = fopen(FILE_NAME, "r");
    if (!fp) return 0;

    char temp[WORD_SIZE];
    int count = 0;

    while (!feof(fp)) {
        read_word(temp, fp);
        if (strcasecmp(words[num], temp) == 0)
            count++;
    }
    fclose(fp);
    return count;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <filename> <num_threads>\n", argv[0]);
        return 1;
    }

    FILE_NAME = argv[1];
    int n_threads = atoi(argv[2]);

    double t = omp_get_wtime();
    #pragma omp parallel for num_threads(n_threads)
    for (int i = 0; i < COUNT; i++)
        word_count[i] = get_word_count(i);
    t = omp_get_wtime() - t;

    for (int i = 0; i < COUNT; i++)
        printf("%s: %d\n", words[i], word_count[i]);

    printf("Time: %.4f seconds\n", t);
    return 0;
}

