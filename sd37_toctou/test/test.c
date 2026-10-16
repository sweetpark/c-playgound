#define _DEFAULT_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include "toctou.h"

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
    char base[] = "/tmp/sd37_XXXXXX";
    char target[PATH_MAX], link[PATH_MAX], fresh[PATH_MAX];
    char content[128];

    if (mkdtemp(base) == NULL) { perror("mkdtemp"); return 1; }
    snprintf(target, sizeof(target), "%s/victim_target", base);
    snprintf(link,   sizeof(link),   "%s/toctou_link", base);
    snprintf(fresh,  sizeof(fresh),  "%s/fresh_report", base);

    write_file(target, "ORIGINAL");
    symlink(target, link);   /* 검사 시점에는 정상 파일을 가리키던 이름 */

    T_TRUE(file_operation_bad(link, "HACKED") == 0, "Bad는 심볼릭 링크를 따라 정상적으로 '성공'해야 한다");
    read_file(target, content, sizeof(content));
    T_TRUE(strcmp(content, "HACKED") == 0,
        "Bad는 이름만 보고 fopen 했으므로, 링크 뒤에 있던 victim_target이 그대로 덮어써져야 한다(취약점 재현)");

    write_file(target, "ORIGINAL");   /* 다음 검증을 위해 복원 */

    T_TRUE(file_operation_good(link, "HACKED2") == -1,
        "Good은 O_CREAT|O_EXCL이므로 이미 존재하는 이름(심볼릭 링크 포함)에는 실패해야 한다");
    read_file(target, content, sizeof(content));
    T_TRUE(strcmp(content, "ORIGINAL") == 0,
        "Good이 거부했다면 victim_target 내용은 전혀 바뀌지 않아야 한다");

    T_TRUE(file_operation_good(fresh, "FRESH") == 0, "Good은 완전히 새 경로에는 정상적으로 성공해야 한다");
    read_file(fresh, content, sizeof(content));
    T_TRUE(strcmp(content, "FRESH") == 0, "새로 만든 파일에는 내용이 정확히 써져야 한다");

    unlink(link); unlink(target); unlink(fresh); rmdir(base);

    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
