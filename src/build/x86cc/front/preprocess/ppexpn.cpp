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
#include "frontrun.hpp"


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

    #define DbgDefault( msg )






     // stack to store operands

    // stack to store operators




/* include _ctokens.h for the precedence values */
static int  const Prec[] = {     // table of token precedences
    #define pick(token,string,class_,oper,prec) prec,
    #define OPERATORS_ONLY
    #include "_ctokens.h"
    #undef OPERATORS_ONLY
    #undef pick
};


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

static bool CStart( FrontRun& front );
static bool CRightParen( FrontRun& front );
static bool CLeftParen( FrontRun& front );
static bool CConditional( FrontRun& front );
static bool CLogicalOr( FrontRun& front );
static bool CLogicalAnd( FrontRun& front );
static bool COr( FrontRun& front );
static bool CXOr( FrontRun& front );
static bool CAnd( FrontRun& front );
static bool CEquality( FrontRun& front );
static bool CRelational( FrontRun& front );
static bool CShift( FrontRun& front );
static bool CAdditive( FrontRun& front );
static bool CMultiplicative( FrontRun& front );
static bool CUnary( FrontRun& front );

namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct PpexpnState: ppexpn_state.h) */

}  // namespace


static void PushOperator( FrontRun& front, TOKEN token, loc_info *loc, int prec )
/**************************************************************/
{
    PPEXPN_OPERATOR_STACK *stack_entry;

    stack_entry = (PPEXPN_OPERATOR_STACK *)CMemAlloc( front, sizeof( PPEXPN_OPERATOR_STACK ) );
    stack_entry->token = token;
    stack_entry->loc = *loc;
    stack_entry->prec = prec;
    StackPush( &front.ppexpn.HeadOperator, stack_entry );
}

static void PushCurToken( FrontRun& front, int prec )
/**********************************/
{
    loc_info loc;

    loc.locn = front.cvars.SrcFileLoc;
    loc.pos = front.ppexpn.Pos;
    PushOperator( front, front.cvars.CurToken, &loc, prec );
}


static bool PopOperator( FrontRun& front, TOKEN *token, loc_info *loc )
/****************************************************/
{
    PPEXPN_OPERATOR_STACK *stack_entry;

    stack_entry = static_cast<PPEXPN_OPERATOR_STACK *>(StackPop( &front.ppexpn.HeadOperator ));
    if( stack_entry != NULL ) {
        if( token != NULL ) {
            *token = stack_entry->token;
        }
        if( loc != NULL ) {
            *loc = stack_entry->loc;
        }
        CMemFree( front, stack_entry );
        return( true );
    }
    return( false );
}

static bool TopOperator( FrontRun& front, TOKEN *token, int *prec )
/************************************************/
{
    PPEXPN_OPERATOR_STACK *stack_entry;

    stack_entry = static_cast<PPEXPN_OPERATOR_STACK *>(StackPop( &front.ppexpn.HeadOperator ));
    if( stack_entry != NULL ) {
        if( token != NULL ) {
            *token = stack_entry->token;
        }
        if( prec != NULL ) {
            *prec = stack_entry->prec;
        }
        StackPush( &front.ppexpn.HeadOperator, stack_entry );
        return( true );
    }
    return( false );
}

static void PushOperand( FrontRun& front, ppvalue p, loc_info *loc )
/*************************************************/
{
    PPEXPN_OPERAND_STACK *stack_entry;

    stack_entry = (PPEXPN_OPERAND_STACK *)CMemAlloc( front, sizeof( PPEXPN_OPERAND_STACK ) );
    stack_entry->value = p;
    stack_entry->loc = *loc;
    StackPush( &front.ppexpn.HeadOperand, stack_entry );
}

static void PushOperandCurLocation( FrontRun& front, ppvalue p )
/*********************************************/
{
    loc_info loc;

    loc.locn = front.cvars.SrcFileLoc;
    loc.pos = front.ppexpn.Pos;
    PushOperand( front, p, &loc );
}

static bool PopOperand( FrontRun& front, ppvalue *p, loc_info *loc )
/*************************************************/
{
    PPEXPN_OPERAND_STACK *stack_entry;

    stack_entry = static_cast<PPEXPN_OPERAND_STACK *>(StackPop( &front.ppexpn.HeadOperand ));
    if( stack_entry != NULL ) {
        if( p != NULL ) {
            *p = stack_entry->value;
        }
        if( loc != NULL ) {
            *loc = stack_entry->loc;
        }
        CMemFree( front, stack_entry );
        return( true );
    }
    return( false );
}

