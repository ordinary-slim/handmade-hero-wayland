#include <cstdio>
#include "wayland_seat.h"
#include <client_state.h>

const struct wl_seat_listener wl_seat_listener = {
       .capabilities = wl_seat_capabilities,
       .name = wl_seat_name,
};

void
wl_seat_capabilities(void *data, struct wl_seat *wl_seat, uint32_t capabilities)
{
       struct client_state *state = (struct client_state *)data;

       bool have_pointer = capabilities & WL_SEAT_CAPABILITY_POINTER;
       bool have_keyboard = capabilities & WL_SEAT_CAPABILITY_KEYBOARD;
       bool have_touch = capabilities & WL_SEAT_CAPABILITY_TOUCH;

       if (have_pointer && state->wl_pointer == NULL) {
               state->wl_pointer = (struct wl_pointer *)wl_seat_get_pointer(state->wl_seat);
               wl_pointer_add_listener(state->wl_pointer,
                               &wl_pointer_listener, state);
       } else if (!have_pointer && state->wl_pointer != NULL) {
               wl_pointer_release(state->wl_pointer);
               state->wl_pointer = NULL;
       }

       if (have_keyboard && state->wl_keyboard == NULL) {
               state->wl_keyboard = (struct wl_keyboard *)wl_seat_get_keyboard(state->wl_seat);
               wl_keyboard_add_listener(state->wl_keyboard,
                               &wl_keyboard_listener, state);
       } else if (!have_keyboard && state->wl_keyboard != NULL) {
               wl_keyboard_release(state->wl_keyboard);
               state->wl_keyboard = NULL;
       }

       if (have_touch && state->wl_touch == NULL) {
               state->wl_touch = (struct wl_touch *)wl_seat_get_touch(state->wl_seat);
               wl_touch_add_listener(state->wl_touch,
                               &wl_touch_listener, state);
       } else if (!have_touch && state->wl_touch != NULL) {
               wl_touch_release(state->wl_touch);
               state->wl_touch = NULL;
       }
}

void
wl_seat_name(void *data, struct wl_seat *wl_seat, const char *name)
{
       fprintf(stderr, "seat name: %s\n", name);
}
