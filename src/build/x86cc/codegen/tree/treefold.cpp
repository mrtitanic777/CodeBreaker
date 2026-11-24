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
* Description:  Processor independent constant folding optimizations.
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "tree.h"
#include "zoiks.h"
#include "i64.h"
#include "data.h"
#include "types.h"
#include "treefold.h"
#include "treeprot.h"
#include "bldins.h"
#include "utils.h"
#include "namelist.h"
#include "u32moddi.h"
#include "feprotos.h"
#include "backrun.hpp"


inline bool HasBigConst( const type_def *t ) { return ( ( (t)->attr & TYPE_FLOAT ) || (t)->length == 8 ); }

static const cg_op RevOpcode[] = {
    O_EQ,    /* O_EQ*/
    O_NE,    /* O_NE*/
    O_LT,    /* O_GT*/
    O_GE,    /* O_LE*/
    O_GT,    /* O_LT*/
    O_LE     /* O_GE*/
};

/* CheckCmpRange is based on code stolen from CheckMeaninglessCompare(),
 * used by the C++ front end. It is quite handy to be able to perform the
 * folding inside the cg, especially since the C++ front end doesn't do it!
 */

#define SIGN_BIT        (0x8000)
inline int NumSign( int a ) { return ((a) & SIGN_BIT); }
inline int NumBits( int a ) { return ((a) & 0x7fff); }

typedef enum {
    CMP_VOID    = 0,    /* comparison can't be folded */
    CMP_FALSE   = 1,
    CMP_TRUE    = 2,
} cmp_result;

typedef enum {
    REL_EQ,    // x == c
    REL_LT,    // x < c
    REL_LE,    // x <= c
    REL_SIZE
} rel_op;

//  a <= x <=  b   i.e range of x is between a and b
enum  case_range {
    CASE_LOW,         // c < a
    CASE_LOW_EQ,      // c == a
    CASE_HIGH,        // c > b
    CASE_HIGH_EQ,     // c == b
    CASE_SIZE
};

static const char CmpResult[REL_SIZE][CASE_SIZE] = {
//    c < a      c == a     c > b      c == b
    { CMP_FALSE, CMP_VOID , CMP_FALSE, CMP_VOID },  // x == c
    { CMP_FALSE, CMP_FALSE, CMP_TRUE , CMP_VOID },  // x < c
    { CMP_FALSE, CMP_VOID , CMP_TRUE , CMP_TRUE },  // x <= c
};

#define MAXSIZE         64      /* Must not be greater! */

static int CmpType( const type_def *tipe )
/****************************************/
/* Convert cg type to a value used by CheckCmpRange */
{
    int     ret;

    ret =  tipe->length * 8;
    if( tipe->attr & TYPE_SIGNED )
        ret |= SIGN_BIT;
    return( ret );
}

static cmp_result CheckCmpRange( BackRun& back, cg_op opcode, int op_type, float_handle cf )
/***************************************************************************/
/* Check if comparison 'op' of operand of type 'op_type' against constant
 * 'val' can be folded, eg. '(unsigned char)x <= 255'. Integer only, can
 * be used for bitfields (op_type contains number of bits).
 */
{
    enum case_range     range;
    cmp_result          ret;
    signed_64           low;
    signed_64           high;
    signed_64           val;
    rel_op              rel;
    bool                rev_ret = false;

    /* Map cg rel ops to equivalent cases */
    switch( opcode ) {
    case O_NE:
        rev_ret = true;
        /* fall through */
    case O_EQ:
        rel = REL_EQ;
        break;
    case O_GE:
        rev_ret = true;
        /* fall through */
    case O_LT:
        rel = REL_LT;
        break;
    case O_GT:
        rev_ret = true;
        /* fall through */
    case O_LE:
        rel = REL_LE;
        break;
    default:
        rel = static_cast<rel_op>(0);
        Zoiks( back, ZOIKS_112 );
    }
    /* Determine type range */
    if( NumSign( op_type ) ) {
        Set64Val( low, 0, 0x80000000 );
        I64ShiftREq( &low, MAXSIZE - NumBits( op_type ) );
        U64Not( high, low );
    } else {
        Set64ValZero( low );
        U64Not( high, low );
        U64ShiftREq( &high, MAXSIZE - NumBits( op_type ) );
    }
    /* Determine how to compare */
    val = CFCnvF64( cf );
    if( U64isEq( val, low ) ) {
        range = CASE_LOW_EQ;
    } else if( U64isEq( val, high ) ) {
        range = CASE_HIGH_EQ;
    } else if( NumBits( op_type ) < MAXSIZE ) { /* Can't be outside range */
        if( I64Cmp( &val, &low ) < 0 ) {      /* Don't need unsigned compare */
            range = CASE_LOW;
        } else if( I64Cmp( &val, &high ) > 0 ) {
            range = CASE_HIGH;
        } else {
            range = CASE_SIZE;
        }
    } else {
        range = CASE_SIZE;
    }
    /* Figure out comparison result, if possible */
    if( range != CASE_SIZE ) {
        ret = static_cast<cmp_result>(CmpResult[rel][range]);
        if( rev_ret && (ret != CMP_VOID) ) {
            /* Flip result */
            if( ret == CMP_FALSE ) {
                ret = CMP_TRUE;
            } else {
                ret = CMP_FALSE;
            }
        }
    } else {
        ret = CMP_VOID;
    }
    return( ret );
}

static int_32 CFConvertByType( float_handle cf, const type_def *tipe )
/********************************************************************/
{
    int_32      data;

    data = CFCnvF32( cf );
    switch( tipe->length ) {
    case 1:
        if( tipe->attr & TYPE_SIGNED ) {
            data = static_cast<signed_8>(data);
        } else {
            data = static_cast<unsigned_8>(data);
        }
        break;
    case 2:
        if( tipe->attr & TYPE_SIGNED ) {
            data = static_cast<int_16>(data);
        } else {
            data = static_cast<uint_16>(data);
        }
        break;
    }
    return( data );
}

static signed_64 CFGetInteger64Value( BackRun& back, float_handle cf )
/*****************************************************/
{
    signed_64       value;
    int             neg;
    float_handle    trunc;

    trunc = CFTrunc( back, &back.cfloat.cgh, cf );
    neg = CFTest( trunc );
    if( neg < 0 )
        CFNegate( trunc );
    value = CFCnvF64( trunc );
    if( neg < 0 )
        U64NegEq( &value );
    CFFree( back, &back.cfloat.cgh, trunc );
    return( value );
}

static  float_handle IntToCF( BackRun& back, signed_64 value, const type_def *tipe )
/*******************************************************************/
{
    if( tipe->attr & TYPE_SIGNED ) {
        switch( tipe->length ) {
        case 1:
            return( CFCnvI32F( back, &back.cfloat.cgh, I64LowByte( value ) ) );
        case 2:
            return( CFCnvI32F( back, &back.cfloat.cgh, I64LowWord( value ) ) );
        case 4:
        case 6:
            return( CFCnvI32F( back, &back.cfloat.cgh, I64Low( value ) ) );
        case 8:
            return( CFCnvI64F( back, &back.cfloat.cgh, U64Low( value ), U64High( value ) ) );
        default:
            Zoiks( back, ZOIKS_112 );
            return( NULL );
        }
    } else {
        switch( tipe->length ) {
        case 1:
            return( CFCnvU32F( back, &back.cfloat.cgh, U64LowByte( value ) ) );
        case 2:
            return( CFCnvU32F( back, &back.cfloat.cgh, U64LowWord( value ) ) );
        case 4:
        case 6:
            return( CFCnvU32F( back, &back.cfloat.cgh, U64Low( value ) ) );
        case 8:
            return( CFCnvU64F( back, &back.cfloat.cgh, U64Low( value ), U64High( value ) ) );
        default:
            Zoiks( back, ZOIKS_112 );
            return( NULL );
        }
    }
}

