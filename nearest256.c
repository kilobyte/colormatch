#include <stdio.h>
#include "color_distance.h"
#include "256.h"

static int closest(int rgb)
{
    int best, bd = 0x7fffffff;

    for (int i = 0; i < 256; ++i)
    {
        int d = color_diff(col256_to_rgb(i), rgb);
        if (d < bd)
            best = i, bd = d;
    }
    return best;
}

int main()
{
    printf("%d\n", closest(0x0000a0));
    return 0;
}
