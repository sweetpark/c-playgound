# P032 · level_table — enum switch 와 default

## 요구사항
로그 레벨 열거형 `enum level` 의 이름 표를 만든다.
1. `level_name(lv)` — 레벨 이름. 범위 밖(음수, `LV_COUNT` 이상)은 `"UNKNOWN"`. **모든 경로에서 값을 돌려준다.**
2. `level_parse(s)` — 대소문자 무시, 정확히 일치하는 이름만. 없으면 -1.
3. `level_names(out, cap)` — 모든 이름을 `out` 에 채우고 개수를 돌려준다. 칸이 모자라면 아무것도 쓰지 않고 0.

## 시그니처
```c
enum level { LV_DEBUG, LV_INFO, LV_WARN, LV_ERROR, LV_COUNT };   /* solution.c 에 이미 있음 */

const char *level_name(int lv);
int         level_parse(const char *s);                  /* enum 값 / -1 */
size_t      level_names(const char **out, size_t cap);   /* LV_COUNT / 0 */
```

## 입출력 예시
| 호출 | 결과 |
|---|---|
| `level_name(LV_WARN)` | `"WARN"` |
| `level_name(-1)`, `(LV_COUNT)`, `(99)` | `"UNKNOWN"` |
| `level_parse("info")`, `("WaRn")` | `LV_INFO`, `LV_WARN` |
| `level_parse("INFOX")`, `("")`, `(NULL)` | -1 |
| `level_names(buf, 8)` / `(buf, 2)` | 4 / 0 |

## 제약
- `switch` 에는 `default` 를 두고 거기서 오류 값을 돌려준다(경고 `-Wreturn-type` 의 원인: 일부 경로에 return 없음).
- enum 을 순회할 때 `size_t` 가 아니라 `int`(또는 enum 타입)로. 비교 시 부호 혼합 주의.
- `LV_COUNT` 를 센티널로 써서 배열 크기·순회 상한에 쓴다(하드코딩 4 금지).

## 통과 기준
`make P032` → `PASS n/n`, 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.
