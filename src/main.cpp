#define _POSIX_C_SOURCE 200112L
#include "wayland/wayland_surface.h"
#include "wayland/wayland_registry.h"
#include <xkbcommon/xkbcommon.h>
#include "xdg-shell-client-protocol.h"

void wayland_render_loop() {
  struct client_state state;
  state.width = 640, state.height = 480;
  state.wl_display = wl_display_connect(NULL);
  state.wl_registry = wl_display_get_registry(state.wl_display);
  state.xkb_context = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
  wl_registry_add_listener(state.wl_registry, &wl_registry_listener, &state);
  wl_display_roundtrip(state.wl_display);

  state.wl_surface1 = wl_compositor_create_surface(state.wl_compositor);
  state.xdg_surface1 = xdg_wm_base_get_xdg_surface(
          state.xdg_wm_base, state.wl_surface1);

  xdg_surface_add_listener(state.xdg_surface1, &xdg_surface_listener, &state);
  state.xdg_toplevel = xdg_surface_get_toplevel(state.xdg_surface1);
  xdg_toplevel_add_listener(state.xdg_toplevel,
      &xdg_toplevel_listener, &state);
  xdg_toplevel_set_title(state.xdg_toplevel, "Example client");
  wl_surface_commit(state.wl_surface1);

  struct wl_callback *cb = wl_surface_frame(state.wl_surface1);
  wl_callback_add_listener(cb, &wl_surface_frame_listener, &state);

  while (wl_display_dispatch(state.wl_display) && not(state.closed)) {
  }
}

int
main(int argc, char *argv[])
{
  wayland_render_loop();
  return 0;
}
