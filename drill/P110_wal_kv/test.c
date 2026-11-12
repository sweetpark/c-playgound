#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "drill_test.h"
#include "kv.h"
#include "solution.c"

static char g_dir[64];
static char g_wal[128], g_snap[128];

static int count_lines(const char *path)
{
	FILE *f = fopen(path, "r");
	int n = 0, c;

	if (f == NULL)
		return -1;
	while ((c = fgetc(f)) != EOF)
		if (c == '\n')
			n++;
	fclose(f);
	return n;
}

static long file_size(const char *path)
{
	struct stat st;

	return stat(path, &st) == 0 ? (long)st.st_size : -1;
}

static size_t slurp(const char *path, char *buf, size_t sz)
{
	FILE *f = fopen(path, "r");
	size_t n;

	if (f == NULL)
		return 0;
	n = fread(buf, 1, sz - 1, f);
	buf[n] = '\0';
	fclose(f);
	return n;
}

static void write_file(const char *path, const char *data)
{
	FILE *f = fopen(path, "w");

	if (f) {
		fputs(data, f);
		fclose(f);
	}
}

static void append_file(const char *path, const char *data)
{
	FILE *f = fopen(path, "a");

	if (f) {
		fputs(data, f);
		fclose(f);
	}
}

/* 올바른 crc 가 붙은 로그 한 줄을 만들어 덧붙인다 */
static void append_record(const char *body)
{
	char line[256];
	unsigned sum = 0;
	size_t i;

	for (i = 0; body[i]; i++)
		sum += (unsigned char)body[i];
	snprintf(line, sizeof(line), "%s %04x\n", body, sum & 0xffffu);
	append_file(g_wal, line);
}

