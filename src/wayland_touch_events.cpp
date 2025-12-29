#include "wayland_touch_events.h"

static struct touch_point *
get_touch_point(struct client_state *client_state, int32_t id)
{
       struct touch_event *touch = &client_state->touch_event;
       const size_t nmemb = sizeof(touch->points) / sizeof(struct touch_point);
       int invalid = -1;
       for (size_t i = 0; i < nmemb; ++i) {
               if (touch->points[i].id == id) {
                       return &touch->points[i];
               }
               if (invalid == -1 && !touch->points[i].valid) {
                       invalid = i;
               }
       }
       if (invalid == -1) {
               return NULL;
       }
       touch->points[invalid].valid = true;
       touch->points[invalid].id = id;
       return &touch->points[invalid];
}

void
wl_touch_down(void *data, struct wl_touch *wl_touch, uint32_t serial,
               uint32_t time, struct wl_surface *surface, int32_t id,
               wl_fixed_t x, wl_fixed_t y)
{
       struct client_state *client_state = (struct client_state *)data;
       struct touch_point *point = get_touch_point(client_state, id);
       if (point == NULL) {
               return;
       }
       point->event_mask |= TOUCH_EVENT_UP;
       point->surface_x = wl_fixed_to_double(x),
               point->surface_y = wl_fixed_to_double(y);
       client_state->touch_event.time = time;
       client_state->touch_event.serial = serial;
}

void
wl_touch_up(void *data, struct wl_touch *wl_touch, uint32_t serial,
               uint32_t time, int32_t id)
{
       struct client_state *client_state = (struct client_state *)data;
       struct touch_point *point = get_touch_point(client_state, id);
       if (point == NULL) {
               return;
       }
       point->event_mask |= TOUCH_EVENT_UP;
}

void
wl_touch_motion(void *data, struct wl_touch *wl_touch, uint32_t time,
               int32_t id, wl_fixed_t x, wl_fixed_t y)
{
       struct client_state *client_state = (struct client_state*)data;
       struct touch_point *point = get_touch_point(client_state, id);
       if (point == NULL) {
               return;
       }
       point->event_mask |= TOUCH_EVENT_MOTION;
       point->surface_x = x, point->surface_y = y;
       client_state->touch_event.time = time;
}

void
wl_touch_cancel(void *data, struct wl_touch *wl_touch)
{
       struct client_state *client_state = (struct client_state*)data;
       client_state->touch_event.event_mask |= TOUCH_EVENT_CANCEL;
}

void
wl_touch_shape(void *data, struct wl_touch *wl_touch,
               int32_t id, wl_fixed_t major, wl_fixed_t minor)
{
       struct client_state *client_state = (struct client_state*)data;
       struct touch_point *point = get_touch_point(client_state, id);
       if (point == NULL) {
               return;
       }
       point->event_mask |= TOUCH_EVENT_SHAPE;
       point->major = major, point->minor = minor;
}

void
wl_touch_orientation(void *data, struct wl_touch *wl_touch,
               int32_t id, wl_fixed_t orientation)
{
       struct client_state *client_state = (struct client_state*)data;
       struct touch_point *point = get_touch_point(client_state, id);
       if (point == NULL) {
               return;
       }
       point->event_mask |= TOUCH_EVENT_ORIENTATION;
       point->orientation = orientation;
}

void
wl_touch_frame(void *data, struct wl_touch *wl_touch)
{
       struct client_state *client_state = (struct client_state*)data;
       struct touch_event *touch = &client_state->touch_event;
       const size_t nmemb = sizeof(touch->points) / sizeof(struct touch_point);
       fprintf(stderr, "touch event @ %d:\n", touch->time);

       for (size_t i = 0; i < nmemb; ++i) {
               struct touch_point *point = &touch->points[i];
               if (!point->valid) {
                       continue;
               }
               fprintf(stderr, "point %d: ", touch->points[i].id);

               if (point->event_mask & TOUCH_EVENT_DOWN) {
                       fprintf(stderr, "down %f,%f ",
                                       wl_fixed_to_double(point->surface_x),
                                       wl_fixed_to_double(point->surface_y));
               }

               if (point->event_mask & TOUCH_EVENT_UP) {
                       fprintf(stderr, "up ");
               }

               if (point->event_mask & TOUCH_EVENT_MOTION) {
                       fprintf(stderr, "motion %f,%f ",
                                       wl_fixed_to_double(point->surface_x),
                                       wl_fixed_to_double(point->surface_y));
               }

               if (point->event_mask & TOUCH_EVENT_SHAPE) {
                       fprintf(stderr, "shape %fx%f ",
                                       wl_fixed_to_double(point->major),
                                       wl_fixed_to_double(point->minor));
               }

               if (point->event_mask & TOUCH_EVENT_ORIENTATION) {
                       fprintf(stderr, "orientation %f ",
                                       wl_fixed_to_double(point->orientation));
               }

               point->valid = false;
               fprintf(stderr, "\n");
       }
}
