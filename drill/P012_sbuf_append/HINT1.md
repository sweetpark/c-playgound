# HINT1
`sbuf_t *tmp = realloc(b, sizeof(*b) + need); if (!tmp) return -1;` 로 임시 변수에 받아 확인한 뒤에만 `b = tmp; *ps = b;`.
복사는 `memcpy(b->data + b->len, s, slen + 1)` (널 문자까지), 그다음 `b->len += slen`.
