#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define EX 1
#if EX
#define MAX_SIZE 1024
void trstr(char *c_str, int size, char orig, char rep){
	int i = 0;
	char buf[MAX_SIZE];
	strncpy(buf, c_str, MAX_SIZE);
	for (i = 0; i < size; i++){
		if (buf[i] == orig){
			buf[i] = rep;
		}
	}

}
#else
void trstr(char *c_str, char orig, char rep) {
	/* 문자열 비교의 경우 strcmp 또는 한글자씩 볼경우 적절한 메서드를 이용해 추출하여 한글자씩 비교 필요 */
	while (*c_str != '\0') {
		if (*c_str == orig) {
			*c_str = rep;
		}
		++c_str;
	}
}
#endif

void func(void) {
#if EX
	int slen = 0;
#endif
	char *env = getenv("TEST_ENV");
	if (env == NULL) {
		return ;
	}
	/* env 사이즈 필요  */
#if EX
	slen = strlen(env);
	trstr(env, slen, '_', ':');
#endif
	//trstr(env,'_', ':');

	printf("TEST_ENV=%s\n", env) ;
} 

void func2(void) {
	char *env = getenv("TEST_ENV");
	if (env == NULL) {
		return ;
	}

	printf("TEST_ENV=%s\n", env) ;
} 

int main()
{
	func() ;

	func2() ;
	return 0 ;
}
