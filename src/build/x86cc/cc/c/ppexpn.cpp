/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2026 The Open Watcom Contributors. All Rights Reserved.
*    Portions Copyright (c) 1983-2002 Sybase, Inc. All Rights Reserved.
*
*  ========================================================================
*
*    This file contains Original Code and/or Modifications of Original
*    Code as defined in and that are subject to the Sybase Open Watcom
*    Public License version 1.0 (the 'License'). You may not use this file
*    except in compliance with the License. BY USING THIS FILE YOU AGREE TO
*    ALL TERMS AND CONDITIONS OF THE LICENSE. A copy of the License is
*    provided with the Original Code and Modifications, and is also
*    available at www.sybase.com/developer/opensource.
*
*    The Original Code and all software distributed under the License are
*    distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
*    EXPRESS OR IMPLIED, AND SYBASE AND ALL CONTRIBUTORS HEREBY DISCLAIM
*    ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF
*    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR
*    NON-INFRINGEMENT. Please see the License for the specific language
*    governing rights and limitations under the License.
*
*  ========================================================================
*
* Description:  Preprocessor expression evaluator for C compiler.
*
****************************************************************************/


#include "cvars.h"
#include <limits.h>
#include <errno.h>
#include "i64.h"
#include "cmacadd.h"
#include "ppexpn.h"
#include "cgdefs.h"
#include "feprotos.h"


#define I64isNegPP(a)       I64isNeg((a).u.sval)
#define U64isZeroPP(a)      U64isZero((a).u.uval)
#define U64isntZeroPP(a)    U64isntZero((a).u.uval)
#define U64isEqPP(a,b)      U64isEq((a).u.uval,(b).u.uval)

#define U64LowPP(a)         U64Low((a).u.uval)

#define Set64ValZeroPP(a)   Set64ValZero((a).u.uval)
#define Set64Val1mPP(a)     Set64Val1m((a).u.uval)
#define Set64ValU32PP(a,b)  Set64ValU32((a).u.uval,(b))

#define U64CmpPP(a,b)       U64Cmp(&((a).u.uval),&((b).u.uval))
#define I64CmpPP(a,b)       I64Cmp(&((a).u.sval),&((b).u.sval))
#define U64CmpU32PP(a,b)    U64CmpU32((a).u.uval,(b))

#define U64AddEqPP(a,b)     U64AddEq(&((a).u.uval),&((b).u.uval))
#define U64SubEqPP(a,b)     U64SubEq(&((a).u.uval),&((b).u.uval))
#define U64MulEqPP(a,b)     U64MulEq(&((a).u.uval),&((b).u.uval))
#define U64NegEqPP(a)       U64NegEq(&((a).u.uval))
#define U64AndEqPP(a,b)     U64AndEq((a).u.uval,(b).u.uval)
#define U64OrEqPP(a,b)      U64OrEq((a).u.uval,(b).u.uval)
#define U64XorEqPP(a,b)     U64XorEq((a).u.uval,(b).u.uval)
#define U64NotEqPP(a)       U64NotEq((a).u.uval)

#define I64ShiftREqPP(a,b)  I64ShiftREq(&((a).u.sval),(b))
#define U64ShiftREqPP(a,b)  U64ShiftREq(&((a).u.uval),(b))
#define U64ShiftLEqPP(a,b)  U64ShiftLEq(&((a).u.uval),(b))

#define U64DivPP(a,b,c,d)   U64Div(&((a).u.uval),&((b).u.uval),&((c).u.uval),&((d).u.uval))
#define I64DivPP(a,b,c,d)   I64Div(&((a).u.sval),&((b).u.sval),&((c).u.sval),&((d).u.sval))

#define LAST_TOKEN_PREC     ARRAY_SIZE( Prec )

#define IS_OPERAND( token ) ( IS_ID_OR_KEYWORD( token ) || token == T_CONSTANT )

#ifdef DEVBUILD
    #define DbgDefault( msg )   default: CFatal( msg __location )
#else
    #define DbgDefault( msg )
#endif

#define Stack_forall( hdr, i ) \
    for( i = hdr; i != NULL; i = *((void **)i) )

typedef struct ppvalue {
    union {
        unsigned_64     uval;
        signed_64       sval;
    } u;
    bool_        no_sign;
} ppvalue;

