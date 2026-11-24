/* x86obj_state.h -- the variables of x86obj.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "_cgstd.h"
#include "coderep.h"
#include "model.h"
#include "x86objd.h"
#include "objout.h"
#include "x86obj.h"
#include "x86emit.h"

struct X86objState {
    const x86_obj_emitter *Emitter = &X86OmfEmitter;
};
