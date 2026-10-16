#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"
#include "log.h"

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

#define T_EQ_BYTES(_got, _exp, _n)                                      \
    do {                                                                \
        if (memcmp((_got), (_exp), (_n)) != 0) { g_fail++;              \
            printf("  X %s:%d  bytes differ\n", __func__, __LINE__); } \
    } while (0)
#include "tlv.h"


static void test_unknown_ie(void)   /* 모르는 IE 가 있어도 통과해야 한다 */
{
    const uint8_t raw[] = {
        0x01, 0x04, 0x00, 0x00, 0x30, 0x39,   /* SessionId = 12345 */
        0x99, 0x03, 0xAA, 0xBB, 0xCC,         /* 미지원 IEI */
        0x02, 0x01, 0x07,                     /* Cause = 7 */
    };
    msg_body_t b;

    T_EQ_INT(tlv_parse(raw, sizeof(raw), &b), RET_OK);   /* ★ OK 여야 한다 */
    T_EQ_INT(b.session_id, 12345);
    T_EQ_INT(b.cause, 7);
    T_EQ_INT(b.unknown_num, 1);
    T_EQ_INT(b.unknown[0].iei, 0x99);
    T_EQ_INT(b.unknown[0].len, 3);
}

static void test_truncated(void)    /* 길이가 거짓말을 할 때 */
{
    const uint8_t raw[] = { 0x10, 0x20, 0x41, 0x42 };   /* L=32 인데 값은 2바이트 */
    msg_body_t b;

    T_EQ_INT(tlv_parse(raw, sizeof(raw), &b), RET_BAD_FORMAT);
}

static void test_zero_len(void)     /* L=0 은 합법 */
{
    const uint8_t raw[] = {
        0x01, 0x04, 0x00, 0x00, 0x00, 0x01,
        0x10, 0x00,                            /* UeName, 길이 0 */
        0x02, 0x01, 0x00,
    };
    msg_body_t b;

    T_EQ_INT(tlv_parse(raw, sizeof(raw), &b), RET_OK);
    T_EQ_INT(b.has_name, 1);
    T_EQ_INT(b.name[0], '\0');
}

static void test_missing_mandatory(void)
{
    const uint8_t no_sid[]   = { 0x02, 0x01, 0x07 };                        /* SessionId 없음 */
    const uint8_t no_cause[] = { 0x01, 0x04, 0x00, 0x00, 0x00, 0x01 };      /* Cause 없음 */
    msg_body_t b;

    T_EQ_INT(tlv_parse(no_sid, sizeof(no_sid), &b), RET_BAD_FORMAT);
    T_EQ_INT(tlv_parse(no_cause, sizeof(no_cause), &b), RET_BAD_FORMAT);
    T_EQ_INT(tlv_parse((const uint8_t *)"", 0, &b), RET_BAD_FORMAT);        /* 빈 입력 */
}

static void test_length_mismatch(void)
{
    const uint8_t sid2[]   = { 0x01, 0x02, 0x00, 0x01,  0x02, 0x01, 0x00 };  /* SessionId L=2 */
    const uint8_t cause64[] = { 0x01, 0x04, 0, 0, 0, 1,  0x02, 0x01, 64 };   /* Cause 범위 초과 */
    msg_body_t b;

    T_EQ_INT(tlv_parse(sid2, sizeof(sid2), &b), RET_BAD_FORMAT);
    T_EQ_INT(tlv_parse(cause64, sizeof(cause64), &b), RET_BAD_FORMAT);
}

static void test_dup_and_overflow(void)
{
    const uint8_t dup[] = { 0x01, 0x04, 0, 0, 0, 1,  0x01, 0x04, 0, 0, 0, 2,  0x02, 0x01, 0x00 };
    uint8_t       raw[6 + 3 + 9 * 3];
    msg_body_t    b;
    size_t        off = 0;
    int           i = 0;

    T_EQ_INT(tlv_parse(dup, sizeof(dup), &b), RET_OK);
    T_EQ_INT(b.session_id, 1);                           /* ★ 첫 번째 채택 */

    memcpy(raw, "\x01\x04\x00\x00\x00\x01\x02\x01\x00", 9);  off = 9;
    for (i = 0; i < 9; i++) { raw[off++] = (uint8_t)(0x90 + i); raw[off++] = 1; raw[off++] = 0; }
    T_EQ_INT(tlv_parse(raw, off, &b), RET_OK);          /* 9개: 8개까지만 보관, 죽지 않는다 */
    T_EQ_INT(b.unknown_num, 8);
}

