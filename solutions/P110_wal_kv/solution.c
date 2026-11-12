/* P110 kv 모범답안 */
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "kv.h"

#define KV_MAXTOK 32

struct kv {
	char dir[400];
	char **keys;
	char **vals;
	size_t n, cap;
	unsigned long lsn;
	FILE *wal;
};

static void path_of(const kv_t *k, const char *name, char *out, size_t sz)
{
	snprintf(out, sz, "%s/%s", k->dir, name);
}

static int valid_tok(const char *s)
{
	size_t i, n;

	if (s == NULL)
		return 0;
	n = strlen(s);
	if (n < 1 || n > KV_MAXTOK)
		return 0;
	for (i = 0; i < n; i++)
		if (!isalnum((unsigned char)s[i]) && s[i] != '_')
			return 0;
	return 1;
}

static long find(const kv_t *k, const char *key)
{
	size_t i;

	for (i = 0; i < k->n; i++)
		if (strcmp(k->keys[i], key) == 0)
			return (long)i;
	return -1;
}

static int mem_put(kv_t *k, const char *key, const char *val)
{
	long i = find(k, key);
	char *v;

	if (i >= 0) {
		v = strdup(val);
		if (v == NULL)
			return -1;
		free(k->vals[i]);
		k->vals[i] = v;
		return 0;
	}
	if (k->n == k->cap) {
		size_t nc = k->cap ? k->cap * 2 : 16;
		char **nk = realloc(k->keys, nc * sizeof(*nk));
		char **nv;

		if (nk == NULL)
			return -1;
		k->keys = nk;
		nv = realloc(k->vals, nc * sizeof(*nv));
		if (nv == NULL)
			return -1;
		k->vals = nv;
		k->cap = nc;
	}
	k->keys[k->n] = strdup(key);
	k->vals[k->n] = strdup(val);
	if (k->keys[k->n] == NULL || k->vals[k->n] == NULL) {
		free(k->keys[k->n]);
		free(k->vals[k->n]);
		return -1;
	}
	k->n++;
	return 0;
}

static void mem_del(kv_t *k, const char *key)
{
	long i = find(k, key);

	if (i < 0)
		return;
	free(k->keys[i]);
	free(k->vals[i]);
	k->keys[i] = k->keys[k->n - 1];
	k->vals[i] = k->vals[k->n - 1];
	k->n--;
}

static void mem_clear(kv_t *k)
{
	size_t i;

	for (i = 0; i < k->n; i++) {
		free(k->keys[i]);
		free(k->vals[i]);
	}
	k->n = 0;
}

static unsigned crc_of(const char *s, size_t len)
{
	unsigned sum = 0;
	size_t i;

	for (i = 0; i < len; i++)
		sum += (unsigned char)s[i];
	return sum & 0xffffu;
}

/* 스냅샷을 읽는다. END 줄까지 읽어야 유효. 유효하지 않으면 메모리를 비우고 lsn 0 */
static void load_snapshot(kv_t *k)
{
	char p[512], line[256], key[64], val[64];
	unsigned long lsn = 0;
	int got_end = 0;
	FILE *f;

	path_of(k, "snapshot", p, sizeof(p));
	f = fopen(p, "r");
	if (f == NULL)
		return;
	if (fgets(line, sizeof(line), f) == NULL || sscanf(line, "LSN %lu", &lsn) != 1) {
		fclose(f);
		return;
	}
	while (fgets(line, sizeof(line), f) != NULL) {
		if (strcmp(line, "END\n") == 0) {
			got_end = 1;
			break;
		}
		if (sscanf(line, "%63s %63s", key, val) != 2 || !valid_tok(key) || !valid_tok(val) || mem_put(k, key, val) != 0)
			break;
	}
	fclose(f);
	if (got_end)
		k->lsn = lsn;
	else
		mem_clear(k);
}

