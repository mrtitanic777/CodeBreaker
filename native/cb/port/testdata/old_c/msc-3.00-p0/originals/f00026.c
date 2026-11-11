extern int g_09d6, g_19c6, g_19ca, g_19cc, g_09d8, g_19c8;
extern f00002();

f00026(file, line)
int file, line;
{
    g_09d6++;
    if (file == g_19c6 && line == g_19ca && g_19cc == g_09d8) {
        if (++g_19c8 > 25)
            f00002("Infinite spill loop\n", 0);
    } else {
        g_19c8 = 0;
        g_19c6 = file;
        g_19ca = line;
        g_19cc = g_09d8;
    }
}
