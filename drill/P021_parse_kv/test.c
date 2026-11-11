#include <stdlib.h>
#include <string.h>

#include "drill_test.h"
#include "solution.c"

#define PAD 0x7E

/* line 은 수정되면 안 된다: 쓰기 가능한 복사본을 넘기고 비교. key/val 은 정확한 크기의 힙 버퍼 */
static int run(const char *line, size_t ks, size_t vs, const char *wk, const char *wv)
{
	char w[256];
	char *k = malloc(ks);
	char *v = malloc(vs);
	int r, ok;
	strcpy(w, line);
	memset(k, PAD, ks);
	memset(v, PAD, vs);
	r = parse_kv(w, k, ks, v, vs);
	ok = (strcmp(w, line) == 0);                       /* 입력 불변 */
	if (wk) {
		ok = ok && memchr(k, 0, ks) && strcmp(k, wk) == 0;
		ok = ok && memchr(v, 0, vs) && strcmp(v, wv) == 0;
	}
	free(k);
	free(v);
	return ok ? r : -100;
}

int main(void)
{
	char k[16], v[32];

	CHECK(run("  log_level = debug  ", 16, 32, "log_level", "debug") == 0);
	CHECK(run("a=b", 16, 32, "a", "b") == 0);
	CHECK(run("a=b=c", 16, 32, "a", "b=c") == 0);          /* 첫 = 로만 분리 */
	CHECK(run("key=", 16, 32, "key", "") == 0);             /* 빈 값 */
	CHECK(run("\tk\t=\tv v\t", 16, 32, "k", "v v") == 0);   /* 탭, 값 안의 공백 유지 */
	CHECK(run("k = a = b ", 16, 32, "k", "a = b") == 0);

	/* 에러 코드, key/val 은 "" */
	CHECK(run("novalue", 16, 32, "", "") == -2);
	CHECK(run("=v", 16, 32, "", "") == -3);
	CHECK(run("   = v", 16, 32, "", "") == -3);
	CHECK(run("", 16, 32, "", "") == -2);

	/* 경계: key 버퍼 16 → 15 글자 OK, 16 글자 -4 */
	CHECK(run("123456789012345=v", 16, 32, "123456789012345", "v") == 0);
	CHECK(run("1234567890123456=v", 16, 32, "", "") == -4);
	/* val 버퍼 32 → 31 OK, 32 -5 */
	CHECK(run("k=1234567890123456789012345678901", 16, 32, "k", "1234567890123456789012345678901") == 0);
	CHECK(run("k=12345678901234567890123456789012", 16, 32, "", "") == -5);
	/* 공백은 제거한 뒤의 길이로 판단 */
	CHECK(run("   123456789012345   =  v  ", 16, 32, "123456789012345", "v") == 0);

	/* 인자 오류: 버퍼를 건드리지 않는다 */
	memset(k, PAD, sizeof(k));
	memset(v, PAD, sizeof(v));
	CHECK(parse_kv(NULL, k, sizeof(k), v, sizeof(v)) == -1);
	CHECK(parse_kv("a=b", NULL, 16, v, sizeof(v)) == -1);
	CHECK(parse_kv("a=b", k, 0, v, sizeof(v)) == -1);
	CHECK(parse_kv("a=b", k, sizeof(k), NULL, 32) == -1);
	CHECK(parse_kv("a=b", k, sizeof(k), v, 0) == -1);
	CHECK((unsigned char)k[0] == PAD && (unsigned char)v[0] == PAD);

	/* 리터럴(읽기 전용)을 그대로 넘겨도 안전해야 한다 */
	CHECK(parse_kv("  x = y ", k, sizeof(k), v, sizeof(v)) == 0);
	CHECK(strcmp(k, "x") == 0 && strcmp(v, "y") == 0);

	return drill_report();
}
