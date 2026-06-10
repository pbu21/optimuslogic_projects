import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("heap.csv")

plt.figure(figsize=(10,5))
plt.plot(df["Iteration"], df["FreeHeap"])

plt.xlabel("Iteration")
plt.ylabel("Free Heap (Bytes)")
plt.title("ESP32 Heap Fragmentation Test")
plt.grid(True)

plt.savefig("heap_graph.png")
plt.show()