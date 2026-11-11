#include <string.h>
#include "secure_transmit.h"

void prepare_for_send_bad(const char *pw, char *out, size_t out_size)
{
    strncpy(out, pw, out_size - 1);
    out[out_size - 1] = '\0';
}

static void xor_mask(const char *in, char *out, size_t out_size)
{
    size_t i;
    for (i = 0; i + 1 < out_size && in[i]; i++) out[i] = (char)(in[i] ^ 0x5A);
    out[i] = '\0';
}

void prepare_for_send_good(const char *pw, char *out, size_t out_size)
{
    xor_mask(pw, out, out_size);
}

void undo_mask(const char *masked, char *out, size_t out_size)
{
    xor_mask(masked, out, out_size);   /* XOR은 대칭이라 같은 함수로 복원된다 */
}
