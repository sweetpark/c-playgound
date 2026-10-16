#include <ctype.h>
#include <strings.h>
#include "common.h"
#include "log.h"
#include "guard.h"
#include "am_api.h"

/* 규격 인용을 주석으로 그대로 남긴다.
 * 나중에 "이 검사 왜 있지?" 가 되는 걸 막는 유일한 방법이다. */
int am_data_rsp_validate(const am_data_rsp_t *rsp, const am_data_req_t *req)
{
    CHK_PTR(rsp);
    CHK_PTR(req);

    /* M: nssai 는 항상 있어야 하고 default 가 1~8개 */
    if (rsp->nssai.default_num == 0 || rsp->nssai.default_num > NSSAI_NUM_MAX) {
        LOG_ERR("nssai.default_num=%u", rsp->nssai.default_num);
        return RET_BAD_FORMAT;
    }

    /* C: "가입자가 정지 상태가 아니면 subscribedUeAmbr 필수" */
    if (!rsp->suspended && !rsp->has_ue_ambr) {
        LOG_ERR("subscribedUeAmbr missing (not suspended)");
        return RET_BAD_FORMAT;                 /* → 400 MANDATORY_IE_MISSING */
    }

    /* C: "요청에 supportedFeatures 가 있었으면 응답에도 필수" */
    if (req->has_features && !rsp->has_features) {
        LOG_ERR("supportedFeatures missing (req had it)");
        return RET_BAD_FORMAT;
    }

    /* O: 있으면 값만 검사 */
    if (rsp->has_gpsi && strncmp(rsp->gpsi, "msisdn-", 7) != 0) {
        LOG_ERR("gpsi prefix: %s", rsp->gpsi);
        return RET_BAD_FORMAT;
    }

    return RET_OK;
}

/* "100 Mbps" / "1.5 Gbps" / "64 Kbps" → bps */
int bitrate_from_str(const char *s, uint64_t *out_bps)
{
    double   val  = 0.0;
    char     unit[8];
    uint64_t mul  = 0;

    CHK_STR(s);
    CHK_PTR(out_bps);
    *out_bps = 0;
    memset(unit, 0x00, sizeof(unit));

    /* %7s 로 폭을 제한한다. 안 하면 스택 오버플로 */
    if (sscanf(s, "%lf %7s", &val, unit) != 2) {
        LOG_ERR("bitrate parse: %s", s);
        return RET_BAD_FORMAT;
    }
    if      (strcasecmp(unit, "bps")  == 0) mul = 1ULL;
    else if (strcasecmp(unit, "Kbps") == 0) mul = 1000ULL;
    else if (strcasecmp(unit, "Mbps") == 0) mul = 1000ULL * 1000;
    else if (strcasecmp(unit, "Gbps") == 0) mul = 1000ULL * 1000 * 1000;
    else { LOG_ERR("unknown unit: %s", unit); return RET_BAD_FORMAT; }

    if (val < 0.0 || val > 1.0e12) return RET_BAD_FORMAT;
    *out_bps = (uint64_t)(val * (double)mul);
    return RET_OK;
}


problem_t ret_to_problem(int ret)
{
    switch (ret) {
    case RET_OK:            return (problem_t){ 200, NULL };
    case RET_INVALID_ARG:   return (problem_t){ 400, "MANDATORY_IE_MISSING" };
    case RET_BAD_FORMAT:    return (problem_t){ 400, "INVALID_MSG_FORMAT" };
    case RET_NOT_FOUND:     return (problem_t){ 404, "USER_NOT_FOUND" };
    case RET_NOT_SUPPORTED: return (problem_t){ 501, "UNSUPPORTED_FEATURE" };
    case RET_TIMEOUT:       return (problem_t){ 504, "TIMEOUT" };
    default:                return (problem_t){ 500, "INTERNAL_ERROR" };
    }
}