static  tn      IntToType( BackRun& back, int_32 value, const type_def *tipe )
/*************************************************************/
{
    signed_64   temp;

    Set64ValI32( temp, value );
    return( TGConst( back, IntToCF( back, temp, tipe ), tipe ) );
}


static  tn      Int64ToType( BackRun& back, signed_64 value, const type_def *tipe )
/******************************************************************/
{
    return( TGConst( back, IntToCF( back, value, tipe ), tipe ) );
}


static  tn      CFToType( BackRun& back, float_handle cf, const type_def *tipe )
/***************************************************************/
{
    tn          result;

    if( (tipe->attr & TYPE_FLOAT) == 0 ) {
        result = TGConst( back, IntToCF( back, CFGetInteger64Value( back, cf ), tipe ), tipe );
        CFFree( back, &back.cfloat.cgh, cf );
    } else {
        result = TGConst( back, cf, tipe );
    }
    return( result );
}


int     GetLog2( uint_32 value )
/******************************/
{
    uint_32     count;
    int         log;

    if( IsPowerOfTwo( value ) && value != 0 ) {
        log = 0;
        count = 1;
        for( ;; ) {
            if( count == value )
                return( log );
            count += count;
            ++log;
        }
    }
    return( -1 );
}


tn      FoldTimes( BackRun& back, tn left, tn rite, const type_def *tipe )
/*********************************************************/
{
    tn              temp;
    tn              fold;
    int             test;
    int             log;
    float_handle    lv;
    float_handle    rv;
    uint_32         li;
    uint_32         ri;

    if( left->class_ == TN_CONS ) {
        temp = left;
        left = rite;
        rite = temp;
    }
    if( rite->class_ != TN_CONS )
        return( NULL );
    if( left->class_ == TN_BINARY && left->u1.t.op == O_TIMES &&
        tipe == left->tipe && !HasBigConst( tipe ) ) {
        if( left->u.left->class_ == TN_CONS ) {
            left->u.left = FoldTimes( back, left->u.left, rite, tipe );
            return( left );
        }
        if( left->u1.t.rite->class_ == TN_CONS ) {
            left->u1.t.rite = FoldTimes( back, left->u1.t.rite, rite, tipe );
            return( left );
        }
    }
    if( left->class_ == TN_BINARY && left->u1.t.op == O_LSHIFT && tipe == left->tipe ) {
        if( !HasBigConst( tipe ) && left->u1.t.rite->class_ == TN_CONS ) {
            li = 1;
            for( log = left->u1.t.rite->u.name->c.lo.u.int_value; log > 0; --log ) {
                li <<= 1;
            }
            BurnTree( back, left->u1.t.rite );
            left->u1.t.rite = CFToType( back, CFCnvU32F( back, &back.cfloat.cgh, li ), tipe );
            left->u1.t.op = O_TIMES;
            left->u1.t.rite = FoldTimes( back, left->u1.t.rite, rite, tipe );
            return( left );
        }
    }
    rv = rite->u.name->c.u.cfval;
    if( left->class_ == TN_CONS ) {
        lv = left->u.name->c.u.cfval;
        if( !HasBigConst( tipe ) && CFIs32( lv ) && CFIs32( rv ) ) {
            li = CFConvertByType( lv, tipe );
            ri = CFConvertByType( rv, tipe );
            ri = li * ri;
            fold = IntToType( back, ri, tipe );
        } else {
            fold = CFToType( back, CFMul( back, &back.cfloat.cgh, left->u.name->c.u.cfval, rv ), tipe );
        }
        BurnTree( back, left );
        BurnTree( back, rite );
        return( fold );
    }
    if( !HasBigConst( tipe ) && rite->u.name->c.lo.u.int_value == 1 ) {
        BurnTree( back, rite );
        return( left );
    }
    test = CFTest( rv );
    if( test == 0 ) {
        return( TGBinary( back, O_COMMA, TGTrash( left ), rite, tipe ) );
    }
    if( HasBigConst( tipe ) )
        return( NULL );
    if( test < 0 ) {
        CFNegate( rv );
    }
    fold = NULL;
    if( CFIsU32( rv ) ) {
        log = GetLog2( CFConvertByType( rv, tipe ) );
        if( log != -1 ) {
            fold = TGBinary( back, O_LSHIFT, left,
                          IntToType( back, log, back.data.TypeInteger ), tipe );
            if( test < 0 ) {
                fold = TGUnary( back, O_UMINUS, fold, tipe );
            }
            BurnTree( back, rite );
        }
    }
    if( test < 0 ) {
        CFNegate( rv );
    }
    return( fold );
}


float_handle OkToNegate( BackRun& back, float_handle value, const type_def *tipe )
/*****************************************************************/
/* make sure we don't negate an unsigned and get out of range */
/* for example -MAX_LONG is no longer an integer type */
{
    float_handle    neg;

    if( HasBigConst( tipe ) && ( tipe->attr & TYPE_FLOAT ) == 0 )
        return( NULL );
    neg = CFCopy( back, &back.cfloat.cgh, value );
    CFNegate( neg );
    if( HasBigConst( tipe ) )
        return( neg );
    if( tipe->attr & TYPE_SIGNED )
        return( neg );
    if( CFIsSize( neg, tipe->length ) )
        return( neg );
    CFFree( back, &back.cfloat.cgh, neg );
    return( NULL );
}

tn      FoldMinus( BackRun& back, tn left, tn rite, const type_def *tipe )
/*********************************************************/
{
    tn              fold;
    float_handle    lv;
    float_handle    rv;
    uint_32         li;
    uint_32         ri;

    fold = NULL;
    if( left->class_ == TN_CONS ) {
        lv = left->u.name->c.u.cfval;
        if( rite->class_ == TN_CONS ) {
            rv = rite->u.name->c.u.cfval;
            if( !HasBigConst( tipe ) && CFIs32( lv ) && CFIs32( rv ) ) {
                li = CFConvertByType( lv, tipe );
                ri = CFConvertByType( rv, tipe );
                ri = li - ri;
                fold = IntToType( back, ri, tipe );
            } else {
                fold = CFToType( back, CFSub( back, &back.cfloat.cgh, lv, rite->u.name->c.u.cfval ),
                                  tipe );
            }
            BurnTree( back, rite );
            BurnTree( back, left );
        } else if( CFTest( lv ) == 0 ) {
            fold = TGUnary( back, O_UMINUS, rite, tipe );
            BurnTree( back, left );
        }
    } else if( rite->class_ == TN_CONS ) {
        rv = OkToNegate( back, rite->u.name->c.u.cfval, tipe );
        if( rv != NULL ) {
            fold = TGBinary( back, O_PLUS, left, TGConst( back, rv, rite->tipe ),
                              tipe );
            BurnTree( back, rite );
        }
    } else if( rite->class_ == TN_UNARY && rite->u1.t.op == O_UMINUS ) {
        fold = TGBinary( back, O_PLUS, left, rite->u.left, tipe );
        rite->u.left = NULL;
        BurnTree( back, rite );
    }
    return( fold );
}


