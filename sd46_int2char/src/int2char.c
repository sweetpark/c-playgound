#include "int2char.h"

void store_offset_bad(doc_bad_t *doc, int line, int offset)
{
    doc->offsets_bad[line] = (char)offset;      /* 255 초과 시 잘림 */
}

int load_offset_bad(doc_bad_t *doc, int line)
{
    return (int)(unsigned char)doc->offsets_bad[line];
}

void store_offset_good(doc_good_t *doc, int line, int offset)
{
    doc->offsets_good[line] = offset;            /* 용도에 맞는 타입이라 잘림 없음 */
}

int load_offset_good(doc_good_t *doc, int line)
{
    return doc->offsets_good[line];
}
