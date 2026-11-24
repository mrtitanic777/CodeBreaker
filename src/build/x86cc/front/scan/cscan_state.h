/* cscan_state.h -- the variables of cscan.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "cvars.h"
#include "scan.h"
#include "escchars.h"
#include "asciiout.h"
#include "i64.h"
#include "kwhash.h"
#include "unicode.h"
#include "cmacadd.h"
#include "cscanbuf.h"

struct CscanState {
    FCB rescan_fcb;
    unsigned char ClassTable[LCHR_MAX];
    TOKEN (*ScanFunc[14])( FrontRun& );
};
