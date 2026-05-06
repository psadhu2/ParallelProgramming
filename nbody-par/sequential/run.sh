#!/bin/bash
#SBATCH --job-name=nbody
#SBATCH --output=nbody_%j.out
#SBATCH --error=nbody_%j.err
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=8
#SBATCH --mem=4G
#SBATCH --time=00:30:00

# Set OpenMP threads to match allocated CPUs
export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK

echo "Running on node: $SLURMD_NODENAME"
echo "Using $OMP_NUM_THREADS OpenMP threads"
echo "Job ID: $SLURM_JOB_ID"
echo "----------------------------------------"

# Build with make
echo "Building..."
make clean
make

# Run the executable
echo "----------------------------------------"
echo "Running nbody.exe..."
./nbody.exe