int bitrate_to_str(uint64_t bps, char *buf, size_t n)
{
    static const struct { uint64_t mul; int digits; const char *unit; } T[] = {
        { 1000000000ULL, 9, "Gbps" }, { 1000000ULL, 6, "Mbps" }, { 1000ULL, 3, "Kbps" },
    };
    char   frac[16];
    int    len = 0;
    size_t i   = 0;

    CHK_PTR(buf);
    for (i = 0; i < sizeof(T) / sizeof(T[0]); i++) {
        if (bps >= T[i].mul) {
            uint64_t whole = bps / T[i].mul, rem = bps % T[i].mul;
            if (rem == 0) len = snprintf(buf, n, "%llu %s", (unsigned long long)whole, T[i].unit);
            else {
                snprintf(frac, sizeof(frac), "%0*llu", T[i].digits, (unsigned long long)rem);
                for (int k = (int)strlen(frac) - 1; k > 0 && frac[k] == '0'; k--) frac[k] = '\0';
                len = snprintf(buf, n, "%llu.%s %s", (unsigned long long)whole, frac, T[i].unit);
            }
            return (len < 0 || (size_t)len >= n) ? RET_TOO_SMALL : RET_OK;
        }
    }
    len = snprintf(buf, n, "%llu bps", (unsigned long long)bps);
    return (len < 0 || (size_t)len >= n) ? RET_TOO_SMALL : RET_OK;
}

static int all_digits(const char *s, size_t lo, size_t hi)
{
    size_t n = strlen(s), i = 0;
    if (n < lo || n > hi) return 0;
    for (i = 0; i < n; i++) if (!isdigit((unsigned char)s[i])) return 0;
    return 1;
}

int plmn_parse(const char *mcc, const char *mnc, plmn_t *out)
{
    CHK_PTR(mcc); CHK_PTR(mnc); CHK_PTR(out);
    memset(out, 0x00, sizeof(*out));
    if (!all_digits(mcc, 3, 3) || !all_digits(mnc, 2, 3)) return RET_BAD_FORMAT;
    snprintf(out->mcc, sizeof(out->mcc), "%s", mcc);
    snprintf(out->mnc, sizeof(out->mnc), "%s", mnc);
    out->mnc_len = (uint8_t)strlen(mnc);
    return RET_OK;
}

int plmn_to_strs(const plmn_t *p, char mcc[4], char mnc[4])
{
    CHK_PTR(p); CHK_PTR(mcc); CHK_PTR(mnc);
    snprintf(mcc, 4, "%s", p->mcc);
    snprintf(mnc, 4, "%s", p->mnc);
    return RET_OK;
}

int features_from_hex(const char *s, uint32_t *out)
{
    size_t   i = 0, n = 0;
    uint32_t v = 0;

    CHK_PTR(s); CHK_PTR(out);
    n = strlen(s);
    if (n == 0 || n > 8) return RET_BAD_FORMAT;
    for (i = 0; i < n; i++) {
        char c = s[i];
        uint32_t d = 0;
        if      (c >= '0' && c <= '9') d = (uint32_t)(c - '0');
        else if (c >= 'a' && c <= 'f') d = (uint32_t)(c - 'a' + 10);
        else if (c >= 'A' && c <= 'F') d = (uint32_t)(c - 'A' + 10);
        else return RET_BAD_FORMAT;
        v = (v << 4) | d;
    }
    *out = v;
    return RET_OK;
}

int features_to_hex(uint32_t v, char *buf, size_t n)
{
    int len = 0;
    CHK_PTR(buf);
    len = snprintf(buf, n, "%x", v);
    return (len < 0 || (size_t)len >= n) ? RET_TOO_SMALL : RET_OK;
}

uint32_t features_negotiate(uint32_t peer, uint32_t mine) { return peer & mine; }

