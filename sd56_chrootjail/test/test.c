#define _DEFAULT_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <limits.h>
#include "chrootjail.h"

static int g_fail = 0;
#define T_TRUE(cond, msg) \
    do { if (!(cond)) { g_fail++; \
        printf("  X %s:%d %s\n", __func__, __LINE__, msg); } } while (0)

static char g_base[] = "/tmp/sd56_XXXXXX";
static char g_jail_root[PATH_MAX];
static char g_outside_secret[PATH_MAX];
static char g_inside_public[PATH_MAX + 16];

static void write_file(const char *path, const char *content)
{
    FILE *f = fopen(path, "w");
    if (f) { fputs(content, f); fclose(f); }
}

static int read_file(const char *path, char *buf, size_t n)
{
    FILE *f = fopen(path, "r");
    size_t r;
    if (!f) return -1;
    r = fread(buf, 1, n - 1, f);
    buf[r] = '\0';
    fclose(f);
    return 0;
}

static void setup(void)
{
    if (mkdtemp(g_base) == NULL) { perror("mkdtemp"); exit(1); }
    snprintf(g_jail_root, sizeof(g_jail_root), "%s/ftproot", g_base);
    snprintf(g_outside_secret, sizeof(g_outside_secret), "%s/etc_shadow_like", g_base);
    snprintf(g_inside_public, sizeof(g_inside_public), "%s/public.txt", g_jail_root);

    mkdir(g_jail_root, 0700);
    write_file(g_outside_secret, "TOP-SECRET-SHADOW");
    write_file(g_inside_public, "public data");
}

static void teardown(void)
{
    unlink(g_outside_secret);
    unlink(g_inside_public);
    rmdir(g_jail_root);
    rmdir(g_base);
}

static void test_bad_escapes_jail(void)
{
    char out[PATH_MAX], content[128];
    T_TRUE(resolve_in_jail_bad(g_base, "etc_shadow_like", out, sizeof(out)) == 0,
        "Bad는 chdir(\"/\")을 안 해 여전히 jail 밖 작업 디렉터리를 기준으로 상대 경로를 찾아야 한다(취약점 재현)");
    read_file(out, content, sizeof(content));
    T_TRUE(strcmp(content, "TOP-SECRET-SHADOW") == 0,
        "그 결과 jail 밖의 비밀 파일 내용을 그대로 읽어낼 수 있어야 한다");
}

static void test_good_blocks_escape(void)
{
    char out[PATH_MAX];
    T_TRUE(resolve_in_jail_good(g_jail_root, "etc_shadow_like", out, sizeof(out)) == -1,
        "Good은 chdir(\"/\")로 기준점이 jail 루트 자신으로 바뀌어 같은 상대 경로로는 jail 밖 파일을 찾지 못해야 한다");
}

static void test_good_normal(void)
{
    char out[PATH_MAX], content[128];
    T_TRUE(resolve_in_jail_good(g_jail_root, "public.txt", out, sizeof(out)) == 0,
        "Good도 jail 안의 정상 파일은 그대로 찾아야 한다");
    read_file(out, content, sizeof(content));
    T_TRUE(strcmp(content, "public data") == 0, "jail 안 파일의 내용이 정확해야 한다");
}

int main(void)
{
    setup();
    test_bad_escapes_jail();
    test_good_blocks_escape();
    test_good_normal();
    teardown();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
