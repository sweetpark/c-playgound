#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "mock_ldap.h"
#include "ldap_auth.h"

int ldap_lookup_bad(const char *name)
{
    char filter[64];
    snprintf(filter, sizeof(filter), "(name=%s)", name);
    return mock_ldap_search(filter);
}

/* 영숫자만 허용 — LDAP 특수문자(* ( ) \ NUL)를 전부 배제한다 */
static int is_safe_ldap_value(const char *s)
{
    size_t i, len = strlen(s);
    if (len == 0 || len > 32) return 0;
    for (i = 0; i < len; i++) {
        if (!isalnum((unsigned char)s[i])) return 0;
    }
    return 1;
}

int ldap_lookup_good(const char *name)
{
    char filter[64];
    if (!is_safe_ldap_value(name)) return -1;
    snprintf(filter, sizeof(filter), "(name=%s)", name);
    return mock_ldap_search(filter);
}
