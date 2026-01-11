CC = gcc
CFLAGS = -Wall -O2 -fopenmp
TARGET = pi_tasks
SRC = pi_tasks.c


all: $(TARGET)


$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)


run: $(TARGET)
	./$(TARGET) 20000 16 10000 1000000 42

clean:
	rm -f $(TARGET)
