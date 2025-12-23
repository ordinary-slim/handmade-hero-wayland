#define _POSIX_C_SOURCE 200112L
#include <cstdio>
#include <sys/mman.h>
#include <unistd.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <wayland-client.h>
#include "xdg-shell-client-protocol.h"

enum pointer_event_mask {
       POINTER_EVENT_ENTER = 1 << 0,
       POINTER_EVENT_LEAVE = 1 << 1,
       POINTER_EVENT_MOTION = 1 << 2,
       POINTER_EVENT_BUTTON = 1 << 3,
       POINTER_EVENT_AXIS = 1 << 4,
       POINTER_EVENT_AXIS_SOURCE = 1 << 5,
       POINTER_EVENT_AXIS_STOP = 1 << 6,
       POINTER_EVENT_AXIS_DISCRETE = 1 << 7,
};

struct pointer_event {
       uint32_t event_mask;
       wl_fixed_t surface_x, surface_y;
       uint32_t button, state;
       uint32_t time;
       uint32_t serial;
       struct {
               bool valid;
               wl_fixed_t value;
               int32_t discrete;
       } axes[2];
       uint32_t axis_source;
};

/* Wayland code */
struct client_state {
    /* Globals */
    struct wl_display *wl_display;
    struct wl_registry *wl_registry;
    struct wl_shm *wl_shm;
    struct wl_compositor *wl_compositor;
    struct xdg_wm_base *xdg_wm_base;
    struct wl_seat *wl_seat;
    /* Objects */
    struct wl_surface *wl_surface;
    struct xdg_surface *xdg_surface;
    struct xdg_toplevel *xdg_toplevel;
    struct wl_keyboard *wl_keyboard;
    struct wl_pointer *wl_pointer;
    struct wl_touch *wl_touch;
    /* State */
    float offset;
    uint32_t last_frame;
    struct pointer_event pointer_event;
};

void
wl_pointer_enter(void *data, struct wl_pointer *wl_pointer,
               uint32_t serial, struct wl_surface *surface,
               wl_fixed_t surface_x, wl_fixed_t surface_y)
{
       struct client_state *client_state = (struct client_state*)data;
       client_state->pointer_event.event_mask |= POINTER_EVENT_ENTER;
       client_state->pointer_event.serial = serial;
       client_state->pointer_event.surface_x = surface_x,
               client_state->pointer_event.surface_y = surface_y;
}

void
wl_pointer_leave(void *data, struct wl_pointer *wl_pointer,
               uint32_t serial, struct wl_surface *surface)
{
       struct client_state *client_state = (struct client_state*)data;
       client_state->pointer_event.serial = serial;
       client_state->pointer_event.event_mask |= POINTER_EVENT_LEAVE;
}

void
wl_pointer_motion(void *data, struct wl_pointer *wl_pointer, uint32_t time,
               wl_fixed_t surface_x, wl_fixed_t surface_y)
{
       struct client_state *client_state = (struct client_state*)data;
       client_state->pointer_event.event_mask |= POINTER_EVENT_MOTION;
       client_state->pointer_event.time = time;
       client_state->pointer_event.surface_x = surface_x,
               client_state->pointer_event.surface_y = surface_y;
}

void
wl_pointer_button(void *data, struct wl_pointer *wl_pointer, uint32_t serial,
               uint32_t time, uint32_t button, uint32_t state)
{
       struct client_state *client_state = (struct client_state*)data;
       client_state->pointer_event.event_mask |= POINTER_EVENT_BUTTON;
       client_state->pointer_event.time = time;
       client_state->pointer_event.serial = serial;
       client_state->pointer_event.button = button,
               client_state->pointer_event.state = state;
}

void
wl_pointer_axis(void *data, struct wl_pointer *wl_pointer, uint32_t time,
               uint32_t axis, wl_fixed_t value)
{
       struct client_state *client_state = (struct client_state*)data;
       client_state->pointer_event.event_mask |= POINTER_EVENT_AXIS;
       client_state->pointer_event.time = time;
       client_state->pointer_event.axes[axis].valid = true;
       client_state->pointer_event.axes[axis].value = value;
}

