#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "drill_test.h"
#include "solution.c"

static char g_dir[] = "/tmp/drillXXXXXX";

static void put(const char *name, const char *data, size_t n)
{
	char p[128];
	FILE *fp;
	snprintf(p, sizeof(p), "%s/%s", g_dir, name);
	fp = fopen(p, "wb");
	if (fp) { fwrite(data, 1, n, fp); fclose(fp); }
}

static long count(const char *name)
{
	char p[128];
	snprintf(p, sizeof(p), "%s/%s", g_dir, name);
	return count_lines(p);
}

int main(void)
{
	char longline[200];
	char many[2000];
	size_t i, m = 0;
	char p[128];

	CHECK(mkdtemp(g_dir) != NULL);

	put("a", "a\nb\nc\n", 6);
	CHECK(count("a") == 3);
	put("b", "a\nb", 3);                 /* 마지막 줄에 개행 없음 */
	CHECK(count("b") == 2);
	put("c", "", 0);
	CHECK(count("c") == 0);
	put("d", "\n\n\n", 3);
	CHECK(count("d") == 3);

	/* 줄이 읽기 버퍼(16)보다 길어도 한 줄은 한 줄 */
	memset(longline, 'x', 100);
	longline[100] = '\n';
	put("e", longline, 101);
	CHECK(count("e") == 1);
	/* 버퍼 경계: 14/15/16/17 글자 + 개행 */
	for (i = 14; i <= 17; i++) {
		char nm[8];
		memset(longline, 'y', i);
		longline[i] = '\n';
		snprintf(nm, sizeof(nm), "f%zu", i);
		put(nm, longline, i + 1);
		CHECK(count(nm) == 1);
	}
	/* 길이가 섞인 여러 줄 */
	for (i = 0; i < 40; i++) {
		size_t len = (i * 7) % 30;
		memset(many + m, 'z', len);
		m += len;
		many[m++] = '\n';
	}
	put("g", many, m);
	CHECK(count("g") == 40);
	/* 버퍼보다 긴 마지막 줄(개행 없음) */
	memset(longline, 'q', 50);
	put("h", longline, 50);
	CHECK(count("h") == 1);

	/* 열기 실패 / 읽기 오류(디렉터리) / NULL */
	CHECK(count("nofile") == -1);
	CHECK(count_lines(NULL) == -1);
	CHECK(count_lines(g_dir) == -2);     /* fopen 은 되지만 읽기가 EISDIR : EOF 와 구분 */

	for (i = 0; i < 8; i++) {
		static const char *names[] = {"a","b","c","d","e","g","h","f14"};
		snprintf(p, sizeof(p), "%s/%s", g_dir, names[i]);
		unlink(p);
	}
	for (i = 15; i <= 17; i++) {
		snprintf(p, sizeof(p), "%s/f%zu", g_dir, i);
		unlink(p);
	}
	rmdir(g_dir);
	return drill_report();
}
