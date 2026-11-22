/* cgetch_state.h -- the variables of cgetch.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "cvars.h"
#include <errno.h>
#include "wio.h"
#include "scan.h"

struct tri_graph {
    char        tri;
    char        new_;
};

struct CgetchState {
    struct tri_graph TriGraphs[10] = {
        { '=', '#' },
        { '(', '[' },
        { '/', '\\'},
        { ')', ']' },
        { '\'','^' },
        { '<', '{' },
        { '!', '|' },
        { '>', '}' },
        { '-', '~' },
        { '\0','\0'}
    };
    int LastChar;
    int Blank1Count;
    int Blank2Count;
    int Tab1Count;
    int IncFileDepth;
};
