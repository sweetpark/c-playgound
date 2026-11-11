# HINT2 (막혔을 때만)
```
int resolve_uid_bad(int isCorrectUser)
{
    /* 1. isCorrectUser 이면 return 0   ← 로그인만 성공하면 root
     * 2. 아니면 return -1 */
}

int resolve_uid_good(group_t group)
{
    /* [ ] 1. 그룹별 테이블 (grp_uid_t { group_t grp; int uid; })
     *        { GRP_ADMIN, 0 }, { GRP_NORMAL, 1000 }, { GRP_GUEST, 1001 }
     *        GRP_UNKNOWN 행은 **일부러 만들지 않는다** */

    /* [ ] 2. 테이블을 순회한다
     *        TBL[i].grp == group 이면 → return TBL[i].uid */

    /* [ ] 3. 못 찾았으면 → return -1   (모르는 그룹은 권한 없음) */
}
```
