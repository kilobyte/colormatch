#include <stdio.h>
#include "color_distance.h"
#include "256.h"

static void pc(int c)
{
    int v = col256_to_rgb(c);
    printf("%d;%d;%d", (v>>16)&255, (v>>8)&255, v&255);
}

static int neg16(int c)
{
    if (c <= 8)
        return 97;
    return 30;
}

int main()
{
    printf("     ");
    for (int x=0; x<16; ++x)
    {
        printf("\e[48;2;");
        pc(x);
        printf(";%d;4m%3d \e[0m", neg16(x), x);
    }
    printf("\n");

    for (int y=0; y<16; ++y)
    {
        printf("\e[48;2;");
        pc(y);
        printf(";%dm%3d \e[0m ", neg16(y), y);

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