typedef struct loc_info {
    int         pos;        // incremented when token read
    source_loc  locn;       // token location in source file
} loc_info;

typedef struct operand_stack {
    struct operand_stack *next;
    ppvalue     value;
    loc_info    loc;
} PPEXPN_OPERAND_STACK;     // stack to store operands

typedef struct operator_stack {
    struct operator_stack *next;
    TOKEN       token;
    loc_info    loc;
    int         prec;
} PPEXPN_OPERATOR_STACK;    // stack to store operators

typedef struct stack {
    struct stack    *next;
} STACK;


/* include _ctokens.h for the precedence values */
static int  const Prec[] = {     // table of token precedences
    #define pick(token,string,class_,oper,prec) prec,
    #define OPERATORS_ONLY
    #include "_ctokens.h"
    #undef OPERATORS_ONLY
    #undef pick
};

#ifdef DEVBUILD
static void CFatal( char *msg )
/*****************************/
{
    FEMessage( FEMSG_FATAL, msg );
}
#endif

static void *StackPush( void *hdr, void *elt )
/********************************************/
{
    void    **header = static_cast<void **>(hdr);
    STACK   *e;

    e = static_cast<STACK *>(elt);
    e->next = static_cast<struct stack *>(*header);
    *header = e;
    return( e );
}

static void *StackPop( void *hdr )
/********************************/
{
    void    **header = static_cast<void **>(hdr);
    STACK   *e;

    e = static_cast<STACK *>(*header);
    if( e == NULL ) {
        return( NULL );
    }
    *header = e->next;
    return( e );
}

static bool_ CStart( void );
static bool_ CRightParen( void );
static bool_ CLeftParen( void );
static bool_ CConditional( void );
static bool_ CLogicalOr( void );
static bool_ CLogicalAnd( void );
static bool_ COr( void );
static bool_ CXOr( void );
static bool_ CAnd( void );
static bool_ CEquality( void );
static bool_ CRelational( void );
static bool_ CShift( void );
static bool_ CAdditive( void );
static bool_ CMultiplicative( void );
static bool_ CUnary( void );

namespace {

/* the statics of this file, one set for each run of the compiler */
struct PpexpnState {
    PPEXPN_OPERAND_STACK *HeadOperand = NULL;
    PPEXPN_OPERATOR_STACK *HeadOperator = NULL;
    int Pos = 0;   // position of CurToken in parsing
    TOKEN PpNextToken_prev_token;
};

}  // namespace

static inline PpexpnState& ppexpn_state() { return cbrun_part<PpexpnState>(); }

static void PushOperator( TOKEN token, loc_info *loc, int prec )
/**************************************************************/
{
    PPEXPN_OPERATOR_STACK *stack_entry;

    stack_entry = (PPEXPN_OPERATOR_STACK *)CMemAlloc( sizeof( PPEXPN_OPERATOR_STACK ) );
    stack_entry->token = token;
    stack_entry->loc = *loc;
    stack_entry->prec = prec;
    StackPush( &ppexpn_state().HeadOperator, stack_entry );
}

static void PushCurToken( int prec )
/**********************************/
{
    loc_info loc;

    loc.locn = cvars_state().SrcFileLoc;
    loc.pos = ppexpn_state().Pos;
    PushOperator( cvars_state().CurToken, &loc, prec );
}


static bool_ PopOperator( TOKEN *token, loc_info *loc )
/****************************************************/
{
    PPEXPN_OPERATOR_STACK *stack_entry;

    stack_entry = static_cast<PPEXPN_OPERATOR_STACK *>(StackPop( &ppexpn_state().HeadOperator ));
    if( stack_entry != NULL ) {
        if( token != NULL ) {
            *token = stack_entry->token;
        }
        if( loc != NULL ) {
            *loc = stack_entry->loc;
        }
        CMemFree( stack_entry );
        return( true );
    }
    return( false );
}

static bool_ TopOperator( TOKEN *token, int *prec )
/************************************************/
{
    PPEXPN_OPERATOR_STACK *stack_entry;

    stack_entry = static_cast<PPEXPN_OPERATOR_STACK *>(StackPop( &ppexpn_state().HeadOperator ));
    if( stack_entry != NULL ) {
        if( token != NULL ) {
            *token = stack_entry->token;
        }
        if( prec != NULL ) {
            *prec = stack_entry->prec;
        }
        StackPush( &ppexpn_state().HeadOperator, stack_entry );
        return( true );
    }
    return( false );
}

