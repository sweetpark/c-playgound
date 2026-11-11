# NOTE · P003 build_path (풀고 난 뒤에 보세요)

**원본 글** (읽기 전용, 여기에 복사하지 않음)
- 「[C] 문자열 함수 실습 — strncpy·strncat 직접 구현과 오버플로 함정」 §6 "파일명 생성 — snprintf + time + wx" (쓰기 **전에** 크기 검사, `snprintf` 반환값)
- 「[C] 에러 처리·API 설계 실습 — strtol my_atoi·snprintf_m·sum_file·conformant 배열」 (`snprintf` 래핑)
- 소스: [`c_basic/str/str_ex.c`](../../c_basic/str/str_ex.c) `make_filename`

**함정 요약**
- `snprintf` 는 넘쳐도 안전하게 자르지만, **잘렸다는 사실은 반환값으로만** 알 수 있다. 반환값을 안 보면 잘린 경로로 파일을 만든다.
- `NULL` 과 `""` 는 다르다: `NULL` 은 "값 없음(오류)", `""` 는 "길이 0 의 유효한 문자열". `dir[strlen(dir)-1]` 은 `dir == ""` 일 때 `dir[-1]` (UB).
- 쓰고 나서 검사하면 이미 늦다. 실패 때는 `out` 을 `""` 로 비워 두면 호출자가 쓰레기를 읽지 않는다.

**한 줄 설명 연습**: "`snprintf` 의 반환값은 쓰인 길이가 아니라 필요한 길이라서, `>= outsize` 가 곧 잘림이다."
