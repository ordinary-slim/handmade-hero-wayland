#pragma once
#include <errno.h>
#include <time.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

// Shared memory (CPU + RAM)
void randname(char *buf);
int create_shm_file(void);
int allocate_shm_file(size_t size);
