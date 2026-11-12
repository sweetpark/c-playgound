# HINT2
```
mq_create : qid = msgget(IPC_PRIVATE, IPC_CREAT|0600); IPC_STAT; if (qbytes < ds.msg_qbytes) { ds.msg_qbytes = qbytes; IPC_SET }
mq_send_one: msglen 범위 검사 → m.mtype = 1; msgsnd(qid, &m, msglen, IPC_NOWAIT) < 0 이면 return -errno
mq_fill    : while (mq_send_one(...) == 0) n++;  마지막 반환값이 멈춘 이유
mq_drain   : loop { msgrcv(qid, &m, sizeof m.mtext, 0, IPC_NOWAIT) < 0 → errno==ENOMSG 면 n 반환 ; n++ }
```