static void PushOperand( ppvalue p, loc_info *loc )
/*************************************************/
{
    PPEXPN_OPERAND_STACK *stack_entry;

    stack_entry = (PPEXPN_OPERAND_STACK *)CMemAlloc( sizeof( PPEXPN_OPERAND_STACK ) );
    stack_entry->value = p;
    stack_entry->loc = *loc;
    StackPush( &ppexpn_state().HeadOperand, stack_entry );
}

static void PushOperandCurLocation( ppvalue p )
/*********************************************/
{
    loc_info loc;

    loc.locn = cvars_state().SrcFileLoc;
    loc.pos = ppexpn_state().Pos;
    PushOperand( p, &loc );
}

static bool_ PopOperand( ppvalue *p, loc_info *loc )
/*************************************************/
{
    PPEXPN_OPERAND_STACK *stack_entry;

    stack_entry = static_cast<PPEXPN_OPERAND_STACK *>(StackPop( &ppexpn_state().HeadOperand ));
    if( stack_entry != NULL ) {
        if( p != NULL ) {
            *p = stack_entry->value;
        }
        if( loc != NULL ) {
            *loc = stack_entry->loc;
        }
        CMemFree( stack_entry );
        return( true );
    }
    return( false );
}

static bool_ CheckToken( TOKEN prev_token )
/****************************************/
{
    if( IS_OPERAND( prev_token )
      && IS_OPERAND( cvars_state().CurToken ) ) {
        CErr1( ERR_CONSECUTIVE_OPERANDS );  // can't have 2 operands in a row
        return( true );
    }
    if( ( cvars_state().CurToken == T_PLUS )
      || ( cvars_state().CurToken == T_MINUS ) ) {
        if( ( prev_token != T_RIGHT_PAREN )
          && ( !IS_OPERAND( prev_token )
          || prev_token == T_START ) ) {
            if( cvars_state().CurToken == T_PLUS ) {
                cvars_state().CurToken = T_UNARY_PLUS;
            } else {
                cvars_state().CurToken = T_UNARY_MINUS;
            }
        }
    }
    return( false );
}

static bool_ PpNextToken( void )
/******************************
 * scan the next token and check for errors
 */
{

    ppexpn_state().PpNextToken_prev_token = cvars_state().CurToken;
    NextToken();
    ppexpn_state().Pos++;
    return( CheckToken( ppexpn_state().PpNextToken_prev_token ) );
}

static void unexpectedCurToken( void )
/************************************/
{
    CErr2p( ERR_UNEXPECTED_IN_CONSTANT_EXPRESSION, TokenString[cvars_state().CurToken] );
}

static double SafeAtof( char *p )
/*******************************/
{
    double r;

    errno = 0;
    r = atof( p );
    if( errno ) {
        if( r == 0
          && errno == ERANGE ) {
            CErr1( ERR_FLOATING_CONSTANT_UNDERFLOW );
        } else {
            CErr1( ERR_FLOATING_CONSTANT_OVERFLOW );
        }
    }
    return( r );
}

