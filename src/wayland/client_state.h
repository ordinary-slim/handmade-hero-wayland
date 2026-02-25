#pragma once
#include <sys/mman.h>
#include <wayland-client.h>
#include <cstdio>
#include "wayland/wayland_pointer_events.h"
#include "wayland/wayland_keyboard_events.h"
#include "wayland/wayland_touch_events.h"

struct wayland_buffer {
    struct wl_buffer* buffer = nullptr;
    struct wl_shm* wl_shm = nullptr;
    uint32_t* data = nullptr;
    int width = 0;
    int height = 0;
    size_t size = 0;
    bool released = false;
};

static void
wl_buffer_release(void *data, struct wl_buffer *wl_buffer)
{
    /* Sent by the compositor when it's no longer using this buffer */
    // munmap(buffer.data, buffer.size);
    // wl_buffer_destroy(wl_buffer);
}

static const struct wl_buffer_listener wl_buffer_listener = {
    .release = wl_buffer_release,
};

struct client_state {
    /* Globals */
    struct wl_display *wl_display = nullptr;
    struct wl_registry *wl_registry = nullptr;
    struct wl_compositor *wl_compositor = nullptr;
    struct xdg_wm_base *xdg_wm_base = nullptr;
    struct wl_seat *wl_seat = nullptr;
    /* Objects */
    struct wl_surface *wl_surface1 = nullptr;
    struct xdg_surface *xdg_surface1 = nullptr;
    struct xdg_toplevel *xdg_toplevel = nullptr;
    struct wl_keyboard *wl_keyboard = nullptr;
    struct wl_pointer *wl_pointer = nullptr;
    struct wl_touch *wl_touch = nullptr;

    struct wl_shm* wl_shm = nullptr;
    int buffer_count = 1;
    wayland_buffer* buffers = nullptr;

    /* State */
    float offset;
    uint32_t last_frame;
    bool first_frame = true;

    bool closed;
    int width = 1920, height = 1080;
    int c_x = width / 2, c_y = height / 2;
    const int c_rad = 100, c_wid = 30;

    struct pointer_event pointer_event;

    struct xkb_state *xkb_state = nullptr;
    struct xkb_context *xkb_context = nullptr;
    struct xkb_keymap *xkb_keymap = nullptr;

    struct touch_event touch_event;

    client_state();
    ~client_state();
};


void center_circle(struct client_state *state);
wayland_buffer get_buffer(struct client_state *state);
void init_buffers(struct client_state *state);
void delete_buffers(client_state* self);
