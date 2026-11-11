# 하루 한 문제 드릴 — 사용법은 README.md. WSL(Ubuntu) 또는 macOS 에서 실행.
# 기본 목표는 없다: make P001 처럼 번호를 준다.
.PHONY: clean list verify


# ══ 하루 한 문제 드릴 ═══════════════════════════════════════════
#   make P001            drill/P001_*/solution.c 를 빌드·채점 (gcc, -Werror, ASan/UBSan)
#   make P001 ANS=1      solutions/ 의 모범답안으로 채점 (풀고 난 뒤 확인용)
#   make P001 SKEL=1     백지(skeleton.c)로 채점 — 반드시 FAIL 이어야 정상
#   make reset-P001      solution.c 를 백지로 되돌림 (반복 연습용, 작성 중인 내용은 사라짐)
#   make list            문제 목록 / make verify  전체 채점 가능성 검증
DRILL_CC     ?= gcc
DRILL_CFLAGS := -Wall -Wextra -Werror -std=c11 -D_POSIX_C_SOURCE=200809L -g -DUNIT_TEST -Idrill/common -Idrill
DRILL_SAN    := -fsanitize=address,undefined -fno-sanitize-recover=all -fno-omit-frame-pointer
DRILL_LDLIBS ?= -pthread

P%:
	@d=$$(ls -d drill/$@_* 2>/dev/null | head -n 1); \
	if [ -z "$$d" ]; then echo "문제 없음: $@  (make list 로 목록 확인)"; exit 2; fi; \
	if [ -n "$(ANS)" ] || [ -n "$(SKEL)" ]; then \
	  s=$$(ls -d solutions/$@_* | head -n 1); w=build/work/$@; \
	  rm -rf $$w; mkdir -p $$w; cp $$d/*.c $$d/*.h $$d/ldlibs $$w/ 2>/dev/null; \
	  if [ -n "$(ANS)" ]; then cp $$s/solution.c $$w/solution.c; else cp $$s/skeleton.c $$w/solution.c; fi; \
	  d=$$w; \
	fi; \
	mkdir -p build; \
	echo "── $@  ($$d) ──"; \
	$(DRILL_CC) $(DRILL_CFLAGS) $(DRILL_SAN) -I$$d $$d/test.c $(DRILL_LDLIBS) $$(cat $$d/ldlibs 2>/dev/null) -o build/$@ && ./build/$@

reset-P%:
	@d=$$(ls -d drill/P$*_* | head -n 1); s=$$(ls -d solutions/P$*_* | head -n 1); \
	cp $$s/skeleton.c $$d/solution.c && echo "reset: $$d/solution.c (백지로 복원)"

list:
	@for d in drill/P*_*; do echo "$${d#drill/}"; done

verify:
	@sh drill/verify.sh

clean:
	@rm -rf build