static void test_ext_len(void)       /* L = 0xFF → 뒤 2바이트가 실제 길이 */
{
    uint8_t    raw[6 + 4 + 300 + 3 + 4 + 128];
    msg_body_t b;
    size_t     off = 0;

    memcpy(raw, "\x01\x04\x00\x00\x00\x01", 6);  off = 6;
    raw[off++] = 0x99; raw[off++] = 0xFF; raw[off++] = 0x01; raw[off++] = 0x2C;   /* 300 */
    memset(raw + off, 0xEE, 300);  off += 300;
    raw[off++] = 0x98; raw[off++] = 0xFF; raw[off++] = 0x00; raw[off++] = 0x80;   /* 128 */
    memset(raw + off, 0xDD, 128);  off += 128;
    raw[off++] = 0x02; raw[off++] = 0x01; raw[off++] = 0x00;

    T_EQ_INT(tlv_parse(raw, off, &b), RET_OK);
    T_EQ_INT(b.has_cause, 1);                            /* ★ 확장 길이를 건너뛴 뒤에도 정렬이 맞다 */
    T_EQ_INT(b.unknown_num, 1);                          /* 300 은 버리고 128 은 보관 */
    T_EQ_INT(b.unknown[0].iei, 0x98);
    T_EQ_INT(b.unknown[0].len, 128);

    {
        const uint8_t cut_ext[] = { 0x99, 0xFF, 0x01 };  /* 확장 길이 자리가 모자람 */
        T_EQ_INT(tlv_parse(cut_ext, sizeof(cut_ext), &b), RET_BAD_FORMAT);
    }
}

static void test_nested(void)
{
    const uint8_t ok[]   = { 0x01, 0x04, 0, 0, 0, 1,  0x30, 0x03, 0x02, 0x01, 0x07 };       /* 내부에 Cause */
    const uint8_t deep[] = { 0x01, 0x04, 0, 0, 0, 1,  0x30, 0x05, 0x30, 0x03, 0x02, 0x01, 0x07 };
    msg_body_t b;

    T_EQ_INT(tlv_parse(ok, sizeof(ok), &b), RET_OK);
    T_EQ_INT(b.cause, 7);
    T_EQ_INT(tlv_parse(deep, sizeof(deep), &b), RET_BAD_FORMAT);   /* 깊이 2 */
}

static void test_cut_all(void)       /* 한 바이트씩 잘라 넣는다 — 크래시가 없어야 한다 */
{
    const uint8_t raw[] = {
        0x01, 0x04, 0x00, 0x00, 0x30, 0x39,
        0x99, 0x03, 0xAA, 0xBB, 0xCC,
        0x10, 0x02, 'a', 'b',
        0x02, 0x01, 0x07,
    };
    msg_body_t b;
    size_t     cut = 0;

    T_EQ_INT(tlv_parse(raw, sizeof(raw), &b), RET_OK);
    for (cut = 0; cut < sizeof(raw); cut++) {
        T_EQ_INT(tlv_parse(raw, cut, &b) != RET_OK, 1);      /* Cause 가 맨 끝이라 어디서 잘라도 실패 */
    }
}

static void test_build_and_roundtrip(void)
{
    msg_body_t    x, y;
    uint8_t       buf[256];
    size_t        n = 0;
    const uint8_t exp[] = { 0x01, 0x04, 0x00, 0x00, 0x00, 0x01,
                            0x02, 0x01, 0x07,
                            0x99, 0x03, 0xAA, 0xBB, 0xCC };

    memset(&x, 0x00, sizeof(x));
    x.session_id = 1;  x.has_session_id = true;
    x.cause = 7;       x.has_cause = true;
    x.unknown[0].iei = 0x99;  x.unknown[0].len = 3;
    x.unknown[0].val[0] = 0xAA;  x.unknown[0].val[1] = 0xBB;  x.unknown[0].val[2] = 0xCC;
    x.unknown_num = 1;

    T_EQ_INT(tlv_build(&x, buf, sizeof(buf), &n), RET_OK);
    T_EQ_INT(n, sizeof(exp));
    T_EQ_BYTES(buf, exp, sizeof(exp));                       /* ★ 알려진 IE → 모르는 IE 순서, 바이트 정확도 */

    T_EQ_INT(tlv_build(&x, buf, sizeof(exp) - 1, &n), RET_TOO_SMALL);    /* 한 칸 모자람 */
    T_EQ_INT(n, 0);

    T_EQ_INT(tlv_build(&x, buf, sizeof(buf), &n), RET_OK);
    T_EQ_INT(tlv_parse(buf, n, &y), RET_OK);                 /* 왕복 */
    T_EQ_INT(y.session_id, 1);
    T_EQ_INT(y.cause, 7);
    T_EQ_INT(y.unknown_num, 1);
    T_EQ_INT(y.unknown[0].iei, 0x99);
    T_EQ_BYTES(y.unknown[0].val, x.unknown[0].val, 3);
}

static void test_null(void)
{
    const uint8_t raw[] = { 0x01, 0x04, 0, 0, 0, 1,  0x02, 0x01, 0x00 };
    msg_body_t    b;
    uint8_t       buf[16];
    size_t        n = 0;

    T_EQ_INT(tlv_parse(NULL, 9, &b), RET_INVALID_ARG);
    T_EQ_INT(tlv_parse(raw, sizeof(raw), NULL), RET_INVALID_ARG);
    T_EQ_INT(tlv_build(NULL, buf, sizeof(buf), &n), RET_INVALID_ARG);
    T_EQ_INT(tlv_build(&b, NULL, sizeof(buf), &n), RET_INVALID_ARG);
    T_EQ_INT(tlv_build(&b, buf, sizeof(buf), NULL), RET_INVALID_ARG);
}

int main(void)
{
    test_unknown_ie();
    test_truncated();
    test_zero_len();
    test_missing_mandatory();
    test_length_mismatch();
    test_dup_and_overflow();
    test_ext_len();
    test_nested();
    test_cut_all();
    test_build_and_roundtrip();
    test_null();

    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
