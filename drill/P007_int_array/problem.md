# P007 · int_array — 0 으로 초기화된 int 배열 할당과 해제

## 요구사항
`int` 원소 `n` 개짜리 배열을 힙에 만들어 돌려주고(**모두 0 으로 초기화**), 다 쓰면 해제하는 짝 함수를 만든다.
해제 함수는 호출자의 포인터까지 `NULL` 로 만들어 두 번 불러도 안전해야 한다.
`n` 이 0 이거나, 상한(`1 << 20` 개)을 넘거나, `n * sizeof(int)` 가 `size_t` 를 넘치는 값이면 할당하지 않고 `NULL`.

## 시그니처
```c
int  *int_array_new(size_t n);    /* 성공: 0 으로 채운 n 개 / 실패: NULL */
void  int_array_free(int **pp);   /* *pp 를 해제하고 *pp = NULL.  pp == NULL, *pp == NULL 이어도 안전 */
```

## 입출력 예시
| 호출 | 결과 |
|---|---|
| `int_array_new(4)` | 길이 4, 전부 `0` |
| `int_array_new(0)` | `NULL` |
| `int_array_new((1<<20) + 1)` | `NULL` |
| `int_array_new(SIZE_MAX / sizeof(int) + 1)` | `NULL` (곱하면 래핑됨) |
| `int_array_free(&p)` 후 | `p == NULL` |
| `int_array_free(&p)` 한 번 더 / `int_array_free(NULL)` | 아무 일 없음 |

## 제약
- `malloc` 직후의 메모리는 초기화되어 있지 않다. 쓰레기값을 읽으면 실패(ASan 은 0xBE 로 채워 둔다).
- `n * sizeof(int)` 를 직접 곱한다면 곱하기 전에 검사한다.
- 누수 0 (LeakSanitizer).

## 통과 기준
`make P007` → `PASS n/n`, 경고 0, ASan/UBSan/LSan 0. 힌트: `HINT1.md` → `HINT2.md`.
