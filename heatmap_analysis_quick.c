#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

unsigned long my_rand(unsigned long* state, unsigned long lower, unsigned long upper) {
    *state ^= *state >> 12;
    *state ^= *state << 25;
    *state ^= *state >> 27;

    unsigned long result = (*state * 0x2545F4914F6CDD1DULL);
    unsigned long range = (upper > lower) ? (upper - lower) : 0UL;

    return (range > 0) ? (result % range + lower) : lower;
}

unsigned concatenate(unsigned x, unsigned y) {
    unsigned pow = 10;
    while (y >= pow)
        pow *= 10;

    return x * pow + y;
}


unsigned long hash(unsigned long x) {
    x ^= (x >> 21);
    x *= 2654435761UL;
    x ^= (x >> 13);
    x *= 2654435761UL;
    x ^= (x >> 17);
    return x;
}



int main(int argc, char* argv[]) {
    double start = omp_get_wtime();

    if (argc != 10) {
        printf("Usage: %s <columns> <rows> <seed> <lower> <upper> <height> <verbose> <num_threads> <work_factor>\n", argv[0]);
        return 1;
    }

    int columns = atoi(argv[1]);
    int rows = atoi(argv[2]);
    unsigned long seed = strtoll(argv[3], NULL, 10);
    unsigned long lower = strtoll(argv[4], NULL, 10);
    unsigned long upper = strtoll(argv[5], NULL, 10);
    int window_height = atoi(argv[6]);
    int verbose = atoi(argv[7]);
    int num_threads = atoi(argv[8]);
    int work_factor = atoi(argv[9]);

    printf("Starting heatmap_analysis\n");
    printf("Parameters: columns: %d, rows: %d, seed: %lu, lower: %lu, upper: %lu, window height: %d, verbose: %d, num threads: %d, work factor: %d\n", columns, rows, seed, lower, upper, window_height, verbose, num_threads, work_factor);

    omp_set_num_threads(num_threads);

    unsigned long **A = malloc(rows * sizeof(unsigned long *));
    #pragma omp parallel for
    for (int i = 0; i < rows; ++i)
        A[i] = malloc(columns * sizeof(unsigned long));

    unsigned long * maximums = malloc(columns * sizeof(unsigned long));
    unsigned long * row_hotspots = malloc(rows * sizeof(unsigned long));
    // Initialize arrays to zero
    #pragma omp parallel for
    for (int i = 0; i < columns; ++i) maximums[i] = 0;
    #pragma omp parallel for
    for (int i = 0; i < rows; ++i) row_hotspots[i] = 0; 

    // Initialize array A
    #pragma omp parallel for collapse(2)
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            unsigned long s = seed * concatenate(i, j);
            A[i][j] = my_rand(&s, lower, upper);
        }
    }

    for (int w = 0; w < work_factor; ++w)
    {
        #pragma omp parallel for collapse(2)
        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < columns; ++j)
            {
                A[i][j] = hash(A[i][j]);
            }
        }
    }
    
    unsigned long total_hotspots = 0;
    
    // Hotspot analysis
    int abort_program = 0;

    #pragma omp parallel shared(A, row_hotspots) reduction(+:total_hotspots)
    {
        #pragma omp for
        for (int i = 0; i < rows; ++i) {
            int local_row_hotspots = 0;

            for (int j = 0; j < columns; ++j) {
                bool is_hotspot = true;

                if (i > 0 && A[i-1][j] >= A[i][j]) is_hotspot = false;
                if (i < rows-1 && A[i+1][j] >= A[i][j]) is_hotspot = false;
                if (j > 0 && A[i][j-1] >= A[i][j]) is_hotspot = false;
                if (j < columns-1 && A[i][j+1] >= A[i][j]) is_hotspot = false;

                if (is_hotspot) {
                    local_row_hotspots++;
                    total_hotspots++;
                }
            }

            row_hotspots[i] = local_row_hotspots;

            if (local_row_hotspots == 0) {
                printf("Row %d contains no hotspots\n", i);
                abort_program = 1;
                #pragma omp cancel for
            }

            #pragma omp cancellation point for
        }
    }

    // early exit if not hotspots found in a row
    if (abort_program) {
        printf("Early exit\n");
        double end_time = omp_get_wtime();
        printf("Execution took: %f s\n", end_time - start);
        for (int k = 0; k < rows; ++k) free(A[k]);
        free(A);
        free(maximums);
        free(row_hotspots);
        return 1;
    }

    if (verbose) {
        printf("A:\n");
        #pragma omp parallel for
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                printf("%lu ", A[i][j]);
            }
            printf("\n");
        }
    }
    // max sliding sum calculation
    #pragma omp parallel for
    for (int j = 0; j < columns; ++j) {
        unsigned long maximum = 0;
        unsigned long sum = 0;
        
        for (int i = 0; i < rows; ++i) {
            sum += A[i][j];                   
            if (i >= window_height) {         
                sum -= A[i - window_height][j];
            }
            if (i >= window_height - 1) {
                if (sum > maximum) maximum = sum;
            }
        }
        
        maximums[j] = maximum;
    }

    // Print maximum sliding sums per column
    if (verbose) {
        printf("Max sliding sums per column:\n");
        #pragma omp parallel for
        for (int j = 0; j < columns; ++j) {
            printf("%lu", maximums[j]);
            if (j != columns - 1) printf(", ");
        }
        printf("\n");
    }

    // Count and print hotspots 
    if (verbose) {
        printf("Hotspots per row:\n");
        #pragma omp parallel for
        for (int i = 0; i < rows; ++i) {
            printf("Row %d: %lu hotspot(s)\n", i, row_hotspots[i]);
        }
    }
 
    printf("Total hotspots found: %lu\n", total_hotspots);

    double end = omp_get_wtime();
    printf("Execution took: %f s\n", end - start);

    
    for (int i = 0; i < rows; ++i) {
        free(A[i]);
    }
    free(A);

    free(maximums);
    free(row_hotspots);

    return 0;
}
