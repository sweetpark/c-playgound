#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "drill_test.h"
#include "solution.c"

static void put(const char *p, const char *s)
{
	FILE *fp = fopen(p, "w");
	if (fp) { fputs(s, fp); fclose(fp); }
}

static int get(const char *p, char *buf, size_t cap)
{
	FILE *fp = fopen(p, "r");
	size_t n;
	if (!fp) return -1;
	n = fread(buf, 1, cap - 1, fp);
	buf[n] = '\0';
	fclose(fp);
	return (int)n;
}

static int exists(const char *p)
{
	return access(p, F_OK) == 0;
}

int main(void)
{
	char dir[] = "/tmp/drillXXXXXX";
	char path[128], tmp[140], sub[140], inner[160], bad[160], buf[4096];
	char *big;

	CHECK(mkdtemp(dir) != NULL);
	snprintf(path, sizeof(path), "%s/conf", dir);
	snprintf(tmp, sizeof(tmp), "%s/conf.tmp", dir);

	/* 기존 파일 교체 */
	put(path, "old");
	CHECK(save_atomic(path, "new content\n") == 0);
	CHECK(get(path, buf, sizeof(buf)) == 12 && strcmp(buf, "new content\n") == 0);
	CHECK(!exists(tmp));                 /* 임시 파일이 남지 않는다 */

	/* 처음 저장 / 빈 데이터 */
	unlink(path);
	CHECK(save_atomic(path, "first") == 0);
	CHECK(get(path, buf, sizeof(buf)) == 5);
	CHECK(save_atomic(path, "") == 0);
	CHECK(get(path, buf, sizeof(buf)) == 0);

	/* 큰 데이터 */
	big = malloc(1 << 20);
	CHECK(big != NULL);
	if (big) {
		memset(big, 'k', (1 << 20) - 1);
		big[(1 << 20) - 1] = '\0';
		CHECK(save_atomic(path, big) == 0);
		{
			struct stat st;
			CHECK(stat(path, &st) == 0 && st.st_size == (1 << 20) - 1);
		}
		free(big);
	}

	/* 실패하면 원본은 그대로, 임시 파일은 정리 : rename 대상이 비어 있지 않은 디렉터리 */
	snprintf(sub, sizeof(sub), "%s/sub", dir);
	snprintf(inner, sizeof(inner), "%s/sub/inner", dir);
	mkdir(sub, 0700);
	put(inner, "x");
	CHECK(save_atomic(sub, "data") == -1);
	CHECK(exists(inner));
	snprintf(bad, sizeof(bad), "%s/sub.tmp", dir);
	CHECK(!exists(bad));

	/* 없는 디렉터리, NULL */
	snprintf(bad, sizeof(bad), "%s/nodir/conf", dir);
	CHECK(save_atomic(bad, "x") == -1);
	CHECK(save_atomic(NULL, "x") == -1);
	CHECK(save_atomic(path, NULL) == -1);

	unlink(inner);
	rmdir(sub);
	unlink(path);
	rmdir(dir);
	return drill_report();
}
