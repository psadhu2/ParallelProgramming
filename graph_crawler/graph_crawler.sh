#!/bin/bash
#SBATCH --job-name=pranav-graph-crawler
#SBATCH --partition=Centaurus
#SBATCH --mem=10G
#SBATCH --time=00:05:00

SOURCE_FILE="bfs.cpp"
EXECUTABLE="bfs.exe"

# Print what we're doing
echo "Compiling $SOURCE_FILE..."
echo "Arguments: Actor='$1', Depth='$2'"

# Compile with error checking
g++ "$SOURCE_FILE" -I. -o "$EXECUTABLE" -lcurl

# Check if compilation succeeded
if [ $? -ne 0 ]; then
    echo "Compilation failed!"
    exit 1
fi

echo "Compilation successful. Running program..."

# Run with arguments
./"$EXECUTABLE" "$1" "$2"

# Check exit status
if [ $? -ne 0 ]; then
    echo "Program execution failed!"
    exit 1
fi

echo "Program completed successfully."