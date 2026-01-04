CC = gcc
CFLAGS = -Wall -Wextra -std=c99

OBJ = console.o trafficjam.o
TARGET = trafficjam

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

console.o: console.c console.h
	$(CC) $(CFLAGS) -c console.c

trafficjam.o: trafficjam.c console.h
	$(CC) $(CFLAGS) -c trafficjam.c

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)
