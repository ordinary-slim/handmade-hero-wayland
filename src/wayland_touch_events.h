#pragma once
#include "wayland.h"

extern const struct wl_touch_listener wl_touch_listener;

void
wl_touch_down(void *data, struct wl_touch *wl_touch, uint32_t serial,
               uint32_t time, struct wl_surface *surface, int32_t id,
               wl_fixed_t x, wl_fixed_t y);

void
wl_touch_up(void *data, struct wl_touch *wl_touch, uint32_t serial,
               uint32_t time, int32_t id);

void
wl_touch_motion(void *data, struct wl_touch *wl_touch, uint32_t time,
               int32_t id, wl_fixed_t x, wl_fixed_t y);

void
wl_touch_cancel(void *data, struct wl_touch *wl_touch);

void
wl_touch_shape(void *data, struct wl_touch *wl_touch,
               int32_t id, wl_fixed_t major, wl_fixed_t minor);

void
wl_touch_orientation(void *data, struct wl_touch *wl_touch,
               int32_t id, wl_fixed_t orientation);

void
wl_touch_frame(void *data, struct wl_touch *wl_touch);
