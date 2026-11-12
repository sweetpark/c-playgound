/* P105 mqprobe 모범답안 */
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "mqprobe.h"

struct mq_msg {
	long mtype;
	char mtext[8192];
};

int mq_create(unsigned long qbytes)
{
	struct msqid_ds ds;
	int qid = msgget(IPC_PRIVATE, IPC_CREAT | 0600);

	if (qid < 0)
		return -1;
	if (msgctl(qid, IPC_STAT, &ds) < 0) {
		msgctl(qid, IPC_RMID, NULL);
		return -1;
	}
	if (qbytes < ds.msg_qbytes) { /* 올리는 건 특권이 필요하다. 줄이는 건 누구나 */
		ds.msg_qbytes = qbytes;
		if (msgctl(qid, IPC_SET, &ds) < 0) {
			msgctl(qid, IPC_RMID, NULL);
			return -1;
		}
	}
	return qid;
}

int mq_send_one(int qid, int msglen)
{
	struct mq_msg m;

	if (msglen < 0 || msglen > (int)sizeof(m.mtext))
		return -EINVAL;
	m.mtype = 1;
	memset(m.mtext, 'x', (size_t)msglen);
	if (msgsnd(qid, &m, (size_t)msglen, IPC_NOWAIT) < 0)
		return -errno;
	return 0;
}

int mq_fill(int qid, int msglen, int *sent)
{
	int rc, n = 0;

	while ((rc = mq_send_one(qid, msglen)) == 0)
		n++;
	if (sent)
		*sent = n;
	return rc;
}

int mq_stat(int qid, unsigned long *nmsgs, unsigned long *nbytes, unsigned long *qbytes)
{
	struct msqid_ds ds;

	if (msgctl(qid, IPC_STAT, &ds) < 0)
		return -errno;
	if (nmsgs)
		*nmsgs = (unsigned long)ds.msg_qnum;
	if (nbytes)
		*nbytes = (unsigned long)ds.__msg_cbytes;
	if (qbytes)
		*qbytes = (unsigned long)ds.msg_qbytes;
	return 0;
}

int mq_drain(int qid)
{
	struct mq_msg m;
	int n = 0;

	for (;;) {
		if (msgrcv(qid, &m, sizeof(m.mtext), 0, IPC_NOWAIT) < 0) {
			if (errno == ENOMSG)
				return n;
			return -errno;
		}
		n++;
	}
}

int mq_remove(int qid)
{
	if (msgctl(qid, IPC_RMID, NULL) < 0)
		return -errno;
	return 0;
}