static bool_ COperand( void )
/**************************/
{
    ppvalue p;
    loc_info loc;
    source_loc left_loc;
    bool_ done;

    done = false;
    p.no_sign = false;
    switch( cvars_state().CurToken ) {
    case T_ID:
        loc.locn = cvars_state().SrcFileLoc; // need this to store result
        loc.pos = ppexpn_state().Pos;
        ppexpn_state().Pos++;
        if( IS_PPOPERATOR_DEFINED( cvars_state().Buffer ) ) {
            ppctl_t old_ppctl;

            old_ppctl = cvars_state().PPControl;
            PPCTL_DISABLE_MACROS();
            NextToken();        // Don't error check: can have T_ID T_ID here
            if( cvars_state().CurToken == T_LEFT_PAREN ) {
                left_loc = cvars_state().SrcFileLoc;
                NextToken();    // no need to error check or advance Pos
                cvars_state().PPControl = old_ppctl;
                Set64ValU32PP( p, MacroLookup( cvars_state().Buffer ) != NULL );
                NextToken();    // no need to error check or advance Pos
                if( cvars_state().CurToken != T_RIGHT_PAREN ) {
                    SetErrLoc( &left_loc );
                    CErr1( ERR_UNMATCHED_LEFT_PAREN );
                    done = true;
                }
            } else {
                cvars_state().PPControl = old_ppctl;
                Set64ValU32PP( p, MacroLookup( cvars_state().Buffer ) != NULL );
            }
        } else {
            CWarn2p( ERR_UNDEFD_MACRO_IS_ZERO, cvars_state().Buffer );
            Set64ValZeroPP( p );
        }
        if( !done ) {
            PushOperand( p, &loc );
            done = PpNextToken();
        }
        break;
#if 0
    case T_FALSE:
    case T_TRUE:
        Set64ValU32PP( p, CurToken == T_TRUE );
        PushOperandCurLocation( p );
        done = PpNextToken();
        break;
#endif
    case T_CONSTANT:
        switch( cvars_state().ConstType ) {
        case TYP_FLOAT:
        case TYP_DOUBLE:
        case TYP_LONG_DOUBLE:
            CErr1( ERR_EXPR_MUST_BE_INTEGRAL );
            done = true;
            p.u.sval.u._64[0] = (long long)SafeAtof( cvars_state().Buffer );
            // add long double support if available
            break;
        case TYP_WCHAR:
        case TYP_UCHAR:
        case TYP_USHORT:
        case TYP_UINT:
        case TYP_ULONG:
        case TYP_ULONG64:
            p.no_sign = true;
            /* fall through */
        default:
            p.u.uval = cvars_state().Constant64;
            break;
        }
        if( !done ) {
            PushOperandCurLocation( p );
            done = PpNextToken();
        }
        break;
    default:
        CErr2p( ERR_UNDEFD_MACRO_IS_ZERO, cvars_state().Buffer );
        Set64ValZeroPP( p );
        PushOperandCurLocation( p );
        done = PpNextToken();
    }
    return( done );
}

static void CErrCheckpoint( unsigned *save )
/******************************************/
{
    *save = cvars_state().ErrCount;
}

static bool_ CErrOccurred( unsigned *previous_state )
/**************************************************/
{
    if( *previous_state != cvars_state().ErrCount ) {
        return( true );
    }
    return( false );
}

static bool_ CRightParen( void )
/******************************
 * reduce extra )
 */
{
    TOKEN right_paren;
    loc_info right_info;

    // as ) doesn't get pushed for (expr), this must be an unmatched )
    if( PopOperator( &right_paren, &right_info ) ) {
        SetErrLoc( &right_info.locn );
        CErr1( ERR_UNMATCHED_RIGHT_PAREN );
    }
    return( false );
}

static bool_ CLeftParen( void )
/*****************************
 * reduce (expr)
 */
{
    TOKEN left_paren;
    loc_info left_info;
    loc_info e1_info;
    ppvalue e1;

    PopOperator( &left_paren, &left_info );
    if( cvars_state().CurToken != T_RIGHT_PAREN ) {   // expect ) as current token
        SetErrLoc( &left_info.locn );
        CErr1( ERR_UNMATCHED_LEFT_PAREN );
        return( true );
    }
    if( PopOperand( &e1, &e1_info ) ) {
        if( ( e1_info.pos < ppexpn_state().Pos )
          && ( e1_info.pos > left_info.pos ) ) {
            PushOperand( e1, &e1_info );
            return( PpNextToken() );
        }
    }
    SetErrLoc( &left_info.locn );
    CErr1( ERR_EMPTY_PAREN );
    return( true );
}

