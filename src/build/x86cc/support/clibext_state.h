/* clibext_state.h -- the variables of clibext.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
    #include <stddef.h> /* need to load _comdef.h */
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include <errno.h>
    #include <time.h>
    #include <windows.h>
    #include "_dtaxxx.h"
    #include "ntext.h"
    #include "wio.h"
    #include <string.h>
    #include <ctype.h>
    #include <errno.h>
    #include <windows.h>
    #include "_dtaxxx.h"
    #include "ntext.h"
    #include "wio.h"
    #include "wreslang.h"
#include "clibint.h"
#include "clibext.h"

struct ClibextFileState {
#ifdef __WATCOMC__
#elif defined(_MSC_VER)
    char __altoptchar = '/';   /* alternate option character */
#endif
#ifdef __WATCOMC__
#elif defined(_MSC_VER)
    char __optchar;   /* matched option char ('-' or altoptchar) */
#endif
#ifdef __WATCOMC__
#elif defined(_MSC_VER)
    int opt_offset = 0;   /* position in currently parsed argument */
#endif
};
