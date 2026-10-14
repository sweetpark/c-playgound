#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
/*프로그램 실행하면 LOG_PATH 환경변수를 확인하여 로그파일 생성, 없는 경우 default.log 파일 생성*/

/* 사용자로부터 입력받아 해당 로그파일에 기재 */

/* 사용자가 q 또는 quit으로 종료 가능 */

/* 프로그램을 종료할때 atexit() 함수 사용하여 clean-up() 함수를 등록하여 종료 절차 수행 */

/* clean-up() 함수에서는 종료시 로그파일에 Program terminated normally 기재 후 로그파일 close */


#define MAX_SIZE 1024

/* atexit 로 등록한 clean-up 이 접근해야 하므로 전역(static) */
static FILE *g_fp  = NULL;
static char *g_buf = NULL;

static void clean_up(void){
	if(g_fp != NULL){
		fprintf(g_fp, "Program terminated normally\n");
		if(fclose(g_fp) != 0){          /* 쓰기 오류는 fclose 에서 드러날 수 있다 */
			perror("fclose");
		}
		g_fp = NULL;
	}
	free(g_buf);
	g_buf = NULL;
}


int main(void){

	const char *path = getenv("LOG_PATH");
	char message[MAX_SIZE];
	int fd = -1;

	if(atexit(clean_up) != 0){
		fprintf(stderr, "atexit failed\n");
		return -1;
	}

	g_buf = malloc(MAX_SIZE);
	if(g_buf == NULL){
		perror("malloc");
		return -1;
	}

	/* LOG_PATH 가 없거나 비어 있으면 default.log */
	if(path == NULL || path[0] == '\0' || strlen(path) > 1024){
		snprintf(g_buf, MAX_SIZE, "default.log");
	}else{
		snprintf(g_buf, MAX_SIZE, "%s.log", path);
	}

	/* 접근 모드(O_WRONLY)가 반드시 필요하다. 없으면 O_RDONLY 로 열려 fdopen("w") 가 실패한다.
	 * O_EXCL 대신 O_APPEND: 다시 실행해도 기존 로그 뒤에 이어 쓴다 */
	fd = open(g_buf, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if(fd < 0){
		perror("open");
		return -1;
	}

	g_fp = fdopen(fd, "w");
	if(g_fp == NULL){
		perror("fdopen");
		close(fd);               /* fdopen 실패 시 fd 는 아직 우리 책임 */
		return -1;
	}

	while(1){
		printf(" > : ");
		fflush(stdout);

		if(fgets(message, sizeof(message), stdin) == NULL){   /* EOF(Ctrl+D) / 오류 */
			printf("\n");
			break;
		}
		message[strcspn(message, "\r\n")] = '\0';             /* 개행 제거 */

		if(strcmp(message, "quit") == 0 || strcmp(message, "q") == 0){
			break;
		}

		fprintf(g_fp, "%s\n", message);                       /* 줄 단위로 기록 */
		fflush(g_fp);                                         /* 비정상 종료에도 남도록 */
	}

	return 0;      /* 종료 시 atexit 로 등록한 clean_up() 이 호출된다 */
}