static bool CheckToken( FrontRun& front, TOKEN prev_token )
/****************************************/
{
    if( IS_OPERAND( prev_token )
      && IS_OPERAND( front.cvars.CurToken ) ) {
        CErr1( front, ERR_CONSECUTIVE_OPERANDS );  // can't have 2 operands in a row
        return( true );
    }
    if( ( front.cvars.CurToken == T_PLUS )
      || ( front.cvars.CurToken == T_MINUS ) ) {
        if( ( prev_token != T_RIGHT_PAREN )
          && ( !IS_OPERAND( prev_token )
          || prev_token == T_START ) ) {
            if( front.cvars.CurToken == T_PLUS ) {
                front.cvars.CurToken = T_UNARY_PLUS;
            } else {
                front.cvars.CurToken = T_UNARY_MINUS;
            }
        }
    }
    return( false );
}

static bool PpNextToken( FrontRun& front )
/******************************
 * scan the next token and check for errors
 */
{

    front.ppexpn.PpNextToken_prev_token = front.cvars.CurToken;
    NextToken(front);
    front.ppexpn.Pos++;
    return( CheckToken( front, front.ppexpn.PpNextToken_prev_token ) );
}

static void unexpectedCurToken( FrontRun& front )
/************************************/
{
    CErr2p( front, ERR_UNEXPECTED_IN_CONSTANT_EXPRESSION, TokenString[front.cvars.CurToken] );
}

static double SafeAtof( FrontRun& front, char *p )
/*******************************/
{
    double r;

    errno = 0;
    r = atof( p );
    if( errno ) {
        if( r == 0
          && errno == ERANGE ) {
            CErr1( front, ERR_FLOATING_CONSTANT_UNDERFLOW );
        } else {
            CErr1( front, ERR_FLOATING_CONSTANT_OVERFLOW );
        }
    }
    return( r );
}

static bool COperand( FrontRun& front )
/**************************/
{
    ppvalue p;
    loc_info loc;
    source_loc left_loc;
    bool done;

    done = false;
    p.no_sign = false;
    switch( front.cvars.CurToken ) {
    case T_ID:
        loc.locn = front.cvars.SrcFileLoc; // need this to store result
        loc.pos = front.ppexpn.Pos;
        front.ppexpn.Pos++;
        if( IS_PPOPERATOR_DEFINED( front.cvars.Buffer ) ) {
            ppctl_t old_ppctl;

            old_ppctl = front.cvars.PPControl;
            PPCTL_DISABLE_MACROS();
            NextToken(front);        // Don't error check: can have T_ID T_ID here
            if( front.cvars.CurToken == T_LEFT_PAREN ) {
                left_loc = front.cvars.SrcFileLoc;
                NextToken(front);    // no need to error check or advance Pos
                front.cvars.PPControl = old_ppctl;
                Set64ValU32PP( p, MacroLookup( front, front.cvars.Buffer ) != NULL );
                NextToken(front);    // no need to error check or advance Pos
                if( front.cvars.CurToken != T_RIGHT_PAREN ) {
                    SetErrLoc( front, &left_loc );
                    CErr1( front, ERR_UNMATCHED_LEFT_PAREN );
                    done = true;
                }
            } else {
                front.cvars.PPControl = old_ppctl;
                Set64ValU32PP( p, MacroLookup( front, front.cvars.Buffer ) != NULL );
            }
        } else {
            CWarn2p( front, ERR_UNDEFD_MACRO_IS_ZERO, front.cvars.Buffer );
            Set64ValZeroPP( p );
        }
        if( !done ) {
            PushOperand( front, p, &loc );
            done = PpNextToken(front);
        }
        break;
    case T_CONSTANT:
        switch( front.cvars.ConstType ) {
        case TYP_FLOAT:
        case TYP_DOUBLE:
        case TYP_LONG_DOUBLE:
            CErr1( front, ERR_EXPR_MUST_BE_INTEGRAL );
            done = true;
            p.u.sval.u._64[0] = (long long)SafeAtof( front, front.cvars.Buffer );
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
            p.u.uval = front.cvars.Constant64;
            break;
        }
        if( !done ) {
            PushOperandCurLocation( front, p );
            done = PpNextToken(front);
        }
        break;
    default:
        CErr2p( front, ERR_UNDEFD_MACRO_IS_ZERO, front.cvars.Buffer );
        Set64ValZeroPP( p );
        PushOperandCurLocation( front, p );
        done = PpNextToken(front);
    }
    return( done );
}

static void CErrCheckpoint( FrontRun& front, unsigned *save )
/******************************************/
{
    *save = front.cvars.ErrCount;
}

