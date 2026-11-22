/* ctype_state.h -- variables of one file of the compiler that other files read, kept for the run (run/cbrun.hpp) */
#pragma once
#include "cvars.h"
#include "i64.h"

/* the variables of ctype_state.h, one set for each run of the compiler (run/cbrun.hpp) */
struct CtypeState {
    TYPEPTR CTypeHash[DATA_TYPE_SIZE];
    TYPEPTR PtrTypeHash[DATA_TYPE_SIZE];
    TAGPTR TagHash[ID_HASH_SIZE + 1];
    FIELDPTR FieldHash[ID_HASH_SIZE];
};
inline CtypeState& ctype_state() { return cbrun_part<CtypeState>(); }
