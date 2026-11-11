# NOTE · P014 run_step (풀고 난 뒤에 보세요)

**원본 글** (읽기 전용, 여기에 복사하지 않음)
- 「[C] errno 사용 규칙 — 초기화·저장·검사·설정 패턴」
- 「[C] 동적 메모리 실습 — malloc·free 오류 패턴과 유연 배열 구조체 연결리스트」 §3 "goto로 자원 정리 — 9-07" (`ret_val = errno; goto CLEAN`)
- 소스: [`c_basic/lecture/err/`](../../c_basic/lecture/err/), [`c_basic/lecture/mem/9-07.c`](../../c_basic/lecture/mem/9-07.c)

**함정 요약**
- `errno` 는 전역(스레드별) 변수이고 **성공한 호출도 바꿀 수 있다**. 실패 → `printf`/`fclose`/`free` → `errno` 읽기 순서면 엉뚱한 원인이 나온다.
- 규칙: 실패 직후 지역 변수에 저장 → 정리 → 필요하면 복원해서 반환.
- `errno == 0` 인 실패(오류 원인을 못 남기는 함수)는 호출자가 구분할 수 있게 기본값(EIO)을 정한다.

**한 줄 설명 연습**: "errno 는 실패한 바로 다음 줄에서 지역 변수로 옮기고, 그 뒤의 어떤 호출도 믿지 않는다."
