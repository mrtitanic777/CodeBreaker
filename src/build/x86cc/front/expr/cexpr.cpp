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
* Description:  C expression parsing.
*
****************************************************************************/


#include <ctype.h>
#include "cvars.h"
#include "cmacadd.h"
#include "cg.h"
#include "cgdefs.h"
#include "cgswitch.h"
#include "i64.h"
#include "caux.h"
#include "cfeinfo.h"
#include "frontrun.hpp"


#define TOKEN2BOOL(t)   ((bool)(t))
#define BOOL2TOKEN(b)   ((TOKEN)(b))


static const struct mathfuncs {
    const char      *name;
    const char      *iname;
    int             parm_count;
    unsigned char   mathop;
} MathFuncs[] = {
    #define mathfunc(name,iname,num,op) {name,iname,num,op}
    #include "cmathfun.h"
    #undef mathfunc
    { NULL, NULL,   0, 0 }
};

static TREEPTR      GenNextParm(FrontRun& front, TREEPTR,TYPEPTR **);
static TREEPTR      StartFunc(FrontRun& front, TREEPTR,TYPEPTR **);
static TREEPTR      GetExpr(FrontRun& front);
static TREEPTR      ExprId(FrontRun& front);
static TREEPTR      ExprOpnd(FrontRun& front);
static TREEPTR      SizeofOp(FrontRun& front, TYPEPTR);
static TREEPTR      ScalarExpr(FrontRun& front, TREEPTR);
static TREEPTR      UnaryPlus(FrontRun& front, TREEPTR);
static TREEPTR      TernOp(FrontRun& front, TREEPTR,TREEPTR,TREEPTR);
static TREEPTR      ColonOp(FrontRun& front, TREEPTR);
static TREEPTR      StartTernary(FrontRun& front, TREEPTR);
static TREEPTR      NotOp(FrontRun& front, TREEPTR);
static TREEPTR      AndAnd(FrontRun& front, TREEPTR);
static TREEPTR      OrOr(FrontRun& front, TREEPTR);
static TREEPTR      GenFuncCall(FrontRun& front, TREEPTR);
static TREEPTR      IndexOp(FrontRun& front, TREEPTR,TREEPTR);
static TREEPTR      SegOp(FrontRun& front, TREEPTR,TREEPTR);
static void         PopNestedParms( FrontRun& front, TYPEPTR **plistptr );
static void         IncSymWeight( FrontRun& front, SYMPTR sym );
static void         AddCallNode( FrontRun& front, TREEPTR tree );

namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct CexprState: cexpr_state.h) */

}  // namespace



void ExprInit( FrontRun& front )
{
    front.cvars.ExprLevel = 0;
    front.cvars.NestedParms = NULL;
    front.cvars.CallNodeList = NULL;
    front.cexpr.LastCallLnk = &front.cvars.CallNodeList;
    front.cexpr.FirstCallLnk = front.cexpr.LastCallLnk;
    InitExprTree(front);
}

//-----------------------------CNODE-----------------------------------

static TREEPTR CallNode( FrontRun& front, TREEPTR func, TREEPTR parms, TYPEPTR func_result_type )
{
    TREEPTR     tree;
    TYPEPTR     typ;

    tree = ExprNode( front, func, OPR_CALL, parms );
    tree->u.expr_type = func_result_type;
    tree->op.u2.result_type = func_result_type;
    typ = func_result_type;
    SKIP_TYPEDEFS( typ );
    if( typ->decl_type != TYP_VOID
      && TypeSize( front, typ ) == 0 ) {
        CErr1( front, ERR_INCOMPLETE_EXPR_TYPE );
    }
    return( tree );
}

static TYPEPTR Far16Type( FrontRun& front, TYPEPTR typ )
/**************************************
 * TODO all this stuff should be part of the func defn.
 * not some hack on for later
 */
{
    TYPEPTR     typ2;

    typ2 = typ;
    SKIP_TYPEDEFS( typ2 );
    if( typ2->decl_type == TYP_ENUM ) {
        typ2 = typ2->object;
    }
    switch( typ2->decl_type ) {
    case TYP_INT:
        typ = GetType( front, TYP_SHORT );
        break;
    case TYP_UINT:
        typ = GetType( front, TYP_USHORT );
        break;
    default:
        break;
    }
    return( typ );
}

static TREEPTR FarPtr16Cvt( FrontRun& front, TREEPTR newparm  )
{
    TYPEPTR         parmtyp;
    pointer_class   op1_ptrclass;
    pointer_class   op2_ptrclass;

    parmtyp = newparm->u.expr_type;
    SKIP_TYPEDEFS( parmtyp );
    if( parmtyp->decl_type == TYP_POINTER ) {
        op1_ptrclass = PTRCLS_FAR16;
        op2_ptrclass = ExprTypeClass( front, newparm->u.expr_type );
        newparm = ExprNode( front, NULL, OPR_CONVERT_PTR, newparm );
        newparm->u.expr_type = parmtyp;
        newparm->op.u2.sp.old_ptrclass = op2_ptrclass;
        newparm->op.u2.sp.new_ptrclass = op1_ptrclass;
    }
    return( newparm );
}

static TREEPTR ParmNode( FrontRun& front, TREEPTR parmlist, TREEPTR newparm, bool far16_func )
{
    TREEPTR     tree;
    TYPEPTR     parmtyp;

    if( far16_func ) {
        newparm = FarPtr16Cvt( front, newparm );
    }
    tree = ExprNode( front, parmlist, OPR_PARM, newparm );
    if( far16_func ) {
        parmtyp = Far16Type( front, newparm->u.expr_type );
    } else {
        parmtyp = newparm->u.expr_type;
    }
    tree->u.expr_type = parmtyp;
    tree->op.u2.result_type = parmtyp;
    return( tree );
}

TREEPTR ErrorNode( FrontRun& front, TREEPTR tree )
{
    FreeExprTree( front, tree );
    tree = LeafNode( front, OPR_ERROR );
    tree->u.expr_type = GetType( front, TYP_INT );
    return( tree );
}

op_flags OpFlags( type_modifiers flags )
{
    op_flags      ops;

    ops = OPFLAG_NONE;
    if( flags & FLAG_CONST )
        ops |= OPFLAG_CONST;
    if( flags & FLAG_VOLATILE )
        ops |= OPFLAG_VOLATILE;
    if( flags & FLAG_UNALIGNED )
        ops |= OPFLAG_UNALIGNED;
#if _INTEL_CPU
    if( flags & FLAG_NEAR )
        ops |= OPFLAG_NEARPTR;
    if( flags & FLAG_FAR )
        ops |= OPFLAG_FARPTR;
    if( flags & FLAG_HUGE )
        ops |= OPFLAG_HUGEPTR;
    if( flags & FLAG_FAR16 )
        ops |= OPFLAG_FAR16PTR;
#endif
    return( ops );
}

type_modifiers FlagOps( op_flags ops )
{
    type_modifiers      flags;

    flags = FLAG_NONE;
    if( ops & OPFLAG_CONST )
        flags |= FLAG_CONST;
    if( ops & OPFLAG_VOLATILE )
        flags |= FLAG_VOLATILE;
    if( ops & OPFLAG_UNALIGNED )
        flags|= FLAG_UNALIGNED;
#if _INTEL_CPU
    if( ops & OPFLAG_NEARPTR )
        flags |= FLAG_NEAR;
    if( ops & OPFLAG_FARPTR )
        flags |= FLAG_FAR;
    if( ops & OPFLAG_HUGEPTR )
        flags |= FLAG_HUGE;
    if( Far16Pointer(ops) )
        flags |= FLAG_FAR16;
#endif
    return( flags );
}

static TREEPTR ConstLeaf( FrontRun& front )
{
    TREEPTR     leaf;
    FLOATVAL    *flt;

    /*
     * assume integer value
     */
    leaf = LeafNode( front, OPR_PUSHINT );
    leaf->op.u1.const_type = front.cvars.ConstType;
    switch( front.cvars.ConstType ) {
    case TYP_WCHAR:
#if TARGET_WCHAR == TARGET_UINT
        leaf->op.u1.const_type = TYP_UINT;
#else
        leaf->op.u1.const_type = TYP_USHORT;
#endif
        leaf->op.u2.long_value = U32FetchTrunc( front.cvars.Constant64 );
        break;
    case TYP_CHAR:
    case TYP_UCHAR:
        leaf->op.u1.const_type = TYP_INT;
        /* fall through */
    case TYP_INT:
        leaf->op.u2.long_value = I32FetchTrunc( front.cvars.Constant64 );
        break;
    case TYP_UINT:
        leaf->op.u2.ulong_value = U32FetchTrunc( front.cvars.Constant64 );
        break;
    case TYP_LONG:
        leaf->op.u2.long_value = I32FetchTrunc( front.cvars.Constant64 );
        break;
    case TYP_ULONG:
        leaf->op.u2.ulong_value = U32FetchTrunc( front.cvars.Constant64 );
        break;
    case TYP_LONG64:
        leaf->op.u2.long64_value = front.cvars.Constant64;
        break;
    case TYP_ULONG64:
        leaf->op.u2.ulong64_value = front.cvars.Constant64;
        break;
    case TYP_FLOAT:
    case TYP_DOUBLE:
    case TYP_LONG_DOUBLE:
    case TYP_FIMAGINARY:
    case TYP_DIMAGINARY:
    case TYP_LDIMAGINARY:
        flt = static_cast<FLOATVAL *>(CMemAlloc( front, sizeof( FLOATVAL ) - 1 + 1 + front.cvars.TokenLen + 1 ));
        flt->string[0] = '+';
        strcpy( flt->string + 1, front.cvars.Buffer );
        flt->len = (unsigned char)( 1 + front.cvars.TokenLen );
        flt->type = front.cvars.ConstType;
        flt->next = NULL;
        leaf->op.u2.float_value = flt;
        leaf->op.opr = OPR_PUSHFLOAT;
        /*
         * Convert constant to binary; that ensures it'll be cast properly.
         */
        MakeBinaryFloat( front, leaf );
        break;
    }
    leaf->u.expr_type = GetType( front, leaf->op.u1.const_type );
    return( leaf );
}

TREEPTR IntLeaf( FrontRun& front, target_int value )
{
    TREEPTR     leaf;

    leaf = LeafNode( front, OPR_PUSHINT );
    leaf->op.u1.const_type = TYP_INT;
    leaf->op.u2.long_value = value;
    leaf->u.expr_type = GetType( front, TYP_INT );
    return( leaf );
}

TREEPTR UIntLeaf( FrontRun& front, target_uint value )
{
    TREEPTR     leaf;

    leaf = LeafNode( front, OPR_PUSHINT );
    leaf->op.u1.const_type = TYP_UINT;
    leaf->op.u2.ulong_value = value;
    leaf->u.expr_type = GetType( front, TYP_UINT );
    return( leaf );
}

TREEPTR LongLeaf( FrontRun& front, target_long value )
{
    TREEPTR     leaf;

    leaf = LeafNode( front, OPR_PUSHINT );
    leaf->op.u1.const_type = TYP_LONG;
    leaf->op.u2.long_value = value;
    leaf->u.expr_type = GetType( front, TYP_LONG );
    return( leaf );
}

static TREEPTR EnumLeaf( FrontRun& front, ENUMPTR ep )
{
    DATA_TYPE   decl_type;
    TREEPTR     leaf;

    leaf = LeafNode( front, OPR_PUSHINT );
    decl_type = ep->parent->sym_type->object->decl_type;
    switch( decl_type ) {
    case TYP_CHAR:
    case TYP_SHORT:
        decl_type = TYP_INT;
        /* fall through */
    case TYP_INT:
    case TYP_LONG:
        leaf->op.u2.long_value = (signed_32)U64Low( ep->value );
        break;
    case TYP_UCHAR:
    case TYP_USHORT:
        decl_type = TYP_INT;
        /* fall through */
    case TYP_UINT:
    case TYP_ULONG:
        leaf->op.u2.long_value = U64Low( ep->value );
        break;
    case TYP_LONG64:
    case TYP_ULONG64:
        leaf->op.u2.long64_value = ep->value;
        break;
    }
    leaf->op.u1.const_type = decl_type;
    leaf->u.expr_type = GetType( front, decl_type );
    return( leaf );
}

TREEPTR VarLeaf( FrontRun& front, SYMPTR sym, SYM_HANDLE sym_handle )
{
    TREEPTR         leaf;
    TYPEPTR         typ;
    type_modifiers  flags;

    leaf = LeafNode( front, OPR_PUSHADDR );
    leaf->op.u2.sym_handle = sym_handle;
    typ = sym->sym_type;
    leaf->u.expr_type = typ;
    flags = sym->mods & MASK_PTR;
    leaf->op.flags = OpFlags( flags );
    return( leaf );
}

