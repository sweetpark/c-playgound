#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <limits.h>
#include <unistd.h>

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

char g_buffer[MAX_SIZE];                /* char *[] (포인터 배열) 이 아니라 문자 버퍼 */
volatile sig_atomic_t s_hup_f = 0;
volatile sig_atomic_t s_USR1_f = 0;
volatile sig_atomic_t s_USR2_f = 0;
volatile sig_atomic_t s_TERM_f = 0;
volatile sig_atomic_t s_INT_f = 0;

static int interval = 1;

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

/* 설정파일에서 interval 을 읽는다. 실패하면 -1 (호출한 쪽이 기존 값을 유지) */
static int load_interval(const char *path, int *out){
	FILE *fp = NULL;
	char *end = NULL;
	long val = 0;

	fp = fopen(path, "r");                  /* 읽기 전용. 설정파일을 지우거나 덮어쓰지 않는다 */
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


int main(void){

	int hCnt = 0;
	int u1Cnt = 0;
	int u2Cnt = 0;
	int tCnt = 0;
	int iCnt = 0;
	int new_interval = 0;

	int current_cnt = 0;

	signal(SIGHUP, hup_signal);
	signal(SIGUSR1, usr1_signal);
	signal(SIGUSR2, usr2_signal);
	signal(SIGTERM, term_signal);
	signal(SIGINT, int_signal);

	while(!s_TERM_f){

		/* else if 로 묶으면 동시에 온 시그널은 한 번에 하나만 처리된다 → 각각 독립 if */
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
		}

		sleep(1);
		current_cnt = current_cnt + interval;
	}

	/* while 조건이 TERM 이면 빠져나오므로 루프 안의 else if(s_TERM_f) 는 실행될 수 없었다 */
	tCnt++;
	printf("final counter : %d (term : %d)\n", current_cnt, tCnt);

	return 0;
}
