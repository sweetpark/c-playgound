#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>

/* 
 * - 무한 루프를 돌면서 count를 1 증가시키고, 해당 값을 1초 후에 파일로 저장하는 로직
 *   1. SIGINT는 무시, SIGTERM에만 종료
 *   2. write하기 위해 1초 대기하는 경우 count +1 만큼 증가
 * 
 * */

static void skip_hdlr(int signo){
	printf("skip signal \n");
}

volatile int g_stop_sig = 1;

static void stop_hdlr(int signo){
	g_stop_sig = 0;
}


int main(void){
	
	int fd = 0;
	FILE *fp = NULL;

	signal(SIGINT, skip_hdlr);
	signal(SIGTERM, stop_hdlr);

	fd = open("count.txt", O_WRONLY|O_CREAT|O_TRUNC, 0644);
	if(fd < 0){
		printf("open fail\n");
		return -1;
	}

	fp = fdopen(fd, "w");
	if(fp == NULL){
		printf("fdopen failed\n");
		return -1;
	}
	

	int count = 0;
	while(g_stop_sig){
		count++;
	}

	fprintf(fp, "%d", count);

	
	fclose(fp);
	return 0;

}
