CC = gcc
CFLAGS = -Wall -O2 -fopenmp
SRC = heatmap_analysis.c heatmap_analysis_quick.c pi_tasks.c
TARGETS = heatmap_analysis heatmap_analysis_quick pi_tasks

SLURM_SCRIPTS = slurmAll_1.sh slurmAll_2.sh slurmAll_3.sh

fix-crlf:
	@echo "Converting CRLF to LF in source and SLURM files..."
	@sed -i 's/\r$$//' *

# Compile all C programs
build: fix-crlf $(TARGETS)

# Rules for each target
heatmap_analysis: heatmap_analysis.c
	$(CC) $(CFLAGS) -o $@ $<

heatmap_analysis_quick: heatmap_analysis_quick.c
	$(CC) $(CFLAGS) -o $@ $<

pi_tasks: pi_tasks.c
	$(CC) $(CFLAGS) -o $@ $<

# Submit all SLURM scripts
submit:
	@echo "Submitting all SLURM jobs..."
	@for job in $(SLURM_SCRIPTS); do \
		echo "Submitting $$job"; \
		sbatch $$job; \
	done

# Submit SLURM scripts individually
submit_task1:
	@echo "Submitting slurmAll_1.sh"
	sbatch slurmAll_1.sh

submit_task2:
	@echo "Submitting slurmAll_2.sh"
	sbatch slurmAll_2.sh

submit_task3:
	@echo "Submitting slurmAll_3.sh"
	sbatch slurmAll_3.sh


# Clean build and output files
clean:
	rm -f $(TARGETS) *.out *.err
	cd ../out
	rm -f *
