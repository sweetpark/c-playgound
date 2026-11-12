/* P110 kv — 요구사항은 problem.md. kv.h 의 선언과 파일 형식을 보고 채운다. */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "kv.h"

struct kv {
	/* TODO: 디렉터리, 키/값 배열, 개수, lsn, 로그 파일 핸들 */
	int unused;
};

kv_t *kv_open(const char *dir)
{
	(void)dir;
	/* TODO: 스냅샷 읽기 → 로그 재생 → 찢어진 꼬리 잘라내기 → 로그를 이어쓰기로 열기 */
	return NULL;
}

void kv_close(kv_t *k)
{
	(void)k;
	/* TODO */
}

int kv_put(kv_t *k, const char *key, const char *val)
{
	(void)k;
	(void)key;
	(void)val;
	/* TODO: 로그에 먼저(flush+fsync), 그다음 메모리 */
	return -1;
}

int kv_del(kv_t *k, const char *key)
{
	(void)k;
	(void)key;
	/* TODO */
	return -1;
}

const char *kv_get(const kv_t *k, const char *key)
{
	(void)k;
	(void)key;
	/* TODO */
	return NULL;
}

unsigned long kv_count(const kv_t *k)
{
	(void)k;
	/* TODO */
	return 0;
}

unsigned long kv_lsn(const kv_t *k)
{
	(void)k;
	/* TODO */
	return 0;
}

int kv_checkpoint(kv_t *k)
{
	(void)k;
	/* TODO: snapshot.tmp 작성 → fsync → rename → 그다음 wal.log 비우기 */
	return -1;
}
