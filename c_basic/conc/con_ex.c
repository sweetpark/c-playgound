/*
 *[요구사항]
 1. 계좌 10개를 만들고, 각 계좌의 잔액을 1000으로 초기화. 각 계좌는 해당 계좌를 보호하는 mutex를 가짐
 2. 이체 스레드 4개 - 출금 계좌, 입금 계좌, 금액을 임의로 선택하여 이체를 반복 수행. 출금 계좌의 잔액이 부족하면 이체하지 않고 실패 횟수만 증가
 3. 감시 스레드 1개 - 1초마다 전체 계좌의 잔액 합계를 구해서 초기 합계(10000)와 같은지 확인하고 결과 출력
 4. main 스레드 1개 - 10초 후 종료 flag를 설정하고, 모든 스레드가 종료되면 스레드별 이체 성공/실패 횟수와 최종 잔액 합계를 출력
 *
 */

/*
 * [설계 요약]
 *
 * 1) 락 단위
 *    - 계좌마다 mutex 1개 (계좌 하나의 balance 만 보호)
 *    - 이체는 "출금 계좌"와 "입금 계좌" 두 개를 동시에 잡아야 한다.
 *      (잔액 확인 -> 출금 -> 입금 이 한 덩어리여야 중간 상태가 외부에 보이지 않음)
 *
 * 2) 데드락 방지: 락 획득 순서 고정
 *    - 스레드 A 가 (0 -> 1), 스레드 B 가 (1 -> 0) 순서로 잡으면 서로 상대 락을 기다리며 영원히 멈춘다.
 *    - 그래서 이체 방향과 상관없이 "항상 계좌 번호가 작은 쪽을 먼저" 잡는다.
 *    - 감시 스레드도 0번부터 9번까지 같은 순서(오름차순)로 잡으므로 이체 스레드와 데드락이 나지 않는다.
 *
 * 3) 감시 스레드는 전체 계좌 락을 모두 잡고 합계를 구한다.
 *    - 락 없이 한 계좌씩 읽으면, 이체 도중의 돈(출금은 됐고 입금은 아직 안 된 상태)이 사라지거나
 *      두 번 세어져서 합계가 10000 이 아닌 것처럼 보인다 (거짓 불일치).
 *
 * 4) 성공/실패 횟수는 스레드별 구조체에 따로 저장한다.
 *    - 각 스레드가 자기 구조체만 쓰므로 락이 필요 없고, main 은 join 이후에 읽으므로 안전하다.
 *
 * 5) 종료 flag 는 __atomic 연산으로 읽고 쓴다.
 *    - 일반 int 를 while 조건에서 읽으면 컴파일러가 최적화로 값을 레지스터에 캐싱해서
 *      영원히 루프를 못 빠져나올 수 있다.
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <unistd.h>

#define ACCOUNT_CNT			10
#define TRANSFER_THR_CNT	4
#define INIT_BALANCE		1000
#define INIT_TOTAL_BALANCE	(ACCOUNT_CNT * INIT_BALANCE)	/* 10000 */
#define MAX_TRANSFER_AMOUNT	500		/* 이체 금액은 1 ~ 500 사이 임의 값 */
#define MONITOR_PERIOD_SEC	1		/* 감시 주기(초) */
#ifndef RUN_SEC
#define RUN_SEC				10		/* main 이 종료 flag 를 세우기까지의 시간(초), 테스트 시 -DRUN_SEC=2 로 변경 */
#endif

/* 계좌: 잔액 + 그 잔액을 보호하는 mutex */
typedef struct _account {
	int balance;
	pthread_mutex_t lock;
} Account;

/* 이체 스레드별 통계 (각 스레드가 자기 것만 갱신 -> 락 불필요) */
typedef struct _transfer_stat {
	int id;					/* 스레드 번호 */
	unsigned int seed;		/* rand_r 용 스레드별 seed (rand() 는 전역 상태라 스레드 안전하지 않음) */
	unsigned long success;	/* 이체 성공 횟수 */
	unsigned long fail;		/* 잔액 부족으로 이체하지 않은 횟수 */
} transfer_stat_t;

/* 감시 스레드 통계 */
typedef struct _monitor_stat {
	unsigned long checks;	/* 합계 검사 횟수 */
	unsigned long mismatch;	/* 합계가 10000 이 아니었던 횟수 */
} monitor_stat_t;

static Account g_accounts[ACCOUNT_CNT];
static int g_stop_flag = 0;		/* 1 이면 모든 스레드는 루프를 종료 (반드시 아래 함수로만 접근) */

static int is_stopped(void)
{
	return __atomic_load_n(&g_stop_flag, __ATOMIC_ACQUIRE);
}