static TREEPTR CheckSym( FrontRun& front, id_hash_idx hash, const char *id,
                        SYM_ENTRY *sym_out, SYM_HANDLE *symhandle )
{
    SYM_HANDLE  sym_handle;
    SYM_HANDLE  sym_handle0;
    ENUMPTR     ep;
    SYM_ENTRY   sym;

    ep = EnumLookup( front, hash, id );
    sym_handle = SymLook( front, hash, id );
    if( sym_handle == SYM_NULL ) {
        if( ep != NULL )
            return( EnumLeaf( front, ep ) );
        SymCreate( front, &sym, id );
    } else {
        SymGet( front, &sym, sym_handle );
        if( ep != NULL
          && ep->parent->level > sym.level ) {
            return( EnumLeaf( front, ep ) );
        }
        if( sym.attribs.stg_class == SC_EXTERN
          && sym.level > 0 ) {
            sym_handle0 = Sym0Look( front, hash, id );
            if( sym_handle0 != SYM_NULL ) {
                SymGet( front, &sym, sym_handle0 );
                sym_handle = sym_handle0;
            }
        }

        if( sym.attribs.stg_class == SC_TYPEDEF ) {
            CErr2p( front, ERR_CANT_USE_TYPEDEF_AS_VAR, id );
            return( IntLeaf( front, 0 ) );
        }
    }

    *symhandle = sym_handle;
    memcpy( sym_out, &sym, sizeof( sym ) );
    return( NULL );
}

static TREEPTR SymLeaf( FrontRun& front )
{
    SYM_HANDLE  sym_handle;
    id_hash_idx hash;
    TREEPTR     tree;
    SYM_ENTRY   sym;

    if( front.cvars.CurToken == T_SAVED_ID ) {
        hash = CalcHashID( front.cvars.SavedId );
        front.cvars.CurToken = front.cvars.LAToken;
        tree = CheckSym( front, hash, front.cvars.SavedId, &sym, &sym_handle );
    } else {
        hash = CalcHashID( front.cvars.Buffer );
        tree = CheckSym( front, hash, front.cvars.Buffer, &sym, &sym_handle );
        NextToken(front);
    }

    if( tree == NULL ) {
        /*
         * if( SizeOfCount == 0 )  causes defined but not referenced
         * always turning it on can cause referenced but not assigned
         * for the case:  int i;  j = sizeof(i);
         */
        sym.flags |= SYM_REFERENCED;
        if( sym_handle == SYM_NULL ) {
            if( front.cvars.CurToken == T_LEFT_PAREN ) {
                sym.attribs.stg_class = SC_FORWARD;     /* indicate forward decl */
                /*
                 * Warn about unprototyped function
                 */
                CWarn2p( front, ERR_ASSUMED_IMPORT, sym.name );
                sym_handle = SymAddL0( front, hash, &sym ); /* add symbol to level 0 */
                sym.flags |= SYM_FUNCTION;
                sym.sym_type = FuncNode( front, GetType( front, TYP_INT ), FLAG_NONE, NULL );
            } else {
                sym.attribs.stg_class = SC_EXTERN;      /* indicate extern decl */
                CErr2p( front, ERR_UNDECLARED_SYM, sym.name );
                sym_handle = SymAdd( front, hash, &sym ); /* add sym to current level*/
                sym.sym_type = GetType( front, TYP_INT );
            }
        }
        IncSymWeight( front, &sym );
        tree = VarLeaf( front, &sym, sym_handle );
        SymReplace( front, &sym, sym_handle );
    }
    return( tree );
}


static void IncSymWeight( FrontRun& front, SYMPTR sym )
{
    static int  const LoopWeights[] = { 1, 0x10, 0x100, 0x1000 };

    if( sym->level != 0 ) {
        if( (sym->flags & SYM_FUNCTION) == 0 ) {
            if( front.cvars.LoopDepth > 3 ) {
                sym->u.var.offset = ~0U >> 1;
            } else {
                sym->u.var.offset += LoopWeights[front.cvars.LoopDepth];
            }
        }
    }
}

//-----------------------------CMATH------------------------------------

static bool IsStruct( TYPEPTR typ )
/*********************************/
{
    SKIP_TYPEDEFS( typ );
    return( typ->decl_type == TYP_STRUCT || typ->decl_type == TYP_UNION );
}

bool IsLValue( TREEPTR tree )
{
    switch( tree->op.opr ) {
    case OPR_ERROR:
    case OPR_ARROW:
    case OPR_INDEX:
    case OPR_PUSHADDR:
    case OPR_POINTS:
        return( true );
    case OPR_DOT:
        while( tree->op.opr == OPR_DOT )
            tree = tree->left;
        switch( tree->op.opr ) {
        case OPR_CALL:
        case OPR_QUESTION:
        case OPR_COMMA:
            /*
             * These are not lvalues
             */
            break;
        default:
            return( true );
            break;
        }
        break;
    }
    return( false );
}

static bool IsCallValue( TREEPTR tree )
{
    while( tree->op.opr == OPR_DOT || tree->op.opr == OPR_INDEX ) {
        tree = tree->left;
    }
    return( tree->op.opr == OPR_CALL );
}

static TREEPTR TakeRValue( FrontRun& front, TREEPTR tree, int void_ok )
/*****************************************************
 * This RVALUE thing is backwards
 */
{
    TYPEPTR             typ;
    sym_flags           symb_flags;
    type_modifiers      decl_flags;
    target_uint         value;
    SYM_ENTRY           sym;
    segment_id          segid;

    if( tree->op.opr == OPR_ERROR )
        return( tree );
    if( !IS_PPCTL_NORMAL() ) {
        if( tree->op.opr == OPR_PUSHFLOAT ) {
            CErr1( front, ERR_EXPR_MUST_BE_INTEGRAL );
            return( ErrorNode( front, tree ) );
        }
    }
    typ = tree->u.expr_type;
    SKIP_TYPEDEFS( typ );
    if( typ->decl_type == TYP_VOID ) {
        if( ! void_ok ) {
            CErr1( front, ERR_EXPR_HAS_VOID_TYPE );
            return( ErrorNode( front, tree ) );
        }
    } else if( typ->decl_type == TYP_ARRAY ) {
        if( tree->op.opr == OPR_PUSHSTRING ) {
            if( typ->object->decl_type == TYP_USHORT ) {
                typ = PtrNode( front, typ->object, FLAG_NONE, SEG_DATA );
            } else {
                front.cvars.StringArrayType = typ;
                typ = front.cvars.StringType;
            }
            tree->u.expr_type = typ;
        } else {
            segid = SEG_DATA;
            decl_flags = FlagOps( tree->op.flags );
            if( tree->op.opr == OPR_PUSHADDR ) {
                SymGet( front, &sym, tree->op.u2.sym_handle );
                symb_flags = sym.flags;
                sym.flags |= SYM_REFERENCED | SYM_ASSIGNED;
                if( symb_flags != sym.flags ) {
                    SymReplace( front, &sym, tree->op.u2.sym_handle );
                }
                if( sym.attribs.stg_class == SC_AUTO ) {
                    segid = SEG_STACK;
                }
            }
            if( IsCallValue( tree ) ) {
                CErr1( front, ERR_CANT_TAKE_ADDR_OF_RVALUE );
            }
            tree = ExprNode( front, NULL, OPR_ADDROF, tree );
            if( segid == SEG_STACK ) {
                front.cvars.CompFlags.addr_of_auto_taken = true;
#if _INTEL_CPU
                if( front.cvars.TargetSwitches & CGSW_X86_FLOATING_SS ) {
                    decl_flags = (decl_flags & ~FLAG_NEAR) | FLAG_FAR;
                }
#endif
            }
            tree->u.expr_type = PtrNode( front, typ->object, decl_flags, segid );
        }
    } else if( typ->decl_type == TYP_FUNCTION ) {

        if( tree->op.opr == OPR_PUSHADDR ) {
            SymGet( front, &sym, tree->op.u2.sym_handle );
            decl_flags = sym.mods;
            symb_flags = sym.flags;
            sym.flags |= SYM_REFERENCED | SYM_ADDR_TAKEN;
            if( symb_flags != sym.flags ) {
                SymReplace( front, &sym, tree->op.u2.sym_handle );
            }
        } else if( tree->op.opr == OPR_POINTS ) {
            decl_flags = tree->op.u2.result_type->u.p.decl_flags;
        } else {
            decl_flags = FLAG_NONE;
        }
        tree = ExprNode( front, NULL, OPR_ADDROF, tree );
        tree->u.expr_type = PtrNode( front, typ, decl_flags, SEG_NULL );
    } else if( TypeSize( front, typ ) == 0 ) {
        SetDiagType1( front, typ );
        CErr1( front, ERR_INCOMPLETE_EXPR_TYPE );
        SetDiagPop(front);
        return( ErrorNode( front, tree ) );
    } else {
        if( front.cvars.SizeOfCount == 0 ) {
            if( tree->op.flags & OPFLAG_VOLATILE ) {
                front.cvars.CompFlags.useful_side_effect = true;
            }
        }
        if( tree->op.opr == OPR_PUSHSYM
          || tree->op.opr == OPR_PUSHADDR ) {
            SymGet( front, &sym, tree->op.u2.sym_handle );
            symb_flags = sym.flags;
            sym.flags |= SYM_REFERENCED;
            if( !front.cvars.CompFlags.label_dropped
              && front.cvars.SizeOfCount == 0 ) {
                if( sym.level != 0
                  && sym.attribs.stg_class != SC_STATIC
                  && sym.attribs.stg_class != SC_EXTERN ) {
                    if( (sym.flags & SYM_ASSIGNED) == 0 ) {
                        /*
                         * turn on flag so msg only comes out once per sym
                         */
                        sym.flags |= SYM_ASSIGNED;
                        CWarn2p( front, ERR_SYM_NOT_ASSIGNED, SymName( front, &sym, tree->op.u2.sym_handle ) );
                    }
                }
            }
            if( symb_flags != sym.flags ) {
                SymReplace( front, &sym, tree->op.u2.sym_handle );
            }
        }
    }
    switch( tree->op.opr ) {
    case OPR_PUSHADDR:
        tree->op.opr = OPR_PUSHSYM;
        break;
    case OPR_QUESTION:
    case OPR_COMMA:
    case OPR_EQUALS:
        if( IsStruct( tree->u.expr_type ) ) {
            tree->op.flags |= OPFLAG_RVALUE;
        }
        break;
    case OPR_DOT:                       /* sym.field */
    case OPR_ARROW:                     /* ptr->field */
    case OPR_INDEX:                     /* array[index] */
        tree->op.flags |= OPFLAG_RVALUE;
        break;
    case OPR_CALL:                      /* func() */
        tree->op.flags |= OPFLAG_RVALUE;
        if( tree->left->op.opr == OPR_FUNCNAME ) {
            SymGet( front, &sym, tree->left->op.u2.sym_handle );
            if( sym.attribs.stg_class == SC_FORWARD
              && (sym.flags & SYM_TYPE_GIVEN) == 0 ) {
                 sym.flags |= SYM_TYPE_GIVEN;
                 SymReplace( front, &sym, tree->left->op.u2.sym_handle );
            }
        }
        break;
    case OPR_POINTS:
        if( tree->left->op.opr == OPR_PUSHSTRING ) {    /* *"abc" */
            STR_HANDLE  string;

            string = tree->left->op.u2.string_handle;
            value = string->literal[0];
            typ = tree->left->u.expr_type;
            if( typ->object->decl_type == TYP_USHORT ) {
                value = (string->literal[1] << 8) | value;
            }
            FreeExprTree( front, tree );
            tree = IntLeaf( front, value );
        } else {
            tree->op.flags |= OPFLAG_RVALUE;
        }
        break;
    }
    return( tree );
}


static TREEPTR VoidRValue( FrontRun& front, TREEPTR tree )
{
    tree = TakeRValue( front, tree, 1 );
    return( tree );
}

TREEPTR RValue( FrontRun& front, TREEPTR tree )
{
    tree = TakeRValue( front, tree, 0 );
    return( tree );
}

#if 0
static void CheckAddrOfArray( TYPEPTR typ )
{
    if( typ != NULL ) {
        SKIP_TYPEDEFS( typ );
        if( typ->decl_type == TYP_ARRAY ) {
            CWarn1( ERR_ADDR_OF_ARRAY );
        }
    }
}
#endif