static const type_def *FixAddType( tn left, tn rite, const type_def *tipe )
{
    if( left->tipe != tipe ) {
        if( left->tipe == rite->tipe ) {
            tipe = left->tipe;
        }
    }
    return( tipe );
}


tn      FoldPlus( BackRun& back, tn left, tn rite, const type_def *tipe )
/**********************************************************/
{
    tn              fold;
    tn              temp;
    float_handle    lv;
    float_handle    rv;
    uint_32         li;
    uint_32         ri;

    if( left->class_ == TN_CONS ) {
        temp = left;
        left = rite;
        rite = temp;
    }
    fold = NULL;
    if( left->class_ == TN_CONS ) {
        lv = left->u.name->c.u.cfval;
        if( rite->class_ == TN_CONS ) {
            rv = rite->u.name->c.u.cfval;
            if( !HasBigConst( tipe ) && CFIs32( lv ) && CFIs32( rv ) ) {
                li = CFConvertByType( lv, tipe );
                ri = CFConvertByType( rv, tipe );
                ri = li + ri;
                fold = IntToType( back, ri, tipe );
            } else {
                fold = CFToType( back, CFAdd( back, &back.cfloat.cgh, lv, rite->u.name->c.u.cfval ),
                                  tipe );
            }
            BurnTree( back, rite );
            BurnTree( back, left );
        } else if( CFTest( lv ) == 0 ) {
            fold = TGConvert( back, rite, tipe );
            BurnTree( back, left );
        }
    } else if( rite->class_ == TN_CONS ) {
        if( CFTest( rite->u.name->c.u.cfval ) == 0 ) {
            fold = TGConvert( back, left, tipe );
            BurnTree( back, rite );
        } else if( left->class_ == TN_BINARY && left->u1.t.op == O_PLUS &&
                   tipe == left->tipe && !HasBigConst( tipe ) ) {
            if( left->u.left->class_ == TN_CONS ) {
                tipe = FixAddType( left->u.left, rite, tipe );
                if( left->u.left->tipe == tipe ) {
                    fold = FoldPlus( back, left->u.left, rite, tipe );
                    left->u.left = fold;
                    fold = left;
                }
            } else if( left->u1.t.rite->class_ == TN_CONS ) {
                tipe = FixAddType( left->u1.t.rite, rite, tipe );
                if( left->u1.t.rite->tipe == tipe ) {
                    fold = FoldPlus( back, left->u1.t.rite, rite, tipe );
                    left->u1.t.rite = fold;
                    fold = left;
                }
            }
        }
    } else if( rite->class_ == TN_UNARY && rite->u1.t.op == O_UMINUS ) {
        fold = TGBinary( back, O_MINUS, left, rite->u.left, tipe );
        rite->u.left = NULL;
        BurnTree( back, rite );
    } else if( left->class_ == TN_UNARY && left->u1.t.op == O_UMINUS ) {
        fold = TGBinary( back, O_MINUS, rite, left->u.left, tipe );
        left->u.left = NULL;
        BurnTree( back, left );
    }
    return( fold );
}


static  tn      Halve( BackRun& back, tn left, const type_def *tipe )
/****************************************************/
{
#define ONE_HALF "0.5"

    return( TGBinary( back, O_TIMES, left, TGConst( back, CFCnvSF( back, &back.cfloat.cgh, ONE_HALF ), tipe ), tipe ) );
}

tn      FoldPow( BackRun& back, tn left, tn rite, const type_def *tipe )
/*********************************************************/
{
    tn          fold;

    fold = NULL;
    if( left->class_ == TN_UNARY && left->u1.t.op == O_SQRT ) {
        fold = Halve( back, rite, tipe );
        fold = TGBinary( back, O_POW, left->u.left, fold, tipe );
        left->u.left = NULL;
        BurnTree( back, left );
    } else if( left->class_ == TN_UNARY && left->u1.t.op == O_EXP ) {
        left->u.left = TGBinary( back, O_TIMES, left->u.left, rite, tipe );
        fold = left;
    } else if( left->class_ == TN_BINARY && left->u1.t.op == O_POW ) {
        left->u1.t.rite = TGBinary( back, O_TIMES, left->u1.t.rite, rite, tipe );
        fold = left;
    }
    return( fold );
}


tn      FoldAnd( BackRun& back, tn left, tn rite, const type_def *tipe )
/*********************************************************/
{
    tn              fold;
    float_handle    rv;
    float_handle    lv;

    if( left->class_ == TN_CONS ) {
        fold = left;
        left = rite;
        rite = fold;
    }
    fold = NULL;
    if( left->class_ == TN_CONS ) {
        lv = left->u.name->c.u.cfval;
        rv = rite->u.name->c.u.cfval;
        if( CFIs32( lv ) && CFIs32( rv ) ) {
            uint_32     and_;

            and_ = CFConvertByType( rv, tipe ) & CFConvertByType( lv, tipe );
            fold = IntToType( back, and_, tipe );
            BurnTree( back, left );
            BurnTree( back, rite );
        } else if( CFIs64( lv ) && CFIs64( rv ) ) {
            unsigned_64     and_;
            unsigned_64     li;
            unsigned_64     ri;

            li = CFGetInteger64Value( back, lv );
            ri = CFGetInteger64Value( back, rv );

            U64And( and_, li, ri );
            fold = Int64ToType( back, and_, tipe );
            BurnTree( back, left );
            BurnTree( back, rite );
        }
    } else if( rite->class_ == TN_CONS ) {
        rv = rite->u.name->c.u.cfval;
        /* For any X: X & 0 = 0, and X & ~0 = X */
        if( CFTest( rv ) == 0 ) {
            left = TGTrash( left );
            fold = TGBinary( back, O_COMMA, left, IntToType( back, 0, tipe ), tipe );
            BurnTree( back, rite );
        } else if( !HasBigConst( tipe ) && rite->u.name->c.lo.u.int_value == -1 ) {
            fold = TGConvert( back, left, tipe );
            BurnTree( back, rite );
        }
    }
    return( fold );
}


