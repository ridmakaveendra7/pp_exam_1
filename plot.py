import matplotlib.pyplot as plt

threads = [1, 2, 4, 8, 16, 32, 64]

print("Enter execution time for each thread count:")

times = []

for t in threads:
    value = float(input(f"Time for {t} threads: "))
    times.append(value)

plot_name = input("Enter plot name/title: ")

T1 = times[0]                      
speedup = [T1 / tp for tp in times]

plt.figure()
plt.plot(threads, speedup, marker="o")

plt.xlabel("Thread Count")
plt.ylabel("Speedup")
plt.title(plot_name)

plt.xticks(range(0, 68, 8))

plt.grid(True)

filename = plot_name + ".png"
plt.savefig(filename)

print(f"Plot saved as {filename}")