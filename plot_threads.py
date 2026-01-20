import matplotlib.pyplot as plt

threads = [1, 2, 4, 8, 16, 32, 64]

print("Enter execution time for each thread count:")

times = []

for t in threads:
    value = float(input(f"Time for {t} threads: "))
    times.append(value)

plot_name = input("Enter plot name/title: ")

plt.figure()
plt.plot(threads, times, marker="o")

plt.xlabel("Thread Count")
plt.ylabel("Execution Time")
plt.title(plot_name)

plt.xticks(range(0, 68, 8))

for x, y in zip(threads, times):
    plt.text(x, y, f"({x}, {y:.4f})")

plt.grid(True)

filename = plot_name.replace(" ", "_") + ".png"
plt.savefig(filename)

print(f"Plot saved as {filename}")

