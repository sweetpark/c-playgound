#ifndef AM_API_H
#define AM_API_H
#include <jansson.h>
#include "am_types.h"

int  bitrate_from_str(const char *s, uint64_t *out_bps);
int  bitrate_to_str(uint64_t bps, char *buf, size_t n);
int  plmn_parse(const char *mcc, const char *mnc, plmn_t *out);
int  plmn_to_strs(const plmn_t *p, char mcc[4], char mnc[4]);
problem_t ret_to_problem(int ret);
int  features_from_hex(const char *s, uint32_t *out);
int  features_to_hex(uint32_t v, char *buf, size_t n);
uint32_t features_negotiate(uint32_t peer, uint32_t mine);
int  am_req_parse(const char *url, am_data_req_t *out);
int  am_req_validate(const am_data_req_t *req, bool roaming);
int  am_rsp_parse(json_t *root, am_data_rsp_t *out);
int  am_rsp_build(const am_data_rsp_t *in, json_t **out);
int  am_data_rsp_validate(const am_data_rsp_t *rsp, const am_data_req_t *req);
int  access_info_parse(json_t *root, access_info_t *out);
int  ue_identity_parse(json_t *obj, ue_identity_t *out);
int  ue_identity_build(const ue_identity_t *id, json_t *obj);
int  ue_ip_validate(const ue_ip_t *p);
#endif
