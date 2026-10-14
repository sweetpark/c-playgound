#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <limits.h>
#include <time.h>
#include <unistd.h>
#include <sys/select.h>

/*
 * 시그널로 운영되어 동작하는 구성
 * main : 루프를 돌면서 1초마다 카운터를 interval 씩 증가 (초기 interval = 1)
 * 1. SIGHUP : 설정파일(12.07.cnf)를 읽어서 interval값 셋팅
 * 2. SIGUSR1 : 현재 카운터값, interval값, 시그널 종류별 처리횟수 화면 출력
 * 3. SIGUSR2 : 카운터 값 0으로 초기화
 * 4. SIGTERM : 최종 카운터값을 출력하고 프로그램 종료
 * 5. SIGINT : 무시하고 계속 진행
 *
 *
 * 주의)
 * 1. sigaction 사용, sig mask 사용
 * 2. 시그널 핸들러 volatile sig_atomic_t 타입의 flag 설정만 수행
 * 3. sleep() 중에 시그널 수신되면 1초를 다 가다리지 않고 바로 해당 명령 철 (단, 시그널이 수신되더라도 카운터는 1초에 한번만 증가)
 * 4. 설정파일이 없거나 interval이 잘못된 경우 에러메시 출력 하고 기존 interval 값 유지
 */


#define MAX_SIZE 1024
#define CONF_PATH "12-07.conf"

char g_buffer[MAX_SIZE];
volatile sig_atomic_t s_hup_f = 0;
volatile sig_atomic_t s_USR1_f = 0;
volatile sig_atomic_t s_USR2_f = 0;
volatile sig_atomic_t s_TERM_f = 0;
volatile sig_atomic_t s_INT_f = 0;

static int interval = 1;

/* 핸들러는 flag 설정만 한다 */
static void hup_signal(int signo){
	(void)signo;
	s_hup_f = 1;
}

static void usr1_signal(int signo){
	(void)signo;
	s_USR1_f = 1;
}

static void usr2_signal(int signo){
	(void)signo;
	s_USR2_f = 1;
}

static void term_signal(int signo){
	(void)signo;
	s_TERM_f = 1;
}

static void int_signal(int signo){
	(void)signo;
	s_INT_f = 1;
}

/* signal() 대신 sigaction. SA_RESTART 는 쓰지 않는다 (대기가 시그널에 깨어나야 하므로) */
static int install_handler(int signo, void (*hdlr)(int)){
	struct sigaction sa;

	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = hdlr;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	return sigaction(signo, &sa, NULL);
}

/* 설정파일에서 interval 을 읽는다. 실패하면 -1 (호출한 쪽이 기존 값을 유지) */
static int load_interval(const char *path, int *out){
	FILE *fp = NULL;
	char *end = NULL;
	long val = 0;

	fp = fopen(path, "r");
	if(fp == NULL){
		perror(path);
		return -1;
	}

	if(fgets(g_buffer, sizeof(g_buffer), fp) == NULL){
		fprintf(stderr, "%s: empty or read error\n", path);
		fclose(fp);
		return -1;
	}
	fclose(fp);

	errno = 0;
	val = strtol(g_buffer, &end, 10);
	if(end == g_buffer || (*end != '\n' && *end != '\0') || errno == ERANGE
			|| val <= 0 || val > INT_MAX){
		fprintf(stderr, "%s: invalid interval\n", path);
		return -1;
	}

	*out = (int)val;
	return 0;
}

/* next(절대 시각)까지 기다린다.
 *  반환  1 : 시각에 도달 (카운터를 올릴 때)
 *        0 : 시그널로 깨어남 (남은 시간은 그대로, 호출한 쪽이 flag 를 처리한 뒤 다시 호출)
 *       -1 : 오류
 * 시그널은 평소에 block 돼 있고, pselect 가 원래 mask(orig_mask)로 바꾸는 동안에만 도착한다.
 * "flag 확인 → 대기 시작" 사이에 오는 시그널을 놓치지 않기 위해 pselect 를 쓴다
 * (sleep 이나 unblock 후 별도 sleep 이면 그 사이에 온 시그널은 1초 뒤에야 처리된다). */
