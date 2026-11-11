extern int g_0c90, g_2642, g_2646, g_2648, g_0c8a, g_2644;
extern f01756();

f01775(file, line)
int file, line;
{
    g_0c90++;
    if (file == g_2642 && line == g_2646 && g_2648 == g_0c8a) {
        if (++g_2644 > 25)
            f01756("Infinite spill loop\n", 0);
    } else {
        g_2644 = 0;
        g_2642 = file;
        g_2646 = line;
        g_2648 = g_0c8a;
    }
}
