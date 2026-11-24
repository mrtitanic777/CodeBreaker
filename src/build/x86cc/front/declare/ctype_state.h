/* ctype_state.h -- the variables of ctype.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "cvars.h"
#include "i64.h"

typedef enum {
    M_NONE          = 0,
    M_CHAR          = 0x0001,
    M_INT           = 0x0002,
    M_SHORT         = 0x0004,
    M_LONG          = 0x0008,
    M_SIGNED        = 0x0010,
    M_UNSIGNED      = 0x0020,
    M_FLOAT         = 0x0040,
    M_DOUBLE        = 0x0080,
    M_LONG_LONG     = 0x0100,
    M_VOID          = 0x0200,
    M_COMPLEX       = 0x0400,
    M_IMAGINARY     = 0x0800,
    M_BOOL          = 0x1000,
} type_mask;

struct CtypeState {
    TYPEPTR CTypeHash[DATA_TYPE_SIZE];
    TYPEPTR PtrTypeHash[DATA_TYPE_SIZE];
    FIELDPTR FieldHash[ID_HASH_SIZE];
    field_level_stype GetFields_struct_level = 0;
};