tn      FoldOr( BackRun& back, tn left, tn rite, const type_def *tipe )
/********************************************************/
{
    tn              fold;
    float_handle    rv;
    float_handle    lv;

    if( left->class_ == TN_CONS ) {
        fold = left;
        left = rite;
        rite = fold;
    }
    fold = NULL;
    if( left->class_ == TN_CONS ) {
        lv = left->u.name->c.u.cfval;
        rv = rite->u.name->c.u.cfval;
        if( CFIs32( lv ) && CFIs32( rv ) ) {
            uint_32     or_;

            or_ = CFConvertByType( rv, tipe ) | CFConvertByType( lv, tipe );
            fold = IntToType( back, or_, tipe );
            BurnTree( back, left );
            BurnTree( back, rite );
        } else if( CFIs64( lv ) && CFIs64( rv ) ) {
            unsigned_64     or_;
            unsigned_64     li;
            unsigned_64     ri;

            li = CFGetInteger64Value( back, lv );
            ri = CFGetInteger64Value( back, rv );

            U64Or( or_, li, ri );
            fold = Int64ToType( back, or_, tipe );
            BurnTree( back, left );
            BurnTree( back, rite );
        }
    } else if( rite->class_ == TN_CONS ) {
        rv = rite->u.name->c.u.cfval;
        /* For any X: X | 0 = X, and X | ~0 = ~0 */
        if( CFTest( rv ) == 0 ) {
            fold = TGConvert( back, left, tipe );
            BurnTree( back, rite );
        } else if( !HasBigConst( tipe ) && rite->u.name->c.lo.u.int_value == -1 ) {
            left = TGTrash( left );
            fold = TGBinary( back, O_COMMA, left, IntToType( back, -1, tipe ), tipe );
            BurnTree( back, rite );
        }
    }
    return( fold );
}


tn      FoldXor( BackRun& back, tn left, tn rite, const type_def *tipe )
/*********************************************************/
{
    tn              fold;
    float_handle    rv;
    float_handle    lv;

    if( left->class_ == TN_CONS ) {
        fold = left;
        left = rite;
        rite = fold;
    }
    fold = NULL;
    if( left->class_ == TN_CONS ) {
        lv = left->u.name->c.u.cfval;
        rv = rite->u.name->c.u.cfval;
        if( CFIs32( lv ) && CFIs32( rv ) ) {
            uint_32     li;
            uint_32     ri;

            li = CFConvertByType( lv, tipe );
            ri = CFConvertByType( rv, tipe );
            ri = li ^ ri;
            fold = IntToType( back, ri, tipe );
            BurnTree( back, left );
            BurnTree( back, rite );
        } else if( CFIs64( lv ) && CFIs64( rv ) ) {
            unsigned_64     xor_;
            unsigned_64     li;
            unsigned_64     ri;

            li = CFGetInteger64Value( back, lv );
            ri = CFGetInteger64Value( back, rv );

            U64Xor( xor_, li, ri );
            fold = Int64ToType( back, xor_, tipe );
            BurnTree( back, left );
            BurnTree( back, rite );
        }
    } else if( rite->class_ == TN_CONS ) {
        rv = rite->u.name->c.u.cfval;
        /* For any X: X ^ 0 = X, and X ^ ~0 = ~X */
        if( CFTest( rv ) == 0 ) {
            fold = TGConvert( back, left, tipe );
            BurnTree( back, rite );
        } else if( !HasBigConst( tipe ) && rite->u.name->c.lo.u.int_value == -1 ) {
            fold = TGUnary( back, O_COMPLEMENT, left, tipe );
            BurnTree( back, rite );
        }
    }
    return( fold );
}


tn      FoldRShift( BackRun& back, tn left, tn rite, const type_def *tipe )
/************************************************************/
{
    tn              fold;
    float_handle    rv;
    float_handle    lv;
    int_32          ri;

    fold = NULL;
    if( rite->class_ == TN_CONS ) {
        rv = rite->u.name->c.u.cfval;
        ri = CFConvertByType( rv, tipe );
        if( left->class_ == TN_CONS ) {
            bool    done = false;

            if( ri >= (tipe->length * 8) ) {
                if( tipe->attr & TYPE_SIGNED ) {
                    // For signed shifts, reduce the shift amount and
                    // then do the math
                    ri = (tipe->length * 8) - 1;
                } else {
                    fold = IntToType( back, 0, tipe );
                    done = true;
                }
            }
            if( !done ) {
                lv = left->u.name->c.u.cfval;
                if( !HasBigConst( tipe ) && CFIs32( lv ) && CFIs32( rv ) ) {
                    int_32      li;
                    uint_32     shft;

                    li = CFConvertByType( lv, tipe );
                    if( tipe->attr & TYPE_SIGNED ) {
                        shft = li >> ri;
                    } else {
                        shft = static_cast<uint_32>(li) >> static_cast<uint_32>(ri);
                    }
                    fold = IntToType( back, shft, tipe );
                } else if( CFIs64( lv ) && CFIs64( rv ) ) {
                    signed_64       rsh;
                    signed_64       li;

                    li = CFGetInteger64Value( back, lv );

                    if( tipe->attr & TYPE_SIGNED ) {
                        I64ShiftR( &rsh, &li, ri );
                    } else {
                        U64ShiftR( &rsh, &li, ri );
                    }
                    fold = Int64ToType( back, rsh, tipe );
                }
            }
            BurnTree( back, left );
            BurnTree( back, rite );
        } else if( ri == 0 ) {
            fold = TGConvert( back, left, tipe );
            BurnTree( back, rite );
        } else if( ri >= (tipe->length * 8) && (tipe->attr & TYPE_SIGNED) == 0 ) {
            fold = TGBinary( back, O_COMMA, TGTrash( left ), IntToType( back, 0, tipe ), tipe );
            BurnTree( back, rite );
        }
    }
    return( fold );
}


tn      FoldLShift( BackRun& back, tn left, tn rite, const type_def *tipe )
/************************************************************/
{
    tn              fold;
    float_handle    rv;
    float_handle    lv;
    int_32          ri;

    fold = NULL;
    ri = 0;
    if( rite->class_ == TN_CONS ) {
        rv = rite->u.name->c.u.cfval;
        if( CFIs32( rv ) ) {
            ri = CFConvertByType( rv, tipe );
        } else if( CFIs64( rv ) ) {
            /* If shift amount won't fit into 32 bits, anything big enough will do */
            ri = 0xffff;
        }
        if( left->class_ == TN_CONS ) {
            if( ri >= (tipe->length * 8) ) {
                fold = IntToType( back, 0, tipe );
            } else {
                lv = left->u.name->c.u.cfval;
                if( !HasBigConst( tipe ) && CFIs32( lv ) && CFIs32( rv ) ) {
                    int_32  li;

                    li = CFConvertByType( lv, tipe );
                    fold = IntToType( back, li << ri, tipe );
                } else if( CFIs64( lv ) && CFIs64( rv ) ) {
                    signed_64       lsh;
                    signed_64       li;

                    li = CFGetInteger64Value( back, lv );

                    U64ShiftL( &lsh, &li, ri );
                    fold = Int64ToType( back, lsh, tipe );
                }
            }
            BurnTree( back, left );
            BurnTree( back, rite );
        } else if( ri == 0 ) {
            fold = TGConvert( back, left, tipe );
            BurnTree( back, rite );
        } else if( ri >= (tipe->length * 8) ) {
            fold = TGBinary( back, O_COMMA, TGTrash( left ), IntToType( back, 0, tipe ), tipe );
            BurnTree( back, rite );
        }
    }
    return( fold );
}


