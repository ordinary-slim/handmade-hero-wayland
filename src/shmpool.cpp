#include "shmpool.h"
#include <cerrno>

int create_shm_file(size_t size) {
    int retries = 100;
    char name[] = "/handmade-hero-wayland";

    int fd = memfd_create(name, MFD_CLOEXEC | MFD_ALLOW_SEALING);

    if (fd < 0)
      return -1;

    fcntl(fd, F_ADD_SEALS, F_SEAL_SHRINK);

    int ret;
    do {
      ret = posix_fallocate(fd, 0, size);
    } while (ret == EINTR);
    if (ret != 0) {
      close(fd);
      errno = ret;
      return -1;
    }

    return fd;
}
