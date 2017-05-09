#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "color_distance.h"
#include "256.h"

static int closest(int rgb)
{
    int best=0, bd = 0x7fffffff;

    for (int i = 0; i < 256; ++i)
    {
        int d = color_diff(col256_to_rgb(i), rgb);
        if (d < bd)
            best = i, bd = d;
    }
    return best;
}

static unsigned char nt[16777216];

int main()
{
    for (int c = 0; c < 16777216; ++c)
    {
        nt[c] = closest(c);
        if (!(c&0xff))
            printf("%06X\n", c);
    }

    int fd = creat("best.rgb", 0666);
    write(fd, nt, sizeof(nt));
    close(fd);

    return 0;
}