tn      FoldDiv( BackRun& back, tn left, tn rite, const type_def *tipe )
/*********************************************************/
{
    tn              fold;
    float_handle    rv;
    float_handle    lv;
    float_handle    tmp;
    int             log;

    fold = NULL;
    if( rite->class_ == TN_CONS ) {
        rv = rite->u.name->c.u.cfval;
        if( HasBigConst( tipe ) ) {
            lv = left->u.name->c.u.cfval;
            if( CFTest( rv ) != 0 && left->class_ == TN_CONS ) {
                if( tipe->attr & TYPE_FLOAT ) {
                    fold = CFToType( back, CFDiv( back, &back.cfloat.cgh, lv, rv ), tipe );
                } else {    /* Must be a 64-bit integer. */
                    signed_64       div;
                    signed_64       rem;
                    signed_64       li;
                    signed_64       ri;

                    assert( CFIs64( lv ) );
                    assert( CFIs64( rv ) );
                    li = CFGetInteger64Value( back, lv );
                    ri = CFGetInteger64Value( back, rv );

                    if( tipe->attr & TYPE_SIGNED ) {
                        I64Div( &li, &ri, &div, &rem );
                    } else {
                        U64Div( &li, &ri, &div, &rem );
                    }
                    fold = Int64ToType( back, div, tipe );
                }
                BurnTree( back, left );
                BurnTree( back, rite );
            } else if( ( tipe->attr & TYPE_FLOAT ) &&
                ( IsModel( back, CGSW_GEN_FP_UNSTABLE_OPTIMIZATION ) ||
                ( CFIsU32( rv ) && GetLog2( CFConvertByType( rv, tipe ) ) != -1 ) ) ) {
                if( CFTest( rv ) != 0 ) {
                    tmp = CFInverse( back, &back.cfloat.cgh, rv );
                    if( tmp != NULL ) {
                        fold = TGBinary( back, O_TIMES, left, TGConst( back, tmp, tipe ), tipe );
                        BurnTree( back, rite );
                    }
                }
            }
        } else if( CFTest( rv ) != 0 && left->class_ == TN_CONS ) {
            lv = left->u.name->c.u.cfval;
            if( tipe->attr & TYPE_SIGNED ) {
                if( CFIsI32( lv ) && CFIsI32( rv ) ) {
                    int_32  li;
                    int_32  ri;

                    li = CFConvertByType( lv, tipe );
                    ri = CFConvertByType( rv, tipe );
                    fold = IntToType( back, li / ri, tipe );
                    BurnTree( back, left );
                    BurnTree( back, rite );
                }
            } else {
                if( CFIsU32( lv ) && CFIsU32( rv ) ) {
                    uint_32     li;
                    uint_32     ri;

                    li = CFCnvF32( lv );
                    ri = CFCnvF32( rv );
                    U32ModDiv( &li, ri );
                    fold = IntToType( back, li, tipe );
                    BurnTree( back, left );
                    BurnTree( back, rite );
                }
            }
        } else if( rite->u.name->c.lo.u.int_value == 1 ) {
            fold = TGConvert( back, left, tipe );
            BurnTree( back, rite );
        } else if( !HasBigConst( tipe ) && ( left->tipe->attr & TYPE_SIGNED ) == 0 ) {
            if( CFIsU32( rv ) ) {
                log = GetLog2( rite->u.name->c.lo.u.int_value );
                if( log != -1 ) {
                    fold = TGBinary( back, O_RSHIFT, left, IntToType( back, log, back.data.TypeInteger ), tipe );
                    BurnTree( back, rite );
                }
            }
        }
    } else if( left->class_ == TN_CONS ) {
        if( CFTest( left->u.name->c.u.cfval ) == 0 ) {
            rite = TGTrash( rite );
            fold = TGBinary( back, O_COMMA, rite, TGConvert( back, left, tipe ), tipe );
        }
    }
    return( fold );
}


tn      FoldMod( BackRun& back, tn left, tn rite, const type_def *tipe )
/*********************************************************/
{
    tn              fold;
    float_handle    rv;
    float_handle    lv;
    int             log;

    fold = NULL;
    if( rite->class_ == TN_CONS ) {
        rv = rite->u.name->c.u.cfval;
        if( CFTest( rv ) != 0 && left->class_ == TN_CONS ) {
            lv = left->u.name->c.u.cfval;
            if( CFIs64( lv ) && CFIs64( rv ) ) {
                signed_64       div;
                signed_64       rem;
                signed_64       li;
                signed_64       ri;

                li = CFGetInteger64Value( back, lv );
                ri = CFGetInteger64Value( back, rv );

                if( tipe->attr & TYPE_SIGNED ) {
                    I64Div( &li, &ri, &div, &rem );
                } else {
                    U64Div( &li, &ri, &div, &rem );
                }
                fold = Int64ToType( back, rem, tipe );
                BurnTree( back, left );
                BurnTree( back, rite );
            } else {
                if( tipe->attr & TYPE_SIGNED ) {
                    if( CFIsI32( lv ) && CFIsI32( rv ) ) {
                        int_32  ri;
                        int_32  li;

                        li = CFConvertByType( lv, tipe );
                        ri = CFConvertByType( rv, tipe );
                        fold = IntToType( back, li % ri, tipe );
                        BurnTree( back, left );
                        BurnTree( back, rite );
                    }
                } else {
                    if( CFIsU32( lv ) && CFIsU32( rv ) ) {
                        uint_32     ri;
                        uint_32     li;

                        li = CFConvertByType( lv, tipe );
                        ri = CFConvertByType( rv, tipe );
                        li = U32ModDiv( &li, ri );
                        fold = IntToType( back, li, tipe );
                        BurnTree( back, left );
                        BurnTree( back, rite );
                    }
                }
            }
        } else if( !HasBigConst( tipe ) && rite->u.name->c.lo.u.int_value == 1 ) {
            fold = CFToType( back, CFCnvIF( back, &back.cfloat.cgh, 0 ), tipe );
            fold = TGBinary( back, O_COMMA, left, fold, tipe );
            BurnTree( back, rite );
        } else if( !HasBigConst( tipe ) ) {
            if( ( left->tipe->attr & TYPE_SIGNED ) == 0 ) {
                if( CFIsU32( rv ) ) {
                    uint_32 ri = CFConvertByType( rv, tipe );
                    log = GetLog2( ri );
                    if( log != -1 ) {
                        fold = TGBinary( back, O_AND, left, IntToType( back, ri - 1, tipe ), tipe );
                        BurnTree( back, rite );
                    }
                }
            } else if( IsntModel( back, CGSW_GEN_NO_OPTIMIZATION )
              && IsntModel( back, CGSW_GEN_DBG_LOCALS )
              && TGCanDuplicate( left ) ) {
                /* signed int a; optimize a % rv like IBM does:
                   int b = a >> 31;
                   t1 = (a ^ b) - b;
                   t2 = t & (rv - 1);
                   fold = (t2 ^ b) - b;
                 */
                int_32 ri = CFConvertByType( rv, tipe );
                if( ri < 0 )    /* sign of constant doesn't matter */
                    ri = -ri;
                log = GetLog2( ri );
                if( log != -1 && ri > 1 ) {
                    tn b1, b2, b3, b4; /* 4 copies of b - will be treated as CSE */
                    tn left1, t1, t2;

                    left1 = TGDuplicate( back, left );
                    b1 = TGBinary( back, O_RSHIFT, left1, IntToType( back, 31, tipe ), tipe );
                    b2 = TGDuplicate( back, b1 );
                    b3 = TGDuplicate( back, b1 );
                    b4 = TGDuplicate( back, b1 );
                    t1 = TGBinary( back, O_MINUS, TGBinary( back, O_XOR, left, b1, tipe ), b2, tipe );
                    t2 = TGBinary( back, O_AND, t1, IntToType( back, ri-1, tipe ), tipe );
                    fold = TGBinary( back, O_MINUS, TGBinary( back, O_XOR, t2, b3, tipe ), b4, tipe );
                    BurnTree( back, rite );
                }
            }
        }
    } else if( left->class_ == TN_CONS ) {
        if( CFTest( left->u.name->c.u.cfval ) == 0 ) {
            rite = TGTrash( rite );
            fold = TGBinary( back, O_COMMA, rite, IntToType( back, 0, tipe ), tipe );
            BurnTree( back, left );
        }
    }
    return( fold );
}


