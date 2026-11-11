# P052 · sd07 ldapbase — LDAP 처리 — 베이스 DN 상수 고정으로 치환

## 요구사항

"신입사원(NewHires)" 부서 안에서 매니저가 `m1` 인 팀원만 조회해야 한다.
- 검색 스코프(base)는 원래 `"ou=NewHires"` 로 고정되어야 한다.
- 필터(`manager=m1`)는 고정값이다.

이미 `solution.c` 에 **Bad 함수가 들어 있다**(취약점을 먼저 관찰하세요). 같은 소재의 **Good 함수만 구현**한다. 치환 규칙: **베이스 DN 상수 고정으로 치환**.

신뢰 경계:

| 값 | 출처 | 검증 없이 흘러가는 곳 |
| :--- | :--- | :--- |
| `userSuppliedBase` | 외부(파일에서 읽은 값 등) | LDAP 검색 스코프(base) |

## 시그니처

```c
int ldap_by_manager_bad(const char *userSuppliedBase);
int ldap_by_manager_good(void);
```

## 입출력 예시

| 입력(base) | Bad 결과(매칭 개수) | 이유 |
| :--- | :--- | :--- |
| `"ou=NewHires"` (정상) | 2 (alice, bob) | 정상 동작 |
| `"*"` | **3 (alice, bob, carol)** | 스코프가 무력화되어 다른 부서(Finance)의 carol까지 노출된다 |

## 제약

- Bad 함수와 `mock_*` 파일(있다면)은 수정하지 않는다. `solution.c` 의 Good 함수만 채운다.
- Bad 는 **실제로 뚫린다**는 것이 시험의 일부다(자식 프로세스로 실행). Good 은 같은 공격을 막고 정상 입력은 그대로 통과시켜야 한다(과잉 차단 금지).
- 경고 0, ASan/UBSan 0 (`-Wall -Wextra -Werror`).

## 통과 기준

`make P052` → `PASS` (실패 시 `FAIL k` 와 어긋난 항목 출력), 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.

시험이 확인하는 것:

- ou=NewHires
- Bad는 base가 오염되면 스코프가 무력화되어 다른 부서(carol)까지 노출된다(취약점 재현)
- Good은 base를 상수로 고정했으므로 항상 NewHires 2명만 나온다(우회 자체가 불가능)
