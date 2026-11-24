/* cdump_state.h -- the variables of cdump.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "cvars.h"

typedef struct {
    char    *data;
    size_t  cursize;
    size_t  maxsize;
} STRCHUNK;

struct CdumpState {
    bool do_message_output;   /* Optimize output for human */
};