tn      Fold1sComp( BackRun& back, tn left, const type_def *tipe )
/***************************************************/
{
    tn              new_tn;
    float_handle    lv;

    new_tn = NULL;
    if( !HasBigConst( tipe ) && left->class_ == TN_CONS ) {
        lv = left->u.name->c.u.cfval;
        if( CFIs32( lv ) ) {
            new_tn = IntToType( back, ~CFConvertByType( lv, tipe ), tipe );
            BurnTree( back, left );
        }
    }
    return( new_tn );
}


tn      FoldUMinus( BackRun& back, tn left, const type_def *tipe )
/***************************************************/
{
    tn              new_tn;
    float_handle    lv;

    new_tn = NULL;
    if( left->class_ == TN_CONS ) {
        lv = OkToNegate( back, left->u.name->c.u.cfval, tipe );
        if( lv != NULL ) {
            new_tn = CFToType( back, lv, tipe );
            BurnTree( back, left );
        }
    } else if( left->class_ == TN_UNARY && left->u1.t.op == O_UMINUS ) {
        new_tn = left->u.left;
        left->u.left = NULL;
        BurnTree( back, left );
    }
    return( new_tn );
}


tn      FoldSqrt( BackRun& back, tn left, const type_def *tipe )
/*************************************************/
{
    tn          fold;

    fold = NULL;
    if( left->class_ == TN_UNARY && left->u1.t.op == O_EXP ) {
        left->u.left = Halve( back, left->u.left, tipe );
        fold = left;
    } else if( left->class_ == TN_BINARY && left->u1.t.op == O_POW ) {
        left->u1.t.rite = Halve( back, left->u1.t.rite, tipe );
        fold = left;
    }
    return( fold );
}


tn      FoldLog( BackRun& back, cg_op opcode, tn left, const type_def *tipe )
/************************************************************/
{
    tn          fold;

    fold = NULL;
    if( left->class_ == TN_UNARY && left->u1.t.op == O_SQRT ) {
        fold = TGUnary( back, opcode, left->u.left, tipe );
        fold = Halve( back, fold, tipe );
        left->u.left = NULL;
        BurnTree( back, left );
    } else if( left->class_ == TN_BINARY && left->u1.t.op == O_POW ) {
        fold = TGUnary( back, opcode, left->u.left, tipe );
        fold = TGBinary( back, O_TIMES, left->u1.t.rite, fold, tipe );
        left->u.left = NULL;
        left->u1.t.rite = NULL;
        BurnTree( back, left );
    }
    return( fold );
}


tn      FoldFlAnd( BackRun& back, tn left, tn rite )
/*******************************************/
{
    tn  fold;

    fold = NULL;
    if( left->class_ == TN_CONS ) {
        if( rite->class_ == TN_CONS ) {
            if( CFTest( left->u.name->c.u.cfval ) && CFTest( rite->u.name->c.u.cfval ) ) {
                fold = IntToType( back, FETrue(), back.data.TypeInteger );
            } else {
                fold = IntToType( back, 0, back.data.TypeInteger );
            }
            BurnTree( back, left );
            BurnTree( back, rite );
        } else if( CFTest( left->u.name->c.u.cfval ) == 0 ) {
            // ( 0 && expr ) -> 0
            fold = IntToType( back, 0, back.data.TypeInteger );
            BurnTree( back, left );
            BurnTree( back, rite );
        } else {
            // ( 1 && expr ) -> ( expr )
            fold = rite;
            BurnTree( back, left );
        }
    } else if( rite->class_ == TN_CONS ) {
        if( CFTest( rite->u.name->c.u.cfval ) ) {
            // ( expr && 1 ) -> ( expr )
            fold = left;
            BurnTree( back, rite );
        } else {
            // ( expr && 0 ) -> ( expr, 0 )
            fold = TGNode( back, TN_COMMA, O_NOP, left, IntToType( back, 0, back.data.TypeInteger ), back.data.TypeInteger );
            BurnTree( back, rite );
        }
    }
    return( fold );
}


tn      FoldFlOr( BackRun& back, tn left, tn rite )
/******************************************/
{
    tn  fold;

    fold = NULL;
    if( left->class_ == TN_CONS ) {
        if( CFTest( left->u.name->c.u.cfval ) ) {
            // ( 1 || expr ) -> ( true )
            fold = IntToType( back, FETrue(), back.data.TypeInteger );
            BurnTree( back, left );
            BurnTree( back, rite );
        } else {
            // ( 0 || expr ) -> ( expr )
            fold = rite;
            BurnTree( back, left );
        }
    } else if( rite->class_ == TN_CONS ) {
        if( CFTest( rite->u.name->c.u.cfval ) ) {
            // ( expr || 1 ) -> ( expr, true )
            fold = TGNode( back, TN_COMMA, O_NOP, left, IntToType( back, FETrue(), back.data.TypeInteger ), back.data.TypeInteger );
            BurnTree( back, rite );
        } else {
            // ( expr || 0 ) -> ( expr )
            fold = left;
            BurnTree( back, rite );
        }
    }
    return( fold );
}


tn      FoldFlNot( BackRun& back, tn left )
/**********************************/
{
    tn          fold;
    int         result;


    fold = NULL;
    if( left->class_ == TN_CONS ) {
        if( CFTest( left->u.name->c.u.cfval ) == 0 ) {
            result = FETrue();
        } else {
            result = 0;
        }
        fold = IntToType( back, result, back.data.TypeInteger );
        BurnTree( back, left );
    }
    return( fold );
}

tn      FoldBitCompare( BackRun& back, cg_op opcode, tn left, tn rite )
/**********************************************************/
{
    tn          fold;
    uint_32     new_c;
    uint_32     mask;

    if( left->class_ == TN_CONS ) {
        fold = left;
        left = rite;
        rite = fold;
        opcode = RevOpcode[opcode - O_EQ];
    }
    fold = NULL;
    if( rite->class_ == TN_CONS && left->class_ == TN_BIT_RVALUE && !left->u1.b.is_signed && !HasBigConst( left->tipe ) ) {
        new_c = rite->u.name->c.lo.u.int_value;
        new_c <<= left->u1.b.start;
        mask = TGMask32( left );
        if( ( new_c & ~mask ) == 0 ) { /* idiot comparing out of range*/
            fold = TGUnary( back, O_POINTS, left->u.left, left->tipe );
            fold = TGBinary( back, O_AND, fold, IntToType( back, mask, left->tipe ), left->tipe );
            fold = TGCompare( back, opcode, fold, IntToType( back, new_c, left->tipe ), left->tipe );
            left->u.left = NULL; /* so no recursion*/
            BurnTree( back, left );
            BurnTree( back, rite );
        }
    }
    return( fold );
}


