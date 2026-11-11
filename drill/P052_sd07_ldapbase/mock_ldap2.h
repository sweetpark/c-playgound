#ifndef MOCK_LDAP2_H
#define MOCK_LDAP2_H
/* -1: 잘못된 인자, 0 이상: base(스코프)+filter를 모두 통과한 엔트리 개수 */
int mock_ldap_search_base(const char *base, const char *fixed_filter);
#endif
