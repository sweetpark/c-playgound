# HINT2 (막혔을 때만)
```
int ldap_by_manager_bad(const char *userSuppliedBase)
{
    /* 1. return mock_ldap_search_base(userSuppliedBase, "manager=m1")
     *    ← base 가 외부 입력 */
}

int ldap_by_manager_good(void)
{
    /* [ ] 1. 시그니처에서 base 인자를 **없앤다**
     *        (검증이 아니라 입력 경로 제거) */

    /* [ ] 2. return mock_ldap_search_base("ou=NewHires", "manager=m1")
     *        base 와 필터는 둘 다 코드 안의 상수 */
}
```
