#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "color_distance.h"
#include "256.h"

static unsigned char nt[16777216];

int main(int argc, char **argv)
{
    if (argc!=2)
        fprintf(stderr, "Usage: rate file.rgb\n"), exit(1);

    int fd = open(argv[1], O_RDONLY);
    if (read(fd, nt, sizeof(nt))!=sizeof(nt))
        perror("Can't read table file"), exit(1);
    close(fd);

    int maxe=0;
    long long ss=0;
    int worst=0;

    for (int i = 0; i < 16777216; ++i)
    {
        int d = color_diff(i, col256_to_rgb(nt[i]));
        if (d > maxe)
            maxe = d, worst = i;
        ss += ((long long)d) * d / 16777216;
    }

    printf("%-15s maxd=%#-7.6g (\e[38;2;%d;%d;%dm#%06X\e[0m vs \e[38;5;%dm%3d\e[0m:\e[38;2;%d;%d;%dm#%06X\e[0m) Σd²=%#-8.6g\n",
        argv[1],
        maxe/16777216.0,
        (worst>>16)&0xff, (worst>>8)&0xff, worst&0xff, worst,
        nt[worst], nt[worst],
        (col256_to_rgb(nt[worst])>>16)&0xff, (col256_to_rgb(nt[worst])>>8)&0xff, col256_to_rgb(nt[worst])&0xff,
        col256_to_rgb(nt[worst]),
        ss/16777216.0);

    return 0;
}
