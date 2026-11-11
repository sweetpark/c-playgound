#include <stdlib.h>
#include <string.h>

#include "drill_test.h"
#include "solution.c"

#define PAD 0x7E

/* data 의 n 바이트를 담은 임시 파일을 만들어 처음으로 되감는다 */
static FILE *mkfile(const char *data, size_t n)
{
	FILE *fp = tmpfile();
	if (!fp) return NULL;
	if (n) fwrite(data, 1, n, fp);
	rewind(fp);
	return fp;
}
#define MKFILE(s) mkfile((s), sizeof(s) - 1)

static int eq(const char *buf, size_t cap, const char *expect)
{
	return memchr(buf, '\0', cap) != NULL && strcmp(buf, expect) == 0;
}

/* buf[size..] 가 PAD 그대로인가 (경계 밖 쓰기 검출) */
static int rest_pad(const char *buf, size_t size, size_t total)
{
	size_t i;
	for (i = size; i < total; i++)
		if ((unsigned char)buf[i] != PAD) return 0;
	return 1;
}

int main(void)
{
	char buf[16];
	FILE *fp;
	int r;

	/* 여러 줄을 차례로 */
	fp = MKFILE("abc\nde\n");
	CHECK(fp != NULL);
	if (fp) {
		memset(buf, PAD, sizeof(buf));
		r = read_line(fp, buf, 8);
		CHECK(r == 0);
		CHECK(eq(buf, 8, "abc"));
		CHECK(rest_pad(buf, 8, sizeof(buf)));
		r = read_line(fp, buf, 8);
		CHECK(r == 0);
		CHECK(eq(buf, 8, "de"));
		r = read_line(fp, buf, 8);
		CHECK(r == -1);                 /* 더 읽을 문자 없음 */
		CHECK(eq(buf, 8, ""));
		fclose(fp);
	}

	/* 경계: size-1 글자 + 개행 = 딱 맞음 */
	fp = MKFILE("1234\n");
	if (fp) {
		memset(buf, PAD, sizeof(buf));
		r = read_line(fp, buf, 5);
		CHECK(r == 0);
		CHECK(eq(buf, 5, "1234"));
		CHECK(rest_pad(buf, 5, sizeof(buf)));
		CHECK(read_line(fp, buf, 5) == -1);
		fclose(fp);
	}

	/* 경계: size 글자 → 잘림, 나머지는 읽어서 버린다 */
	fp = MKFILE("12345\nnext\n");
	if (fp) {
		memset(buf, PAD, sizeof(buf));
		r = read_line(fp, buf, 5);
		CHECK(r == 1);
		CHECK(eq(buf, 5, "1234"));
		CHECK(rest_pad(buf, 5, sizeof(buf)));
		r = read_line(fp, buf, 5);      /* 다음 줄에 섞이지 않아야 한다 */
		CHECK(r == 0);
		CHECK(eq(buf, 5, "next"));
		fclose(fp);
	}

	/* 훨씬 긴 줄 */
	fp = MKFILE("123456789012345678901234567890\nxyz\n");
	if (fp) {
		memset(buf, PAD, sizeof(buf));
		r = read_line(fp, buf, 5);
		CHECK(r == 1);
		CHECK(eq(buf, 5, "1234"));
		r = read_line(fp, buf, 5);
		CHECK(r == 0);
		CHECK(eq(buf, 5, "xyz"));
		fclose(fp);
	}

	/* 마지막 줄에 개행이 없어도 정상 */
	fp = MKFILE("end");
	if (fp) {
		r = read_line(fp, buf, 8);
		CHECK(r == 0);
		CHECK(eq(buf, 8, "end"));
		CHECK(read_line(fp, buf, 8) == -1);
		fclose(fp);
	}

	/* 빈 줄 / 빈 파일 */
	fp = MKFILE("\n\n");
	if (fp) {
		memset(buf, PAD, sizeof(buf));
		r = read_line(fp, buf, 8);
		CHECK(r == 0 && eq(buf, 8, ""));
		r = read_line(fp, buf, 8);
		CHECK(r == 0 && eq(buf, 8, ""));
		CHECK(read_line(fp, buf, 8) == -1);
		fclose(fp);
	}
	fp = mkfile("", 0);
	if (fp) {
		memset(buf, PAD, sizeof(buf));
		r = read_line(fp, buf, 8);
		CHECK(r == -1);
		CHECK(eq(buf, 8, ""));
		fclose(fp);
	}

	/* 함정: 0xFF 바이트. fgetc 결과를 char 에 담아 EOF(-1) 와 비교하면 여기서 끝난다 */
	fp = mkfile("\xff\xfe\n", 3);
	if (fp) {
		memset(buf, PAD, sizeof(buf));
		r = read_line(fp, buf, 8);
		CHECK(r == 0);
		CHECK(eq(buf, 8, "\xff\xfe"));
		fclose(fp);
	}

	/* size == 1: 문자를 담을 자리가 없다 */
	fp = MKFILE("a\nb\n");
	if (fp) {
		memset(buf, PAD, sizeof(buf));
		r = read_line(fp, buf, 1);
		CHECK(r == 1);
		CHECK(buf[0] == '\0');
		CHECK(rest_pad(buf, 1, sizeof(buf)));
		r = read_line(fp, buf, 1);
		CHECK(r == 1);
		CHECK(read_line(fp, buf, 1) == -1);
		fclose(fp);
	}
	fp = MKFILE("\n");
	if (fp) {
		r = read_line(fp, buf, 1);
		CHECK(r == 0);
		CHECK(buf[0] == '\0');
		fclose(fp);
	}

	/* 인자 오류 */
	fp = MKFILE("x\n");
	if (fp) {
		CHECK(read_line(NULL, buf, 8) == -2);
		CHECK(read_line(fp, NULL, 8) == -2);
		CHECK(read_line(fp, buf, 0) == -2);
		fclose(fp);
	}

	return drill_report();
}
