/* main.c: the pass's switches (getflags.c reads the table), the lists of names they gather, the token file */
#include <stdio.h>
#include "getflags.h"
static char *sccsid = "@(#)main.c:1.13";
char *g_05d2 = "";              /* -il: the intermediate files' name */
int g_05d4 = 0;
intptr_t g_05d6 = 0;                 /* -C */
intptr_t g_05d8 = 0;                 /* -E */
intptr_t g_05da = 0;                 /* -P */
intptr_t g_05dc = 0;                 /* -xc */
intptr_t g_05de = 2;                 /* -W: the warning level */
int g_05e0 = 0;                 /* -Ze */
int g_05e2 = 0;                 /* -Zp */
int g_05e4 = 0;                 /* -Zd */
int g_05e6 = 0;                 /* -Zg */
FILE *g_05e8 = stdout;          /* the token file */
int g_05ea = 0x16f;             /* (?) */
struct slots g_05ec_s = { 20, { 0 } };	/* (g_05ec, g_05ee: one place, as the old linker kept them) */

struct slots g_0616 = { 20 };   /* -D: the names defined */
intptr_t g_0640 = 0;                 /* -db */
char *g_0642 = 0;               /* -f: the source file */
intptr_t g_0644 = 0;                 /* a switch not known */
static struct sub zsubs[] = {
    { 'p', 1, &g_05e2 },
    { 'e', 1, &g_05e0 },
    { 'd', 1, &g_05e4 },
    { 'g', 1, &g_05e6 },
    { 0 },
};
struct flagdef g_0664[] = {
    { "-C", &g_05d6, 1, 0x01 },
    { "-D#", (intptr_t *)&g_0616, 1, 0x46 },
    { "-E", &g_05d8, 1, 0x01 },
    { "-I#", (intptr_t *)&g_05ec, 1, 0x46 },
    { "-P", &g_05da, 1, 0x01 },
    { "-f", (intptr_t *)&g_0642, 1, 0x42 },
    { "-W#", &g_05de, 1, 0x44 },
    { "-Z*", (intptr_t *)zsubs, 1, 0x43 },
    { "-db#", &g_0640, 1, 0x42 },
    { "-il#", (intptr_t *)&g_05d2, 1, 0x42 },
    { "-xc", &g_05dc, 1, 0x01 },
    { "*", &g_0644, 0, 0x42 },
    { 0 },
};
