/* P104 alog — 비동기 로거의 뼈대: 한정 크기 큐 + 소비 스레드. 선언만 있다. 구현은 solution.c 에. */
#ifndef ALOG_H
#define ALOG_H

#include <stddef.h>

#define ALOG_OK     0
#define ALOG_BADARG (-1)   /* NULL 같은 잘못된 인자 */
#define ALOG_FULL   (-2)   /* 큐가 가득 차서 이 로그는 버려졌다 */

/* 소비 스레드가 로그 한 줄을 실제 목적지(파일, 소켓…)에 쓰는 함수. 느릴 수 있다. */
typedef void (*alog_sink_fn)(void *ctx, const char *line);

typedef struct alog alog_t;

/* capacity 칸짜리 큐와 소비 스레드를 만든다. 실패하면 NULL (capacity <= 0, sink == NULL, 메모리 부족 …). */
alog_t *alog_create(int capacity, alog_sink_fn sink, void *ctx);

/* 로그 한 줄을 큐에 넣는다 (문자열은 복사해 둔다).
 * 절대 기다리지 않는다: 큐가 가득 차 있으면 이 줄을 버리고 ALOG_FULL 을 돌려준다.
 * 여러 스레드가 동시에 불러도 된다. */
int alog_write(alog_t *a, const char *line);

/* "가득 참" 상태인가. 큐가 한 번 가득 차면 켜지고,
 * 빈 칸이 capacity/10 칸 이상(최소 1칸) 생겨야 꺼진다 (칸이 하나 비었다고 바로 꺼지면 켜졌다 꺼졌다를 반복한다). */
int alog_is_full(alog_t *a);

/* 통계: enq = 큐에 받아 둔 줄 수, deq = 소비 스레드가 큐에서 꺼낸 줄 수,
 *       dropped = 가득 차서 버린 줄 수, used = 지금 큐에 쌓여 있는 줄 수. 모르는 값은 NULL 로 넘겨도 된다. */
void alog_stats(alog_t *a, unsigned long *enq, unsigned long *deq, unsigned long *dropped, int *used);

/* 큐에 남은 줄을 전부 sink 로 내보낸 뒤, 소비 스레드를 join 하고, 모든 메모리를 해제한다. */
void alog_destroy(alog_t *a);

#endif
