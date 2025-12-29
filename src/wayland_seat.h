#include "wayland_keyboard_events.h"
#include "wayland_pointer_events.h"
#include "wayland_touch_events.h"

extern const struct wl_seat_listener wl_seat_listener;

void
wl_seat_capabilities(void *data, struct wl_seat *wl_seat, uint32_t capabilities);

void
wl_seat_name(void *data, struct wl_seat *wl_seat, const char *name);
