# P069 · sd24 hcpasswd — 하드코드된 패스워드 — 외부 설정 로드로 치환

## 요구사항

서버 주소, 사용자명, 패스워드로 DB 접속 문자열을 만든다.
- 패스워드는 절대 소스 코드에 상수로 있으면 안 된다.

이미 `solution.c` 에 **Bad 함수가 들어 있다**(취약점을 먼저 관찰하세요). 같은 소재의 **Good 함수만 구현**한다. 치환 규칙: **외부 설정 로드로 치환**.

## 시그니처

```c
void db_connect_bad(const char *server, const char *user, char *out, size_t out_size);
void db_connect_good(const char *server, const char *user, const char *passwd, char *out, size_t out_size);
```

## 입출력 예시

| 상황 | Bad 결과 | 이유 |
| :--- | :--- | :--- |
| 어떤 서버·사용자로 호출해도 | **`pw=asdf` 로 고정** | 패스워드가 소스에 박혀 있어 절대 안 바뀐다 |

## 제약

- Bad 함수와 `mock_*` 파일(있다면)은 수정하지 않는다. `solution.c` 의 Good 함수만 채운다.
- Bad 는 **실제로 뚫린다**는 것이 시험의 일부다(자식 프로세스로 실행). Good 은 같은 공격을 막고 정상 입력은 그대로 통과시켜야 한다(과잉 차단 금지).
- 경고 0, ASan/UBSan 0 (`-Wall -Wextra -Werror`).

## 통과 기준

`make P069` → `PASS` (실패 시 `FAIL k` 와 어긋난 항목 출력), 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.

시험이 확인하는 것:

- pw=asdf
- pw=asdf
- pw=s3cr3t!
- Good은 패스워드를 바꿔 부르면 결과도 달라져야 한다(교체 가능함을 증명)
