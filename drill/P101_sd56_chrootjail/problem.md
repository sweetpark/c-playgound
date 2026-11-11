# P101 · sd56 chrootjail — 작업 디렉터리 변경 없는 chroot Jail 생성 — chdir 동반으로 치환

## 요구사항

클라이언트가 지정한 파일명을 jail 디렉터리 안에서만 열어야 한다.
- jail 설정 이후 상대 경로로 jail 밖 파일에 접근할 수 없어야 한다.

이미 `solution.c` 에 **Bad 함수가 들어 있다**(취약점을 먼저 관찰하세요). 같은 소재의 **Good 함수만 구현**한다. 치환 규칙: **chdir 동반으로 치환**.

신뢰 경계:

| 값 | 출처 | 검증 없이 흘러가는 곳 |
| :--- | :--- | :--- |
| 작업 디렉터리(상대 경로 해석 기준점) | `chroot()` 이후 남겨진(또는 새로 설정된) cwd | 클라이언트가 보낸 상대 경로(`filename`)를 그 기준점으로 풀어내는 지점 |

## 시그니처

```c
int resolve_in_jail_bad(const char *stale_cwd, const char *user_path, char *out, size_t out_size);
int resolve_in_jail_good(const char *jail_root, const char *user_path, char *out, size_t out_size);
```

## 입출력 예시

| 상황 | Bad 결과 | 이유 |
| :--- | :--- | :--- |
| `chroot()` 후 `chdir` 없이 상대 경로 `"etc_shadow_like"` 요청 | **jail 밖의 비밀 파일 내용을 그대로 읽어냄** | 작업 디렉터리가 여전히 jail 밖(`chroot` 이전 위치)이라, 상대 경로가 그 위치를 기준으로 풀린다 |
| 같은 상황(Good) | 같은 이름의 파일을 jail 밖에서 찾지 못해 실패 | `chdir("/")`로 작업 디렉터리 자체가 jail 루트로 옮겨져, 상대 경로의 기준점이 jail 안으로 바뀐다 |
| jail 안의 정상 파일 요청(Good) | 정상적으로 읽음 | jail 루트를 기준으로 한 상대 경로이므로 올바르게 해석된다 |

## 제약

- Bad 함수와 `mock_*` 파일(있다면)은 수정하지 않는다. `solution.c` 의 Good 함수만 채운다.
- Bad 는 **실제로 뚫린다**는 것이 시험의 일부다(자식 프로세스로 실행). Good 은 같은 공격을 막고 정상 입력은 그대로 통과시켜야 한다(과잉 차단 금지).
- 경고 0, ASan/UBSan 0 (`-Wall -Wextra -Werror`).

## 통과 기준

`make P101` → `PASS` (실패 시 `FAIL k` 와 어긋난 항목 출력), 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.

시험이 확인하는 것:

- mkdtemp
- )을 안 해 여전히 jail 밖 작업 디렉터리를 기준으로 상대 경로를 찾아야 한다(취약점 재현)
- TOP-SECRET-SHADOW
- )로 기준점이 jail 루트 자신으로 바뀌어 같은 상대 경로로는 jail 밖 파일을 찾지 못해야 한다
- Good도 jail 안의 정상 파일은 그대로 찾아야 한다
- public data
