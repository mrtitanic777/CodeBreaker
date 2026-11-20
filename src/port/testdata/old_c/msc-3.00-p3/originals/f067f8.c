extern int g_3a20[64];
extern int g_3572[64];
extern struct hsym *f067b5();

/* the symbol numbered num: a global one, else the function's (the value left in AX, as the original has it) */
f067f8(num)
{
    register p;

    if (!(p = f067b5(num, g_3a20)))
        f067b5(num, g_3572);
}
