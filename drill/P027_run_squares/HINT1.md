# HINT1
`job_t jobs[64]` 처럼 호출 함수 안에 스레드 수만큼의 인자 배열을 두고 `jobs[i]` 를 각각 넘긴다 — join 이 끝날 때까지 살아 있으므로 안전. 만든 개수(`started`)만큼 join.
