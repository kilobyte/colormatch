#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "256.h"

static int sqrd(unsigned char a, unsigned char b)
{
    int _ = ((int)a) - ((int)b);
    return _*_;
}

static int rgb_diff(int x, int y)
{
    int rx = x>>16;
    int ry = y>>16;
    int gx = (x>>8)&0xff;
    int gy = (y>>8)&0xff;
    int bx = x&0xff;
    int by = y&0xff;
    int rm = (rx+ry)/2;
    return 2*sqrd(rx, ry)
         + 4*sqrd(gx, gy)
         + 3*sqrd(bx, by)
         + rm*(sqrd(rx, ry)-sqrd(bx, by))/256;
}

static int closest(int rgb)
{
    int best=0, bd = 0x7fffffff;

    for (int i = 0; i < 256; ++i)
    {
        int d = rgb_diff(col256_to_rgb(i), rgb);
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

    int fd = creat("riemersma.rgb", 0666);
    write(fd, nt, sizeof(nt));
    close(fd);

    return 0;
}
