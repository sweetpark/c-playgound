#define _DEFAULT_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include "symmap.h"

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
    char base[] = "/tmp/sd39_XXXXXX";
    char victim[PATH_MAX], guess[PATH_MAX];
    char out1[PATH_MAX], out2[PATH_MAX], content[128];

    if (mkdtemp(base) == NULL) { perror("mkdtemp"); return 1; }
    snprintf(victim, sizeof(victim), "%s/victim_target", base);
    snprintf(guess,  sizeof(guess),  "%s/guess.tmp", base);

    write_file(victim, "ORIGINAL");
    symlink(victim, guess);   /* 공격자가 예측 가능한 이름에 미리 심어둔 트랩 */

    T_TRUE(open_predictable_tmp_bad(base, "HACKED") == 0, "Bad 호출 자체는 정상적으로 '성공'해야 한다");
    read_file(victim, content, sizeof(content));
    T_TRUE(strcmp(content, "HACKED") == 0,
        "Bad는 이름이 항상 같으므로 공격자가 미리 심어둔 심볼릭 링크를 그대로 따라가 victim_target을 덮어써야 한다(취약점 재현)");

    write_file(victim, "ORIGINAL");   /* 복원 */

    T_TRUE(create_random_tmp_good(base, out1, sizeof(out1), "SAFE1") == 0, "Good 첫 번째 호출은 성공해야 한다");
    T_TRUE(create_random_tmp_good(base, out2, sizeof(out2), "SAFE2") == 0, "Good 두 번째 호출도 성공해야 한다");
    T_TRUE(strcmp(out1, out2) != 0,
        "Good은 호출마다 다른 무작위 파일명을 만들어야 한다(예측 불가능해야 트랩을 피할 수 있다)");
    T_TRUE(strcmp(out1, guess) != 0 && strcmp(out2, guess) != 0,
        "Good이 만든 실제 경로는 공격자가 미리 심어둔 예측 가능한 이름과 달라야 한다");

    read_file(victim, content, sizeof(content));
    T_TRUE(strcmp(content, "ORIGINAL") == 0,
        "Good은 victim_target 근처에 손도 대지 않아야 한다(예측 불가능한 이름 자체가 방어)");

    read_file(out1, content, sizeof(content));
    T_TRUE(strcmp(content, "SAFE1") == 0, "Good이 실제로 만든 파일에는 내용이 정확히 써져야 한다");

    unlink(guess); unlink(victim); unlink(out1); unlink(out2); rmdir(base);

    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