static bool CErrOccurred( FrontRun& front, unsigned *previous_state )
/**************************************************/
{
    if( *previous_state != front.cvars.ErrCount ) {
        return( true );
    }
    return( false );
}

static bool CRightParen( FrontRun& front )
/******************************
 * reduce extra )
 */
{
    TOKEN right_paren;
    loc_info right_info;

    // as ) doesn't get pushed for (expr), this must be an unmatched )
    if( PopOperator( front, &right_paren, &right_info ) ) {
        SetErrLoc( front, &right_info.locn );
        CErr1( front, ERR_UNMATCHED_RIGHT_PAREN );
    }
    return( false );
}

static bool CLeftParen( FrontRun& front )
/*****************************
 * reduce (expr)
 */
{
    TOKEN left_paren;
    loc_info left_info;
    loc_info e1_info;
    ppvalue e1;

    PopOperator( front, &left_paren, &left_info );
    if( front.cvars.CurToken != T_RIGHT_PAREN ) {   // expect ) as current token
        SetErrLoc( front, &left_info.locn );
        CErr1( front, ERR_UNMATCHED_LEFT_PAREN );
        return( true );
    }
    if( PopOperand( front, &e1, &e1_info ) ) {
        if( ( e1_info.pos < front.ppexpn.Pos )
          && ( e1_info.pos > left_info.pos ) ) {
            PushOperand( front, e1, &e1_info );
            return( PpNextToken(front) );
        }
    }
    SetErrLoc( front, &left_info.locn );
    CErr1( front, ERR_EMPTY_PAREN );
    return( true );
}

static bool CConditional( FrontRun& front )
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

    PopOperator( front, &op2, &op2_info );
    if( op2 != T_COLON ) {
        if( ( op2 != T_QUESTION
          && op2 != T_START )
          || front.cvars.CurToken == T_NULL ) {
            SetErrLoc( front, &op2_info.locn );
            CErr1( front, ERR_CONDITIONAL_MISSING_COLON );
            return( true );
        } else {
            PushOperator( front, op2, &op2_info, Prec[op2] );
            PushCurToken( front, Prec[front.cvars.CurToken] );
            return( PpNextToken(front) );
        }
    }
    // always something to pop next, even if its T_START
    PopOperator( front, &op1, &op1_info );
    if( op1 != T_QUESTION ) {
        if( ( op1 != T_COLON
          && op1 != T_START )
          || ( front.cvars.CurToken == T_NULL ) ) {
            SetErrLoc( front, &op1_info.locn );
            CErr1( front, ERR_CONDITIONAL_MISSING_QUESTION );
            return( true );
        }
    }
    if( op2 != T_COLON
      || op1 != T_QUESTION ) {
        PushOperator( front, op1, &op1_info, Prec[op1] );
        PushOperator( front, op2, &op2_info, Prec[op2] );
        PushCurToken( front, Prec[front.cvars.CurToken] );
        return( PpNextToken(front) );
    }
    if( PopOperand( front, &e3, &e3_info )
      && ( e3_info.pos > op2_info.pos ) ) {
        if( PopOperand( front, &e2, &e2_info )
          && ( e2_info.pos < op2_info.pos )
          && ( e2_info.pos > op1_info.pos ) ) {
            if( PopOperand( front, &e1, &e1_info )
              && ( e1_info.pos < op1_info.pos ) ) {
                if( U64isntZeroPP( e1 ) ) {
                    e1.u.sval = e2.u.sval;
                } else {
                    e1.u.sval = e3.u.sval;
                }
                e1.no_sign = e2.no_sign || e3.no_sign;
                PushOperand( front, e1, &e1_info );
                return( false );
            } else {
                SetErrLoc( front, &op1_info.locn );    // missing a in a?b:c
                CErr1( front, ERR_CONDITIONAL_MISSING_FIRST_OPERAND );
            }
        } else {
            SetErrLoc( front, &op1_info.locn );        // missing b in a?b:c
            CErr1( front, ERR_CONDITIONAL_MISSING_SECOND_OPERAND );
        }
    } else {
        SetErrLoc( front, &op2_info.locn );            // missing c in a?b:c
        CErr1( front, ERR_CONDITIONAL_MISSING_THIRD_OPERAND );
    }
    return( true );
}

