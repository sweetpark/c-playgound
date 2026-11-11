# P021 · parse_kv — 공백 제거·경계·입력 불변

## 요구사항
설정 파일 한 줄 `"  log_level = debug  "` 를 `key="log_level"`, `value="debug"` 로 나눈다. **교육자료 추가 과제(8-7) `parse_kv`**.
- 첫 번째 `=` 로 분리한다(값에 `=` 가 더 있어도 값의 일부).
- key·value 앞뒤의 공백(space, tab)을 제거한다. 값 안쪽 공백은 유지.
- `line` 을 수정하지 않는다(리터럴일 수 있다) — 위치만 구해 `memcpy` 로 복사.
- 버퍼에 다 안 들어가면 잘라 저장하지 말고 **오류**.
- 인자 오류 외의 에러에서는 `key`, `val` 을 `""` 로 만든다.

## 시그니처
```c
int parse_kv(const char *line, char *key, size_t key_size, char *val, size_t val_size);
/*  0 성공
 * -1 인자 오류 (NULL, 크기 0)        -> key/val 을 건드리지 않음
 * -2 '=' 없음     -3 key 가 빈 문자열
 * -4 key 버퍼 부족  -5 val 버퍼 부족  -> key, val = "" */
```

## 입출력 예시
| `line` | key(16) / val(32) | 반환 |
|---|---|---|
| `"  log_level = debug  "` | `"log_level"` / `"debug"` | 0 |
| `"a=b=c"` | `"a"` / `"b=c"` | 0 |
| `"key="` | `"key"` / `""` | 0 |
| `"novalue"` | `""` / `""` | -2 |
| `"   = v"` | `""` / `""` | -3 |
| key 15 글자 / 16 글자 | 성공 / `""` | 0 / -4 |
| val 31 글자 / 32 글자 | 성공 / `""` | 0 / -5 |

## 제약
- `strchr` 가 돌려준 포인터로 `*p = 0` 을 쓰는 것도 수정이다 — 금지(P005 와 같은 문제).
- 공백 제거는 앞과 뒤를 따로. 전부 공백이면 빈 문자열(key 라면 -3).
- 길이 판단은 공백 제거 **후**, 널 문자 자리 +1.

## 통과 기준
`make P021` → `PASS n/n`, 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.
