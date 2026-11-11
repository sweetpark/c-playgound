# NOTE · P037 join_strs (풀고 난 뒤에 보세요)

**원본 글** (읽기 전용, 여기에 복사하지 않음)
- 「[C] 에러 처리·API 설계 실습 — strtol my_atoi·snprintf_m·sum_file·conformant 배열」 §1 "13-02 sprintf 반환값(쓴 길이)을 누적해 위치로 사용, 오류·오버플로 검사 없음 → in-band 에러 지시자를 피한다, snprintf 사용"
- 소스: [`c_basic/err/snprintf.c`](../../c_basic/err/snprintf.c), [`c_basic/lecture/err/13-02.c`](../../c_basic/lecture/err/13-02.c)

**함정 요약**
- `pos += sprintf(...)` 누적: 버퍼 크기를 모르는 채로 쓰고, 한 번이라도 넘치면 이후 위치가 이미 범위 밖.
- `snprintf` 반환값은 "필요했을 길이" — 남은 크기와 비교해서 잘림을 판정하고, 잘렸을 때는 `pos` 를 더하지 않는다.
- 오류는 반환값으로, 값(문자열)은 인자로(in-band 금지).

**한 줄 설명 연습**: "snprintf 로 이어 붙일 때는 매번 남은 크기를 넘기고, 반환값이 남은 크기 이상이면 거기서 멈춘다."
