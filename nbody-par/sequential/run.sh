#!/bin/bash
#SBATCH --job-name=nbody
#SBATCH --output=nbody_%j.out
#SBATCH --error=nbody_%j.err
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=8
#SBATCH --mem=4G
#SBATCH --time=00:30:00
#SBATCH --partition=Centaurus

# Set OpenMP threads to match allocated CPUs
export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK

echo "Running on node: $SLURMD_NODENAME"
echo "Using $OMP_NUM_THREADS OpenMP threads"
echo "Job ID: $SLURM_JOB_ID"
echo "----------------------------------------"

# Remove old binary to force recompile on this node's architecture
rm -f nbody

# Build with make
echo "Building..."
make

# Run the executable
echo "----------------------------------------"
echo "Running nbody.exe..."
chmod +x nbody
echo "Running solar system simulation..."
./nbody planet 200 5000000 10000 $OMP_NUM_THREADS > solar.out

echo "Running random simulation..."
./nbody 1000 1 10000 100 $OMP_NUM_THREADS > random.out