#include <stdio.h>
#include "color_distance.h"

int main()
{
    double a[3], b[3];
    rgb2lab(0xfffffe, a);
    printf("%g %g %g\n", a[0], a[1], a[2]);
    rgb2lab(0xffffff, b);
    printf("%g %g %g\n", b[0], b[1], b[2]);
    printf("dist=%g\n", deltaE2000(a,b));
    return 0;
}
