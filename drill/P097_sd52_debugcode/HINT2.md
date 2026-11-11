# HINT2 (막혔을 때만)
```
void debug_output_bad(void)
{
    /* 1. puts("Debug message")   ← 배포본에도 조건 없이 항상 실행 */
}

void debug_output_good(void)
{
    /* [ ] 1. 헤더에 DEBUG_LOG 매크로 두 갈래
     *        #ifdef DEBUG_ENABLE
     *            #define DEBUG_LOG(message)  do { puts(message); } while (0)
     *        #else
     *            #define DEBUG_LOG(message)  do { } while (0)    ← 배포: 아무 일도 안 함
     *        #endif */

    /* [ ] 2. DEBUG_LOG("Debug message")   ← puts 대신 이 한 단어만 바뀐다 */
}
```