static void set_stop(void)
{
	__atomic_store_n(&g_stop_flag, 1, __ATOMIC_RELEASE);
}

/*
 * 한 번의 이체를 수행한다.
 *  - 출금/입금 계좌와 금액을 임의로 고른다 (출금 != 입금)
 *  - 두 계좌의 락을 "번호가 작은 순서"로 잡는다 (데드락 방지)
 *  - 잔액이 부족하면 이체하지 않고 fail 만 증가, 충분하면 이체하고 success 증가
 * 반환: 0 정상, -1 락 획득 실패(비정상)
 */
static int transfer_once(transfer_stat_t *st)
{
	int from, to, amount;
	int first, second;		/* 락을 잡는 순서: first(작은 번호) -> second(큰 번호) */

	from = rand_r(&st->seed) % ACCOUNT_CNT;
	do {
		to = rand_r(&st->seed) % ACCOUNT_CNT;
	} while (to == from);	/* 자기 자신에게 이체하면 같은 락을 두 번 잡아 데드락 */
	amount = rand_r(&st->seed) % MAX_TRANSFER_AMOUNT + 1;

	first  = (from < to) ? from : to;
	second = (from < to) ? to : from;

	if (pthread_mutex_lock(&g_accounts[first].lock) != 0) {
		return -1;
	}
	if (pthread_mutex_lock(&g_accounts[second].lock) != 0) {
		pthread_mutex_unlock(&g_accounts[first].lock);
		return -1;
	}

	/* ---- 임계구역: 확인 -> 출금 -> 입금이 끊기지 않아야 함 ---- */
	if (g_accounts[from].balance < amount) {
		st->fail++;
	} else {
		g_accounts[from].balance -= amount;
		g_accounts[to].balance   += amount;
		st->success++;
	}
	/* ------------------------------------------------------- */

	/* 해제는 잡은 역순 */
	pthread_mutex_unlock(&g_accounts[second].lock);
	pthread_mutex_unlock(&g_accounts[first].lock);

	return 0;
}

/* 이체 스레드: 종료 flag 가 서기 전까지 이체를 계속 반복 */
static void *func_transfer(void *arg)
{
	transfer_stat_t *st = (transfer_stat_t *)arg;

	while (!is_stopped()) {
		if (transfer_once(st) < 0) {
			fprintf(stderr, "[transfer %d] mutex lock fail\n", st->id);
			set_stop();		/* 락이 깨진 상태로 계속 돌 수 없으므로 전체 종료 */
			break;
		}
	}

	return NULL;
}

/*
 * 전체 계좌의 잔액 합계를 "일관된 스냅샷"으로 구한다.
 *  - 모든 계좌 락을 0번 -> 9번 순서로 잡고(이체 스레드와 같은 순서 -> 데드락 없음),
 *    합계를 구한 뒤 역순으로 해제한다.
 *  - 출력은 락 밖에서 하도록 합계만 돌려준다 (락을 쥔 채 I/O 하지 않기 위해).
 * 반환: 0 정상, -1 락 실패
 */
static int snapshot_total(int *total)
{
	int i, sum = 0;

	for (i = 0; i < ACCOUNT_CNT; i++) {
		if (pthread_mutex_lock(&g_accounts[i].lock) != 0) {
			while (--i >= 0) {		/* 이미 잡은 락 되돌리기 */
				pthread_mutex_unlock(&g_accounts[i].lock);
			}
			return -1;
		}
	}

	for (i = 0; i < ACCOUNT_CNT; i++) {
		sum += g_accounts[i].balance;
	}

	for (i = ACCOUNT_CNT - 1; i >= 0; i--) {
		pthread_mutex_unlock(&g_accounts[i].lock);
	}

	*total = sum;
	return 0;
}

/*
 * 종료 flag 를 확인하면서 sec 초 대기.
 * sleep(1) 한 번이면 flag 가 서도 최대 1초 뒤에야 종료하므로 0.1초씩 쪼개서 확인한다.
 */
static void wait_with_stop_check(int sec)
{
	int i;

	for (i = 0; i < sec * 10 && !is_stopped(); i++) {
		usleep(100 * 1000);
	}
}

/* 감시 스레드: 1초마다 전체 합계를 구해서 초기 합계와 비교하고 결과 출력 */
static void *func_monitor(void *arg)
{
	monitor_stat_t *st = (monitor_stat_t *)arg;
	int total;
	int sec = 0;

	while (!is_stopped()) {
		if (snapshot_total(&total) < 0) {
			fprintf(stderr, "[monitor] mutex lock fail\n");
			set_stop();
			break;
		}

		st->checks++;
		if (total == INIT_TOTAL_BALANCE) {
			printf("[monitor %2ds] total=%d == init(%d) : OK\n", sec, total, INIT_TOTAL_BALANCE);
		} else {
			st->mismatch++;
			printf("[monitor %2ds] total=%d != init(%d) : MISMATCH\n", sec, total, INIT_TOTAL_BALANCE);
		}

		wait_with_stop_check(MONITOR_PERIOD_SEC);
		sec += MONITOR_PERIOD_SEC;
	}

	return NULL;
}

