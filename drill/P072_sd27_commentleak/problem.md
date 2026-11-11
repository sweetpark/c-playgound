# P072 · sd27 commentleak — 주석문안에 포함된 주요정보 — 배포 전 자동 검사로 치환

## 요구사항

소스 코드 문자열을 입력받아, 블록 주석(`/* ... */`) 안에 `password`, `secret`, `apikey` 같은 위험 키워드가 있으면 배포를 막는다.
- 대소문자를 구분하지 않는다.

이미 `solution.c` 에 **Bad 함수가 들어 있다**(취약점을 먼저 관찰하세요). 같은 소재의 **Good 함수만 구현**한다. 치환 규칙: **배포 전 자동 검사로 치환**.

## 시그니처

```c
int find_secret_in_comments(const char *source_code);
int release_ok_bad(const char *source_code);
int release_ok_good(const char *source_code);
```

## 입출력 예시

| 소스 코드 | Bad 결과 | 이유 |
| :--- | :--- | :--- |
| 주석에 `default password is "abracadabra"` 포함 | **배포 승인(1)** | 검사 자체를 안 한다 |
| 정상 주석만 있음 | 배포 승인(1) | 위험 요소 없음 |

## 제약

- Bad 함수와 `mock_*` 파일(있다면)은 수정하지 않는다. `solution.c` 의 Good 함수만 채운다.
- Bad 는 **실제로 뚫린다**는 것이 시험의 일부다(자식 프로세스로 실행). Good 은 같은 공격을 막고 정상 입력은 그대로 통과시켜야 한다(과잉 차단 금지).
- 경고 0, ASan/UBSan 0 (`-Wall -Wextra -Werror`).

## 통과 기준

`make P072` → `PASS` (실패 시 `FAIL k` 와 어긋난 항목 출력), 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.

시험이 확인하는 것:

- Bad는 주석에 패스워드가 있어도 배포를 그대로 승인해야 한다(취약점 재현)
- Good은 주석에 위험 키워드가 있으면 배포를 막아야 한다
- Good은 위험 키워드가 없는 정상 소스는 배포를 승인해야 한다
