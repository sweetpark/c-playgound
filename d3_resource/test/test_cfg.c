/* 시험 파일 공통 머리 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "log.h"

log_level_t g_log_level = LOG_LV_ERR;      /* 실패 주입 때 로그가 쏟아지지 않게 */
int g_alloc_fail_at = -1;
int g_alloc_count   = 0;

static int g_fail = 0;

#define T_EQ_INT(_got, _exp)                                            \
    do {                                                                \
        long long _g = (long long)(_got), _e = (long long)(_exp);       \
        if (_g != _e) { g_fail++;                                       \
            printf("  X %s:%d  got=%lld exp=%lld\n",                    \
                   __func__, __LINE__, _g, _e); }                       \
    } while (0)

#define T_EQ_STR(_got, _exp)                                            \
    do {                                                                \
        if (strcmp((_got), (_exp)) != 0) { g_fail++;                    \
            printf("  X %s:%d  got=\"%s\" exp=\"%s\"\n",                \
                   __func__, __LINE__, (_got), (_exp)); }               \
    } while (0)
#include "cfg.h"

#include "cfg.h"

static void write_file(const char *path, const char *text)
{
    FILE *fp = fopen(path, "w");

    if (fp == NULL) { g_fail++; printf("  X cannot write %s\n", path); return; }
    fputs(text, fp);
    fclose(fp);
}

static void test_normal(void)
{
    cfg_t      *cfg = NULL;
    const char *s   = NULL;
    int         n   = 0;

    write_file("build/t_ok.cfg",
               "# comment\n"
               "\n"
               "listen_port = 8080\n"
               "worker_num=4\n"
               "  name =  foo  \n");

    T_EQ_INT(cfg_create("build/t_ok.cfg", &cfg), RET_OK);
    T_EQ_INT(cfg_get_int(cfg, "listen_port", &n), RET_OK);
    T_EQ_INT(n, 8080);
    T_EQ_INT(cfg_get_str(cfg, "name", &s), RET_OK);
    T_EQ_STR(s, "foo");                                   /* 앞뒤 공백 제거 */
    T_EQ_INT(cfg_get_str(cfg, "nothing", &s), RET_NOT_FOUND);
    cfg_destroy(cfg);
}

static void test_edge(void)
{
    cfg_t *cfg = NULL;
    int    n   = 0;
    char   big[64 * 1024];
    size_t off = 0;
    int    i   = 0;

    write_file("build/t_dup.cfg", "listen_port=1\nworker_num=4\nworker_num=8\n");
    T_EQ_INT(cfg_create("build/t_dup.cfg", &cfg), RET_OK);
    T_EQ_INT(cfg_get_int(cfg, "worker_num", &n), RET_OK);
    T_EQ_INT(n, 8);                                       /* 나중 것이 이긴다 */
    cfg_destroy(cfg);

    write_file("build/t_p1.cfg", "listen_port=65535\nworker_num=64\n");
    T_EQ_INT(cfg_create("build/t_p1.cfg", &cfg), RET_OK); /* 상한 */
    cfg_destroy(cfg);

    write_file("build/t_p2.cfg", "listen_port=65536\nworker_num=4\n");
    T_EQ_INT(cfg_create("build/t_p2.cfg", &cfg), RET_BAD_FORMAT);   /* 한 칸 초과 */
    T_EQ_INT(cfg == NULL, 1);                             /* ★ 실패하면 NULL */

    write_file("build/t_p3.cfg", "listen_port=0\nworker_num=4\n");
    T_EQ_INT(cfg_create("build/t_p3.cfg", &cfg), RET_BAD_FORMAT);   /* 한 칸 아래 */

    write_file("build/t_p4.cfg", "listen_port=80\nworker_num=65\n");
    T_EQ_INT(cfg_create("build/t_p4.cfg", &cfg), RET_BAD_FORMAT);

    write_file("build/t_p5.cfg", "listen_port=abc\nworker_num=4\n");
    T_EQ_INT(cfg_create("build/t_p5.cfg", &cfg), RET_BAD_FORMAT);   /* 숫자 아님 */

    write_file("build/t_p6.cfg", "worker_num=4\n");
    T_EQ_INT(cfg_create("build/t_p6.cfg", &cfg), RET_NOT_FOUND);    /* 필수 키 없음 */

    write_file("build/t_p7.cfg", "listen_port 8080\nworker_num=4\n");
    T_EQ_INT(cfg_create("build/t_p7.cfg", &cfg), RET_BAD_FORMAT);   /* '=' 없음 */

    write_file("build/t_p8.cfg", "=1\nlisten_port=80\nworker_num=4\n");
    T_EQ_INT(cfg_create("build/t_p8.cfg", &cfg), RET_BAD_FORMAT);   /* 빈 key */

    T_EQ_INT(cfg_create("build/no_such.cfg", &cfg), RET_IO_ERROR);  /* 파일 없음 */
    T_EQ_INT(cfg == NULL, 1);

    /* 항목 300개: 8 → 16 → ... → 512 로 확장되는 경로(realloc)를 탄다 */
    off += (size_t)snprintf(big + off, sizeof(big) - off, "listen_port=80\nworker_num=4\n");
    for (i = 0; i < 300; i++) {
        off += (size_t)snprintf(big + off, sizeof(big) - off, "key%d=%d\n", i, i);
    }
    write_file("build/t_big.cfg", big);
    T_EQ_INT(cfg_create("build/t_big.cfg", &cfg), RET_OK);
    {
        const char *s = NULL;
        T_EQ_INT(cfg_get_str(cfg, "key299", &s), RET_OK);
        T_EQ_STR(s, "299");
    }
    cfg_destroy(cfg);
}

static void test_null(void)
{
    cfg_t *cfg = NULL;

    T_EQ_INT(cfg_create(NULL, &cfg), RET_INVALID_ARG);
    T_EQ_INT(cfg_create("build/t_ok.cfg", NULL), RET_INVALID_ARG);
    cfg_destroy(NULL);                                    /* 죽지 않으면 통과 */
}

/* 0번째부터 20번째 할당까지 하나씩 실패시켜 본다 */
static void test_alloc_fail_all(void)
{
    int k = 0;

    for (k = 0; k < 20; k++) {
        cfg_t *cfg = NULL;
        int    ret = 0;

        g_alloc_fail_at = k;
        g_alloc_count   = 0;

        ret = cfg_create("build/t_ok.cfg", &cfg);

        if (ret != RET_OK) T_EQ_INT(cfg == NULL, 1);      /* 실패했으면 cfg 는 반드시 NULL */
        cfg_destroy(cfg);                                 /* 성공했으면 여기서 해제 */
    }
    g_alloc_fail_at = -1;
}

int main(void)
{
    test_normal();
    test_edge();
    test_null();
    test_alloc_fail_all();

    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
