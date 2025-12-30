#pragma once
#include "wayland_seat.h"
#include "draw_frame.h"

extern const struct xdg_surface_listener xdg_surface_listener;
extern const struct xdg_wm_base_listener xdg_wm_base_listener;
extern const struct wl_callback_listener wl_surface_frame_listener;

void
xdg_surface_configure(void *data,
        struct xdg_surface *xdg_surface, uint32_t serial);

void
xdg_wm_base_ping(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial);

void
wl_surface_frame_done(void *data, struct wl_callback *cb, uint32_t time);

// Toplevel
extern const struct xdg_toplevel_listener xdg_toplevel_listener;

void
xdg_toplevel_configure(void *data,
    struct xdg_toplevel *xdg_toplevel, int32_t width, int32_t height,
    struct wl_array *states);

void
xdg_toplevel_close(void *data, struct xdg_toplevel *toplevel);
