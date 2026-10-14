#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>

typedef enum {
	OK=1,
	OVERFLOW=-1,
	UNDERFLOW=-2,
	INVALID_CHAR=-3,
	INVALID_PARAM=-4,
} ret_t;

int my_atoi(const char *str, int *out){
	char *end = NULL;
	long val = 0;

	/* 포인터는 사용하기 전에 먼저 검사한다 (*out 에 쓰기 전에) */
	if(str == NULL || out == NULL){
		return INVALID_PARAM;
	}

	errno = 0;
	val = strtol(str, &end, 10);        /* end 는 &end (끝 위치를 돌려받으려면 포인터의 주소) */

	if(end == str || *end != '\0'){     /* 숫자가 하나도 없거나("", "abc"), 뒤에 문자가 남음("12ab") */
		return INVALID_CHAR;
	}

	/* long 범위를 벗어난 경우 (strtol 이 LONG_MAX / LONG_MIN 으로 고정하고 ERANGE) */
	if(errno == ERANGE){
		return (val == LONG_MAX) ? OVERFLOW : UNDERFLOW;
	}

	/* long 이 int 보다 넓은 환경(64비트)에서는 int 범위도 따로 검사 */
	if(val > INT_MAX){
		return OVERFLOW;
	}
	if(val < INT_MIN){
		return UNDERFLOW;
	}

	*out = (int)val;                    /* 성공했을 때만 out 을 갱신한다 */
	return OK;
}
