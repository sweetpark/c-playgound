#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>

/*
 * PATH 환경변수에서 명령어 위치 찾아주는 프로그램 (which와 유사)
 * 1. 프로그램 실행 인자로 명령어 이름 받기
 * 2. PATH에 설정된 디렉토리를 앞에서부터 순서대로 검색
 * 3. 해당 이름의 실행 가능한 파일이 있는 첫 번째 디렉토리를 찾아서 전체 경로 출력( access(path, X_OK) 로 실행 가능여부 확인 )
 * 4. -a 옵션이 주어지면 첫번쨰 경로만이 아니라 일치하는 모든 경로 출력
 * 5. 종료 코드 : 명령어를 찾으면 (0), 찾지 못하면 (1), 인자 오류 또는 PATH가 설정되지 않으면 (2)
 *
 * 주의사항)
 * 1. getenv() 반환 문자열 수정 금지
 * 2. PATH 길이, 각 디렉토리 경로 길이, 명령어 이름 길이 가정 x (동적할당)
 * 3. PATH에 빈 항목이 있는 경우 현재디렉토리로 가정
 * 4. 동적으로 할당한 메모리 atexit()로 등록
 * 5. 검색한 디렉토리 개수를 stderr로 출력하는 exit handler 등록
 * 6. 프로그램이 어떤 경로로 종료되더라도 등록한 exit handler 수행
 *
 */

#define RC_FOUND     0
#define RC_NOT_FOUND 1
#define RC_ERROR     2

/* exit handler 가 접근해야 하므로 전역(static) */
static char  *g_path_copy = NULL;   /* getenv 결과를 복사한 것 (원본은 수정 금지) */
static char **g_dirs      = NULL;   /* g_path_copy 안을 가리키는 디렉토리 포인터 배열 */
static size_t g_searched  = 0;      /* 실제로 검색한 디렉토리 개수 */

/* atexit 는 나중에 등록한 것부터 실행된다 (LIFO) */
static void free_all(void){
	free(g_dirs);
	free(g_path_copy);
	g_dirs = NULL;
	g_path_copy = NULL;
}

static void report_searched(void){
	fprintf(stderr, "searched directories: %zu\n", g_searched);
}

/* PATH 복사본을 ':' 기준으로 쪼갠다. 빈 항목은 "." (현재 디렉토리).
 * strtok 은 연속된 ':' 를 건너뛰므로 빈 항목을 놓친다 → 직접 쪼갠다. */
static int split_path(const char *path, size_t *count){
	size_t n = 1;
	size_t i = 0;
	const char *q = NULL;
	char *p = NULL;
	char *next = NULL;

	for(q = path; *q != '\0'; q++){      /* 원본(getenv)은 읽기만 한다 */
		if(*q == ':'){
			n++;
		}
	}

	g_path_copy = strdup(path);
	g_dirs = malloc(sizeof(*g_dirs) * n);
	if(g_path_copy == NULL || g_dirs == NULL){
		perror("malloc");
		return -1;
	}

	p = g_path_copy;
	for(i = 0; i < n; i++){
		next = strchr(p, ':');
		if(next != NULL){
			*next = '\0';                     /* 복사본이라 수정해도 된다 */
		}
		g_dirs[i] = (*p == '\0') ? "." : p;
		p = (next != NULL) ? next + 1 : p;
	}

	*count = n;
	return 0;
}

/* 디렉토리가 아닌 일반 파일이면서 실행 가능한가 */
static int is_executable(const char *full){
	struct stat st;

	if(access(full, X_OK) != 0){
		return 0;
	}
	if(stat(full, &st) != 0 || !S_ISREG(st.st_mode)){
		return 0;
	}
	return 1;
}

int main(int argc, char **argv){
	const char *command = NULL;
	const char *path = NULL;
	char *full = NULL;
	size_t ndirs = 0;
	size_t i = 0;
	size_t len = 0;
	int all = 0;
	int found = 0;
	int rc = RC_NOT_FOUND;
	int a = 0;

	/* 어떤 경로로 종료돼도 실행되도록 가장 먼저 등록 (실행은 역순: report → free) */
	if(atexit(free_all) != 0 || atexit(report_searched) != 0){
		fprintf(stderr, "atexit failed\n");
		return RC_ERROR;
	}

	/* 인자 파싱: "cmd", "-a cmd", "cmd -a" 모두 허용 */
	for(a = 1; a < argc; a++){
		if(strcmp(argv[a], "-a") == 0){
			all = 1;
		}else if(argv[a][0] == '-' || command != NULL){
			fprintf(stderr, "usage: %s [-a] command\n", argv[0]);
			return RC_ERROR;
		}else{
			command = argv[a];
		}
	}
	if(command == NULL || command[0] == '\0'){
		fprintf(stderr, "usage: %s [-a] command\n", argv[0]);
		return RC_ERROR;
	}

	path = getenv("PATH");
	if(path == NULL){
		fprintf(stderr, "PATH is not set\n");
		return RC_ERROR;
	}

	if(split_path(path, &ndirs) != 0){
		return RC_ERROR;
	}

	for(i = 0; i < ndirs; i++){
		g_searched++;

		len = strlen(g_dirs[i]) + 1 + strlen(command) + 1;   /* dir + '/' + cmd + '\0' */
		full = malloc(len);
		if(full == NULL){
			perror("malloc");
			return RC_ERROR;
		}
		snprintf(full, len, "%s/%s", g_dirs[i], command);

		if(is_executable(full)){
			printf("%s\n", full);
			found = 1;
		}
		free(full);
		full = NULL;

		if(found && !all){
			break;
		}
	}

	rc = found ? RC_FOUND : RC_NOT_FOUND;
	return rc;     /* main 에서 return 해도 exit() 가 호출되어 handler 가 실행된다 */
}
