# P033 · array_params — conformant 배열 매개변수

## 요구사항
배열과 그 길이를 받는 함수 3개를 **conformant 배열 매개변수**(`size_t n, const int a[n]` — 크기를 앞에 선언해 배열 선언에 연결)로 만든다. 길이를 따로 믿을 수 있게 하는 API 설계 연습이다.
- `sum_arr` — 합을 `long` 으로. `a == NULL` 이면 0.
- `max_arr` — 최댓값. 빈 배열·NULL 은 오류.
- `copy_arr` — 대상 크기(`dn`)가 원본(`sn`)보다 작으면 거절.

## 시그니처
```c
long sum_arr(size_t n, const int a[n]);
int  max_arr(size_t n, const int a[n], int *out);                     /* 0 / -1 (n == 0, NULL) — out 불변 */
int  copy_arr(size_t dn, int dst[dn], size_t sn, const int src[sn]);  /* 0 / -1 (NULL, dn < sn) — dst 불변 */
```

## 입출력 예시
| 호출 | 결과 |
|---|---|
| `sum_arr(5, {3,-1,9,4,9})` | 24 |
| `sum_arr(2, {INT_MAX, INT_MAX})` | 4294967294 (int 누적이면 오버플로) |
| `max_arr(1, {-7}, &m)` | 0, `m = -7` (0 으로 시작하면 틀림) |
| `max_arr(0, a, &m)` | -1 |
| `copy_arr(4, d, 5, a)` | -1, `d` 불변 |
| `copy_arr(5, d, 0, a)` | 0 |

## 제약
- 크기 매개변수는 배열 매개변수보다 **앞에** 쓴다(`size_t n, int a[n]`). 뒤에 쓰면 선언에서 `n` 을 모른다.
- 함수 안에서 `sizeof(a)` 는 포인터 크기라는 점(P002)을 기억하고 항상 `n` 을 쓴다.
- 곱셈 `sn * sizeof(...)` 는 `dn >= sn` 확인 뒤.

## 통과 기준
`make P033` → `PASS n/n`, 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.
