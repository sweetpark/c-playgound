#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <jansson.h>
#include "common.h"
#include "log.h"
#include "am_types.h"
#include "am_api.h"                     /* 함수 선언 (bitrate_* / plmn_* / am_* / access_* …) */

log_level_t g_log_level = LOG_LV_ERR;

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

static json_t *J(const char *text)               /* 시험용: 문자열 → JSON (실패하면 시험 실패) */
{
    json_error_t e;
    json_t      *r = json_loads(text, 0, &e);

    if (r == NULL) { g_fail++; printf("  X bad json in test: %s\n", e.text); }
    return r;
}
static void test_req(void)
{
    am_data_req_t r;
    const char   *base = "/subscriber-data/imsi-450051234567890/am-data";
    char          url[256];

    T_EQ_INT(am_req_parse("/subscriber-data/imsi-450051234567890/am-data"
                          "?plmn-id=45005&supported-features=3", &r), RET_OK);
    T_EQ_STR(r.supi, "imsi-450051234567890");
    T_EQ_INT(r.has_plmn, 1);   T_EQ_STR(r.plmn.mcc, "450");   T_EQ_STR(r.plmn.mnc, "05");
    T_EQ_INT(r.plmn.mnc_len, 2);
    T_EQ_INT(r.has_features, 1);   T_EQ_STR(r.features, "3");
    T_EQ_INT(ret_to_problem(RET_OK).status, 200);                     /* 1 */

    T_EQ_INT(am_req_parse(base, &r), RET_OK);                         /* 쿼리가 없다 */
    T_EQ_INT(r.has_plmn, 0);   T_EQ_INT(r.has_features, 0);

    snprintf(url, sizeof(url), "%s?plmn-id=310260", base);
    T_EQ_INT(am_req_parse(url, &r), RET_OK);
    T_EQ_STR(r.plmn.mnc, "260");   T_EQ_INT(r.plmn.mnc_len, 3);

    T_EQ_INT(am_req_parse("/subscriber-data//am-data", &r), RET_INVALID_ARG);       /* 2: supi 없음 */
    T_EQ_STR(ret_to_problem(RET_INVALID_ARG).cause, "MANDATORY_IE_MISSING");
    T_EQ_INT(am_req_parse("/subscriber-data/imsi-45005123456789/am-data", &r), RET_BAD_FORMAT);   /* 14자리 */
    T_EQ_INT(am_req_parse("/subscriber-data/imsi-45005123456789A/am-data", &r), RET_BAD_FORMAT);
    snprintf(url, sizeof(url), "%s?plmn-id=4505", base);
    T_EQ_INT(am_req_parse(url, &r), RET_BAD_FORMAT);                  /* mnc 1자리 */
    snprintf(url, sizeof(url), "%s?plmn-id=4500567", base);
    T_EQ_INT(am_req_parse(url, &r), RET_BAD_FORMAT);                  /* mnc 4자리 */
    snprintf(url, sizeof(url), "%s?supported-features=zz", base);
    T_EQ_INT(am_req_parse(url, &r), RET_BAD_FORMAT);

    T_EQ_INT(am_req_parse(NULL, &r), RET_INVALID_ARG);                /* 3 */
    T_EQ_INT(am_req_parse(base, NULL), RET_INVALID_ARG);

    am_req_parse(base, &r);
    T_EQ_INT(am_req_validate(&r, true), RET_INVALID_ARG);             /* 로밍인데 plmnId 없음 (C) */
    T_EQ_INT(am_req_validate(&r, false), RET_OK);
}

static void test_problem(void)
{
    T_EQ_INT(ret_to_problem(RET_OK).cause == NULL, 1);
    T_EQ_INT(ret_to_problem(RET_INVALID_ARG).status, 400);
    T_EQ_STR(ret_to_problem(RET_BAD_FORMAT).cause, "INVALID_MSG_FORMAT");
    T_EQ_INT(ret_to_problem(RET_NOT_FOUND).status, 404);
    T_EQ_STR(ret_to_problem(RET_NOT_FOUND).cause, "USER_NOT_FOUND");
    T_EQ_INT(ret_to_problem(RET_NOT_SUPPORTED).status, 501);
    T_EQ_INT(ret_to_problem(RET_TIMEOUT).status, 504);
    T_EQ_INT(ret_to_problem(RET_FAIL).status, 500);
    T_EQ_STR(ret_to_problem(-999).cause, "INTERNAL_ERROR");           /* 모르는 값도 500 */
}

