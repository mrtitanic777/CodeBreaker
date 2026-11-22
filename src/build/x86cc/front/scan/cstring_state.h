/* cstring_state.h -- the variables of cstring.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "cvars.h"
#include "wio.h"
#include "watcom.h"
#include "cgswitch.h"
#include "scan.h"
#include "asciiout.h"
#include "unicode.h"
#include "escchars.h"
#include "toggles.h"
#include "clibext.h"

struct CstringState {
    target_size CLitLength;   /* length of string literal */
    const unsigned char *pbuf = NULL;
};
