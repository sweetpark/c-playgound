# HINT2 (막혔을 때만)
```
take_ptr: CHK_PTR(s); CHK_RANGE(v,0,100); LOG_INF(...); return RET_OK
take_res: ret = RET_FAIL; buf = NULL; fp = NULL
          CHK_PTR(path)                                  # 아직 자원 없음 -> 즉시 return
          buf = calloc(1,64); CHK_PTR_GOTO(buf, ret, CLEANUP)
          fp = fopen(path,"r"); CHK_COND_GOTO(fp != NULL, RET_IO_ERROR, ret, CLEANUP)
          CHK_RET_GOTO(take_ptr("inner",10), ret, CLEANUP)
          ret = RET_OK
          CLEANUP: SAFE_FCLOSE(fp); SAFE_FREE(buf); return ret
```
