# HINT2 (막혔을 때만)
```
while n > 0:
    w = write(fd, p, n)
    if w > 0: p += w; n -= w; continue
    if w < 0 and errno == EINTR: continue
    if w < 0 and errno in (EAGAIN, EWOULDBLOCK):
        pr = poll({fd, POLLOUT}, 1, 10000); if pr > 0 or (pr < 0 and errno == EINTR): continue; return -1
    return -1
return 0
```
