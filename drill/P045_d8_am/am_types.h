#ifndef AM_TYPES_H
#define AM_TYPES_H
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define SUPI_SIZE      24
#define GPSI_SIZE      24
#define SSID_LEN_MAX   32
#define BSSID_SIZE     18
#define IPV4_STR_SIZE  16
#define IPV6_STR_SIZE  46
#define NSSAI_NUM_MAX   8
#define RAT_NUM_MAX     8
#define UNKNOWN_NUM_MAX 8

typedef enum { RAT_UNKNOWN = 0, RAT_NR, RAT_EUTRA, RAT_WLAN, RAT_END } rat_type_t;

typedef struct { char mcc[4]; char mnc[4]; uint8_t mnc_len; } plmn_t;
typedef struct { int status; const char *cause; } problem_t;

typedef enum { ACCESS_UNKNOWN = 0, ACCESS_3GPP, ACCESS_NON_3GPP, ACCESS_END } access_type_t;
typedef struct { uint32_t gnb_id; uint16_t tac; } gnb_info_t;
typedef struct { char ssid[SSID_LEN_MAX + 1]; char bssid[BSSID_SIZE]; } wlan_info_t;
typedef struct {
    access_type_t type;
    union { gnb_info_t gnb; wlan_info_t wlan; } u;
} access_info_t;

typedef enum { UEID_NONE = 0, UEID_SUPI, UEID_GPSI, UEID_END } ue_id_kind_t;
typedef struct {
    ue_id_kind_t kind;
    union { char supi[SUPI_SIZE]; char gpsi[GPSI_SIZE]; } u;
} ue_identity_t;

typedef struct {
    bool has_v4;  char v4[IPV4_STR_SIZE];
    bool has_v6;  char v6[IPV6_STR_SIZE];
} ue_ip_t;

typedef struct {
    char supi[SUPI_SIZE];
    bool has_plmn;      plmn_t plmn;
    bool has_features;  char features[17];
} am_data_req_t;

typedef struct {
    bool suspended;
    bool has_gpsi;      char gpsi[GPSI_SIZE];
    bool has_ue_ambr;   struct { uint64_t ul_bps, dl_bps; } ue_ambr;
    struct { uint8_t default_num; uint16_t default_sst[NSSAI_NUM_MAX];
             uint8_t single_num;  uint16_t single_sst[NSSAI_NUM_MAX]; } nssai;
    uint8_t rat_num;    rat_type_t rat[RAT_NUM_MAX];
    bool has_features;  char features[17];
    struct { char key[32]; char raw[256]; } unknown[UNKNOWN_NUM_MAX];
    uint8_t unknown_num;
    access_info_t access;
    ue_identity_t ue_id;
    ue_ip_t       ue_ip;
} am_data_rsp_t;
#endif
