#include <stdio.h>

enum {MAX_BUF_SIZE = 128} ;

void func(void) {
	int i;
	ssize_t count = 0;
	char buf[MAX_BUF_SIZE] ;

	char hex_num[9] = {1, 2, 3, 4, 5, 6, 7, 8, 9} ;

	for (i = 0; i < 9; ++i) {
		// sprintf의 예외처리가 되어있지 않음 (in-band 에러 표시자를 피하자 -> 성공여부의 int를 넘기는걸로 하고, 값을 넘기는건 지양)
		count += sprintf( buf + count, "%02x ", hex_num[i]);
	}
	count += sprintf(buf + count, "\n");

	printf("data=%s", buf) ;
}

int main(void) {
	func() ;

	return 0 ;
}
