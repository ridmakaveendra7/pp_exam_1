#!/usr/bin/env bash
####### Mail Notify / Job Name / Comment #######
#SBATCH --job-name="pi_tasks"

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
./pi_tasks $NUM_TASKS $OMP_NUM_THREADS $BASE_ARGS
