cmake_minimum_required(VERSION 3.23)
project(handmade-hero-wayland VERSION 1.0 LANGUAGES C CXX)

find_program(WAYLAND_SCANNER wayland-scanner REQUIRED)

set(XDG_SHELL_PROTOCOL_C ${CMAKE_SOURCE_DIR}/src/xdg-shell-protocol.c)
set(XDG_SHELL_CLIENT_PROTOCOL_H ${CMAKE_SOURCE_DIR}/src/xdg-shell-client-protocol.h)

# wayland-scanner private-code \ < /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml \ > xdg-shell-protocol.c wayland-scanner client-header \ < /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml \ > xdg-shell-client-protocol.h gcc -o main main.cpp alloc_shmpool.cpp xdg-shell-protocol.c -lwayland-client -lrt
add_custom_command(
    OUTPUT ${XDG_SHELL_PROTOCOL_C} ${XDG_SHELL_CLIENT_PROTOCOL_H}
    COMMAND ${WAYLAND_SCANNER} private-code
            < /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml > ${XDG_SHELL_PROTOCOL_C}
    COMMAND ${WAYLAND_SCANNER} client-header
            < /usr/share/wayland-protocols/stable/xdg-shell/xdg-shell.xml > ${XDG_SHELL_CLIENT_PROTOCOL_H}
    COMMENT "Generating Wayland XDG Shell protocol files"
)

add_executable(handmade-hero-wayland
    src/main.cpp
    src/alloc_shmpool.cpp
    ${XDG_SHELL_PROTOCOL_C}
)

target_link_libraries(handmade-hero-wayland
    PRIVATE
    wayland-client
    rt
)
