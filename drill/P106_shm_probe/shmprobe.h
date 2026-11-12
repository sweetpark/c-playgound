/* P106 shmprobe — System V 공유메모리 세그먼트의 "수명"을 관찰하는 도구. 구현은 solution.c */
#ifndef SHMPROBE_H
#define SHMPROBE_H

/* IPC_PRIVATE 로 size 바이트 세그먼트를 만든다. 성공하면 shmid(>=0), 실패 -1 */
int shm_make(unsigned long size);

/* 현재 붙어 있는(attach) 프로세스 수 (shm_nattch). 실패 -errno */
long shm_nattch(int shmid);

/* 세그먼트가 "삭제 예약" 상태인가 (IPC_RMID 를 불렀지만 아직 누군가 붙어 있어 남아 있는 상태). 예약이면 1, 아니면 0, 실패 -errno */
int shm_marked_dead(int shmid);

/* 세그먼트를 붙여 포인터를 돌려준다 (실패 NULL) / 뗀다 */
void *shm_map(int shmid);
int shm_unmap(void *p);

/* 삭제 예약(IPC_RMID). 성공 0, 실패 -errno */
int shm_remove(int shmid);

/* 이 shmid 가 아직 존재하는가. 존재 1, 없음 0 */
int shm_exists(int shmid);

#endif
