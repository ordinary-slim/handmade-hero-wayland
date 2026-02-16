#pragma once
#include <errno.h>
#include <time.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

// Shared memory (CPU + RAM)
int create_shm_file(size_t size);