static bool_ CConditional( void )
/*******************************
 * reduce an a?b:c expression
 */
{
    loc_info e1_info;
    loc_info e2_info;
    loc_info e3_info;
    loc_info op1_info;
    loc_info op2_info;
    ppvalue e1;
    ppvalue e2;
    ppvalue e3;
    TOKEN op1;
    TOKEN op2;

    PopOperator( &op2, &op2_info );
    if( op2 != T_COLON ) {
        if( ( op2 != T_QUESTION
          && op2 != T_START )
          || cvars_state().CurToken == T_NULL ) {
            SetErrLoc( &op2_info.locn );
            CErr1( ERR_CONDITIONAL_MISSING_COLON );
            return( true );
        } else {
            PushOperator( op2, &op2_info, Prec[op2] );
            PushCurToken( Prec[cvars_state().CurToken] );
            return( PpNextToken() );
        }
    }
    // always something to pop next, even if its T_START
    PopOperator( &op1, &op1_info );
    if( op1 != T_QUESTION ) {
        if( ( op1 != T_COLON
          && op1 != T_START )
          || ( cvars_state().CurToken == T_NULL ) ) {
            SetErrLoc( &op1_info.locn );
            CErr1( ERR_CONDITIONAL_MISSING_QUESTION );
            return( true );
        }
    }
    if( op2 != T_COLON
      || op1 != T_QUESTION ) {
        PushOperator( op1, &op1_info, Prec[op1] );
        PushOperator( op2, &op2_info, Prec[op2] );
        PushCurToken( Prec[cvars_state().CurToken] );
        return( PpNextToken() );
    }
    if( PopOperand( &e3, &e3_info )
      && ( e3_info.pos > op2_info.pos ) ) {
        if( PopOperand( &e2, &e2_info )
          && ( e2_info.pos < op2_info.pos )
          && ( e2_info.pos > op1_info.pos ) ) {
            if( PopOperand( &e1, &e1_info )
              && ( e1_info.pos < op1_info.pos ) ) {
                if( U64isntZeroPP( e1 ) ) {
                    e1.u.sval = e2.u.sval;
                } else {
                    e1.u.sval = e3.u.sval;
                }
                e1.no_sign = e2.no_sign || e3.no_sign;
                PushOperand( e1, &e1_info );
                return( false );
            } else {
                SetErrLoc( &op1_info.locn );    // missing a in a?b:c
                CErr1( ERR_CONDITIONAL_MISSING_FIRST_OPERAND );
            }
        } else {
            SetErrLoc( &op1_info.locn );        // missing b in a?b:c
            CErr1( ERR_CONDITIONAL_MISSING_SECOND_OPERAND );
        }
    } else {
        SetErrLoc( &op2_info.locn );            // missing c in a?b:c
        CErr1( ERR_CONDITIONAL_MISSING_THIRD_OPERAND );
    }
    return( true );
}

static bool_ Binary( TOKEN *token, ppvalue *e1, ppvalue *e2, loc_info *loc )
/**************************************************************************
 * pop binary operator and two operands, error check
 */
{
    loc_info e1_info;
    loc_info e2_info;


    PopOperator( token, loc );
    if( PopOperand( e2, &e2_info )
      && ( e2_info.pos > loc->pos ) ) {
        if( PopOperand( e1, &e1_info )
          && ( e1_info.pos < loc->pos ) ) {
            return( true );
        } else {
            SetErrLoc( &loc->locn );
            CErr2p( ERR_BINARY_MISSING_LEFT_OPERAND, TokenString[*token] );
        }
    } else {
        SetErrLoc( &loc->locn );
        CErr2p( ERR_BINARY_MISSING_RIGHT_OPERAND, TokenString[*token] );
    }
    return( false );
}

static bool_ CLogicalOr( void )
/*****************************
 * reduce a || b
 */
{
    ppvalue e1;
    ppvalue e2;
    loc_info loc;
    TOKEN token;

    if( Binary( &token, &e1, &e2, &loc ) ) {
        Set64ValU32PP( e1, U64isntZeroPP( e1 ) || U64isntZeroPP( e2 ) );
        e1.no_sign = false;
        PushOperand( e1, &loc );
        return( false );
    }
    return( true );
}

static bool_ CLogicalAnd( void )
/******************************
 * reduce a && b
 */
{
    ppvalue e1;
    ppvalue e2;
    loc_info loc;
    TOKEN token;

    if( Binary( &token, &e1, &e2, &loc ) ) {
        Set64ValU32PP( e1, U64isntZeroPP( e1 ) && U64isntZeroPP( e2 ) );
        e1.no_sign = false;
        PushOperand( e1, &loc );
        return( false );
    }
    return( true );
}

