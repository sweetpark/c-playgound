#include <stddef.h>
#include <string.h>
#include <strings.h>

/* P032 level_table — 요구사항은 problem.md */
enum level { LV_DEBUG, LV_INFO, LV_WARN, LV_ERROR, LV_COUNT };

const char *level_name(int lv)
{
	(void)lv;
	/* TODO */
	return "";
}

int level_parse(const char *s)
{
	(void)s;
	/* TODO */
	return -2;
}

size_t level_names(const char **out, size_t cap)
{
	(void)out;
	(void)cap;
	/* TODO */
	return 99;
}
