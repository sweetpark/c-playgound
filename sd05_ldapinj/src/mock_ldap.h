#ifndef MOCK_LDAP_H
#define MOCK_LDAP_H
/* -1: 잘못된 인자, 0 이상: 매칭된 엔트리 개수 */
int mock_ldap_search(const char *filter);
#endif
