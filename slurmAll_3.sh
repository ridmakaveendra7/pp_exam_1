#!/usr/bin/env bash

THREADS_LIST=(1 2 4 8 16 32 64)

# WITHOUT thread count
BASE_ARGS="100000 1000000 42"
NUM_TASKS=15000

for t in "${THREADS_LIST[@]}"; do
    sbatch \
      --export=OMP_NUM_THREADS=$t,BASE_ARGS="$BASE_ARGS",NUM_TASKS="$NUM_TASKS" \
      --job-name=pi_tasks_$t \
      slurmSubmit_3.sh
done
