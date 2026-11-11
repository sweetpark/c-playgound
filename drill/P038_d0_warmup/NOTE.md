# NOTE · P038 d0_warmup (풀고 난 뒤에 보세요)

**원본 글** (읽기 전용, 여기에 복사하지 않음)
- 「[D] D0. 워밍업 — common·guard·log 뼈대 타자 연습」 — 템플릿 체화 드릴
- 관련: P009 (early return 누수), P014 (errno 저장)

**함정 요약**
- 자원을 잡은 뒤에 `CHK_PTR` 같은 **즉시 return** 가드를 쓰면 정리를 건너뛴다(누수). 잡은 뒤에는 `*_GOTO`.
- `CLEANUP:` 에서 포인터가 `NULL` 로 초기화돼 있지 않으면 실패한 경로에서 쓰레기를 해제한다.
- 정리 순서는 획득의 역순. `SAFE_FREE`/`SAFE_FCLOSE` 는 해제 후 `NULL` 대입까지 한다.

**한 줄 설명 연습**: "가드는 자원을 잡기 전엔 즉시 return, 잡은 뒤엔 CLEANUP 으로 goto — 둘을 섞지 않는다."
