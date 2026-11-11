#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "drill_test.h"
#include "solution.c"

static long fsize(const char *p, char *content, size_t cap)
{
	FILE *fp = fopen(p, "r");
	size_t n;
	if (!fp) return -1;
	n = fread(content, 1, cap - 1, fp);
	content[n] = '\0';
	fclose(fp);
	return (long)n;
}

int main(void)
{
	char dir[] = "/tmp/drillXXXXXX";
	char path[128], bad[160], buf[64];
	FILE *fp;

	CHECK(mkdtemp(dir) != NULL);
	snprintf(path, sizeof(path), "%s/a.txt", dir);
	snprintf(bad, sizeof(bad), "%s/nodir/b.txt", dir);

	/* 새 파일: 만들어진다 */
	CHECK(create_new(path) == 0);
	CHECK(fsize(path, buf, sizeof(buf)) == 0);

	/* 이미 있으면 1, 내용은 그대로(덮어쓰지 않는다) */
	fp = fopen(path, "w");
	if (fp) { fputs("keep", fp); fclose(fp); }
	CHECK(create_new(path) == 1);
	CHECK(fsize(path, buf, sizeof(buf)) == 4 && strcmp(buf, "keep") == 0);

	/* 그 외 오류(디렉터리 없음)는 -1 : 이미 존재(1)와 구분 */
	CHECK(create_new(bad) == -1);
	CHECK(create_new(NULL) == -1);

	unlink(path);
	rmdir(dir);
	return drill_report();
}
