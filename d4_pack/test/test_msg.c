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

#define T_EQ_BYTES(_got, _exp, _n)                                      \
    do {                                                                \
        if (memcmp((_got), (_exp), (_n)) != 0) { g_fail++;              \
            printf("  X %s:%d  bytes differ\n", __func__, __LINE__); } \
    } while (0)
#include "msg.h"

#include "msg.h"

static void test_roundtrip(void)
{
    int i = 0;

    for (i = 0; i < 1000; i++) {
        msg_t    a, b;
        uint8_t  buf[2048];
        size_t   len = 0, used = 0;

        memset(&a, 0x00, sizeof(a));
        memset(&b, 0x00, sizeof(b));

        do { a.ver = (uint8_t)(rand() % 8); } while (a.ver == 2);   /* Ver=2 는 변형 C 의 확장 */
        a.type        = (uint8_t)(rand() % 32);
        a.flags       = (uint8_t)(rand() % 256);      /* 모르는 비트 포함 */
        a.prio        = (uint8_t)(rand() % 16);
        a.session_id  = (uint32_t)rand();
        a.seq         = (uint32_t)(rand() % 0x1000000);
        a.payload_len = (uint16_t)(rand() % 1401);
        for (size_t k = 0; k < a.payload_len; k++) a.payload[k] = (uint8_t)(rand() % 256);

        T_EQ_INT(msg_pack(&a, buf, sizeof(buf), &len), RET_OK);
        T_EQ_INT(msg_unpack(buf, len, &b, &used), RET_OK);
        T_EQ_INT(used, len);

        /* 구조체 memcmp 금지 — 패딩 때문에 실패한다. 필드별로 비교 */
        T_EQ_INT(b.ver, a.ver);   T_EQ_INT(b.type, a.type);
        T_EQ_INT(b.flags, a.flags);   /* ★ 모르는 비트가 살아있는지 */
        T_EQ_INT(b.prio, a.prio);
        T_EQ_INT(b.session_id, a.session_id);
        T_EQ_INT(b.seq, a.seq);
        T_EQ_INT(b.payload_len, a.payload_len);
        T_EQ_INT(memcmp(b.payload, a.payload, a.payload_len), 0);
    }
}

static void test_edge(void)
{
    msg_t   m = { .ver = 1, .type = 2, .payload_len = 0 };
    uint8_t buf[16];
    size_t  len = 0;

    /* 버퍼가 1바이트 모자라면 TOO_SMALL — 절대 쓰지 말고 */
    T_EQ_INT(msg_pack(&m, buf, 13, &len), RET_TOO_SMALL);
    T_EQ_INT(msg_pack(&m, buf, 14, &len), RET_OK);   /* 12 + 0 + 2 */

    /* 잘린 수신 버퍼는 전부 BAD_FORMAT/TOO_SMALL, 절대 크래시 아님 */
    for (size_t cut = 0; cut < len; cut++) {
        msg_t  out;
        size_t used = 0;
        int    ret  = msg_unpack(buf, cut, &out, &used);
        T_EQ_INT(ret != RET_OK, 1);
    }

    /* Reserved 오염 */
    buf[11] |= 0x0F;
    { msg_t o; size_t u = 0; T_EQ_INT(msg_unpack(buf, len, &o, &u), RET_BAD_FORMAT); }
}
/* ── 엔디안 헬퍼: 바이트 위치까지 확인한다 ─────────────────────────── */
static void test_be(void)
{
    uint8_t b[8] = {0};

    wr_u16be(b, 0x1234);
    T_EQ_INT(b[0], 0x12);  T_EQ_INT(b[1], 0x34);
    T_EQ_INT(rd_u16be(b), 0x1234);

    wr_u24be(b, 0xABCDEF);
    T_EQ_INT(b[0], 0xAB);  T_EQ_INT(b[1], 0xCD);  T_EQ_INT(b[2], 0xEF);
    T_EQ_INT(rd_u24be(b), 0xABCDEF);

    wr_u32be(b, 0x01020304);
    T_EQ_INT(b[0], 0x01);  T_EQ_INT(b[3], 0x04);
    T_EQ_INT(rd_u32be(b), 0x01020304);

    memset(b, 0xFF, sizeof(b));
    T_EQ_INT(rd_u32be(b), 0xFFFFFFFFLL);              /* ★ 부호 확장 방지: (uint32_t) 로 올려서 시프트 */
}

