# HINT1
- 상태는 `struct shmid_ds ds; shmctl(id, IPC_STAT, &ds);` → `ds.shm_nattch`, `ds.shm_perm.mode & SHM_DEST`.
- `shmat` 이 실패하면 `(void *)-1` 을 돌려준다.
- `shmctl(id, IPC_RMID, NULL)` 은 삭제 **예약**이다.
- `SHM_DEST` 가 정의되지 않았다고 나오면 `_GNU_SOURCE` 없이 컴파일 중이다. `#ifndef SHM_DEST` / `#define SHM_DEST 01000` 으로 직접 정의해도 된다 (리눅스 값).
