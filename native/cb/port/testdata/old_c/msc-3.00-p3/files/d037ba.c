/* main.c: the third pass's switches (getflags.c reads the table) and its files */
#include <stdio.h>
#include "opnd.h"
#include "ins.h"
#include "psym.h"
#include "getflags.h"
struct item;
char *g_0c4c = "@(#)main.c:1.39";
int g_0c4e = 0;
int g_0c50 = 0;
int g_0c52 = 0;
int g_0c54 = 0;
int g_0c56 = 0;
int g_0c58 = 0;
FILE *g_0c5a = 0;              /* (the object file) */
int g_0c5c = 0;
int g_0c5e = 0;
struct item *g_0c60 = 0;
int g_0c62 = 0;
int g_0c64 = 0;
int g_0c66 = 0;
char *g_0c68 = "";
int g_0c6a = 0;
int g_0c6c = 0;
int g_0c6e = 0;
int g_0c70 = 0;
int g_0c72 = 0;
int g_0c74 = 0;
int g_0c76 = 0;
int g_0c78 = 0;
int g_0c7a = 0;
int g_0c7c = 0;
char *g_0c7e = 0;
int g_0c80 = 0;
int g_0c82 = 1;
int g_0c84 = 1;
int g_0c86 = 1;
int g_0c88 = 1;
int g_0c8a = -1;
int g_0c8c = 0;
int g_0c8e = 0;
int g_0c90 = 0;
int g_0c92 = 0;
int g_0c94 = 0;
int g_0c96 = 0;
int g_0c98 = 1;
int g_0c9a = 0;
int g_0c9c = 0;
int g_0c9e = 0;
int g_0ca0 = 0;
int g_0ca2 = 0;
int g_0ca4 = 0;
int g_0ca6 = 0;
long g_0ca8 = 0L;
int g_0cac = 0;
int g_0cae = 0;
int g_0cb0 = 0;
int g_0cb2 = 0;
long g_0cb4 = 0L;
int g_0cb8 = 0;
int g_0cba = 0;
int g_0cbc = 0;
int g_0cbe = 0;
int g_0cc0 = 0;
int g_0cc2 = 0;
struct psym *g_0cc4 = 0;
struct ins g_0cc6 = { 0 };
static struct sub osubs[] = {
    { 'd', 5, &g_0c82 },
    { 'a', 1, &g_0c92 },
    { 's', 5, &g_0c96 },
    { 't', 1, &g_0c96 },
    { '1', 5, &g_0c84 },
    { '2', 5, &g_0c86 },
    { '3', 5, &g_0c88 },
    { 0 },
};
struct flagdef g_0d04[] = {
    { "-L", (intptr_t *)&g_0c72, 1, 0x42 },
    { "-A#", (intptr_t *)&g_0c66, 1, 0x42 },
    { "-MSDOS", (intptr_t *)&g_0c8c, 1, 0x01 },
    { "-ND", (intptr_t *)&g_0c6c, 1, 0x42 },
    { "-NGT", (intptr_t *)&g_0c7a, 1, 0x42 },
    { "-NM", (intptr_t *)&g_0c76, 1, 0x42 },
    { "-NT", (intptr_t *)&g_0c7c, 1, 0x42 },
    { "-CC", (intptr_t *)&g_0c64, 1, 0x42 },
    { "-O*", (intptr_t *)osubs, 1, 0x43 },
    { "-PLM", (intptr_t *)&g_0c94, 1, 0x01 },
    { "-S", (intptr_t *)&g_0c9c, 1, 0x01 },
    { "-SA#", (intptr_t *)&g_0c62, 1, 0x42 },
    { "-Zd", (intptr_t *)&g_0c9a, 1, 0x01 },
    { "-c", (intptr_t *)&g_0c78, 1, 0x42 },
    { "-d", (intptr_t *)&g_0c9e, 1, 0x01 },
    { "-db#", (intptr_t *)&g_0c6e, 1, 0x42 },
    { "-il#", (intptr_t *)&g_0c68, 1, 0x42 },
    { "-nl#", (intptr_t *)&g_0c8e, 1, 0x44 },
    { "-noalias", (intptr_t *)&g_0c92, 1, 0x01 },
    { "-source", (intptr_t *)&g_0c74, 1, 0x42 },
    { "-profile#", (intptr_t *)&g_0ca2, 1, 0x44 },
    { "-FPa", (intptr_t *)&g_0cba, 1, 0x01 },
    { "-FPc", (intptr_t *)&g_0cbc, 1, 0x01 },
    { "-FPc87", (intptr_t *)&g_0cbe, 1, 0x01 },
    { "-FPi87", (intptr_t *)&g_0cc0, 1, 0x01 },
    { "-FPi", (intptr_t *)&g_0cc2, 1, 0x01 },
    { "-Bnc", (intptr_t *)&g_0cb8, 1, 0x01 },
    { "*", (intptr_t *)&g_0c7e, 5, 0x42 },
    { 0 },
};