/* ── 체크섬: RFC 1071 의 예제 ────────────────────────────────────── */
static void test_csum(void)
{
    const uint8_t rfc[8] = { 0x00, 0x01, 0xF2, 0x03, 0xF4, 0xF5, 0xF6, 0xF7 };
    const uint8_t odd[1] = { 0x01 };

    T_EQ_INT(inet_csum(rfc, 8), 0x220D);
    T_EQ_INT(inet_csum(odd, 1), 0xFEFF);              /* 홀수 바이트: 0x0100 의 보수 */
}

/* ── 바이트 정확도: 규격 그림에서 손으로 계산한 기대값 ───────────────── */
static void test_vector(void)
{
    msg_t   m;
    uint8_t buf[32];
    size_t  n = 0;
    /* ver=1 type=2 → 0x22 / flags=0x85 / len=0 / session=01020304 / seq=0A0B0C / prio=9 → 0x90
     * 체크섬 = ~(2285+0000+0102+0304+0A0B+0C90) = ~3D26 = C2D9 */
    const uint8_t exp[14] = { 0x22, 0x85, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04,
                              0x0A, 0x0B, 0x0C, 0x90, 0xC2, 0xD9 };

    memset(&m, 0x00, sizeof(m));
    m.ver = 1;  m.type = 2;  m.flags = 0x85;  m.prio = 9;
    m.session_id = 0x01020304;  m.seq = 0x0A0B0C;

    T_EQ_INT(msg_pack(&m, buf, sizeof(buf), &n), RET_OK);
    T_EQ_INT(n, 14);
    T_EQ_BYTES(buf, exp, 14);                         /* ★ 왕복이 못 잡는 오류를 잡는다 */
}

static void test_errors(void)
{
    msg_t   m, o;
    uint8_t buf[2048];
    size_t  n = 0, u = 0;

    memset(&m, 0x00, sizeof(m));
    m.ver = 1;  m.type = 2;

    T_EQ_INT(msg_pack(&m, buf, sizeof(buf), &n), RET_OK);

    m.ver = 8;   T_EQ_INT(msg_pack(&m, buf, sizeof(buf), &n), RET_INVALID_ARG);   m.ver = 1;
    m.type = 32; T_EQ_INT(msg_pack(&m, buf, sizeof(buf), &n), RET_INVALID_ARG);   m.type = 2;
    m.prio = 16; T_EQ_INT(msg_pack(&m, buf, sizeof(buf), &n), RET_INVALID_ARG);   m.prio = 0;
    m.seq = 0x1000000;       T_EQ_INT(msg_pack(&m, buf, sizeof(buf), &n), RET_BAD_FORMAT);   m.seq = 0;
    m.payload_len = 1401;    T_EQ_INT(msg_pack(&m, buf, sizeof(buf), &n), RET_BAD_FORMAT);
    T_EQ_INT(n, 0);                                   /* ★ 실패하면 out_len 은 0 */
    m.payload_len = 1400;
    T_EQ_INT(msg_pack(&m, buf, sizeof(buf), &n), RET_OK);                         /* 상한 */

    m.payload_len = 0;
    msg_pack(&m, buf, sizeof(buf), &n);
    buf[n - 1] ^= 0x01;                                                           /* 체크섬 오염 */
    T_EQ_INT(msg_unpack(buf, n, &o, &u), RET_BAD_FORMAT);
    T_EQ_INT(u, 0);

    wr_u16be(buf + 2, 1401);                                                      /* 길이 필드 거짓말 */
    T_EQ_INT(msg_unpack(buf, n, &o, &u), RET_BAD_FORMAT);

    T_EQ_INT(msg_pack(NULL, buf, sizeof(buf), &n), RET_INVALID_ARG);
    T_EQ_INT(msg_pack(&m, NULL, sizeof(buf), &n), RET_INVALID_ARG);
    T_EQ_INT(msg_pack(&m, buf, sizeof(buf), NULL), RET_INVALID_ARG);
    T_EQ_INT(msg_unpack(NULL, 14, &o, &u), RET_INVALID_ARG);
    T_EQ_INT(msg_unpack(buf, 14, NULL, &u), RET_INVALID_ARG);
    T_EQ_INT(msg_unpack(buf, 14, &o, NULL), RET_INVALID_ARG);
}

int main(void)
{
    test_be();
    test_csum();
    test_vector();
    test_roundtrip();
    test_edge();
    test_errors();

    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
