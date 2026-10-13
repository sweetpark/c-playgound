#include <stdio.h>
#include <string.h>
#include <time.h>


int make_filename(char *base_name, char *filename, size_t size){
	
	int index = 0;
	char ts[16];
	time_t now = time(NULL);
	strftime(ts, sizeof(ts), "%Y%m%d%H%M", localtime(&now));
	FILE *fp = NULL;

	/* 날짜 + index 붙이기 */
	do{	
		int n = snprintf(filename, size, "%s_%s[%d]",base_name,ts, index);
		
		if( n < 0 || (size_t)n >= size){
			printf("Error overflow name\n");
			return -1;
		}

		fp = fopen(filename, "wx");
		index++;
	}while(fp == NULL);
	
	fclose(fp);
	return 1;
}

int my_strncpy(char *dst, size_t dstsize, char *src) {

	size_t i = 0;
	size_t slen = strlen(src);

	/* dst의 크기가 src의 크기(널 문자 포함)보다 작은지 확인 */
	if (dstsize < slen + 1){
		printf("Error overflow \n");
		return -1;
	}
	memset(dst, 0, dstsize);

	/* dst에 반복하여 src 삽입 */
	for( i = 0; i <= slen; i++ ) {
		dst[i] = src[i];
	}


	return 1;
}

int my_strncat(char *dst, size_t dstsize, char *src) {

	size_t i = 0;
	size_t dlen = strlen(dst);
	size_t slen = strlen(src);

	/* dst의 크기가 src를 합쳤을때 보다 작은지 확인 */
	if ( dstsize < dlen + slen + 1 ){
		printf("Error overflow\n");
		return -1;
	}

	/* dst에서 이후 로직으로 추가 */
	for(i = 0; i <= slen; i++){
		dst[i+dlen] = src[i];
	}

	return 1;
}


int main(void){
	/*
	char dst[10] = "dst";
	char src[5] = "src";

	my_strncpy(dst, 10, src);
	printf("cpy test %s \n", dst);
    
    	
	my_strncat(dst, 10, src);
	printf("cat test %s\n", dst);
	*/

	char base_name[] = "base_name";	/* 널 문자까지 포함해 10바이트 */
	// 0 ~ 9
	char file_name[26];
	int n = 1;

	n = make_filename(base_name, file_name, 26);
	if(n < 0){
		printf("Error main \n");
	}


	return 0;
}
