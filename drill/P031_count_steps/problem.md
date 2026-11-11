# P031 · count_steps — size_t 루프의 랩어라운드

## 요구사항
`for (i = start; i <= end; i += step)` 가 도는 **횟수**를 계산한다. 이 루프는 `end` 가 `SIZE_MAX` 근처이면 `i += step` 이 랩어라운드되어 **영원히** 돈다. 루프를 돌리지 않고, 증가시키기 **전에** 남은 범위로 계산한다.

## 시그니처
```c
size_t count_steps(size_t start, size_t end, size_t step);
/* start, start+step, start+2*step, ... 중 end 이하인 값의 개수.
 * step == 0 또는 start > end 이면 0.   개수가 SIZE_MAX 를 넘으면(표현 불가) SIZE_MAX 로 포화 */
```

## 입출력 예시
| start, end, step | 결과 |
|---|---|
| 0, 10, 3 | 4 (0 3 6 9) |
| 5, 5, 1 | 1 |
| 5, 4, 1 | 0 |
| 0, 10, 0 | 0 |
| `SIZE_MAX-5`, `SIZE_MAX`, 2 | 3 |
| 0, `SIZE_MAX`, `SIZE_MAX` | 2 |
| 0, `SIZE_MAX`, 1 | `SIZE_MAX` (포화) |

## 제약
- 반복문으로 세지 않는다(10 초 안에 끝나야 한다 — 아니면 `FAIL (timeout: infinite loop?)`).
- `(end - start) / step` 은 `start <= end` 를 확인한 뒤에만(`size_t` 뺄셈 언더플로).
- `+ 1` 이 랩어라운드되는 경우를 처리한다.

## 통과 기준
`make P031` → `PASS n/n`, 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.
