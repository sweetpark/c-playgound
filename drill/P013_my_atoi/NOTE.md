# NOTE · P013 my_atoi (풀고 난 뒤에 보세요)

**원본 글** (읽기 전용, 여기에 복사하지 않음)
- 「[C] errno 사용 규칙 — 초기화·저장·검사·설정 패턴」
- 「[C] 에러 처리·API 설계 실습 — strtol my_atoi·snprintf_m·sum_file·conformant 배열」
- 소스: [`c_basic/err/my_atoi.c`](../../c_basic/err/my_atoi.c), [`c_basic/lecture/err/`](../../c_basic/lecture/err/)

**함정 요약**
- `errno` 는 성공해도 지워지지 않는다 → 호출 **전**에 `0` 으로 초기화하고, 호출 직후에만 검사.
- `strtol` 의 반환값만 보면 "0" 과 "실패" 를 구분할 수 없다 → `endptr` 두 가지 검사(`end == s`, `*end != '\0'`).
- `long` → `int` 변환은 범위를 직접 확인(64비트에서는 `ERANGE` 가 나지 않는다).
- 실패 시 출력 인자를 건드리지 않아야 호출자가 쓰레기 값을 쓰지 않는다.

**한 줄 설명 연습**: "`strtol` 은 errno 를 미리 0 으로 만들고, 끝 포인터와 ERANGE 와 대상 타입의 범위를 모두 확인해야 끝난다."
