# HINT2 (막혔을 때만)
```
int is_allowed_cipher_bad(const char *cipher_name)
{
    /* 1. (void)cipher_name
     * 2. return 1   ← 무엇이든 허용 */
}

int is_allowed_cipher_good(const char *cipher_name)
{
    /* [ ] 1. 목록 두 개
     *        BANNED  = { "DES-ECB", "3DES", "RC4", "MD5", "SHA1" }
     *        ALLOWED = { "AES-128-CBC", "AES-256-GCM", "ARIA-128-CBC", "SEED-CBC" } */

    /* [ ] 2. BANNED 를 먼저 순회한다
     *        strcmp(cipher_name, BANNED[i]) == 0 → return 0 */

    /* [ ] 3. ALLOWED 를 순회한다
     *        strcmp(cipher_name, ALLOWED[i]) == 0 → return 1 */

    /* [ ] 4. 둘 다 아니면 → return 0   (모르면 거부) */
}
```
