# HINT2 (막혔을 때만)
```
if step == NULL: return EINVAL
rc = 0
if step(ctx) != 0:
    saved = errno               # 바로 저장
    rc = saved ? saved : EIO
if cleanup: cleanup(ctx)        # errno 가 바뀔 수 있다
if rc: errno = rc
return rc
```
