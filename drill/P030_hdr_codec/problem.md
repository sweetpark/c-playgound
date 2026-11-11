# P030 · hdr_codec — 메시지 헤더 인코드/디코드

## 요구사항
8바이트 고정 메시지 헤더를 인코드/디코드한다. 필드별로 빅엔디안 변환하고 **구조체 통째 `memcpy` 는 쓰지 않는다**(패딩·엔디안이 새어 나온다). 검증을 통과한 것만 내보내고(encode), 검증 후에만 출력 구조체를 채운다(decode).

```
byte: 0   1    2-3   4-7
      ver type len   id          (len, id 는 빅엔디안)
```

## 시그니처
```c
typedef struct { uint8_t ver; uint8_t type; uint16_t len; uint32_t id; } hdr_t;   /* solution.c 에 이미 있음 */

int hdr_encode(const hdr_t *h, uint8_t *buf, size_t size);
/* 8  : 쓴 바이트 수
 * -1 : NULL 인자, size < 8        -2 : ver != 1 또는 type > 3        -3 : len > 1024   (buf 불변) */
int hdr_decode(const uint8_t *buf, size_t size, hdr_t *h);
/* 0  : 성공
 * -1 / -2 / -3 : 위와 같은 의미 (h 불변) */
```

## 입출력 예시
| 구조체 | 바이트열 |
|---|---|
| `{ver 1, type 2, len 0x0234, id 0xA1B2C3D4}` | `01 02 02 34 A1 B2 C3 D4` |
| `{1, 0, 0, 0xFFFFFFFF}` | `01 00 00 00 FF FF FF FF` |
| `ver 2` 또는 `type 4` | -2 |
| `len 1025` / `len 1024` | -3 / 성공 |
| `size 7` | -1 |

## 제약
- 8바이트 밖은 쓰거나 읽지 않는다(정확히 8바이트 힙 버퍼로 시험).
- `buf[2] << 8` 같은 연산은 `uint16_t`/`uint32_t` 로 올려서. 32비트 `id` 의 최상위 바이트는 `(uint32_t)buf[4] << 24`.
- 디코드는 값을 지역 변수에 모두 읽고 검증한 뒤에만 `*h` 에 쓴다.

## 통과 기준
`make P030` → `PASS n/n`, 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.