/* wal.log 를 재생하고, 유효한 부분의 끝 오프셋을 돌려준다 */
static long replay_wal(kv_t *k)
{
	char p[512], line[256], key[64], val[64];
	unsigned long lsn;
	unsigned crc;
	char op;
	int used;
	long good = 0, off;
	FILE *f;

	path_of(k, "wal.log", p, sizeof(p));
	f = fopen(p, "r");
	if (f == NULL)
		return 0;
	for (;;) {
		char *sp;

		off = ftell(f);
		if (fgets(line, sizeof(line), f) == NULL)
			break;
		used = 0;
		if (sscanf(line, "%lu %c %63s %63s %x%n", &lsn, &op, key, val, &crc, &used) != 5 || used == 0 ||
		    line[used] != '\n' || line[used + 1] != '\0')
			break;
		sp = strrchr(line, ' ');
		if (sp == NULL || crc_of(line, (size_t)(sp - line)) != crc)
			break;
		if (!valid_tok(key) || (op == 'P' ? !valid_tok(val) : (op != 'D' || strcmp(val, "-") != 0)))
			break;
		if (lsn <= k->lsn) {
			good = ftell(f);
			continue;
		}
		if (lsn != k->lsn + 1)
			break;
		if (op == 'P') {
			if (mem_put(k, key, val) != 0)
				break;
		} else {
			mem_del(k, key);
		}
		k->lsn = lsn;
		good = ftell(f);
	}
	(void)off;
	fclose(f);
	return good;
}

kv_t *kv_open(const char *dir)
{
	struct stat st;
	kv_t *k;
	char p[512];
	long good;

	if (dir == NULL || strlen(dir) >= sizeof(k->dir) || stat(dir, &st) != 0 || !S_ISDIR(st.st_mode))
		return NULL;
	k = calloc(1, sizeof(*k));
	if (k == NULL)
		return NULL;
	strcpy(k->dir, dir);
	load_snapshot(k);
	good = replay_wal(k);
	path_of(k, "wal.log", p, sizeof(p));
	if (access(p, F_OK) == 0 && truncate(p, good) != 0) {
		kv_close(k);
		return NULL;
	}
	k->wal = fopen(p, "a");
	if (k->wal == NULL) {
		kv_close(k);
		return NULL;
	}
	return k;
}

void kv_close(kv_t *k)
{
	if (k == NULL)
		return;
	mem_clear(k);
	free(k->keys);
	free(k->vals);
	if (k->wal)
		fclose(k->wal);
	free(k);
}

static int log_record(kv_t *k, const char *body)
{
	if (fprintf(k->wal, "%s %04x\n", body, crc_of(body, strlen(body))) < 0)
		return -1;
	if (fflush(k->wal) != 0 || fsync(fileno(k->wal)) != 0)
		return -1;
	return 0;
}

int kv_put(kv_t *k, const char *key, const char *val)
{
	char body[160];

	if (k == NULL || !valid_tok(key) || !valid_tok(val))
		return -1;
	snprintf(body, sizeof(body), "%lu P %s %s", k->lsn + 1, key, val);
	if (log_record(k, body) != 0)
		return -1;
	if (mem_put(k, key, val) != 0)
		return -1;
	k->lsn++;
	return 0;
}

int kv_del(kv_t *k, const char *key)
{
	char body[160];

	if (k == NULL || !valid_tok(key))
		return -1;
	if (find(k, key) < 0)
		return 1;
	snprintf(body, sizeof(body), "%lu D %s -", k->lsn + 1, key);
	if (log_record(k, body) != 0)
		return -1;
	mem_del(k, key);
	k->lsn++;
	return 0;
}

const char *kv_get(const kv_t *k, const char *key)
{
	long i;

	if (k == NULL || key == NULL)
		return NULL;
	i = find(k, key);
	return i < 0 ? NULL : k->vals[i];
}

unsigned long kv_count(const kv_t *k)
{
	return k ? (unsigned long)k->n : 0;
}

unsigned long kv_lsn(const kv_t *k)
{
	return k ? k->lsn : 0;
}

int kv_checkpoint(kv_t *k)
{
	char tmp[512], fin[512], wal[512];
	FILE *f;
	size_t i;

	if (k == NULL)
		return -1;
	path_of(k, "snapshot.tmp", tmp, sizeof(tmp));
	path_of(k, "snapshot", fin, sizeof(fin));
	path_of(k, "wal.log", wal, sizeof(wal));
	f = fopen(tmp, "w");
	if (f == NULL)
		return -1;
	fprintf(f, "LSN %lu\n", k->lsn);
	for (i = 0; i < k->n; i++)
		fprintf(f, "%s %s\n", k->keys[i], k->vals[i]);
	fprintf(f, "END\n");
	if (fflush(f) != 0 || fsync(fileno(f)) != 0) {
		fclose(f);
		return -1;
	}
	fclose(f);
	if (rename(tmp, fin) != 0)
		return -1;
	/* 스냅샷이 완전히 자리 잡은 뒤에야 옛 로그를 비운다 */
	fclose(k->wal);
	k->wal = fopen(wal, "w");
	if (k->wal == NULL)
		return -1;
	fclose(k->wal);
	k->wal = fopen(wal, "a");
	return k->wal ? 0 : -1;
}
