# NOTE · P032 level_table (풀고 난 뒤에 보세요)

**원본 글** (읽기 전용, 여기에 복사하지 않음)
- 「[C] 바이트 순서·메시지 헤더·컴파일러 경고 실습 — htonll·enc_dec와 size_t·switch 함정」 "16-01 size_t 로 enum 을 순회, switch 에 default 가 없어 범위 밖 값에서 return 누락", "16-05 모든 경로에 return"
- 소스: [`c_basic/lecture/msc/16-01.c`](../../c_basic/lecture/msc/16-01.c), [`16-05.c`](../../c_basic/lecture/msc/16-05.c)

**함정 요약**
- enum 인자는 실제로는 정수다. 외부 입력이 범위 밖 값을 만들 수 있으니 `default` 에서 오류 처리.
- `size_t i` 로 enum 을 순회하면 부호 혼합 비교 경고가 나고, 감소 루프에서는 랩어라운드.
- `return` 이 없는 경로는 쓰레기 값을 돌려주는 UB (컴파일러 `-Werror=return-type`).

**한 줄 설명 연습**: "enum 을 받는 함수는 범위 밖 값이 올 수 있다고 가정하고 default 에서 오류를 낸다."
