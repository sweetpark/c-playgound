# NOTE · P001 safe_copy (풀고 난 뒤에 보세요)

**원본 글** (읽기 전용, 여기에 복사하지 않음)
- 「[C] 문자열 함수 실습 — strncpy·strncat 직접 구현과 오버플로 함정」 §1 "strncpy는 널 문자를 보장하지 않는다 (8-01)"
- 소스: [`c_basic/lecture/str/8-01.c`](../../c_basic/lecture/str/8-01.c)

**함정 요약**
- `strncpy(a, src, sizeof(a))` — `src` 가 `sizeof(a)` 이상이면 `\0` 이 없다. 이후 `printf("%s")` 는 버퍼 밖을 읽는다.
- 크기 검사는 `n >= dstsize` (널 문자 자리 +1). `>` 로 쓰면 딱 맞는 길이에서 1바이트 넘친다.
- `src == NULL` 검사 누락, `dstsize == 0` 일 때 `dstsize - 1` 이 `size_t` 언더플로(거대한 값)가 되는 것.

**한 줄 설명 연습**: "`strncpy` 는 안전한 복사가 아니라 고정폭 필드 채우기라서, 널 종료는 내가 보장해야 한다."
