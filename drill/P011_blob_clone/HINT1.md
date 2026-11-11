# HINT1
복사 바이트 수는 `sizeof(*src) + src->n * sizeof(src->data[0])`. 새로 할당하는 크기도 똑같이 계산해 `memcpy(dup, src, bytes)` 한 번.
