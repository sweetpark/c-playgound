# HINT2
```
feed: !has_last → FIRST,0
      value >= last → OK, value-last
      last > max - max/4 → WRAP, (max-last)+1+value
      else RESET, value
      last = value; has_last = 1
sum_check: d = (long)req - (long)(rsp+to+cancel+inflight); *diff = d; return d == 0
```
