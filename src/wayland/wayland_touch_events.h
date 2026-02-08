#pragma once
#include <wayland-client.h>

enum touch_event_mask {
       TOUCH_EVENT_DOWN = 1 << 0,
       TOUCH_EVENT_UP = 1 << 1,
       TOUCH_EVENT_MOTION = 1 << 2,
       TOUCH_EVENT_CANCEL = 1 << 3,
       TOUCH_EVENT_SHAPE = 1 << 4,
       TOUCH_EVENT_ORIENTATION = 1 << 5,
};

struct touch_point {
       bool valid;
       int32_t id;
       uint32_t event_mask;
       wl_fixed_t surface_x, surface_y;
       wl_fixed_t major, minor;
       wl_fixed_t orientation;
};

struct touch_event {
       uint32_t event_mask;
       uint32_t time;
       uint32_t serial;
       struct touch_point points[10];
};


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
