# HINT2 (막혔을 때만)
```
/* cfg.c */
/* [ ] 1. 상수와 구조체
 *        CFG_KEY_MAX 31 / CFG_VAL_MAX 127 / CFG_LINE_MAX 255
 *        cfg_item_t { key[+1], val[+1] }
 *        struct cfg_s { items, num, cap }     (헤더에는 typedef 만 노출 = 불투명) */

/* [ ] 2. static char *trim(char *s) — 앞뒤 공백 제거 (제자리) */

/* [ ] 3. static int cfg_put(cfg_t *cfg, const char *key, const char *val)
 *        가드 3개
 *        이미 있는 key 면 val 만 덮어쓴다 (나중 것이 이긴다)
 *        num == cap 이면 확장
 *            new_cap = (cap == 0) ? 8 : cap * 2
 *            tmp = realloc(items, ...)     ← tmp 로 받는다
 *            NULL 이면 RET_NO_MEM (원본은 그대로)
 *            성공했을 때만 items / cap 갱신
 *        새 항목은 snprintf 로 복사, num++ */

/* [ ] 4. cfg_get_str / cfg_get_int
 *        가드 → 항목 순회 → 없으면 RET_NOT_FOUND
 *        get_int 는 strtol + endptr + errno(ERANGE) 로 검사, 숫자가 아니면 RET_BAD_FORMAT */

/* [ ] 5. int cfg_create(const char *path, cfg_t **out_cfg)
 *        ① int ret = RET_FAIL
 *        ② cfg / fp / line 을 NULL 로 선언
 *        ③ CHK_PTR(path), CHK_PTR(out_cfg)  → 즉시 return
 *           *out_cfg = NULL
 *        ④ cfg = CALLOC(...)               → CHK_ALLOC_GOTO
 *           line = CALLOC(...)             → CHK_ALLOC_GOTO
 *           fp = fopen(path, "r")          → 실패면 LOG_ERRNO, CHK_COND_GOTO(IO_ERROR)
 *        ⑤ fgets 루프
 *           개행 제거 → trim → 빈 줄 / '#' 줄은 건너뛴다
 *           '=' 가 없으면 BAD_FORMAT
 *           key / val 을 trim, 길이 검사 (key 빈 문자열 / 31 초과 / val 127 초과 → BAD_FORMAT)
 *           CHK_RET_GOTO(cfg_put(...))
 *        ⑥ 필수 키: listen_port(1~65535), worker_num(1~64)
 *           없으면 NOT_FOUND, 범위 밖이면 BAD_FORMAT
 *        ⑦ 소유권 이전
 *           *out_cfg = cfg;
 *           cfg      = NULL;               ← 이 한 줄이 심장
 *           ret      = RET_OK;
 *        ⑧ CLEANUP (잡은 역순)
 *           SAFE_FCLOSE(fp) → SAFE_FREE(line) → cfg_destroy(cfg)
 *           return ret */

/* [ ] 6. void cfg_destroy(cfg_t *cfg)
 *        NULL 이면 return / items 해제 / 객체 자체도 free */
```