static bool_ COr( void )
/**********************
 * reduce a|b
 */
{
    ppvalue e1;
    ppvalue e2;
    loc_info loc;
    TOKEN token;

    if( Binary( &token, &e1, &e2, &loc ) ) {
        U64OrEqPP( e1, e2 );
        e1.no_sign |= e2.no_sign;
        PushOperand( e1, &loc );
        return( false );
    }
    return( true );
}

static bool_ CXOr( void )
/***********************
 * reduce a^b
 */
{
    ppvalue e1;
    ppvalue e2;
    loc_info loc;
    TOKEN token;

    if( Binary( &token, &e1, &e2, &loc ) ) {
        U64XorEqPP( e1, e2 );
        e1.no_sign |= e2.no_sign;
        PushOperand( e1, &loc );
        return( false );
    }
    return( true );
}

static bool_ CAnd( void )
/***********************
 * reduce a&b
 */
{
    ppvalue e1;
    ppvalue e2;
    loc_info loc;
    TOKEN token;

    if( Binary( &token, &e1, &e2, &loc ) ) {
        U64AndEqPP( e1, e2 );
        e1.no_sign |= e2.no_sign;
        PushOperand( e1, &loc );
        return( false );
    }
    return( true );
}

static bool_ CEquality( void )
/****************************
 * reduce a == b or a != b
 */
{
    ppvalue e1;
    ppvalue e2;
    loc_info loc;
    TOKEN token;
    bool_ eq;

    if( Binary( &token, &e1, &e2, &loc ) ) {
        eq = U64isEqPP( e1, e2 );
        Set64ValU32PP( e1, ( token == T_EQ ) ? eq : !eq );
        e1.no_sign = false;
        PushOperand( e1, &loc );
        return( false );
    }
    return( true );
}

static bool_ CRelational( void )
/******************************
 * reduce a<b, a>b, a<=b, or a>=b
 */
{
    ppvalue e1;
    ppvalue e2;
    loc_info loc;
    TOKEN token;
    int cmp;

    if( Binary( &token, &e1, &e2, &loc ) ) {
        if( e1.no_sign
          || e2.no_sign ) {
            cmp = U64CmpPP( e1, e2 );
        } else {
            cmp = I64CmpPP( e1, e2 );
        }
        switch( token ) {
        case T_LT:
            Set64ValU32PP( e1, cmp < 0 );
            break;
        case T_LE:
            Set64ValU32PP( e1, cmp <= 0 );
            break;
        case T_GT:
            Set64ValU32PP( e1, cmp > 0 );
            break;
        case T_GE:
            Set64ValU32PP( e1, cmp >= 0 );
            break;
        DbgDefault( "Default in CRelational\n" );
        }
        e1.no_sign = false;
        PushOperand( e1, &loc );
        return( false );
    }
    return( true );
}

static bool_ CShift( void )
/*************************
 * reduce a<<b or a>>b
 */
{
    ppvalue e1;
    ppvalue e2;
    loc_info loc;
    TOKEN token;

    if( Binary( &token, &e1, &e2, &loc ) ) {
        switch( token ) {
        case T_RSHIFT:
            if( U64CmpU32PP( e2, 64 ) >= 0 ) {
                if( !e1.no_sign && I64isNegPP( e1 ) ) {
                    Set64Val1mPP( e1 );
                } else {
                    Set64ValZeroPP( e1 );
                }
            } else if( U64isntZeroPP( e2 ) ) {
                if( e1.no_sign ) {
                    U64ShiftREqPP( e1, U64LowPP( e2 ) );
                } else {
                    I64ShiftREqPP( e1, U64LowPP( e2 ) );
                }
            }
            break;
        case T_LSHIFT:
            if( U64CmpU32PP( e2, 64 ) >= 0 ) {
                Set64ValZeroPP( e1 );
            } else if( U64isntZeroPP( e2 ) ) {
                U64ShiftLEqPP( e1, U64LowPP( e2 ) );
            }
            break;
        DbgDefault( "Default in CShift\n" );
        }
        PushOperand( e1, &loc );
        return( false );
    }
    return( true );
}