int main(void)
{
	pthread_t transfer_thr[TRANSFER_THR_CNT];
	pthread_t monitor_thr;
	transfer_stat_t tstat[TRANSFER_THR_CNT];
	monitor_stat_t mstat = { 0, 0 };
	int transfer_created = 0;		/* 실제로 만들어진 이체 스레드 수 (join 대상) */
	int monitor_created = 0;
	int i, ret;
	int init_ok = 0;				/* 락 초기화가 끝난 계좌 수 */
	int final_total = 0;
	unsigned long total_success = 0, total_fail = 0;
	int rc = 0;						/* 프로그램 종료 코드 */

	/* 1. 계좌 10개 초기화: 잔액 1000 + 계좌별 mutex */
	for (i = 0; i < ACCOUNT_CNT; i++) {
		g_accounts[i].balance = INIT_BALANCE;
		if ((ret = pthread_mutex_init(&g_accounts[i].lock, NULL)) != 0) {
			printf("pthread_mutex_init fail(%d), account=%d\n", ret, i);
			rc = 1;
			goto cleanup;
		}
		init_ok++;
	}

	printf("start: accounts=%d, init total=%d, run=%dsec\n", ACCOUNT_CNT, INIT_TOTAL_BALANCE, RUN_SEC);

	/* 2. 이체 스레드 4개 생성 (스레드별 통계 구조체와 seed 를 넘김) */
	for (i = 0; i < TRANSFER_THR_CNT; i++) {
		tstat[i].id = i;
		tstat[i].seed = (unsigned int)time(NULL) ^ (unsigned int)(i * 2654435761u);
		tstat[i].success = 0;
		tstat[i].fail = 0;

		if ((ret = pthread_create(&transfer_thr[i], NULL, func_transfer, &tstat[i])) != 0) {
			printf("pthread_create(transfer %d) fail(%d)\n", i, ret);
			rc = 1;
			set_stop();		/* 이미 만든 스레드를 멈추고 정리하러 간다 */
			goto join_threads;
		}
		transfer_created++;
	}

	/* 3. 감시 스레드 1개 생성 */
	if ((ret = pthread_create(&monitor_thr, NULL, func_monitor, &mstat)) != 0) {
		printf("pthread_create(monitor) fail(%d)\n", ret);
		rc = 1;
		set_stop();
		goto join_threads;
	}
	monitor_created = 1;

	/* 4. RUN_SEC 초 동안 동작시킨 뒤 종료 flag 설정 */
	sleep(RUN_SEC);
	set_stop();

join_threads:
	/* 생성에 성공한 스레드만 join (join 하지 않으면 통계를 안전하게 읽을 수 없음) */
	for (i = 0; i < transfer_created; i++) {
		if ((ret = pthread_join(transfer_thr[i], NULL)) != 0) {
			printf("pthread_join(transfer %d) fail(%d)\n", i, ret);
			rc = 1;
		}
	}
	if (monitor_created && (ret = pthread_join(monitor_thr, NULL)) != 0) {
		printf("pthread_join(monitor) fail(%d)\n", ret);
		rc = 1;
	}

	/* 5. 결과 출력: 모든 스레드가 끝났으므로 락 없이 읽어도 안전 (join 이 메모리 가시성을 보장) */
	printf("\n==== result ====\n");
	for (i = 0; i < transfer_created; i++) {
		printf("transfer thread %d : success=%lu, fail=%lu\n", i, tstat[i].success, tstat[i].fail);
		total_success += tstat[i].success;
		total_fail    += tstat[i].fail;
	}
	printf("transfer total     : success=%lu, fail=%lu\n", total_success, total_fail);
	printf("monitor            : checks=%lu, mismatch=%lu\n", mstat.checks, mstat.mismatch);

	for (i = 0; i < init_ok; i++) {
		final_total += g_accounts[i].balance;
	}
	printf("final total balance: %d (init %d) : %s\n", final_total, INIT_TOTAL_BALANCE,
		(final_total == INIT_TOTAL_BALANCE) ? "OK" : "MISMATCH");

	if (final_total != INIT_TOTAL_BALANCE || mstat.mismatch != 0) {
		rc = 1;
	}

cleanup:
	/* 6. 자원 정리: 초기화에 성공한 mutex 만 destroy */
	for (i = 0; i < init_ok; i++) {
		pthread_mutex_destroy(&g_accounts[i].lock);
	}

	return rc;
}