static bool Binary( FrontRun& front, TOKEN *token, ppvalue *e1, ppvalue *e2, loc_info *loc )
/**************************************************************************
 * pop binary operator and two operands, error check
 */
{
    loc_info e1_info;
    loc_info e2_info;


    PopOperator( front, token, loc );
    if( PopOperand( front, e2, &e2_info )
      && ( e2_info.pos > loc->pos ) ) {
        if( PopOperand( front, e1, &e1_info )
          && ( e1_info.pos < loc->pos ) ) {
            return( true );
        } else {
            SetErrLoc( front, &loc->locn );
            CErr2p( front, ERR_BINARY_MISSING_LEFT_OPERAND, TokenString[*token] );
        }
    } else {
        SetErrLoc( front, &loc->locn );
        CErr2p( front, ERR_BINARY_MISSING_RIGHT_OPERAND, TokenString[*token] );
    }
    return( false );
}

static bool CLogicalOr( FrontRun& front )
/*****************************
 * reduce a || b
 */
{
    ppvalue e1;
    ppvalue e2;
    loc_info loc;
    TOKEN token;

    if( Binary( front, &token, &e1, &e2, &loc ) ) {
        Set64ValU32PP( e1, U64isntZeroPP( e1 ) || U64isntZeroPP( e2 ) );
        e1.no_sign = false;
        PushOperand( front, e1, &loc );
        return( false );
    }
    return( true );
}

static bool CLogicalAnd( FrontRun& front )
/******************************
 * reduce a && b
 */
{
    ppvalue e1;
    ppvalue e2;
    loc_info loc;
    TOKEN token;

    if( Binary( front, &token, &e1, &e2, &loc ) ) {
        Set64ValU32PP( e1, U64isntZeroPP( e1 ) && U64isntZeroPP( e2 ) );
        e1.no_sign = false;
        PushOperand( front, e1, &loc );
        return( false );
    }
    return( true );
}

static bool COr( FrontRun& front )
/**********************
 * reduce a|b
 */
{
    ppvalue e1;
    ppvalue e2;
    loc_info loc;
    TOKEN token;

    if( Binary( front, &token, &e1, &e2, &loc ) ) {
        U64OrEqPP( e1, e2 );
        e1.no_sign |= e2.no_sign;
        PushOperand( front, e1, &loc );
        return( false );
    }
    return( true );
}

static bool CXOr( FrontRun& front )
/***********************
 * reduce a^b
 */
{
    ppvalue e1;
    ppvalue e2;
    loc_info loc;
    TOKEN token;

    if( Binary( front, &token, &e1, &e2, &loc ) ) {
        U64XorEqPP( e1, e2 );
        e1.no_sign |= e2.no_sign;
        PushOperand( front, e1, &loc );
        return( false );
    }
    return( true );
}

static bool CAnd( FrontRun& front )
/***********************
 * reduce a&b
 */
{
    ppvalue e1;
    ppvalue e2;
    loc_info loc;
    TOKEN token;

    if( Binary( front, &token, &e1, &e2, &loc ) ) {
        U64AndEqPP( e1, e2 );
        e1.no_sign |= e2.no_sign;
        PushOperand( front, e1, &loc );
        return( false );
    }
    return( true );
}

static bool CEquality( FrontRun& front )
/****************************
 * reduce a == b or a != b
 */
{
    ppvalue e1;
    ppvalue e2;
    loc_info loc;
    TOKEN token;
    bool eq;

    if( Binary( front, &token, &e1, &e2, &loc ) ) {
        eq = U64isEqPP( e1, e2 );
        Set64ValU32PP( e1, ( token == T_EQ ) ? eq : !eq );
        e1.no_sign = false;
        PushOperand( front, e1, &loc );
        return( false );
    }
    return( true );
}

static bool CRelational( FrontRun& front )
/******************************
 * reduce a<b, a>b, a<=b, or a>=b
 */
{
    ppvalue e1;
    ppvalue e2;
    loc_info loc;
    TOKEN token;
    int cmp;

    if( Binary( front, &token, &e1, &e2, &loc ) ) {
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
        PushOperand( front, e1, &loc );
        return( false );
    }
    return( true );
}

static bool CShift( FrontRun& front )
/*************************
 * reduce a<<b or a>>b
 */
{
    ppvalue e1;
    ppvalue e2;
    loc_info loc;
    TOKEN token;

    if( Binary( front, &token, &e1, &e2, &loc ) ) {
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
        PushOperand( front, e1, &loc );
        return( false );
    }
    return( true );
}

static bool CAdditive( FrontRun& front )
/****************************
 * reduce a+b or a-b
 */
{
    ppvalue e1;
    ppvalue e2;
    loc_info loc;
    TOKEN token;

    if( Binary( front, &token, &e1, &e2, &loc ) ) {
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
        PushOperand( front, e1, &loc );
        return( false );
    }
    return( true );
}


