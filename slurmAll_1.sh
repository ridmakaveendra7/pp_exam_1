#!/usr/bin/env bash

THREADS_LIST=(1 2 4 8 16 32 64)

# WITHOUT thread count
BASE_ARGS="10240 7860 1337 0 100 20 0"

WORK_FACTOR=70

for t in "${THREADS_LIST[@]}"; do
    sbatch \
      --export=OMP_NUM_THREADS=$t,BASE_ARGS="$BASE_ARGS",WORK_FACTOR="$WORK_FACTOR" \
      --job-name=heatmap_analysis_$t \
      slurmSubmit_1.sh
done