//-----------------------------COPS------------------------------------

static TREEPTR AddrOp( FrontRun& front, TREEPTR tree )
{
    TYPEPTR         typ;
    TREEPTR         leaf;
    type_modifiers  modifiers;
    segment_id      segid;
//    SYM_HANDLE      based_sym;
    SYM_ENTRY       sym;

    if( tree->op.opr == OPR_ERROR )
        return( tree );
    typ = tree->u.expr_type;
    if( typ->decl_type == TYP_FIELD
      || typ->decl_type == TYP_UFIELD ) {
        CErr1( front, ERR_CANT_TAKE_ADDR_OF_BIT_FIELD );
        return( ErrorNode( front, tree ) );
    } else if( typ->decl_type == TYP_VOID ) {
        CErr1( front, ERR_CANT_TAKE_ADDR_OF_RVALUE );
        return( ErrorNode( front, tree ) );
    }
    if( tree->op.flags & OPFLAG_LVALUE_CAST ) {
        if( front.cvars.CompFlags.extensions_enabled ) {
            tree->op.flags &= ~(OPFLAG_LVALUE_CAST | OPFLAG_RVALUE);
            CWarn1( front, ERR_LVALUE_CAST );
        } else {
            CErr1( front, ERR_CANT_TAKE_ADDR_OF_RVALUE );
            return( ErrorNode( front, tree ) );
        }
    }
    if( (tree->op.opr == OPR_CONVERT
      || tree->op.opr == OPR_CONVERT_PTR)
      && front.cvars.CompFlags.extensions_enabled ) {
        tree = LCastAdj( front, tree );
    }
    leaf = tree;
    if( tree->op.opr == OPR_DOT ) {
        if( tree->left->op.opr == OPR_PUSHADDR ) {
            leaf = tree->left;
        }
    } else if( tree->op.opr == OPR_ARROW ) {
        /*
         * checking for offsetof macro construct
         * #define offsetof(typ,field) (size_t)&(((typ*)0)->field)
         */
        if( tree->left->op.opr == OPR_PUSHINT ) {
            leaf = tree->left;
            leaf->op.u2.ulong_value += tree->right->op.u2.ulong_value;
            tree->left = NULL;
            FreeExprTree( front, tree );
            leaf->u.expr_type = PtrNode( front, typ, FLAG_NONE, SEG_DATA );
            return( leaf );
        }
    }
//    based_sym = SYM_NULL;
    modifiers = FLAG_NONE;
    segid = SEG_DATA;
    if( leaf->op.opr == OPR_PUSHADDR ) {
        SymGet( front, &sym, leaf->op.u2.sym_handle );
        if( sym.attribs.stg_class == SC_REGISTER ) {
            CErr1( front, ERR_CANT_TAKE_ADDR_OF_REGISTER );
        }
        if( sym.level != 0 ) {
            sym.flags |= SYM_ASSIGNED;
        }
        sym.flags |= SYM_ADDR_TAKEN | SYM_REFERENCED;
        SymReplace( front, &sym, leaf->op.u2.sym_handle );
    }
    if( leaf->op.opr == OPR_PUSHADDR ) {
//        CheckAddrOfArray( typ );
        SymGet( front, &sym, leaf->op.u2.sym_handle );
        modifiers = sym.mods;
        if( sym.attribs.stg_class == SC_AUTO ) {
            segid = SEG_STACK;
            front.cvars.CompFlags.addr_of_auto_taken = true;
#if _INTEL_CPU
            if( front.cvars.TargetSwitches & CGSW_X86_FLOATING_SS ) {
                modifiers |= FLAG_FAR;
            }
#endif
        }
        typ = PtrNode( front, typ, modifiers & MASK_PTR, segid );
    } else if( tree->op.opr == OPR_POINTS ) {
        typ = tree->op.u2.result_type;
    } else {
        modifiers = FlagOps( tree->op.flags );
        typ = PtrNode( front, typ, modifiers, SEG_DATA );
    }
    if( IsLValue( tree ) ) {
        tree = ExprNode( front, NULL, OPR_ADDROF, tree );
    } else {
        CErr1( front, ERR_CANT_TAKE_ADDR_OF_RVALUE );
        return( ErrorNode( front, tree ) );
    }
    tree->u.expr_type = typ;
    return( tree );
}

static TREEPTR FarPtrCvt( FrontRun& front, SYMPTR sym, SYM_HANDLE sym_handle )
{
    TYPEPTR         typ;
    TREEPTR         tree;

    typ = sym->sym_type;
    SKIP_TYPEDEFS( typ );
    if( typ->decl_type == TYP_POINTER ) {
        if( typ->u.p.decl_flags & FLAG_FAR ) {
            tree = VarLeaf( front, sym, sym_handle );
            tree->op.opr = OPR_PUSHSYM;
        } else if( typ->u.p.decl_flags & FLAG_BASED ) {
            tree = VarLeaf( front, sym, sym_handle );
            tree->op.opr = OPR_PUSHSYM;
            tree = BasedPtrNode( front, typ, tree );
        } else {
            tree = VarLeaf( front, sym, sym_handle );
            tree->op.opr = OPR_PUSHSYM;
            tree = ExprNode( front, NULL, OPR_CONVERT, tree );
            tree->u.expr_type = typ;
            tree->op.u2.result_type = PtrNode( front, typ->object, FLAG_FAR, SEG_NULL );
        }
    } else {
        assert( 0 );
        tree = NULL;
    }
    return( tree );
}

static TREEPTR MakeFarOp( FrontRun& front, TREEPTR based_sym, TREEPTR tree )
{
    TYPEPTR     typ;

    typ = tree->u.expr_type;
    SKIP_TYPEDEFS( typ );
    typ = PtrNode( front, typ->object, FLAG_FAR, SEG_NULL );
    tree = ExprNode( front, based_sym, OPR_FARPTR, tree );
    tree->u.expr_type = typ;
    return( tree );
}

TREEPTR BasedPtrNode( FrontRun& front, TYPEPTR ptrtyp, TREEPTR tree )
{
    TREEPTR     based_sym;
    SYM_HANDLE  sym_handle;
    SYMPTR      sym;
    segment_id  segid;

    sym_handle = ptrtyp->u.p.based_sym;
    segid =  ptrtyp->u.p.segid;
    switch( ptrtyp->u.p.based_kind ) {
    case BASED_VOID:
        /*
         * __based( void )       segment:>offset base op
         */
        break;
    case BASED_NONE:
        break;
    case BASED_SELFSEG:
        /*
         * __based( (__segment) __self ) use seg of self
         */
        tree->op.flags &= ~OPFLAG_RVALUE;
        based_sym = ExprNode( front, tree, OPR_DUPE, NULL );
        based_sym->op.flags |= OPFLAG_RVALUE;
        based_sym->op.u2.result_type = tree->op.u2.result_type;
        tree = MakeFarOp( front, based_sym, tree );
        tree->right = NULL;
        break;
    case BASED_VAR:
        /*
         * __based( <var> )             add offset to var pointer
         */
      {
        TYPEPTR     typ;
        TYPEPTR     old;

        old = tree->u.expr_type;
        SKIP_TYPEDEFS( old );
        sym = SymGetPtr( front, sym_handle );
        based_sym = VarLeaf( front, sym, sym_handle );
        based_sym->op.opr = OPR_PUSHSYM;
        typ = sym->sym_type;
        SKIP_TYPEDEFS( typ );
        if( typ->u.p.decl_flags & FLAG_BASED ) {
            based_sym = BasedPtrNode( front, typ, based_sym );
            typ = based_sym->u.expr_type;
            SKIP_TYPEDEFS( typ );
        }
        old = PtrNode( front, old->object, typ->u.p.decl_flags, SEG_NULL );
        tree = ExprNode( front, based_sym, OPR_ADD, tree );
        tree->u.expr_type = old;
        tree->op.u2.result_type = old;
      } break;
    case BASED_SEGVAR:
        /*
         * __based( <var> )             add offset to seg var
         */
        sym = SymGetPtr( front, sym_handle );
        based_sym = VarLeaf( front, sym, sym_handle );
        based_sym->op.opr = OPR_PUSHSYM;
        tree = MakeFarOp( front, based_sym, tree );
        break;
    case BASED_VARSEG:
        /*
         * __based( (__segment) <var> ) ) use seg of var
         */
        sym = SymGetPtr( front, sym_handle );
        based_sym = FarPtrCvt( front, sym, sym_handle );
        tree = MakeFarOp( front, based_sym, tree );
        break;
    case BASED_SEGNAME:
        /*
         * __based( __segname( "name" )   use seg of segname
         */
        if( sym_handle != SYM_NULL ) {
            sym = SymGetPtr( front, sym_handle );
            based_sym = VarLeaf( front, sym, sym_handle );
            based_sym->op.opr = OPR_PUSHSEG;
        } else {
            /*
             * error I guess
             */
            based_sym = LeafNode( front, OPR_PUSHINT );
            based_sym->op.u1.const_type = TYP_INT;
            based_sym->op.u2.long_value = segid;
            based_sym->u.expr_type = GetType( front, TYP_USHORT );
        }
        tree = MakeFarOp( front, based_sym, tree );
    }
    return( tree  );
}


static TREEPTR CheckBasedPtr( FrontRun& front, TREEPTR tree, TYPEPTR typ, type_modifiers *p_flags )
{
    type_modifiers  flags;

    flags = typ->u.p.decl_flags & MASK_PTR;
    if( flags & FLAG_BASED ) {
        tree = BasedPtrNode( front, typ, tree );
        flags &= ~(FLAG_NEAR | FLAG_BASED);
        if( typ->u.p.based_kind == BASED_VAR ) {
            SYM_HANDLE  base_sym_handle;
            SYMPTR      base_sym;
            TYPEPTR     base_typ;

            /*
             * For pointers based on another pointer, copy flags from the base ptr
             */
            base_sym_handle = typ->u.p.based_sym;
            base_sym = SymGetPtr( front, base_sym_handle );
            base_typ = base_sym->sym_type;
            flags |= base_typ->u.p.decl_flags & MASK_PTR;
        } else {
            flags |= FLAG_FAR;
        }
    }
    *p_flags = flags;
    return( tree );
}


static TREEPTR PtrOp( FrontRun& front, TREEPTR tree )
{
    TYPEPTR         ptrtyp;
    TYPEPTR         typ;
    type_modifiers  flags;

    if( tree->op.opr == OPR_ERROR )
        return( tree );
    typ = tree->u.expr_type;
    SKIP_TYPEDEFS( typ );
    if( typ->decl_type != TYP_POINTER ) {
        CErr1( front, ERR_EXPR_MUST_BE_POINTER_TO );
        return( ErrorNode( front, tree ) );
    }
    ptrtyp = typ;
    tree = CheckBasedPtr( front, tree, typ, &flags );
    tree = ExprNode( front, tree, OPR_POINTS, NULL );
    typ = typ->object;
//    SKIP_TYPEDEFS( typ );
    tree->u.expr_type = typ;
    tree->op.u2.result_type = ptrtyp;
    tree->op.flags = OpFlags( flags );
    if( front.cvars.SizeOfCount == 0 ) {
        if( tree->op.flags & OPFLAG_VOLATILE ) {
            front.cvars.CompFlags.useful_side_effect = true;
        }
    }
    return( tree );
}


FIELDPTR SearchFields( TYPEPTR *class_typ, target_size *field_offset, const char *name )
{
    FIELDPTR    field;
    FIELDPTR    subfield;
    TYPEPTR     typ;
    TAGPTR      tag;

    tag = (*class_typ)->u.tag;
    for( field = tag->u.field_list; field != NULL; field = field->next_field ) {
        typ = field->field_type;
        SKIP_TYPEDEFS( typ );
        if( field->name[0] == '\0' ) {  /* if nameless field */
            if( typ->decl_type == TYP_STRUCT
              || typ->decl_type == TYP_UNION ) {
                subfield = SearchFields( &typ, field_offset, name );
                if( subfield != NULL ) {
                    *field_offset += field->offset;
                    *class_typ = typ;
                    return( subfield );
                }
            }
        } else if( strcmp( name, field->name ) == 0 ) {
            if( typ->decl_type != TYP_FUNCTION ) {
                *field_offset += field->offset;
            }
            return( field );
        }
    }
    return( NULL );
}


static TYPEPTR Ptr2Struct( TYPEPTR typ )
{
    if( typ->decl_type != TYP_POINTER ) {
        return( NULL );
    }
    typ = typ->object;
    SKIP_TYPEDEFS( typ );
    if( typ->decl_type == TYP_STRUCT
      || typ->decl_type == TYP_UNION ) {
        return( typ );
    }
    return( NULL );
}


