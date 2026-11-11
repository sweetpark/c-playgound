# P011 · blob_clone — 유연 배열 구조체는 헤더만 복사되지 않게

## 요구사항
유연 배열 멤버(`int data[]`)를 가진 구조체 `blob_t` 를 다룬다.
1. `blob_new(n)` — 헤더 + `int` n 개를 **한 번의 할당**으로 만들고 data 를 0 으로 채운다.
2. `blob_clone(src)` — data 까지 포함한 **깊은 복사본**을 새로 할당해 돌려준다.
3. `blob_sum(b)` — data 전체의 합.

## 시그니처
```c
typedef struct { size_t n; int data[]; } blob_t;   /* solution.c 에 이미 있음 */

blob_t *blob_new(size_t n);              /* n == 0 또는 n > 1024 이면 NULL */
blob_t *blob_clone(const blob_t *src);   /* src == NULL 이면 NULL */
long    blob_sum(const blob_t *b);       /* b == NULL 이면 0 */
```

## 입출력 예시
| 호출 | 결과 |
|---|---|
| `blob_new(4)` | `n == 4`, `data[0..3] == 0` |
| data 를 `1, 11, 21, 31` 로 채운 뒤 `blob_sum` | `64` |
| `blob_clone(a)` 후 `a->data[0] = 999` | 복사본의 `data[0]` 은 `1` 그대로 |
| `blob_new(0)`, `blob_new(1025)`, `blob_clone(NULL)` | `NULL` |

## 제약
- `*dup = *src` 로 복사하면 헤더(`n`)만 복사된다 — 금지. `sizeof(*src)` 는 data 를 **포함하지 않는다**.
- 구조체를 스택 변수로 선언해 쓰지 않는다(`data[]` 자리가 없다). 항상 `malloc` 으로.
- 곱셈은 `n` 이 상한(1024) 이하임을 확인한 뒤.

## 통과 기준
`make P011` → `PASS n/n`, 경고 0, ASan/UBSan/LSan 0. 힌트: `HINT1.md` → `HINT2.md`.
