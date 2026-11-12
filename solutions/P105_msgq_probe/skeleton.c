/* P105 mqprobe — 요구사항은 problem.md. mqprobe.h 의 선언을 보고 채운다. */
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "mqprobe.h"

int mq_create(unsigned long qbytes)
{
	(void)qbytes;
	/* TODO: msgget(IPC_PRIVATE) 후 msg_qbytes 를 qbytes 로 줄인다 */
	return -1;
}

int mq_send_one(int qid, int msglen)
{
	(void)qid;
	(void)msglen;
	/* TODO */
	return -ENOSYS;
}

int mq_fill(int qid, int msglen, int *sent)
{
	(void)qid;
	(void)msglen;
	if (sent)
		*sent = 0;
	/* TODO */
	return -ENOSYS;
}

int mq_stat(int qid, unsigned long *nmsgs, unsigned long *nbytes, unsigned long *qbytes)
{
	(void)qid;
	(void)nmsgs;
	(void)nbytes;
	(void)qbytes;
	/* TODO */
	return -ENOSYS;
}

int mq_drain(int qid)
{
	(void)qid;
	/* TODO */
	return -ENOSYS;
}

int mq_remove(int qid)
{
	(void)qid;
	/* TODO */
	return -ENOSYS;
}
