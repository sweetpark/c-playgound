# c_basic

C 기초 강의(문자열·동적 메모리·파일 I/O·환경변수·시그널·에러 처리·API 설계) 실습 코드. 오류가 있는 강의 코드를 고쳐 보고, 직접 구현하며 개선한 최종 버전을 둔다.
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
| `lecture/` | `8-xx` ~ `14-xx` | 강의에서 제공된 코드 (수정·주석 포함) | 각 노트 본문 인용 |

각 파일은 독립된 프로그램이다. 버전은 파일을 덮어쓰고 커밋하는 방식으로 git 히스토리에 남긴다.

## 빌드·실행 (Linux / WSL)

```bash
make -C c_basic                 # str, mem, fio, env, sig, err, api 전체 빌드 → c_basic/build/
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
- 강의 코드 `13-03.c`는 의도적으로 `close(FILE *)`를 쓰므로 gcc 14+ 에서는 `-Wno-error=implicit-function-declaration`으로 경고로 낮춰 빌드한다(`make lecture`).
