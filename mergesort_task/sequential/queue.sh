#!/bin/bash
#SBATCH --job-name=mergesort
#SBATCH --output=mergesort_%j.out
#SBATCH --error=mergesort_%j.err
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=8
#SBATCH --mem=4G
#SBATCH --time=00:30:00
#SBATCH --partition=Centaurus

# Build
make clean
make

echo "========================================"
echo "Merge Sort Benchmark"
echo "========================================"

SIZES=(100000 1000000 10000000)
THREADS=(1 2 4 8)

for SIZE in "${SIZES[@]}"; do
  echo ""
  echo "--- Array size: $SIZE ---"

  echo -n "Sequential (1 thread): "
  ./mergesort_par $SIZE 1 2>&1 | tail -1

  for T in "${THREADS[@]}"; do
    echo -n "Parallel ($T threads): "
    ./mergesort_par $SIZE $T 2>&1 | tail -1
  done

done

echo ""
echo "========================================"
echo "Done"
echo "========================================"
