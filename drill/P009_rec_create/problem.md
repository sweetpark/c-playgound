# P009 · rec_create — 중간에 실패해도 새지 않게 (early return 누수)

## 요구사항
레코드 하나를 만든다. 힙 블록 **세 개**를 이 순서로 할당한다: ① `rec_t` 자체 ② 이름 복사본 ③ `int` 배열(`nvals` 개, 0 초기화).
**어느 단계에서 할당이 실패해도** 앞서 할당한 블록을 전부 해제하고 `-1` 을 돌려준다. 실패하면 `*out` 은 건드리지 않는다.
`rec_destroy` 는 `rec_create` 가 만든 것을 모두 해제한다.

## 시그니처
```c
typedef struct { char *name; int *vals; size_t nvals; } rec_t;   /* solution.c 에 이미 있음 */

int  rec_create(rec_t **out, const char *name, size_t nvals);
/* 0  : 성공, *out = 새 레코드
 * -1 : out/name == NULL, nvals == 0 또는 > 1024, 또는 할당 실패.  *out 은 그대로, 누수 0 */
void rec_destroy(rec_t *r);   /* r == NULL 이면 아무것도 안 함 */
```

## 입출력 예시
| 상황 | 반환 | `*out` | 살아있는 힙 블록 |
|---|---|---|---|
| `("alice", 3)` 정상 | 0 | 새 레코드 | 3 (destroy 후 0) |
| ① 에서 malloc 실패 | -1 | 그대로 | 0 |
| ② 에서 malloc 실패 | -1 | 그대로 | 0 |
| ③ 에서 malloc 실패 | -1 | 그대로 | 0 |
| `nvals == 0`, `name == NULL` | -1 | 그대로 | 0 |

## 제약
- `malloc` 은 정확히 세 번(위 순서)만 부른다. `strdup`/`calloc` 대신 `malloc` + `memcpy`/`memset` (테스트가 `malloc`/`free` 를 가로채 센다).
- 실패 경로가 여러 개 — 정리 코드를 경로마다 쓰지 말고 `goto fail` 같은 **한 곳**으로 모아라.
- 성공할 때만 `*out` 에 쓴다. 이중 해제 금지.

## 통과 기준
`make P009` → `PASS n/n`, 경고 0, ASan/UBSan/LSan 0. 힌트: `HINT1.md` → `HINT2.md`.
