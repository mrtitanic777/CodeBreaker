#include "pool.h"
extern char g_19d0[], g_1dcc[], g_1fd8[], g_21e4[], g_23f0[];  /* (the pools' first blocks) */
char *g_06fe = "@(#)memallo.c:1.5";
struct pool g_0700[] = {
    { (char **)g_19d0, 1016, g_19d0 + 4, (char **)g_19d0, 1016, 1 },
    { (char **)g_1dcc, 520, g_1dcc + 4, (char **)g_1dcc, 520, 1 },
    { (char **)g_1fd8, 520, g_1fd8 + 4, (char **)g_1fd8, 520, 1 },
    { (char **)g_21e4, 520, g_21e4 + 4, (char **)g_21e4, 520, 1 },
    { (char **)g_23f0, 520, g_23f0 + 4, (char **)g_23f0, 520, 1 },
};
