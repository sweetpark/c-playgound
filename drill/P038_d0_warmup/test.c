#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"
#include "log.h"
#include "drill_test.h"

log_level_t g_log_level = LOG_LV_ERR;   /* 가드가 남기는 에러 로그가 시험 출력을 덮지 않게 */

#include "solution.c"

/* 열려 있는 파일 디스크립터 개수 (Linux) */
static int open_fds(void)
{
	DIR *d = opendir("/proc/self/fd");
	struct dirent *e;
	int n = 0;
	if (!d) return -1;
	while ((e = readdir(d)) != NULL)
		if (e->d_name[0] != '.') n++;
	closedir(d);
	return n;
}

int main(void)
{
	char path[] = "/tmp/drillXXXXXX";
	int fd = mkstemp(path);
	int before, after, i;

	/* take_ptr: 가드 두 개 */
	CHECK(take_ptr(NULL, 10) == RET_INVALID_ARG);
	CHECK(take_ptr("hi", 999) == RET_INVALID_ARG);
	CHECK(take_ptr("hi", -1) == RET_INVALID_ARG);
	CHECK(take_ptr("hi", 101) == RET_INVALID_ARG);
	CHECK(take_ptr("hi", 0) == RET_OK);          /* 경계 */
	CHECK(take_ptr("hi", 100) == RET_OK);
	CHECK(take_ptr("hi", 50) == RET_OK);

	/* take_res: 자원을 잡은 뒤에는 goto CLEANUP 으로 */
	CHECK(take_res(NULL) == RET_INVALID_ARG);
	CHECK(fd >= 0);
	if (fd >= 0) close(fd);
	CHECK(take_res(path) == RET_OK);
	CHECK(take_res("/no/such/file") == RET_IO_ERROR);

	/* 성공·실패 경로를 많이 반복해도 fd 가 새지 않는다 (SAFE_FCLOSE), 힙도 새지 않는다 (LSan) */
	before = open_fds();
	for (i = 0; i < 200; i++) {
		take_res(path);
		take_res("/no/such/file");
	}
	after = open_fds();
	CHECK(before >= 0 && before == after);

	unlink(path);
	return drill_report();
}