static bool_ CAdditive( void )
/****************************
 * reduce a+b or a-b
 */
{
    ppvalue e1;
    ppvalue e2;
    loc_info loc;
    TOKEN token;

    if( Binary( &token, &e1, &e2, &loc ) ) {
        switch( token ) {
        case T_PLUS:
            U64AddEqPP( e1, e2 );
            e1.no_sign |= e2.no_sign;
            break;
        case T_MINUS:
            U64SubEqPP( e1, e2 );
            e1.no_sign = false;
            break;
        DbgDefault( "Default in CAdditive\n" );
        }
        PushOperand( e1, &loc );
        return( false );
    }
    return( true );
}


static bool_ CMultiplicative( void )
/**********************************
 * reduce a/b or a*b
 */
{
    ppvalue e1;
    ppvalue e2;
    loc_info loc;
    TOKEN token;
    ppvalue unused;

    if( Binary( &token, &e1, &e2, &loc ) ) {
        switch( token ) {
        case T_TIMES:
            U64MulEqPP( e1, e2 );
            break;
        case T_DIV:
            if( U64isZeroPP( e2 ) ) {
                Set64ValZeroPP( e1 );
            } else if( e1.no_sign
              || e2.no_sign ) {
                U64DivPP( e1, e2, e1, unused );
            } else {
                I64DivPP( e1, e2, e1, unused );
            }
            break;
        case T_PERCENT:
            if( U64isZeroPP( e2 ) ) {
                Set64ValZeroPP( e1 );
            } else if( e1.no_sign
              || e2.no_sign ) {
                U64DivPP( e1, e2, unused, e1 );
            } else {
                I64DivPP( e1, e2, unused, e1 );
            }
            break;
        DbgDefault( "Default in CMultiplicative\n" );
        }
        e1.no_sign |= e2.no_sign;
        PushOperand( e1, &loc );
        return( false );
    }
    return( true );
}

static bool_ CUnary( void )
/*************************
 * reduce +a or -a or !a or ~a
 */
{
    ppvalue p;
    loc_info operator_info;
    loc_info operand_info;
    TOKEN top;

    PopOperator( &top, &operator_info );
    if( PopOperand( &p, &operand_info )
      && ( operator_info.pos < operand_info.pos ) ) {
        switch( top ) {
        case T_UNARY_PLUS:
            break;
        case T_UNARY_MINUS:
            U64NegEqPP( p );
            break;
        case T_EXCLAMATION:
//        case T_ALT_EXCLAMATION:
            Set64ValU32PP( p, U64isZeroPP( p ) );
            p.no_sign = false;
            break;
        case T_TILDE:
//        case T_ALT_TILDE:
            U64NotEqPP( p );
            break;
        DbgDefault( "Default in CUnary\n" );
        }
        PushOperand( p, &operator_info );
    } else {
        SetErrLoc( &operator_info.locn );
        CErr2p( ERR_UNARY_OPERATOR_MISSING_OPERAND, TokenString[top] );
        return( true );
    }
    return( false );
}

static bool_ CStart( void )
/************************/
{
    TOKEN top;

    PopOperator( &top, NULL );
    if( cvars_state().CurToken != T_NULL ) {
        unexpectedCurToken();
    }
    return( true );
}

#if 0
static void stringize( char *s )
/******************************/
{
    char    *d;

    d = s;
    while( *s != '\0' ) {
        if( s[0] == '\\' ) {
            if( s[1] == '\\'
              || s[1] == '\"' ) {
                s++;
            }
        }
        *d++ = *s++;
    }
    *d = '\0';
}

TOKEN Process_Pragma( bool_ internal )
/***********************************/
{
    PpNextToken();
    if( CurToken == T_LEFT_PAREN ) {
        PpNextToken();
        if( CurToken == T_STRING ) {
            char    *token_buf;

            token_buf = CMemStrdup( Buffer );
            PpNextToken();
            if( CurToken == T_RIGHT_PAREN ) {
                ppctl_t old_ppctl;

                stringize( token_buf );
                InsertReScanPragmaTokens( token_buf, internal );
                // call CPragma()
                old_ppctl = PPControl;
                PPCTL_ENABLE_EOL();
                CPragma();
                PPControl = old_ppctl;
            } else {
                /* error, incorrect syntax of the operator _Pragma() */
            }
            CMemFree( (void *)token_buf );
            PpNextToken();
        } else {
            /* error, incorrect syntax of the operator _Pragma() */
        }
    } else {
        InsertToken( CurToken, Buffer, internal );
        strcpy( Buffer, PPOPERATOR_PRAGMA );
        TokenLen = LENLIT( PPOPERATOR_PRAGMA );
        CurToken = T_ID;
    }
    return( CurToken );
}
#endif

