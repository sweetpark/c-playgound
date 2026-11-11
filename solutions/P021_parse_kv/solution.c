#include <stddef.h>
#include <string.h>

/* P021 parse_kv — 모범답안 */
static int is_blank(char c)
{
	return c == ' ' || c == '\t';
}

int parse_kv(const char *line, char *key, size_t key_size, char *val, size_t val_size)
{
	const char *eq, *kb, *ke, *vb, *ve;
	size_t kl, vl;

	if (line == NULL || key == NULL || val == NULL || key_size == 0 || val_size == 0)
		return -1;
	key[0] = '\0';                       /* 에러 경로마다 쓰지 않도록 시작에서 한 번 */
	val[0] = '\0';

	eq = strchr(line, '=');              /* line 은 읽기만 한다 (위치만 구한다) */
	if (eq == NULL)
		return -2;

	kb = line;
	ke = eq;
	while (kb < ke && is_blank(*kb)) kb++;
	while (ke > kb && is_blank(ke[-1])) ke--;
	if (ke == kb)
		return -3;
	kl = (size_t)(ke - kb);
	if (kl + 1 > key_size)
		return -4;

	vb = eq + 1;
	ve = vb + strlen(vb);
	while (vb < ve && is_blank(*vb)) vb++;
	while (ve > vb && is_blank(ve[-1])) ve--;
	vl = (size_t)(ve - vb);
	if (vl + 1 > val_size)
		return -5;

	memcpy(key, kb, kl);
	key[kl] = '\0';
	memcpy(val, vb, vl);
	val[vl] = '\0';
	return 0;
}
