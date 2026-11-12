/* P105 mqprobe — System V 메시지 큐를 "관찰"하는 작은 도구 모음. 구현은 solution.c */
#ifndef MQPROBE_H
#define MQPROBE_H

/* 새 큐를 만들고 용량(msg_qbytes)을 qbytes 로 줄인다. 성공하면 큐 id(>=0), 실패하면 -1 */
int mq_create(unsigned long qbytes);

/* msglen 바이트 메시지를 IPC_NOWAIT 로 하나 보낸다. 성공 0, 실패는 -errno (예: 가득 차면 -EAGAIN) */
int mq_send_one(int qid, int msglen);

/* 가득 찰 때까지 msglen 바이트 메시지를 계속 보낸다. 보낸 개수를 *sent 에 넣고,
 * 멈춘 이유(-errno)를 돌려준다. 시험은 -EAGAIN 을 기대한다 */
int mq_fill(int qid, int msglen, int *sent);

/* 큐 상태: 쌓인 메시지 수, 쌓인 바이트, 용량(바이트). 성공 0, 실패 -errno. 인자는 NULL 이면 무시 */
int mq_stat(int qid, unsigned long *nmsgs, unsigned long *nbytes, unsigned long *qbytes);

/* 쌓인 메시지를 전부 IPC_NOWAIT 로 꺼내 버린다. 꺼낸 개수를 돌려준다 (실패 -errno) */
int mq_drain(int qid);

/* 큐를 지운다. 성공 0, 실패 -errno */
int mq_remove(int qid);

#endif
