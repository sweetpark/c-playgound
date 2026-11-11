# HINT2 (막혔을 때만)
```
level_name: switch with default -> "UNKNOWN"
level_parse: s==NULL -> -1; loop i in 0..LV_COUNT: strcasecmp(s, level_name(i))==0 -> i; else -1
level_names: out==NULL or cap < LV_COUNT -> 0; fill out[i]=level_name(i); return LV_COUNT
```
