/* ccmain_state.h -- the variables of ccmain.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "cvars.h"
#include <stdarg.h>
#include <stdio.h>
#include <errno.h>
#include <sys/stat.h>
#include "iopath.h"
#include "scan.h"
#include "jmpbuf.h"
#include "cgdefs.h"
#include "feprotos.h"
#include "swchar.h"
#include "cfeinfo.h"
#include "pathlist.h"
#include "cmacadd.h"
#include "pathgrp2.h"
#include "cscanbuf.h"
#include "clibint.h"
#include "clibext.h"

struct CcmainState {
    bool IsStdIn;
    bool IsNullDev;
    char *FNameBuf = NULL;
    unsigned CppColumn = 0;
    unsigned CppWidth = 0;
    unsigned CommentChar = '\0';
    bool CppFirstChar = false;
};
