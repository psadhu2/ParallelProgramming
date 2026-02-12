#!/bin/bash
#SBATCH --job-name=nbody-plot
#SBATCH --partition=Centaurus
#SBATCH --mem=10G
#SBATCH --time=02:00:00
#SBATCH --output=nbody-plot-%j.out

set -euo pipefail


if [[ -n "${SLURM_SUBMIT_DIR:-}" ]]; then
    cd "${SLURM_SUBMIT_DIR}"
else
    cd "$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
fi

WORK_DIR="$(pwd)"
PLOT_SCRIPT="plot.py"

if [[ ! -f "${PLOT_SCRIPT}" ]]; then
    echo "Error: ${PLOT_SCRIPT} not found in ${WORK_DIR}"
    exit 1
fi


ARROW_SCALE="0.0"

if [[ $# -ge 1 ]]; then
    ARROW_SCALE="$1"
    shift
fi

declare -a TSV_FILES

if [[ $# -gt 0 ]]; then
    TSV_FILES=("$@")
else
    mapfile -t TSV_FILES < <(ls -1 bench-*.tsv 2>/dev/null || true)
fi

if [[ ${#TSV_FILES[@]} -eq 0 ]]; then
    echo "No TSV files found. Pass file names explicitly or generate bench-*.tsv first."
    exit 1
fi

echo "Using arrow scale: ${ARROW_SCALE}"

for tsv in "${TSV_FILES[@]}"; do
    if [[ ! -f "${tsv}" ]]; then
        echo "Skipping missing file: ${tsv}"
        continue
    fi

    pdf="${tsv%.tsv}.pdf"
    echo "Plotting ${tsv} -> ${pdf}"
    python "${PLOT_SCRIPT}" "${tsv}" "${pdf}" "${ARROW_SCALE}"
done

echo "Done."
