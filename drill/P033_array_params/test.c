#include <stdlib.h>
#include <string.h>

#include "drill_test.h"
#include "solution.c"

int main(void)
{
	int a[5] = {3, -1, 9, 4, 9};
	int one[1] = {-7};
	int d[5];
	int *hd;
	int *hs;
	int mx = 0;
	size_t i;
	int same;

	/* sum_arr */
	CHECK(sum_arr(5, a) == 24);
	CHECK(sum_arr(1, one) == -7);
	CHECK(sum_arr(0, a) == 0);
	CHECK(sum_arr(3, NULL) == 0);       /* NULL 이면 0 */
	{
		int big[2] = {2147483647, 2147483647};
		CHECK(sum_arr(2, big) == 4294967294L);   /* int 로 누적하면 오버플로 */
	}

	/* max_arr */
	CHECK(max_arr(5, a, &mx) == 0 && mx == 9);
	mx = 123;
	CHECK(max_arr(0, a, &mx) == -1 && mx == 123);       /* 빈 배열: 최댓값 없음 */
	CHECK(max_arr(5, NULL, &mx) == -1);
	CHECK(max_arr(5, a, NULL) == -1);
	CHECK(max_arr(1, one, &mx) == 0 && mx == -7);       /* 모두 음수여도 0 으로 시작하면 안 된다 */

	/* copy_arr: 대상 크기가 원본보다 작으면 거절하고 대상은 그대로 */
	memset(d, 0x55, sizeof(d));
	CHECK(copy_arr(5, d, 5, a) == 0);
	same = 1;
	for (i = 0; i < 5; i++)
		if (d[i] != a[i]) same = 0;
	CHECK(same);
	memset(d, 0x55, sizeof(d));
	CHECK(copy_arr(4, d, 5, a) == -1);
	CHECK(d[0] == 0x55555555 && d[3] == 0x55555555);
	CHECK(copy_arr(5, NULL, 5, a) == -1);
	CHECK(copy_arr(5, d, 5, NULL) == -1);
	CHECK(copy_arr(5, d, 0, a) == 0);                   /* 0 개 복사는 성공 */

	/* 정확한 크기의 힙 배열: 경계 밖을 읽거나 쓰면 ASan */
	hs = malloc(5 * sizeof(int));
	hd = malloc(5 * sizeof(int));
	if (hs && hd) {
		for (i = 0; i < 5; i++) hs[i] = (int)i + 1;
		CHECK(copy_arr(5, hd, 5, hs) == 0);
		CHECK(sum_arr(5, hd) == 15);
		CHECK(max_arr(5, hd, &mx) == 0 && mx == 5);
	}
	free(hs);
	free(hd);

	return drill_report();
}
