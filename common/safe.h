#define SAFE_FREE(_p) \
    do{ \
        if((_p) != NULL) { \
            free(_p); \
            (_p) = NULL; \
        } \
    } while(0)

#define SAFE_FCLOSE(_fp) \
    do { if ((_fp) != NULL) { fclose(_fp); (_fp) = NULL;}} while(0)

#define SAFE_CLOSE(_fd) \
    do { if ((_fd) >= 0) { close(_fd); (_fd) = -1; } } while(0)

#define SAFE_CPY(_dst, _src) \
    do { \
        snprintf((_dst), sizeof(_dst), "%s", (_src) ? (_src) : ""); \
    } while(0)
