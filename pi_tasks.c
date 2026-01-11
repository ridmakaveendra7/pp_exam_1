#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

unsigned long my_rand(unsigned long* state,
                      unsigned long lower,
                      unsigned long upper) {
    *state ^= *state >> 12;
    *state ^= *state << 25;
    *state ^= *state >> 27;
    unsigned long result = (*state * 0x2545F4914F6CDD1DULL);
    unsigned long range = (upper > lower) ? (upper - lower) : 0UL;
    return (range > 0) ? (result % range + lower) : lower;
}
// integral computation of pi
double compute_pi(long num_steps) {
    double step = 1.0 / (double) num_steps;
    double sum = 0.0;

    for (long i = 0; i < num_steps; i++) {
        double x = (i + 0.5) * step;
        sum += 4.0 / (1.0 + x * x);
    }
    return step * sum;
}

int num_tasks;
unsigned long lower, upper;
double pi_sum = 0.0;
int created_tasks = 0;
int *tasks_per_thread;

void process_task(unsigned long task_seed) {
    int task_id;
    double pi;
    #pragma omp atomic capture
    task_id = created_tasks++;
    
    if (task_id >= num_tasks) return;
    unsigned long steps = my_rand(&task_seed, lower, upper + 1);
    #pragma omp task
    pi = compute_pi((long) steps);
    
    #pragma omp atomic
    pi_sum += pi;
    
    #pragma omp atomic
    tasks_per_thread[omp_get_thread_num()]++;
    
    unsigned long n_new = my_rand(&task_seed, 1, 5);
    for (unsigned long i = 0; i < n_new; i++) {
        unsigned long child_seed = task_seed + (task_id + 1) * 1009 + i * 17;
        
        #pragma omp task
        process_task(child_seed);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 6) {
        printf("Usage: %s num_tasks num_threads lower upper seed\n", argv[0]);
        return 1;
    }

    num_tasks = atoi(argv[1]);
    int num_threads = atoi(argv[2]);
    lower = atol(argv[3]);
    upper = atol(argv[4]);
    unsigned long seed = atol(argv[5]);

    tasks_per_thread = calloc(num_threads, sizeof(int));
    omp_set_num_threads(num_threads);
    
    double start_time = omp_get_wtime();

    #pragma omp parallel
    {
        #pragma omp single
        {
            process_task(seed);
        }
        #pragma omp taskwait
    }

    double end_time = omp_get_wtime();
    printf("Average pi: %.10f\n", pi_sum / created_tasks);

    for (int i = 0; i < num_threads; i++) {
        printf("Thread %d computed %d tasks\n", i, tasks_per_thread[i]);
    }

    printf("Execution took %.4f s\n", end_time - start_time);

    free(tasks_per_thread);
    return 0;
}
