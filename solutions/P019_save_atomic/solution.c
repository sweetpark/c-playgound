#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

/* P019 save_atomic — 모범답안 */
int save_atomic(const char *path, const char *data)
{
	char tmp[512];
	FILE *fp;
	size_t len;
	int n;

	if (path == NULL || data == NULL)
		return -1;
	n = snprintf(tmp, sizeof(tmp), "%s.tmp", path);
	if (n < 0 || (size_t)n >= sizeof(tmp))
		return -1;

	fp = fopen(tmp, "w");
	if (fp == NULL)
		return -1;

	len = strlen(data);
	if (fwrite(data, 1, len, fp) != len || fflush(fp) != 0 || fsync(fileno(fp)) != 0) {
		fclose(fp);
		remove(tmp);
		return -1;
	}
	if (fclose(fp) != 0) {             /* fclose 의 오류도 쓰기 오류다 */
		remove(tmp);
		return -1;
	}
	if (rename(tmp, path) != 0) {      /* 같은 파일시스템의 rename 은 원자적 */
		remove(tmp);
		return -1;
	}
	return 0;
}
