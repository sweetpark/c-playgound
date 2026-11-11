# HINT2 (막혔을 때만)
```
int resolve_service_port_bad(const char *portStr)
{
    /* 1. return atoi(portStr)   ← 외부 문자열이 그대로 설정값 */
}

int resolve_service_port_good(int index)
{
    /* [ ] 1. 내부 테이블
     *        PORT_TABLE[] = { 21, 2121, 8021 }
     *        PORT_TABLE_SIZE = sizeof 로 계산 */

    /* [ ] 2. 인덱스 범위 검사
     *        index < 0 또는 (size_t)index >= PORT_TABLE_SIZE → return -1 */

    /* [ ] 3. return PORT_TABLE[index] */
}
```
