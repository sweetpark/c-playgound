#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "file_perm.h"

static int get_file_mode(const char *path)
{
    struct stat st;
    if (stat(path, &st) != 0) return -1;
    return (int)(st.st_mode & 0777);
}

int create_file_bad(const char *path)
{
    mode_t old = umask(0);
    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    umask(old);
    if (fd < 0) return -1;
    close(fd);
    return get_file_mode(path);
}

int create_file_good(const char *path)
{
    mode_t old = umask(077);
    int fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    umask(old);
    if (fd < 0) return -1;
    close(fd);
    return get_file_mode(path);
}
