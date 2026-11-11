#ifndef DEBUGCODE_H
#define DEBUGCODE_H
#include <stdio.h>
void debug_output_bad(void);
#ifdef DEBUG_ENABLE
#define DEBUG_LOG(message) do { puts(message); } while (0)
#else
#define DEBUG_LOG(message) do { } while (0)
#endif
void debug_output_good(void);
#endif
