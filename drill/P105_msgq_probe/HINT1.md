# HINT1
- 보낼 구조체는 `struct { long mtype; char mtext[N]; }`. `msgsnd` 의 길이 인자는 **mtext 의 길이**(mtype 제외).
- `mtype` 은 1 이상이어야 한다 (0 이면 `EINVAL`).
- 상태는 `struct msqid_ds` 의 `msg_qnum`(개수), `__msg_cbytes`(바이트), `msg_qbytes`(용량).
- 용량 줄이기: `IPC_STAT` → `msg_qbytes = qbytes` → `IPC_SET`.
