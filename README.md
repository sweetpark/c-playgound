# c-playgound

C 템플릿 체화 드릴(D0–D9)과 시큐어코딩 체화 드릴(SD01–SD58) 실습 저장소.
문제/뼈대 스펙은 Notion 볼트의 각 드릴 노트를 따른다 — 여기는 실제로 손으로 치는 코드만 둔다.

- [c코드 템플릿 (블로그)](https://sweetpark.github.io/개발-(cs)/언어/c언어/c코드-템플릿/c코드-템플릿-목록) — 드릴이 체화시키려는 뼈대 원본
- [템플릿 체화 드릴 (블로그)](https://sweetpark.github.io/개발-(cs)/언어/c언어/실습/템플릿-체화-드릴/템플릿-체화-드릴-목록) — D0~D9 드릴 스펙, 회차 규칙, 채점표
- [시큐어코딩가이드 (블로그)](https://sweetpark.github.io/개발-(cs)/언어/c언어/시큐어코딩가이드/시큐어코딩가이드-목록) — 58개 취약점의 Bad→Good 치환 원본(전자정부 C 시큐어코딩 가이드 기반)
- [시큐어코딩 체화 드릴 (블로그)](https://sweetpark.github.io/개발-(cs)/언어/c언어/실습/시큐어코딩-체화-드릴/readme) — SD01~SD58 드릴 스펙, 재현 전략, 회차 규칙, 채점표

## 오늘의 한 문제 (하루 30분 루틴)

코딩테스트처럼 **문제 한 장 = 폴더 하나**. 오늘 뭘 할지 고르지 않는다 — [PROGRESS.md](PROGRESS.md) 의 일정표 순서대로 푼다.
시작이 막히면 [MISTAKES.md](MISTAKES.md) 의 맨 윗줄들을 먼저 소리 내어 읽는다.

```
drill/P001_safe_copy/
  problem.md   문제만 (요구사항 / 시그니처 / 입출력 예시 / 제약 / 통과 기준)
  solution.c   내가 채우는 파일 (시그니처 + TODO)
  test.c       채점용: 'PASS n/m' 출력. NULL·크기 0·경계값·널 종료 누락 같은 함정 포함
  HINT1.md     한 줄 힌트
  HINT2.md     의사코드 (막혔을 때만)
  NOTE.md      푼 뒤에 보는 해설: 원본 블로그 글 링크 + 함정 요약
solutions/P001_safe_copy/
  solution.c   모범답안 (풀기 전에는 열지 않는다)
  skeleton.c   solution.c 의 백지 원본 (make reset-P001 이 이걸로 되돌린다)
```

| 순서 | 시간 | 할 일 |
|---|---|---|
| ① 백지 구현 | 10분 | `problem.md` 만 보고 `solution.c` 를 채운다. 막히면 HINT1 → HINT2 순서 |
| ② 일부러 깨뜨리기 | 8분 | 일부러 틀린 버전(`strncpy` 그대로, `sizeof(c)`, `free` 순서 바꾸기…)을 만들어 `make P001` 이 잡아내는지 본다 |
| ③ 노트와 비교 | 5분 | `NOTE.md` 와 원본 글, 필요하면 `make P001 ANS=1` 로 모범답안 채점 결과를 본다 |
| ④ 한 줄 설명 | 7분 | NOTE 맨 아래 "한 줄 설명 연습"을 내 말로 쓴다. 틀린 건 MISTAKES.md 에 한 줄 추가 |

```bash
make P001              # drill/P001_*/solution.c 빌드 + 채점  (gcc -Wall -Wextra -Werror, ASan/UBSan/LSan)
make P001 ANS=1        # 모범답안으로 채점 (푼 뒤 확인용)
make reset-P001        # solution.c 를 백지로 되돌림 (같은 문제를 다시 풀 때. 작성 중인 내용은 사라짐)
make list              # 문제 목록
make verify            # 전 문제 채점 가능성 검증 (모범답안 PASS + 백지 FAIL)
```

- 출력은 `PASS 32/32` 또는 `FAIL 12/32` 와 실패한 검사의 줄 번호. 메모리 오류는 새니타이저가 바로 프로세스를 죽이며 보고한다(그것도 FAIL).
- `test.c` 가 `solution.c` 를 `#include` 하므로(매크로·`static` 도 시험할 수 있도록) 컴파일 단위는 `test.c` 하나다.
- 변형 문제는 같은 문서에 섞지 않고 **새 번호의 문제**로 추가한다.

- 문제 번호 구성: **P001–P037** C 기초(문자열·메모리·errno·파일·환경·시그널·동시성·바이트 순서·API), **P038–P045** 템플릿 체화 드릴 d0–d8(변형 A), **P046–P103** 시큐어코딩 체화 드릴 SD01–SD58(`P(45+SD번호)`), **P104–** 이론 글과 짝인 개념 드릴(P104 비동기 로거). 번호 순서가 학습 순서이고 목록은 [PROGRESS.md](PROGRESS.md).
- P038 이후(d*, sd*)는 기존 시험을 옮긴 것이라 출력이 `PASS` / `FAIL k` 형식이다(P001–P037 은 `PASS n/m`).
- d*/sd* 문제는 타입·상수가 든 헤더와 (sd 는) Bad 함수·mock 이 **이미 문제 폴더에 있다**. 채울 곳은 `solution.c` 의 `TODO` 뿐.
- P045(d8)는 `libjansson-dev` 가 필요하다(문제 폴더의 `ldlibs` 파일에 `-ljansson`). 없으면 그 문제만 빌드되지 않는다: `sudo apt install libjansson-dev`.

## 환경

### Windows

- 컴파일/실행/디버깅은 전부 **WSL2 Ubuntu** 안에서 한다 (POSIX 시그널·스레드·poll 때문에 네이티브 Windows에서는 일부 코드가 그대로 안 돌아감).
- VS Code에서 `Ctrl+Shift+P` → `WSL: Reopen Folder in WSL` 로 이 폴더를 열면 이후 터미널·빌드·디버깅이 전부 Ubuntu 안에서 실행된다.
- 디버깅 설정: `launch.json`의 "Debug drill (gdb, WSL)".

### macOS

- macOS는 그 자체로 POSIX(BSD 계열)라 WSL 같은 우회 레이어가 필요 없다. `xcode-select --install` 로 Command Line Tools(clang/make/lldb)만 설치하면 끝.
- **gdb 대신 lldb**를 쓴다. VS Code에 [CodeLLDB](https://marketplace.visualstudio.com/items?itemName=vadimcn.vscode-lldb) 확장을 설치하고, 디버깅 설정은 `launch.json`의 "Debug drill (lldb, macOS)"를 고른다.
- Remote 확장 불필요 — 폴더를 그냥 열면 된다.
- `c_cpp_properties.json`에서 IntelliSense 구성은 "macOS"를 선택(Apple Silicon 기준 `macos-clang-arm64`; 인텔 맥이면 `macos-clang-x64`로 바꿀 것).

## 폴더 지도

| 경로 | 내용 |
|---|---|
| `drill/Pxxx_이름/` | **하루 한 문제** 형식의 문제 한 장 (위 참조). `drill/drill_test.h` 는 `CHECK`/`PASS n/m` 헬퍼, `drill/verify.sh` 는 전체 검증 |
| `solutions/Pxxx_이름/` | 모범답안(`solution.c`)과 백지 원본(`skeleton.c`) — 풀기 전에는 열지 않는다 |
| `c_basic/` | 원본 C 기초 강의 실습 코드 (문제의 소재, 채점 없음. 문제 ↔ 원본 대응표는 `c_basic/README.md`) |
| `drill/common/` | 공용 헤더 (P038–P045 가 쓴다) (`common.h`/`guard.h`/`log.h`/`safe.h`/`mem.h`/`strx.h`) |
| `MISTAKES.md`, `PROGRESS.md` | 틀린 것 기록 / 진행 현황 |
| `Makefile` | `make P001` 등 (위 참조) |

옛 `d0_warmup … d8_3gpp/`, `sd01_… sd58_…/` 폴더와 `Makefile.drill` 은 새 형식(P038–P103)으로 옮긴 뒤 삭제했다. 이전 모습은 git 이력에 있다.

## 커밋 컨벤션 — 반복(체화) 기록

같은 문제를 여러 번 치는 게 이 저장소의 목적이라, 회차마다 새 파일을 만들지 않고
**같은 파일을 덮어쓰고 커밋**한다. 그러면 git log/diff 자체가 회차별 변화(복붙 → 재구성 → 백지)를
보여주는 기록이 된다.

- **커밋 시점**: 한 회차를 끝냈을 때(통과든 중간에 막혔든). 진행 중 컴파일 에러 스냅샷 등은 커밋하지 않는다.
- 같은 문제를 다시 풀 때는 `make reset-Pxxx` 로 백지로 되돌리고 같은 `solution.c` 를 덮어쓴다.
- **커밋 메시지 형식**: `<문제> r<회차> <소요시간> <자가채점점수>`
  예: `P001_safe_copy r1 28min 90pt`
- `build/` 는 gitignore 대상이라 신경 쓸 필요 없음.
