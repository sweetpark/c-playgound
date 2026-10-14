/*
 * snprintf_m() 가변 인자함수를 구현.
 * snprintf()함수의 inband 에러 지시자를 제거한 함수로, 함수의 리턴 값은 에러 값, 인자로 (buf, buf의 길이, 버퍼 쓴 count, 포멧 문자열, 가변길이 인자 (포멧 문자와 연관된 변수))를 받는 함수로 다음과 같다
 *
 * errno_t snprintf_m ( char *buf, size_t buf_len, size_t *count, const char *fmt, ...)
 *
 *  - 반환값 : 0 성공 / EINVAL 인자 오류 / ERANGE 버퍼가 모자라 잘림 / EIO 포맷 변환 오류
 *  - *count : 버퍼에 실제로 쓴 글자 수 (널 문자 제외). 값을 돌려줘야 하므로 포인터로 받는다
 * */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <stdarg.h>

/* va_list 사용 순서 (참고)
 *   va_list ap;
 *   va_start(ap, 마지막_고정_매개변수);   // 이 코드에서는 fmt
 *   ... va_arg(ap, 타입) 또는 vsnprintf(..., ap) ...
 *   va_end(ap);                          // 모든 경로에서 반드시 호출
 */

typedef int errno_t;      /* 성공 0, 실패는 errno 값 (C11 Annex K 의 errno_t 와 같은 규약) */

/* fmt 가 4번째, 가변 인자는 5번째부터 — 호출하는 쪽의 포맷/인자 불일치를 컴파일러가 검사 */
errno_t snprintf_m(char *buf, size_t buf_len, size_t *count, const char *fmt, ...)
	__attribute__((format(printf, 4, 5)));

errno_t snprintf_m(char *buf, size_t buf_len, size_t *count, const char *fmt, ...){

	va_list param;
	int ret_val = 0;

	if(buf == NULL || buf_len == 0 || count == NULL || fmt == NULL){
		return EINVAL;
	}

	*count = 0;
	buf[0] = '\0';                          /* 실패해도 buf 는 항상 유효한 빈 문자열 */

	va_start(param, fmt);                   /* 마지막 고정 매개변수는 fmt */
	ret_val = vsnprintf(buf, buf_len, fmt, param);   /* va_list 는 snprintf 가 아니라 vsnprintf */
	va_end(param);

	if(ret_val < 0){                        /* 인코딩 등 포맷 변환 오류 */
		buf[0] = '\0';
		return EIO;
	}

	if((size_t)ret_val >= buf_len){         /* 쓰려던 길이가 버퍼 이상 = 잘림 */
		*count = buf_len - 1;               /* 실제로 쓴 글자 수 (널 문자 자리 제외) */
		return ERANGE;
	}

	*count = (size_t)ret_val;
	return 0;

}
