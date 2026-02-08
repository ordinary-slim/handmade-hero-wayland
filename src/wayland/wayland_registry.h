#pragma once
#include <wayland-client.h>

extern const struct wl_registry_listener wl_registry_listener;

void
registry_global(void *data, struct wl_registry *wl_registry,
        uint32_t name, const char *interface, uint32_t version);

void
registry_global_remove(void *data,
        struct wl_registry *wl_registry, uint32_t name);
