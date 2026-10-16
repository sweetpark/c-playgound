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
#include "subs_type.h"
static void test_normal(void)   /* 필수만 채운 최소 레코드가 통과해야 한다 */
{
    subs_profile_t p;
    subs_profile_init(&p);
    snprintf(p.supi, sizeof(p.supi), "450081234567890");
    p.nssai_num       = 1;
    p.nssai[0].sst    = 1;
    subs_profile_set_prepaid_balance(&p, 0);     /* chargingMethod 는 M — 안 정하면 반려된다 */
    T_EQ_INT(subs_profile_validate(&p), RET_OK);
}

static void test_edge(void)     /* 경계: 배열 0개 / 9개 / 범위 초과 */
{
    subs_profile_t p;
    subs_profile_init(&p);
    snprintf(p.supi, sizeof(p.supi), "450081234567890");
    subs_profile_set_prepaid_balance(&p, 0);

    p.nssai_num = 0;                    /* 규격상 최소 1개 */
    T_EQ_INT(subs_profile_validate(&p), RET_BAD_FORMAT);

    p.nssai_num = 9;                    /* 규격상 최대 8개 */
    T_EQ_INT(subs_profile_validate(&p), RET_BAD_FORMAT);

    p.nssai_num = 1; p.nssai[0].sst = 255;
    p.has_ambr_ul = true; p.ambr_ul = 4000001;   /* 상한 한 칸 초과 */
    T_EQ_INT(subs_profile_validate(&p), RET_BAD_FORMAT);
}

static void test_null(void)
{
    T_EQ_INT(subs_profile_validate(NULL), RET_INVALID_ARG);
    T_EQ_INT(rat_type_from_str(NULL, NULL), RET_INVALID_ARG);
}
#include <stddef.h>                    /* offsetof */
#include <stdalign.h>                  /* alignof (C11) */

#define FSZ(_t, _m)   sizeof(((_t *)0)->_m)

