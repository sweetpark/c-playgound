# P037 · join_strs — snprintf 반환값 누적

## 요구사항
문자열 `cnt` 개를 구분자 `sep` 로 이어 `out` 에 만든다(`"a, b, c"`). `snprintf` 의 반환값을 **그대로 누적해서 위치로 쓰면 안 된다** — 반환값은 "쓴 길이"가 아니라 "쓰려던 길이"다. 잘리면 오류로 알리고 `out` 을 `""` 로 만든다.

## 시그니처
```c
int join_strs(char *out, size_t outsize, const char *const *items, size_t cnt, const char *sep);
/* 0  : 성공 (cnt == 0 이면 "")
 * -1 : out/sep == NULL, outsize == 0, items == NULL(cnt > 0), items[i] == NULL   (out 불변)
 * -2 : 결과가 outsize 에 안 들어감 (out = "") */
```

## 입출력 예시
| items / sep / outsize | 반환 | out |
|---|---|---|
| `a,b,c` / `", "` / 32 | 0 | `"a, b, c"` |
| `solo` 1 개 | 0 | `"solo"` (구분자는 사이에만) |
| 0 개 | 0 | `""` |
| `a,b,c` / `", "` / 8 | 0 | `"a, b, c"` (딱 맞음) |
| 〃 / 7 | -2 | `""` |
| 중간에 `NULL` 항목 | -1 | 불변 |

## 제약
- 매번 `snprintf(out + pos, outsize - pos, ...)` — 남은 크기를 다시 계산해서 넘긴다.
- 반환값 `n` 이 `outsize - pos` 이상이면 잘린 것이다. 그 뒤로 `pos += n` 하면 `out + pos` 가 버퍼 밖을 가리킨다.
- 쓰기 전에 모든 입력(`items[i] != NULL`)을 검증해 `-1` 일 때 `out` 을 건드리지 않는다.

## 통과 기준
`make P037` → `PASS n/n`, 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.
