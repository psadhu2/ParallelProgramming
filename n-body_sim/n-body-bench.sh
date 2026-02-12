#!/bin/bash
#SBATCH --job-name=nbody-bench
#SBATCH --partition=Centaurus
#SBATCH --mem=10G
#SBATCH --time=02:00:00
#SBATCH --output=nbody-bench-%j.out

set -euo pipefail

# Under Slurm, run from the directory where sbatch was invoked.
# Fallback to script directory for local/manual runs.
if [[ -n "${SLURM_SUBMIT_DIR:-}" ]]; then
    cd "${SLURM_SUBMIT_DIR}"
else
    SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
    cd "${SCRIPT_DIR}"
fi

BIN="./nbody"
SRC="main.cpp"
SUMMARY_FILE="benchmark-summary.txt"

echo "Building ${BIN} from ${SRC}..."
g++ -std=c++17 -O3 -march=native -DNDEBUG "${SRC}" -o "${BIN}"

echo "Benchmark run on $(date)" | tee "${SUMMARY_FILE}"
echo "Node: $(hostname)" | tee -a "${SUMMARY_FILE}"
echo "" | tee -a "${SUMMARY_FILE}"

run_case() {
    local label="$1"
    shift
    local out_file="$1"
    shift

    echo "Running: ${label}" | tee -a "${SUMMARY_FILE}"
    local elapsed
    elapsed=$(/usr/bin/time -f "%e" "${BIN}" "$@" "${out_file}" 2>&1 >/dev/null)
    echo "Elapsed seconds: ${elapsed}" | tee -a "${SUMMARY_FILE}"
    echo "" | tee -a "${SUMMARY_FILE}"
}

# 1) Solar system from file, dt=200, steps=5,000,000
run_case \
    "Solar system (from solar.tsv), dt=200, steps=5000000" \
    "bench-solar.tsv" \
    "solar.tsv" 200 5000000 10000

# 2) Random 100 particles, dt=1, steps=10,000
run_case \
    "Random 100 particles, dt=1, steps=10000" \
    "bench-100.tsv" \
    100 1 10000 100

# 3) Random 1000 particles, dt=1, steps=10,000
run_case \
    "Random 1000 particles, dt=1, steps=10000" \
    "bench-1000.tsv" \
    1000 1 10000 100

echo "Done. Summary written to ${SUMMARY_FILE}"
