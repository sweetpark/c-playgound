# c-playgound

C 템플릿 체화 드릴(D0~D9)과 시큐어코딩 체화 드릴(SD01~SD58) 실습 저장소.
문제/뼈대 스펙은 Notion 볼트의 각 드릴 노트를 따른다 — 여기는 실제로 손으로 치는 코드만 둔다.

- [c코드 템플릿 (블로그)](https://sweetpark.github.io/개발-(cs)/언어/c언어/c코드-템플릿/c코드-템플릿-목록) — 드릴이 체화시키려는 뼈대 원본
- [템플릿 체화 드릴 (블로그)](https://sweetpark.github.io/개발-(cs)/언어/c언어/실습/템플릿-체화-드릴/템플릿-체화-드릴-목록) — D0~D9 드릴 스펙, 회차 규칙, 채점표
- [시큐어코딩가이드 (블로그)](https://sweetpark.github.io/개발-(cs)/언어/c언어/시큐어코딩가이드/시큐어코딩가이드-목록) — 58개 취약점의 Bad→Good 치환 원본(전자정부 C 시큐어코딩 가이드 기반)
- [시큐어코딩 체화 드릴 (블로그)](https://sweetpark.github.io/개발-(cs)/언어/c언어/실습/시큐어코딩-체화-드릴/readme) — SD01~SD58 드릴 스펙, 재현 전략, 회차 규칙, 채점표

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

## 사용법

```bash
make D=d1_pure T=main            # 빌드 + 실행 (템플릿 체화 드릴)
make D=sd01_sqli T=main check    # 경고 0 + 새니타이저 0 검사 (시큐어코딩 체화 드릴)
make clean
```

VS Code에서는 `Ctrl+Shift+B`(빌드) 또는 `F5`(디버깅)를 누르면 드릴 이름(D)과 타깃 이름(T)을 물어본다.

## 폴더

각 드릴은 `d?_이름/src/`, `d?_이름/test/` 를 쓴다(시큐어코딩 체화 드릴은 같은 자리에 `sd??_이름/src/`, `sd??_이름/test/`). 공용 헤더(`common.h`/`guard.h`/`log.h`)는
`common/`에 두되, D0(워밍업)만은 매번 백지에서 새로 친다.

시큐어코딩 체화 드릴 중 일부(`sd01_sqli`, `sd04_oscmd`, `sd05_ldapinj`, `sd07_ldapbase`,
`sd08_cookietamper`, `sd20_idor`, `sd26_hckey` 등)는 `src/` 아래에 `mock_*.h`/`.c` 환경 코드가
미리 포함돼 있다 — 실제 DB·LDAP·세션 저장소 없이 취약점을 재현하기 위한 최소한의 스텁이며,
직접 타이핑하는 대상이 아니다. Bad/Good 구현과 `test/`는 노트 스펙을 보고 직접 친다.

## 커밋 컨벤션 — 반복(체화) 기록

같은 드릴을 여러 번 치는 게 이 저장소의 목적이라, 회차마다 새 파일을 만들지 않고
**같은 파일을 덮어쓰고 커밋**한다. 그러면 git log/diff 자체가 회차별 변화(복붙 → 재구성 → 백지)를
보여주는 기록이 된다.

- **커밋 시점**: 한 회차를 끝냈을 때(통과든 중간에 막혔든). 진행 중 컴파일 에러 스냅샷 등은 커밋하지 않는다.
- **커밋 메시지 형식**: `<드릴> r<회차>(<변형>) <소요시간> <자가채점점수>`
  예: `d1_pure r2(B) 8min 95pt`
- `build/`는 gitignore 대상이라 신경 쓸 필요 없음.
