# P036 · safe_macros — FREE / XNEW / XNEWN

## 요구사항
방어 매크로 3개와 도우미 함수 1개를 만든다.
- `FREE(ptr)` — 해제 후 `ptr = NULL`. `NULL` 이어도 안전. `if (c) FREE(p); else ...` 에서 깨지지 않는 **한 문장**.
- `XNEW(ptr)` — `sizeof(*ptr)` 만큼 0 초기화해 할당해 `ptr` 에 대입. 식의 값은 그 포인터.
- `XNEWN(ptr, n)` — 원소 `n` 개 0 초기화 배열. `n == 0` 이거나 `n * 크기` 가 넘치면 `NULL`.
- `alloc_zeroed_array(n, sz)` — 위 둘이 쓰는 도우미(곱셈 오버플로 검사 포함).

## 시그니처
```c
static void *alloc_zeroed_array(size_t n, size_t sz);   /* n == 0, sz == 0, n*sz 오버플로 -> NULL */
#define FREE(ptr)       ...
#define XNEW(ptr)       ...
#define XNEWN(ptr, n)   ...
```

## 입출력 예시
| 사용 | 결과 |
|---|---|
| `p = malloc(16); FREE(p); FREE(p);` | `p == NULL`, double free 없음 |
| `if (p) FREE(p); else flag = 1;` | 컴파일됨 |
| `big_t *b; XNEW(b);` | 0 초기화된 `sizeof(big_t)` 바이트 |
| `int *a; XNEWN(a, 10)` | 0 초기화된 `int[10]` |
| `XNEWN(a, 0)`, `XNEWN(a, SIZE_MAX/sizeof(int)+1)` | `a == NULL` |

## 제약
- 매크로 본문이 `{ ... }` 블록이면 `if (c) FREE(p); else` 에서 `;` 때문에 `else` 가 짝을 잃는다 → `do { ... } while (0)`.
- `sizeof(ptr)` 가 아니라 `sizeof(*(ptr))` (P010). 매크로 인자는 괄호로 감싼다.
- `XNEW`, `XNEWN` 은 대입 **식**이어야 `if (XNEW(p) == NULL)` 처럼 쓸 수 있다.

## 통과 기준
`make P036` → `PASS n/n`, 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.