static TREEPTR DotOp( FrontRun& front, TREEPTR tree )
{
    TYPEPTR             typ;
    TYPEPTR             get_typ;
    FIELDPTR            field;
    target_size         offset;
    op_flags            opflag;
    SYM_ENTRY           sym;

    if( front.cvars.CurToken != T_ID ) {
        CErr1( front, ERR_EXPECTING_ID );
        return( ErrorNode( front, tree ) );
    }
    if( tree->op.opr == OPR_ERROR )
        return( tree );
    typ = tree->u.expr_type;
    SKIP_TYPEDEFS( typ );
    if( typ->decl_type != TYP_STRUCT
      && typ->decl_type != TYP_UNION ) {
        CErr1( front, ERR_MUST_BE_STRUCT_OR_UNION );
        return( ErrorNode( front, tree ) );
    }
    get_typ = typ;
    offset = 0;
    field = SearchFields( &get_typ, &offset, front.cvars.Buffer );
    if( field == NULL ) {
        CErr3p( front, ERR_NAME_NOT_FOUND_IN_STRUCT, front.cvars.Buffer, typ->u.tag->name );
        return( ErrorNode( front, tree ) );
    }
    typ = field->field_type;
    opflag = tree->op.flags | OpFlags( field->attrib );
    if( front.cvars.CompFlags.emit_browser_info ) {
        field->xref->next_xref = NewXref( front, field->xref->next_xref );
    }
    if( tree->op.opr == OPR_DOT
      || tree->op.opr == OPR_ARROW ) {
        tree->right->op.u2.ulong_value += offset;
    } else {
        if( tree->op.opr == OPR_PUSHADDR ) {
            SymGet( front, &sym, tree->op.u2.sym_handle );
            if( (sym.flags & SYM_ASSIGNED) == 0 ) {
                sym.flags |= SYM_ASSIGNED;
                SymReplace( front, &sym, tree->op.u2.sym_handle );
            }
        }
        tree = ExprNode( front, tree, OPR_DOT, UIntLeaf( front, offset ) );
    }
    tree->u.expr_type = typ;
    tree->op.u2.result_type = typ;
    tree->op.flags = opflag;
    return( tree );
}


static TREEPTR ArrowOp( FrontRun& front, TREEPTR tree )
{
    TYPEPTR             typ;
    TYPEPTR             get_typ;
    FIELDPTR            field;
    target_size         offset;
    type_modifiers      flags;

    if( front.cvars.CurToken != T_ID ) {
        CErr1( front, ERR_EXPECTING_ID );
        return( ErrorNode( front, tree ) );
    }
    if( tree->op.opr == OPR_ERROR )
        return( tree );
    typ = tree->u.expr_type;
    SKIP_TYPEDEFS( typ );
    if( Ptr2Struct( typ ) == NULL ) {
        CErr1( front, ERR_MUST_BE_PTR_TO_STRUCT_OR_UNION );
        return( ErrorNode( front, tree ) );
    }
    if( tree->op.opr == OPR_PUSHADDR ) {
        tree->op.opr = OPR_PUSHSYM;
    }
    tree = CheckBasedPtr( front, tree, typ, &flags );
    typ = typ->object;
    SKIP_TYPEDEFS( typ );
    get_typ = typ;
    offset = 0;
    field = SearchFields( &get_typ, &offset, front.cvars.Buffer );
    if( field == NULL ) {
        CErr3p( front, ERR_NAME_NOT_FOUND_IN_STRUCT, front.cvars.Buffer, typ->u.tag->name );
        return( ErrorNode( front, tree ) );
    }
    if( front.cvars.CompFlags.emit_browser_info ) {
        field->xref->next_xref = NewXref( front, field->xref->next_xref );
    }
    tree = ExprNode( front, tree, OPR_ARROW, UIntLeaf( front, offset ) );
    typ = field->field_type;
    tree->u.expr_type = typ;
    tree->op.u2.result_type = typ;
    flags |= field->attrib;
    tree->op.flags |= OpFlags( flags );
    return( tree );
}

static TREEPTR IncDec( FrontRun& front, TREEPTR tree, TOKEN opr )
{
    op_flags    volatile_flag;

    volatile_flag = tree->op.flags & OPFLAG_VOLATILE;
    CheckConst( front, tree );
    tree = AddOp( front, tree, opr, IntLeaf( front, 1 ) );
    tree->op.flags |= volatile_flag;
    front.cvars.CompFlags.meaningless_stmt = false;
    front.cvars.CompFlags.useful_side_effect = true;
    return( tree );
}

//-----------------------------CEXPR-----------------------------------

bool ConstExprAndType( FrontRun& front, const_val *val )
{
    TREEPTR     tree;
    call_list   **save;
    bool        ret;

    save = front.cexpr.FirstCallLnk;
    ++front.cvars.ExprLevel;                    /* allow for nested expressions */
    tree = SingleExpr(front);
    --front.cvars.ExprLevel;                    /* put Level back */
    front.cexpr.FirstCallLnk = save;
    val->type = TYP_INT;
    switch( tree->op.opr ) {
    case OPR_PUSHINT:
        val->type = tree->op.u1.const_type;
        switch( tree->op.u1.const_type ) {
        case TYP_LONG64:
        case TYP_ULONG64:
            val->value = tree->op.u2.long64_value;
            break;
        case TYP_ULONG:
        case TYP_UINT:
            Set64ValU32( val->value, tree->op.u2.long_value );
            break;
        default:
            Set64ValI32( val->value, tree->op.u2.long_value );
            break;
        }
        ret = true;
        break;
    case OPR_PUSHFLOAT:
        CErr1( front, ERR_EXPR_MUST_BE_INTEGRAL );
        val->value.u._64[0] = (long long)atof( tree->op.u2.float_value->string );
        ret = false;
        break;
    default:
        if( tree->op.opr != OPR_ERROR ) {
            CErr1( front, ERR_NOT_A_CONSTANT_EXPR );
        }
        Set64ValZero( val->value );
        ret = false;
        break;
    }
    FreeExprTree( front, tree );
    return( ret );
}

TREEPTR Expr( FrontRun& front )
{
    front.cvars.Class[front.cvars.ExprLevel] = TC_START;
    return( GetExpr(front) );
}


TREEPTR CommaExpr( FrontRun& front )
{
    front.cvars.Class[front.cvars.ExprLevel] = TC_START1;
    return( GetExpr(front) );
}


TREEPTR AddrExpr( FrontRun& front )
{
    return( SingleExpr(front) );
}


TREEPTR SingleExpr( FrontRun& front )
{
    TREEPTR     tree;

    front.cvars.Class[front.cvars.ExprLevel] = TC_START2;
    tree = GetExpr(front);
    FoldExprTree( front, tree );
    return( tree );
}


static TREEPTR GetExpr( FrontRun& front )
{
    TREEPTR     tree, op1;
    TYPEPTR     typ;
    token_class curclass;
    TYPEPTR     *plist;

    front.cvars.CompFlags.useful_side_effect = false;
    front.cexpr.FirstCallLnk = front.cexpr.LastCallLnk;
    tree = NULL;
    plist = NULL;
    for( ;; ) {
        if( tree == NULL )
            tree = ExprOpnd(front);
        curclass = TokenClass[front.cvars.CurToken];
        while( curclass <= front.cvars.Class[front.cvars.ExprLevel] ) {
            op1 = front.cvars.ValueStack[front.cvars.ExprLevel];
            /*
             * the following cases are listed from lowest to highest
             * priority
             */
            switch( front.cvars.Class[front.cvars.ExprLevel] ) {
            case TC_START:
            case TC_START1:
            case TC_START2:
            case TC_START_UNARY:
                return( tree );
            case TC_LEFT_PAREN:         /* bracketed expression */
                MustRecog( front, T_RIGHT_PAREN );
                curclass = TokenClass[front.cvars.CurToken];
                break;
            case TC_COMMA:
                if( op1->op.opr == OPR_ERROR ) {
                    FreeExprTree( front, tree );
                    return( op1 );
                }
                if( tree->op.opr == OPR_ERROR ) {
                    FreeExprTree( front, op1 );
                    return( tree );
                }
                op1 = VoidRValue( front, op1 );
                typ = TypeOf( tree );
                if( IsStruct( typ ) ) {
                    if( tree->op.opr == OPR_PUSHADDR ) {
                        tree->op.opr = OPR_PUSHSYM;
                    }
                } else {
                    tree = VoidRValue( front, tree );
                }
                tree = ExprNode( front, op1, OPR_COMMA, tree );
                tree->u.expr_type = typ;
                tree->op.u2.result_type = typ;
                break;
            case TC_ASSIGNMENT:  /*  = += -= *= /= %= >>= <<= &= ^= |=  */
                CheckConst( front, op1 );
                tree = AsgnOp( front, op1, front.cvars.Token[front.cvars.ExprLevel], tree );
                front.cvars.CompFlags.meaningless_stmt = false;
                front.cvars.CompFlags.useful_side_effect = true;
                break;
            case TC_TERNARY:
                MustRecog( front, T_COLON );
                break;
            case TC_TERNARY_DONE:
                front.cvars.CompFlags.meaningless_stmt &= TOKEN2BOOL( front.cvars.Token[front.cvars.ExprLevel] );
                --front.cvars.ExprLevel;
                tree = TernOp( front, front.cvars.ValueStack[front.cvars.ExprLevel], op1, tree );
                front.cvars.CompFlags.pending_dead_code = false;
                break;
            case TC_OR_OR:
                if( front.cvars.Token[front.cvars.ExprLevel] == T_NULL ) {
                    BoolExpr( front, tree );   /* checks type of op2 */
                    --front.cvars.SizeOfCount;
                    tree = op1;
                } else {
                    tree = FlowOp( front, op1, OPR_OR_OR, BoolExpr( front, tree ) );
                }
                front.cvars.CompFlags.meaningless_stmt = true;
                front.cvars.CompFlags.pending_dead_code = false;
                break;
            case TC_AND_AND:
                if( front.cvars.Token[front.cvars.ExprLevel] == T_NULL ) {
                    BoolExpr( front, tree );   /* checks type of op2 */
                    --front.cvars.SizeOfCount;
                    tree = op1;
                } else {
                    tree = FlowOp( front, op1, OPR_AND_AND, BoolExpr( front, tree ) );
                }
                front.cvars.CompFlags.meaningless_stmt = true;
                front.cvars.CompFlags.pending_dead_code = false;
                break;
            case TC_OR:
            case TC_XOR:
            case TC_AND:
                tree = IntOp( front, op1, front.cvars.Token[front.cvars.ExprLevel], tree );
                front.cvars.CompFlags.meaningless_stmt = true;
                break;
            case TC_EQ_NE:
            case TC_REL_OP:
                tree = RelOp( front, op1, front.cvars.Token[front.cvars.ExprLevel], tree );
                front.cvars.CompFlags.meaningless_stmt = true;
                break;
            case TC_SHIFT_OP:
                tree = ShiftOp( front, op1, front.cvars.Token[front.cvars.ExprLevel], tree );
                front.cvars.CompFlags.meaningless_stmt = true;
                break;
            case TC_ADD_OP:
                tree = AddOp( front, op1, front.cvars.Token[front.cvars.ExprLevel], tree );
                front.cvars.CompFlags.meaningless_stmt = true;
                break;
            case TC_MUL_OP:
                tree = BinOp( front, RValue(front, op1), front.cvars.Token[front.cvars.ExprLevel], tree );
                front.cvars.CompFlags.meaningless_stmt = true;
                break;
            case TC_PREINC:
                tree = IncDec( front, tree, front.cvars.Token[front.cvars.ExprLevel] );
                break;
            case TC_ADDR:
                tree = AddrOp( front, tree );
                front.cvars.CompFlags.meaningless_stmt = true;
                break;
            case TC_EXCLAMATION:
                tree = NotOp( front, RValue( front, tree ) );
                front.cvars.CompFlags.meaningless_stmt = true;
                break;
            case TC_PLUS:                       /* unary plus */
                tree = UnaryPlus( front, tree );
                front.cvars.CompFlags.meaningless_stmt = true;
                break;
            case TC_MINUS:                      /* unary minus */
                tree = UMinus( front, tree );
                front.cvars.CompFlags.meaningless_stmt = true;
                break;
            case TC_TILDE:
                tree = UComplement( front, tree );
                front.cvars.CompFlags.meaningless_stmt = true;
                break;
            case TC_SIZEOF:
                typ = tree->u.expr_type;
                if( typ != NULL
                  && typ->decl_type == TYP_FUNCTION ) {
                    tree = AddrOp( front, tree );
                    typ = tree->u.expr_type;
                }
                FreeExprTree( front, tree );
                if( typ != NULL ) {
                    tree = SizeofOp( front, typ );
                } else {
                    /*
                     * error use this as default
                     */
                    tree = UIntLeaf( front, 1 );
                }
                --front.cvars.SizeOfCount;
                front.cvars.CompFlags.meaningless_stmt = true;
                break;
            case TC_INDIRECTION:
                tree = PtrOp( front, RValue( front, tree ) );
                front.cvars.CompFlags.meaningless_stmt = true;
                break;
            case TC_CAST:
                tree = CnvOp( front, tree, TypeOf( op1 ), true );
                FreeExprTree( front, op1 );
                break;
            case TC_INDEX:
                tree = IndexOp( front, op1, tree );
                MustRecog( front, T_RIGHT_BRACKET );
                curclass = TokenClass[front.cvars.CurToken];
                front.cvars.CompFlags.meaningless_stmt = true;
                break;
            case TC_SEG_OP:
                tree = SegOp( front, op1, tree );
                front.cvars.CompFlags.meaningless_stmt = true;
                break;
            case TC_PARM_LIST:          /* do func call */
                {
                    SYMPTR          sym;
                    expr_level_type n;
                    TREEPTR         functree;

                    /*
                     * find the corresponding function symbol
                     */
                    n = front.cvars.ExprLevel;
                    while( front.cvars.Token[n] != T_LEFT_PAREN ) {
                        --n;
                    }
                    functree = front.cvars.ValueStack[n];
                    sym = SymGetPtr( front, functree->op.u2.sym_handle );
                    if( (sym->flags & SYM_TEMP) == 0 )
                        SetDiagSymbol( front, sym, functree->op.u2.sym_handle );
                    tree = GenNextParm( front, tree, &plist );
                    tree = GenFuncCall( front, tree );
                    MustRecog( front, T_RIGHT_PAREN );
                    if( plist != NULL ) {
                        /*
                         * function has prototype
                         */
                        if( *plist != NULL
                          && (*plist)->decl_type != TYP_DOT_DOT_DOT ) {
                            ParmCountMismatch(front);
                        }
                    } else {
                        AddCallNode( front, tree );
                    }
                    if( (sym->flags & SYM_TEMP) == 0 )
                        SetDiagPop(front);
                    PopNestedParms( front, &plist );
                    curclass = TokenClass[front.cvars.CurToken];
                    front.cvars.CompFlags.meaningless_stmt = false;
                    front.cvars.CompFlags.useful_side_effect = true;
                }
                break;
            }
            --front.cvars.ExprLevel;
        }
        switch( curclass ) {
        case TC_RIGHT_PAREN:
            CErr1( front, ERR_UNEXPECTED_RIGHT_PAREN );
            NextToken(front);
            continue;
        case TC_QUESTION:
            tree = StartTernary( front, tree );
            curclass = TC_TERNARY;
            break;
        case TC_COLON:
            tree = ColonOp( front, tree );
            front.cvars.CurToken = BOOL2TOKEN( front.cvars.CompFlags.meaningless_stmt );
            curclass = TC_TERNARY_DONE;
            break;
        case TC_OR_OR:
            tree = BoolExpr( front, tree );
            tree = OrOr( front, tree );
            break;
        case TC_AND_AND:
            tree = BoolExpr( front, tree );
            tree = AndAnd( front, tree );
            break;
        case TC_OR:
        case TC_XOR:
        case TC_AND:
        case TC_EQ_NE:
        case TC_REL_OP:
        case TC_SHIFT_OP:
        case TC_ADD_OP:
        case TC_MUL_OP:
//            tree = RValue( tree );
            break;
        case TC_ASSIGN_OP:
            curclass = TC_ASSIGNMENT;
            break;
        case TC_LEFT_BRACKET:
            curclass = TC_INDEX;
            break;
        case TC_COMMA:
            if( front.cvars.Class[front.cvars.ExprLevel] != TC_PARM_LIST ) {
                if( front.cvars.CompFlags.meaningless_stmt ) {
                    if( front.cvars.CompFlags.useful_side_effect ) {
                        CWarn1( front, ERR_USEFUL_SIDE_EFFECT );
                    } else {
                        CWarn1( front, ERR_MEANINGLESS );
                    }
                }
            } else {
                tree = GenNextParm( front, tree, &plist );
                curclass = TC_PARM_LIST;
            }
            break;
        case TC_FUNC_CALL:
            tree = StartFunc( front, tree, &plist );
            continue;
        case TC_DOT:
            NextToken(front);
            tree = DotOp( front, tree );
            if( front.cvars.CurToken != T_SEMI_COLON )
                NextToken(front);
            front.cvars.CompFlags.meaningless_stmt = true;
            continue;
        case TC_ARROW:
            NextToken(front);
            tree = ArrowOp( front, RValue( front, tree ) );
            if( front.cvars.CurToken != T_SEMI_COLON )
                NextToken(front);
            front.cvars.CompFlags.meaningless_stmt = true;
            continue;
        case TC_POSTINC:
            tree = IncDec( front, tree, front.cvars.CurToken );
            NextToken(front);
            continue;
        }
        if( front.cvars.ExprLevel >= (MAX_LEVEL - 1) ) {
            CErr1( front, ERR_EXPR_TOO_COMPLICATED );
            CSuicide(front);
        }
        if( tree != NULL ) {
            ++front.cvars.ExprLevel;
            front.cvars.ValueStack[front.cvars.ExprLevel] = tree;
            front.cvars.Class[front.cvars.ExprLevel] = curclass;
            front.cvars.Token[front.cvars.ExprLevel] = front.cvars.CurToken;
            tree = NULL;            /* indicate need opnd */
        }
        NextToken(front);
    }
}


