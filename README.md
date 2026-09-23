# c-playgound

C 템플릿 체화 드릴(D0~D9) 실습 저장소. 문제/뼈대 스펙은 Notion 볼트의
`템플릿 체화 드릴` 노트를 따른다 — 여기는 실제로 손으로 치는 코드만 둔다.

## 환경

- 컴파일/실행/디버깅은 전부 **WSL2 Ubuntu** 안에서 한다 (POSIX 시그널·스레드·poll 때문에 네이티브 Windows에서는 일부 코드가 그대로 안 돌아감).
- VS Code에서 `Ctrl+Shift+P` → `WSL: Reopen Folder in WSL` 로 이 폴더를 열면 이후 터미널·빌드·디버깅이 전부 Ubuntu 안에서 실행된다.

## 사용법

```bash
make D=d1_pure T=main        # 빌드 + 실행
make D=d1_pure T=main check  # 경고 0 + 새니타이저 0 검사
make clean
```

VS Code에서는 `Ctrl+Shift+B`(빌드) 또는 `F5`(디버깅)를 누르면 드릴 이름(D)과 타깃 이름(T)을 물어본다.

## 폴더

각 드릴은 `d?_이름/src/`, `d?_이름/test/` 를 쓴다. 공용 헤더(`common.h`/`guard.h`/`log.h`)는
`common/`에 두되, D0(워밍업)만은 매번 백지에서 새로 친다.
