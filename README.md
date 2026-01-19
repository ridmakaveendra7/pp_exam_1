# OpenMP Heatmap & Pi Computation Project

This project contains multiple C programs using **OpenMP** for parallel computation, along with **SLURM batch scripts** for execution on an HPC cluster.  
A Makefile is provided to automate compilation and job submission.

---

## Requirements

- **Linux / HPC environment**
- **GCC** (with OpenMP support)
- **SLURM workload manager**

---

## Project Structure

```
.
|-- Makefile
|-- heatmap_analysis.c
|-- heatmap_analysis_quick.c
|-- pi_tasks.c
|-- slurmAll_1.sh
|-- slurmAll_2.sh
|-- slurmAll_3.sh
`-- README.md

````

---

## Workflow

### 1. Copy all files to the cluster
Use `scp` to transfer files from your local machine to the remote HPC server:

```bash
scp {files_on_local_machine} fd*****@hpc.informatik.hs-fulda.de:{working_directory_on_remote_server}
````

---

### 2. Compile all C sources

```bash
make build
```

This will generate the executables:

* `heatmap_analysis`
* `heatmap_analysis_quick`
* `pi_tasks`

---

### 3. Submit all SLURM jobs

```bash
make submit
```

---

### 4. Submit jobs individually (optional)

```bash
# Submit the heatmap_analysis job
make submit_task1

# Submit the heatmap_analysis_quick job
make submit_task2

# Submit the pi_tasks job
make submit_task3
```

---

### 5. Configure output directory in SLURM scripts

By default, SLURM output files are generated based on the paths specified in the SLURM scripts.
If you want to store them in a custom directory, edit the following lines in the corresponding `slurmAll_*.sh` scripts:

```bash
####### Output #######
#SBATCH --output=/{out_location}/out/%x.out.%j
#SBATCH --error=/{out_location}/out/%x.err.%j
```

* Replace `{out_location}` with the relative or absolute path of your working directory.
* Create an `out` directory at that location if it does not exist:

```bash
mkdir -p {out_location}/out
```

---

### 6. Output file naming format

* **Standard output file**: `{program_name}_{thread_count}.out.{task_id}`
* **Error file**: `{program_name}_{thread_count}.err.{task_id}`

Example:

```
heatmap_analysis_4.out.12345
heatmap_analysis_4.err.12345
```

---

### 7. Clean build and output files (optional)

To remove compiled executables and SLURM output files:

```bash
make clean
```

This will delete:

* All compiled binaries (`heatmap_analysis`, `heatmap_analysis_quick`, `pi_tasks`)
* All `.out` and `.err` files