static void test_validate(void)
{
    am_data_rsp_t r;
    am_data_req_t q;

    memset(&q, 0x00, sizeof(q));
    memset(&r, 0x00, sizeof(r));
    r.nssai.default_num = 1;  r.has_ue_ambr = true;
    T_EQ_INT(am_data_rsp_validate(&r, &q), RET_OK);

    r.has_ue_ambr = false;                                            /* 4: 정지 아님 + ambr 없음 */
    T_EQ_INT(am_data_rsp_validate(&r, &q), RET_BAD_FORMAT);
    r.suspended = true;                                               /* 정지 상태면 ambr 없어도 정상 */
    T_EQ_INT(am_data_rsp_validate(&r, &q), RET_OK);

    r.suspended = false;  r.has_ue_ambr = true;
    q.has_features = true;                                            /* 요청에 있었으면 응답에도 필수 */
    T_EQ_INT(am_data_rsp_validate(&r, &q), RET_BAD_FORMAT);
    r.has_features = true;
    T_EQ_INT(am_data_rsp_validate(&r, &q), RET_OK);

    r.has_gpsi = true;  snprintf(r.gpsi, sizeof(r.gpsi), "tel-1234");
    T_EQ_INT(am_data_rsp_validate(&r, &q), RET_BAD_FORMAT);           /* gpsi 접두 */
    snprintf(r.gpsi, sizeof(r.gpsi), "msisdn-821012345678");
    T_EQ_INT(am_data_rsp_validate(&r, &q), RET_OK);

    r.nssai.default_num = 0;
    T_EQ_INT(am_data_rsp_validate(&r, &q), RET_BAD_FORMAT);           /* 경계: 0 */
    r.nssai.default_num = 9;
    T_EQ_INT(am_data_rsp_validate(&r, &q), RET_BAD_FORMAT);           /* 경계: 9 */
    r.nssai.default_num = 8;
    T_EQ_INT(am_data_rsp_validate(&r, &q), RET_OK);

    T_EQ_INT(am_data_rsp_validate(NULL, &q), RET_INVALID_ARG);
    T_EQ_INT(am_data_rsp_validate(&r, NULL), RET_INVALID_ARG);
}

