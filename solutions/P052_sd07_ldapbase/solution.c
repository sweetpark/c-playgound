#include "mock_ldap2.h"
#include "ldap_base.h"

int ldap_by_manager_bad(const char *userSuppliedBase)
{
    return mock_ldap_search_base(userSuppliedBase, "manager=m1");
}

int ldap_by_manager_good(void)
{
    return mock_ldap_search_base("ou=NewHires", "manager=m1");
}
