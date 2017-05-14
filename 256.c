#include <stdlib.h>

int col256_to_rgb(int i)
{
    if (i<0 || i > 255)
        abort();

    // Standard colours.
    if (i < 16)
    {
        int c = (i&1 ? 0xaa0000 : 0x000000)
              | (i&2 ? 0x00aa00 : 0x000000)
              | (i&4 ? 0x0000aa : 0x000000);
        return (i >= 8) ? c + 0x555555 : c;
    }
    // 6x6x6 colour cube.
    else if (i < 232)
    {
        i -= 16;
        int r = i / 36, g = i / 6 % 6, b = i % 6;
        return (r ? r * 0x280000 + 0x370000 : 0)
             | (g ? g * 0x002800 + 0x003700 : 0)
             | (b ? b * 0x000028 + 0x000037 : 0);
    }
    // Grayscale ramp.
    else // i < 256
        return (i * 10 - 2312) * 0x010101;
}
