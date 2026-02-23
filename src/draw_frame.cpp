#include "draw_frame.h"

void
draw_frame(struct client_state *state)
{
    wayland_buffer buffer = state->buffers[0];
    /* Draw circle */
    for (int y = 0; y < buffer.height; ++y) {
        for (int x = 0; x < buffer.width; ++x) {
            double dist = sqrt((double)(pow(x - state->c_x, 2) + pow(y - state->c_y, 2)));
            if ((dist <= state->c_rad + state->c_wid) && (state->c_rad + state->c_wid))
                buffer.data[y * buffer.width + x] = 0xFF666666;
            else
                buffer.data[y * buffer.width + x] = 0xFFEEEEEE;
        }
    }
}
