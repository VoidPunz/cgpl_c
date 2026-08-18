COL_RESET = \033[0m
COL_PRIMARY = \033[33m # Yellow
COL_SUCCESS = \033[32m # Green
COL_FAIL = \033[31m #Red

DISCARDED_WARNINGS = -Wno-int-to-pointer-cast -Wno-discarded-qualifiers
_CFLAGS = -std=c11
OPT = -O1
CC = gcc
CFLAGS = $(_CFLAGS) $(OPT) $(DISCARDED_WARNINGS)

OBJ_DIR = ../../obj
vpath %.o $(OBJ_DIR) # Look for %.o in the OBJ_DIR directory

EXE = .exe
RM = rm -f
CAT = cat

SRC = $(wildcard test_*.c)
OBJ = $(SRC:%.c=.o)
BIN = $(SRC:.c=$(EXE)) # test_*.c -> test_.exe*

.PHONY: all clean $(BIN)

define run_test
	@printf "$(COL_PRIMARY)Running test for file..."
	@./$(1) > test_run.log 2>&1 && \
		(printf "$(COL_SUCCESS)$(1): SUCCESS\n$(COL_RESET)" && $(RM) test_run.log) || \
		(printf "$(COL_FAIL)$(1): FAIL\n$(COL_RESET)" && $(CAT) test_run.log && $(RM) test_run.log && exit 1)
endef

all: $(BIN) clean

test_hashmap$(EXE): test_hashmap.c hashmap.o
test_list$(EXE): test_list.c list.o

$(BIN): test_%$(EXE): test_%.c %.o
	@printf "$(COL_PRIMARY)Building test source file... "
	$(CC) $(CFLAGS) $(OPT) $^ -o $@
	$(call run_test,$@)

clean:
	@printf "$(COL_PRIMARY)"
	rm -f $(BIN)
