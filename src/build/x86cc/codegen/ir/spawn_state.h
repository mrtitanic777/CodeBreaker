/* spawn_state.h -- the variables of spawn.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include <setjmp.h>
#include "jmpbuf.h"
#include "spawn.h"

struct SpawnState {
    jmp_buf *SpawnStack;
};
