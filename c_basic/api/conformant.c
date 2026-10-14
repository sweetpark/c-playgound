#define _POSIX_C_SOURCE 200809L   /* strnlen */
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>

/*
 * 크기(destcount, srccount)를 배열 매개변수보다 먼저 선언하는 conformant 배열 매개변수 방식.
 *
 * [규약]
 *  - n : src 에서 복사(추가)할 최대 글자 수 (널 문자 제외). 단, src 배열 크기(srccount)를 넘어 읽지 않는다.
 *        실제 복사 길이 = strnlen(src, min(n, srccount))
 *  - 항상 널 종료한다. dest 에 (복사 길이 + 널 문자) 가 들어가지 않으면 실패.
 *  - 실패하면 NULL 을 돌려주고 errno 를 설정한다. dest 는 변경하지 않는다.
 *      EINVAL : NULL 인자, dest 크기 0, dest 와 src 가 겹침, (strncat) dest 가 널 종료되지 않음
 *      ERANGE : dest 공간 부족
 *  - 함수 안에서 메시지를 출력하지 않는다.
 */

/* 두 영역이 겹치는가 (memcpy 는 겹치면 미정의 동작) */
static int overlap(const void *a, size_t alen, const void *b, size_t blen){
	uintptr_t pa = (uintptr_t)a;
	uintptr_t pb = (uintptr_t)b;

	return pa < pb + blen && pb < pa + alen;
}

/* src 에서 복사할 길이: 널 문자 앞까지, n 과 srccount 중 작은 값을 넘지 않는다 */
static size_t copy_len(size_t srccount, const char src[srccount], size_t n){
	size_t limit = (n < srccount) ? n : srccount;

	return strnlen(src, limit);
}

char* improved_strncpy(size_t destcount, char dest[destcount], size_t srccount, const char src[srccount], size_t n) {

	size_t len = 0;

	if(dest == NULL || src == NULL || destcount == 0){
		errno = EINVAL;
		return NULL;
	}

	len = copy_len(srccount, src, n);

	if(len >= destcount){                       /* 널 문자 자리(+1)까지 들어가야 한다 */
		errno = ERANGE;
		return NULL;
	}

	if(overlap(dest, len + 1, src, len)){
		errno = EINVAL;
		return NULL;
	}

	memcpy(dest, src, len);
	dest[len] = '\0';                           /* strncpy 와 달리 항상 널 종료 */
	return dest;
}


char* improved_strncat(size_t destcount, char dest[destcount],  size_t srccount, const char src[srccount], size_t n){

	size_t dlen = 0;
	size_t len = 0;

	if(dest == NULL || src == NULL || destcount == 0){
		errno = EINVAL;
		return NULL;
	}

	/* strlen 대신 destcount 안에서만 널 문자를 찾는다 (널 종료가 안 된 dest 를 넘어서 읽지 않도록) */
	if(memchr(dest, '\0', destcount) == NULL){
		errno = EINVAL;
		return NULL;
	}
	dlen = strlen(dest);                        /* 여기서는 dlen < destcount 가 보장된다 */

	len = copy_len(srccount, src, n);

	if(len >= destcount - dlen){                /* 널 문자 자리(+1) 포함. 뺄셈은 dlen < destcount 라 안전 */
		errno = ERANGE;
		return NULL;
	}

	if(overlap(dest + dlen, len + 1, src, len)){
		errno = EINVAL;
		return NULL;
	}

	memcpy(dest + dlen, src, len);
	dest[dlen + len] = '\0';
	return dest;
}
