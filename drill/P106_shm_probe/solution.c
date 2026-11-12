/* P106 shmprobe — 요구사항은 problem.md. shmprobe.h 의 선언을 보고 채운다. */
#include <errno.h>
#include <stddef.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "shmprobe.h"

int shm_make(unsigned long size)
{
	(void)size;
	/* TODO */
	return -1;
}

long shm_nattch(int shmid)
{
	(void)shmid;
	/* TODO */
	return -ENOSYS;
}

int shm_marked_dead(int shmid)
{
	(void)shmid;
	/* TODO */
	return -ENOSYS;
}

void *shm_map(int shmid)
{
	(void)shmid;
	/* TODO */
	return NULL;
}

int shm_unmap(void *p)
{
	(void)p;
	/* TODO */
	return -ENOSYS;
}

int shm_remove(int shmid)
{
	(void)shmid;
	/* TODO */
	return -ENOSYS;
}

int shm_exists(int shmid)
{
	(void)shmid;
	/* TODO */
	return 0;
}
