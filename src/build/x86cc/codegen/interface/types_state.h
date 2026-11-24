/* types_state.h -- the variables of types.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "_cgstd.h"
#include "coderep.h"
#include "cgmem.h"
#include "data.h"
#include "types.h"

typedef enum {
    TYPE_DEFINITION,
    TYPE_ALIAS
} type_class;
typedef struct alias {
    const struct type_def   *tptr;
    cg_type                 refno;
} alias;
typedef struct type_alloc {
    union {
        struct alias        alias;
        struct type_def     type_def;
    } u;
} type_alloc;
typedef struct type_list {
    type_alloc          tipe;
    struct type_list    *link;
    type_class          format;
} type_list;

struct TypesFileState {
    type_list *TypeList;
};
