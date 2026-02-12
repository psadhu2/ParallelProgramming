#!/bin/bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "${SCRIPT_DIR}"

PLOT_SCRIPT="plot.py"

if [[ ! -f "${PLOT_SCRIPT}" ]]; then
    echo "Error: ${PLOT_SCRIPT} not found in ${SCRIPT_DIR}"
    exit 1
fi

# Usage:
#   ./n-body-plot-bench.sh
#   ./n-body-plot-bench.sh 0.0
#   ./n-body-plot-bench.sh 10000 bench-100.tsv bench-1000.tsv
#
# Default arrow scale if omitted:
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
