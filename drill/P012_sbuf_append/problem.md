# P012 · sbuf_append — realloc 은 주소를 바꾼다 (이중 포인터와 임시 변수)

## 요구사항
유연 배열 문자열 버퍼 `sbuf_t` 에 문자열을 이어 붙이는 `sbuf_append` 를 만든다.
모자라면 `realloc` 으로 필요한 만큼(`len + slen + 1`)만 늘린다. `realloc` 은 주소를 바꿀 수 있으므로 호출자의 포인터를 갱신해야 한다.
`realloc` 이 실패하면 **원본 버퍼는 그대로 유효**해야 한다.

## 시그니처
```c
#define SBUF_MAX 64                                    /* 널 포함 최대 바이트 */
typedef struct { size_t len, cap; char data[]; } sbuf_t;   /* solution.c 에 이미 있음 */

sbuf_t *sbuf_new(size_t cap);                 /* 빈 문자열 버퍼. cap == 0 또는 > SBUF_MAX 이면 NULL */
int     sbuf_append(sbuf_t **ps, const char *s);
/* 0  : 성공
 * -1 : ps/*ps/s == NULL, 결과가 SBUF_MAX 초과, realloc 실패.  이때 *ps 는 원래 버퍼 그대로 */
```

## 입출력 예시
| 동작 | 결과 |
|---|---|
| `new(4)`, append `"ab"` | `"ab"`, realloc 없음 |
| 이어서 append `"cdef"` | `"abcdef"`, realloc 1회, `*ps` 갱신 |
| 63 글자 append 후 `"z"` | -1 (널 포함 65 > 64), 내용 그대로 |
| realloc 실패 주입 후 append | -1, `*ps` 이전 포인터 그대로, 내용 `"xy"` 유지 |

## 제약
- `b = realloc(b, ..)` 처럼 반환값을 바로 원래 포인터에 받으면 안 된다(실패 시 원본 소실·누수).
- 이어 붙일 위치는 `cap`(할당 크기)이 아니라 현재 길이 `len`.
- 새 `cap` 은 `len + slen + 1` 이상이면 된다. 누수 0.

## 통과 기준
`make P012` → `PASS n/n`, 경고 0, ASan/UBSan/LSan 0. 힌트: `HINT1.md` → `HINT2.md`.
