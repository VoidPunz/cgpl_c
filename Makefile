CC = gcc
_CFLAGS = -std=c11 -Wall -l m
OPT = -O2
CFLAGS = $(_CFLAGS) $(OPT)
TARGET = cgpl.exe

SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)

release: $(TARGET) clean_objs

debug: CFLAGS = $(_CFLAGS) -g -O0 -DDEBUG
debug: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean_objs:
	rm -f src/*.o

# del /q src\*.o $(TARGET) # Use this if on windows and not using VSC terminal or Powershell
clean:
	rm -f src/*.o $(TARGET)