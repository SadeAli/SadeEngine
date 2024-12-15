#pragma once

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Color_t {
    int r, g, b, a;
} Color;

void clearBG(const Color c);

#ifdef __cplusplus
}
#endif
