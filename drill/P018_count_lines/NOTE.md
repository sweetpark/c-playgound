# NOTE · P018 count_lines (풀고 난 뒤에 보세요)

**원본 글** (읽기 전용, 여기에 복사하지 않음)
- 「[C] 파일 입출력 실습 — fopen·open 오류 패턴과 임시파일 + rename 안전 저장」 §1 "10-03 rewind 는 성공 여부를 알 수 없음, fclose 누락" 과 「[C] 에러 처리·API 설계 실습 …」 "14-01 읽기 루프가 끝난 이유를 구분 안 함"
- 소스: [`c_basic/lecture/fio/10-03.c`](../../c_basic/lecture/fio/10-03.c), [`c_basic/fio/fio_ex.c`](../../c_basic/fio/fio_ex.c)

**함정 요약**
- 조각 하나 = 줄 하나로 세면 긴 줄이 여러 줄로 불어난다.
- 마지막 줄에 개행이 없을 때 하나 덜 세는 off-by-one.
- `feof` 만 보고 오류를 EOF 로 오인 → 깨진 입력을 정상 처리. 루프 뒤 `ferror`/`feof` 로 구분.
- 오류 경로에서 `fclose` 누락(파일 디스크립터 누수).

**한 줄 설명 연습**: "fgets 가 NULL 을 돌려줬다고 끝난 게 아니라, 그 이유가 EOF 인지 오류인지 확인해야 정상 종료다."
