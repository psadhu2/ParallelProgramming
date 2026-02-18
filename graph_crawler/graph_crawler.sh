#!/bin/bash
#SBATCH --job-name=pranav-graph-crawler
#SBATCH --partition=Centaurus
#SBATCH --mem=10G
#SBATCH --time=00:05:00

SOURCE_FILE="bfs.cpp"
EXECUTABLE="bfs.exe"

echo "Compiling $SOURCE_FILE..."
echo "Actor: $ACTOR"
echo "Depth: $DEPTH"

g++ "$SOURCE_FILE" -I. -o "$EXECUTABLE" -lcurl

if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

./"$EXECUTABLE" "$ACTOR" "$DEPTH"