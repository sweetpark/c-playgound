# HINT2 (막혔을 때만)
```
if from==NULL or to==NULL or from==to or amt<=0: -1
first, second = id 작은 쪽, 큰 쪽
lock(first); lock(second)
if from->bal >= amt: from->bal -= amt; to->bal += amt; rc = 0 else rc = -1
unlock(second); unlock(first)
return rc
```
