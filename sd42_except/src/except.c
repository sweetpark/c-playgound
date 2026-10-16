#include <string.h>
#include "except.h"

/* 진짜 fgets() 대신, "읽기 실패로 널 종료가 안 된 상태"를 결정론적으로 재현하는 내부 헬퍼 */
static char *mock_fgets(char *buf, size_t size, int simulate_fail)
{
    if (simulate_fail) {
        memset(buf, 'A', size);      /* 널 바이트 없이 버퍼를 가득 채움 -- 실제 fgets 실패를 흉내 */
        return NULL;                  /* fgets가 실패하면 NULL을 반환한다 */
    }
    strncpy(buf, "hi", size);
    return buf;
}

void copy_input_bad(char *from_buf, size_t from_size, int simulate_read_fail,
                     char *to_buf, size_t to_size)
{
    (void)to_size;
    mock_fgets(from_buf, from_size, simulate_read_fail);   /* 반환값을 검사하지 않는다 */
    strcpy(to_buf, from_buf);                               /* 널 종료가 안 되면 오버플로우 */
}

int copy_input_good(char *from_buf, size_t from_size, int simulate_read_fail,
                     char *to_buf, size_t to_size)
{
    char *ret = mock_fgets(from_buf, from_size, simulate_read_fail);
    if (ret != from_buf) return -1;                         /* 함수 호출 이후 결과 값을 비교 */
    if (to_size <= strlen(from_buf)) return -1;
    strcpy(to_buf, from_buf);
    return 0;
}
