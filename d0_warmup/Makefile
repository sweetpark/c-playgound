# 사용법 (WSL / macOS 공통, d0_warmup 디렉터리에서 실행): make
CC       := clang
CFLAGS   := -Wall -Wextra -Werror -pedantic -std=c11 -g -I../common -Wno-gnu-zero-variadic-macro-arguments
SAN      := -fsanitize=address,undefined -fno-omit-frame-pointer

SRC      := main.c
BIN      := build/main

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
