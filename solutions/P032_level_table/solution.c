#include <stddef.h>
#include <string.h>
#include <strings.h>

/* P032 level_table — 모범답안 */
enum level { LV_DEBUG, LV_INFO, LV_WARN, LV_ERROR, LV_COUNT };

const char *level_name(int lv)
{
	switch (lv) {
	case LV_DEBUG: return "DEBUG";
	case LV_INFO:  return "INFO";
	case LV_WARN:  return "WARN";
	case LV_ERROR: return "ERROR";
	default:       return "UNKNOWN";   /* 범위 밖에서도 반드시 return */
	}
}

int level_parse(const char *s)
{
	int i;

	if (s == NULL)
		return -1;
	for (i = 0; i < (int)LV_COUNT; i++)    /* size_t 가 아니라 int 로 enum 순회 */
		if (strcasecmp(s, level_name(i)) == 0)
			return i;
	return -1;
}

size_t level_names(const char **out, size_t cap)
{
	int i;

	if (out == NULL || cap < (size_t)LV_COUNT)
		return 0;
	for (i = 0; i < (int)LV_COUNT; i++)
		out[i] = level_name(i);
	return (size_t)LV_COUNT;
}
