#include <stddef.h>
#include <string.h>

/* P033 array_params — 요구사항은 problem.md
 * 크기 인자를 배열 매개변수보다 앞에 선언해 [n] 으로 연결한다 (conformant 배열 매개변수). */

long sum_arr(size_t n, const int a[n])
{
	(void)n;
	(void)a;
	/* TODO */
	return -1;
}

int max_arr(size_t n, const int a[n], int *out)
{
	(void)n;
	(void)a;
	(void)out;
	/* TODO */
	return -2;
}

int copy_arr(size_t dn, int dst[dn], size_t sn, const int src[sn])
{
	(void)dn;
	(void)dst;
	(void)sn;
	(void)src;
	/* TODO */
	return -2;
}
