# c_basic

C 기초 강의(문자열·동적 메모리·파일 I/O·환경변수·시그널·에러 처리·API 설계·동시성·바이트 순서·컴파일러 경고) 실습 코드. 오류가 있는 강의 코드를 고쳐 보고, 직접 구현하며 개선한 최종 버전을 둔다.
(템플릿·시큐어코딩 체화 드릴은 상위 폴더의 `d?_*`, `sd??_*`를 쓴다 — 이 폴더는 별개)

## 구성

| 폴더 | 파일 | 내용 | 블로그 노트 |
|---|---|---|---|
| `str/` | `str_ex.c` | `my_strncpy`, `my_strncat`, `make_filename` | 문자열 함수 실습 |
| `mem/` | `mem_ex.c` | 유연 배열 구조체 + `realloc` 이중 포인터 `append` | 동적 메모리 실습 |
| `mem/` | `linkedlist.c` | node + list 구조체 연결리스트 (유연 배열 멤버) | 동적 메모리 실습 |
| `fio/` | `fio_ex.c` | `stat` 검사 + 임시파일 + `rename`으로 공백 제거 복사 | 파일 입출력 실습 |
| `fio/` | `fio_ex2.c` | 줄 수·최대 길이 + 줄 번호 붙여 저장 | 파일 입출력 실습 |
| `env/` | `env_ex.c` | `LOG_PATH` 로그 기록기 (`open`+`fdopen`, `atexit` clean-up) | 환경변수·시그널 실습 |
| `env/` | `env_ex2_v1.c`, `env_ex2_v2.c` | PATH 탐색기(which). v1 = 초안, v2 = `strtok` 대신 직접 분리·빈 항목 `.`·`atexit` | 환경변수·시그널 실습 |
| `sig/` | `sig_ex.c`, `sig_ex_v2.c` | 1초 카운터. v2 = `sigaction` + `nanosleep` 남은 시간 재대기 | 환경변수·시그널 실습 |
| `sig/` | `sig_ex2.c`, `sig_ex2_v2.c` | 시그널로 운영되는 카운터. v2 = `sigprocmask` + `pselect` 절대 시각 | 환경변수·시그널 실습 |
| `err/` | `my_atoi.c` | `strtol` 로 만든 예외 반환 atoi (main 없음, 컴파일만 검사) | 에러 처리·API 설계 실습 |
| `err/` | `snprintf.c` | in-band 에러를 없앤 가변 인자 `snprintf_m` (main 없음) | 에러 처리·API 설계 실습 |
| `err/` | `sum.c` | `sum_file` + `main` : 줄 단위 합계, `SUM_E*` 에러 코드 | 에러 처리·API 설계 실습 |
| `api/` | `conformant.c` | conformant 배열 파라미터 `improved_strncpy/strncat` (main 없음) | 에러 처리·API 설계 실습 |
| `conc/` | `con_ex.c` | 이체 스레드 4 + 감시 스레드 1 + main. 계좌별 mutex, 락 획득 순서 고정(번호 오름차순), 전체 락 스냅샷으로 합계 검사, 스레드별 통계, atomic 종료 flag | 동시성·스레드 실습 |
| `conc/msgq/` | `15-00-main.c`, `15-00-q.c`, `15-00-q.h` | 스레드 안전 메시지 큐(mutex + 조건변수 2개 + `close_q`)와 producer/consumer. SIGINT/SIGTERM은 `sigwait` 전용 스레드가 받아 `close_q` 호출. 원본은 `#else`, 수정본은 `#if MODIFY` | 동시성·스레드 실습 |
| `msc/` | `byteorder.c` | 64비트 `htonll`/`ntohll` + 네트워크 순서·왕복·`__builtin_bswap64` 검증 | 바이트 순서·컴파일러 경고 실습 |
| `msc/` | `enc_dec.h`, `enc_dec.c` | 16바이트 메시지 헤더 encode/decode (필드 단위 변환, 원인별 음수 에러, 실패 시 출력 불변), `msg_type_str` | 바이트 순서·컴파일러 경고 실습 |
| `msc/` | `enc_dec_test.c` | 위 모듈의 테스트 프로그램 (hex 출력, 왕복, 에러 리턴, 34개 검사) | 바이트 순서·컴파일러 경고 실습 |
| `lecture/` | `8-xx` ~ `16-xx` | 강의에서 제공된 코드 (수정·주석 포함). `conc/`은 15-01 ~ 15-15, `msc/`는 16-01 ~ 16-05와 `tag_length_value.c`(코드 리뷰 과제) | 각 노트 본문 인용 |

각 파일은 독립된 프로그램이다. 버전은 파일을 덮어쓰고 커밋하는 방식으로 git 히스토리에 남긴다.

