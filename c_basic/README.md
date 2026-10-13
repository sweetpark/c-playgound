# c_basic

C 기초 강의(문자열·동적 메모리·파일 I/O) 실습 코드. 오류가 있는 강의 코드를 고쳐 보고, 직접 구현하며 개선한 최종 버전을 둔다.
(템플릿·시큐어코딩 체화 드릴은 상위 폴더의 `d?_*`, `sd??_*`를 쓴다 — 이 폴더는 별개)

## 구성

| 폴더 | 파일 | 내용 | 블로그 노트 |
|---|---|---|---|
| `str/` | `str_ex.c` | `my_strncpy`, `my_strncat`, `make_filename` | 문자열 함수 실습 |
| `mem/` | `mem_ex.c` | 유연 배열 구조체 + `realloc` 이중 포인터 `append` | 동적 메모리 실습 |
| `mem/` | `linkedlist.c` | node + list 구조체 연결리스트 (유연 배열 멤버) | 동적 메모리 실습 |
| `fio/` | `fio_ex.c` | `stat` 검사 + 임시파일 + `rename`으로 공백 제거 복사 | 파일 입출력 실습 |
| `fio/` | `fio_ex2.c` | 줄 수·최대 길이 + 줄 번호 붙여 저장 | 파일 입출력 실습 |
| `lecture/` | `8-xx`, `9-xx`, `10-xx` | 강의에서 제공된 코드 (수정·주석 포함) | 각 노트 본문 인용 |

각 파일은 독립된 프로그램이다. 버전은 파일을 덮어쓰고 커밋하는 방식으로 git 히스토리에 남긴다.

## 빌드·실행 (Linux / WSL)

```bash
make -C c_basic                 # str, mem, fio 전체 빌드 → c_basic/build/
make -C c_basic lecture         # 강의 코드 빌드 (의도적 버그가 있어 경고가 나올 수 있음)

./c_basic/build/mem/linkedlist c_basic/lecture/mem/9-09.txt
./c_basic/build/fio/fio_ex c_basic/fio/sample/src.txt /tmp/dst.txt
./c_basic/build/fio/fio_ex2 c_basic/fio/sample/src.txt /tmp/out.txt
```

- `fio_ex`, `fio_ex2`는 **POSIX 함수**(`stat`, `mkstemp`, `fsync`, `fdopen` 등)를 쓰므로 Linux/macOS/WSL에서 빌드한다.
- `fio_ex2`는 dst가 이미 있으면 실패한다(`O_EXCL`). 다시 실행하려면 출력 파일을 지운다.
- `str_ex`는 실행 위치에 `base_name_<날짜>[n]` 파일을 만든다(`make_filename` 시연).
- 디버깅: `-g -O0`으로 빌드되므로 바로 `gdb c_basic/build/<경로>`로 쓸 수 있다.
