# HINT2 (막혔을 때만)
```
conn_reset(c):      if c==NULL return;  memset(c, 0, sizeof *c);  c->fd = -1
conn_reset_all:     if arr==NULL return;  for i in 0..n: conn_reset(&arr[i])
conn_init(c,h,p):   if c==NULL or h==NULL: return -1
                    conn_reset(c); snprintf(c->host, sizeof c->host, "%s", h); c->port = p; return 0
```