static int wait_until(const struct timespec *next, const sigset_t *orig_mask){
	struct timespec now;
	struct timespec remain;

	if(clock_gettime(CLOCK_MONOTONIC, &now) != 0){
		return -1;
	}

	remain.tv_sec = next->tv_sec - now.tv_sec;
	remain.tv_nsec = next->tv_nsec - now.tv_nsec;
	if(remain.tv_nsec < 0){
		remain.tv_sec--;
		remain.tv_nsec += 1000000000L;
	}
	if(remain.tv_sec < 0){
		return 1;                       /* 이미 지났다 */
	}

	if(pselect(0, NULL, NULL, NULL, &remain, orig_mask) == 0){
		return 1;                       /* 타임아웃 = 시각 도달 */
	}
	return (errno == EINTR) ? 0 : -1;
}


int main(void){

	int hCnt = 0;
	int u1Cnt = 0;
	int u2Cnt = 0;
	int tCnt = 0;
	int iCnt = 0;
	int new_interval = 0;
	int rc = 0;

	int current_cnt = 0;

	sigset_t block_mask;
	sigset_t orig_mask;
	struct timespec next;

	if(install_handler(SIGHUP, hup_signal) != 0 || install_handler(SIGUSR1, usr1_signal) != 0
			|| install_handler(SIGUSR2, usr2_signal) != 0 || install_handler(SIGTERM, term_signal) != 0
			|| install_handler(SIGINT, int_signal) != 0){
		perror("sigaction");
		return 1;
	}

	/* 다섯 시그널을 평소에는 block. 대기(pselect) 중에만 풀린다 */
	sigemptyset(&block_mask);
	sigaddset(&block_mask, SIGHUP);
	sigaddset(&block_mask, SIGUSR1);
	sigaddset(&block_mask, SIGUSR2);
	sigaddset(&block_mask, SIGTERM);
	sigaddset(&block_mask, SIGINT);
	if(sigprocmask(SIG_BLOCK, &block_mask, &orig_mask) != 0){
		perror("sigprocmask");
		return 1;
	}

	if(clock_gettime(CLOCK_MONOTONIC, &next) != 0){
		perror("clock_gettime");
		return 1;
	}
	next.tv_sec += 1;                   /* 첫 카운터 증가는 1초 뒤 */

	while(1){

		/* 여기는 시그널이 block 된 구간이라 flag 를 안전하게 읽고 지울 수 있다.
		 * else if 로 묶지 않아 동시에 온 시그널도 모두 처리한다 */
		if(s_hup_f){
			s_hup_f = 0;
			hCnt++;
			if(load_interval(CONF_PATH, &new_interval) == 0){
				interval = new_interval;
			}else{
				fprintf(stderr, "interval unchanged : %d\n", interval);
			}
		}
		if(s_USR1_f){
			s_USR1_f = 0;
			u1Cnt++;
			printf("current : %d, interval : %d, hup : %d, u1 : %d, u2 : %d, term : %d, SIGINT : %d\n", current_cnt, interval, hCnt, u1Cnt, u2Cnt, tCnt, iCnt);
			fflush(stdout);
		}
		if(s_USR2_f){
			s_USR2_f = 0;
			u2Cnt++;
			current_cnt = 0;
		}
		if(s_INT_f){
			s_INT_f = 0;
			iCnt++;
			printf("skip\n");
			fflush(stdout);
		}
		if(s_TERM_f){
			tCnt++;
			break;                      /* 1초를 기다리지 않고 바로 종료 */
		}

		rc = wait_until(&next, &orig_mask);
		if(rc < 0){
			perror("wait_until");
			return 1;
		}
		if(rc == 1){                    /* 시그널과 무관하게 카운터는 1초에 한 번만 증가 */
			current_cnt = current_cnt + interval;
			next.tv_sec += 1;
		}
	}

	printf("final counter : %d (term : %d)\n", current_cnt, tCnt);
	return 0;
}