float_handle CnvCFToType( BackRun& back, float_handle cf, const type_def *tipe )
/*****************************************************************/
{
    if( (tipe->attr & TYPE_FLOAT) == 0 ) {
        cf = IntToCF( back, CFGetInteger64Value( back, cf ), tipe );
    } else {
        cf = CFCopy( back, &back.cfloat.cgh, cf );
    }
    return( cf );
}

static  tn      FindBase( tn tree, bool op_eq )
/*********************************************/
{
    type_attr           child_attr;
    type_attr           this_attr;

    for( ;; ) {
        if( tree->class_ != TN_UNARY )
            break;
        if( tree->u1.t.op != O_CONVERT )
            break;
        if( tree->u.left->tipe->length > tree->tipe->length )
            break;
        child_attr = tree->u.left->tipe->attr;
        this_attr = tree->tipe->attr;
        if( op_eq ) {
            // if we are doing a EQ/NE comparison we can ignore sign changes
            child_attr &= ~TYPE_SIGNED;
            this_attr  &= ~TYPE_SIGNED;
        } else if( (this_attr & TYPE_SIGNED) && tree->u.left->tipe->length < tree->tipe->length ) {
            // if we went from smaller unsigned to larger signed type,
            // sign change isn't a problem either
            child_attr |= TYPE_SIGNED;
        }
        if( child_attr != this_attr )
            break;
        tree = tree->u.left;
    }
    return( tree );
}

static  void    BurnToBase( BackRun& back, tn root, tn base )
/********************************************/
{
    tn          next;
    tn          curr;

    for( curr = root; curr != base; curr = next ) {
        assert( curr->class_ == TN_UNARY && curr->u1.t.op == O_CONVERT );
        next = curr->u.left;
        curr->u.left = NULL;
        BurnTree( back, curr );
    }
}

static bool IsObjectAddr( tn tree )
/*********************************/
{
    if( tree->class_ == TN_LEAF && tree->u.addr->format == NF_ADDR ) {
        switch( tree->u.addr->class_ ) {
        case CL_ADDR_GLOBAL:
        case CL_ADDR_TEMP:
            return( true );
        default:
            break;
        }
    }
    return( false );
}

tn  FoldCompare( BackRun& back, cg_op opcode, tn left, tn rite, const type_def *tipe )
/*********************************************************************/
{
    int             compare;
    tn              temp;
    int             result;
    int             true_value;
    float_handle    lv;
    float_handle    rv;
    tn              base_r;
    tn              base_l;
    bool            op_eq;

    if( left->class_ == TN_CONS ) {
        temp = left;
        left = rite;
        rite = temp;
        opcode = RevOpcode[opcode - O_EQ];
    }
    op_eq = false;
    if( ( opcode == O_EQ ) || ( opcode == O_NE ) ) {
        op_eq = true;
    }
    true_value = FETrue();
    result = 0;
    if( left->class_ == TN_CONS ) {
        lv = CnvCFToType( back, left->u.name->c.u.cfval, tipe );
        rv = CnvCFToType( back, rite->u.name->c.u.cfval, tipe );
        compare = CFCompare( lv, rv );
        CFFree( back, &back.cfloat.cgh, lv );
        CFFree( back, &back.cfloat.cgh, rv );
        switch( opcode ) {
        case O_EQ:
            if( compare == 0 ) {
                result = true_value;
            }
            break;
        case O_NE:
            if( compare != 0 ) {
                result = true_value;
            }
            break;
        case O_GT:
            if( compare > 0 ) {
                result = true_value;
            }
            break;
        case O_LT:
            if( compare < 0 ) {
                result = true_value;
            }
            break;
        case O_GE:
            if( compare >= 0 ) {
                result = true_value;
            }
            break;
        case O_LE:
            if( compare <= 0 ) {
                result = true_value;
            }
            break;
        }
        BurnTree( back, left );
        BurnTree( back, rite );
        return( IntToType( back, result, back.data.TypeInteger ) );
    } else if( rite->class_ == TN_CONS ) {
        if( left->class_ != TN_BINARY ) {
            base_l = FindBase( left, false );
            if( base_l != left ) {
                /* For folding comparisons, consider the variable's original type. If eg. a short
                 * was converted to long, we know its value has to be in the short's range.
                 */
                tipe = base_l->tipe;
            }
            if( (tipe->attr & (TYPE_FLOAT | TYPE_POINTER )) == 0 ) {
                cmp_result  cmp;

                cmp = CheckCmpRange( back, opcode, CmpType( tipe ), rite->u.name->c.u.cfval );
                if( cmp != CMP_VOID ) {
                    if( cmp == CMP_TRUE ) {
                        result = true_value;
                    }
                    /* Throw away constant but keep non-const part */
                    BurnTree( back, rite );
                    left = TGTrash( left );
                    return( TGBinary( back, O_COMMA, left, IntToType( back, result, back.data.TypeInteger ), back.data.TypeInteger ) );
                }
                if( IsntModel( back, CGSW_GEN_NULL_DEREF_OK )
                  && IsObjectAddr( left )
                  && ( CFTest( rite->u.name->c.u.cfval ) == 0 ) ) {
                    /* Addresses of globals or local variables are guaranteed not to be null
                     * unless CGSW_GEN_NULL_DEREF_OK is in effect
                     */
                    BurnTree( back, left );
                    BurnTree( back, rite );
                    return( FoldCompare( back, opcode, IntToType( back, 1, back.data.TypeInteger ), IntToType( back, 0, back.data.TypeInteger ), back.data.TypeInteger ) );
                }
            }
            if( ( base_l != left ) && (tipe->attr & TYPE_FLOAT) == 0 ) {
                // If we couldn't fold the comparison, get rid of some lame converts
                // the C++ compiler likes to emit. Careful with floats!
                BurnToBase( back, left, base_l );
                return( TGNode( back, TN_COMPARE, opcode, base_l, rite, back.data.TypeBoolean ) );
            }
            return( NULL );
        }
        if( left->u1.t.op != O_AND )
            return( NULL );
        if( opcode != O_EQ && opcode != O_NE )
            return( NULL );
        if( left->u.left->class_ == TN_CONS ) {
            temp = left->u.left;
            left->u.left = left->u1.t.rite;
            left->u1.t.rite = temp;
        }
        if( left->u1.t.rite->class_ == TN_CONS
         && left->u1.t.rite->u.left == rite->u.left
         && !HasBigConst( tipe )
         && GetLog2( rite->u.name->c.lo.u.int_value ) != -1 ) {
            rite->u.name = AllocIntConst( back, 0 );
            if( opcode == O_EQ ) {
                opcode = O_NE;
            } else {
                opcode = O_EQ;
            }
        }
        if( CFTest( rite->u.name->c.u.cfval ) != 0 )
            return( NULL );
        BurnTree( back, rite );
        left->class_ = TN_COMPARE;
        left->tipe = back.data.TypeBoolean;
        if( opcode == O_EQ ) {
            left->u1.t.op = static_cast<cg_op>(OP_BIT_TEST_FALSE);
        } else {
            left->u1.t.op = static_cast<cg_op>(OP_BIT_TEST_TRUE);
        }
        return( left );
    } else {
        base_r = FindBase( rite, op_eq );
        base_l = FindBase( left, op_eq );
        if( base_r != rite || base_l != left ) {
            if( ( base_r->tipe == base_l->tipe ) && ( ( opcode == O_EQ ) || ( opcode == O_NE ) ) ) {
                BurnToBase( back, rite, base_r );
                BurnToBase( back, left, base_l );
                return( TGNode( back, TN_COMPARE, opcode, base_l, base_r, back.data.TypeBoolean ) );
            }
        }
    }
    return( NULL );
}