/* ── 요청 URL ─────────────────────────────────────────── */
int am_req_parse(const char *url, am_data_req_t *out)
{
    static const char PRE[] = "/subscriber-data/", POST[] = "/am-data";
    const char *p = NULL, *end = NULL, *q = NULL;
    char        supi[SUPI_SIZE], query[256], *tok = NULL, *save = NULL;
    size_t      n = 0, i = 0;

    CHK_PTR(url); CHK_PTR(out);
    memset(out, 0x00, sizeof(*out));
    if (strncmp(url, PRE, sizeof(PRE) - 1) != 0) return RET_BAD_FORMAT;
    p   = url + sizeof(PRE) - 1;
    end = strstr(p, POST);
    if (end == NULL) return RET_BAD_FORMAT;
    n = (size_t)(end - p);
    if (n == 0) { LOG_ERR("supi empty"); return RET_INVALID_ARG; }
    if (n >= sizeof(supi)) return RET_BAD_FORMAT;
    memcpy(supi, p, n);  supi[n] = '\0';
    if (strncmp(supi, "imsi-", 5) != 0 || !all_digits(supi + 5, 15, 15)) return RET_BAD_FORMAT;
    if (n >= sizeof(out->supi)) return RET_BAD_FORMAT;
    snprintf(out->supi, sizeof(out->supi), "%s", supi);

    q = end + sizeof(POST) - 1;
    if (*q == '\0') return RET_OK;
    if (*q != '?') return RET_BAD_FORMAT;
    snprintf(query, sizeof(query), "%s", q + 1);
    for (tok = strtok_r(query, "&", &save); tok != NULL; tok = strtok_r(NULL, "&", &save)) {
        char *eq = strchr(tok, '=');
        if (eq == NULL) continue;
        *eq = '\0';
        if (strcmp(tok, "plmn-id") == 0) {
            char mcc[4], mnc[4];
            size_t len = strlen(eq + 1);
            if (!all_digits(eq + 1, 5, 6)) return RET_BAD_FORMAT;
            memcpy(mcc, eq + 1, 3);  mcc[3] = '\0';
            for (i = 0; i < len - 3; i++) mnc[i] = eq[1 + 3 + i];
            mnc[len - 3] = '\0';
            if (plmn_parse(mcc, mnc, &out->plmn) != RET_OK) return RET_BAD_FORMAT;
            out->has_plmn = true;
        } else if (strcmp(tok, "supported-features") == 0) {
            uint32_t v = 0;
            if (strlen(eq + 1) >= sizeof(out->features)) return RET_BAD_FORMAT;
            if (features_from_hex(eq + 1, &v) != RET_OK) return RET_BAD_FORMAT;
            snprintf(out->features, sizeof(out->features), "%s", eq + 1);
            out->has_features = true;
        }                                                  /* 모르는 키는 무시 */
    }
    return RET_OK;
}

int am_req_validate(const am_data_req_t *req, bool roaming)
{
    CHK_PTR(req);
    if (roaming && !req->has_plmn) { LOG_ERR("plmnId required when roaming"); return RET_INVALID_ARG; }
    return RET_OK;
}

/* ── 응답 JSON ────────────────────────────────────────── */
static int parse_sst_list(json_t *arr, uint16_t *dst, uint8_t *num, size_t maxn)
{
    size_t i = 0;
    if (!json_is_array(arr)) return RET_BAD_FORMAT;
    if (json_array_size(arr) > maxn) return RET_BAD_FORMAT;
    for (i = 0; i < json_array_size(arr); i++) {
        json_t *sst = json_object_get(json_array_get(arr, i), "sst");
        if (!json_is_integer(sst) || json_integer_value(sst) < 0 || json_integer_value(sst) > 255) return RET_BAD_FORMAT;
        dst[i] = (uint16_t)json_integer_value(sst);
    }
    *num = (uint8_t)json_array_size(arr);
    return RET_OK;
}

