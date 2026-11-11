# P003 · build_path — snprintf 반환값, NULL 과 "" 는 다르다

## 요구사항
디렉터리 `dir` 와 파일명 `file` 을 `/` 로 이어 `out` 에 만든다. 이어 붙이는 `/` 는 필요할 때만 넣는다.
- `dir` 가 `""`(빈 문자열)이면 `file` 만 결과가 된다. `dir` 가 `NULL` 이면 오류다 — **둘은 다른 것**이다.
- `dir` 가 이미 `/` 로 끝나면 `/` 를 또 넣지 않는다.
- 결과가 `outsize` 에 (널 문자 포함) 안 들어가면 실패, `out` 은 `""` 로 만든다.

## 시그니처
```c
int build_path(char *out, size_t outsize, const char *dir, const char *file);
/* 0  : 성공
 * -1 : out/dir/file == NULL, outsize == 0, file == ""   (이때 out 은 건드리지 않는다)
 * -1 : 결과가 안 들어감                                  (이때 outsize >= 1 이므로 out = "")  */
```

## 입출력 예시
| dir | file | outsize | 반환 | out |
|---|---|---|---|---|
| `"/home"` | `"a.txt"` | 32 | 0 | `"/home/a.txt"` |
| `"/home/"` | `"a.txt"` | 32 | 0 | `"/home/a.txt"` |
| `""` | `"a.txt"` | 32 | 0 | `"a.txt"` |
| `"/"` | `"a"` | 32 | 0 | `"/a"` |
| `"/h"` | `"f"` | 5 | 0 | `"/h/f"` (딱 맞음) |
| `"/h"` | `"f"` | 4 | -1 | `""` |
| `NULL` | `"f"` | 32 | -1 | (변화 없음) |
| `"/h"` | `""` | 32 | -1 | (변화 없음) |

## 제약
- 입력은 널 종료 문자열. `out` 은 `outsize` 바이트짜리(힙일 수 있음). 경계 밖 쓰기 금지.
- `snprintf` 를 쓴다면 **반환값**으로 잘림을 판정한다(`n < 0` 도 실패).
- 경로 정규화(`..`, `//` 정리)는 하지 않는다.

## 통과 기준
`make P003` → `PASS n/n`, 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.
