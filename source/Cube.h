#pragma once

#define TEXTURE_WIDTH  8
#define TEXTURE_HEIGHT 8

UINT TextureData[] =
{
    0xffffffff, 0xff7f7f7f, 0xffffffff, 0xff7f7f7f, 0xffffffff, 0xff7f7f7f, 0xffffffff, 0xff7f7f7f,
    0xff7f7f7f, 0xffffffff, 0xff7f7f7f, 0xffffffff, 0xff7f7f7f, 0xffffffff, 0xff7f7f7f, 0xffffffff,
    0xffffffff, 0xff7f7f7f, 0xffffffff, 0xff7f7f7f, 0xffffffff, 0xff7f7f7f, 0xffffffff, 0xff7f7f7f,
    0xff7f7f7f, 0xffffffff, 0xff7f7f7f, 0xffffffff, 0xff7f7f7f, 0xffffffff, 0xff7f7f7f, 0xffffffff,
    0xffffffff, 0xff7f7f7f, 0xffffffff, 0xff7f7f7f, 0xffffffff, 0xff7f7f7f, 0xffffffff, 0xff7f7f7f,
    0xff7f7f7f, 0xffffffff, 0xff7f7f7f, 0xffffffff, 0xff7f7f7f, 0xffffffff, 0xff7f7f7f, 0xffffffff,
    0xffffffff, 0xff7f7f7f, 0xffffffff, 0xff7f7f7f, 0xffffffff, 0xff7f7f7f, 0xffffffff, 0xff7f7f7f,
    0xff7f7f7f, 0xffffffff, 0xff7f7f7f, 0xffffffff, 0xff7f7f7f, 0xffffffff, 0xff7f7f7f, 0xffffffff,
};

typedef struct {
    float position[3];
    float texcoord[2];
    float normal[3];
} Vertex;

// Borrowed from DevkitPro:
// https://github.com/devkitPro/3ds-examples/blob/master/graphics/gpu/textured_cube/source/main.c#L17
Vertex VertexData[] = {
    // First face (PZ)
    // First triangle
    { {-0.5f, -0.5f, +0.5f}, {0.0f, 0.0f}, {0.0f, 0.0f, +1.0f} },
    { {+0.5f, -0.5f, +0.5f}, {1.0f, 0.0f}, {0.0f, 0.0f, +1.0f} },
    { {+0.5f, +0.5f, +0.5f}, {1.0f, 1.0f}, {0.0f, 0.0f, +1.0f} },
    // Second triangle
    { {+0.5f, +0.5f, +0.5f}, {1.0f, 1.0f}, {0.0f, 0.0f, +1.0f} },
    { {-0.5f, +0.5f, +0.5f}, {0.0f, 1.0f}, {0.0f, 0.0f, +1.0f} },
    { {-0.5f, -0.5f, +0.5f}, {0.0f, 0.0f}, {0.0f, 0.0f, +1.0f} },

    // Second face (MZ)
    // First triangle
    { {-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f} },
    { {-0.5f, +0.5f, -0.5f}, {1.0f, 0.0f}, {0.0f, 0.0f, -1.0f} },
    { {+0.5f, +0.5f, -0.5f}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f} },
    // Second triangle
    { {+0.5f, +0.5f, -0.5f}, {1.0f, 1.0f}, {0.0f, 0.0f, -1.0f} },
    { {+0.5f, -0.5f, -0.5f}, {0.0f, 1.0f}, {0.0f, 0.0f, -1.0f} },
    { {-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {0.0f, 0.0f, -1.0f} },

    // Third face (PX)
    // First triangle
    { {+0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {+1.0f, 0.0f, 0.0f} },
    { {+0.5f, +0.5f, -0.5f}, {1.0f, 0.0f}, {+1.0f, 0.0f, 0.0f} },
    { {+0.5f, +0.5f, +0.5f}, {1.0f, 1.0f}, {+1.0f, 0.0f, 0.0f} },
    // Second triangle
    { {+0.5f, +0.5f, +0.5f}, {1.0f, 1.0f}, {+1.0f, 0.0f, 0.0f} },
    { {+0.5f, -0.5f, +0.5f}, {0.0f, 1.0f}, {+1.0f, 0.0f, 0.0f} },
    { {+0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {+1.0f, 0.0f, 0.0f} },

    // Fourth face (MX)
    // First triangle
    { {-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f} },
    { {-0.5f, -0.5f, +0.5f}, {1.0f, 0.0f}, {-1.0f, 0.0f, 0.0f} },
    { {-0.5f, +0.5f, +0.5f}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f} },
    // Second triangle
    { {-0.5f, +0.5f, +0.5f}, {1.0f, 1.0f}, {-1.0f, 0.0f, 0.0f} },
    { {-0.5f, +0.5f, -0.5f}, {0.0f, 1.0f}, {-1.0f, 0.0f, 0.0f} },
    { {-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f} },

    // Fifth face (PY)
    // First triangle
    { {-0.5f, +0.5f, -0.5f}, {0.0f, 0.0f}, {0.0f, +1.0f, 0.0f} },
    { {-0.5f, +0.5f, +0.5f}, {1.0f, 0.0f}, {0.0f, +1.0f, 0.0f} },
    { {+0.5f, +0.5f, +0.5f}, {1.0f, 1.0f}, {0.0f, +1.0f, 0.0f} },
    // Second triangle
    { {+0.5f, +0.5f, +0.5f}, {1.0f, 1.0f}, {0.0f, +1.0f, 0.0f} },
    { {+0.5f, +0.5f, -0.5f}, {0.0f, 1.0f}, {0.0f, +1.0f, 0.0f} },
    { {-0.5f, +0.5f, -0.5f}, {0.0f, 0.0f}, {0.0f, +1.0f, 0.0f} },

    // Sixth face (MY)
    // First triangle
    { {-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f} },
    { {+0.5f, -0.5f, -0.5f}, {1.0f, 0.0f}, {0.0f, -1.0f, 0.0f} },
    { {+0.5f, -0.5f, +0.5f}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f} },
    // Second triangle
    { {+0.5f, -0.5f, +0.5f}, {1.0f, 1.0f}, {0.0f, -1.0f, 0.0f} },
    { {-0.5f, -0.5f, +0.5f}, {0.0f, 1.0f}, {0.0f, -1.0f, 0.0f} },
    { {-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f}, {0.0f, -1.0f, 0.0f} },
};
