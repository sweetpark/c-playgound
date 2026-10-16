#define _DEFAULT_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>
#include <limits.h>
#include "path_guard.h"

static int g_fail = 0;
#define T_TRUE(cond, msg) \
    do { if (!(cond)) { g_fail++; \
        printf("  X %s:%d %s\n", __func__, __LINE__, msg); } } while (0)

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

int main(void)
{
    char base[]    = "/tmp/sd06_XXXXXX";
    char sandbox[PATH_MAX], secret[PATH_MAX], report1[PATH_MAX + 16];
    char bad_path[PATH_MAX], good_path[PATH_MAX];
    char content[128];

    if (mkdtemp(base) == NULL) { perror("mkdtemp"); return 1; }
    snprintf(sandbox, sizeof(sandbox), "%s/sandbox", base);
    snprintf(secret,  sizeof(secret),  "%s/secret.txt", base);       /* sandbox 바깥 */
    snprintf(report1, sizeof(report1), "%s/report1.txt", sandbox);   /* sandbox 안 */

    mkdir(sandbox, 0700);
    write_file(secret,  "TOP-SECRET");
    write_file(report1, "public report");

    if (resolve_report_path_bad(sandbox, "../secret.txt", bad_path, sizeof(bad_path)) != NULL) {
        read_file(bad_path, content, sizeof(content));
        T_TRUE(strcmp(content, "TOP-SECRET") == 0,
            "Bad는 ../secret.txt 로 sandbox 밖 파일을 실제로 읽어내야 한다(취약점 재현)");
    } else {
        g_fail++; printf("  X Bad 경로 조합 자체가 실패함(테스트 설계 오류)\n");
    }

    if (resolve_report_path_bad(sandbox, "report1.txt", bad_path, sizeof(bad_path)) != NULL) {
        read_file(bad_path, content, sizeof(content));
        T_TRUE(strcmp(content, "public report") == 0, "Bad도 정상 파일은 읽어야 한다");
    }

    T_TRUE(resolve_report_path_good(sandbox, "../secret.txt", good_path, sizeof(good_path)) == -1,
        "Good은 sandbox 밖 경로를 거부해야 한다");

    if (resolve_report_path_good(sandbox, "report1.txt", good_path, sizeof(good_path)) == 0) {
        read_file(good_path, content, sizeof(content));
        T_TRUE(strcmp(content, "public report") == 0, "Good도 정상 파일은 읽어야 한다");
    } else {
        g_fail++; printf("  X Good이 정상 파일명까지 막아버림(과잉 차단)\n");
    }

    remove(secret); remove(report1); rmdir(sandbox); rmdir(base);

    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
