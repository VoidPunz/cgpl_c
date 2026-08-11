export

COL_RESET = \033[0m
COL_PRIMARY = \033[34m # Blue

CC = gcc
_CFLAGS = -std=c11 -Wall -l m
OPT = -O2
CFLAGS = $(_CFLAGS) $(OPT)
TARGET = cgpl.exe

SRC_DIR = src
OBJ_DIR = obj
TEST_DIR = tests/unit_tests

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

.PHONY = release debug test clean

release: $(TARGET)

debug: CFLAGS = $(_CFLAGS) -g -O0 -DDEBUG
debug: $(TARGET)

$(TARGET): $(OBJ)
	@printf "$(COL_PRIMARY)"
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)
	@printf "$(COL_RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@printf "$(COL_PRIMARY)"
	$(CC) $(CFLAGS) -c $< -o $@
	@printf "$(COL_RESET)"

$(OBJ_DIR):
	@printf "$(COL_PRIMARY)"
	mkdir -p $(OBJ_DIR)
	@printf "$(COL_RESET)"

test:
	@printf "$(COL_PRIMARY)"
	$(MAKE) -C $(TEST_DIR) CFLAGS="$(_CFLAGS)"
	@printf "$(COL_RESET)"

# del /q src\*.o $(TARGET) # Use this if on windows and not using VSC terminal or Powershell
clean:
	@printf "$(COL_PRIMARY)"
	rm -f $(OBJ_DIR)/*.o $(TARGET)
	@printf "$(COL_RESET)"