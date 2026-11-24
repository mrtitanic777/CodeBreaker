/* ppexpn_state.h -- the variables of ppexpn.cpp for one run of the compiler: the run holds them as a member (see the run header) */
#pragma once
#include "cvars.h"
#include <limits.h>
#include <errno.h>
#include "i64.h"
#include "cmacadd.h"
#include "ppexpn.h"
#include "cgdefs.h"
#include "feprotos.h"

typedef struct ppvalue {
    union {
        unsigned_64     uval;
        signed_64       sval;
    } u;
    bool        no_sign;
} ppvalue;
typedef struct loc_info {
    int         pos;        // incremented when token read
    source_loc  locn;       // token location in source file
} loc_info;
typedef struct operand_stack {
    struct operand_stack *next;
    ppvalue     value;
    loc_info    loc;
} PPEXPN_OPERAND_STACK;
typedef struct operator_stack {
    struct operator_stack *next;
    TOKEN       token;
    loc_info    loc;
    int         prec;
} PPEXPN_OPERATOR_STACK;
typedef struct stack {
    struct stack    *next;
} STACK;

struct PpexpnState {
    PPEXPN_OPERAND_STACK *HeadOperand = NULL;
    PPEXPN_OPERATOR_STACK *HeadOperator = NULL;
    int Pos = 0;   // position of CurToken in parsing
    TOKEN PpNextToken_prev_token;
};
