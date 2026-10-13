# 사용법:  make -C c_basic            # str/mem/fio 의 직접 작성한 코드 전부 빌드
#          make -C c_basic lecture    # 강의 제공 코드 빌드 (의도적 버그 포함, 경고 허용)
#          make -C c_basic clean
CC      := gcc
CFLAGS  := -std=gnu11 -g -O0 -Wall -Wextra

SRCS    := $(wildcard str/*.c mem/*.c fio/*.c)
BINS    := $(patsubst %.c,build/%,$(SRCS))

LSRCS   := $(wildcard lecture/*/*.c)
LBINS   := $(patsubst %.c,build/%,$(LSRCS))

all: $(BINS)

lecture: $(LBINS)

build/%: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf build

.PHONY: all lecture clean
