# HINT2 (막혔을 때만)
```
block signo, save old mask
ts = {timeout_ms/1000, (timeout_ms%1000)*1e6}
do r = sigtimedwait(&set, NULL, &ts) while r<0 and errno==EINTR
result = r>=0 ? 1 : (errno==EAGAIN ? 0 : -1)
restore old mask (errno 보존); return result
```