static  bool    SimpleLeaf( tn tree )
/***********************************/
{
    if( tree->class_ == TN_UNARY && tree->u1.t.op == O_POINTS ) {
        tree = tree->u.left;
    }
    return( tree->class_ == TN_LEAF );
}


tn      FoldPostGetsCompare( BackRun& back, cg_op opcode, tn left, tn rite, const type_def *tipe )
/*********************************************************************************/
{
//    tn              compare;
    tn              temp;
    int_32          ri;
    int_32          li;
    int_32          value;
    float_handle    rv;
    float_handle    lv;

///    compare = NULL;
    if( left->class_ == TN_CONS ) {
        temp = left;
        left = rite;
        rite = temp;
        opcode = RevOpcode[opcode - O_EQ];
    }

    if( opcode == O_GT && (tipe->attr & TYPE_SIGNED) == 0 ) {
        if( rite->class_ == TN_CONS ) {
            if( CFTest( rite->u.name->c.u.cfval ) == 0 ) {
                opcode = O_NE;
            }
        }
    }

    /*
     * This is a little sick - basically, we have some code which looks
     * like "while( n-- ) {" and we don't want it to mess up our dataflo
     * so we do a little tree re-write here to smooth things over.
     */

    if( opcode != O_EQ && opcode != O_NE )
        return( NULL );
    if( rite->class_ == TN_CONS &&
        left->class_ == TN_POST_GETS &&
        ( left->u1.t.op == O_MINUS || left->u1.t.op == O_PLUS ) ) {
        if( left->u1.t.rite->class_ == TN_CONS && SimpleLeaf( left->u.left ) ) {
            if( tipe == left->tipe ) {
                rv = rite->u.name->c.u.cfval;
                lv = left->u1.t.rite->u.name->c.u.cfval;
                if( !HasBigConst( tipe ) && CFIs32( lv ) && CFIs32( rv ) ) {
                    li = CFConvertByType( lv, tipe );
                    ri = CFConvertByType( rv, tipe );
                    value = ( left->u1.t.op == O_MINUS ) ? ( ri - li ) : ( ri + li );
                    temp = IntToType( back, value, tipe );
                    left->class_ = TN_PRE_GETS;
                    left->optipe = left->tipe;
                    temp = TGNode( back, TN_COMPARE, opcode, left, temp, back.data.TypeBoolean );
                    BurnTree( back, rite );
                    return( temp );
                }
            }
        }
    }
    return( NULL );
}


/* routines above here are called while building the tree */
/* routines below here are called while tearing the tree apart */

static  an Flip( BackRun& back, an name, bool op_false, bool op_true )
/*****************************************************/
{
    label_handle    *temp;

    if( op_false ) {
        if( op_true ) {
            BGDone( back, name );
            name = BGInteger( back, FETrue(), back.data.TypeInteger );
        } else {
            temp = name->u.b.f;
            name->u.b.f = name->u.b.t;
            name->u.b.t = temp;
        }
    } else {
        if( op_true ) {
            /* nothing*/
        } else {
            BGDone( back, name );
            name = BGInteger( back, 0, back.data.TypeInteger );
        }
    }
    return( name );
}


an FoldConsCompare( BackRun& back, cg_op opcode, tn left, tn rite, const type_def *tipe )
/************************************************************************/
{
    tn              temp;
    an              fold;
    int             compare;
    int             compare_true;
    float_handle    f;
    float_handle    t;
    float_handle    rv;

    if( left->class_ == TN_CONS ) {
        temp = left;
        left = rite;
        rite = temp;
        opcode = RevOpcode[opcode - O_EQ];
    }
    fold = NULL;
    if( rite->class_ == TN_CONS ) {
        if( left->class_ == TN_FLOW_OUT ) {
            f = CFCnvIF( back, &back.cfloat.cgh, 0 );
            t = CFCnvIF( back, &back.cfloat.cgh, FETrue() );
            rv = CnvCFToType( back, rite->u.name->c.u.cfval, tipe );
            compare = CFCompare( f, rv );
            compare_true = CFCompare( t, rv );
            CFFree( back, &back.cfloat.cgh, rv );
            CFFree( back, &back.cfloat.cgh, f );
            CFFree( back, &back.cfloat.cgh, t );
            fold = TreeGen( back, left->u.left );
            switch( opcode ) {
            case O_EQ:
                fold = Flip( back, fold, compare == 0, compare_true == 0 );
                break;
            case O_NE:
                fold = Flip( back, fold, compare != 0, compare_true != 0 );
                break;
            case O_GT:
                fold = Flip( back, fold, compare > 0, compare_true > 0 );
                break;
            case O_LT:
                fold = Flip( back, fold, compare < 0, compare_true < 0 );
                break;
            case O_GE:
                fold = Flip( back, fold, compare >= 0, compare_true >= 0 );
                break;
            case O_LE:
                fold = Flip( back, fold, compare <= 0, compare_true <= 0 );
                break;
            case static_cast<cg_op>(OP_BIT_TEST_TRUE):
                if( !HasBigConst( tipe ) ) {
                    fold = Flip( back, fold, false, (rite->u.name->c.lo.u.int_value & FETrue()) != 0 );
                }
                break;
            case static_cast<cg_op>(OP_BIT_TEST_FALSE):
                if( !HasBigConst( tipe ) ) {
                    fold = Flip( back, fold, true, (rite->u.name->c.lo.u.int_value & FETrue()) == 0 );
                }
                break;
            }
            BurnTree( back, rite );
            left->u.left = NULL;
            BurnTree( back, left );
        }
    }
    return( fold );
}


bool    FoldIfTrue( BackRun& back, tn left, label_handle lbl )
/*****************************************************/
{
    bool        folded;

    folded = false;
    if( left->class_ == TN_CONS ) {
        if( CFTest( left->u.name->c.u.cfval ) != 0 ) {
            BGGenCtrl( back, O_GOTO, NULL, lbl, true );
        }
        folded = true;
        BurnTree( back, left );
    }
    return( folded );
}


bool    FoldIfFalse( BackRun& back, tn left, label_handle lbl )
/******************************************************/
{
    bool        folded;

    folded = false;
    if( left->class_ == TN_CONS ) {
        if( CFTest( left->u.name->c.u.cfval ) == 0 ) {
            BGGenCtrl( back, O_GOTO, NULL, lbl, true );
        }
        folded = true;
        BurnTree( back, left );
    }
    return( folded );
}