static TREEPTR ExprOpnd( FrontRun& front )
{
    TREEPTR     tree;
    TYPEPTR     typ;

    front.cvars.CompFlags.meaningless_stmt = true;
    for( ;; ) {
        ++front.cvars.ExprLevel;
        switch( front.cvars.CurToken ) {
        case T_PLUS_PLUS:
        case T_MINUS_MINUS:
            front.cvars.Token[front.cvars.ExprLevel] = static_cast<TOKEN>(front.cvars.CurToken - 1);
            front.cvars.Class[front.cvars.ExprLevel] = TC_PREINC;
            NextToken(front);
            continue;
        case T_AND:
            front.cvars.Class[front.cvars.ExprLevel] = TC_ADDR;
            NextToken(front);
            continue;
        case T_EXCLAMATION:
            front.cvars.Class[front.cvars.ExprLevel] = TC_EXCLAMATION;
            NextToken(front);
            continue;
        case T_PLUS:
            front.cvars.Class[front.cvars.ExprLevel] = TC_PLUS;
            NextToken(front);
            continue;
        case T_MINUS:
            front.cvars.Class[front.cvars.ExprLevel] = TC_MINUS;
            NextToken(front);
            continue;
        case T_TILDE:
            front.cvars.Class[front.cvars.ExprLevel] = TC_TILDE;
            NextToken(front);
            continue;
        case T_SIZEOF:
            if( !IS_PPCTL_NORMAL() ) {
                CErr1( front, ERR_NO_SIZEOF_DURING_PP );
            }
            front.cvars.Class[front.cvars.ExprLevel] = TC_SIZEOF;
            NextToken(front);
            front.cvars.Token[front.cvars.ExprLevel] = front.cvars.CurToken;
            if( front.cvars.CurToken == T_LEFT_PAREN ) {
                NextToken(front);
                typ = TypeName(front);
                if( typ != NULL ) {
                    tree = SizeofOp( front, typ );
                    MustRecog( front, T_RIGHT_PAREN );
                    break;
                }
                ++front.cvars.ExprLevel;
                front.cvars.Class[front.cvars.ExprLevel] = TC_LEFT_PAREN;
            } else {
                typ = TypeName(front);
                if( typ != NULL ) {
                    tree = SizeofOp( front, typ );
                    break;
                }
            }
            ++front.cvars.SizeOfCount;
            continue;
        case T___BUILTIN_ISFLOAT:
            NextToken(front);
            if( front.cvars.CurToken == T_LEFT_PAREN ) {
                NextToken(front);
                typ = TypeName(front);
                if( typ != NULL ) {
                    SKIP_TYPEDEFS( typ );
                    switch( typ->decl_type ) {
                    case TYP_FLOAT:
                    case TYP_DOUBLE:
                        tree = IntLeaf( front, 1 );
                        break;
                    default:
                        tree = IntLeaf( front, 0 );
                        break;
                    }
                } else {
                    CErr1( front, ERR_OPND_OF_BUILTIN_ISFLOAT_MUST_BE_TYPE );
                    NextToken(front);
                    tree = LeafNode( front, OPR_ERROR );
                }
                MustRecog( front, T_RIGHT_PAREN );
            } else {
                MustRecog( front, T_LEFT_PAREN );
                tree = LeafNode( front, OPR_ERROR );
            }
            break;
        case T_TIMES:
            front.cvars.Class[front.cvars.ExprLevel] = TC_INDIRECTION;
            NextToken(front);
            continue;
        case T_LEFT_PAREN:
            NextToken(front);
            typ = TypeName(front);
            if( typ != NULL ) {
                if( !IS_PPCTL_NORMAL() ) {
                    CErr1( front, ERR_NO_CAST_DURING_PP );
                }
                MustRecog( front, T_RIGHT_PAREN );
                front.cvars.Class[front.cvars.ExprLevel] = TC_CAST;
                tree = LeafNode( front, OPR_CAST );
                tree->u.expr_type = typ;
                front.cvars.ValueStack[front.cvars.ExprLevel] = tree;
            } else {
                front.cvars.Class[front.cvars.ExprLevel] = TC_LEFT_PAREN;
            }
            continue;
        case T_ID:
            tree = ExprId(front);
            front.cvars.CompFlags.meaningless_stmt = false;
            break;
        case T_SAVED_ID:
            tree = SymLeaf(front);
            CMemFree( front, front.cvars.SavedId );
            front.cvars.SavedId = NULL;
            front.cvars.CurToken = front.cvars.LAToken;
            front.cvars.CompFlags.meaningless_stmt = false;
            break;
        case T_CONSTANT:
            tree = ConstLeaf(front);
            NextToken(front);
            break;
        case T_STRING:
            if( front.cvars.CompFlags.strings_in_code_segment ) {
                tree = StringLeaf( front, STRLIT_CONST );
            } else {
                tree = StringLeaf( front, STRLIT_NONE );
            }
            break;
        case T_BAD_TOKEN:
            CErr1( front, front.cvars.BadTokenInfo );
            front.cvars.BadTokenInfo = ERR_NONE;
            NextToken(front);
            tree = ErrorNode( front, NULL );
            break;
        case T___OW_IMAGINARY_UNIT:
            {
                FLOATVAL    *flt;

#define FLOAT_PLUS_ONE "+1.0"
                tree = LeafNode( front, OPR_PUSHFLOAT );
                tree->op.u1.const_type = TYP_DIMAGINARY;
                flt = static_cast<FLOATVAL *>(CMemAlloc( front, sizeof( FLOATVAL ) - 1 + LENLIT( FLOAT_PLUS_ONE ) + 1 ));
                strcpy( flt->string, FLOAT_PLUS_ONE );
                flt->len = LENLIT( FLOAT_PLUS_ONE );
                flt->type = TYP_DIMAGINARY;
                flt->next = NULL;
                tree->op.u2.float_value = flt;
                tree->op.opr = OPR_PUSHFLOAT;
                tree->u.expr_type = GetType( front, tree->op.u1.const_type );
            }
            NextToken(front);
            break;
        default:
            if( !IS_PPCTL_NORMAL()
              && IS_KEYWORD( front.cvars.CurToken ) ) {
                tree = ExprId(front);
            } else {
                CErr1( front, ERR_MISSING_OPERAND );
                tree = ErrorNode( front, NULL );
            }
        }
        break;
    }
    --front.cvars.ExprLevel;
    return( tree );
}


static bool IsMacroDefined( FrontRun& front )
{
    MEPTR       mentry;

    if( front.cvars.CurToken != T_ID ) {
        ExpectIdentifier(front);
    } else {
        mentry = MacroLookup( front, front.cvars.Buffer );
        if( mentry != NULL ) {
            mentry->macro_flags |= MFLAG_REFERENCED;
            return( true );
        }
    }
    return( false );
}