void
wl_pointer_axis_source(void *data, struct wl_pointer *wl_pointer,
               uint32_t axis_source)
{
       struct client_state *client_state = (struct client_state*)data;
       client_state->pointer_event.event_mask |= POINTER_EVENT_AXIS_SOURCE;
       client_state->pointer_event.axis_source = axis_source;
}

void
wl_pointer_axis_stop(void *data, struct wl_pointer *wl_pointer,
               uint32_t time, uint32_t axis)
{
       struct client_state *client_state = (struct client_state*)data;
       client_state->pointer_event.time = time;
       client_state->pointer_event.event_mask |= POINTER_EVENT_AXIS_STOP;
       client_state->pointer_event.axes[axis].valid = true;
}

void
wl_pointer_axis_discrete(void *data, struct wl_pointer *wl_pointer,
               uint32_t axis, int32_t discrete)
{
       struct client_state *client_state = (struct client_state*)data;
       client_state->pointer_event.event_mask |= POINTER_EVENT_AXIS_DISCRETE;
       client_state->pointer_event.axes[axis].valid = true;
       client_state->pointer_event.axes[axis].discrete = discrete;
}

void
wl_pointer_frame(void *data, struct wl_pointer *wl_pointer)
{
       struct client_state *client_state = (struct client_state*)data;
       struct pointer_event *event = &client_state->pointer_event;
       fprintf(stderr, "pointer frame @ %d: ", event->time);

       if (event->event_mask & POINTER_EVENT_ENTER) {
               fprintf(stderr, "entered %f, %f ",
                               wl_fixed_to_double(event->surface_x),
                               wl_fixed_to_double(event->surface_y));
       }

       if (event->event_mask & POINTER_EVENT_LEAVE) {
               fprintf(stderr, "leave");
       }

       if (event->event_mask & POINTER_EVENT_MOTION) {
               fprintf(stderr, "motion %f, %f ",
                               wl_fixed_to_double(event->surface_x),
                               wl_fixed_to_double(event->surface_y));
       }

       if (event->event_mask & POINTER_EVENT_BUTTON) {
               const char *state = event->state == WL_POINTER_BUTTON_STATE_RELEASED ?
                       "released" : "pressed";
               fprintf(stderr, "button %d %s ", event->button, state);
       }

       static const uint32_t axis_events = POINTER_EVENT_AXIS
               | POINTER_EVENT_AXIS_SOURCE
               | POINTER_EVENT_AXIS_STOP
               | POINTER_EVENT_AXIS_DISCRETE;
       static const char *axis_name[2] = {
               [WL_POINTER_AXIS_VERTICAL_SCROLL] = "vertical",
               [WL_POINTER_AXIS_HORIZONTAL_SCROLL] = "horizontal",
       };
       static const char *axis_source[4] = {
               [WL_POINTER_AXIS_SOURCE_WHEEL] = "wheel",
               [WL_POINTER_AXIS_SOURCE_FINGER] = "finger",
               [WL_POINTER_AXIS_SOURCE_CONTINUOUS] = "continuous",
               [WL_POINTER_AXIS_SOURCE_WHEEL_TILT] = "wheel tilt",
       };
       if (event->event_mask & axis_events) {
               for (size_t i = 0; i < 2; ++i) {
                       if (!event->axes[i].valid) {
                               continue;
                       }
                       fprintf(stderr, "%s axis ", axis_name[i]);
                       if (event->event_mask & POINTER_EVENT_AXIS) {
                               fprintf(stderr, "value %f ", wl_fixed_to_double(
                                                       event->axes[i].value));
                       }
                       if (event->event_mask & POINTER_EVENT_AXIS_DISCRETE) {
                               fprintf(stderr, "discrete %d ",
                                               event->axes[i].discrete);
                       }
                       if (event->event_mask & POINTER_EVENT_AXIS_SOURCE) {
                               fprintf(stderr, "via %s ",
                                               axis_source[event->axis_source]);
                       }
                       if (event->event_mask & POINTER_EVENT_AXIS_STOP) {
                               fprintf(stderr, "(stopped) ");
                       }
               }
       }

       fprintf(stderr, "\n");
       memset(event, 0, sizeof(*event));
}
