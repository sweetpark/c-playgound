#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* P028 split_words — 모범답안 */
int split_words(const char *s, const char *delims, char out[][16], size_t maxn)
{
	char *copy, *save = NULL, *tok;
	size_t n = 0;
	int rc = 0;

	if (s == NULL || delims == NULL || out == NULL || maxn == 0)
		return -1;
	copy = malloc(strlen(s) + 1);           /* strtok 은 원본을 바꾼다 → 사본에서 */
	if (copy == NULL)
		return -1;
	memcpy(copy, s, strlen(s) + 1);

	for (tok = strtok_r(copy, delims, &save); tok != NULL; tok = strtok_r(NULL, delims, &save)) {
		size_t len = strlen(tok);
		if (len + 1 > sizeof(out[0])) { rc = -2; goto done; }
		if (n == maxn)                { rc = -3; goto done; }
		memcpy(out[n], tok, len + 1);
		n++;
	}
	rc = (int)n;
done:
	free(copy);
	return rc;
}
