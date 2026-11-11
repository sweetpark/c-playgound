# HINT2 (막혔을 때만)
```
if n<=0 or n>64 or out==NULL: -1
for i in 0..n: jobs[i] = {i, &out[i]}; if pthread_create(&th[i], f, &jobs[i]) fails: rc=-1; break; started++
for i in 0..started: pthread_join(th[i])
return rc
f(arg): *arg->slot = arg->i * arg->i
```