static void test_bitrate(void)
{
    uint64_t bps = 1;
    char     buf[32];
    int      i = 0;
    uint64_t samples[] = { 0, 1, 999, 64000, 100000000ULL, 1500000000ULL };

    T_EQ_INT(bitrate_from_str("100 Mbps", &bps), RET_OK);  T_EQ_INT(bps == 100000000ULL, 1);
    T_EQ_INT(bitrate_from_str("1.5 Gbps", &bps), RET_OK);  T_EQ_INT(bps == 1500000000ULL, 1);
    T_EQ_INT(bitrate_from_str("64 Kbps", &bps), RET_OK);   T_EQ_INT(bps == 64000ULL, 1);
    T_EQ_INT(bitrate_from_str("10 bps", &bps), RET_OK);    T_EQ_INT(bps == 10ULL, 1);
    T_EQ_INT(bitrate_from_str("100 mbps", &bps), RET_OK);  T_EQ_INT(bps == 100000000ULL, 1);   /* 대소문자 무시 */

    T_EQ_INT(bitrate_from_str("100", &bps), RET_BAD_FORMAT);
    T_EQ_INT(bps == 0, 1);                                            /* ★ 실패하면 0 */
    T_EQ_INT(bitrate_from_str("abc Mbps", &bps), RET_BAD_FORMAT);
    T_EQ_INT(bitrate_from_str("100 Tbps", &bps), RET_BAD_FORMAT);
    T_EQ_INT(bitrate_from_str("-1 Mbps", &bps), RET_BAD_FORMAT);
    T_EQ_INT(bitrate_from_str("100 Mbpsxxxxxxxxxxxxxxxx", &bps), RET_BAD_FORMAT);   /* 단위 폭 초과 */
    T_EQ_INT(bitrate_from_str("", &bps), RET_INVALID_ARG);
    T_EQ_INT(bitrate_from_str(NULL, &bps), RET_INVALID_ARG);
    T_EQ_INT(bitrate_from_str("1 Mbps", NULL), RET_INVALID_ARG);

    T_EQ_INT(bitrate_to_str(100000000ULL, buf, sizeof(buf)), RET_OK);  T_EQ_STR(buf, "100 Mbps");
    T_EQ_INT(bitrate_to_str(64000, buf, sizeof(buf)), RET_OK);         T_EQ_STR(buf, "64 Kbps");
    T_EQ_INT(bitrate_to_str(1500000000ULL, buf, sizeof(buf)), RET_OK); T_EQ_STR(buf, "1.5 Gbps");
    T_EQ_INT(bitrate_to_str(999, buf, sizeof(buf)), RET_OK);           T_EQ_STR(buf, "999 bps");
    T_EQ_INT(bitrate_to_str(0, buf, sizeof(buf)), RET_OK);             T_EQ_STR(buf, "0 bps");
    T_EQ_INT(bitrate_to_str(100000000ULL, buf, 4), RET_TOO_SMALL);     /* 한 칸 모자람이 아니라 한참 모자람 */

    for (i = 0; i < 6; i++) {                                         /* 왕복 */
        T_EQ_INT(bitrate_to_str(samples[i], buf, sizeof(buf)), RET_OK);
        T_EQ_INT(bitrate_from_str(buf, &bps), RET_OK);
        T_EQ_INT(bps == samples[i], 1);
    }
}

static void test_plmn(void)
{
    plmn_t p;
    char   mcc[4], mnc[4];

    T_EQ_INT(plmn_parse("450", "05", &p), RET_OK);
    T_EQ_INT(p.mnc_len, 2);
    T_EQ_INT(plmn_to_strs(&p, mcc, mnc), RET_OK);
    T_EQ_STR(mcc, "450");
    T_EQ_STR(mnc, "05");                                              /* 6: ★ "5" 가 아니다 */

    T_EQ_INT(plmn_parse("450", "005", &p), RET_OK);
    T_EQ_INT(p.mnc_len, 3);
    T_EQ_INT(plmn_to_strs(&p, mcc, mnc), RET_OK);
    T_EQ_STR(mnc, "005");                                             /* "05" 와 다른 사업자 */

    T_EQ_INT(plmn_parse("45", "05", &p), RET_BAD_FORMAT);
    T_EQ_INT(plmn_parse("450", "5", &p), RET_BAD_FORMAT);
    T_EQ_INT(plmn_parse("450", "0005", &p), RET_BAD_FORMAT);
    T_EQ_INT(plmn_parse("450", "0a", &p), RET_BAD_FORMAT);
    T_EQ_INT(plmn_parse("4a0", "05", &p), RET_BAD_FORMAT);
    T_EQ_INT(plmn_parse(NULL, "05", &p), RET_INVALID_ARG);
    T_EQ_INT(plmn_parse("450", "05", NULL), RET_INVALID_ARG);
}

