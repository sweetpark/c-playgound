# P070 · sd25 hcaccount — 하드코드된 사용자 계정 — 계정명도 외부 전달로 치환

## 요구사항

서비스가 DB에 접속할 계정명과 패스워드로 로그인 문자열을 만든다.
- 계정명은 절대 소스에 상수로 있으면 안 된다(패스워드를 바꿔도 계정을 바꿀 수 없는 구조는 안 된다).

이미 `solution.c` 에 **Bad 함수가 들어 있다**(취약점을 먼저 관찰하세요). 같은 소재의 **Good 함수만 구현**한다. 치환 규칙: **계정명도 외부 전달로 치환**.

## 시그니처

```c
void db_login_bad(const char *passwd, char *out, size_t out_size);
void db_login_good(const char *user, const char *passwd, char *out, size_t out_size);
```

## 입출력 예시

| 상황 | Bad 결과 | 이유 |
| :--- | :--- | :--- |
| 패스워드를 무엇으로 바꿔도 | **`user=root` 로 고정** | 계정명이 소스에 박혀 있다 |

## 제약

- Bad 함수와 `mock_*` 파일(있다면)은 수정하지 않는다. `solution.c` 의 Good 함수만 채운다.
- Bad 는 **실제로 뚫린다**는 것이 시험의 일부다(자식 프로세스로 실행). Good 은 같은 공격을 막고 정상 입력은 그대로 통과시켜야 한다(과잉 차단 금지).
- 경고 0, ASan/UBSan 0 (`-Wall -Wextra -Werror`).

## 통과 기준

`make P070` → `PASS` (실패 시 `FAIL k` 와 어긋난 항목 출력), 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.

시험이 확인하는 것:

- user=root
- user=root
- user=svc_alice
- Good은 계정명을 바꿔 부르면 결과도 달라져야 한다
