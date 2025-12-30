#include "wayland_surface.h"
#include "xdg-shell-client-protocol.h"

const struct xdg_surface_listener xdg_surface_listener = {
    .configure = xdg_surface_configure,
};

const struct xdg_wm_base_listener xdg_wm_base_listener = {
    .ping = xdg_wm_base_ping,
};

const struct wl_callback_listener wl_surface_frame_listener = {
  .done = wl_surface_frame_done,
};

void
xdg_surface_configure(void *data,
        struct xdg_surface *xdg_surface, uint32_t serial)
{
    struct client_state *state = (struct client_state *)data;
    xdg_surface_ack_configure(xdg_surface, serial);

    if (state->first_frame) {
      center_circle(state);
      state->first_frame = false;
    }
    struct wl_buffer *buffer = draw_frame(state);
    wl_surface_attach(state->wl_surface1, buffer, 0, 0);
    wl_surface_commit(state->wl_surface1);
}

void
xdg_wm_base_ping(void *data, struct xdg_wm_base *xdg_wm_base, uint32_t serial)
{
    xdg_wm_base_pong(xdg_wm_base, serial);
}

void
wl_surface_frame_done(void *data, struct wl_callback *cb, uint32_t time);

void
wl_surface_frame_done(void *data, struct wl_callback *cb, uint32_t time)
{
  /* Destroy this callback */
  wl_callback_destroy(cb);

  /* Request another frame */
  struct client_state *state = (struct client_state *)data;
  cb = wl_surface_frame(state->wl_surface1);
  wl_callback_add_listener(cb, &wl_surface_frame_listener, state);

  /* Update scroll amount at 24 pixels per second */
  if (state->last_frame != 0) {
    int elapsed = time - state->last_frame;
    state->offset += elapsed / 1000.0 * 24;
  }

  /* Submit a frame for this event */
  struct wl_buffer *buffer = draw_frame(state);
  wl_surface_attach(state->wl_surface1, buffer, 0, 0);
  wl_surface_damage_buffer(state->wl_surface1, 0, 0, INT32_MAX, INT32_MAX);
  wl_surface_commit(state->wl_surface1);

  state->last_frame = time;
}

const struct xdg_toplevel_listener xdg_toplevel_listener = {
  .configure = xdg_toplevel_configure,
  .close = xdg_toplevel_close,
};

void
xdg_toplevel_configure(void *data,
    struct xdg_toplevel *xdg_toplevel, int32_t width, int32_t height,
    struct wl_array *states)
{
  struct client_state *state = (struct client_state *)data;
  if (width == 0 || height == 0) {
    /* Compositor is deferring to us */
    return;
  }
  state->width = width, state->height = height;
}

void
xdg_toplevel_close(void *data, struct xdg_toplevel *toplevel)
{
  struct client_state *state = (struct client_state *)data;
  state->closed = true;
}
