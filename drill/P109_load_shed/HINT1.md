# HINT1
- 제한기 구조체: `limit`, `window`, `count`, `started`, `admitted`, `rejected`.
- 새 구간 조건은 `w > window` (또는 처음). `w < window` 인 과거 시각은 새 구간이 아니다.
- 백오프: `d = base` 에서 `attempt` 번 두 배 하되, 매번 `d` 가 `cap` 을 넘으면 `cap` 으로 자르고 **멈춘다** (`1 << attempt` 를 쓰면 attempt 가 클 때 정의되지 않은 동작).
- 지터: `lo = d*(100-j)/100`, 결과 `lo + (d-lo)*(rnd%1001)/1000`.
