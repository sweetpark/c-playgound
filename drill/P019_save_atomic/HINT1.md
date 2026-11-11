# HINT1
`fopen(tmp,"w")` → `fwrite` → `fflush` → `fsync(fileno(fp))` → `fclose` → `rename(tmp, path)`. 각 단계가 실패하면 `remove(tmp)` 하고 `-1`.