static int known_key(const char *k)
{
    static const char *K[] = { "gpsi", "subscribedUeAmbr", "nssai", "ratRestrictions", "supportedFeatures",
                               "accessType", "gnbInfo", "wlanInfo", "supi", "ueIpv4", "ueIpv6" };
    size_t i = 0;
    for (i = 0; i < sizeof(K) / sizeof(K[0]); i++) if (strcmp(k, K[i]) == 0) return 1;
    return 0;
}

int am_rsp_parse(json_t *root, am_data_rsp_t *out)
{
    json_t     *j = NULL, *nssai = NULL;
    const char *key = NULL;
    json_t     *val = NULL;
    size_t      i = 0;

    CHK_PTR(root); CHK_PTR(out);
    memset(out, 0x00, sizeof(*out));
    if (!json_is_object(root)) return RET_BAD_FORMAT;

    nssai = json_object_get(root, "nssai");
    if (nssai == NULL) { LOG_ERR("nssai missing"); return RET_INVALID_ARG; }
    if (!json_is_object(nssai)) return RET_BAD_FORMAT;
    if (parse_sst_list(json_object_get(nssai, "defaultSingleNssais"), out->nssai.default_sst,
                       &out->nssai.default_num, NSSAI_NUM_MAX) != RET_OK) return RET_BAD_FORMAT;
    j = json_object_get(nssai, "singleNssais");
    if (j != NULL && parse_sst_list(j, out->nssai.single_sst, &out->nssai.single_num, NSSAI_NUM_MAX) != RET_OK)
        return RET_BAD_FORMAT;

    j = json_object_get(root, "gpsi");
    if (j != NULL) {
        if (!json_is_string(j) || strlen(json_string_value(j)) >= GPSI_SIZE) return RET_BAD_FORMAT;
        snprintf(out->gpsi, sizeof(out->gpsi), "%s", json_string_value(j));
        out->has_gpsi = true;
    }
    j = json_object_get(root, "subscribedUeAmbr");
    if (j != NULL) {
        json_t *ul = json_object_get(j, "uplink"), *dl = json_object_get(j, "downlink");
        if (!json_is_string(ul) || !json_is_string(dl)) return RET_BAD_FORMAT;
        if (bitrate_from_str(json_string_value(ul), &out->ue_ambr.ul_bps) != RET_OK) return RET_BAD_FORMAT;
        if (bitrate_from_str(json_string_value(dl), &out->ue_ambr.dl_bps) != RET_OK) return RET_BAD_FORMAT;
        out->has_ue_ambr = true;
    }
    j = json_object_get(root, "ratRestrictions");
    if (j != NULL) {
        if (!json_is_array(j) || json_array_size(j) > RAT_NUM_MAX) return RET_BAD_FORMAT;
        for (i = 0; i < json_array_size(j); i++) {
            const char *s = json_string_value(json_array_get(j, i));
            if (s == NULL) return RET_BAD_FORMAT;
            out->rat[i] = strcmp(s, "NR") == 0 ? RAT_NR : strcmp(s, "EUTRA") == 0 ? RAT_EUTRA
                        : strcmp(s, "WLAN") == 0 ? RAT_WLAN : RAT_UNKNOWN;
        }
        out->rat_num = (uint8_t)json_array_size(j);
    }
    j = json_object_get(root, "supportedFeatures");
    if (j != NULL) {
        if (!json_is_string(j) || strlen(json_string_value(j)) >= sizeof(out->features)) return RET_BAD_FORMAT;
        snprintf(out->features, sizeof(out->features), "%s", json_string_value(j));
        out->has_features = true;
    }

    json_object_foreach(root, key, val) {                  /* 모르는 최상위 키: 원문 보관 */
        char *raw = NULL;
        if (known_key(key)) continue;
        if (out->unknown_num >= UNKNOWN_NUM_MAX) { LOG_WRN("drop unknown key %s", key); continue; }
        raw = json_dumps(val, JSON_COMPACT | JSON_ENCODE_ANY);
        if (raw == NULL) return RET_NO_MEM;
        if (strlen(raw) >= sizeof(out->unknown[0].raw) || strlen(key) >= sizeof(out->unknown[0].key)) {
            LOG_WRN("drop unknown key %s (too long)", key);
        } else {
            snprintf(out->unknown[out->unknown_num].key, sizeof(out->unknown[0].key), "%s", key);
            snprintf(out->unknown[out->unknown_num].raw, sizeof(out->unknown[0].raw), "%s", raw);
            out->unknown_num++;
        }
        free(raw);
    }
    return RET_OK;
}

