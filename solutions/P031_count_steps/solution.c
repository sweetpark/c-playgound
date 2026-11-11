#include <stddef.h>
#include <stdint.h>

/* P031 count_steps — 모범답안 */
size_t count_steps(size_t start, size_t end, size_t step)
{
	size_t n;

	if (step == 0 || start > end)
		return 0;
	n = (end - start) / step;      /* 증가시키기 전에 남은 범위로 계산 (루프를 돌리지 않는다) */
	if (n == SIZE_MAX)
		return SIZE_MAX;           /* n + 1 이 표현 불가: 포화 */
	return n + 1;
}
