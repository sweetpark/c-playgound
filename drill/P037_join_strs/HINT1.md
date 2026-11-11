# HINT1
`pos = 0`; 각 항목마다 `n = snprintf(out + pos, outsize - pos, "%s%s", i ? sep : "", items[i]); if (n < 0 || (size_t)n >= outsize - pos) { out[0] = 0; return -2; } pos += n;`.
