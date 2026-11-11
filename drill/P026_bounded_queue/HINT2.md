# HINT2 (막혔을 때만)
```
push: lock
      while count==CAP and !closed: wait(not_full)
      if closed: unlock; return -1
      buf[(head+count)%CAP]=v; count++; signal(not_empty); unlock; 0
pop:  lock
      while count==0 and !closed: wait(not_empty)
      if count==0: unlock; return -1
      *out=buf[head]; head=(head+1)%CAP; count--; signal(not_full); unlock; 0
close: lock; closed=1; broadcast(not_empty); broadcast(not_full); unlock
```
