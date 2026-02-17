#!/bin/bash
#SBATCH --job-name=pranav-graph-crawler
#SBATCH --partition=Centaurus
#SBATCH --mem=10G
#SBATCH --time=00:05:00

SOURCE_FILE="bfs.cpp"

EXECUTABLE="bfs.exe"

g++ "$SOURCE_FILE" -I. -o "$EXECUTABLE" -lcurl

QUERY="$1"
DEPTH="$2"

if [ -z "$QUERY" ] || [ -z "$DEPTH" ]; then
  echo "Usage: sbatch graph_crawler.sh \"Actor or Movie\" <depth>"
  exit 1
fi

echo "Running with query=[$QUERY], depth=[$DEPTH]"
./"$EXECUTABLE" "$QUERY" "$DEPTH"