static TREEPTR ExprId( FrontRun& front )
{
    TREEPTR     tree;
    int         value;
    int         count;

    if( !IS_PPCTL_NORMAL() ) {
        if( IS_PPOPERATOR_DEFINED( front.cvars.Buffer ) ) {
            ppctl_t old_ppctl;

            old_ppctl = front.cvars.PPControl;
            PPCTL_DISABLE_MACROS();    /* don't want macro expanded */
            NextToken(front);
            if( front.cvars.CurToken == T_LEFT_PAREN ) {
                NextToken(front);
                value = IsMacroDefined(front);
                NextToken(front);
                MustRecog( front, T_RIGHT_PAREN );
                front.cvars.PPControl = old_ppctl;
            } else {
                value = IsMacroDefined(front);
                front.cvars.PPControl = old_ppctl;
                NextToken(front);
            }
        } else {
//            if( SizeOfCount == 0 ) {
//                CWarn2p( ERR_UNDECLARED_PP_SYM, Buffer);
//            }
            NextToken(front);
            if( front.cvars.CurToken == T_LEFT_PAREN ) {
                count = 0;
                for(;;) {               /* flush to ')' or end of line */
                    NextToken(front);
                    if( front.cvars.CurToken == T_NULL )
                        break;
                    if( front.cvars.CurToken == T_LEFT_PAREN )
                        ++count;
                    if( front.cvars.CurToken == T_RIGHT_PAREN ) {
                        if( count == 0 ) {
                            NextToken(front);
                            break;
                        }
                        --count;
                    }
                }
            }
            value = 0;
        }
        tree = IntLeaf( front, value );
    } else {
        tree = SymLeaf(front);
    }
    return( tree );
}


#if 0
static bool LValueArray( TREEPTR tree )
{
    TYPEPTR     typ;

    if( tree->op.opr == OPR_PUSHSTRING ) {
        return( true );
    } else if( IsLValue( tree ) ) {
        return( true );
    } else {
        typ = tree->u.expr_type;
        SKIP_TYPEDEFS( typ );
        if( typ->decl_type == TYP_POINTER ) {
            return( true );
        }
    }
    return( false );
}
#endif

static TREEPTR GenIndex( FrontRun& front, TREEPTR tree, TREEPTR index_expr )
{
    TYPEPTR         typ;
    op_flags        tree_flags;

#if 0
    if( ! LValueArray( tree ) ) {
        CErr1( ERR_CANT_TAKE_ADDR_OF_RVALUE );
        FreeExprTree( index_expr );
        return( ErrorNode( tree ) );
    }
#endif
    index_expr = RValue( front, index_expr );
    if( DataTypeOf( TypeOf( index_expr ) ) > TYP_ULONG64 ) {
        CErr1( front, ERR_EXPR_MUST_BE_INTEGRAL );
        FreeExprTree( front, tree );
        return( ErrorNode( front, index_expr ) );
    }
    if( TypeOf( index_expr )->type_flags & TF2_TYP_PLAIN_CHAR ) {
        CWarn1( front, ERR_PLAIN_CHAR_SUBSCRIPT );
    }
    typ = tree->u.expr_type;
    SKIP_TYPEDEFS( typ );
    if( typ->decl_type == TYP_ARRAY ) {
        tree_flags = tree->op.flags;  /* get modifiers of array obj */
    } else if( typ->decl_type == TYP_POINTER ) {
        type_modifiers  flags;

        tree = CheckBasedPtr( front, tree, typ, &flags );
        /*
         * We will indirect so get modifiers of indirected obj
         */
        tree_flags = OpFlags( flags );
    } else {
        CErr2p( front, ERR_FATAL_ERROR, "Bad array index tree" );
        tree_flags = OPFLAG_NONE;
    }
    typ = typ->object;
    SKIP_TYPEDEFS( typ );
    /*
     * Some crappy little optimization that probably does nothing
     */
    if( index_expr->op.opr == OPR_PUSHINT
      && tree->u.expr_type->decl_type == TYP_ARRAY ) {
        index_expr->op.u2.long_value *= SizeOfArg( front, typ );
        tree = ExprNode( front, tree, OPR_DOT, index_expr );
#if _CPU == 8086
        if( index_expr->op.u2.long_value > TARGET_INT_MAX ) {
            index_expr->op.u1.const_type = TYP_LONG;
            index_expr->u.expr_type = GetType( front, TYP_LONG );
        }
#endif
    } else {
#if _CPU == 8086
        if( DataTypeOf( TypeOf( index_expr ) ) == TYP_UINT ) {
            if( (tree_flags & OPFLAG_HUGEPTR)
              || (IsHugeData()
              && (tree_flags & (OPFLAG_NEARPTR | OPFLAG_FARPTR)) == 0) ) {
                index_expr = CnvOp( front, index_expr, GetType( front, TYP_LONG ), false );
            }
        }
#endif
        tree = ExprNode( front, tree, OPR_INDEX, index_expr );
    }
    tree->u.expr_type = typ;
    tree->op.u2.result_type = typ;
    tree->op.flags = tree_flags;
    return( tree );
}

static TREEPTR ArrayIndex( FrontRun& front, TREEPTR tree, TREEPTR index_expr )
{
    tree = GenIndex( front, tree, index_expr );
    return( tree );
}


static TREEPTR IndexOp( FrontRun& front, TREEPTR tree, TREEPTR index_expr )
{
    TYPEPTR     typ;

    if( tree->op.opr != OPR_ERROR ) {
        typ = tree->u.expr_type;
        SKIP_TYPEDEFS( typ );
        if( typ->decl_type == TYP_ARRAY ) {
            tree = ArrayIndex( front, tree, index_expr );
        } else if( typ->decl_type == TYP_POINTER ) {
            tree = GenIndex( front, RValue( front, tree ), index_expr );
        } else {                    /* try index[array] */
            typ = index_expr->u.expr_type;
            SKIP_TYPEDEFS( typ );
            if( typ->decl_type == TYP_ARRAY ) {
                tree = ArrayIndex( front, index_expr, tree );
            } else if( typ->decl_type == TYP_POINTER ) {
                tree = GenIndex( front, RValue( front, index_expr ), tree );
            } else {
                CErr1( front, ERR_EXPR_MUST_BE_ARRAY );
                FreeExprTree( front, index_expr );
                tree = ErrorNode( front, tree );
            }
        }
    }
    return( tree );
}

static void AddCallNode( FrontRun& front, TREEPTR tree )
/**************************************
 * if a function call has no prototype wait till end
 * to check it out
 */
{
    call_list   *new_;

    if( tree->left != NULL ) {  /* could be errors */
        new_ = static_cast<call_list *>(CMemAlloc( front, sizeof( call_list ) ));
        new_->next = NULL;
        new_->callnode = tree;
        new_->src_loc = front.cvars.SrcLoc;
        *front.cexpr.LastCallLnk = new_;
        front.cexpr.LastCallLnk = &new_->next;
    }
}

void CheckCallNode( FrontRun& front, TREEPTR tree )
{
    call_list   **lnk;
    call_list   *curr;

    for( lnk = front.cexpr.FirstCallLnk; (curr = *lnk) != NULL; lnk = &curr->next ) {
        if( curr->callnode == tree ) {
            curr->callnode = NULL;
            break;
        }
    }
}

static int ParmNum( FrontRun& front )
/*************************
 * get current parm num
 */
{
    int             parm_count;
    expr_level_type n;

    parm_count = 1;
    for( n = front.cvars.ExprLevel; front.cvars.Token[n] != T_LEFT_PAREN; --n ) {
        ++parm_count;
    }
    return( parm_count );
}

static TREEPTR GenNextParm( FrontRun& front, TREEPTR tree, TYPEPTR **plistptr )
{
    TYPEPTR     *plist;
    TYPEPTR     typ;
    TYPEPTR     typ2;
    TYPEPTR     parm_typ;

    tree = RValue( front, tree );
    if( tree->op.opr == OPR_ERROR )
        return( tree );
    /*
     * skip typedefs, go into enum base
     */
    typ = SkipTypeFluff( tree->u.expr_type );
    plist = *plistptr;
    if( plist != NULL ) {
        if( *plist == NULL ) {  /* To many parm trees */
#if _CPU == 386
            /*
             * can allow wrong number of parms with -3s option
             */
            if( !front.cvars.CompFlags.register_conventions ) {
                CWarn1( front, ERR_PARM_COUNT_WARNING );
            } else {
                ParmCountMismatch(front);
            }
#else
            ParmCountMismatch(front);
#endif
            *plist = GetType( front, TYP_DOT_DOT_DOT );        /* shut up message */
            plist = NULL;
        } else if( (*plist)->decl_type == TYP_DOT_DOT_DOT ) {
            plist = NULL;
        } else if( (*plist)->decl_type == TYP_VOID ) {
            return( tree );
        }
    }
    if( plist != NULL ) {
        /*
         * do conversion from tree to parm type
         */
        parm_typ = *plist;
        CheckParmAssign( front, parm_typ, tree, ParmNum(front), true );
        if( parm_typ != NULL ) {
            /*
             * skip typedefs, go into enum base
             */
            parm_typ = SkipTypeFluff( parm_typ );
            if( parm_typ != typ ) {
                tree = FixupAss( front, tree, parm_typ );
            }
            ++*plistptr;
        }
    } else {
        if( typ->decl_type == TYP_FLOAT ) {     /* default conversions */
            typ = GetType( front, TYP_DOUBLE );
            tree = FixupAss( front, tree, typ );
        } else if( typ->decl_type == TYP_POINTER ) {
            typ2 = typ->object;
            SKIP_TYPEDEFS( typ );
            if( typ2->decl_type != TYP_FUNCTION ) {
                if( typ->u.p.decl_flags & FLAG_NEAR ) {
                    if( front.cvars.DataPtrSize == TARGET_FAR_POINTER ) {
                        typ = PtrNode( front, typ2, FLAG_NONE, SEG_DATA );
                        tree = FixupAss( front, tree, typ );
                    }
                }
            }
        }
    }
    if( typ->decl_type == TYP_POINTER )
        typ = typ->object;
    SKIP_TYPEDEFS( typ );
    if( typ->decl_type == TYP_DOUBLE
      || typ->decl_type == TYP_LONG_DOUBLE
      || typ->decl_type == TYP_FLOAT ) {
        front.cvars.CompFlags.float_used = true;
    }
    return( tree );
}


#if _RISC_CPU
/*
 * This really ought to be defined somewhere else...
 */
  #if _CPU == _AXP
    #define REG_SIZE    8
  #else
    #define REG_SIZE    4
  #endif
static TREEPTR GenVaStartNode( TREEPTR last_parm )
/*************************************************
 * there should be 3 parms __builtin_va_start( list, parm_name, stdarg )
 * - first parm should be name of va_list
 * - second parm should be name of the parameter before the ...
 * - last_parm should be an integer 0 or 1 (0=>varargs, 1=>stdarg)
 */
{
    SYMPTR      sym;
    SYM_HANDLE  sym_handle;
    int         offset;
    TREEPTR     parmsym;
    TREEPTR     tree;

    tree = NULL;
    if( ExprLevel >= 2
      && Token[ExprLevel] != T_LEFT_PAREN
      && Token[ExprLevel-1] != T_LEFT_PAREN
      && Token[ExprLevel-2] == T_LEFT_PAREN ) {
        offset = 0;
        if( last_parm->op.opr == OPR_PUSHINT ) {
            /*
             * check for varargs
             */
            if( CheckZeroConstant( last_parm ) ) {
                offset = -REG_SIZE;
            }
        }
        /*
         * get name of parameter
         */
        parmsym = ValueStack[ExprLevel];
        sym_handle = SYM_NULL;
        if( parmsym->op.opr == OPR_PUSHSYM ) {
            for( sym_handle = CurFunc->u.func.parms; sym_handle != SYM_NULL; sym_handle = sym->handle ) {
                sym = SymGetPtr( sym_handle );
                offset += _RoundUp( SizeOfArg( sym->sym_type ), REG_SIZE );
                if( sym_handle == parmsym->op.u2.sym_handle ) {
                    break;
                }
            }
        }
        if( offset == 0
          || sym_handle == SYM_NULL ) {
            /*
             * error: name not found in symbol list
             */
            sym = SymGetPtr( parmsym->op.u2.sym_handle );
            CErr2p( ERR_SYM_NOT_IN_PARM_LIST, SymName( sym, parmsym->op.u2.sym_handle ) );
        }
        FreeExprNode( parmsym );
        tree = ValueStack[ExprLevel - 1];
        if( tree->op.opr == OPR_PUSHSYM ) {
            tree->op.opr = OPR_PUSHADDR;            /* want address of va_list */
        }
        tree->op.flags &= ~OPFLAG_RVALUE;
        tree = ExprNode( tree, OPR_VASTART, IntLeaf( offset ) );
        ExprLevel -= 2;
    } else {
        /*
         * error
         */
    }
    return( tree );
}

