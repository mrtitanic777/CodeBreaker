/* cmdscan_state.h -- the variables of cmdscan.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include <ctype.h>
#include <stdlib.h>
#include "bool.h"
#include "cmdscan.h"

typedef struct cmd_scan_ctl {
    char const  *curr_ptr;      // current scan position
    char const  *switch_ptr;    // start of current switch
    bool        unix_mode;      // scan mode
} cmd_scan_ctl;

struct CmdscanState {
    cmd_scan_ctl cmd;
};
