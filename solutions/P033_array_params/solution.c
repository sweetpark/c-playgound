#include <stddef.h>
#include <string.h>

/* P033 array_params — 모범답안 */

long sum_arr(size_t n, const int a[n])
{
	long s = 0;
	size_t i;

	if (a == NULL)
		return 0;
	for (i = 0; i < n; i++)
		s += a[i];                  /* long 으로 누적 */
	return s;
}

int max_arr(size_t n, const int a[n], int *out)
{
	size_t i;
	int m;

	if (n == 0 || a == NULL || out == NULL)
		return -1;
	m = a[0];                       /* 0 이 아니라 첫 원소로 시작 */
	for (i = 1; i < n; i++)
		if (a[i] > m)
			m = a[i];
	*out = m;
	return 0;
}

int copy_arr(size_t dn, int dst[dn], size_t sn, const int src[sn])
{
	if (dst == NULL || src == NULL)
		return -1;
	if (dn < sn)                    /* 대상이 작으면 거절 (잘라 넣지 않는다) */
		return -1;
	if (sn > 0)
		memcpy(dst, src, sn * sizeof(src[0]));
	return 0;
}
