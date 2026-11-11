# P010 · conn_reset — 구조체 비우기, sizeof 는 포인터가 아니라 구조체에

## 요구사항
연결 정보 구조체 `conn_t`(`solution.c` 에 정의됨)를 다루는 함수 세 개.
1. `conn_reset(c)` — 구조체 **전체**를 0 으로 만들고 `fd` 만 `-1`(닫힘)로.
2. `conn_reset_all(arr, n)` — 배열 `n` 개 모두 reset.
3. `conn_init(c, host, port)` — reset 한 뒤 `host`(버퍼보다 길면 잘라서, 널 종료)와 `port` 를 채운다.

## 시그니처
```c
void conn_reset(conn_t *c);                       /* c == NULL 이면 아무것도 안 함 */
void conn_reset_all(conn_t *arr, size_t n);       /* arr == NULL, n == 0 이어도 안전 */
int  conn_init(conn_t *c, const char *host, unsigned short port);
/* 0 성공 / -1 c == NULL 또는 host == NULL */
```

## 입출력 예시
| 호출 전 `c` | 호출 | 호출 후 |
|---|---|---|
| 전부 `0xAA` 쓰레기 | `conn_reset(&c)` | `fd == -1`, `host`/`tag` 전부 `\0`, `port == 0`, `stats` 0 |
| 전부 `0xAA` 쓰레기 | `conn_init(&c, "example.com", 8080)` | `host == "example.com"` (뒤는 전부 `\0`), `port == 8080`, `fd == -1` |
| 〃 | `conn_init(&c, <40자 이름>, 1)` | `host` 는 31 글자 + `\0` |
| 〃 | `conn_init(&c, NULL, 1)` | -1 |

## 제약
- `memset(c, 0, sizeof(c))` 는 틀린다 — `c` 는 포인터다. 왜 틀리는지 `NOTE.md` 에서.
- 필드를 하나씩 대입하지 말고 한 번에 비운다(나중에 필드가 늘어도 안전하도록).
- 호스트 복사에 `strcpy` 금지.

## 통과 기준
`make P010` → `PASS n/n`, 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.
