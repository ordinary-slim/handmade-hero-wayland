#include "draw_frame.h"
#include "shmpool.h"

static void
wl_buffer_release(void *data, struct wl_buffer *wl_buffer)
{
    /* Sent by the compositor when it's no longer using this buffer */
    wl_buffer_destroy(wl_buffer);
}

static const struct wl_buffer_listener wl_buffer_listener = {
    .release = wl_buffer_release,
};

struct wl_buffer *
draw_frame(struct client_state *state)
{
    int stride = state->width * 4;
    size_t size = stride * state->height;

    int fd = create_shm_file(size);
    if (fd == -1) {
        return NULL;
    }

    uint32_t *data = (uint32_t *)mmap(NULL, size,
            PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED) {
        close(fd);
        return NULL;
    }

    struct wl_shm_pool *pool = wl_shm_create_pool(state->wl_shm, fd, size);
    struct wl_buffer *buffer = wl_shm_pool_create_buffer(pool, 0,
            state->width, state->height, stride, WL_SHM_FORMAT_XRGB8888);
    wl_shm_pool_destroy(pool);
    close(fd);

    /* Draw circle */
    //int offset = (int)state->offset;
    for (int y = 0; y < state->height; ++y) {
        for (int x = 0; x < state->width; ++x) {
            double dist = sqrt((double)(pow(x - state->c_x, 2) + pow(y - state->c_y, 2)));
            if ((dist <= state->c_rad + state->c_wid) && (state->c_rad + state->c_wid))
                data[y * state->width + x] = 0xFF666666;
            else
                data[y * state->width + x] = 0xFFEEEEEE;
        }
    }

    munmap(data, size);
    wl_buffer_add_listener(buffer, &wl_buffer_listener, NULL);
    return buffer;
}
