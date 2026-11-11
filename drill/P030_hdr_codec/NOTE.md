# NOTE · P030 hdr_codec (풀고 난 뒤에 보세요)

**원본 글** (읽기 전용, 여기에 복사하지 않음)
- 「[C] 바이트 순서·메시지 헤더·컴파일러 경고 실습 — htonll·enc_dec와 size_t·switch 함정」 "enc_dec 구조체 통째 memcpy 는 엔디안·패딩이 새어 나옴 → 필드 단위 시프트 변환, 검증 후에만 출력"
- 소스: [`c_basic/msc/enc_dec.c`](../../c_basic/msc/enc_dec.c), [`enc_dec.h`](../../c_basic/msc/enc_dec.h), [`enc_dec_test.c`](../../c_basic/msc/enc_dec_test.c)

**함정 요약**
- 구조체 `memcpy` 는 호스트 엔디안과 패딩 바이트(초기화 안 된 쓰레기 = 정보 노출)를 그대로 내보낸다.
- 검증 없이 디코드한 값을 쓰면 다른 코드가 `len` 을 믿고 복사하다 오버플로.
- 실패 시 출력 구조체를 반쯤 채워 놓는 실수.

**한 줄 설명 연습**: "와이어 포맷은 필드마다 바이트를 직접 조립하고, 검증이 끝난 값만 안으로 들인다."
