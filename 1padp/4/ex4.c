#include <stdio.h>
#include <stdlib.h>
#include <gd.h>
#include <omp.h>

void process(gdImagePtr img, int w, int h, omp_sched_t sched, const char *sched_name, const char *out_file) {
    omp_set_schedule(sched, 50); // Set scheduling type with chunk size
    gdImagePtr img_copy = gdImageClone(img); // Clone original image to avoid modifying the same one

    double t = omp_get_wtime();

    #pragma omp parallel for collapse(2) schedule(runtime)
    for (int x = 0; x < w; x++)
        for (int y = 0; y < h; y++) {
            int c = gdImageGetPixel(img_copy, x, y);
            int gray = 0.2126 * gdImageRed(img_copy, c) + 
                       0.7152 * gdImageGreen(img_copy, c) + 
                       0.0722 * gdImageBlue(img_copy, c);
            gdImageSetPixel(img_copy, x, y, gdImageColorAllocate(img_copy, gray, gray, gray));
        }

    printf("%s: %.4f sec\n", sched_name, omp_get_wtime() - t);

    FILE *ofp = fopen(out_file, "wb");
    if (ofp) {
        gdImagePng(img_copy, ofp);
        fclose(ofp);
    } else {
        perror("Error saving file");
    }

    gdImageDestroy(img_copy);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <in.png> <out_prefix>\n", argv[0]);
        return 1;
    }

    FILE *ifp = fopen(argv[1], "rb");
    if (!ifp) return perror("Error opening input file"), 1;

    gdImagePtr img = gdImageCreateFromPng(ifp);
    fclose(ifp);
    if (!img) return fprintf(stderr, "Error reading PNG\n"), 1;

    int w = gdImageSX(img), h = gdImageSY(img);

    char out_file[256];
    snprintf(out_file, sizeof(out_file), "%s_default.png", argv[2]);
    process(img, w, h, omp_sched_auto, "Default", out_file);

    snprintf(out_file, sizeof(out_file), "%s_static.png", argv[2]);
    process(img, w, h, omp_sched_static, "Static", out_file);

    snprintf(out_file, sizeof(out_file), "%s_dynamic.png", argv[2]);
    process(img, w, h, omp_sched_dynamic, "Dynamic", out_file);

    snprintf(out_file, sizeof(out_file), "%s_guided.png", argv[2]);
    process(img, w, h, omp_sched_guided, "Guided", out_file);

    gdImageDestroy(img);

    return 0;
}


/*
gcc -fopenmp -o ex4 ex4.c -lgd
 ./ex4 rvce.png output.png
 
*/

