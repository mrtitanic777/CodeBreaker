/* ideentry_state.h -- the variables of ideentry.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "cvars.h"
#include <stdlib.h>
#include <string.h>
    #include <malloc.h>
#include <setjmp.h>
#include "bool.h"
#include "jmpbuf.h"
#include "idedll.h"
#include "cgdefs.h"
#include "cgmisc.h"
#include "cgcli.h"
#include "feprotos.h"
#include "clibext.h"

struct IdeentryState {
    IDECBHdl IdeHdl;   // - handle for this instantiation
    IDECallBacks *IdeCbs;   // - call backs into IDE
    jmp_buf *FatalEnv;
};