static void test_features(void)
{
    uint32_t v = 0;
    char     buf[16];

    T_EQ_INT(features_from_hex("3", &v), RET_OK);          T_EQ_INT(v, 3);
    T_EQ_INT(features_from_hex("0A", &v), RET_OK);         T_EQ_INT(v, 10);
    T_EQ_INT(features_from_hex("ffffffff", &v), RET_OK);   T_EQ_INT(v == 0xFFFFFFFFu, 1);
    T_EQ_INT(features_from_hex("", &v), RET_BAD_FORMAT);
    T_EQ_INT(features_from_hex("xyz", &v), RET_BAD_FORMAT);
    T_EQ_INT(features_from_hex("123456789", &v), RET_BAD_FORMAT);     /* 9자리 */

    T_EQ_INT(features_to_hex(3, buf, sizeof(buf)), RET_OK);    T_EQ_STR(buf, "3");
    T_EQ_INT(features_to_hex(0, buf, sizeof(buf)), RET_OK);    T_EQ_STR(buf, "0");
    T_EQ_INT(features_to_hex(255, buf, sizeof(buf)), RET_OK);  T_EQ_STR(buf, "ff");

    T_EQ_INT(features_negotiate(0xFF, 0x03), 0x03);           /* 7: 상대가 모르는 비트는 끈다 */
    T_EQ_INT(features_negotiate(0x00, 0x03), 0x00);
    T_EQ_INT(features_negotiate(0x02, 0x03), 0x02);
}

static void test_unknown_echo(void)
{
    const char    *txt = "{\"nssai\":{\"defaultSingleNssais\":[{\"sst\":1}],\"singleNssais\":[]},"
                         "\"subscribedUeAmbr\":{\"uplink\":\"100 Mbps\",\"downlink\":\"1 Gbps\"},"
                         "\"x-vendor\":{\"a\":1}}";
    json_t        *root = J(txt), *out = NULL, *bad = NULL;
    am_data_rsp_t  rsp;

    T_EQ_INT(am_rsp_parse(root, &rsp), RET_OK);
    T_EQ_INT(rsp.nssai.default_num, 1);
    T_EQ_INT(rsp.has_ue_ambr, 1);
    T_EQ_INT(rsp.ue_ambr.ul_bps == 100000000ULL, 1);
    T_EQ_INT(rsp.ue_ambr.dl_bps == 1000000000ULL, 1);
    T_EQ_INT(rsp.unknown_num, 1);                                     /* 5: 모르는 키 보관 */
    T_EQ_STR(rsp.unknown[0].key, "x-vendor");

    T_EQ_INT(am_rsp_build(&rsp, &out), RET_OK);
    T_EQ_INT(json_equal(json_object_get(out, "x-vendor"),
                        json_object_get(root, "x-vendor")), 1);       /* ★ 원문 그대로 echo */
    T_EQ_STR(json_string_value(json_object_get(json_object_get(out, "subscribedUeAmbr"), "uplink")),
             "100 Mbps");
    json_decref(out);
    json_decref(root);

    bad = J("{}");
    T_EQ_INT(am_rsp_parse(bad, &rsp), RET_INVALID_ARG);               /* nssai 는 M */
    json_decref(bad);
    bad = J("{\"nssai\":5}");
    T_EQ_INT(am_rsp_parse(bad, &rsp), RET_BAD_FORMAT);                /* 타입 불일치 */
    json_decref(bad);
    T_EQ_INT(am_rsp_parse(NULL, &rsp), RET_INVALID_ARG);
}

static void test_access(void)
{
    access_info_t a;
    json_t       *j = NULL;

    j = J("{\"accessType\":\"3GPP_ACCESS\",\"gnbInfo\":{\"gnbId\":17,\"tac\":300}}");
    T_EQ_INT(access_info_parse(j, &a), RET_OK);
    T_EQ_INT(a.type, ACCESS_3GPP);
    T_EQ_INT(a.u.gnb.gnb_id, 17);   T_EQ_INT(a.u.gnb.tac, 300);
    json_decref(j);

    j = J("{\"accessType\":\"NON_3GPP_ACCESS\",\"wlanInfo\":{\"ssid\":\"abc\",\"bssid\":\"00:11:22:33:44:55\"}}");
    T_EQ_INT(access_info_parse(j, &a), RET_OK);
    T_EQ_INT(a.type, ACCESS_NON_3GPP);
    T_EQ_STR(a.u.wlan.ssid, "abc");
    json_decref(j);

    j = J("{\"accessType\":\"3GPP_ACCESS\"}");                        /* 11: 조건 참인데 gnbInfo 없음 */
    T_EQ_INT(access_info_parse(j, &a), RET_INVALID_ARG);
    json_decref(j);

    j = J("{\"accessType\":\"3GPP_ACCESS\",\"gnbInfo\":{\"gnbId\":1,\"tac\":2},"
          "\"wlanInfo\":{\"ssid\":\"x\",\"bssid\":\"00:11:22:33:44:55\"}}");
    T_EQ_INT(access_info_parse(j, &a), RET_OK);                       /* 반대편이 같이 오면 무시 + WRN */
    T_EQ_INT(a.type, ACCESS_3GPP);
    json_decref(j);

    j = J("{\"accessType\":\"WIRED\"}");
    T_EQ_INT(access_info_parse(j, &a), RET_BAD_FORMAT);               /* 모르는 값: M 이라 반려 */
    json_decref(j);
    j = J("{}");
    T_EQ_INT(access_info_parse(j, &a), RET_INVALID_ARG);              /* accessType 이 M */
    json_decref(j);
    T_EQ_INT(access_info_parse(NULL, &a), RET_INVALID_ARG);
}

