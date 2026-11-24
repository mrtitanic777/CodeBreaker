/* cmac1_state.h -- the variables of cmac1.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "cvars.h"
#include "i64.h"
#include "scan.h"
#include "cfeinfo.h"
#include "cmacadd.h"
#include "ppexpn.h"
#include "cscanbuf.h"
#include "dumpapi.h"
#include "clibext.h"

typedef enum exp_state {
    EXPANDABLE_NO       = 0,    // macro is currently not expandable
    EXPANDABLE_YES      = 1,    // macro is expandable
    EXPANDABLE_WSSKIP   = 2     // we skipped over some white space
} exp_state;
typedef struct macro_token {
    struct macro_token  *next;
    TOKEN               token;
    char                data[1];
} MACRO_TOKEN;
typedef struct macro_arg {
    char    *arg;
} MACRO_ARG;
typedef struct nested_macros {
    struct nested_macros    *next;
    MEPTR                   mentry;
    MACRO_ARG               *macro_parms;
    bool                    rescanning;
    bool                    substituting_parms;
} NESTED_MACRO;
typedef struct special_macro_names {
    const char      *name;
    special_macros  value;
    macro_flags     flags;
} special_macro_names;

struct Cmac1State {
    NESTED_MACRO *NestedMacros;
    MACRO_TOKEN *MTokenList;
    size_t TokenBufSize;
    struct special_macro_names SpcMacros[9] = {
    #define pick(s,i,f)     { s, i, f },
    #include "specmac.h"
    #undef pick
    };
};
