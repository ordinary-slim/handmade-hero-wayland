#include "client_state.h"
#include "shmpool.h"
#include <time.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

wayland_buffer
get_buffer(struct client_state *state) {
    // 4 bytes per pixel (WL_SHM_FORMAT_XRGB8888)
    wayland_buffer buffer = {.width = state->width, .height = state->height};
    int stride = buffer.width * 4;
    buffer.size = stride * buffer.height;

    int fd = create_shm_file(buffer.size);
    if (fd == -1) {
        exit(1);
    }

    buffer.data = (uint32_t *)mmap(NULL, buffer.size,
            PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (buffer.data == MAP_FAILED) {
        close(fd);
        exit(1);
    }

    struct wl_shm_pool *pool = wl_shm_create_pool(state->wl_shm, fd, buffer.size);
    buffer.buffer = wl_shm_pool_create_buffer(pool, 0,
            state->width, state->height, stride, WL_SHM_FORMAT_XRGB8888);
    wl_shm_pool_destroy(pool);
    close(fd);
    wl_buffer_add_listener(buffer.buffer, &wl_buffer_listener, NULL);
    return buffer;
}

void init_buffers(client_state* self) {
  delete_buffers(self);
  for (int i = 0; i < self->buffer_count; i++) {
    self->buffers[i] = get_buffer(self);
  }
}

void delete_buffers(client_state* self) {
  for (int i = 0; i < self->buffer_count; ++i) {
    wayland_buffer& b = self->buffers[i];

    if (not(b.released)) {
      continue;
    }
    if (b.data && b.size) {
      munmap(b.data, b.size);
      b.data = nullptr;
      b.size = 0;
    }
    if (b.buffer) {
      wl_buffer_destroy(b.buffer);
      b.buffer = nullptr;
    }
  }
}

void center_circle(struct client_state *state) {
    state->c_x = state->width / 2;
    state->c_y = state->height / 2;
}

client_state::client_state() {
  buffers = new wayland_buffer[buffer_count];
}

client_state::~client_state() {
  delete_buffers(this);
  delete[] buffers;
}
