#include "common.h"
#include "log.h"
#include "guard.h"
#include "subs_type.h"

/* 인덱스 = enum 값. 순서가 어긋나면 전부 틀어지므로 한 곳에서만 관리한다 */
static const char *g_rat_str[RAT_END] = {
    [RAT_UNKNOWN] = "UNKNOWN",
    [RAT_NR]      = "NR",
    [RAT_EUTRA]   = "EUTRA",
    [RAT_WLAN]    = "WLAN",
};

const char *rat_type_to_str(rat_type_t v)
{
    if (v < RAT_UNKNOWN || v >= RAT_END) return "UNKNOWN";
    return g_rat_str[v];
}

int rat_type_from_str(const char *s, rat_type_t *out)
{
    int i = 0;

    CHK_PTR(s);
    CHK_PTR(out);

    *out = RAT_UNKNOWN;
    for (i = RAT_UNKNOWN; i < RAT_END; i++) {
        if (strcmp(s, g_rat_str[i]) == 0) { *out = (rat_type_t)i; return RET_OK; }
    }
    LOG_WRN("unknown ratType: %s", s);   /* 모르는 값은 에러가 아니라 UNKNOWN */
    return RET_OK;
}

void subs_profile_init(subs_profile_t *p)
{
    if (p == NULL) return;
    memset(p, 0x00, sizeof(*p));
    p->suspended = false;                /* 기본값은 memset 후 명시적으로 */
    p->rat_type  = RAT_UNKNOWN;
}
#define V_FAIL(_fmt, ...)                                               \
    do { LOG_ERR("validate: " _fmt, __VA_ARGS__); return RET_BAD_FORMAT; } while (0)

static int cond_suspend_cause_required(const subs_profile_t *p)
{
    return p->suspended;
}

int subs_profile_validate(const subs_profile_t *p)
{
    size_t len = 0;
    int    i   = 0;

    CHK_PTR(p);

    len = strnlen(p->supi, sizeof(p->supi));
    if (len == 0 || len > SUPI_LEN_MAX) V_FAIL("supi len=%zu", len);
    for (i = 0; p->supi[i] != '\0'; i++) {
        if (p->supi[i] < '0' || p->supi[i] > '9') V_FAIL("supi[%d] not digit", i);
    }

    if (p->nssai_num == 0 || p->nssai_num > NSSAI_NUM_MAX)
        V_FAIL("nssai_num=%u", p->nssai_num);

    for (i = 0; i < (int)p->nssai_num; i++) {
        if (!p->nssai[i].has_sd) continue;
        if (strnlen(p->nssai[i].sd, sizeof(p->nssai[i].sd)) != SD_LEN)
            V_FAIL("nssai[%d].sd len", i);
    }

    if (p->has_ambr_ul && p->ambr_ul > AMBR_KBPS_MAX) V_FAIL("ambr_ul=%u", p->ambr_ul);
    if (p->has_ambr_dl && p->ambr_dl > AMBR_KBPS_MAX) V_FAIL("ambr_dl=%u", p->ambr_dl);
    if (p->has_rat_type && (p->rat_type <= RAT_UNKNOWN || p->rat_type >= RAT_END))
        V_FAIL("rat_type=%d", (int)p->rat_type);
    if (p->has_last_seen && p->last_seen < 0) V_FAIL("last_seen=%lld",
                                                     (long long)p->last_seen);

    if (cond_suspend_cause_required(p)) {
        if (!p->has_suspend_cause) { LOG_ERR("suspend_cause required when suspended=%d", 1); return RET_INVALID_ARG; }
        if (strnlen(p->suspend_cause, sizeof(p->suspend_cause)) == 0)
            V_FAIL("suspend_cause empty%s", "");
    } else if (p->has_suspend_cause) {
        LOG_WRN("suspend_cause ignored: not suspended");
    }

    switch (p->charging_method) {
    case CHARGE_PREPAID:
        break;
    case CHARGE_POSTPAID: {
        const postpaid_info_t *q = &p->info.postpaid;
        if (q->billing_day < 1 || q->billing_day > 28) V_FAIL("billing_day=%u", q->billing_day);
        if (q->email_cnt > INVOICE_EMAIL_NUM_MAX)      V_FAIL("email_cnt=%u", q->email_cnt);
        for (i = 0; i < (int)q->email_cnt; i++) {
            if (q->emails[i][0] == '\0') V_FAIL("emails[%d] empty", i);
        }
        break;
    }
    default:
        V_FAIL("charging_method=%d", (int)p->charging_method);
    }

    return RET_OK;
}

const prepaid_info_t *subs_profile_get_prepaid(const subs_profile_t *p)
{
    if (p == NULL || p->charging_method != CHARGE_PREPAID) return NULL;
    return &p->info.prepaid;
}

const postpaid_info_t *subs_profile_get_postpaid(const subs_profile_t *p)
{
    if (p == NULL || p->charging_method != CHARGE_POSTPAID) return NULL;
    return &p->info.postpaid;
}

int subs_profile_set_prepaid_balance(subs_profile_t *p, uint32_t balance)
{
    CHK_PTR(p);
    p->charging_method = CHARGE_PREPAID;
    memset(&p->info, 0x00, sizeof(p->info));
    p->info.prepaid.balance = balance;
    return RET_OK;
}

int subs_profile_set_postpaid(subs_profile_t *p, const postpaid_info_t *src)
{
    CHK_PTR(p);
    CHK_PTR(src);

    p->charging_method = CHARGE_POSTPAID;
    memset(&p->info, 0x00, sizeof(p->info));
    p->info.postpaid = *src;
    return RET_OK;
}

void subs_profile_dump(const subs_profile_t *p)
{
    int i = 0;
    if (p == NULL) { LOG_INF("profile: (null)"); return; }

    LOG_INF("supi=%s suspended=%s", p->supi, p->suspended ? "Y" : "N");
    LOG_INF("  nickname=%s", p->has_nickname ? p->nickname : "-");
    LOG_INF("  rat=%s", p->has_rat_type ? rat_type_to_str(p->rat_type) : "-");

    if (p->has_ambr_ul) LOG_INF("  ambrUl=%u kbps", p->ambr_ul);
    else                LOG_INF("  ambrUl=-");

    for (i = 0; i < (int)p->nssai_num; i++) {
        LOG_INF("  nssai[%d] sst=%u sd=%s", i, p->nssai[i].sst,
                p->nssai[i].has_sd ? p->nssai[i].sd : "-");
    }
}