static TREEPTR GenAllocaNode( TREEPTR size_parm )
{
    /*
     * there should be 1 parm __builtin_alloca( size )
     */
    TREEPTR     tree;

    if( Token[ExprLevel] == T_LEFT_PAREN ) {
        tree = ExprNode( NULL, OPR_ALLOCA, size_parm );
        tree->u.expr_type = PtrNode( GetType( TYP_VOID ), FLAG_NONE, SEG_STACK );
    } else {
        /*
         * error
         */
        tree = NULL;
    }
    return( tree );
}
#endif

#if _CPU == _PPC
static TREEPTR GenVaArgNode( TREEPTR last_parm )
{
    /*
     * there should be 2 parms __builtin_va_arg( list, type_arg )
     * - first parm should be name of va_list
     * - last_parm should be an integer 0 or 1 (0=>varargs, 1=>stdarg)
     */

    TREEPTR     parmsym;
    TREEPTR     tree;

    tree = NULL;
    if( ExprLevel >= 2
      && Token[ExprLevel] != T_LEFT_PAREN
      && Token[ExprLevel-1] != T_LEFT_PAREN
      && Token[ExprLevel-2] == T_LEFT_PAREN ) {
        if( last_parm->op.opr == OPR_PUSHINT ) {
            /*
             * check for varargs
             */
            if( CheckZeroConstant( last_parm ) ) {
            }
        }
        /*
         * get name of parameter
         */
        parmsym = ValueStack[ExprLevel];
        FreeExprNode( parmsym );
        tree = ValueStack[ExprLevel - 1];
        if( tree->op.opr == OPR_PUSHSYM ) {
            tree->op.opr = OPR_PUSHADDR;            /* want address of va_list */
        }
        tree->op.flags &= ~OPFLAG_RVALUE;
        tree = ExprNode( tree, OPR_VASTART, IntLeaf( 0 ) );
        ExprLevel -= 2;
    } else {
        /*
         * error
         */
    }
    return( tree );
}
#endif

static TREEPTR GenFuncCall( FrontRun& front, TREEPTR last_parm )
{
    expr_level_type i;
    expr_level_type n;
    TREEPTR         functree;
    TREEPTR         tree;
    TYPEPTR         func_result_type;
    TYPEPTR         typ;
    type_modifiers  flags;
    target_size     string_len;
    bool            optimized;
//    bool            recursive = false;
    bool            far16_func = false;
    int             parm_count;
    SYM_NAMEPTR     sym_name;
    SYM_ENTRY       sym;

    flags = static_cast<type_modifiers>(0);
    parm_count = 1;
    for( n = front.cvars.ExprLevel; front.cvars.Token[n] != T_LEFT_PAREN; --n ) {
        ++parm_count;
    }
    functree = front.cvars.ValueStack[n];
    typ = TypeOf( functree );
    if( typ->decl_type == TYP_FUNCTION ) {
        SymGet( front, &sym, functree->op.u2.sym_handle );
        sym_name = SymName( front, &sym, functree->op.u2.sym_handle );
        flags = sym.mods;
        typ = typ->object;
        SKIP_TYPEDEFS( typ );
        if( typ->decl_type == TYP_DOUBLE
          || typ->decl_type == TYP_LONG_DOUBLE
          || typ->decl_type == TYP_FLOAT ) {
            front.cvars.CompFlags.float_used = true;
        }
#if _CPU == 386
        {
            aux_info    *inf;

            inf = FindInfo( front, &sym, functree->op.u2.sym_handle );
            if( inf == &Far16PascalInfo
              || inf == &Far16CdeclInfo ) {
                far16_func = true;
                typ = Far16Type( front, typ );
            }
        }
#endif
    } else {
        typ = TypeDefault(front);
    }
    func_result_type = typ;
    if( ParmsToBeReversed( flags, NULL ) ) {
        i = n;
        if( i == front.cvars.ExprLevel ) {
            tree = ParmNode( front, NULL, last_parm, far16_func );
        } else {
            ++i;
            tree = ParmNode( front, NULL, front.cvars.ValueStack[i], far16_func );
        }
        tree->op.u2.result_type = typ;
        while( i != front.cvars.ExprLevel ) {
            ++i;
            tree = ParmNode( front, tree, front.cvars.ValueStack[i], far16_func );
        }
        if( n != front.cvars.ExprLevel ) {
            tree = ParmNode( front, tree, last_parm, far16_func );     /* this is 1'st parm */
        }
        front.cvars.ExprLevel = n;
    } else {
        optimized = false;
        string_len = 0;
        if( last_parm->op.opr == OPR_PUSHSTRING ) {
            if( (front.cvars.GenSwitches & CGSW_GEN_NO_OPTIMIZATION) == 0 ) {
                string_len = last_parm->op.u2.string_handle->length;
                if( functree->op.opr == OPR_PUSHADDR ) {
                    SymGet( front, &sym, functree->op.u2.sym_handle );
                    sym_name = SymName( front, &sym, functree->op.u2.sym_handle );
                    if( strcmp( sym_name, "_inline_strcmp" ) == 0 ) {
                        SymGet( front, &sym, front.cvars.SymMEMCMP );
                        tree = VarLeaf( front, &sym, front.cvars.SymMEMCMP );
                        optimized = true;
                    }
                }
            }
        } else {
            if( functree->op.opr == OPR_FUNCNAME ) {
                SymGet( front, &sym, functree->op.u2.sym_handle );
#if 0
                if( functree->op.u2.sym_handle == CurFuncHandle ) {
                    recursive = true;
                }
#endif
                sym_name = SymName( front, &sym, functree->op.u2.sym_handle );
                if( (front.cvars.GenSwitches & CGSW_GEN_NO_OPTIMIZATION) == 0
                  && front.cvars.CompFlags.extensions_enabled ) {
                    for( i = 0; MathFuncs[i].name != NULL; ++i ) {
                        if( parm_count == MathFuncs[i].parm_count
                          && ( strcmp( sym_name, MathFuncs[i].name ) == 0
                          || (sym.flags & SYM_INTRINSIC)
                          && strcmp( sym_name, MathFuncs[i].iname ) == 0 ) ) {
                            FreeExprNode( front, functree );
                            if( parm_count == 1 ) {
                                tree = ExprNode( front, NULL, OPR_MATHFUNC, last_parm );
                            } else {  /* parm_count == 2 */
                                tree = front.cvars.ValueStack[front.cvars.ExprLevel]; /* op1 */
                                --front.cvars.ExprLevel;
                                tree = ExprNode( front, tree, OPR_MATHFUNC2, last_parm );
                            }
                            tree->u.expr_type = GetType( front, TYP_DOUBLE );
                            tree->op.u1.mathfunc = MathFuncs[i].mathop;
                            return( tree );
                        }
                    }
                }
#if _RISC_CPU
                if( strcmp( sym_name, "__builtin_va_start" ) == 0 ) {
                    return( GenVaStartNode( last_parm ) );
                }
                if( strcmp( sym_name, "__builtin_alloca" ) == 0 ) {
                    return( GenAllocaNode( last_parm ) );
                }
    #if _CPU == _PPC
                if( strcmp( sym_name, "__builtin_va_arg" ) == 0 ) {
                    return( GenVaArgNode( last_parm ) );
                }
    #endif
#endif
            }
        }
        if( !optimized ) {
            tree = ParmNode( front, NULL, last_parm, far16_func );
        } else {
            tree = ParmNode( front, NULL, IntLeaf( front, string_len ), far16_func );
            tree = ParmNode( front, tree, last_parm, far16_func );
        }
        while( front.cvars.Token[front.cvars.ExprLevel] != T_LEFT_PAREN ) {
            tree = ParmNode( front, tree, front.cvars.ValueStack[front.cvars.ExprLevel], far16_func );
            --front.cvars.ExprLevel;
        }
    }
    tree = CallNode( front, functree, tree, func_result_type );
    if( front.cvars.DeadCode == 0 ) {
        if( functree->op.opr == OPR_FUNCNAME ) {
            front.cvars.CompFlags.pending_dead_code |= FunctionAborts( front, &sym, functree->op.u2.sym_handle );
        }
    }
    return( tree );
}

static void PopNestedParms( FrontRun& front, TYPEPTR **plistptr )
{
    struct nested_parm_lists    *npl;

    if( front.cvars.NestedParms != NULL ) {
        npl = front.cvars.NestedParms;
        *plistptr = npl->next_parm_type;
        front.cvars.NestedParms = npl->prev_list;
        CMemFree( front, npl );
    } else {
        *plistptr = NULL;
    }
}

static TREEPTR DummyFuncName( FrontRun& front )
{
    TREEPTR             tree;
    TYPEPTR             typ;
    SYM_HANDLE          sym_handle;
    SYM_ENTRY           sym;

    typ = FuncNode( front, GetType( front, TYP_INT ), FLAG_NONE, NULL );
    sym_handle = MakeNewDotSym( front, &sym, 'F', typ, SC_AUTO );
    sym.flags |= SYM_FUNCTION;
    sym.mods = FLAG_NONE;
    SymReplace( front, &sym, sym_handle );
    tree = VarLeaf( front, &sym, sym_handle );
    return( tree );
}

static TREEPTR StartFunc( FrontRun& front, TREEPTR tree, TYPEPTR **plistptr )
{
    TYPEPTR             typ;
    TYPEPTR             orig_typ;
    TYPEPTR             *parm_types;
    TYPEPTR             parm_type;
    type_modifiers      decl_flags;
//    bool                recursive = false;
#ifdef __SEH__
    opr_code            opr;
#endif
    SYM_HANDLE          sym_handle;
    SYM_ENTRY           sym;

    if( tree->op.opr == OPR_ERROR ) {   /* hoke something up */
        tree = DummyFuncName(front);
    }
    decl_flags = FLAG_NONE;
    typ = tree->u.expr_type;
    orig_typ = typ;
    SKIP_TYPEDEFS( typ );
    if( typ->decl_type == TYP_POINTER ) {
        tree = PtrOp( front, RValue( front, tree ) );
        typ = tree->u.expr_type;
        orig_typ = typ;
        SKIP_TYPEDEFS( typ );
    }
    if( tree->op.opr == OPR_PUSHADDR ) {        /* direct call */
        tree->op.opr = OPR_FUNCNAME;            /* change to funcname */
        sym_handle = tree->op.u2.sym_handle;
        SymGet( front, &sym, sym_handle );
#if 0
        if( tree->op.u2.sym_handle == CurFuncHandle ) {
            recursive = true;
        }
#endif
        decl_flags = sym.mods;
        /*
         * is it OK to inline?
         */
    } else {                                    /* indirect call */
        if( front.cvars.CompFlags.initializing_data ) {
            CErr1( front, ERR_NOT_A_CONSTANT_EXPR );
            sym_handle = SYM_NULL;
            sym.flags = SYM_NONE;
        } else {
            if( tree->op.opr == OPR_POINTS ) {  /* need to recover decl flags */
                decl_flags = typ->u.fn.decl_flags;
            }
            sym_handle = MakeNewDotSym( front, &sym, 'F', orig_typ, SC_AUTO );
            sym.flags |= SYM_FUNCTION;
            sym.mods = decl_flags;
            SymReplace( front, &sym, sym_handle );
            tree = ExprNode( front, NULL, OPR_CALL_INDIRECT, tree );
            tree->u.expr_type = typ;
            tree->op.u2.sym_handle = sym_handle;
        }
    }
    if( typ->decl_type != TYP_FUNCTION ) {
        CErr1( front, ERR_NOT_A_FUNCTION );
        tree = DummyFuncName(front);
        typ = tree->u.expr_type;
    } else {
        if( decl_flags & FLAG_FAR16 ) {
            if( front.cvars.DeadCode == 0 ) {
                front.cvars.CurFuncSym.flags |= SYM_FUNC_NEEDS_THUNK;
            }
        }
    }
    parm_types = typ->u.fn.parms;   /* Parameters (prototype), if any */
    NextToken(front);                        /* skip over '(' */
    if( front.cvars.CurToken != T_RIGHT_PAREN ) {
        /*
         * push previous plist for nested calls
         */
        struct nested_parm_lists    *npl;

        npl = static_cast<struct nested_parm_lists *>(CMemAlloc( front, sizeof( struct nested_parm_lists ) ));
        npl->prev_list = front.cvars.NestedParms;
        npl->next_parm_type = *plistptr;
        front.cvars.NestedParms = npl;

        *plistptr = parm_types;
        ++front.cvars.ExprLevel;
        front.cvars.ValueStack[front.cvars.ExprLevel] = tree;
        front.cvars.Class[front.cvars.ExprLevel] = TC_PARM_LIST;
        front.cvars.Token[front.cvars.ExprLevel] = T_LEFT_PAREN;
        tree = NULL;               /* indicate need operand for parm */
    } else { /* foo() build call */
#ifdef __SEH__
        opr = OPR_CALL;
        if( front.cvars.CompFlags.exception_filter_expr
          || front.cvars.CompFlags.in_finally_block
          || front.cvars.CompFlags.exception_handler ) {
            SYM_NAMEPTR         func_name;

            if( (sym.flags & SYM_TEMP) == 0 ) {
                sym_handle = tree->op.u2.sym_handle;
                SymGet( front, &sym, sym_handle );
                func_name = SymName( front, &sym, sym_handle );
                if( strcmp( func_name, "_exception_code" ) == 0 ) {
                    if( front.cvars.CompFlags.exception_filter_expr
                      || front.cvars.CompFlags.exception_handler ) {
                        opr = OPR_EXCEPT_CODE;
                    }
                } else if( strcmp( func_name, "_exception_info" ) == 0 ) {
                    if( front.cvars.CompFlags.exception_filter_expr ) {
                        opr = OPR_EXCEPT_INFO;
                    }
                } else if( strcmp( func_name, "_abnormal_termination" ) == 0 ) {
                    if( front.cvars.CompFlags.in_finally_block ) {
                        opr = OPR_ABNORMAL_TERMINATION;
                    }
                }
                if( opr != OPR_CALL ) {
                    FreeExprNode( front, tree );
                    tree = LeafNode( front, opr );
                    tree->u.expr_type = GetType( front, TYP_INT );
                }
            }
        }
        if( opr == OPR_CALL ) {
#endif
            typ = typ->object; /* get return type */
            SKIP_TYPEDEFS( typ );
            if( (sym.flags & SYM_TEMP) == 0 ) {
                SetDiagSymbol( front, &sym, sym_handle );
            }
            tree = CallNode( front, tree, NULL, typ );
            if( parm_types != NULL ) {        /* function has prototype */
                parm_type = *parm_types;
                if( parm_type != NULL
                  && parm_type->decl_type != TYP_VOID ) {
                    ParmCountMismatch(front);
                }
            } else {                                /* check later */
                AddCallNode( front, tree );
            }
            if( (sym.flags & SYM_TEMP) == 0 ) {
                SetDiagPop(front);
            }
#ifdef __SEH__
        }
#endif
        front.cvars.CompFlags.meaningless_stmt = false;
        front.cvars.CompFlags.useful_side_effect = true;
        NextToken(front);                    /* skip over ')' */
    }
    return( tree );
}