## 빌드·실행 (Linux / WSL)

```bash
make -C c_basic                 # str, mem, fio, env, sig, err, api, con, msc 전체 빌드 → c_basic/build/
make -C c_basic lecture         # 강의 코드 빌드 (의도적 버그가 있어 경고가 나올 수 있음)

./c_basic/build/mem/linkedlist c_basic/lecture/mem/9-09.txt
./c_basic/build/fio/fio_ex c_basic/fio/sample/src.txt /tmp/dst.txt
./c_basic/build/fio/fio_ex2 c_basic/fio/sample/src.txt /tmp/out.txt
```

- `fio_ex`, `fio_ex2`는 **POSIX 함수**(`stat`, `mkstemp`, `fsync`, `fdopen` 등)를 쓰므로 Linux/macOS/WSL에서 빌드한다.
- `fio_ex2`는 dst가 이미 있으면 실패한다(`O_EXCL`). 다시 실행하려면 출력 파일을 지운다.
- `str_ex`는 실행 위치에 `base_name_<날짜>[n]` 파일을 만든다(`make_filename` 시연).
- 디버깅: `-g -O0`으로 빌드되므로 바로 `gdb c_basic/build/<경로>`로 쓸 수 있다.

### env / sig / err / api 실습 실행 예

```bash
LOG_PATH=mylog ./c_basic/build/env/env_ex            # mylog.log 에 입력을 기록, q 또는 quit 으로 종료
./c_basic/build/env/env_ex2_v2 -a ls                 # PATH 에서 ls 를 모두 찾기 (종료 코드 0/1/2)

cd c_basic/sig/sample                                # 12-07.conf 를 읽으므로 이 폴더에서 실행
../../build/sig/sig_ex2_v2 &                         # kill -HUP/-USR1/-USR2/-INT/-TERM <PID> 로 조작

./c_basic/build/err/sum 파일1 [파일2 ...]              # 각 파일의 줄별 정수 합계 (main 포함)
```

- `err/my_atoi.c`, `err/snprintf.c`, `api/conformant.c`는 `main`이 없는 **함수 모음**이라 링크하지 않고 `build/*.o`로 컴파일만 해서 경고를 검사한다.
- `sig_ex*`는 실행 위치에 `count.txt`를 만든다. `.gitignore`에 들어 있다.
- `lecture/api/14-04-cli.c`, `14-04-serv.c`는 한 쌍의 non-blocking 소켓 예제(서버를 먼저 실행).

### con / msc 실습 실행 예

```bash
./c_basic/build/msc/enc_dec_test                # 헤더 encode 결과 hex 출력 + 왕복 + 에러 리턴 (전부 통과하면 종료 코드 0)
./c_basic/build/msc/byteorder                   # htonll/ntohll 검증

./c_basic/build/conc/con_ex                      # 10초 동안 이체·감시, 마지막에 스레드별 성공/실패와 최종 합계 출력
gcc -std=gnu11 -Wall -Wextra -pthread -DRUN_SEC=2 c_basic/conc/con_ex.c -o /tmp/con_ex   # 실행 시간 줄이기

./c_basic/build/conc/msgq/15-00 3 100            # consumer 3개, producer 대기 100ms. 실행 중 Ctrl-C 로 정상 종료
```

- `conc/msgq`의 `15-00-q.h` 맨 위 `#define MODIFY 1`을 `0`으로 바꾸면 수정 전 원본(동기화 없음)으로 빌드된다. 원본은 스레드 안전하지 않아 반복 실행하면 가끔 크래시·정지한다.
- `lecture/conc/15-04.c`는 `SomeNetworkFile`(임의의 텍스트 파일)을 실행 위치에서 읽는다. 용량이 커서 저장소에는 넣지 않았다.
- `lecture/conc/15-03.c`(`pthread_join` 인자 오류), `15-15.c`(괄호 오류), `lecture/msc/tag_length_value.c`(`state_t` 오타)는 컴파일되지 않는 상태라 `make lecture`에서 제외했다 (`Makefile`의 `LSKIP`).
- 컨테이너의 gcc가 4.8이라 ThreadSanitizer를 쓸 수 없었다. 레이스는 반복 실행과 "락을 뺀 변이 버전이 실제로 실패하는지"로 확인했다. 최신 gcc/clang에서는 `-fsanitize=thread`를 추가로 돌려 볼 수 있다.
- 강의 코드 `13-03.c`는 의도적으로 `close(FILE *)`를 쓰므로 gcc 14+ 에서는 `-Wno-error=implicit-function-declaration`으로 경고로 낮춰 빌드한다(`make lecture`).
