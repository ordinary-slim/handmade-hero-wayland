#include "draw_frame.h"

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
    const int width = 1920, height = 1080;// Window resolution
    const int radius = 100, circle_width = 20, stride_x = 300, stride_y = 300;
    int stride = width * 4;
    size_t size = stride * height;

    int fd = allocate_shm_file(size);
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
            width, height, stride, WL_SHM_FORMAT_XRGB8888);
    wl_shm_pool_destroy(pool);
    close(fd);

    /* Draw moving circles pattern */
    int offset = (int)state->offset;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double d = sqrt((double)(pow((x + offset) % stride_x - stride_x / 2, 2) +
                       pow((y + offset) % stride_y - stride_y / 2, 2)));
            if ((d <= radius + circle_width) && (d >= radius - circle_width))
                data[y * width + x] = 0xFF666666;
            else
                data[y * width + x] = 0xFFEEEEEE;
        }
    }

    munmap(data, size);
    wl_buffer_add_listener(buffer, &wl_buffer_listener, NULL);
    return buffer;
}
