#ifndef SUBS_TYPE_H
#define SUBS_TYPE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>

#define SUPI_LEN_MAX      15
#define NICKNAME_LEN_MAX  31
#define SD_LEN            6
#define NSSAI_NUM_MAX     8
#define AMBR_KBPS_MAX     4000000
#define EMAIL_LEN_MAX     63
#define INVOICE_EMAIL_NUM_MAX 3
#define SUSPEND_CAUSE_LEN_MAX 63

typedef enum {
    RAT_UNKNOWN = 0,
    RAT_NR,
    RAT_EUTRA,
    RAT_WLAN,
    RAT_END
} rat_type_t;

const char *rat_type_to_str(rat_type_t v);
int         rat_type_from_str(const char *s, rat_type_t *out);

typedef struct {
    uint8_t sst;
    bool    has_sd;
    char    sd[SD_LEN + 1];
} snssai_t;

typedef enum {
    CHARGE_UNKNOWN = 0,
    CHARGE_PREPAID,
    CHARGE_POSTPAID,
    CHARGE_END
} charge_method_t;

typedef struct {
    uint32_t balance;
} prepaid_info_t;

typedef struct {
    uint8_t  billing_day;
    uint8_t  email_cnt;
    char     emails[INVOICE_EMAIL_NUM_MAX][EMAIL_LEN_MAX + 1];
} postpaid_info_t;

typedef struct {
    time_t     last_seen;
    uint32_t   ambr_ul;
    uint32_t   ambr_dl;
    rat_type_t rat_type;
    charge_method_t charging_method;
    union {
        prepaid_info_t  prepaid;
        postpaid_info_t postpaid;
    } info;
    bool       suspended;
    bool       has_nickname;
    bool       has_rat_type;
    bool       has_ambr_ul;
    bool       has_ambr_dl;
    bool       has_last_seen;
    bool       has_suspend_cause;
    char       supi[SUPI_LEN_MAX + 1];
    char       nickname[NICKNAME_LEN_MAX + 1];
    char       suspend_cause[SUSPEND_CAUSE_LEN_MAX + 1];
    uint8_t    nssai_num;
    snssai_t   nssai[NSSAI_NUM_MAX];
} subs_profile_t;

_Static_assert(sizeof(subs_profile_t) == 416, "subs_profile_t layout changed");
_Static_assert(offsetof(subs_profile_t, last_seen) == 0, "8B member must be first");

void subs_profile_init(subs_profile_t *p);
int  subs_profile_validate(const subs_profile_t *p);
void subs_profile_dump(const subs_profile_t *p);

const prepaid_info_t  *subs_profile_get_prepaid(const subs_profile_t *p);
const postpaid_info_t *subs_profile_get_postpaid(const subs_profile_t *p);
int subs_profile_set_prepaid_balance(subs_profile_t *p, uint32_t balance);
int subs_profile_set_postpaid(subs_profile_t *p, const postpaid_info_t *src);

#endif
