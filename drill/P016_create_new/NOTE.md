# NOTE · P016 create_new (풀고 난 뒤에 보세요)

**원본 글** (읽기 전용, 여기에 복사하지 않음)
- 「[C] 파일 입출력 실습 — fopen·open 오류 패턴과 임시파일 + rename 안전 저장」 §1 "10-01 이미 존재하는 파일을 덮어씀 / 존재 확인 후 열기의 경쟁 상태"
- 소스: [`c_basic/lecture/fio/10-01.c`](../../c_basic/lecture/fio/10-01.c), [`c_basic/str/str_ex.c`](../../c_basic/str/str_ex.c) `make_filename`

**함정 요약**
- `"w"` 는 기존 파일을 잘라 버린다. 확인 후 열기(access → fopen)는 그 사이에 파일이 바뀔 수 있다(TOCTOU).
- `"wx"` / `open(O_CREAT|O_EXCL)` 는 존재 확인과 생성이 한 번에 일어난다.
- 실패 원인을 `errno` 로 나눠야 한다: `EEXIST` 는 재시도/건너뛰기, 권한·경로 오류는 즉시 실패 (재시도하면 무한 루프).

**한 줄 설명 연습**: "파일이 없을 때만 만드는 것은 확인하고 만드는 두 동작이 아니라 `wx` 한 번의 동작이다."
