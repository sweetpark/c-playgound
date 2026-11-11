# HINT2 (막혔을 때만)
```
append(ps, s):
    if ps==NULL or *ps==NULL or s==NULL: -1
    b = *ps; slen = strlen(s); need = b->len + slen + 1
    if need > SBUF_MAX: -1
    if need > b->cap:
        tmp = realloc(b, sizeof *b + need); if !tmp: -1
        tmp->cap = need; b = tmp; *ps = b
    memcpy(b->data + b->len, s, slen + 1); b->len += slen; return 0
```
