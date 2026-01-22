import matplotlib.pyplot as plt
import numpy as np
import pandas as pd
from pathlib import Path

csv_path = Path(__file__).with_name("results.csv")
df = pd.read_csv(csv_path)


plt.figure()
plt.plot(df["input size"], df["time"], marker="o")
plt.xscale("log")
plt.xlabel("Input Size (N)")
plt.ylabel("Time (seconds)")
plt.title("MergeSort Benchmark")
plt.grid(True)

plt.show()
