# 사용법(WSL 안에서):  make D=d1_pure T=step1
CC       := clang
D        ?= d1_pure
T        ?= main
CFLAGS   := -Wall -Wextra -Werror -pedantic -std=c11 -g -Icommon -I$(D)/src
SAN      := -fsanitize=address,undefined -fno-omit-frame-pointer

SRC      := $(wildcard $(D)/src/*.c $(D)/test/*.c)
BIN      := build/$(D)_$(T)

all: run

build:
	@mkdir -p build

$(BIN): build $(SRC)
	$(CC) $(CFLAGS) $(SAN) $(SRC) -o $(BIN)

run: $(BIN)
	@echo "── run ──────────────────────────────"
	@./$(BIN)

# 위생 검사: 경고 0 + 새니타이저 0 이어야 통과
check: $(BIN)
	@./$(BIN) && echo "PASS" || (echo "FAIL"; exit 1)

clean:
	rm -rf build

.PHONY: all run check clean build
