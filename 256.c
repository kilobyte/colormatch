int col256_to_rgb(int i)
{
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
        return (i / 36 * 85 / 2) << 16
             | (i / 6 % 6 * 85 / 2) << 8
             | (i % 6 * 85 / 2);
    }
    // Grayscale ramp.
    else // i < 256
        return (i * 10 - 2312) * 0x010101;
}
