/* 드릴 채점용 미니 테스트 헬퍼.
 * CHECK 는 assert 와 달리 실패해도 멈추지 않고 세어서, 마지막에 'PASS n/m' 또는 'FAIL n/m' 을 출력한다.
 * (메모리 오류는 ASan/UBSan 이 즉시 프로세스를 죽이므로 그 경우는 n/m 이 출력되지 않고 새니타이저 보고가 나온다.) */
#ifndef DRILL_TEST_H
#define DRILL_TEST_H

#include <stdio.h>

static int g_total, g_pass;

#define CHECK(cond)                                                          \
	do {                                                                     \
		g_total++;                                                           \
		if (cond) g_pass++;                                                  \
		else printf("  x %s:%d  %s\n", __FILE__, __LINE__, #cond);           \
	} while (0)

static int drill_report(void)
{
	int ok = (g_pass == g_total);
	printf("%s %d/%d\n", ok ? "PASS" : "FAIL", g_pass, g_total);
	fflush(stdout); /* 종료 시 LeakSanitizer 가 _exit 하면 버퍼가 사라지므로 미리 내보낸다 */
	return ok ? 0 : 1;
}

#endif
