/* dwarf_state.h -- the variables of dwarf.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "cvars.h"
#include "browsio.h"
#include "cg.h"
#include "cgdefs.h"
#include "cgswitch.h"
#include "cgprotos.h"
#include "i64.h"

typedef enum
{   DC_RETURN           = 0x01,         // this is a return type
    DC_DEFINE           = 0x02,         // generate definition
    DC_DEFAULT          = 0x00          // default behaviour
} DC_CONTROL;

struct DwarfState {
    dw_client Client;
    dw_loc_handle dummyLoc;
    source_loc CurLoc;
    unsigned dwarfFile_Current_File_Index = ~0U;
};
