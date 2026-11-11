# HINT2 (막혔을 때만)
```
encode: NULL/size<8 -> -1; rc = validate(); rc!=0 -> rc
        buf[0]=ver buf[1]=type buf[2..3]=len(BE) buf[4..7]=id(BE); return 8
decode: NULL/size<8 -> -1
        ver=buf[0]; type=buf[1]; len=(buf[2]<<8)|buf[3]; id=(b4<<24)|(b5<<16)|(b6<<8)|b7  (uint32_t 로 올려서)
        rc = validate(); rc!=0 -> rc;  *h = {...}; 0
```