static TREEPTR OrOr( FrontRun& front, TREEPTR tree )
{
    /*
     * This routine is called when || token is found
     */
    if( tree->op.opr == OPR_PUSHINT ) {
        if( !CheckZeroConstant( front, tree ) ) {
            ++front.cvars.SizeOfCount;      /* skip code gen */
            front.cvars.CurToken = T_NULL;
        }
    }
    return( tree );
}


static TREEPTR AndAnd( FrontRun& front, TREEPTR tree )
{
    /*
     * This routine is called when && token is found
     */
    if( tree->op.opr == OPR_PUSHINT ) {
        if( CheckZeroConstant( front, tree ) ) {
            ++front.cvars.SizeOfCount;      /* skip code gen */
            front.cvars.CurToken = T_NULL;
        }
    }
    return( tree );
}


TREEPTR BoolExpr( FrontRun& front, TREEPTR tree )
{
    switch( tree->op.opr ) {
    case OPR_NOT:
    case OPR_CMP:
    case OPR_OR_OR:
    case OPR_AND_AND:
    case OPR_ERROR:
        /*
         * tree is a boolean expression
         */
        break;
    default:
        if( tree->op.opr == OPR_EQUALS ) {
            if( IsConstLeaf( tree ) ) {
                CWarn1( front, ERR_ASSIGNMENT_IN_BOOL_EXPR );
            }
        }
        tree = RValue( front, tree );
        if( DataTypeOf( TypeOf( tree ) ) > TYP_POINTER ) {
            CErr1( front, ERR_EXPR_MUST_BE_SCALAR );
        } else if( tree->op.opr == OPR_PUSHINT ) {
            if( !CheckZeroConstant( front, tree ) ) {
                tree->op.u2.ulong_value = 1;
            }
            tree->op.u1.const_type = TYP_INT;
            tree->u.expr_type = GetType( front, TYP_INT );
        } else {
            tree = RelOp( front, tree, T_NE, IntLeaf( front, 0 ) );
        }
    }
    return( tree );
}


static TREEPTR NotOp( FrontRun& front, TREEPTR tree )
{
    TREEPTR     opnd;
    FLOATVAL    *flt;

    if( IsConstLeaf( tree ) ) {
        switch( tree->op.u1.const_type ) {
        case TYP_CHAR:
        case TYP_UCHAR:
        case TYP_SHORT:
        case TYP_USHORT:
        case TYP_INT:
        case TYP_UINT:
        case TYP_LONG:
        case TYP_ULONG:
            tree->op.u2.long_value = ( tree->op.u2.long_value == 0 );
            tree->op.u1.const_type = TYP_INT;
            break;
        case TYP_LONG64:
        case TYP_ULONG64:
            tree->op.u2.long_value = U64isZero( tree->op.u2.long64_value );
            tree->op.u1.const_type = TYP_INT;
            break;
        case TYP_FLOAT:
        case TYP_DOUBLE:
        case TYP_LONG_DOUBLE:
            flt = tree->op.u2.float_value;
            if( atof( flt->string ) == 0.0 ) {
                tree->op.u2.long_value = 1;
            } else {
                tree->op.u2.long_value = 0;
            }
            CMemFree( front, flt );
            tree->op.opr = OPR_PUSHINT;
            tree->op.u1.const_type = TYP_INT;
            break;
        default:
            tree = RelOp( front, ScalarExpr( front, tree ), T_EQ, IntLeaf( front, 0 ) );
            break;
        }
    } else {
        if( tree->op.opr == OPR_EQUALS ) {
            opnd = tree->right;
            if( IsConstLeaf( opnd ) ) {
                CWarn1( front, ERR_ASSIGNMENT_IN_BOOL_EXPR );
            }
        }
        tree = RelOp( front, ScalarExpr( front, tree ), T_EQ, IntLeaf( front, 0 ) );
    }
    tree->u.expr_type = GetType( front, TYP_INT );
    return( tree );
}


static TREEPTR StartTernary( FrontRun& front, TREEPTR tree )
{
    tree = BoolExpr( front, tree );
    return( tree );
}


static TREEPTR ColonOp( FrontRun& front, TREEPTR true_part )
{
    if( front.cvars.Class[front.cvars.ExprLevel] != TC_TERNARY ) {
        CErr1( front, ERR_MISSING_QUESTION_OR_MISPLACED_COLON );
        true_part = ErrorNode( front, NULL );
    }
    return( true_part );
}


static TREEPTR TernOp( FrontRun& front, TREEPTR expr1, TREEPTR true_part, TREEPTR false_part )
{
    TYPEPTR     typ1, typ2, result_typ;
    TREEPTR     tree;
    op_flags    ops;

    if( true_part->op.opr == OPR_ERROR ) {
        FreeExprTree( front, false_part );
        return( ErrorNode( front, expr1 ) );
    }
    if( false_part->op.opr == OPR_ERROR ) {
        FreeExprTree( front, true_part );
        return( ErrorNode( front, expr1 ) );
    }
    typ1 = true_part->u.expr_type;
    typ2 = false_part->u.expr_type;
    if( !IsStruct( typ1 ) ) {
        true_part = VoidRValue( front, true_part );
        typ1 = true_part->u.expr_type;
    }
    if( !IsStruct( typ2 ) ) {
        false_part = VoidRValue( front, false_part );
        typ2 = false_part->u.expr_type;
    }
    if( typ1->decl_type == TYP_VOID
      || typ2->decl_type == TYP_VOID ) {
        if( typ1->decl_type != typ2->decl_type ) {
            CErr1( front, ERR_TYPE_MISMATCH );
        }
        result_typ = typ1;
    } else {
        result_typ = TernType( front, true_part, false_part );
    }
    if( expr1->op.opr == OPR_PUSHINT ) {
        if( CheckZeroConstant( front, expr1 ) ) {
            FreeExprTree( front, true_part );
            tree = false_part;
        } else {
            FreeExprTree( front, false_part );
            tree = true_part;
        }
        FreeExprNode( front, expr1 );
    } else {
        tree = ExprNode( front, true_part, OPR_COLON, false_part );
        tree->op.u2.label_index = NextLabel(front);
        NextLabel(front);                            /* need 2 labels */
        tree = ExprNode( front, expr1, OPR_QUESTION, tree );
        tree->op.u2.result_type = result_typ;
        ops = OPFLAG_NONE;
        if( !IsConstLeaf( true_part ) ) {
            ops |= (true_part->op.flags & OPFLAG_MEM_MODEL);
        }
        if( !IsConstLeaf( false_part ) ) {
            ops |= (true_part->op.flags & OPFLAG_MEM_MODEL);
        }
        tree->op.flags = ops;
    }
    tree->u.expr_type = result_typ;
    return( tree );
}


static TREEPTR UnaryPlus( FrontRun& front, TREEPTR tree )
{
    TYPEPTR     typ;

    tree = RValue( front, tree );
    typ = tree->u.expr_type;
    switch( typ->decl_type ) {
    case TYP_CHAR:
    case TYP_UCHAR:
    case TYP_ENUM:
    case TYP_SHORT:
    case TYP_USHORT:
    case TYP_FIELD:
    case TYP_UFIELD:
        tree = ExprNode( front, NULL, OPR_CONVERT, tree );
        tree->u.expr_type = GetType( front, TYP_INT );
        tree->op.u2.result_type = tree->u.expr_type;
        break;
    case TYP_ARRAY:
    case TYP_POINTER:
    case TYP_STRUCT:
    case TYP_UNION:
    case TYP_FUNCTION:
    case TYP_VOID:
        CErr1( front, ERR_EXPR_MUST_BE_SCALAR );
        tree = ErrorNode( front, tree );
        break;
    }
    return( tree );
}


TREEPTR ScalarExpr( FrontRun& front, TREEPTR tree )
{
    TYPEPTR     typ;

    if( tree->op.opr == OPR_ERROR )
        return( tree );
    tree = RValue( front, tree );
    typ = tree->u.expr_type;
    switch( typ->decl_type ) {
    case TYP_STRUCT:
    case TYP_UNION:
    case TYP_FUNCTION:
    case TYP_VOID:
        CErr1( front, ERR_EXPR_MUST_BE_SCALAR );
        tree = ErrorNode( front, tree );
        break;
    }
    return( tree );
}


static TREEPTR SizeofOp( FrontRun& front, TYPEPTR typ )
{
    TREEPTR             tree;
    target_size         size;

    if( typ->decl_type == TYP_FIELD
      || typ->decl_type == TYP_UFIELD ) {
        CErr1( front, ERR_CANT_TAKE_SIZEOF_FIELD );
    }
    size = SizeOfArg( front, typ );
    if( size == 0 ) {
        SKIP_TYPEDEFS( typ );
        if( typ->decl_type == TYP_VOID ) {
            CErr1( front, ERR_EXPR_HAS_VOID_TYPE );
        } else {
            SetDiagType1( front, typ );
            CErr1( front, ERR_INCOMPLETE_EXPR_TYPE );
            SetDiagPop(front);
        }
    }
#if TARGET_INT < TARGET_LONG
    if( size > TARGET_UINT_MAX ) {
        tree = LongLeaf( front, size );
    } else {
        tree = UIntLeaf( front, size );
    }
#else
    tree = UIntLeaf( front, size );
#endif
    return( tree );
}

static TREEPTR SegOp( FrontRun& front, TREEPTR seg, TREEPTR offset )
{
    TREEPTR     tree;
    TYPEPTR     typ;

    typ = offset->u.expr_type->object;
    if( typ == NULL )
        typ = GetType( front, TYP_VOID );
    tree = ExprNode( front, RValue(front, seg), OPR_FARPTR, RValue(front, offset) );
    tree->u.expr_type = PtrNode( front, typ, FLAG_FAR, SEG_NULL );
    return( tree );
}
