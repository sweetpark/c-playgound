# HINT1
`while (n > 0) { w = write(fd, p, n); if (w > 0) { p += w; n -= w; continue; } if (errno == EINTR) continue; if (errno == EAGAIN) { poll(&pfd, 1, 10000); continue; } return -1; }`.
