# P028 · split_words — strtok_r 와 입력 불변

## 요구사항
문자열 `s` 를 구분자 집합 `delims` 로 나눠 `out[0..]`(각 칸 16바이트)에 담는다. 연속·앞뒤 구분자는 빈 토큰을 만들지 않는다. **입력 `s` 를 수정하지 않고**(리터럴일 수 있다), **여러 스레드에서 동시에 불러도** 안전해야 한다 — `strtok` 의 내부 정적 상태를 쓰지 않는다.

## 시그니처
```c
int split_words(const char *s, const char *delims, char out[][16], size_t maxn);
/* >= 0 : 토큰 개수
 * -1   : NULL 인자, maxn == 0
 * -2   : 토큰이 15 글자를 초과 (널 포함 16 바이트 초과)
 * -3   : 토큰이 maxn 개를 초과 */
```

## 입출력 예시
| `s` / `delims` | 결과 |
|---|---|
| `"alpha beta gamma"` / `" "` | 3: `alpha`, `beta`, `gamma` |
| `",,a;b , c;;"` / `",; "` | 3: `a`, `b`, `c` |
| `""`, `"   "` | 0 |
| 15 글자 토큰 / 16 글자 토큰 | 성공 / -2 |
| 토큰 3 개, maxn 3 / 4 개, maxn 3 | 3 / -3 |

## 제약
- `strtok` 은 원본에 `'\0'` 을 쓰고 정적 상태를 공유한다 → 사본을 만들어 `strtok_r` 로.
- 사본은 `malloc` 해서 반드시 `free` (모든 반환 경로, 오류 포함).
- 각 토큰은 `memcpy` 후 널 종료, 길이 검사는 널 문자 자리 포함.

## 통과 기준
`make P028` → `PASS n/n`, 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.
