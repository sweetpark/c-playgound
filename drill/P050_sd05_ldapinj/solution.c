/* P050 ldapinj — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
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

int ldap_lookup_good(const char *name)
{
	(void)name;
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

