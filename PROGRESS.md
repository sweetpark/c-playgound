# PROGRESS — 하루 한 문제

아래 **일정표의 순서**대로 하루 한 문제씩 푼다. 루틴은 [README](README.md#오늘의-한-문제-하루-30분-루틴) 참고.
`make P001` 처럼 번호만으로 채점한다. 번호 순서가 곧 학습 순서다 (총 110 문제, 전부 `make verify` 로 모범답안 PASS · 백지 FAIL 확인). 단, 아래 일정표는 **4부(P104~)를 먼저** 권한다.

## 일정표 — 하루 30–60분, 주 5일, 하루 한 문제

날짜가 밀려도 **순서**를 지킨다. 시간은 목표치다.

| 단계 | 주 | 문제 | 비고 |
|---|---|---|---|
| 1. 개념 드릴 (4부) | 1주 | P104 → P105 → P106 → P107 → P108 | 짝이 되는 이론 글을 먼저 읽는다 |
| | 2주 | P109 → P110(**60분, 큰 문제**) → 복습 → P001 → P002 | 복습 = 막혔던 문제를 `make reset-Pxxx` 로 백지에서 다시 |
| 2. C 기초 (1부) | 3–9주 | 주 5문제씩: 3주 P003–P007, 4주 P008–P012, 5주 P013–P017, 6주 P018–P022, 7주 P023–P027, 8주 P028–P032, 9주 P033–P037 | 문자열 → 메모리 → errno·파일 → 시그널·동시성 → 엔디안·API 순서 |
| 3. 템플릿 체화 (2부) | 10–11주 | 10주 P038–P042, 11주 P043–P045 + 복습 2일 | 순수 C 뼈대 |
| 4. 시큐어코딩 (3부, 선택) | 12–23주 | 주 5문제씩 P046–P103 (23주에 P101–P103) | 급하지 않으면 마지막에 |

**반복**: 4부는 4주 뒤 `make reset-P104` 로 백지에서 다시 풀고, 각 `NOTE.md` 의 "한 줄 설명 연습"을 내 말로 써 본다. 막힌 문제는 풀이를 보고 끝내지 말고 **다음 날 백지에서 다시** 푼다.

## 1부. C 기초 · 문자열 / 메모리 / errno / 파일 / 환경 / 시그널 / 동시성 / 바이트 순서 / API (P001–P037)

원본 소재: `c_basic/` 와 블로그 「C 기초 강의 실습」 글. 업무에서 자주 틀리는 순서(문자열 → sizeof → snprintf → malloc/free → errno → 파일 → …).

| 번호 | 제목 |
|---|---|
| P001 | safe_copy — 항상 널로 끝나는 문자열 복사 |
| P002 | sizeof_macros — 배열은 크기를 알고, 포인터는 모른다 |
| P003 | build_path — snprintf 반환값, NULL 과 "" 는 다르다 |
| P004 | str_append — strcat 을 안전하게 (strlen 은 한 번만) |
| P005 | get_dirname — const 문자열은 자를 수 없다, 복사해서 잘라라 |
| P006 | read_line — 길이 상한이 있는 한 줄 읽기 |
| P007 | int_array — 0 으로 초기화된 int 배열 할당과 해제 |
| P008 | list_free — 해제한 노드를 다시 읽지 마라 |
| P009 | rec_create — 중간에 실패해도 새지 않게 (early return 누수) |
| P010 | conn_reset — 구조체 비우기, sizeof 는 포인터가 아니라 구조체에 |
| P011 | blob_clone — 유연 배열 구조체는 헤더만 복사되지 않게 |
| P012 | sbuf_append — realloc 은 주소를 바꾼다 (이중 포인터와 임시 변수) |
| P013 | my_atoi — strtol 로 만드는 엄격한 정수 변환 |
| P014 | run_step — 정리 코드가 errno 를 덮어쓰기 전에 저장 |
| P015 | parse_port — strtoul 은 "-1" 을 받아들인다 |
| P016 | create_new — 있으면 덮어쓰지 않는다 (wx) |
| P017 | chomp — 개행은 있을 때만 지운다 |
| P018 | count_lines — fgets 조각과 EOF/오류 구분 |
| P019 | save_atomic — 임시 파일 + rename |
| P020 | env_copy — getenv 결과는 복사해서 쓴다 |
| P021 | parse_kv — 공백 제거·경계·입력 불변 |
| P022 | sig_counter — sigaction 과 핸들러에서 할 수 있는 일 |
| P023 | wait_signal — 막고 기다리기 (sigtimedwait) |
| P024 | counter_lock — 읽기까지 같은 락, ERRORCHECK 뮤텍스 |
| P025 | transfer — 락 두 개, 획득 순서 고정 |
| P026 | bounded_queue — mutex + 조건변수 2개 + close |
| P027 | run_squares — 스레드 인자의 수명과 join |
| P028 | split_words — strtok_r 와 입력 불변 |
| P029 | be64 — 빅엔디안 64비트 변환 |
| P030 | hdr_codec — 메시지 헤더 인코드/디코드 |
| P031 | count_steps — size_t 루프의 랩어라운드 |
| P032 | level_table — enum switch 와 default |
| P033 | array_params — conformant 배열 매개변수 |
| P034 | sum_file — 읽기 루프 종료 이유와 에러 코드 |
| P035 | write_all — 부분 쓰기·EINTR·EAGAIN |
| P036 | safe_macros — FREE / XNEW / XNEWN |
| P037 | join_strs — snprintf 반환값 누적 |

## 2부. 템플릿 체화 드릴 d0–d8, 변형 A (P038–P045)

원본: 옛 `d?_*/` 드릴(삭제됨, git 이력). 헤더(타입·상수)는 문제 폴더에 이미 있고 함수 본문만 채운다. **d2, d8 은 크다** — 며칠에 나눠 풀어도 된다. d6(설계 산출물)·d9(회고)는 코드가 아니라서 없다.

| 번호 | 제목 |
|---|---|
| P038 | d0 워밍업 — 가드와 CLEANUP 뼈대 |
| P039 | d1 — 자원 없는 함수 — 가드·단일출구·시험 3개 |
| P040 | d2 — 표 3장 → 구조체 — Optional·Conditional·union·정렬 |
| P041 | d3 — 자원 잡는 함수 — goto CLEANUP·create·destroy |
| P042 | d4 — Pack·Unpack — 비트필드·엔디안·왕복시험 |
| P043 | d5 — TLV 파서 — 모르는 태그 건너뛰기 |
| P044 | d7 — 데몬 — 시그널·poll·워커큐·graceful shutdown |
| P045 | d8 — 3GPP — IE 표 한 장을 끝까지 |

## 3부. 시큐어코딩 체화 드릴 SD01–SD58 (P046–P103)

원본: 옛 `sd??_*/` 드릴(삭제됨, git 이력). **P(45+번호)** 규칙: SD01 → P046, SD11 → P056, SD58 → P103. Bad 함수가 이미 들어 있고 **Good 만** 구현한다. mock 환경(`mock_*`)은 문제 폴더에 같이 있다.

| 번호 | 제목 |
|---|---|
| P046 | sd01 sqli — SQL 삽입 — 인자화된 질의문으로 치환 |
| P047 | sd02 resinj — 자원 삽입 — 화이트리스트 범위 검사로 치환 |
| P048 | sd03 xss — 크로스사이트 스크립트(XSS) — 출력 직전 HTML 이스케이프로 치환 |
| P049 | sd04 oscmd — OS 명령어 삽입 — 위험 문자 검사로 치환 |
| P050 | sd05 ldapinj — LDAP 삽입 — 영숫자 화이트리스트로 치환 |
| P051 | sd06 pathtraversal — 디렉터리 경로 조작 — 절대경로 정규화 검증으로 치환 |
| P052 | sd07 ldapbase — LDAP 처리 — 베이스 DN 상수 고정으로 치환 |
| P053 | sd08 cookietamper — 보호 메커니즘을 우회할 수 있는 입력값 변조 — 세션ID만 신뢰하도록 치환 |
| P054 | sd09 sysconfig — 시스템 또는 구성 설정의 외부 제어 — 인덱스 간접참조로 치환 |
| P055 | sd10 proccontrol — 프로세스 제어 — 허용 명령 매핑으로 치환 |
| P056 | sd11 stackoverflow — 스택 버퍼 오버플로우 — 길이 검사 후 strncpy로 치환 |
| P057 | sd12 heapoverflow — 힙 버퍼 오버플로우 — 길이 검사 후 strncpy로 치환 |
| P058 | sd13 writebeforestart — 버퍼 시작 지점 이전에 쓰기 — 하한 검사로 치환 |
| P059 | sd14 readoverflow — 범위 초과해서 읽기 — 길이 동반 전달로 치환 |
| P060 | sd15 writeindex — 검사되지 않은 배열 인덱싱 — 하한·상한 동시 검사로 치환 |
| P061 | sd16 nullterm — 널 종료 문제 — 길이 명시 복사와 수동 종료로 치환 |
| P062 | sd17 intoverflow — 정수 오버플로우 — 연산 전 오버플로우 검사로 치환 |
| P063 | sd18 signext — 의도하지 않은 부호 확장 — 원본 타입으로 직접 검사하도록 치환 |
| P064 | sd19 unsignedtosigned — 무부호 정수를 부호 정수로 타입 변환 오류 — signed로 받아 검증 후 변환으로 치환 |
| P065 | sd20 idor — 부적절한 인가 — 세션 식별자 대조로 치환 |
| P066 | sd21 umask — 중요한 자원에 대한 잘못된 권한허용 — 최소 권한 umask로 치환 |
| P067 | sd22 privgrant — 잘못된 권한 부여 — 그룹별 UID 테이블로 치환 |
| P068 | sd23 privdrop — 최소 권한 적용 위배 — 즉시 권한 복귀로 치환 |
| P069 | sd24 hcpasswd — 하드코드된 패스워드 — 외부 설정 로드로 치환 |
| P070 | sd25 hcaccount — 하드코드된 사용자 계정 — 계정명도 외부 전달로 치환 |
| P071 | sd26 hckey — 하드코드된 암호화 키 — 키 저장소 주입으로 치환 |
| P072 | sd27 commentleak — 주석문안에 포함된 주요정보 — 배포 전 자동 검사로 치환 |
| P073 | sd28 cipherchoice — 취약한 암호화 알고리즘 사용 — 식별자 화이트리스트 검증으로 치환 |
| P074 | sd29 plaintextstore — 사용자 중요정보 평문 저장(또는 전송) — 전송 전 변환으로 치환 |
| P075 | sd30 keylen — 충분하지 못한 키 길이 사용 — 최소 길이 상수 검증으로 치환 |
| P076 | sd31 weakrandom — 적절하지 않은 난수 값의 사용 — 커널 CSPRNG로 치환 |
| P077 | sd32 pwplain — 패스워드 평문 저장 — 해시 저장으로 치환 |
| P078 | sd33 nosalt — 솔트 없이 일방향 해쉬 함수 사용 — 사용자별 난수 솔트로 치환 |
| P079 | sd34 rsapad — 취약한 암호화 적절하지 못한 RSA 패딩 — OAEP 패딩 강제로 치환 |
| P080 | sd35 hcsalt — 취약한 암호화 해쉬함수 하드코드된 솔트 — 호출마다 새 솔트로 치환 |
| P081 | sd36 reuseport — 같은 포트번호의 다중 연결 — SO_REUSEPORT 신중 사용으로 치환 |
| P082 | sd37 toctou — 경쟁 조건 — 파일 디스크립터 원자적 생성으로 치환 |
| P083 | sd38 recursion — 제대로 제어되지 않은 재귀 — 깊이 제한 동반으로 치환 |
| P084 | sd39 symmap — 심볼릭명이 정확한 대상에 매핑되어 있지 않음 — mkstemp로 치환 |
| P085 | sd40 errmsg — 오류 메시지를 통한 정보 노출 — 일반 메시지 응답으로 치환 |
| P086 | sd41 erract — 오류상황 대응 부재 — 실제 조치 동반으로 치환 |
| P087 | sd42 except — 적절하지 않은 예외처리 — 반환값 검사 동반으로 치환 |
| P088 | sd43 nullderef — 널 포인터 역참조 — 반환값 검사 후 사용으로 치환 |
| P089 | sd44 resleak — 부적절한 자원 해제 — 단일 해제 지점으로 치환 |
| P090 | sd45 signconv — 부호 정수를 무부호 정수로 타입 변환 오류 — 안전한 에러값으로 치환 |
| P091 | sd46 int2char — 정수를 문자로 변환 — 용도에 맞는 타입 선언으로 치환 |
| P092 | sd47 stackret — 스택 변수 주소 리턴 — 힙 할당 반환으로 치환 |
| P093 | sd48 macropair — 매크로의 잘못된 사용 — 짝 매크로 동반으로 치환 |
| P094 | sd49 badfree — 코드정확성 스택 주소 해제 — malloc 포인터만 해제로 치환 |
| P095 | sd50 threadexit — 코드정확성 스레드 조기 종료 — join 또는 detach 명시로 치환 |
| P096 | sd51 unbounded — 무한 자원 할당 — 상한 및 풀로 치환 |
| P097 | sd52 debugcode — 남은 디버그 코드 — 빌드 매크로 제거로 치환 |
| P098 | sd53 sysdata — 시스템 데이터 정보노출 — 경로 제외 일반 메시지로 치환 |
| P099 | sd54 dnstrust — DNS lookup에 의존한 보안결정 — IP 화이트리스트로 치환 |
| P100 | sd55 dangerfunc — 위험하다고 알려진 함수 사용 — fork 치환으로 치환 |
| P101 | sd56 chrootjail — 작업 디렉터리 변경 없는 chroot Jail 생성 — chdir 동반으로 치환 |
| P102 | sd57 mbstr — 오용 문자열 관리 — 크기 인자 동반과 멀티바이트 경계 인식으로 치환 |
| P103 | sd58 getloginsim — 다중 스레드 프로그램에서 getlogin() 사용 — getlogin_r로 치환 |

## 4부. 개념 드릴 — 이론 글과 짝 (P104–)

블로그 이론 글의 핵심 동작을 **직접 만들어 보는** 드릴. 글에서 말한 규칙이 시험 항목으로 그대로 들어 있다.

| 번호 | 제목 |
|---|---|
| P104 | alog_ring — 비동기 로거: 큐가 가득 차면 기다리지 않고 버린다 (글: 「[C] 비동기 로깅과 링 버퍼」) |
| P105 | msgq_probe — System V 메시지 큐: 채우기·EAGAIN·보낸 쪽이 죽어도 남는 큐 (글: 「[OS] 메시지 큐가 가득 차면」) |
| P106 | shm_probe — 공유메모리 세그먼트의 수명: nattch, 죽은 프로세스, IPC_RMID 예약(SHM_DEST) (글: 「[OS] 공유메모리 세그먼트 — 크기·key·권한·수명과 삭제」) |
| P107 | alarm_fsm — 임계·히스테리시스·연속 N회로 깜박이지 않는 알람 (글: 「[모니터링] 알람 상태 머신」) |
| P108 | counter_delta — 누적 카운터 차분, WRAP/RESET, 가중 평균, 합계 검산 (글: 「[모니터링] 누적 카운터와 게이지」) |
| P109 | load_shed — 초당 한도(고정 구간)·단계별 거절·지수 백오프+지터 (글: 「[HTTP] 과부하 제어」) |
| P110 | wal_kv — 로그(WAL)+체크포인트 키-값 저장소: 크래시 복구, 찢어진 꼬리, 옛 로그 건너뛰기 (글: 「[DB] 인메모리 DB의 내구성」) |

## 아직 만들지 않은 것 (후속 후보)

- **변형 B / C**: d1–d8, sd01–sd58 의 원본 글에 있는 변형 B/C 요구사항. "변형은 새 번호의 새 문제" 규칙대로 P104 이후로 추가한다 (모범답안·시험이 원본 저장소에 없으므로 새로 써야 한다).
- **c_basic 남은 소재**: `13-01` (stdout 오류는 `ferror`), `13-03` (`abort`/`atexit`/`_Exit` 종료 경로), `11-03` (`atexit` 핸들러 순서), `15-14`/`15-15` (스레드 취소·시그널), `15-04` (락 안에서 I/O), `15-13` (detach/join) — 테스트하기 까다로워 보류.
- **d6 / d9**: 코드가 아닌 산출물(Mermaid, 요구사항→데몬 종합)이라 채점 대상이 아니다.
- **P045(d8)** 는 `libjansson-dev` 가 필요하다 (`ldlibs` 파일에 `-ljansson`). 설치 방법은 README 참고.
