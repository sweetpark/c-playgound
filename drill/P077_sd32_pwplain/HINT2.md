# HINT2 (막혔을 때만)
```
void save_password_bad(const char *pw, char *stored, size_t size)
{
    /* 1. strncpy(stored, pw, size - 1) 후 널 종료   ← 평문 저장 */
}

int verify_password_bad(const char *stored, const char *input)
{
    /* 1. return strcmp(stored, input) == 0 */
}

void save_password_good(const char *pw, unsigned *stored)
{
    /* [ ] 1. toy_hash(s) 를 먼저 만든다
     *        h = 5381
     *        s 의 글자마다 h = h * 33 + (unsigned char)c
     *        return h */

    /* [ ] 2. *stored = toy_hash(pw) */
}

int verify_password_good(unsigned stored, const char *input)
{
    /* [ ] 3. 입력을 다시 해시해서 비교한다
     *        return toy_hash(input) == stored
     *        (저장값을 복호화하지 않는다) */
}
```
