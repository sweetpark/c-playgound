# P074 · sd29 plaintextstore — 사용자 중요정보 평문 저장(또는 전송) — 전송 전 변환으로 치환

## 요구사항

로그인 패킷에 실을 패스워드 필드를 준비한다.
- 패킷에 담기는 값은 원본 패스워드와 달라야 한다.
- 정당한 수신자는 원본을 복원할 수 있어야 한다(암호화는 은닉이지 파기가 아니다).

이미 `solution.c` 에 **Bad 함수가 들어 있다**(취약점을 먼저 관찰하세요). 같은 소재의 **Good 함수만 구현**한다. 치환 규칙: **전송 전 변환으로 치환**.

## 시그니처

```c
void prepare_for_send_bad(const char *pw, char *out, size_t out_size);
void prepare_for_send_good(const char *pw, char *out, size_t out_size);
void undo_mask(const char *masked, char *out, size_t out_size);
```

## 입출력 예시

| 값 | Bad 결과 | 이유 |
| :--- | :--- | :--- |
| `mypassword123` | **패킷에 그대로 실림** | 스니핑 한 번으로 그대로 노출 |

## 제약

- Bad 함수와 `mock_*` 파일(있다면)은 수정하지 않는다. `solution.c` 의 Good 함수만 채운다.
- Bad 는 **실제로 뚫린다**는 것이 시험의 일부다(자식 프로세스로 실행). Good 은 같은 공격을 막고 정상 입력은 그대로 통과시켜야 한다(과잉 차단 금지).
- 경고 0, ASan/UBSan 0 (`-Wall -Wextra -Werror`).

## 통과 기준

`make P074` → `PASS` (실패 시 `FAIL k` 와 어긋난 항목 출력), 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.

시험이 확인하는 것:

- mypassword123
- mypassword123
- mypassword123
