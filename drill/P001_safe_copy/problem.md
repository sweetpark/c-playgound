# P001 · safe_copy — 항상 널로 끝나는 문자열 복사

## 요구사항
`src` 를 `dst`(크기 `dstsize` 바이트)에 복사한다.
`dst` 는 **항상** `'\0'` 으로 끝나야 하고, `dstsize` 바이트 밖은 절대 쓰지 않는다. `src` 가 길면 잘라서 넣는다.
(`strncpy(dst, src, dstsize)` 만 쓰면 안 되는 이유를 직접 확인하는 문제.)

## 시그니처
```c
int safe_copy(char *dst, size_t dstsize, const char *src);
/* 0  : 전부 복사됨
 * 1  : 잘림 (dst 는 그래도 널 종료)
 * -1 : dst == NULL, dstsize == 0, src == NULL  (이때 dst 는 건드리지 않는다) */
```

## 입출력 예시
| dstsize | src | 반환 | dst 내용 |
|---|---|---|---|
| 8 | `"abc"` | 0 | `"abc"` |
| 8 | `""` | 0 | `""` |
| 8 | `"1234567"` (7자) | 0 | `"1234567"` |
| 8 | `"12345678"` (8자) | 1 | `"1234567"` |
| 1 | `"x"` | 1 | `""` |
| 0 | `"x"` | -1 | (변화 없음) |
| 8 | `NULL` | -1 | (변화 없음) |

## 제약
- 입력 `src` 는 널 종료 문자열이다. `dst` 는 `dstsize` 바이트짜리 버퍼다(힙일 수도 있다).
- `dstsize` 바깥 1바이트라도 쓰거나, `src` 를 널 문자 너머로 읽으면 실패(ASan).
- `<string.h>`, `<stdio.h>` 사용 가능. 반복문으로 직접 써도 된다.

## 통과 기준
`make P001` → `PASS n/n`, 경고 0, ASan/UBSan 보고 0.
막히면 `HINT1.md` → `HINT2.md` 순서로. 풀고 나서 `NOTE.md`.
