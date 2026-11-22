/* temps_state.h -- the variables of temps.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "_cgstd.h"
#include "coderep.h"
#include "cgmem.h"
#include "cgaux.h"
#include "data.h"
#include "zoiks.h"
#include "targetin.h"
#include "cgsrtlst.h"
#include "insdead.h"
#include "namelist.h"
#include "optab.h"
#include "temps.h"
#include "varusage.h"
#include "feprotos.h"

typedef struct stack_temp {
        struct stack_temp       *others;
        instruction_id          first;
        instruction_id          last;
} stack_temp;
typedef struct stack_entry {
        struct stack_temp       temp;
        struct stack_entry      *link;
        type_length             location;
        type_length             size;
} stack_entry;

struct TempsState {
    stack_entry *StackMap;
};
