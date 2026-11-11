# P002 · sizeof_macros — 배열은 크기를 알고, 포인터는 모른다

## 요구사항
`sizeof` 로 "배열 변수"의 크기를 쓰는 매크로 3개를 `solution.c` 에 만든다.
1. `ARRAY_LEN(a)` — 배열 `a` 의 **원소 개수** (바이트 수가 아니다).
2. `COPY_STR(dst, src)` — `char` 배열 `dst` 에 `src` 를 넣는다. 길면 잘라서, **항상 널 종료**.
3. `ZERO_ARRAY(a)` — 배열 `a` **전체**를 0 으로 채운다(원소 타입이 무엇이든).

## 시그니처
```c
#define ARRAY_LEN(a)        /* 원소 개수: size_t 상수식 */
#define COPY_STR(dst, src)  /* 문장처럼 쓰인다: COPY_STR(buf, "text"); */
#define ZERO_ARRAY(a)       /* 문장처럼 쓰인다: ZERO_ARRAY(nums); */
```

## 입출력 예시
| 식 | 결과 |
|---|---|
| `int a[7]; ARRAY_LEN(a)` | `7` |
| `long l[3]; ARRAY_LEN(l)` | `3` |
| `int m[3][4]; ARRAY_LEN(m)` / `ARRAY_LEN(m[0])` | `3` / `4` |
| `char b[8]; COPY_STR(b, "0123456789")` | `b == "0123456"` |
| `int v[5]; ZERO_ARRAY(v)` | `v[0..4] == 0` |

## 제약
- 매크로는 **배열 변수 이름**에만 쓴다(함수 인자로 받은 `char *p` 에 쓰면 안 되는 이유는 `NOTE.md`).
- `COPY_STR` 은 `if (c) COPY_STR(..); else ..;` 처럼 `else` 앞에서도 컴파일돼야 한다.
- 인자는 괄호로 감싸 안전하게. `ARRAY_LEN` 은 `enum { N = ARRAY_LEN(a) };` 처럼 상수식으로도 쓸 수 있어야 한다.

## 통과 기준
`make P002` → `PASS n/n`, 경고 0, ASan/UBSan 보고 0. 힌트는 `HINT1.md` → `HINT2.md`.
