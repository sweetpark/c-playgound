# HINT2
```
admit: w = now/1000
       if !started or w > window: started=1; window=w; count=0
       if count < limit: count++; admitted++; return 1
       rejected++; return 0

backoff: base<1 → 0 ; attempt<0 → 0 ; j를 0..100 으로 ; d = min(base, cap)
         for (i=0; i<attempt && d<cap; i++) d = (d > cap/2) ? cap : d*2
         lo = d*(100-j)/100 ; return lo + (d-lo)*(rnd%1001)/1000
```
