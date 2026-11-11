# P004 · str_append — strcat 을 안전하게 (strlen 은 한 번만)

## 요구사항
`dst`(크기 `dstsize`)의 끝에 `src` 를 이어 붙인다. 모자라면 **아무것도 바꾸지 않고** 실패한다(조금만 붙이지 않는다).
`dst` 안에 널 문자가 `dstsize` 바이트 안에 없다면(널 종료 누락) 길이를 알 수 없으므로 역시 실패 — 범위 밖을 읽지 않는다.

## 시그니처
```c
int str_append(char *dst, size_t dstsize, const char *src);
/* 0  : 성공 (dst 는 널 종료)
 * -1 : 실패 — NULL 인자, dstsize == 0, dst 에 널 문자 없음, 결과가 안 들어감. dst 는 그대로 */
```

## 입출력 예시
| dst(앞) | dstsize | src | 반환 | dst(뒤) |
|---|---|---|---|---|
| `"dst"` | 10 | `"src"` | 0 | `"dstsrc"` |
| `"abc"` | 7 | `"def"` | 0 | `"abcdef"` (딱 맞음) |
| `"abc"` | 6 | `"def"` | -1 | `"abc"` |
| `""` | 4 | `"xyz"` | 0 | `"xyz"` |
| `"abc"` | 8 | `""` | 0 | `"abc"` |
| `AAAAAAAA`(널 없음) | 8 | `"x"` | -1 | 그대로 |

## 제약
- `dst`, `src` 는 서로 겹치지 않는다. `dst` 는 정확히 `dstsize` 바이트 힙/스택 버퍼.
- 루프 조건에 `strlen(dst)` 를 넣지 않는다(쓰는 순간 값이 변한다 → 한 칸씩 건너뛰는 버그의 원인).
- `size_t` 끼리의 뺄셈은 언더플로 주의.

## 통과 기준
`make P004` → `PASS n/n`, 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.
