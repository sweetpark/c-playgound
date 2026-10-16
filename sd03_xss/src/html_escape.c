#include <stdio.h>
#include <string.h>
#include "html_escape.h"

void render_title_bad(const char *title, char *out, size_t out_size)
{
    snprintf(out, out_size, "<FONT SIZE=+1><STRONG>%s</STRONG></FONT>", title);
}

static size_t escape_html(const char *in, char *out, size_t out_size)
{
    size_t i, j = 0;
    for (i = 0; in[i] != '\0'; i++) {
        const char *rep = NULL;
        switch (in[i]) {
            case '<':  rep = "&lt;";   break;
            case '>':  rep = "&gt;";   break;
            case '&':  rep = "&amp;";  break;
            case '"':  rep = "&quot;"; break;
            default: break;
        }
        if (rep != NULL) {
            size_t rl = strlen(rep);
            if (j + rl >= out_size) break;
            memcpy(out + j, rep, rl);
            j += rl;
        } else {
            if (j + 1 >= out_size) break;
            out[j++] = in[i];
        }
    }
    out[j] = '\0';
    return j;
}

void render_title_good(const char *title, char *out, size_t out_size)
{
    char escaped[512];
    escape_html(title, escaped, sizeof(escaped));
    snprintf(out, out_size, "<FONT SIZE=+1><STRONG>%s</STRONG></FONT>", escaped);
}