int main(void)
{
	kv_t *k;
	char saved[2048];
	int rc;

	snprintf(g_dir, sizeof(g_dir), "/tmp/p110_XXXXXX");
	if (mkdtemp(g_dir) == NULL) {
		printf("mkdtemp 실패\n");
		return 2;
	}
	snprintf(g_wal, sizeof(g_wal), "%s/wal.log", g_dir);
	snprintf(g_snap, sizeof(g_snap), "%s/snapshot", g_dir);

	/* [1] 열기 */
	CHECK(kv_open(NULL) == NULL);
	CHECK(kv_open("/nonexistent_dir_p110/x") == NULL);
	k = kv_open(g_dir);
	CHECK(k != NULL);
	if (k == NULL) {
		rmdir(g_dir); /* 임시 디렉터리를 남기지 않는다 */
		return drill_report();
	}
	CHECK(kv_count(k) == 0 && kv_lsn(k) == 0);
	CHECK(kv_get(k, "a") == NULL);

	/* [2] put / 덮어쓰기 / del */
	CHECK(kv_put(k, "a", "1") == 0);
	CHECK(kv_put(k, "b", "2") == 0);
	CHECK(kv_put(k, "c", "3") == 0);
	CHECK(kv_lsn(k) == 3 && kv_count(k) == 3);
	CHECK(kv_put(k, "a", "9") == 0);
	CHECK(kv_lsn(k) == 4 && kv_count(k) == 3);
	CHECK(strcmp(kv_get(k, "a"), "9") == 0);
	CHECK(kv_del(k, "b") == 0);
	CHECK(kv_lsn(k) == 5 && kv_count(k) == 2);
	CHECK(kv_del(k, "b") == 1); /* 없는 키: 로그에 안 쓴다 */
	CHECK(kv_lsn(k) == 5);
	CHECK(kv_get(k, "b") == NULL);

	/* [3] 잘못된 키/값은 거부하고 아무것도 안 바뀐다 */
	CHECK(kv_put(k, "", "x") == -1);
	CHECK(kv_put(k, "a b", "x") == -1);
	CHECK(kv_put(k, "k", "") == -1);
	CHECK(kv_put(k, "k", "x y") == -1);
	CHECK(kv_put(k, "123456789012345678901234567890123", "x") == -1); /* 33자 */
	CHECK(kv_put(k, NULL, "x") == -1);
	CHECK(kv_lsn(k) == 5 && kv_count(k) == 2);
	CHECK(count_lines(g_wal) == 5);

	/* [4] 크래시 복구: 체크포인트 없이 닫고 다시 연다 → 로그만으로 복원 */
	kv_close(k);
	k = kv_open(g_dir);
	CHECK(k != NULL);
	CHECK(kv_count(k) == 2 && kv_lsn(k) == 5);
	CHECK(strcmp(kv_get(k, "a"), "9") == 0 && strcmp(kv_get(k, "c"), "3") == 0);
	CHECK(kv_get(k, "b") == NULL);

	/* [5] 체크포인트: 스냅샷이 생기고 로그는 비워진다. 이후 변경만 로그에 쌓인다 */
	CHECK(kv_checkpoint(k) == 0);
	CHECK(file_size(g_snap) > 0);
	CHECK(file_size(g_wal) == 0);
	CHECK(kv_put(k, "d", "4") == 0);
	CHECK(kv_lsn(k) == 6);
	kv_close(k);
	k = kv_open(g_dir);
	CHECK(kv_count(k) == 3 && kv_lsn(k) == 6);
	CHECK(strcmp(kv_get(k, "d"), "4") == 0 && strcmp(kv_get(k, "a"), "9") == 0);
	CHECK(count_lines(g_wal) == 1);

	/* [6] 스냅샷은 만들었는데 로그를 비우기 전에 죽었다: 옛 로그가 그대로 남아 있다.
	 *     이미 스냅샷에 들어간 기록(lsn <= 스냅샷 lsn)을 다시 적용하면 안 된다 */
	CHECK(kv_put(k, "x", "1") == 0); /* lsn 7 */
	CHECK(kv_put(k, "y", "2") == 0); /* lsn 8 */
	CHECK(slurp(g_wal, saved, sizeof(saved)) > 0);
	CHECK(kv_checkpoint(k) == 0);
	kv_close(k);
	write_file(g_wal, saved); /* 크래시 흉내: 비워졌어야 할 로그가 되살아남 */
	k = kv_open(g_dir);
	CHECK(kv_count(k) == 5 && kv_lsn(k) == 8);
	CHECK(strcmp(kv_get(k, "x"), "1") == 0 && strcmp(kv_get(k, "y"), "2") == 0);
	kv_close(k);
	/* 옛 기록(7, 8) 뒤에 새 기록(9)이 이미 붙어 있는 로그: 옛 것은 건너뛰고 새 것은 살려야 한다 */
	write_file(g_wal, saved);
	append_record("9 P z 3");
	k = kv_open(g_dir);
	CHECK(kv_count(k) == 6 && kv_lsn(k) == 9);
	CHECK(kv_get(k, "z") != NULL && strcmp(kv_get(k, "z"), "3") == 0);

	/* [7] 찢어진 꼬리: 쓰다 말고 죽어 줄바꿈 없는 반쪽 줄이 남았다 */
	kv_close(k);
	append_file(g_wal, "10 P ke");
	k = kv_open(g_dir);
	CHECK(k != NULL);
	CHECK(kv_lsn(k) == 9 && kv_count(k) == 6);
	CHECK(kv_get(k, "ke") == NULL);
	CHECK(kv_put(k, "w", "1") == 0); /* 반쪽 줄을 잘라 내지 않았다면 이 기록이 반쪽 줄에 붙어 깨진다 */
	CHECK(kv_lsn(k) == 10);
	kv_close(k);
	k = kv_open(g_dir);
	CHECK(k != NULL);
	CHECK(kv_lsn(k) == 10 && strcmp(kv_get(k, "w"), "1") == 0);

	/* [7b] 내용은 온전한데 줄바꿈만 빠진 마지막 줄: 끝까지 쓰였다는 보장이 없으므로 버린다 */
	kv_close(k);
	{
		char line[64];
		unsigned sum = 0;
		const char *body = "11 P torn 1";
		size_t i;

		for (i = 0; body[i]; i++)
			sum += (unsigned char)body[i];
		snprintf(line, sizeof(line), "%s %04x", body, sum & 0xffffu); /* '\n' 없음 */
		append_file(g_wal, line);
	}
	k = kv_open(g_dir);
	CHECK(kv_lsn(k) == 10 && kv_get(k, "torn") == NULL);

	/* [8] crc 가 틀린 줄: 거기서 멈추고, 그 뒤에 올바른 줄이 있어도 버린다 */
	kv_close(k);
	append_file(g_wal, "11 P q 1 0000\n");
	append_record("12 P r 1");
	k = kv_open(g_dir);
	CHECK(kv_lsn(k) == 10);
	CHECK(kv_get(k, "q") == NULL && kv_get(k, "r") == NULL);
	CHECK(kv_put(k, "m", "1") == 0); /* lsn 11 */
	kv_close(k);
	k = kv_open(g_dir);
	CHECK(kv_lsn(k) == 11 && kv_get(k, "m") != NULL && kv_get(k, "r") == NULL);

	/* [9] lsn 이 건너뛰면(11 다음에 20) 그 줄부터 버린다. 형식 오류도 마찬가지 */
	kv_close(k);
	append_record("20 P g 1");
	k = kv_open(g_dir);
	CHECK(kv_lsn(k) == 11 && kv_get(k, "g") == NULL);
	kv_close(k);
	append_file(g_wal, "garbage line\n");
	k = kv_open(g_dir);
	CHECK(kv_lsn(k) == 11);
	CHECK(kv_put(k, "n", "1") == 0);
	rc = kv_del(k, "n");
	CHECK(rc == 0 && kv_lsn(k) == 13);
	kv_close(k);
	k = kv_open(g_dir);
	CHECK(kv_lsn(k) == 13 && kv_get(k, "n") == NULL); /* 삭제 기록도 재생된다 */

	/* [10] 체크포인트를 연달아 해도 안전하고, 삭제된 키는 스냅샷에 없다 */
	CHECK(kv_checkpoint(k) == 0);
	CHECK(kv_checkpoint(k) == 0);
	kv_close(k);
	k = kv_open(g_dir);
	CHECK(kv_lsn(k) == 13 && kv_get(k, "n") == NULL && kv_get(k, "b") == NULL);
	CHECK(kv_count(k) == 8);
	kv_close(k);

	/* [11] END 줄이 없는 스냅샷은 쓰다 만 것이다: 무시하고 빈 상태에서 시작한다 */
	write_file(g_wal, "");
	write_file(g_snap, "LSN 99\nbogus 1\n");
	k = kv_open(g_dir);
	CHECK(k != NULL);
	CHECK(kv_count(k) == 0 && kv_lsn(k) == 0 && kv_get(k, "bogus") == NULL);
	kv_close(k);

	/* 정리 */
	{
		char p[128];
		snprintf(p, sizeof(p), "%s/snapshot.tmp", g_dir);
		remove(p);
		remove(g_wal);
		remove(g_snap);
		rmdir(g_dir);
	}
	return drill_report();
}
