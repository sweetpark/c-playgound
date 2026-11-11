#include <pthread.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#include "drill_test.h"
#include "solution.c"

static void on_alarm(int s)
{
	static const char m[] = "FAIL (timeout/deadlock)\n";
	(void)s;
	if (write(1, m, sizeof(m) - 1) < 0) { /* ignore */ }
	_exit(1);
}

static int bad_threads;

static void *thr(void *arg)
{
	const char *input = arg;           /* 리터럴: 수정하면 SIGSEGV */
	char out[8][16];
	int i;
	for (i = 0; i < 20000; i++) {
		int n = split_words(input, ",; ", out, 8);
		if (n != 3 || strncmp(out[0], input, 1) != 0) {
			__sync_fetch_and_add(&bad_threads, 1);
			break;
		}
	}
	return NULL;
}

int main(void)
{
	char out[8][16];
	int n;
	pthread_t t[4];
	int i;

	signal(SIGALRM, on_alarm);
	alarm(60);

	memset(out, 0, sizeof(out));
	n = split_words("alpha beta gamma", " ", out, 8);
	CHECK(n == 3);
	CHECK(strcmp(out[0], "alpha") == 0 && strcmp(out[1], "beta") == 0 && strcmp(out[2], "gamma") == 0);

	/* 구분자 여러 개, 연속 구분자와 앞뒤 구분자는 빈 토큰을 만들지 않는다 */
	n = split_words(",,a;b , c;;", ",; ", out, 8);
	CHECK(n == 3);
	CHECK(strcmp(out[0], "a") == 0 && strcmp(out[1], "b") == 0 && strcmp(out[2], "c") == 0);

	n = split_words("", " ", out, 8);
	CHECK(n == 0);
	n = split_words("   ", " ", out, 8);
	CHECK(n == 0);
	n = split_words("single", " ", out, 8);
	CHECK(n == 1 && strcmp(out[0], "single") == 0);

	/* 토큰 길이 경계: 15 글자는 되고 16 글자는 -2 */
	n = split_words("123456789012345 x", " ", out, 8);
	CHECK(n == 2 && strcmp(out[0], "123456789012345") == 0);
	n = split_words("1234567890123456 x", " ", out, 8);
	CHECK(n == -2);

	/* 최대 개수 경계: maxn 개는 되고 더 있으면 -3 */
	n = split_words("a b c", " ", out, 3);
	CHECK(n == 3);
	n = split_words("a b c d", " ", out, 3);
	CHECK(n == -3);

	/* 인자 오류 */
	CHECK(split_words(NULL, " ", out, 8) == -1);
	CHECK(split_words("a", NULL, out, 8) == -1);
	CHECK(split_words("a", " ", NULL, 8) == -1);
	CHECK(split_words("a", " ", out, 0) == -1);

	/* 입력 문자열은 바뀌지 않는다 */
	{
		char w[] = "x y z";
		split_words(w, " ", out, 8);
		CHECK(strcmp(w, "x y z") == 0);
	}

	/* 스레드 안전: strtok 의 내부 정적 상태를 공유하면 서로 망가뜨린다 */
	pthread_create(&t[0], NULL, thr, "a,b;c");
	pthread_create(&t[1], NULL, thr, "x;y z");
	pthread_create(&t[2], NULL, thr, "p q,r");
	pthread_create(&t[3], NULL, thr, "m,n;o");
	for (i = 0; i < 4; i++) pthread_join(t[i], NULL);
	CHECK(bad_threads == 0);

	return drill_report();
}
