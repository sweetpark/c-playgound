#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>

/*
 * - 무한 루프를 돌면서 count를 1 증가시키고, 해당 값을 1초 후에 파일로 저장하는 로직
 *   1. SIGINT는 무시, SIGTERM에만 종료
 *   2. write하기 위해 1초 대기하는 경우 count +1 만큼 증가
 *
 * */

/* 핸들러와 main 이 공유하는 플래그는 volatile sig_atomic_t */
static volatile sig_atomic_t g_stop = 0;

/* 핸들러 안에서는 async-signal-safe 함수만 (printf 금지 → write) */
static void skip_hdlr(int signo){
	const char msg[] = "skip signal\n";
	int saved = errno;          /* write 가 errno 를 바꾸므로 복원 */
	ssize_t r = 0;

	(void)signo;
	r = write(STDOUT_FILENO, msg, sizeof(msg) - 1);
	(void)r;
	errno = saved;
}

static void stop_hdlr(int signo){
	(void)signo;
	g_stop = 1;
}

/* signal() 대신 sigaction. SA_RESTART 를 쓰지 않아 nanosleep 이 시그널에 깨어난다 */
static int install_handler(int signo, void (*hdlr)(int)){
	struct sigaction sa;

	memset(&sa, 0, sizeof(sa));
	sa.sa_handler = hdlr;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	return sigaction(signo, &sa, NULL);
}

/* 1초 대기. SIGINT 처럼 무시하는 시그널로 깨어나면 남은 시간만큼 다시 잔다.
 * SIGTERM(g_stop)이면 대기를 중단하고 바로 돌아간다 (최대 1초 지연). */
static int wait_one_sec(void){
	struct timespec req = { 1, 0 };
	struct timespec rem = { 0, 0 };

	while(nanosleep(&req, &rem) != 0){
		if(errno != EINTR){
			return -1;
		}
		if(g_stop){
			return 0;
		}
		req = rem;
	}
	return 0;
}

/* 파일 맨 앞으로 되돌아가 최신 값으로 덮어쓴다. 값은 계속 커지므로 자릿수가 줄지 않아 truncate 불필요 */
static int save_count(FILE *fp, unsigned long count){
	if(fseek(fp, 0, SEEK_SET) != 0){
		return -1;
	}
	if(fprintf(fp, "%lu\n", count) < 0){
		return -1;
	}
	return fflush(fp);          /* 바로 디스크 쪽으로 내보낸다 */
}

int main(void){

	int fd = -1;
	int ret = 0;
	FILE *fp = NULL;
	unsigned long count = 0;    /* int 였다면 busy loop 에서는 금방 오버플로(UB) */

	if(install_handler(SIGINT, skip_hdlr) != 0 || install_handler(SIGTERM, stop_hdlr) != 0){
		perror("sigaction");
		return 1;
	}

	fd = open("count.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if(fd < 0){
		perror("open");
		return 1;
	}

	fp = fdopen(fd, "w");
	if(fp == NULL){
		perror("fdopen");
		close(fd);              /* fdopen 실패 시 fd 는 아직 우리 책임 */
		return 1;
	}

	while(!g_stop){
		count++;
		if(wait_one_sec() != 0){
			perror("nanosleep");
			ret = 1;
			break;
		}
		if(save_count(fp, count) != 0){     /* SIGTERM 으로 깨어난 경우에도 마지막 값을 저장 */
			perror("save_count");
			ret = 1;
			break;
		}
	}

	if(fclose(fp) != 0){        /* 쓰기 오류는 fclose 에서 드러날 수 있다 */
		perror("fclose");
		ret = 1;
	}
	return ret;
}
