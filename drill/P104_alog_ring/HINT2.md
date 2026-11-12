# HINT2 (막혔을 때만)
```
consumer():
  loop:
    lock
    while count == 0 and not stop: wait(cv)
    if count == 0: unlock; break            # stop 이고 다 비웠다
    line = q[head]; q[head] = NULL; head = (head+1) % cap; count--; deq++
    if full and cap - count >= max(1, cap/10): full = 0
    unlock
    sink(ctx, line)                         # 락 밖
    free(line)

write(a, line):
  if a == NULL or line == NULL: return BADARG
  copy = strdup(line)
  lock
  if count == cap: dropped++; full = 1; unlock; free(copy); return FULL
  q[(head + count) % cap] = copy; count++; enq++
  if count == cap: full = 1
  signal(cv); unlock; return OK

destroy(a):
  lock; stop = 1; broadcast(cv); unlock
  join(thread)                              # 소비 스레드가 남은 줄을 다 내보내고 끝난다
  destroy mutex/cond; free(q); free(a)
```
