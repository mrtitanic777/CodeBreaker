/* pgoMD.c: the post-optimizer's knowledge of the machine - the registers' halves, what each holds, its counts */
#include "opnd.h"
#include "regc.h"
#include "fcon.h"
char *g_1f7e = "@(#)pgoMD.c:1.56";
char *g_1f80[] = {             /* (for each register, the halves of it) */
    "\14\20\77", "\15\21\77", "\16\22\77", "\17\23\77",
    "\0\0", "\0\0", "\0\0", "\0\0",
    "\0\0", "\0\0", "\0\0", "\0\0",
    "\0\77", "\1\77", "\2\77", "\3\77",
    "\0\77", "\1\77", "\2\77", "\3\77",
};
struct regc *g_1fa8[20] = { 0 };  /* (for each register, what it holds) */
char g_1fd0[8] = { 2, 0, 0, 0, 0, 0, 0, 0 };
struct opnd *g_1fd8 = (struct opnd *)g_1fd0;   /* (the constants 0 and -1) */
char g_1fda[8] = { 2, 0, 0, 0, 255, 255, 0, 0 };
struct opnd *g_1fe2 = (struct opnd *)g_1fda;
struct regc *g_1fe4 = 0;       /* (the free ones) */
int g_1fe6 = 0;
int g_1fe8 = 0;
int g_1fea = 0;
int g_1fec = 0;
int g_1fee = 0;
int g_1ff0 = 0;
int g_1ff2 = 0;
int g_1ff4 = 0;
int g_1ff6 = 0;
int g_1ff8 = 0;
int g_1ffa = 0;
int g_1ffc = 0;
int g_1ffe = 0;
int g_2000 = 0;
int g_2002 = 0;
int g_2004 = 0;
int g_2006 = 0;
struct fcon *g_2008 = 0;       /* (the constants loaded) */
struct fcon *g_200a = 0;
