CC := gcc
CFLAGS := -Wall -Wextra -Werror -g -I../../common/v1

SRC := main.c
BIN := build/main

all : run


build :
	@mkdir -p build

$(BIN): build $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(BIN)

run: $(BIN)
	@echo "---run---"
	@./$(BIN)

clean:
	rm -rf build

.PHONY: all run clean build
