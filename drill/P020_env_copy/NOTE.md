# NOTE · P020 env_copy (풀고 난 뒤에 보세요)

**원본 글** (읽기 전용, 여기에 복사하지 않음)
- 「[C] 환경변수·시그널 실습 — getenv 복사·atexit와 sigaction·pselect 시그널 카운터」 §1 "11-01 getenv(PATH) 를 char path[32] 에 strcpy", "11-02 getenv 반환 문자열을 직접 바꾸려 함"
- 소스: [`c_basic/lecture/env/11-01.c`](../../c_basic/lecture/env/11-01.c), [`11-02.c`](../../c_basic/lecture/env/11-02.c), [`c_basic/env/env_ex.c`](../../c_basic/env/env_ex.c)

**함정 요약**
- 환경변수·인자·파일 내용은 모두 **외부 값**: 길이를 코드가 정하는 게 아니다. 고정 버퍼 `strcpy` 는 오버플로.
- `getenv` 가 돌려준 포인터는 환경 영역 자체이고 다음 `setenv`/`putenv` 에 무효화될 수 있다 → 쓸 거면 복사.
- 빈 값(`V=`)과 미설정(NULL)은 다르다.

**한 줄 설명 연습**: "환경변수는 외부가 정한 길이이므로 길이를 확인한 뒤 내 버퍼에 복사해서 쓴다."
