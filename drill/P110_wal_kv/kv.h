/* P110 kv — 로그(WAL)와 체크포인트로 크래시에서 살아남는 작은 키-값 저장소. 구현은 solution.c
 *
 * 디렉터리 dir 안에 파일 두 개를 쓴다.
 *   wal.log   변경 로그. 한 줄 = 한 변경.   "<lsn> P <key> <val> <crc>\n"   또는   "<lsn> D <key> - <crc>\n"
 *   snapshot  체크포인트.                    "LSN <n>\n"  "<key> <val>\n" ...  "END\n"
 *
 *   key, val 은 [A-Za-z0-9_] 1~32자.  lsn 은 1 부터 1 씩 늘어나는 변경 번호.
 *   crc = 줄에서 마지막 공백 앞까지(즉 "<lsn> P <key> <val>" 또는 "<lsn> D <key> -")의 바이트 합 % 65536, "%04x" 로 출력. */
#ifndef KV_H
#define KV_H

typedef struct kv kv_t;

/* dir(이미 존재하는 디렉터리)에서 저장소를 연다. 복구를 한다:
 *   1) snapshot 이 있으면 읽는다 (END 줄까지 있어야 유효. 없으면 무시하고 lsn 0 에서 시작)
 *   2) wal.log 를 앞에서부터 한 줄씩 재생한다.
 *        - lsn <= 지금 lsn      → 이미 반영된 기록: 건너뛴다 (체크포인트 직후 크래시로 남은 옛 로그)
 *        - lsn == 지금 lsn + 1  → 적용
 *        - 그 밖 / crc 불일치 / 형식 오류 / 줄바꿈 없이 끝남 → 거기서 재생을 멈추고, 그 줄부터 끝까지를 버린다
 *   3) 버린 꼬리는 파일에서도 잘라낸다 (그래야 다음에 이어 쓴 기록이 깨진 줄에 붙지 않는다)
 * 실패(디렉터리 없음 등)하면 NULL */
kv_t *kv_open(const char *dir);

/* 메모리만 해제한다. 체크포인트를 하지 않는다. (시험은 이것을 "프로세스가 죽었다"로 쓴다) */
void kv_close(kv_t *k);

/* 로그에 먼저 쓰고(fflush + fsync) 나서 메모리에 반영한다. 같은 key 면 덮어쓴다.
 * 성공 0, 키/값이 규칙에 어긋나거나 쓰기에 실패하면 -1 (이때 lsn 도 메모리도 바뀌지 않는다) */
int kv_put(kv_t *k, const char *key, const char *val);

/* 삭제. 있었으면 로그에 쓰고 0, 없으면 아무것도 쓰지 않고 1, 오류 -1 */
int kv_del(kv_t *k, const char *key);

/* 값을 돌려준다 (없으면 NULL). 다음 put/del 전까지 유효 */
const char *kv_get(const kv_t *k, const char *key);

unsigned long kv_count(const kv_t *k); /* 지금 들어 있는 키 수 */
unsigned long kv_lsn(const kv_t *k);   /* 마지막으로 반영한 lsn */

/* 체크포인트: snapshot.tmp 에 전부 쓰고(END 포함) fsync → snapshot 으로 rename → 그다음에야 wal.log 를 비운다.
 * 이 순서가 바뀌면 중간에 죽을 때 데이터를 잃는다. 성공 0, 실패 -1 */
int kv_checkpoint(kv_t *k);

#endif
