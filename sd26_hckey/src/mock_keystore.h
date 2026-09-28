#ifndef MOCK_KEYSTORE_H
#define MOCK_KEYSTORE_H
#include <stddef.h>
/* key_name으로 등록된 키를 out에 복사. 성공 0, 실패 -1 */
int mock_keystore_load(const char *key_name, unsigned char *out, size_t out_len);
void mock_keystore_register(const char *key_name, const unsigned char *key, size_t key_len);
#endif
