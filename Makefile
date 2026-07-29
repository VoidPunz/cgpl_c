CC = gcc
CFLAGS = -std=c11 -Wall -O2
TARGET = cgpl.exe

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

all: $(TARGET)

debug: CFLAGS = -Wall -g -O0 -DDEBUG
debug: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o $(TARGET)