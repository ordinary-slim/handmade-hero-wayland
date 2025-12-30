#include "wayland_registry.h"
#include "wayland_surface.h"
#include "client_state.h"
#include "xdg-shell-client-protocol.h"
#include <cstring>

const struct wl_registry_listener wl_registry_listener = {
    .global = registry_global,
    .global_remove = registry_global_remove,
};

void
registry_global(void *data, struct wl_registry *wl_registry,
        uint32_t name, const char *interface, uint32_t version)
{
    struct client_state *state = (struct client_state *)data;
    if (strcmp(interface, wl_shm_interface.name) == 0) {
        state->wl_shm = (struct wl_shm *)wl_registry_bind(
                wl_registry, name, &wl_shm_interface, 1);
    } else if (strcmp(interface, wl_compositor_interface.name) == 0) {
        state->wl_compositor = (struct wl_compositor *)wl_registry_bind(
                wl_registry, name, &wl_compositor_interface, 6);
    } else if (strcmp(interface, xdg_wm_base_interface.name) == 0) {
        state->xdg_wm_base = (struct xdg_wm_base *)wl_registry_bind(
                wl_registry, name, &xdg_wm_base_interface, 1);
        xdg_wm_base_add_listener(state->xdg_wm_base,
                &xdg_wm_base_listener, state);
    } else if (strcmp(interface, wl_seat_interface.name) == 0) {
            state->wl_seat = (struct wl_seat *)wl_registry_bind(
                            wl_registry, name, &wl_seat_interface, 9);
            wl_seat_add_listener(state->wl_seat,
                            &wl_seat_listener, state);
    }
}

void
registry_global_remove(void *data,
        struct wl_registry *wl_registry, uint32_t name)
{
    /* This space deliberately left blank */
}
