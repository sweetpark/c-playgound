#ifndef INT2CHAR_H
#define INT2CHAR_H
#define MAX_LINES 4
typedef struct { char offsets_bad[MAX_LINES]; } doc_bad_t;
typedef struct { int  offsets_good[MAX_LINES]; } doc_good_t;
void store_offset_bad(doc_bad_t *doc, int line, int offset);
int  load_offset_bad(doc_bad_t *doc, int line);
void store_offset_good(doc_good_t *doc, int line, int offset);
int  load_offset_good(doc_good_t *doc, int line);
#endif
