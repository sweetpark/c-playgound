# 사용법:  make -C c_basic            # str/mem/fio/env/sig/err/api/conc/msc 의 직접 작성한 코드 전부 빌드
#          make -C c_basic lecture    # 강의 제공 코드 빌드 (의도적 버그 포함, 경고 허용)
#          make -C c_basic clean
CC      := gcc
# 15번(con) 실습이 pthread 를 쓰므로 -pthread (컴파일·링크 모두에 필요)
CFLAGS  := -std=gnu11 -g -O0 -Wall -Wextra -pthread

# main() 이 없는 함수 모음은 링크하지 않고 컴파일(-c)만 해서 문법·경고를 검사한다
# (msc/enc_dec.c 는 enc_dec_test 와 합쳐서 따로 링크하므로 여기서는 컴파일만 검사)
LIBSRCS := err/my_atoi.c err/snprintf.c api/conformant.c msc/enc_dec.c
LIBOBJS := $(patsubst %.c,build/%.o,$(LIBSRCS))

# 여러 소스 파일을 합쳐서 링크하는 프로그램 (아래 명시 규칙으로 빌드)
MULTI   := msc/enc_dec_test.c
MBINS   := build/conc/msgq/15-00 build/msc/enc_dec_test

SRCS    := $(filter-out $(LIBSRCS) $(MULTI),$(wildcard str/*.c mem/*.c fio/*.c env/*.c sig/*.c err/*.c api/*.c conc/*.c msc/*.c))
BINS    := $(patsubst %.c,build/%,$(SRCS))

# 컴파일되지 않는 강의 코드는 제외한다 (실습 중인 코드이거나 결함을 찾는 과제)
#   15-03 : pthread_join 인자가 1개 (too few arguments)
#   15-15 : 괄호 오류 (pthread_mutex_init, printf 줄)
#   tag_length_value : state_t 오타 (선언은 stat_t) — 코드 리뷰 과제
LSKIP   := lecture/conc/15-03.c lecture/conc/15-15.c lecture/msc/tag_length_value.c
LSRCS   := $(filter-out $(LSKIP),$(wildcard lecture/*/*.c))
LBINS   := $(patsubst %.c,build/%,$(LSRCS))

# 강의 코드의 의도적 버그(예: 13-03 의 close(FILE*) 선언 누락)는 gcc 14+ 에서 에러가 되므로 경고로 낮춘다
$(LBINS): CFLAGS += -Wno-error=implicit-function-declaration

all: $(BINS) $(MBINS) $(LIBOBJS)

lecture: $(LBINS)

build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

build/%: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@

# 메시지 큐 (15-00): main + 큐 구현을 함께 링크
build/conc/msgq/15-00: conc/msgq/15-00-main.c conc/msgq/15-00-q.c conc/msgq/15-00-q.h
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) conc/msgq/15-00-main.c conc/msgq/15-00-q.c -o $@

# 메시지 헤더 encode/decode 테스트 프로그램: 테스트 main + enc_dec 모듈
build/msc/enc_dec_test: msc/enc_dec_test.c msc/enc_dec.c msc/enc_dec.h
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) msc/enc_dec_test.c msc/enc_dec.c -o $@

clean:
	rm -rf build

.PHONY: all lecture clean
