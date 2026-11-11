# P048 · sd03 xss — 크로스사이트 스크립트(XSS) — 출력 직전 HTML 이스케이프로 치환

## 요구사항

게시글 제목 문자열을 `<FONT SIZE=+1><STRONG>...</STRONG></FONT>` HTML로 감싸 렌더링한다.
- `<`, `>`, `&`, `"` 네 문자는 각각 `&lt;`, `&gt;`, `&amp;`, `&quot;` 로 치환한다.
- 그 외 문자는 그대로 통과시킨다.

이미 `solution.c` 에 **Bad 함수가 들어 있다**(취약점을 먼저 관찰하세요). 같은 소재의 **Good 함수만 구현**한다. 치환 규칙: **출력 직전 HTML 이스케이프로 치환**.

신뢰 경계:

| 값 | 출처 | 검증 없이 흘러가는 곳 |
| :--- | :--- | :--- |
| `title` | 외부(게시글 작성 폼) | HTML 출력 스트림 |

## 시그니처

```c
void render_title_bad(const char *title, char *out, size_t out_size);
void render_title_good(const char *title, char *out, size_t out_size);
```

## 입출력 예시

| 입력 | Bad 결과 | 이유 |
| :--- | :--- | :--- |
| `Hello World` (정상) | 그대로 출력 | 정상 동작 |
| `<script>alert(1)</script>` | **`<script>` 태그가 그대로 살아남는다** | 열람자 브라우저에서 스크립트가 실행된다 |

## 제약

- Bad 함수와 `mock_*` 파일(있다면)은 수정하지 않는다. `solution.c` 의 Good 함수만 채운다.
- Bad 는 **실제로 뚫린다**는 것이 시험의 일부다(자식 프로세스로 실행). Good 은 같은 공격을 막고 정상 입력은 그대로 통과시켜야 한다(과잉 차단 금지).
- 경고 0, ASan/UBSan 0 (`-Wall -Wextra -Werror`).

## 통과 기준

`make P048` → `PASS` (실패 시 `FAIL k` 와 어긋난 항목 출력), 경고 0, ASan/UBSan 0. 힌트: `HINT1.md` → `HINT2.md`.

시험이 확인하는 것:

- <script>
- Hello World
- <script>
- &lt;script&gt;
- Hello World
