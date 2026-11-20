#include "pool.h"
extern char g_2670[], g_287c[], g_2a88[], g_2c94[], g_2ea0[];  /* (the pools' first blocks) */
char *g_10ce = "@(#)memallo.c:1.5";
struct pool g_10d0[] = {
    { (char **)g_2670, 520, g_2670 + 4, (char **)g_2670, 520, 1 },
    { (char **)g_287c, 520, g_287c + 4, (char **)g_287c, 520, 1 },
    { (char **)g_2a88, 520, g_2a88 + 4, (char **)g_2a88, 520, 1 },
    { (char **)g_2c94, 520, g_2c94 + 4, (char **)g_2c94, 520, 1 },
    { (char **)g_2ea0, 8, g_2ea0 + 4, (char **)g_2ea0, 8, 1 },
};
