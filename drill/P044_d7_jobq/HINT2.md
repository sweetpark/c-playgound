# HINT2 (막혔을 때만)
```
/* jobq.h / jobq.c */
/* [ ] 1. job_t, jobq_t { buf, cap, head, tail, num, closed, lock, not_empty } */
/* [ ] 2. jobq_create(cap, out)
 *        가드 (cap == 0 → INVALID_ARG) / *out = NULL
 *        객체 → buf → mutex init → cond init  (실패하면 잡은 것만 역순)
 *        소유권 이전 */
/* [ ] 3. jobq_push
 *        lock → closed 면 NOT_SUPPORTED, 가득이면 TOO_SMALL
 *        아니면 buf[tail] = *job, tail = (tail + 1) % cap, num++, cond_signal
 *        unlock  (모든 경로에서 unlock 은 한 곳) */
/* [ ] 4. jobq_pop
 *        lock → while (num == 0 && !closed) cond_wait          ← while 로 감싼다
 *        num > 0 이면 꺼냄 (head 전진)
 *        아니면 (closed && empty) NOT_FOUND
 *        unlock */
/* [ ] 5. jobq_close: lock → closed = true → cond_broadcast → unlock   ← signal 이 아니다 */
/* [ ] 6. jobq_destroy: NULL 안전, cond / mutex destroy → buf → 객체 */

/* main.c */
/* [ ] 1. 전역 깃발: static volatile sig_atomic_t g_running = 1, g_reload = 0 */
/* [ ] 2. on_signal: SIGINT / SIGTERM → g_running = 0,  SIGHUP → g_reload = 1   (대입만) */
/* [ ] 3. signal_setup
 *        sigaction 으로 세 개 등록, sa_flags = 0 (SA_RESTART 끈다)
 *        SIGPIPE 는 SIG_IGN */
/* [ ] 4. main — 계단식 라벨
 *        변수: ret / exit_code / cfg / sock(-1) / q / tids / made / port / worker_num
 *        1단계 설정:  cfg_create 실패 → exit 1, goto OUT
 *                     signal_setup 실패 → exit 1, goto ERR_CFG
 *        2단계 소켓:  udp_open 실패 → exit 2, goto ERR_CFG
 *        3단계 큐:    jobq_create 실패 → exit 3, goto ERR_SOCK
 *        4단계 워커:  tids = calloc → 실패 exit 3, goto ERR_QUEUE
 *                     for i < worker_num: 인자를 힙에 만들고 pthread_create
 *                       실패하면 인자 free, exit 3, goto ERR_THREAD   (made 개수만 되돌린다)
 *                       made++
 *        "started: port=.. worker=.." 로그
 *        서비스 루프: while (g_running)
 *                       poll(..., 200ms)
 *                       n < 0 이면 EINTR 는 continue, 아니면 break
 *                       g_reload 면 로그 레벨만 재적재, "config reloaded"
 *                       n == 0 이면 continue (타임아웃)
 *                       POLLIN 이면 recv_one
 *        "shutting down..." 로그, exit_code = 0
 *        ERR_THREAD: jobq_close → made 개 join → tids free
 *        ERR_QUEUE : jobq_destroy
 *        ERR_SOCK  : SAFE_CLOSE(sock)
 *        ERR_CFG   : cfg_destroy
 *        OUT       : 로그, return exit_code */
/* [ ] 5. recv_one(sock, q)
 *        recvfrom → msg_unpack → 실패면 WRN "drop: ..." 후 return (죽지 않는다)
 *        job_t 를 채워 jobq_push → TOO_SMALL 이면 WRN (버린다)
 *        성공하면 INF "recv session=<id>" */
/* [ ] 6. worker_main(arg)
 *        인자를 지역으로 옮기고 free(arg)
 *        while (jobq_pop(q, &job) == RET_OK)
 *            처리 → 응답 송신 → INF "done session=<id>"
 *        return NULL */
/* [ ] 7. 종료 대기에 5초 상한 (wait_workers: clock_gettime + pthread_cond_timedwait) */
```
