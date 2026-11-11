# HINT2 (막혔을 때만)
```
tmp = path + ".tmp"   (snprintf, 잘림 검사)
fp = fopen(tmp, "w"); if !fp: -1
if fwrite != len or fflush != 0 or fsync(fileno(fp)) != 0: fclose; remove(tmp); -1
if fclose(fp) != 0: remove(tmp); -1
if rename(tmp, path) != 0: remove(tmp); -1
0
```
