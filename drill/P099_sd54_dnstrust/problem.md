# P099 · sd54 dnstrust — DNS lookup에 의존한 보안결정 — IP 화이트리스트로 치환

## 요구사항

신뢰할 수 있는 사이트로만 계약서를 전송한다.
- DNS가 조작되더라도 신뢰 판정이 뒤집혀서는 안 된다.

이미 `solution.c` 에 **Bad 함수가 들어 있다**(취약점을 먼저 관찰하세요). 같은 소재의 **Good 함수만 구현**한다. 치환 규칙: **IP 화이트리스트로 치환**.

신뢰 경계:

| 값 | 출처 | 검증 없이 흘러가는 곳 |
| :--- | :--- | :--- |
| `hostname`으로 조회한 DNS 응답(IP) | DNS 서버(캐시 포이즈닝에 취약) | 그 IP를 그대로 전송 목적지로 신뢰하는 지점 |

## 시그니처

```c
const char *resolve_hostname_like(const char *hostname, int poisoned);
int decide_send_target_bad(const char *hostname, int poisoned, char *out_ip, size_t out_size);
int decide_send_target_good(const char *claimed_ip, char *out_ip, size_t out_size);
```

## 입출력 예시

| 상황 | Bad 결과 | 이유 |
| :--- | :--- | :--- |
| DNS 정상 | 진짜 신뢰 IP(`203.0.113.10`)로 전송 | 우연히 문제없이 동작한다 |
| **DNS 포이즈닝됨** | **여전히 "신뢰함"으로 판정되고, 전송 목적지가 공격자 IP(`198.51.100.66`)로 바뀜** | 신뢰 판정 자체가 도메인 **이름**만 보고 이뤄져, DNS가 실제로 무엇을 반환했는지는 검증하지 않는다 |
| 같은 상황(Good) | 공격자 IP는 화이트리스트에 없으므로 거부 | 이름이 아니라 **IP 자체**를 화이트리스트와 비교한다 — DNS 조회 결과가 판정에 아예 관여하지 않는다 |

## 제약

- Bad 함수와 `mock_*` 파일(있다면)은 수정하지 않는다. `solution.c` 의 Good 함수만 채운다.
- Bad 는 **실제로 뚫린다**는 것이 시험의 일부다(자식 프로세스로 실행). Good 은 같은 공격을 막고 정상 입력은 그대로 통과시켜야 한다(과잉 차단 금지).
- 경고 0, ASan/UBSan 0 (`-Wall -Wextra -Werror`).

## 통과 기준

`make P099` → `PASS` (실패 시 `FAIL k` 와 어긋난 항목 출력), 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.

시험이 확인하는 것:

- 포이즈닝이 없으면 정상 전송돼야 한다
- 203.0.113.10
- Bad는 DNS가 포이즈닝돼도 도메인명만 보고 여전히 '신뢰함'으로 판정해야 한다(취약점 재현)
- 198.51.100.66
- 화이트리스트에 있는 IP는 통과해야 한다
- 203.0.113.10
- Good은 DNS 이름을 아예 보지 않고 IP 자체를 비교하므로 공격자 IP를 거부해야 한다