static void test_layout(void)
{
    /* 멤버 크기 합. 멤버를 추가하면 여기도 같이 늘려야 한다 -> 그래서 이 시험이 필요하다 */
    size_t sum = FSZ(subs_profile_t, supi)      + FSZ(subs_profile_t, suspended)
               + FSZ(subs_profile_t, has_nickname) + FSZ(subs_profile_t, nickname)
               + FSZ(subs_profile_t, has_rat_type) + FSZ(subs_profile_t, rat_type)
               + FSZ(subs_profile_t, has_ambr_ul)  + FSZ(subs_profile_t, ambr_ul)
               + FSZ(subs_profile_t, has_ambr_dl)  + FSZ(subs_profile_t, ambr_dl)
               + FSZ(subs_profile_t, has_last_seen)+ FSZ(subs_profile_t, last_seen)
               + FSZ(subs_profile_t, nssai)        + FSZ(subs_profile_t, nssai_num)
               + FSZ(subs_profile_t, has_suspend_cause) + FSZ(subs_profile_t, suspend_cause)
               + FSZ(subs_profile_t, charging_method)   + FSZ(subs_profile_t, info);   /* 확장 필드 */
    size_t pad = sizeof(subs_profile_t) - sum;

    /* 구멍이 하나도 없으면 남는 패딩은 꼬리 패딩뿐이고, 꼬리 패딩은 반드시 정렬값보다 작다 */
    T_EQ_INT(pad < alignof(subs_profile_t), 1);

    /* 큰 것부터 놓였는지: 8B 멤버가 맨 앞 */
    T_EQ_INT((int)offsetof(subs_profile_t, last_seen), 0);
}
static void test_conditional(void)
{
    subs_profile_t p;

    subs_profile_init(&p);
    snprintf(p.supi, sizeof(p.supi), "450081234567890");
    p.nssai_num = 1; p.nssai[0].sst = 1;
    subs_profile_set_prepaid_balance(&p, 1000);      /* charging_method = PREPAID */

    /* ① 조건 참(suspended=true)인데 cause 가 없다 -> 반려 */
    p.suspended = true;
    p.has_suspend_cause = false;
    T_EQ_INT(subs_profile_validate(&p), RET_INVALID_ARG);

    /* ② 조건 참 + cause 있음 -> 통과 */
    p.has_suspend_cause = true;
    snprintf(p.suspend_cause, sizeof(p.suspend_cause), "OVERDUE");
    T_EQ_INT(subs_profile_validate(&p), RET_OK);

    /* ③ 조건 거짓(suspended=false)인데 cause 가 왔다 -> 내가 정한 정책대로.
     *    거절이면 RET_INVALID_ARG, 무시+경고면 RET_OK. 둘 중 하나로 고정되어 있으면 된다 */
    p.suspended = false;
    T_EQ_INT(subs_profile_validate(&p), RET_OK);     /* 무시 정책을 골랐을 때 */
}
static void test_union(void)
{
    subs_profile_t p;
    const postpaid_info_t *pp = NULL;

    subs_profile_init(&p);

    /* ① 태그가 NONE/PREPAID 인데 postpaid 를 읽으려 하면 NULL 이어야 한다.
     *    여기서 쓰레기 포인터가 나오면 getter 가 태그를 안 보고 있다는 뜻 */
    T_EQ_INT(subs_profile_get_postpaid(&p) == NULL, 1);

    /* ② POSTPAID 로 채운다 */
    {
        postpaid_info_t in;
        memset(&in, 0x00, sizeof(in));
        in.billing_day = 25;
        in.email_cnt   = 2;
        snprintf(in.emails[0], sizeof(in.emails[0]), "a@x.com");
        snprintf(in.emails[1], sizeof(in.emails[1]), "b@x.com");
        T_EQ_INT(subs_profile_set_postpaid(&p, &in), RET_OK);
    }
    pp = subs_profile_get_postpaid(&p);
    T_EQ_INT(pp != NULL, 1);
    T_EQ_INT(pp->billing_day, 25);
    T_EQ_INT(pp->email_cnt, 2);

    /* ③ ★ 잔재 시험: PREPAID 로 바꾸면 postpaid 는 더 이상 안 보여야 하고,
     *    prepaid 쪽에 이전 바이트가 섞여 들어오면 안 된다.
     *    set 함수가 union 을 memset 하지 않으면 여기서 balance 가 쓰레기가 된다 */
    T_EQ_INT(subs_profile_set_prepaid_balance(&p, 0), RET_OK);
    T_EQ_INT(subs_profile_get_postpaid(&p) == NULL, 1);
    T_EQ_INT(subs_profile_get_prepaid(&p)->balance, 0);

    /* ④ 태그가 가리키는 쪽만 validate 가 본다 — postpaid 범위를 어겨도 PREPAID 면 통과 */
    snprintf(p.supi, sizeof(p.supi), "450081234567890");
    p.nssai_num = 1; p.nssai[0].sst = 1;
    T_EQ_INT(subs_profile_validate(&p), RET_OK);
}
static void test_enum(void)
{
    int i = 0;

    /* to_str -> from_str 이 제자리로 돌아와야 한다. UNKNOWN 은 포함, END 는 제외 */
    for (i = RAT_UNKNOWN; i < RAT_END; i++) {
        rat_type_t back = RAT_UNKNOWN;
        T_EQ_INT(rat_type_from_str(rat_type_to_str((rat_type_t)i), &back), RET_OK);
        T_EQ_INT(back, i);
    }

    {
        rat_type_t out = RAT_NR;
        T_EQ_INT(rat_type_from_str("6G", &out), RET_OK);   /* 모르는 값은 에러가 아니다 */
        T_EQ_INT(out, RAT_UNKNOWN);
    }
}

int main(void)
{
    test_normal();
    test_edge();
    test_null();
    test_layout();
    test_conditional();
    test_union();
    test_enum();

    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