int am_rsp_build(const am_data_rsp_t *in, json_t **out)
{
    int     ret = RET_FAIL;
    json_t *root = NULL;
    size_t  i = 0;

    CHK_PTR(in); CHK_PTR(out);
    *out = NULL;
    root = json_object();
    CHK_ALLOC_GOTO(root, ret, CLEANUP);

    if (in->has_gpsi) json_object_set_new(root, "gpsi", json_string(in->gpsi));
    if (in->has_ue_ambr) {
        char ul[32], dl[32];
        CHK_RET_GOTO(bitrate_to_str(in->ue_ambr.ul_bps, ul, sizeof(ul)), ret, CLEANUP);
        CHK_RET_GOTO(bitrate_to_str(in->ue_ambr.dl_bps, dl, sizeof(dl)), ret, CLEANUP);
        json_object_set_new(root, "subscribedUeAmbr", json_pack("{s:s,s:s}", "uplink", ul, "downlink", dl));
    }
    {
        json_t *d = json_array(), *s = json_array();
        for (i = 0; i < in->nssai.default_num; i++) json_array_append_new(d, json_pack("{s:i}", "sst", in->nssai.default_sst[i]));
        for (i = 0; i < in->nssai.single_num; i++)  json_array_append_new(s, json_pack("{s:i}", "sst", in->nssai.single_sst[i]));
        json_object_set_new(root, "nssai", json_pack("{s:o,s:o}", "defaultSingleNssais", d, "singleNssais", s));
    }
    if (in->rat_num > 0) {
        json_t *a = json_array();
        static const char *N[] = { "UNKNOWN", "NR", "EUTRA", "WLAN" };
        for (i = 0; i < in->rat_num; i++) json_array_append_new(a, json_string(N[in->rat[i] < RAT_END ? in->rat[i] : 0]));
        json_object_set_new(root, "ratRestrictions", a);
    }
    if (in->has_features) json_object_set_new(root, "supportedFeatures", json_string(in->features));
    for (i = 0; i < in->unknown_num; i++) {                /* echo */
        json_error_t e;
        json_t *v = json_loads(in->unknown[i].raw, JSON_DECODE_ANY, &e);
        CHK_PTR_GOTO(v, ret, CLEANUP);
        json_object_set_new(root, in->unknown[i].key, v);
    }
    *out = root;  root = NULL;
    ret = RET_OK;
CLEANUP:
    if (root != NULL) json_decref(root);
    return ret;
}