static bool_ ( *const CExpr[] )(void) = { // table of functions to reduce expressions
    CStart,             /* Level 0 */
    CRightParen,        /* Level 1 */
    CLeftParen,         /* Level 2 */
    CConditional,       /* Level 3 */
    CConditional,       /* Level 4 */
    CLogicalOr,         /* Level 5 */
    CLogicalAnd,        /* Level 6 */
    COr,                /* Level 7 */
    CXOr,               /* Level 8 */
    CAnd,               /* Level 9 */
    CEquality,          /* Level 10 */
    CRelational,        /* Level 11 */
    CShift,             /* Level 12 */
    CAdditive,          /* Level 13 */
    CMultiplicative,    /* Level 14 */
    CUnary              /* Level 15 */
};

static void PrecedenceParse( ppvalue *p )
/****************************************
 * main precedence parse algorithm
 */
{
    int prec_token;
    int prec_operator;
    bool_ done;
    TOKEN top;
    ppvalue empty;
    loc_info loc;
    unsigned error_info;

    Set64ValZeroPP( *p );  // default value 0

    if( cvars_state().CurToken == T_NULL ) {
        unexpectedCurToken();
        return;
    }
    CErrCheckpoint( &error_info );
    prec_operator = 0;
    ppexpn_state().Pos = 0;
    loc.locn = cvars_state().SrcFileLoc;
    loc.pos = ppexpn_state().Pos;
    PushOperator( T_START, &loc, Prec[T_START] ); // problem because T_START is not a ppvalue
    ppexpn_state().Pos++;
    CheckToken( T_START ); // check for initial unary + or -
    prec_operator = 0;
    done = false;
    while( !done ) {
        if( IS_OPERAND( cvars_state().CurToken ) ) {
            done = COperand(); // get operand and read next token
        } else if( cvars_state().CurToken == T_LEFT_PAREN ) {
            PushCurToken( Prec[T_LEFT_PAREN] ); // always push left paren
            done = PpNextToken();
        } else {
            TopOperator( &top, &prec_operator );
            if( cvars_state().CurToken < LAST_TOKEN_PREC ) {
                prec_token = Prec[cvars_state().CurToken];
                if( prec_token < prec_operator
                  || ( ( prec_token == prec_operator )
                  && ( prec_token != PREC_UNARY ) ) ) {
                    done = CExpr[prec_operator](); // reduce
                } else {
                    PushCurToken( prec_token ); // shift
                    done = PpNextToken();
                }
            } else {
                unexpectedCurToken();
                done = true;
            }
        }
    }
    if( !CErrOccurred( &error_info ) ) {
        PopOperand( p, NULL );
        if( PopOperand( &empty, &loc ) ) {
            SetErrLoc( &loc.locn );
            CErr1( ERR_EXTRA_OPERAND );
        }
    }
    if( CErrOccurred( &error_info ) ) {
        // scan until EOL (T_NULL) to avoid multiple messages
        while( cvars_state().CurToken != T_NULL ) {
            NextToken();
        }
    }
    // empty stacks
    while( PopOperand( NULL, NULL ) ); // free stack
    while( PopOperator( NULL, NULL ) ); // free stack
}

bool_ PpConstExpr( void )
/***********************
 * Entry into ppexpn module
 */
{
    ppvalue val;

    PrecedenceParse( &val );
    return( U64isntZeroPP( val ) );
}

void InitPPexpn( void )
/*********************/
{
    ppexpn_state().HeadOperand = NULL;
    ppexpn_state().HeadOperator = NULL;
}

void FiniPPexpn( void )
/*********************/
{
    if( ppexpn_state().HeadOperand != NULL ) {
        CMemFree( ppexpn_state().HeadOperand );
        ppexpn_state().HeadOperand = NULL;
    }
    if( ppexpn_state().HeadOperator != NULL ) {
        CMemFree( ppexpn_state().HeadOperator );
        ppexpn_state().HeadOperator = NULL;
    }
}
