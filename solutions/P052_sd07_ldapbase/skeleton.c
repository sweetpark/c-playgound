/* P052 ldapbase — 요구사항은 problem.md.  Bad 는 이미 들어 있다(수정 금지). Good 만 구현한다. */
#include "mock_ldap2.h"
#include "ldap_base.h"

int ldap_by_manager_bad(const char *userSuppliedBase)
{
    return mock_ldap_search_base(userSuppliedBase, "manager=m1");
}

int ldap_by_manager_good(void)
{
	/* TODO: problem.md 를 보고 Good 구현 */
	return -1;
}

