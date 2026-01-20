#!/usr/bin/env bash

THREADS_LIST=(1 2 4 8 16 32 64)

# WITHOUT thread count
BASE_ARGS="30 200000 42 0 10000000 2 0"

WORK_FACTOR=1000

for t in "${THREADS_LIST[@]}"; do
    sbatch \
      --export=OMP_NUM_THREADS=$t,BASE_ARGS="$BASE_ARGS",WORK_FACTOR="$WORK_FACTOR",OMP_CANCELLATION=true \
      --job-name=heatmap_analysis_quick_$t \
      slurmSubmit_2.sh
done
