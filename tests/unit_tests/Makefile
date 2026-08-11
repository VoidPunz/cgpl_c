COL_RESET = \033[0m
COL_PRIMARY = \033[33m # Yellow
COL_SUCCESS = \033[32m # Green
COL_FAIL = 033[31m #Red

CC = gcc
CFLAGS = -std=c11 -Wall -l m
OPT = -O1

OBJ_DIR = ../../obj
vpath %.o $(OBJ_DIR) # Look for %.o in the OBJ_DIR directory

SRC = $(wildcard test_*.c)
OBJ = $(SRC:$%.c=.o)
BIN = $(SRC:.c=) # test_*.c -> test_*

.PHONY = all clean $(BIN)

define run_test
	@if ./$(1) > /dev/null 2>&1; then \
		printf "$(COL_SUCCESS)$(1): SUCCESS$(COL_RESET)\n"; \
	else \
		printf "$(COL_FAIL)$(1): FAIL$(COL_RESET)\n"; exit 1; \
	fi
endef

all: $(BIN) clean

test_hashmap: test_hashmap.c hashmap.o
test_list: test_list.c list.o

test_%: test_%.c %.o
	@printf "$(COL_PRIMARY)Building and running test for $*..... "
	$(CC) $(CFLAGS) $^ -o $@
	$(call run_test,$@)

clean:
	@printf "$(COL_PRIMARY)"
	rm -f $(BIN)
	@printf "$(COL_RESET)"
