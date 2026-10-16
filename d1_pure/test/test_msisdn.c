#include <stdio.h>
#include <string.h>
#include "common.h"
#include "log.h"

log_level_t g_log_level = LOG_LV_ERR;   /* log.h 의 extern 정의. 시험 파일마다 한 번 */
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
#include "msisdn.h"

static void test_normal(void)      /* 1) 정상: 대표 입력 1개 */
{
    msisdn_req_t in  = { .in_raw = "010-1234-5678", .def_cc = 82 };
    msisdn_rsp_t out;

    T_EQ_INT(msisdn_normalize(&in, &out), RET_OK);
    T_EQ_STR(out.e164, "821012345678");
    T_EQ_STR(out.nsn,  "1012345678");
    T_EQ_INT(out.cc, 82);
    T_EQ_INT(out.len, 12);
}

static void test_edge(void)        /* 2) 경계: 에러 표의 행마다 1개 */
{
    msisdn_req_t in  = { .in_raw = "+8210123456789012", .def_cc = 82 };
    msisdn_rsp_t out;

    memset(&out, 0xFF, sizeof(out));
    T_EQ_INT(msisdn_normalize(&in, &out), RET_BAD_FORMAT);  /* 15자 초과 */
    T_EQ_INT(out.e164[0], 0);                               /* ★ 실패하면 out 은 0 */
    T_EQ_INT(out.cc, 0);

    snprintf(in.in_raw, sizeof(in.in_raw), "01-0-1234-5678");
    T_EQ_INT(msisdn_normalize(&in, &out), RET_OK);          /* '-' 는 전부 무시 */

    snprintf(in.in_raw, sizeof(in.in_raw), "010+12345678");
    T_EQ_INT(msisdn_normalize(&in, &out), RET_BAD_FORMAT);  /* '+' 가 중간 */

    snprintf(in.in_raw, sizeof(in.in_raw), "010-12ab-5678");
    T_EQ_INT(msisdn_normalize(&in, &out), RET_BAD_FORMAT);  /* 허용되지 않는 문자 */

    snprintf(in.in_raw, sizeof(in.in_raw), "---");
    T_EQ_INT(msisdn_normalize(&in, &out), RET_BAD_FORMAT);  /* 숫자 없음 */

    snprintf(in.in_raw, sizeof(in.in_raw), "+8112345678");
    T_EQ_INT(msisdn_normalize(&in, &out), RET_NOT_SUPPORTED); /* 다른 국가 */
}

static void test_null(void)        /* 3) NULL: 인자 하나씩 + def_cc 범위 */
{
    msisdn_req_t in  = { .in_raw = "01012345678", .def_cc = 82 };
    msisdn_rsp_t out;

    T_EQ_INT(msisdn_normalize(NULL, &out), RET_INVALID_ARG);
    T_EQ_INT(msisdn_normalize(&in, NULL), RET_INVALID_ARG);
    T_EQ_INT(msisdn_normalize(NULL, NULL), RET_INVALID_ARG);

    in.def_cc = 0;
    T_EQ_INT(msisdn_normalize(&in, &out), RET_INVALID_ARG);
    in.def_cc = 1000;
    T_EQ_INT(msisdn_normalize(&in, &out), RET_INVALID_ARG);
}

int main(void)
{
    test_normal();
    test_edge();
    test_null();

    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
