# NOTE · P002 sizeof_macros (풀고 난 뒤에 보세요)

**원본 글** (읽기 전용, 여기에 복사하지 않음)
- 「[C] 문자열 함수 실습 — strncpy·strncat 직접 구현과 오버플로 함정」 §1 (8-01: `strncpy(a, src, sizeof(a))`)
- 「[C] 동적 메모리 실습 — malloc·free 오류 패턴과 유연 배열 구조체 연결리스트」 §2 (9-03·9-11: `sizeof(tmb)` 는 포인터 크기 8바이트)

**함정 요약**
- 배열 이름은 식 안에서 대부분 포인터로 **붕괴(decay)** 하지만 `sizeof` 의 피연산자일 때만 예외다 → 선언한 그 스코프에서만 전체 크기가 나온다.
- 함수 인자 `void f(char buf[16])` 의 `buf` 는 사실 `char *`. 안에서 `sizeof(buf)` 는 **8**(포인터 크기). 그래서 크기는 `size_t` 인자로 따로 받는다 (P001 의 `dstsize`).
- `ARRAY_LEN(p)` 를 포인터에 쓰면 `8 / 1` 같은 엉뚱한 값이 나온다 (gcc 는 `-Wsizeof-pointer-div` 로 일부 경고).
- `memset(a, 0, ARRAY_LEN(a))` — 개수와 바이트 수를 혼동. `int[5]` 는 5 바이트만 지워진다.
- 매크로는 `if (c) M(); else ..` 에서 깨지지 않게 식 또는 `do { } while (0)` 로.

**한 줄 설명 연습**: "`sizeof` 는 선언된 배열에서만 전체 크기를 주고, 함수에 넘긴 순간 포인터 크기만 남는다."
