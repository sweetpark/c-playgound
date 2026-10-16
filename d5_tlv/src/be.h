#ifndef BE_H
#define BE_H
#include <stdint.h>
/* ── 엔디안: 바이트 단위로 직접. htons 를 쓰지 않는다 ─────── */
static inline uint16_t rd_u16be(const uint8_t *b)
{ return (uint16_t)(((uint16_t)b[0] << 8) | (uint16_t)b[1]); }

static inline uint32_t rd_u24be(const uint8_t *b)
{ return ((uint32_t)b[0] << 16) | ((uint32_t)b[1] << 8) | (uint32_t)b[2]; }

static inline uint32_t rd_u32be(const uint8_t *b)
{ return ((uint32_t)b[0] << 24) | ((uint32_t)b[1] << 16)
       | ((uint32_t)b[2] <<  8) |  (uint32_t)b[3]; }

static inline void wr_u16be(uint8_t *b, uint16_t v)
{ b[0] = (uint8_t)(v >> 8); b[1] = (uint8_t)(v); }

static inline void wr_u24be(uint8_t *b, uint32_t v)
{ b[0] = (uint8_t)(v >> 16); b[1] = (uint8_t)(v >> 8); b[2] = (uint8_t)(v); }

static inline void wr_u32be(uint8_t *b, uint32_t v)
{ b[0] = (uint8_t)(v >> 24); b[1] = (uint8_t)(v >> 16);
  b[2] = (uint8_t)(v >>  8); b[3] = (uint8_t)(v); }

#endif
