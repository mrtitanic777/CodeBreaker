/* omf8086.c: the 8086's object module - its default libraries, its segments (_TEXT, _DATA, CONST, _BSS), the
   name the startup code asks for */
#include "psym.h"
#include "seg.h"
#include "grp.h"
extern struct grp g_2f8e, g_2f94;
char *g_123c = "@(#)omf8086.c:1.20";
int g_123e = 0;
char g_1240[4] = { 0 };          /* (the segments' group: none) */
struct grp *g_1244 = &g_2f8e;   /* (DGROUP) */
struct grp *g_1246 = &g_2f94;   /* (the code's group) */
char *g_1248 = "xLIBC";
char *g_124a = "xLIBFP";
char *g_124c = "xLIBFA";
char *g_124e = "EM";
char *g_1250 = "87";
struct psym g_1252 = { 0, 2, 0, 0, (int)"acrtused", 0, 0L, 1, 4, 0, 0, 0L, 0L, 0 };   /* (acrtused) */
struct seg g_1272 = { 0, "_BSS", "BSS", (int *)g_1240, 4, 0, 2, 2, 0L, 0, 0 };
struct seg g_128a = { &g_1272, "CONST", "CONST", (int *)g_1240, 3, 0, 2, 2, 0L, 0, 0 };
struct seg g_12a2 = { &g_128a, "_DATA", "DATA", (int *)g_1240, 2, 0, 2, 2, 0L, 0, 0 };
struct seg g_12ba = { &g_12a2, "_TEXT", "CODE", (int *)g_1240, 1, 0, 1, 2, 0L, 0, 0 };
