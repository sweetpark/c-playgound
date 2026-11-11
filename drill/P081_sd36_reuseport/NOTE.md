# NOTE · P081 sd36 reuseport (풀고 난 뒤에 보세요)

**원본 글** (읽기 전용, 여기에 복사하지 않음)
- 「[SD] SD36. 같은 포트번호의 다중 연결 — SO_REUSEPORT 신중 사용으로 치환」 — 시큐어코딩 체화 드릴 (회차 규칙 A/B/C, 변형 요구사항, 모범답안)
- 시큐어코딩가이드: Part 2-4. 네트워크 계열 — 17. 같은 포트번호의 다중 연결 (`CWE-605`)

**함정 요약**
- `SO_REUSEPORT` 자체를 "무조건 나쁜 옵션"으로 암기 — 멀티 프로세스 부하 분산의 정식 기능이다.
- `SO_REUSEADDR` 와 `SO_REUSEPORT` 를 혼동 — `SO_REUSEADDR` 는 주로 TIME_WAIT 상태의 소켓 재사용을 허용하는 용도이고, 이 항목이 다루는 "포트 가로채기"의 실질적 원인은 `SO_REUSEPORT` 다.
- Good에서도 `INADDR_ANY` 를 그대로 둠 — `SO_REUSEPORT` 를 껐다고 끝이 아니다.

**한 줄 설명 연습**: "SO_REUSEPORT 신중 사용으로 치환" — 이 규칙을 내 말로 한 줄로 쓴다.
