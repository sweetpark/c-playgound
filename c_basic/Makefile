# 사용법:  make -C c_basic            # str/mem/fio/env/sig/err/api 의 직접 작성한 코드 전부 빌드
#          make -C c_basic lecture    # 강의 제공 코드 빌드 (의도적 버그 포함, 경고 허용)
#          make -C c_basic clean
CC      := gcc
CFLAGS  := -std=gnu11 -g -O0 -Wall -Wextra

# main() 이 없는 함수 모음은 링크하지 않고 컴파일(-c)만 해서 문법·경고를 검사한다
LIBSRCS := err/my_atoi.c err/snprintf.c api/conformant.c
LIBOBJS := $(patsubst %.c,build/%.o,$(LIBSRCS))

SRCS    := $(filter-out $(LIBSRCS),$(wildcard str/*.c mem/*.c fio/*.c env/*.c sig/*.c err/*.c api/*.c))
BINS    := $(patsubst %.c,build/%,$(SRCS))

LSRCS   := $(wildcard lecture/*/*.c)
LBINS   := $(patsubst %.c,build/%,$(LSRCS))

# 강의 코드의 의도적 버그(예: 13-03 의 close(FILE*) 선언 누락)는 gcc 14+ 에서 에러가 되므로 경고로 낮춘다
$(LBINS): CFLAGS += -Wno-error=implicit-function-declaration

all: $(BINS) $(LIBOBJS)

lecture: $(LBINS)

build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

build/%: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -rf build

.PHONY: all lecture clean
