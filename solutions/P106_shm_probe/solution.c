/* P106 shmprobe 모범답안 */
#include <errno.h>
#include <stddef.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "shmprobe.h"

#ifndef SHM_DEST
#define SHM_DEST 01000 /* _GNU_SOURCE 없이는 헤더가 안 보여 준다. 리눅스 값 */
#endif

int shm_make(unsigned long size)
{
	return shmget(IPC_PRIVATE, size, IPC_CREAT | 0600);
}

long shm_nattch(int shmid)
{
	struct shmid_ds ds;

	if (shmctl(shmid, IPC_STAT, &ds) < 0)
		return -errno;
	return (long)ds.shm_nattch;
}

int shm_marked_dead(int shmid)
{
	struct shmid_ds ds;

	if (shmctl(shmid, IPC_STAT, &ds) < 0)
		return -errno;
	return (ds.shm_perm.mode & SHM_DEST) ? 1 : 0;
}

void *shm_map(int shmid)
{
	void *p = shmat(shmid, NULL, 0);

	return (p == (void *)-1) ? NULL : p;
}

int shm_unmap(void *p)
{
	return shmdt(p) < 0 ? -errno : 0;
}

int shm_remove(int shmid)
{
	return shmctl(shmid, IPC_RMID, NULL) < 0 ? -errno : 0;
}

int shm_exists(int shmid)
{
	struct shmid_ds ds;

	return shmctl(shmid, IPC_STAT, &ds) == 0;
}
