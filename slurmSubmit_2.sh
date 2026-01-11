#!/usr/bin/env bash
####### Mail Notify / Job Name / Comment #######
#SBATCH --job-name="heatmap_analysis_quick"

####### Partition #######
#SBATCH --partition=all

####### Ressources #######
#SBATCH --time=0-00:05:00

####### Node Info #######
#SBATCH --exclusive
#SBATCH --nodes=1

####### Output #######
#SBATCH --output=/home/fd0001561/out/%x.out.%j
#SBATCH --error=/home/fd0001561/out/%x.err.%j

#cd /bin
OMP_CANCELLATION=true ./heatmap_analysis_quick $BASE_ARGS $OMP_NUM_THREADS $WORK_FACTOR
