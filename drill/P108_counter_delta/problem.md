# P108 · counter_delta — 누적 카운터로 구간 통계 만들기

> 예상 시간: 30분. `test.c` 의 `[3]`~`[5]`(되돌아가는 카운터)를 먼저 읽자.

## 요구사항

서비스는 요청이 올 때마다 **누적 카운터**를 +1 만 한다. 수집기가 1분마다 읽어서 **차이**로 "이번 분 건수"를 만든다.
그런데 카운터는 되돌아갈 수 있다 — 재시작(0 부터 다시), 오버플로(최대값 다음은 0). 또 구간 평균을 합칠 때는 평균의 평균을 내면 틀린다.

| 함수 | 하는 일 |
|---|---|
| `ctr_create(max)` / `ctr_destroy` | 카운터 감시기. `max < 3` 이면 `NULL` |
| `ctr_feed(c, value, &delta)` | 새로 읽은 값으로 증가량을 계산. 종류: `FIRST`/`OK`/`WRAP`/`RESET` |
| `wavg(avg, cnt, n)` | 구간 평균들을 **건수 가중**으로 합친 평균 |
| `sum_check(req, rsp, to, cancel, inflight, &diff)` | `req == rsp+to+cancel+inflight` 검산 |

## 규칙: ctr_feed

| 조건 | 종류 | delta |
|---|---|---|
| 첫 측정 | `FIRST` | 0 |
| `value >= 이전` | `OK` | `value - 이전` |
| `value < 이전`, 이전이 **윗 1/4**(`이전 > max - max/4`) | `WRAP` | `(max - 이전) + 1 + value` |
| `value < 이전`, 그 밖 | `RESET` | `value` |

어느 경우든 "이전 값"은 `value` 로 갱신한다.

## 입출력 예시

| 최대 | 이전 → 지금 | 종류 | delta |
|---|---|---|---|
| 2³²−1 | 4294967290 → 5 | WRAP | 11 |
| 2³²−1 | 1000 → 7 | RESET | 7 |
| 100 | 76 → 10 | WRAP | 35 |
| 100 | 75 → 10 | RESET | 10 |

평균: `(10ms, 100건)` + `(40ms, 900건)` → **37.0** (단순 평균 25 가 아님).

## 제약

- `solution.c` 만 고친다. 경고 0, ASan/UBSan 0 (누수 없음). `max` 가 `~0ULL` 이어도 오버플로가 없어야 한다.

## 통과 기준

`make P108` → `PASS`. 막히면 `HINT1.md` → `HINT2.md`. 풀고 나면 `NOTE.md`.