static void test_oneof(void)
{
    ue_identity_t id;
    json_t       *j = NULL, *obj = json_object();

    j = J("{}");
    T_EQ_INT(ue_identity_parse(j, &id), RET_BAD_FORMAT);              /* 8: 0개 */
    json_decref(j);
    j = J("{\"supi\":\"imsi-450051234567890\",\"gpsi\":\"msisdn-1\"}");
    T_EQ_INT(ue_identity_parse(j, &id), RET_BAD_FORMAT);              /* 9: ★ 2개를 통과시키면 실패 */
    json_decref(j);

    j = J("{\"supi\":\"imsi-450051234567890\"}");
    T_EQ_INT(ue_identity_parse(j, &id), RET_OK);
    T_EQ_INT(id.kind, UEID_SUPI);   T_EQ_STR(id.u.supi, "imsi-450051234567890");
    json_decref(j);
    j = J("{\"gpsi\":\"msisdn-821012345678\"}");
    T_EQ_INT(ue_identity_parse(j, &id), RET_OK);
    T_EQ_INT(id.kind, UEID_GPSI);   T_EQ_STR(id.u.gpsi, "msisdn-821012345678");
    json_decref(j);

    memset(&id, 0x00, sizeof(id));                                    /* kind == NONE */
    T_EQ_INT(ue_identity_build(&id, obj), RET_INVALID_ARG);           /* 12 */
    T_EQ_INT(json_object_size(obj), 0);                               /* 아무것도 쓰지 않았다 */
    id.kind = UEID_SUPI;  snprintf(id.u.supi, sizeof(id.u.supi), "imsi-1");
    T_EQ_INT(ue_identity_build(&id, obj), RET_OK);
    T_EQ_STR(json_string_value(json_object_get(obj, "supi")), "imsi-1");
    json_decref(obj);
}

static void test_anyof(void)
{
    ue_ip_t p;

    memset(&p, 0x00, sizeof(p));
    T_EQ_INT(ue_ip_validate(&p), RET_BAD_FORMAT);                     /* 0개만 위반 */

    p.has_v4 = true;  snprintf(p.v4, sizeof(p.v4), "10.0.0.1");
    T_EQ_INT(ue_ip_validate(&p), RET_OK);

    p.has_v6 = true;  snprintf(p.v6, sizeof(p.v6), "2001:db8::1");    /* 10: 둘 다 */
    T_EQ_INT(ue_ip_validate(&p), RET_OK);
    T_EQ_STR(p.v4, "10.0.0.1");                                       /* ★ 둘째를 채운 뒤에도 첫째가 살아 있다 */
    T_EQ_STR(p.v6, "2001:db8::1");                                    /*   union 이었다면 v4 가 v6 의 바이트로 덮였을 것 */

    T_EQ_INT(ue_ip_validate(NULL), RET_INVALID_ARG);
}

int main(void)
{
    test_req();
    test_problem();
    test_validate();
    test_bitrate();
    test_plmn();
    test_features();
    test_unknown_echo();
    test_access();
    test_oneof();
    test_anyof();

    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
