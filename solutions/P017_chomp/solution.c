#include <stddef.h>
#include <string.h>

/* P017 chomp — 모범답안 */
size_t chomp(char *s)
{
	size_t n;

	if (s == NULL)
		return 0;
	n = strlen(s);
	if (n > 0 && s[n - 1] == '\n') {     /* 개행이 있을 때만 */
		n--;
		if (n > 0 && s[n - 1] == '\r')
			n--;
		s[n] = '\0';
	}
	return n;
}
