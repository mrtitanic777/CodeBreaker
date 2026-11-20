/* omf.c: the object module (OMF) - its records' common parts, the 8087 fixups' names, the segments */
#include "cell.h"
#include "seg.h"
struct rseg;
char *g_116e[] = {             /* its what-string, then four names - its authors', it seems - each byte doubled */
    "@(#)omf.c:1.17",
    "\232\322\350\306\320@\220\302\344\350\312\344",
    "\210\302\334@\234\312\356\312\330\330",
    "\244\302\330\340\320@\244\362\302\334",
    "\220\302\334\346@\246\340\322\330\330\302\344",
};
int g_1178 = 1;
int g_117a = 1;
int g_117c = 0;
int g_117e = 0;
int g_1180 = 83;
int g_1182 = 1;
int g_1184 = 1;
int g_1186 = 1;
int g_1188 = 1;
int g_118a = 1;
int g_118c = 1;
int g_118e = 0;
int g_1190 = 0;
struct cell *g_1192 = 0;
struct cell *g_1194 = 0;
int g_1196 = 0;
struct { char *name; int x; } g_1198[] = {      /* the 8087 fixups' names */
    { "FIWRQQ", 1 },
    { "FIDRQQ", 2 },
    { "FIERQQ", 3 },
    { "FISRQQ", 4 },
    { "FJSRQQ", -1 },
};
int g_11ac = 0;
struct cell *g_11ae = 0;
struct cell *g_11b0 = 0;
struct cell *g_11b2 = 0;
struct seg *g_11b4 = 0;
struct rseg *g_11b6 = 0;
extern struct seg g_1272, g_128a, g_12a2, g_12ba;   /* (omf8086.c's: the segments) */
struct seg *g_11b8 = &g_12ba;
int g_11ba = 0;
struct seg *g_11bc = &g_12ba;
struct seg *g_11be = &g_12a2;
struct seg *g_11c0 = &g_128a;
struct seg *g_11c2 = &g_1272;
