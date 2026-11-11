# P019 · save_atomic — 임시 파일 + rename

## 요구사항
`data` 를 `path` 파일에 **안전하게 저장**한다. 먼저 `path.tmp` 에 전부 쓰고 디스크까지 내린 뒤(`fflush` + `fsync`) `rename` 으로 교체한다. 어느 단계에서 실패해도 **원본 파일은 그대로**여야 하고 임시 파일은 남기지 않는다.

## 시그니처
```c
int save_atomic(const char *path, const char *data);   /* 0 성공 / -1 실패 (NULL 인자 포함) */
```

## 입출력 예시
| 상황 | 결과 |
|---|---|
| 기존 `"old"` 에 `"new content\n"` 저장 | 0, 파일 = 새 내용, `path.tmp` 없음 |
| 파일이 없던 경로 | 0 |
| 빈 문자열 / 1MB 문자열 | 0 |
| `path` 가 비어 있지 않은 디렉터리 (rename 실패) | -1, 디렉터리 그대로, `path.tmp` 정리됨 |
| 존재하지 않는 디렉터리 아래 | -1 |

## 제약
- `fclose` 의 반환값도 검사한다(지연된 쓰기 오류가 여기서 나온다).
- 임시 이름은 `snprintf` 로 만들고 잘림을 검사한다. `path` 에 직접 쓰는 방식 금지.
- `rename` 은 같은 디렉터리(같은 파일시스템) 안에서만 원자적이다.

## 통과 기준
`make P019` → `PASS n/n`, 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.
