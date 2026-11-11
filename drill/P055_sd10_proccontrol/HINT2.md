# HINT2 (막혔을 때만)
```
int build_convert_command_bad(const char *op, const char *documentName,
                              char *out, size_t out_size)
{
    /* 1. snprintf(out, out_size, "%s %s", op, documentName)
     *    ← op 자체가 실행 프로그램명이 된다
     * 2. 잘렸으면 -1, 아니면 0 */
}

int build_convert_command_good(const char *op, const char *documentName,
                               char *out, size_t out_size)
{
    /* [ ] 1. 매핑 테이블
     *        { "DocToHtml" → "convertDocToHtml", "DocToPdf" → "convertDocToPdf" }
     *        op_map_t { const char *op; const char *command; } */

    /* [ ] 2. op 와 일치하는 항목을 찾는다
     *        strcmp(op, OP_TABLE[i].op) == 0 */

    /* [ ] 3. 찾았으면 **매핑된 command** 로 조립
     *        snprintf(out, out_size, "%s %s", OP_TABLE[i].command, documentName)
     *        잘렸으면 -1, 아니면 0 */

    /* [ ] 4. 못 찾았으면 → return -1 */
}
```
