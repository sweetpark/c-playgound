# P020 · env_copy — getenv 결과는 복사해서 쓴다

## 요구사항
환경변수 `name` 의 값을 `out`(크기 `outsize`)에 **복사**해 돌려준다. 환경변수 값의 길이는 외부에서 정해지므로, 안 들어가면 잘라 넣지 말고 오류로 알린다. `getenv` 가 돌려준 포인터를 직접 수정하거나 오래 들고 있지 않는다.

## 시그니처
```c
int env_copy(const char *name, char *out, size_t outsize);
/* 0  : 복사됨 (값이 "" 여도 설정된 것이므로 0)
 * -1 : 미설정, name/out == NULL, outsize == 0   (out 은 건드리지 않는다)
 * -2 : 값이 outsize 에 (널 포함) 안 들어감       (out 은 건드리지 않는다) */
```

## 입출력 예시
| 환경 | outsize | 반환 | out |
|---|---|---|---|
| `V="value"` | 6 | 0 | `"value"` (딱 맞음) |
| `V="value"` | 5 | -2 | 변화 없음 |
| 150 글자 | 16 | -2 | 변화 없음 |
| `V=""` | 4 | 0 | `""` |
| 미설정 | 8 | -1 | 변화 없음 |

## 제약
- `strcpy(path_buf, getenv("PATH"))` 처럼 고정 버퍼에 직접 복사 금지 (11-01).
- `getenv` 결과는 읽기 전용이다. 사본을 바꿔도 환경은 그대로여야 한다.
- 크기 검사는 널 문자 자리 +1.

## 통과 기준
`make P020` → `PASS n/n`, 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.
