#include <errno.h>
#include <string.h>

#include "drill_test.h"
#include "solution.c"

#define SENT 4321

static void ok(const char *s, unsigned short want)
{
	unsigned short out = SENT;
	CHECK(parse_port(s, &out) == 0);
	CHECK(out == want);
}

static void bad(const char *s)
{
	unsigned short out = SENT;
	CHECK(parse_port(s, &out) == -1);
	CHECK(out == SENT);          /* 실패하면 out 을 건드리지 않는다 */
}

int main(void)
{
	unsigned short out = SENT;

	ok("80", 80);
	ok("1", 1);
	ok("65535", 65535);
	ok("080", 80);               /* 10진수로 해석 (8진수 아님) */

	bad("0");
	bad("65536");
	bad("-1");                   /* strtoul 은 음수를 받아 거대한 값으로 래핑한다 */
	bad("+80");
	bad(" 80");
	bad("80 ");
	bad("80x");
	bad("");
	bad("abc");
	bad("99999999999999999999999");   /* ERANGE */
	bad("4294967376");           /* 2^32 + 80: unsigned int 로 자르면 80 이 되어 통과해 버린다 */
	bad("0x50");

	CHECK(parse_port(NULL, &out) == -1);
	CHECK(parse_port("80", NULL) == -1);
	CHECK(out == SENT);

	errno = ERANGE;              /* 남아 있는 errno 에 영향받지 않아야 한다 */
	ok("8080", 8080);

	return drill_report();
}
