#!/bin/bash
#SBATCH --job-name=pranav-graph-crawler
#SBATCH --partition=Centaurus
#SBATCH --mem=10G
#SBATCH --time=00:05:00

set -euo pipefail

SOURCE_FILE="bfs.cpp"
EXECUTABLE="bfs"
QUERY="${1:-}"
DEPTH="${2:-}"

echo "Host: $(hostname)"
echo "Compiler: $(g++ --version | head -n 1)"

if [ -z "$QUERY" ] || [ -z "$DEPTH" ]; then
  echo "Usage: sbatch graph_crawler.sh \"Actor or Movie\" <depth>"
  exit 1
fi

echo "Compiling $SOURCE_FILE -> $EXECUTABLE"
if ! g++ -std=c++17 -O2 "$SOURCE_FILE" -I. -o "$EXECUTABLE" -lcurl; then
  echo "Compile failed."
  exit 1
fi

echo "Running with query=[$QUERY], depth=[$DEPTH]"
./"$EXECUTABLE" "$QUERY" "$DEPTH"