/* ── 배타 C / oneOf / anyOf ───────────────────────────── */
int access_info_parse(json_t *root, access_info_t *out)
{
    json_t     *j_gnb = NULL, *j_wlan = NULL;
    const char *s = NULL;

    CHK_PTR(root); CHK_PTR(out);
    memset(out, 0x00, sizeof(*out));
    j_gnb  = json_object_get(root, "gnbInfo");
    j_wlan = json_object_get(root, "wlanInfo");

    s = json_string_value(json_object_get(root, "accessType"));
    if (s == NULL) return RET_INVALID_ARG;

    if (strcmp(s, "3GPP_ACCESS") == 0) {
        json_t *id = NULL, *tac = NULL;
        if (j_gnb == NULL) { LOG_ERR("gnbInfo required when 3GPP_ACCESS"); return RET_INVALID_ARG; }
        if (j_wlan != NULL) LOG_WRN("wlanInfo ignored: 3GPP_ACCESS");
        id = json_object_get(j_gnb, "gnbId");  tac = json_object_get(j_gnb, "tac");
        if (!json_is_integer(id) || !json_is_integer(tac)) return RET_BAD_FORMAT;
        out->type = ACCESS_3GPP;
        out->u.gnb.gnb_id = (uint32_t)json_integer_value(id);
        out->u.gnb.tac    = (uint16_t)json_integer_value(tac);
    } else if (strcmp(s, "NON_3GPP_ACCESS") == 0) {
        const char *ssid = NULL, *bssid = NULL;
        if (j_wlan == NULL) { LOG_ERR("wlanInfo required when NON_3GPP_ACCESS"); return RET_INVALID_ARG; }
        if (j_gnb != NULL) LOG_WRN("gnbInfo ignored: NON_3GPP_ACCESS");
        ssid  = json_string_value(json_object_get(j_wlan, "ssid"));
        bssid = json_string_value(json_object_get(j_wlan, "bssid"));
        if (ssid == NULL || bssid == NULL || strlen(ssid) < 1 || strlen(ssid) > SSID_LEN_MAX
            || strlen(bssid) != BSSID_SIZE - 1) return RET_BAD_FORMAT;
        out->type = ACCESS_NON_3GPP;
        snprintf(out->u.wlan.ssid, sizeof(out->u.wlan.ssid), "%s", ssid);
        snprintf(out->u.wlan.bssid, sizeof(out->u.wlan.bssid), "%s", bssid);
    } else {
        return RET_BAD_FORMAT;
    }
    return RET_OK;
}

int ue_identity_parse(json_t *obj, ue_identity_t *out)
{
    json_t     *j_supi = NULL, *j_gpsi = NULL;
    int         n_hit = 0;
    const char *v = NULL;

    CHK_PTR(obj); CHK_PTR(out);
    memset(out, 0x00, sizeof(*out));
    j_supi = json_object_get(obj, "supi");
    j_gpsi = json_object_get(obj, "gpsi");
    n_hit  = (j_supi != NULL) + (j_gpsi != NULL);

    if (n_hit != 1) { LOG_ERR("ueIdentity oneOf violated: %d present", n_hit); return RET_BAD_FORMAT; }
    if (j_supi != NULL) {
        v = json_string_value(j_supi);
        if (v == NULL || strlen(v) >= SUPI_SIZE) return RET_BAD_FORMAT;
        snprintf(out->u.supi, sizeof(out->u.supi), "%s", v);
        out->kind = UEID_SUPI;
    } else {
        v = json_string_value(j_gpsi);
        if (v == NULL || strlen(v) >= GPSI_SIZE) return RET_BAD_FORMAT;
        snprintf(out->u.gpsi, sizeof(out->u.gpsi), "%s", v);
        out->kind = UEID_GPSI;
    }
    return RET_OK;
}

int ue_identity_build(const ue_identity_t *id, json_t *obj)
{
    CHK_PTR(id);
    CHK_PTR(obj);
    switch (id->kind) {
    case UEID_SUPI: return json_object_set_new(obj, "supi", json_string(id->u.supi)) == 0
                           ? RET_OK : RET_NO_MEM;
    case UEID_GPSI: return json_object_set_new(obj, "gpsi", json_string(id->u.gpsi)) == 0
                           ? RET_OK : RET_NO_MEM;
    default:        return RET_INVALID_ARG;                       /* ★ NONE 으로는 응답을 못 만든다 */
    }
}

int ue_ip_validate(const ue_ip_t *p)
{
    CHK_PTR(p);
    /* anyOf: "둘 중 하나 이상" — 0개만 위반, 둘 다는 정상 */
    if ((int)p->has_v4 + (int)p->has_v6 < 1) {
        LOG_ERR("ueIpv4/ueIpv6 anyOf violated: none present");
        return RET_BAD_FORMAT;
    }
    return RET_OK;
}
