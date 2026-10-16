#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include "getloginsim.h"

static int g_fail = 0;
#define T_TRUE(cond, msg) \
    do { if (!(cond)) { g_fail++; \
        printf("  X %s:%d %s\n", __func__, __LINE__, msg); } } while (0)

typedef struct { const char *name; const char *result_ptr; } bad_arg_t;
typedef struct { const char *name; char buf[64]; int rc; } good_arg_t;

static void *thread_bad(void *arg)
{
    bad_arg_t *a = (bad_arg_t *)arg;
    a->result_ptr = getlogin_like_bad(a->name);
    return NULL;
}

static void *thread_good(void *arg)
{
    good_arg_t *a = (good_arg_t *)arg;
    a->rc = getlogin_like_good_r(a->name, a->buf, sizeof(a->buf));
    return NULL;
}

static void test_bad_shares_buffer_across_threads(void)
{
    pthread_t t1, t2;
    bad_arg_t a1, a2;
    a1.name = "alice"; a1.result_ptr = NULL;
    a2.name = "bob";   a2.result_ptr = NULL;

    pthread_create(&t1, NULL, thread_bad, &a1);
    pthread_join(t1, NULL);
    pthread_create(&t2, NULL, thread_bad, &a2);
    pthread_join(t2, NULL);

    T_TRUE(a1.result_ptr == a2.result_ptr,
        "Bad는 두 호출이 같은 정적 버퍼 주소를 반환해야 한다(스레드 간 공유 재현)");
    T_TRUE(strcmp(a1.result_ptr, "bob") == 0,
        "정적 버퍼가 공유되므로, 나중 호출(bob)이 먼저 저장해둔 포인터(alice)가 가리키는 내용까지 덮어써야 한다(취약점 재현)");
}

static void test_good_isolates_each_caller(void)
{
    pthread_t t1, t2;
    good_arg_t a1, a2;
    a1.name = "alice";
    a2.name = "bob";

    pthread_create(&t1, NULL, thread_good, &a1);
    pthread_join(t1, NULL);
    pthread_create(&t2, NULL, thread_good, &a2);
    pthread_join(t2, NULL);

    T_TRUE(a1.rc == 0 && a2.rc == 0, "두 호출 모두 성공해야 한다");
    T_TRUE(strcmp(a1.buf, "alice") == 0,
        "Good은 호출자 각자의 버퍼에 쓰므로, 나중 호출(bob) 이후에도 alice의 결과가 그대로 남아야 한다");
    T_TRUE(strcmp(a2.buf, "bob") == 0, "bob의 결과도 정확해야 한다");
}

int main(void)
{
    test_bad_shares_buffer_across_threads();
    test_good_isolates_each_caller();
    printf(g_fail ? "FAIL %d\n" : "PASS\n", g_fail);
    return g_fail ? 1 : 0;
}