static bool CMultiplicative( FrontRun& front )
/**********************************
 * reduce a/b or a*b
 */
{
    ppvalue e1;
    ppvalue e2;
    loc_info loc;
    TOKEN token;
    ppvalue unused;

    if( Binary( front, &token, &e1, &e2, &loc ) ) {
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
        PushOperand( front, e1, &loc );
        return( false );
    }
    return( true );
}

static bool CUnary( FrontRun& front )
/*************************
 * reduce +a or -a or !a or ~a
 */
{
    ppvalue p;
    loc_info operator_info;
    loc_info operand_info;
    TOKEN top;

    PopOperator( front, &top, &operator_info );
    if( PopOperand( front, &p, &operand_info )
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
        PushOperand( front, p, &operator_info );
    } else {
        SetErrLoc( front, &operator_info.locn );
        CErr2p( front, ERR_UNARY_OPERATOR_MISSING_OPERAND, TokenString[top] );
        return( true );
    }
    return( false );
}

static bool CStart( FrontRun& front )
/************************/
{
    TOKEN top;

    PopOperator( front, &top, NULL );
    if( front.cvars.CurToken != T_NULL ) {
        unexpectedCurToken(front);
    }
    return( true );
}


static bool ( *const CExpr[] )(FrontRun&) = { // table of functions to reduce expressions
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

static void PrecedenceParse( FrontRun& front, ppvalue *p )
/****************************************
 * main precedence parse algorithm
 */
{
    int prec_token;
    int prec_operator;
    bool done;
    TOKEN top;
    ppvalue empty;
    loc_info loc;
    unsigned error_info;

    Set64ValZeroPP( *p );  // default value 0

    if( front.cvars.CurToken == T_NULL ) {
        unexpectedCurToken(front);
        return;
    }
    CErrCheckpoint( front, &error_info );
    prec_operator = 0;
    front.ppexpn.Pos = 0;
    loc.locn = front.cvars.SrcFileLoc;
    loc.pos = front.ppexpn.Pos;
    PushOperator( front, T_START, &loc, Prec[T_START] ); // problem because T_START is not a ppvalue
    front.ppexpn.Pos++;
    CheckToken( front, T_START ); // check for initial unary + or -
    prec_operator = 0;
    done = false;
    while( !done ) {
        if( IS_OPERAND( front.cvars.CurToken ) ) {
            done = COperand(front); // get operand and read next token
        } else if( front.cvars.CurToken == T_LEFT_PAREN ) {
            PushCurToken( front, Prec[T_LEFT_PAREN] ); // always push left paren
            done = PpNextToken(front);
        } else {
            TopOperator( front, &top, &prec_operator );
            if( front.cvars.CurToken < LAST_TOKEN_PREC ) {
                prec_token = Prec[front.cvars.CurToken];
                if( prec_token < prec_operator
                  || ( ( prec_token == prec_operator )
                  && ( prec_token != PREC_UNARY ) ) ) {
                    done = CExpr[prec_operator](front); // reduce
                } else {
                    PushCurToken( front, prec_token ); // shift
                    done = PpNextToken(front);
                }
            } else {
                unexpectedCurToken(front);
                done = true;
            }
        }
    }
    if( !CErrOccurred( front, &error_info ) ) {
        PopOperand( front, p, NULL );
        if( PopOperand( front, &empty, &loc ) ) {
            SetErrLoc( front, &loc.locn );
            CErr1( front, ERR_EXTRA_OPERAND );
        }
    }
    if( CErrOccurred( front, &error_info ) ) {
        // scan until EOL (T_NULL) to avoid multiple messages
        while( front.cvars.CurToken != T_NULL ) {
            NextToken(front);
        }
    }
    // empty stacks
    while( PopOperand( front, NULL, NULL ) ); // free stack
    while( PopOperator( front, NULL, NULL ) ); // free stack
}

bool PpConstExpr( FrontRun& front )
/***********************
 * Entry into ppexpn module
 */
{
    ppvalue val;

    PrecedenceParse( front, &val );
    return( U64isntZeroPP( val ) );
}

void InitPPexpn( FrontRun& front )
/*********************/
{
    front.ppexpn.HeadOperand = NULL;
    front.ppexpn.HeadOperator = NULL;
}

void FiniPPexpn( FrontRun& front )
/*********************/
{
    if( front.ppexpn.HeadOperand != NULL ) {
        CMemFree( front, front.ppexpn.HeadOperand );
        front.ppexpn.HeadOperand = NULL;
    }
    if( front.ppexpn.HeadOperator != NULL ) {
        CMemFree( front, front.ppexpn.HeadOperator );
        front.ppexpn.HeadOperator = NULL;
    }
}
