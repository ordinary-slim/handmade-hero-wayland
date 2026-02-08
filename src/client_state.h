#pragma once
#include <wayland-client.h>
#include "wayland/wayland_pointer_events.h"
#include "wayland/wayland_keyboard_events.h"
#include "wayland/wayland_touch_events.h"

struct client_state {
    /* Globals */
    struct wl_display *wl_display;
    struct wl_registry *wl_registry;
    struct wl_shm *wl_shm;
    struct wl_compositor *wl_compositor;
    struct xdg_wm_base *xdg_wm_base;
    struct wl_seat *wl_seat;
    /* Objects */
    struct wl_surface *wl_surface1;
    struct xdg_surface *xdg_surface1;
    struct xdg_toplevel *xdg_toplevel;
    struct wl_keyboard *wl_keyboard;
    struct wl_pointer *wl_pointer;
    struct wl_touch *wl_touch;

    /* State */
    float offset;
    uint32_t last_frame;
    bool first_frame = true;

    bool closed;
    int width = 1920, height = 1080;
    int c_x = width / 2, c_y = height / 2;
    const int c_rad = 100, c_wid = 30;

    struct pointer_event pointer_event;

    struct xkb_state *xkb_state;
    struct xkb_context *xkb_context;
    struct xkb_keymap *xkb_keymap;

    struct touch_event touch_event;
};

void center_circle(struct client_state *state);
