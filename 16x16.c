#include <stdio.h>
#include "color_distance.h"
#include "256.h"

static void pc(int c)
{
    int v = col256_to_rgb(c);
    printf("%d;%d;%d", (v>>16)&255, (v>>8)&255, v&255);
}

int main()
{
    printf("     ");
    for (int x=0; x<16; ++x)
    {
        printf("\e[48;2;");
        pc(x);
        printf(";3%d;4m%3d \e[0m", (x&8)?0:7, x);
    }
    printf("\n");

    for (int y=0; y<16; ++y)
    {
        printf("\e[48;2;");
        pc(y);
        printf(";3%dm%3d \e[0m ", (y&8)?0:7, y);

        for (int x=0; x<16; ++x)
        {
            printf("\e[0;38;2;");
            pc(x);
            printf(";48;2;");
            pc(y);
            printf("m%4d\e[0m", (color_diff(col256_to_rgb(x), col256_to_rgb(y))
                          +16777216/2)/16777216);
        }

        int v = col256_to_rgb(y);
        printf(" %02x%02x%02x\n", (v>>16)&255, (v>>8)&255, v&255);
    }

    return 0;
}
