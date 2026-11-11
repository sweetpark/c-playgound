# HINT2 (막혔을 때만)
```
typedef struct { char offsets_bad[MAX_LINES]; } doc_bad_t;

void store_offset_bad(doc_bad_t *doc, int line, int offset)
{
    /* 1. doc->offsets_bad[line] = (char)offset   ← 255 초과 시 잘린다 */
}

int load_offset_bad(doc_bad_t *doc, int line)
{
    /* 1. return (int)(unsigned char)doc->offsets_bad[line] */
}

typedef struct { int offsets_good[MAX_LINES]; } doc_good_t;

void store_offset_good(doc_good_t *doc, int line, int offset)
{
    /* [ ] 1. 구조체 정의의 원소 타입을 char → int 로 바꾼다
     *        MAX_LINES 4 */

    /* [ ] 2. doc->offsets_good[line] = offset   ← 대입문은 똑같다 */
}

int load_offset_good(doc_good_t *doc, int line)
{
    /* [ ] 3. return doc->offsets_good[line] */
}
```
