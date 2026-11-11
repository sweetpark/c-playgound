#include <errno.h>
#include <poll.h>
#include <stddef.h>
#include <unistd.h>

/* P035 write_all — 모범답안 */
int write_all(int fd, const void *buf, size_t n)
{
	const char *p = buf;

	if (fd < 0 || (buf == NULL && n > 0))
		return -1;

	while (n > 0) {
		ssize_t w = write(fd, p, n);
		if (w > 0) {                       /* 부분 쓰기: 쓴 만큼 건너뛰고 계속 */
			p += w;
			n -= (size_t)w;
			continue;
		}
		if (w < 0 && errno == EINTR)       /* 시그널에 끊김: 재시도 */
			continue;
		if (w < 0 && (errno == EAGAIN || errno == EWOULDBLOCK)) {
			struct pollfd pfd = { fd, POLLOUT, 0 };
			int pr = poll(&pfd, 1, 10000); /* 바쁜 재시도 대신 쓸 수 있을 때까지 대기 */
			if (pr > 0 || (pr < 0 && errno == EINTR))
				continue;
			return -1;                     /* 시간 초과 또는 poll 오류 */
		}
		return -1;                         /* EPIPE 등 */
	}
	return 0;
}
