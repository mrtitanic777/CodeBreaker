#include <stdio.h>
#include "incl.h"
extern int f01cd2(), f01d42();
static char *sccsid = "@(#)p0io.c:1.10";
char *g_09d2 = "Two pass";      /* (nothing uses it) */
char *g_09d4 = 0;
int g_09d6 = 0;
int g_09d8 = 0;
int (*g_09da)() = f01cd2;       /* (the readers of a character and of a line) */
int (*g_09dc)() = f01d42;
struct incl g_09de[10] = { 0 };
int g_0a42 = -1;
FILE *g_0a44 = 0;
