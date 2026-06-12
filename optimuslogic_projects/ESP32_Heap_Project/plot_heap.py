import serial
import matplotlib.pyplot as plt

ser = serial.Serial('COM7', 115200)  # Change COM port

iterations = []
heap_values = []

plt.ion()

while True:
    try:
        line = ser.readline().decode().strip()

        if ',' in line:
            i, heap = line.split(',')

            iterations.append(int(i))
            heap_values.append(int(heap))

            plt.clf()
            plt.plot(iterations, heap_values)

            plt.xlabel("Iteration")
            plt.ylabel("Free Heap (Bytes)")
            plt.title("ESP32 Heap Fragmentation Test")
            plt.grid(True)

            plt.pause(0.01)

    except:
        pass