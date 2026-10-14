#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>

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

#define MAX_SIZE 10

int main(int argc, char **argv){
	char *command = argv[1];
	char *option = argv[2];
	char *path = NULL;
	char **path_list = NULL;

	int index = 0;
	int n = 0;
	char c;


	path = getenv("PATH");

	for(index = 0; path[index] != '\0'; index++){
		if(path[index] == ':'){
			n++;
		}
	}
	path_list = malloc(sizeof(*path_list) * n);
	path_list[0] = strtok(path, ":");

	for(index = 1; index < n; index++){
		path_list[index] = strtok(NULL, ":");
		if(strlen(path_list[index]) <= 0){
			path_list[index] = ".";
		}
	}


	for(index = 0; index < n; index++){
		printf("%s\n", path_list[index]);
	}

	return 0;
}
