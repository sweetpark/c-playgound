# HINT2
```
shm_make      : shmget(IPC_PRIVATE, size, IPC_CREAT|0600)
shm_nattch    : IPC_STAT 실패면 -errno, 아니면 (long)ds.shm_nattch
shm_marked_dead: IPC_STAT 후 (ds.shm_perm.mode & SHM_DEST) ? 1 : 0
shm_map       : p = shmat(id, NULL, 0); p == (void *)-1 ? NULL : p
shm_exists    : shmctl(id, IPC_STAT, &ds) == 0
```
