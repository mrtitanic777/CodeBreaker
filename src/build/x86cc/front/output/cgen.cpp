/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2025 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Interface to Optimizing code generator.
*
****************************************************************************/


#include "cvars.h"
#include "langenv.h"
#include "jmpbuf.h"
#include "i64.h"
#include "cg.h"
#include "cgdefs.h"
#include "cgswitch.h"
#include "cgcli.h"
#include "cgprotos.h"
#include "feprotos.h"
#include "cgen.h"
#include "cfeinfo.h"
#ifdef __SEH__
#include "tryblock.h"
#endif
#include "frontrun.hpp"


#define PushCGName(name)        front.cvars.ValueStack[index++] = (TREEPTR)name
#define PopCGName()             (cg_name)front.cvars.ValueStack[--index]



#ifdef __SEH__

#endif


namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct CgenState: cgen_state.h) */

}  // namespace



struct func_save {
    SYMPTR          func;
    SYM_HANDLE      func_handle;
    OPNODE          *funcnode;
    label_handle    *cglabel_handles;
    LABEL_INDEX     labelindex;
};

/*
 * matches table of type in ctypes.h
 */
static  char    const CGDataType[] = {
    #define pick1(type,dtype,cgtype,x86asmtype,name,size) cgtype,
    #include "cdatatyp.h"
    #undef  pick1
};

static  cg_op   const CGOperator[] = {
    #define pick1(enum,dump,cgenum) cgenum,
    #include "copcodes.h"
    #undef pick1
};

static  cg_op   const CC2CGOp[] = {
    #define pick1(enum,dump,cgenum) cgenum,
    #include "copcond.h"
    #undef pick1
};


static void DefineLabels( FrontRun& front, OPNODE *node )
{
    LABEL_INDEX i;

    front.cvars.LabelIndex = node->u2.label_count;
    if( front.cvars.LabelIndex != 0 ) {
        front.cgen.CGLabelHandles = (label_handle *)CMemAlloc( front, ( front.cvars.LabelIndex + 1 ) * sizeof( label_handle ) );
        for( i = 1; i <= front.cvars.LabelIndex; i++ ) {
            front.cgen.CGLabelHandles[i] = BENewLabel(*front.back);
        }
    }
}

static local_vars *ReleaseVars( FrontRun& front, SYM_HANDLE sym_list, local_vars *local_var_list )
{
    local_vars  *local_entry;

    local_entry = (local_vars *)CMemAlloc( front, sizeof( local_vars ) );
    local_entry->next = local_var_list;
    local_entry->sym_list = sym_list;
    return( local_entry );
}

static void FreeSymBackInfo( FrontRun& front, SYMPTR sym, SYM_HANDLE sym_handle )
{
    if( sym->u1.backinfo != NULL ) {
        BEFiniBack( *front.back, sym->u1.backinfo );
        BEFreeBack( *front.back, sym->u1.backinfo );
        sym->u1.backinfo = NULL;
        SymReplace( front, sym, sym_handle );
    }
}

static void FreeLocalVars( FrontRun& front, SYM_HANDLE sym_list )
{
    SYM_HANDLE          sym_handle;
    SYM_ENTRY           sym;

    for( sym_handle = sym_list; sym_handle != SYM_NULL; sym_handle = sym.handle) {
        SymGet( front, &sym, sym_handle );
        if( sym.attribs.stg_class != SC_EXTERN ) {
            if( (sym.flags & SYM_FUNC_RETURN_VAR) == 0 ) {
                if( sym.sym_type->decl_type != TYP_VOID ) {
                    FreeSymBackInfo( front, &sym, sym_handle );
                }
            }
        }
    }
}


static void RelLocalVars( FrontRun& front, local_vars *local_var_list )
{
    local_vars  *local_entry;

    while( (local_entry = local_var_list) != NULL ) {
        local_var_list = local_entry->next;
        FreeLocalVars( front, local_entry->sym_list );
        CMemFree( front, local_entry );
    }
}

static cg_type ReturnType( FrontRun& front, cg_type type )
{
    if( front.cvars.CompFlags.returns_promoted && (front.cgen.InLineDepth == 0) ) {
        type = FEParmType( front, NULL, NULL, type );
    }
    return( type );
}

#ifdef __SEH__
static void FreeTrySymBackInfo( FrontRun& front )
{
    SYM_ENTRY   sym;

    SymGet( front, &sym, front.cvars.TrySymHandle );
    FreeSymBackInfo( front, &sym, front.cvars.TrySymHandle );
}

static void FreeTryTableBackHandles( FrontRun& front )
{
    try_table_back_handles  *try_backinfo;

    for( ; (try_backinfo = front.cgen.TryTableBackHandles) != NULL; ) {
        front.cgen.TryTableBackHandles = try_backinfo->next;
        BEFreeBack( *front.back, try_backinfo->back_handle );
        CMemFree( front, try_backinfo );
    }
}

static void CallTryRtn( FrontRun& front, SYM_HANDLE try_rtn, cg_name parm )
{
    call_handle call;

    call = CGInitCall( *front.back, CGFEName( *front.back, (CGSYM_HANDLE)try_rtn, TY_POINTER ), TY_INTEGER, (CGSYM_HANDLE)try_rtn );
    CGAddParm( *front.back, call, parm, TY_POINTER );
    CGDone( *front.back, CGCall( call ) );
}

static void CallTryInit( FrontRun& front )
{
    CallTryRtn( front, front.cvars.SymTryInit, CGFEName( *front.back, (CGSYM_HANDLE)front.cvars.TrySymHandle, TY_POINTER ) );
}

static cg_name TryFieldAddr( FrontRun& front, unsigned offset )
{
    cg_name     name;

    name = CGFEName( *front.back, (CGSYM_HANDLE)front.cvars.TrySymHandle, static_cast<cg_type>(front.cgen.TryRefno) );
    name = CGBinary( *front.back, O_PLUS, name, CGInteger( *front.back, offset, TY_UNSIGNED ), TY_POINTER );
    name = CGVolatile( name );
    return( name );
}

static void CallTryFini( FrontRun& front )
{
    cg_name     name;

    name = TryFieldAddr( front, FLD_next );
    name = CGUnary( *front.back, O_POINTS, name, TY_POINTER );
    CallTryRtn( front, front.cvars.SymTryFini, name );
}
#endif

static void EndFunction( FrontRun& front, OPNODE *node )
{
    LABEL_INDEX i;
    cg_name     name;
    cg_type     dtype;
    SYM_ENTRY   sym;

    if( front.cvars.LabelIndex != 0 ) {
        for( i = 1; i <= front.cvars.LabelIndex; i++ ) {
            BEFiniLabel( *front.back, front.cgen.CGLabelHandles[i] );
        }
        CMemFree( front, front.cgen.CGLabelHandles );     // free labels for the function
        front.cgen.CGLabelHandles = NULL;
    }
#ifdef __SEH__
    if( front.cgen.FuncNodePtr->u2.func.flags & FUNC_USES_SEH ) {
        CallTryFini(front);          // generate call to __TryFini
        FreeTrySymBackInfo(front);
    }
#endif
    if( node->u2.sym_handle == SYM_NULL ) {
        dtype = CGenType( front, front.cvars.CurFunc->sym_type->object );
        CGReturn( *front.back, NULL, dtype );
    } else {                            // return value
        SymGet( front, &sym, node->u2.sym_handle );
        dtype = CGenType( front, sym.sym_type );
        name = CGTempName( *front.back, sym.u1.return_var, dtype );
        name = CGUnary( *front.back, O_POINTS, name, dtype );
        CGReturn( *front.back, name, ReturnType( front, dtype ) );
    }
    FreeLocalVars( front, front.cvars.CurFunc->u.func.parms );
    FreeLocalVars( front, front.cvars.CurFunc->u.func.locals );
    if( front.cgen.InLineDepth == 0 ) {
        RelLocalVars( front, front.cgen.LocalVarList );
    }
    if( front.cvars.GenSwitches & CGSW_GEN_DBG_LOCALS ) {
        if( front.cgen.InLineDepth != 0 ) {
            DBEndBlock(*front.back);
        }
    }
    front.cgen.FuncNodePtr->u2.func.flags &= ~FUNC_INUSE;
}

static void ReturnExpression( FrontRun& front, OPNODE *node, cg_name expr )
{
    cg_name     name;
    cg_type     dtype;
    SYM_ENTRY   sym;

    SymGet( front, &sym, node->u2.sym_handle );
    dtype = CGenType( front, sym.sym_type );
    name = CGTempName( *front.back, sym.u1.return_var, dtype );
    CGDone( *front.back, CGAssign( *front.back, name, expr, dtype ) );
}

static cg_type DataPointerType( OPNODE *node )
{
    cg_type     dtype;

    if( Far16Pointer( node->flags ) ) {
        dtype = TY_POINTER;
    } else if( node->flags & OPFLAG_NEARPTR ) {
        dtype = TY_NEAR_POINTER;
    } else if( node->flags & OPFLAG_FARPTR ) {
        dtype = TY_LONG_POINTER;
    } else if( node->flags & OPFLAG_HUGEPTR ) {
        dtype = TY_HUGE_POINTER;
    } else {
        dtype = TY_POINTER;
    }
    return( dtype );
}

static cg_name ForceVolatileFloat( FrontRun& front, cg_name name, TYPEPTR typ )
{
    if( front.cvars.CompFlags.op_switch_used ) {
        if( typ->decl_type == TYP_FLOAT || typ->decl_type == TYP_DOUBLE || typ->decl_type == TYP_LONG_DOUBLE ) {
            name = CGVolatile( name );
        }
    }
    return( name );
}


static cg_name PushSymSeg( FrontRun& front, OPNODE *node )
{
    cg_name     segname;
//    SYMPTR      sym;
    SYM_HANDLE  sym_handle;

    sym_handle = node->u2.sym_handle;
    if( sym_handle == SYM_NULL ) {
        segname = CGInteger( *front.back, 0, TY_UNSIGNED );
    } else {
        if( sym_handle == front.cvars.Sym_CS ) {
            segname = CGFEName( *front.back, (CGSYM_HANDLE)front.cvars.CurFuncHandle, TY_LONG_CODE_PTR );
        } else if( sym_handle == front.cvars.Sym_SS ) {
            if( front.cgen.SSVar == NULL ) {
                front.cgen.SSVar = CGTemp( *front.back, TY_UINT_2 );
            }
            segname = CGTempName( *front.back, front.cgen.SSVar, TY_UINT_2 );
        } else {
//            sym = SymGetPtr( sym_handle );
            segname = CGFEName( *front.back, (CGSYM_HANDLE)sym_handle, TY_UINT_2 );
        }
    }
    return( segname );
}

#ifdef __SEH__
static cg_name TryExceptionInfoAddr( FrontRun& front )
{
    cg_name     name;

    name = TryFieldAddr( front, FLD_exception_info );
//    name = CGUnary( O_POINTS, name, TY_POINTER );
//    name = CGUnary( O_POINTS, name, TY_POINTER );
    return( name );
}

static void SetTryTable( FrontRun& front, BACK_HANDLE except_table )
{
    cg_name     name;
    cg_name     table;

    name = TryFieldAddr( front, FLD_scope_table );
    table = CGBackName( *front.back, except_table, TY_POINTER );
    CGDone( *front.back, CGAssign( *front.back, name, table, TY_POINTER ) );
}

static void SetTryScope( FrontRun& front, tryindex_t scope )
{
    cg_name     name;

    name = TryFieldAddr( front, FLD_scope_index );
    CGDone( *front.back, CGAssign( *front.back, name, CGInteger( *front.back, scope, TY_UINT_1 ), TY_UINT_1 ) );
}

static void EndFinally( FrontRun& front )
{
    cg_name      name;
    cg_name      func;
    LABEL_HANDLE label_handle;
    call_handle  call;

    label_handle = BENewLabel(*front.back);
    name = TryFieldAddr( front, FLD_unwindflag );
    name = CGUnary( *front.back, O_POINTS, name, TY_UINT_1 );
    name = CGCompare( *front.back, O_EQ, name, CGInteger( *front.back, 0, TY_UINT_1 ), TY_UINT_1 );
    CGControl( *front.back, O_IF_TRUE, name, label_handle );
    func = CGFEName( *front.back, (CGSYM_HANDLE)front.cvars.SymFinally, TY_CODE_PTR );
    call = CGInitCall( *front.back, func, TY_INTEGER, (CGSYM_HANDLE)front.cvars.SymFinally );
    CGDone( *front.back, CGCall( call ) );
    CGControl( *front.back, O_LABEL, NULL, label_handle );
    BEFiniLabel( *front.back, label_handle );
}

static cg_name TryAbnormalTermination( FrontRun& front )
{
    cg_name      name;

    name = TryFieldAddr( front, FLD_unwindflag );
    name = CGUnary( *front.back, O_POINTS, name, TY_UINT_1 );
    return( name );
}

static void CallTryUnwind( FrontRun& front, tryindex_t scope_index )
{
    call_handle call;
    cg_name     parm;

    call = CGInitCall( *front.back, CGFEName( *front.back, (CGSYM_HANDLE)front.cvars.SymTryUnwind, TY_POINTER ), TY_INTEGER, (CGSYM_HANDLE)front.cvars.SymTryUnwind );
    parm = CGInteger( *front.back, scope_index, TY_UINT_1 );
    CGAddParm( *front.back, call, parm, TY_INTEGER );
    CGDone( *front.back, CGCall( call ) );
}

static void GenerateTryBlock( FrontRun& front, TREEPTR tree )
{
    TREEPTR     stmt;
    tryindex_t  try_index;
    tryindex_t  max_try_index;

    try_index = 0;
    max_try_index = TRYSCOPE_NONE;
    for( ; tree != NULL; tree = tree->left ) {
        stmt = tree->right;
        if( stmt->op.opr == OPR_FUNCEND )
            break;
        switch( stmt->op.opr ) {
        case OPR_TRY:
            try_index = stmt->op.u2.st.u.try_index;
            if( max_try_index < try_index )
                max_try_index = try_index;
            break;
        case OPR_EXCEPT:
        case OPR_FINALLY:
            front.cvars.ValueStack[try_index] = stmt;
            break;
        default:
            break;
        }
    }
    if( max_try_index != TRYSCOPE_NONE ) {
        segment_id              old_segid;
        BACK_HANDLE             except_label;
        BACK_HANDLE             except_table;
        try_table_back_handles  *try_backinfo;

        old_segid = BESetSeg( *front.back, SEG_DATA );
        except_table = BENewBack( *front.back, NULL );
        try_backinfo = (try_table_back_handles *)CMemAlloc( front, sizeof( try_table_back_handles ) );
        try_backinfo->back_handle = except_table;
        try_backinfo->next = front.cgen.TryTableBackHandles;
        front.cgen.TryTableBackHandles = try_backinfo;
        DGLabel( *front.back, except_table );
        for( try_index = 0; try_index <= max_try_index; try_index++ ) {
            stmt = front.cvars.ValueStack[try_index];
            DGInteger( *front.back, stmt->op.u2.st.parent_scope, TY_UINT_1 );  // parent index
            if( stmt->op.opr == OPR_EXCEPT ) {
                DGInteger( *front.back, 0, TY_UINT_1 );
            } else {
                DGInteger( *front.back, 1, TY_UINT_1 );
            }
            except_label = FEBack( front, (CGSYM_HANDLE)stmt->op.u2.st.u.try_sym_handle );
            DGBackPtr( *front.back, except_label, FESegID( front, (CGSYM_HANDLE)front.cvars.CurFuncHandle ), 0, TY_CODE_PTR );
        }
        BESetSeg( *front.back, old_segid );
        SetTryTable( front, except_table );
        BEFiniBack( *front.back, except_table );
    }
}
#endif  /* __SEH__ */


static cg_type CodePtrType( type_modifiers flags )
{
    cg_type     dtype;

    if( flags & FLAG_FAR ) {
        dtype = TY_LONG_CODE_PTR;
    } else if( flags & FLAG_NEAR ) {
        dtype = TY_NEAR_CODE_PTR;
    } else {
        dtype = TY_CODE_PTR;
    }
    return( dtype );
}

static cg_name PushSym( FrontRun& front, OPNODE *node )
{
    cg_name     name;
    cg_type     dtype;
    TYPEPTR     typ;
    SYM_ENTRY   sym;

    SymGet( front, &sym, node->u2.sym_handle );
    typ = sym.sym_type;
    if( sym.flags & SYM_FUNCTION ) {
        dtype = CodePtrType( sym.mods );
    } else {
        dtype = CGenType( front, typ );
    }
    if( sym.flags & SYM_FUNC_RETURN_VAR ) {
        name = CGTempName( *front.back, sym.u1.return_var, dtype );
    } else {
        name = CGFEName( *front.back, (CGSYM_HANDLE)node->u2.sym_handle, dtype );
    }
    if( node->flags & OPFLAG_UNALIGNED ) {
        name = CGAttr( name, CG_SYM_UNALIGNED );
    }
    if( node->flags & OPFLAG_VOLATILE ) {
        name = CGVolatile( name );
    } else {
        name = ForceVolatileFloat( front, name, typ );
    }
    name = CGUnary( *front.back, O_POINTS, name, dtype );
    return( name );
}

static cg_name PushSymAddr( FrontRun& front, OPNODE *node )
{
    cg_name     name;
    cg_type     dtype;
    SYM_ENTRY   sym;
    TYPEPTR     typ;

    SymGet( front, &sym, node->u2.sym_handle );
    typ = sym.sym_type;
    if( sym.flags & SYM_FUNCTION ) {
        dtype = CodePtrType( sym.mods );
    } else {
        dtype = CGenType( front, typ );
    }
    if( sym.flags & SYM_FUNC_RETURN_VAR ) {
        name = CGTempName( *front.back, sym.u1.return_var, dtype );
    } else {
        name = CGFEName( *front.back, (CGSYM_HANDLE)node->u2.sym_handle, dtype );
        if( sym.flags & SYM_USED_IN_PRAGMA ) {
            name = CGVolatile( name );
        }
    }
    return( name );
}

static cg_name PushRValue( FrontRun& front, OPNODE *node, cg_name name )
{
    TYPEPTR     typ;

    if( node->flags & OPFLAG_RVALUE ) {
        typ = node->u2.result_type;
        if( node->flags & OPFLAG_VOLATILE ) {
            name = CGVolatile( name );
        } else {
            name = ForceVolatileFloat( front, name, typ );
        }
        name = CGUnary( *front.back, O_POINTS, name, CGenType( front, typ ) );
    } else {
    }
    return( name );
}

static cg_name DotOperator( FrontRun& front, cg_name op1, OPNODE *node, cg_name op2 )
{
    TYPEPTR     typ;
    cg_name     name;
    unsigned    width;

    /*
     * node->u2.result_type is the type of the data
     * for the O_PLUS we want a pointer type
     */
    name = CGBinary( *front.back, O_PLUS, op1, op2, DataPointerType( node ) );
    typ = node->u2.result_type;
    if( typ->decl_type == TYP_FIELD || typ->decl_type == TYP_UFIELD ) {
        if( typ->u.f.field_type == TYP_BOOL ) {
            width = 1;
        } else {
            width = typ->u.f.field_width;
        }
        name = CGBitMask( *front.back, name, typ->u.f.field_start, width, CGenType( front, typ ) );
    }
    if( node->flags & OPFLAG_UNALIGNED ) {
        name = CGAttr( name, CG_SYM_UNALIGNED );
    }
    return( PushRValue( front, node, name ) );
}

static cg_name ArrowOperator( FrontRun& front, cg_name op1, OPNODE *node, cg_name op2 )
{
    /*
     * node->u2.result_type is the type of the data
     * for the O_PLUS we want a pointer type
     */
#if _CPU == 386
    if( Far16Pointer( node->flags ) ) {
        op1 = CGUnary( *front.back, O_PTR_TO_NATIVE, op1, TY_POINTER );
    }
#endif
    /*
     *  rvalue has already been done on left side of tree
     */
//    op1 = CGUnary( O_POINTS, op1, DataPointerType( node ) );
    return( DotOperator( front, op1, node, op2 ) );
}

static cg_name IndexOperator( FrontRun& front, cg_name op1, OPNODE *node, cg_name op2 )
{
    int         element_size;
    cg_type     index_type;

    /*
     * node->u2.result_type is the type of the data
     * for the O_PLUS we want a pointer type
     * op2 needs to be multiplied by the element size of the array
     */
#if _CPU == 386
    if( Far16Pointer( node->flags ) ) {
        op1 = CGUnary( *front.back, O_PTR_TO_NATIVE, op1, TY_POINTER );
    }
#endif
    element_size = SizeOfArg( front, node->u2.result_type );
    if( element_size != 1 ) {
        index_type = TY_INTEGER;
#if _CPU == 8086
        if( (node->flags & OPFLAG_HUGEPTR)
          || (IsHugeData() && (node->flags & (OPFLAG_NEARPTR | OPFLAG_FARPTR)) == 0) ) {
            index_type = TY_INT_4;
        }
#endif
        op2 = CGBinary( *front.back, O_TIMES, op2,
                CGInteger( *front.back, element_size, TY_INTEGER ), index_type );
    }
    op1 = CGBinary( *front.back, O_PLUS, op1, op2, DataPointerType( node ) );
    if( node->flags & OPFLAG_UNALIGNED ) {
        op1 = CGAttr( op1, CG_SYM_UNALIGNED );
    }
    return( PushRValue( front, node, op1 ) );
}

static cg_name DoAddSub( FrontRun& front, cg_name op1, OPNODE *node, cg_name op2 )
{
    cg_name     name;
    TYPEPTR     typ;

    typ = node->u2.result_type;
    SKIP_TYPEDEFS( typ );
    name = CGBinary( *front.back, CGOperator[node->opr], op1, op2, CGenType( front, typ ) );
    return( name );
}
static cg_name PushConstant( FrontRun& front, OPNODE *node )
{
    cg_name     name;
    cg_type     dtype;
    FLOATVAL    *flt;
    char        *flt_string;

    dtype = static_cast<cg_type>(CGDataType[node->u1.const_type]);
    switch( node->u1.const_type ) {
    case TYP_CHAR:
    case TYP_UCHAR:
    case TYP_SHORT:
    case TYP_USHORT:
    case TYP_INT:
    case TYP_UINT:
    case TYP_LONG:
    case TYP_ULONG:
        name = CGInteger( *front.back, node->u2.ulong_value, dtype );
        break;
    case TYP_POINTER:
        name = CGInteger( *front.back, node->u2.ulong_value, DataPointerType( node ) );
        break;
    case TYP_LONG64:
    case TYP_ULONG64:
        name = CGInt64( *front.back, node->u2.ulong64_value, dtype );
        break;
    case TYP_FLOAT:
    case TYP_DOUBLE:
    case TYP_LONG_DOUBLE:
    case TYP_FIMAGINARY:
    case TYP_DIMAGINARY:
    case TYP_LDIMAGINARY:
        flt = node->u2.float_value;
        if( flt->len != 0 ) {                   // if still in string form
            flt_string = flt->string;
        } else {                                // else in binary form
            flt_string = ftoa( front, flt );
        }
        name = CGFloat( *front.back, flt_string, dtype );
        break;
    default:
        assert( 0 );
        name = NULL;
        break;
    }
    return( name );
}

target_size EmitBytes( FrontRun& front, STR_HANDLE strlit )
{
    DGBytes( *front.back, strlit->length, strlit->literal );
    return( strlit->length );
}


static segment_id StringSegment( FrontRun& front, STR_HANDLE strlit )
{
    if( strlit->flags & STRLIT_FAR )
        return( front.cvars.FarStringSegId );
    if( strlit->flags & STRLIT_CONST )
        return( SEG_CODE );
    return( SEG_CONST );
}

static void EmitLiteral( FrontRun& front, STR_HANDLE strlit )
{
    segment_id  old_segid;

    old_segid = BESetSeg( *front.back, StringSegment( front, strlit ) );
    if( strlit->flags & STRLIT_WIDE ) {
        DGAlign( *front.back, TARGET_SHORT );    /* NT requires word aligned wide strings */
    }
    DGLabel( *front.back, strlit->back_handle );
    EmitBytes( front, strlit );
    BESetSeg( *front.back, old_segid );
}


static void Emit1String( FrontRun& front, STR_HANDLE str_handle )
{
    if( str_handle->back_handle == NULL ) {
        str_handle->back_handle = BENewBack( *front.back, NULL );
        if( (str_handle->flags & STRLIT_CONST) == 0 ) {
            EmitLiteral( front, str_handle );
        }
    }
}

static cg_name PushString( FrontRun& front, OPNODE *node )
{
    STR_HANDLE      string;

    string = node->u2.string_handle;
    Emit1String( front, string );
    return( CGBackName( *front.back, string->back_handle, TY_UINT_1 ) );
}

static void FreeStrings( FrontRun& front )
{
    STR_HANDLE      strlit;
    STR_HANDLE      strlit_next;
    str_hash_idx    hash;

    for( hash = 0; hash < STRING_HASH_SIZE; hash++ ) {
        for( strlit = front.cvars.StringHash[hash]; strlit != NULL; strlit = strlit_next ) {
            strlit_next = strlit->next_string;
            if( strlit->back_handle != NULL ) {
                BEFiniBack( *front.back, strlit->back_handle );
                BEFreeBack( *front.back, strlit->back_handle );
                strlit->back_handle = NULL;
            }
            FreeLiteral( front, strlit );
        }
        front.cvars.StringHash[hash] = NULL;
    }
}

static void DumpCS_Strings( FrontRun& front, STR_HANDLE strlit )
{
    for( ; strlit != NULL; strlit = strlit->next_string ) {
        if( (strlit->flags & STRLIT_CONST) && strlit->ref_count != 0 ) {
            strlit->back_handle = BENewBack( *front.back, NULL );
            EmitLiteral( front, strlit );
        }
    }
}


static void EmitCS_Strings( FrontRun& front )
{
    str_hash_idx    hash;

    if( front.cvars.CompFlags.strings_in_code_segment ) {
        for( hash = STRING_HASH_SIZE; hash-- > 0; ) {
            DumpCS_Strings( front, front.cvars.StringHash[hash] );
        }
    }
}

static cg_name DoIndirection( FrontRun& front, OPNODE *node, cg_name name )
{
    TYPEPTR     typ;

    /*
     * check for special kinds of pointers, eg. call __Far16ToFlat
     */
    typ = node->u2.result_type;
#if _CPU == 386
    if( Far16Pointer( node->flags ) ) {
        /*
         * Do NOT convert __far16 function pointers to flat because the
         * thunk routine expects 16:16 pointers!
         */
        if( ( typ->object != NULL ) && ( typ->object->decl_type != TYP_FUNCTION ) ) {
            name = CGUnary( *front.back, O_PTR_TO_NATIVE, name, TY_POINTER );
        }
    }
#endif
    if( node->flags & OPFLAG_UNALIGNED ) {
        name = CGAttr( name, CG_SYM_UNALIGNED );
    }
    if( node->flags & OPFLAG_RVALUE ) {
        if( node->flags & OPFLAG_VOLATILE ) {
            name = CGVolatile( name );
        }
        name = CGUnary( *front.back, O_POINTS, name, CGenType( front, typ->object ) );
    }
    return( name );
}

static cg_name ConvertPointer( FrontRun& front, OPNODE *node, cg_name name )
{
#if _CPU == 386
    if( FAR16_PTRCLASS( node->u2.sp.old_ptrclass ) ) {
        name = CGUnary( *front.back, O_PTR_TO_NATIVE, name, TY_POINTER );
    } else if( FAR16_PTRCLASS( node->u2.sp.new_ptrclass ) ) {
        name = CGUnary( *front.back, O_PTR_TO_FOREIGN, name, TY_POINTER );
    }
#else
    /* unused parameters */ (void)node;
#endif
    return( name );
}

static call_handle InitFuncCall( FrontRun& front, OPNODE *node )
{
    cg_name     name;
    cg_type     dtype;
    TYPEPTR     typ;
    SYMPTR      sym;

    sym = SymGetPtr( front, node->u2.sym_handle );
    typ = sym->sym_type;
    dtype = CGenType( front, typ );
    name = CGFEName( *front.back, (CGSYM_HANDLE)node->u2.sym_handle, dtype );
//    dtype = FESymType( sym );
    SKIP_TYPEDEFS( typ );
    return( CGInitCall( *front.back, name, CGenType( front, typ->object ), (CGSYM_HANDLE)node->u2.sym_handle ) );
}

static call_handle InitIndFuncCall( FrontRun& front, OPNODE *node, cg_name name )
{
    TYPEPTR     typ;
    SYMPTR      sym;

    sym = SymGetPtr( front, node->u2.sym_handle );
    typ = sym->sym_type;
    SKIP_TYPEDEFS( typ );
    return( CGInitCall( *front.back, name, CGenType( front, typ->object ), (CGSYM_HANDLE)node->u2.sym_handle ) );
}

static void DoSwitch( FrontRun& front, OPNODE *node, cg_name name )
{
    sel_handle  table;
    SWITCHPTR   sw;
    CASEPTR     ce;

    table = CGSelInit(*front.back);
    sw = node->u2.switch_info;
    for( ce = sw->case_list; ce != NULL; ce = ce->next_case ) {
        CGSelCase( *front.back, table, front.cgen.CGLabelHandles[ce->label], ce->value );
    }
    CGSelOther( table, front.cgen.CGLabelHandles[sw->default_label] );
    CGSelect( *front.back, table, name );
}

static bool IsStruct( TYPEPTR typ )
/*********************************/
{
    SKIP_TYPEDEFS( typ );
    if( typ->decl_type == TYP_STRUCT
      || typ->decl_type == TYP_FCOMPLEX
      || typ->decl_type == TYP_DCOMPLEX
      || typ->decl_type == TYP_LDCOMPLEX
      || typ->decl_type == TYP_UNION ) {
        return( true );
    }
    return( false );
}

static void ThreadNode( FrontRun& front, TREEPTR node )
{
    if( front.cgen.FirstNode == NULL )
        front.cgen.FirstNode = node;
    if( front.cgen.LastNode != NULL )
        front.cgen.LastNode->u.thread = node;
    front.cgen.LastNode = node;
}

static TREEPTR LinearizeTree( FrontRun& front, TREEPTR tree )
{
    front.cgen.FirstNode = NULL;
    front.cgen.LastNode = NULL;
    WalkExprTree( front, tree, ThreadNode, NoOp, NoOp, ThreadNode );
    front.cgen.LastNode->u.thread = NULL;
    return( front.cgen.FirstNode );
}


void EmitInit( FrontRun& front )
{
    front.cvars.SegListHead = NULL;
    ImportSegIdInit(front);
    front.cgen.Refno = TY_FIRST_FREE;
}


static int NewRefno( FrontRun& front )
{
    return( front.cgen.Refno++ );
}


void EmitAbort( void )
{
}

static void EmitSym( FrontRun& front, SYMPTR sym, SYM_HANDLE sym_handle )
{
    TYPEPTR             typ;
    segment_id          segid;
    target_size         size;

    typ = sym->sym_type;
    if( (front.cvars.GenSwitches & CGSW_GEN_DBG_TYPES) && (sym->attribs.stg_class == SC_TYPEDEF) ) {
        if( typ->decl_type != TYP_TYPEDEF ) {
            DBEndName( *front.back, DBBegName( *front.back, sym->name, DBG_NIL_TYPE ), DBType( front, typ ) );
        }
    }
    SKIP_TYPEDEFS( typ );
    CGenType( front, typ );    /* create refno for ARRAY type, etc */
    if( sym->attribs.stg_class != SC_EXTERN &&  /* if not imported */
        sym->attribs.stg_class != SC_TYPEDEF ) {
        if( ( sym->flags & SYM_FUNCTION ) == 0 ) {
            segid = sym->u.var.segid;
            if( (sym->flags & SYM_INITIALIZED) == 0 || segid == SEG_BSS) {
                BESetSeg( *front.back, segid );
                AlignIt( front, typ );
                DGLabel( *front.back, FEBack( front, (CGSYM_HANDLE)sym_handle ) );
                /*
                 * initialize all bytes to 0
                 * if size > 64k, have to break it into chunks of 64k
                 */
                size = SizeOfArg( front, typ );
                if( segid == SEG_BSS ) {
                    DGUBytes( *front.back, size );
                } else {
#if _CPU == 8086
                    while( size >= 0x10000 ) {
                        EmitZeros( front, 0x10000 );
                        size -= 0x10000;
                        if( size == 0 )
                            break;
                        if( segid != SEG_CONST && segid != SEG_DATA ) {
                            ++segid;
                            BESetSeg( *front.back, segid );
                        }
                    }
#endif
                    if( size != 0 ) {
                        EmitZeros( front, size );
                    }
                }
            }
        }
    }
}

static void CDoAutoDecl( FrontRun& front, SYM_HANDLE sym_handle )
{
    TYPEPTR             typ;
    cg_type             dtype;
    bool                emit_debug_info;
    bool                emit_extra_info;
    SYM_ENTRY           sym;

    for( ; sym_handle != SYM_NULL; sym_handle = sym.handle ) {
        SymGet( front, &sym, sym_handle );
        emit_debug_info = false;
        emit_extra_info = false;
        if( front.cvars.GenSwitches & CGSW_GEN_NO_OPTIMIZATION )
            emit_debug_info = true;
        if( sym.attribs.stg_class == SC_STATIC ) {
            emit_debug_info = false;
            if( (sym.flags & SYM_EMITTED) == 0 ) {
                if( sym.sym_type->decl_type != TYP_VOID ) {
                    EmitSym( front, &sym, sym_handle );
                    emit_debug_info = true;
                    SymGet( front, &sym, sym_handle );
                    sym.flags |= SYM_EMITTED;
                    SymReplace( front, &sym, sym_handle );
                }
            }
        } else if( sym.attribs.stg_class != SC_EXTERN &&
                   sym.attribs.stg_class != SC_TYPEDEF ) {
            if( sym.flags & SYM_ADDR_TAKEN ) {
                emit_extra_info = true;
            }
            typ = sym.sym_type;
            SKIP_TYPEDEFS( typ );
            switch( typ->decl_type ) {
            case TYP_UNION:
            case TYP_STRUCT:
            case TYP_ARRAY:
            case TYP_FCOMPLEX:
            case TYP_DCOMPLEX:
            case TYP_LDCOMPLEX:
                emit_extra_info = true;
                break;
            default:
                break;
            }
            dtype = CGenType( front, typ );
            if( sym.flags & SYM_FUNC_RETURN_VAR ) {
                sym.u1.return_var = CGTemp( *front.back, dtype );
                SymReplace( front, &sym, sym_handle );
            } else {
                CGAutoDecl( *front.back, (CGSYM_HANDLE)sym_handle, dtype );
            }
        }
        if( ! front.cvars.CompFlags.debug_info_some )
            emit_extra_info = false;
        if( emit_debug_info || emit_extra_info ) {
            if( front.cgen.Saved_CurFunc == SYM_NULL ) {  /* if we are not inlining */
                if( front.cvars.GenSwitches & CGSW_GEN_DBG_LOCALS ) {
                    if( (sym.flags & SYM_TEMP) == 0 ) {
                        DBLocalSym( *front.back, (CGSYM_HANDLE)sym_handle, TY_DEFAULT );
                    }
                }
            }
        }
    }
}


static void CDoParmDecl( FrontRun& front, SYMPTR sym, SYM_HANDLE sym_handle )
{
    TYPEPTR typ;
    cg_type dtype;

    typ = sym->sym_type;
    SKIP_TYPEDEFS( typ );
    dtype = CGenType( front, typ );
    CGParmDecl( *front.back, (CGSYM_HANDLE)sym_handle, dtype );
#if _CPU == 386
    if( (front.cvars.GenSwitches & CGSW_GEN_NO_OPTIMIZATION)
      || (!front.cvars.CompFlags.register_conventions && front.cvars.CompFlags.debug_info_some) ) {
#else
    if( front.cvars.GenSwitches & CGSW_GEN_NO_OPTIMIZATION ) {
#endif
        if( front.cvars.GenSwitches & CGSW_GEN_DBG_LOCALS ) {
            DBLocalSym( *front.back, (CGSYM_HANDLE)sym_handle, TY_DEFAULT );
        }
    }
}

static void ParmReverse( FrontRun& front, SYM_HANDLE sym_handle )
{
    SYMPTR      sym;

    sym = SymGetPtr( front, sym_handle );
    if( sym->handle ) {
        ParmReverse( front, sym->handle );
        sym = SymGetPtr( front, sym_handle );
    }
    CDoParmDecl( front, sym, sym_handle );
}

static bool DoFuncDefn( FrontRun& front, SYM_HANDLE funcsym_handle )
{
    bool        parms_reversed;
    SYM_HANDLE  sym_handle;
    cg_type     ret_type;

    front.cgen.SSVar = NULL;
    front.cvars.CurFunc = &front.cvars.CurFuncSym;
    SymGet( front, front.cvars.CurFunc, funcsym_handle );
    front.cvars.CurFuncHandle = funcsym_handle;
    if( ! front.cvars.CompFlags.zu_switch_used ) {
        if( front.cvars.CurFunc->mods & FLAG_FARSS ) {      /* function use far stack */
            front.cvars.TargetSwitches |= CGSW_X86_FLOATING_SS;
        } else {
            front.cvars.TargetSwitches &= ~ CGSW_X86_FLOATING_SS;
        }
    }
    ret_type = ReturnType( front, CGenType( front, front.cvars.CurFunc->sym_type->object ) );
    CGProcDecl( *front.back, (CGSYM_HANDLE)funcsym_handle, ret_type );
#if _CPU == 386
    if( front.cvars.TargetSwitches & CGSW_X86_P5_PROFILING ) {
        const char *fn_name;
        size_t     len;
        segment_id old_segid;

        fn_name = FEName( front, (CGSYM_HANDLE)funcsym_handle );
        len = strlen( fn_name ) + 1;
        old_segid = BESetSeg( *front.back, front.cvars.FunctionProfileSegId );
        front.cvars.FunctionProfileBlock = BENewBack( *front.back, NULL );
        DGLabel( *front.back, front.cvars.FunctionProfileBlock );
        DGInteger( *front.back, 0,   TY_INTEGER );
        DGInteger( *front.back, (unsigned)-1,  TY_INTEGER );
        DGInteger( *front.back, 0,   TY_INTEGER );
        DGInteger( *front.back, 0,   TY_INTEGER );
        DGBytes( *front.back, len, fn_name );
        len &= 0x03;
        if( len ) {
            DGIBytes( *front.back, 4 - len, 0 );
        }
        BESetSeg( *front.back, old_segid );
    }
#endif
    if( front.cvars.GenSwitches & CGSW_GEN_DBG_LOCALS ) {
        if( front.cgen.InLineDepth == 0 ) {
            DBModSym( *front.back, (CGSYM_HANDLE)front.cvars.CurFuncHandle, TY_DEFAULT );
        } else {
            DBBegBlock(*front.back);
        }
    }
    parms_reversed = false;
    if( front.cvars.CurFunc->u.func.parms != SYM_NULL ) {
        SYMPTR              psym;
        aux_info            *inf;
        SYM_ENTRY           sym;

        inf = FindInfo( front, &sym, front.cvars.CurFuncHandle );
        if( inf->cclass & FECALL_GEN_REVERSE_PARMS ) {
            ParmReverse( front, front.cvars.CurFunc->u.func.parms );
            parms_reversed = true;
        } else {
            for( sym_handle = front.cvars.CurFunc->u.func.parms; sym_handle != SYM_NULL; sym_handle = psym->handle ) {
                psym = SymGetPtr( front, sym_handle );
                if( psym->sym_type->decl_type == TYP_DOT_DOT_DOT )
                    break;
                CDoParmDecl( front, psym, sym_handle );
            }
        }
    }
    CGLastParm(*front.back);
    CDoAutoDecl( front, front.cvars.CurFunc->u.func.locals );
#ifdef __SEH__
    if( front.cgen.FuncNodePtr->u2.func.flags & FUNC_USES_SEH ) {
        CGAutoDecl( *front.back, (CGSYM_HANDLE)front.cvars.TrySymHandle, static_cast<cg_type>(front.cgen.TryRefno) );
        CallTryInit(front);                  // generate call to __TryInit
    }
#endif
    return( parms_reversed );
}

static void StartFunction( FrontRun& front, OPNODE *node )
{
    front.cgen.FuncNodePtr = node;
    if( front.cgen.InLineDepth == 0 ) {
        front.cgen.LocalVarList = NULL;
    }

    front.cgen.FuncNodePtr->u2.func.flags |= FUNC_INUSE;
    DoFuncDefn( front, node->u2.func.sym_handle );
}

static void EmitNodes( FrontRun& front, TREEPTR tree )
{
    cg_name     op1;
    cg_name     op2;
    cg_name     expr;
    call_handle call;
    int         index;
    OPNODE      *node;

    index = 0;
    for( ; tree != NULL; tree = tree->u.thread ) {
        node = &tree->op;
        switch( node->opr ) {
        case OPR_FUNCTION:              // start of function
            StartFunction( front, node );
            break;
        case OPR_NEWBLOCK:              // start of new block { vars; }
            DBBegBlock(*front.back);
            CDoAutoDecl( front, node->u2.sym_handle );
            break;
        case OPR_ENDBLOCK:              // end of new block { vars; }
            DBEndBlock(*front.back);
            front.cgen.LocalVarList = ReleaseVars( front, node->u2.sym_handle, front.cgen.LocalVarList );
            break;
        case OPR_LABELCOUNT:            // number of labels used in function
            DefineLabels( front, node );
            break;
        case OPR_RETURN:                // return( expression );
            op1 = PopCGName();          // - get expression
            ReturnExpression( front, node, op1 );
            break;
        case OPR_FUNCEND:               // end of function
            EndFunction( front, node );
            break;
        case OPR_EQUALS:                // =
            op2 = PopCGName();          // get right opnd
            op1 = PopCGName();          // get lvalue
            if( node->flags & OPFLAG_VOLATILE ) { // is lvalue volatile
                 op1 = CGVolatile( op1 );
            }
            if( IsStruct( node->u2.result_type ) ) {
                op1 = CGLVAssign( *front.back, op1, op2, CGenType( front, node->u2.result_type ) );
                op1 = PushRValue( front, node, op1 );
            } else {
                op1 = CGAssign( *front.back, op1, op2, CGenType( front, node->u2.result_type ) );
            }
            PushCGName( op1 );
            break;
        case OPR_ADD:                   // +
        case OPR_SUB:                   // -
            op2 = PopCGName();
            op1 = PopCGName();
            PushCGName( DoAddSub( front, op1, node, op2 ) );
            break;
        case OPR_MUL:                   // *
        case OPR_DIV:                   // /
        case OPR_MOD:                   // %
        case OPR_OR:                    // |
        case OPR_AND:                   // &
        case OPR_XOR:                   // ^
        case OPR_RSHIFT:                // >>
        case OPR_LSHIFT:                // <<
            op2 = PopCGName();
            op1 = PopCGName();
            PushCGName( CGBinary( *front.back, CGOperator[node->opr], op1, op2,
                                    CGenType( front, node->u2.result_type ) ) );
            break;
        case OPR_COMMA:                 // expr , expr
          {
            cg_type     dtype;
            op2 = PopCGName();
            op1 = PopCGName();
            dtype =  IsStruct( node->u2.result_type ) ?
                   DataPointerType( node ) : CGenType( front, node->u2.result_type );
            op1 = CGBinary( *front.back, O_COMMA, op1, op2, dtype );
            PushCGName( PushRValue( front, node, op1 ) );
          } break;
        case OPR_PLUS_EQUAL:
        case OPR_MINUS_EQUAL:
        case OPR_TIMES_EQUAL:
        case OPR_DIV_EQUAL:
        case OPR_MOD_EQUAL:
        case OPR_XOR_EQUAL:
        case OPR_AND_EQUAL:
        case OPR_OR_EQUAL:
        case OPR_RSHIFT_EQUAL:
        case OPR_LSHIFT_EQUAL:
            op2 = PopCGName();          // get right opnd
            op1 = PopCGName();          // get lvalue
            if( node->flags & OPFLAG_VOLATILE ) { // is lvalue volatile
                 op1 = CGVolatile( op1 );
            }
            PushCGName( CGPreGets( *front.back, CGOperator[node->opr], op1, op2,
                                    CGenType( front, node->u2.result_type ) ) );
            break;
        case OPR_NEG:                   // negate
            op1 = PopCGName();
            PushCGName( CGUnary( *front.back, O_UMINUS, op1,
                                    CGenType( front, node->u2.result_type ) ) );
            break;
        case OPR_CMP:                   // compare
            op2 = PopCGName();
            op1 = PopCGName();
            PushCGName( CGCompare( *front.back, CC2CGOp[node->u1.cc], op1, op2,
                                    CGenType( front, node->u2.compare_type ) ) );
            break;
        case OPR_COM:                   // ~
            op1 = PopCGName();
            PushCGName( CGUnary( *front.back, O_COMPLEMENT, op1,
                                    CGenType( front, node->u2.result_type ) ) );
            break;
        case OPR_NOT:                   // !
            op1 = PopCGName();
            PushCGName( CGFlow( *front.back, O_FLOW_NOT, op1, NULL ) );
            break;
        case OPR_QUESTION:             // ?
          {
            cg_type     dtype;

            op2 = PopCGName();          // false_part
            op1 = PopCGName();          // true_part
            expr = PopCGName();         // test expr
            dtype =  IsStruct( node->u2.result_type ) ?
                   DataPointerType( node ) : CGenType( front, node->u2.result_type );
            op1 = CGChoose( *front.back, expr, op1, op2, dtype );

            PushCGName( PushRValue( front, node, op1 ) );
          } break;
        case OPR_COLON:                 // :
            /*
             * do nothing, wait for OPR_QUESTION to come along
             */
            break;
        case OPR_OR_OR:                 // ||
        case OPR_AND_AND:               // &&
            op2 = PopCGName();
            op1 = PopCGName();
            PushCGName( CGFlow( *front.back, CGOperator[node->opr], op1, op2 ) );
            break;
        case OPR_POINTS:                // *ptr
            op1 = PopCGName();
            PushCGName( DoIndirection( front, node, op1 ) );
            break;
        case OPR_POSTINC:               // lvalue++
        case OPR_POSTDEC:               // lvalue--
            op2 = PopCGName();          // get amount to inc/dec by
            op1 = PopCGName();          // get lvalue
            if( node->flags & OPFLAG_VOLATILE ) { // is lvalue volatile
                 op1 = CGVolatile( op1 );
            }
            PushCGName( CGPostGets( *front.back, CGOperator[node->opr], op1, op2,
                                    CGenType( front, node->u2.result_type ) ) );
            break;
        case OPR_PUSHSYM:               // push sym_handle
            PushCGName( PushSym( front, node ) );
            break;
        case OPR_PUSHADDR:              // push address of sym_handle
            PushCGName( PushSymAddr( front, node ) );
            break;
        case OPR_PUSHINT:               // push integer constant
        case OPR_PUSHFLOAT:             // push floating-point constant
            PushCGName( PushConstant( front, node ) );
            break;
        case OPR_PUSHSTRING:            // push address of string literal
            PushCGName( PushString( front, node ) );
            break;
        case OPR_PUSHSEG:               // push seg of sym
            PushCGName( PushSymSeg( front, node ) );
            break;
        case OPR_DUPE:               // push seg of sym
         {
            temp_handle temp_name;
            cg_name     name;
            cg_name     left;

            op1 = PopCGName();      // - get expression
            temp_name = CGTemp( *front.back, TY_LONG_POINTER );
            name = CGTempName( *front.back, temp_name, TY_LONG_POINTER );
            left = CGAssign( *front.back, name, op1, TY_LONG_POINTER );
            name = CGTempName( *front.back, temp_name, TY_LONG_POINTER );
            left = CGEval( *front.back, left );  // force order - temp_name used on both sides
            PushCGName( left );
            name = CGUnary( *front.back, O_POINTS, name, TY_LONG_POINTER );
            PushCGName( CGUnary( *front.back, O_POINTS, name, TY_LONG_POINTER ) );
          } break;
        case OPR_CONVERT:
            if( node->u2.result_type->decl_type != TYP_VOID ) {
                op1 = PopCGName();      // - get expression
                PushCGName( CGUnary( *front.back, O_CONVERT, op1, CGenType( front, node->u2.result_type )) );
            }
            break;
        case OPR_CONVERT_PTR:           // convert pointer
            op1 = PopCGName();          // - get expression
            PushCGName( ConvertPointer( front, node, op1 ) );
            break;
        case OPR_CONVERT_SEG:           // convert pointer to segment
          {
            cg_name     name;

            op1 = PopCGName();          // - get expression
            name = CGUnary( *front.back, O_CONVERT, op1, TY_LONG_POINTER );
            name = CGUnary( *front.back, O_CONVERT, name, TY_UINT_4 );
            name = CGBinary( *front.back, O_RSHIFT, name,
                             CGInteger( *front.back, 16, TY_UNSIGNED ), TY_UINT_4 );
            PushCGName( name );
          } break;
        case OPR_MATHFUNC:              // intrinsic math func with 1 parm
            op1 = PopCGName();          // - get expression
            PushCGName( CGUnary( *front.back, static_cast<cg_op>(node->u1.mathfunc), op1, TY_DOUBLE ) );
            break;
        case OPR_MATHFUNC2:             // intrinsic math func with 2 parms
            op2 = PopCGName();          // - get expression
            op1 = PopCGName();          // - get expression
            PushCGName( CGBinary( *front.back, static_cast<cg_op>(node->u1.mathfunc), op1, op2, TY_DOUBLE ) );
            break;
        case OPR_DOT:                   // sym.field
            op2 = PopCGName();          // - get offset of field
            op1 = PopCGName();          // - get address of sym
            PushCGName( DotOperator( front, op1, node, op2 ) );
            break;
        case OPR_ARROW:                 // ptr->field
            op2 = PopCGName();          // - get offset of field
            op1 = PopCGName();          // - get pointer
            PushCGName( ArrowOperator( front, op1, node, op2 ) );
            break;
        case OPR_INDEX:                 // array[index]
            op2 = PopCGName();          // - get index
            op1 = PopCGName();          // - get address of array
            PushCGName( IndexOperator( front, op1, node, op2 ) );
            break;
        case OPR_ADDROF:                // & expr
        case OPR_NOP:                   // nop
            break;
        case OPR_FARPTR:                // segment :> offset
            op2 = PopCGName();          // - get offset
            op1 = PopCGName();          // - get segment
            PushCGName( CGBinary( *front.back, O_CONVERT, op2, op1, TY_LONG_POINTER ) );
            break;
        case OPR_FUNCNAME:              // function name
            PushCGName( InitFuncCall( front, node ) ); // - push call_handle
            break;
        case OPR_CALL_INDIRECT:         // indirect function call
            op1 = PopCGName();                          // - get name
            PushCGName( InitIndFuncCall( front, node, op1 ) ); // - push call_handle
            break;
        case OPR_CALL:                  // function call
            call = PopCGName();         // - get call_handle
            op1 = CGCall( call );
            if( node->flags & OPFLAG_RVALUE ) {
                op1 = CGUnary( *front.back, O_POINTS, op1, CGenType( front, node->u2.result_type ) );
            }
            PushCGName( op1 );
            break;
        case OPR_PARM:                  // function parm
            op1 = PopCGName();          // - get parm
            call = PopCGName();         // - get call_handle
            CGAddParm( *front.back, call, op1, CGenType( front, node->u2.result_type ) );
            PushCGName( call );
            break;
        case OPR_LABEL:                 // label
            CGControl( *front.back, O_LABEL, NULL, front.cgen.CGLabelHandles[node->u2.label_index] );
            break;
        case OPR_CASE:                  // case label
            if( node->u2.case_info->gen_label ) {
                CGControl( *front.back, O_LABEL, NULL, front.cgen.CGLabelHandles[node->u2.case_info->label] );
            }
            break;
        case OPR_JUMP:                  // jump
            CGControl( *front.back, O_GOTO, NULL, front.cgen.CGLabelHandles[node->u2.label_index] );
            break;
        case OPR_JUMPTRUE:              // jump if true
            op1 = PopCGName();
            CGControl( *front.back, O_IF_TRUE, op1, front.cgen.CGLabelHandles[node->u2.label_index] );
            break;
        case OPR_JUMPFALSE:             // jump if false
            op1 = PopCGName();
            CGControl( *front.back, O_IF_FALSE, op1, front.cgen.CGLabelHandles[node->u2.label_index] );
            break;
        case OPR_SWITCH:                // switch
            op1 = PopCGName();
            DoSwitch( front, node, op1 );
            break;
#ifdef __SEH__
        case OPR_TRY:                   // start of try block
            SetTryScope( front, node->u2.st.parent_scope );
            break;
        case OPR_EXCEPT:
        case OPR_FINALLY:
            CGBigLabel( *front.back, FEBack( front, (CGSYM_HANDLE)node->u2.sym_handle ) );
            break;
        case OPR_END_FINALLY:
            EndFinally(front);
            break;
        case OPR_UNWIND:
            CallTryUnwind( front, node->u2.st.u.try_index );
            break;
        case OPR_EXCEPT_CODE:
            op1 = TryExceptionInfoAddr(front);
            op1 = CGUnary( *front.back, O_POINTS, op1, TY_POINTER );
            PushCGName( CGUnary( *front.back, O_POINTS, op1, TY_INTEGER ) );
            break;
        case OPR_EXCEPT_INFO:
            PushCGName( TryExceptionInfoAddr(front) );
            break;
        case OPR_ABNORMAL_TERMINATION:
            PushCGName( TryAbnormalTermination(front) );
            break;
#endif
        default:
           {
             char        msgbuf[MAX_MSG_LEN];

             sprintf( msgbuf, "opr=%d not handled\n", node->opr );
             DebugMsg( front, msgbuf );
           }
            break;
        }
    }
    if( index > 0 ) {
        CGDone( *front.back, PopCGName() );
    }
}

static TREEPTR GenOptimizedCode( FrontRun& front, TREEPTR tree )
{
    unroll_type unroll_count;

    unroll_count = 0;
    for( ; tree != NULL; tree = tree->left ) {
        if( tree->op.u2.src_loc.line != front.cvars.SrcLoc.line || tree->op.u2.src_loc.fno != front.cvars.SrcLoc.fno ) {
            if( front.cgen.Saved_CurFunc == SYM_NULL ) {
                FNAMEPTR    flist;

                flist = FileIndexToFName( front, tree->op.u2.src_loc.fno );
                if( flist->index_db == DBFILE_INVALID ) {
                    char *fullpath;

                    fullpath = FNameFullPath( front, flist );
                    flist->index_db = DBSrcFile( *front.back, fullpath );
                }
                DBSrcCue( *front.back, flist->index_db, tree->op.u2.src_loc.line, 1 );
            }
            front.cvars.SrcLoc = tree->op.u2.src_loc;
        }
        if( tree->op.u1.unroll_count != unroll_count ) {
            unroll_count = tree->op.u1.unroll_count;
            BEUnrollCount( *front.back, unroll_count );
        }
        /*
         * eliminate functions that are always inlined or not used
         */
        if( tree->right->op.opr == OPR_FUNCTION ) {     // if start of func
            TREEPTR right;

            right = tree->right;
            if ( ( right->op.u2.func.flags & FUNC_USED ) == 0 ) {
                if( front.cgen.InLineDepth == 0 ) {
                    while( tree->right->op.opr != OPR_FUNCEND ) {
                        tree = tree->left;
                    }
                    break;
                }
            }
        }
        EmitNodes( front, LinearizeTree( front, tree->right ) );
#ifdef __SEH__
        if( tree->right->op.opr == OPR_FUNCTION ) {     // if start of func
            if( front.cgen.FuncNodePtr->u2.func.flags & FUNC_USES_SEH ) {
                GenerateTryBlock( front, tree->left );
            }
        }
#endif
        if( tree->right->op.opr == OPR_FUNCEND ) {
            break;
        }
    }
    return( tree );
}

static void DoInLineFunction( FrontRun& front, TREEPTR tree )
/*******************************************
 * Push some state info and use InLineDepth for turning some stuff off
 */
{
    struct func_save    save;

    ++front.cgen.InLineDepth;
    save.func = front.cvars.CurFunc;
    save.func_handle = front.cvars.CurFuncHandle;
    save.funcnode = front.cgen.FuncNodePtr;
    save.cglabel_handles = front.cgen.CGLabelHandles;
    save.labelindex = front.cvars.LabelIndex;
    front.cvars.CurFuncHandle = SYM_NULL;
    front.cgen.FuncNodePtr = NULL;
    front.cgen.CGLabelHandles = NULL;
    front.cvars.LabelIndex = 0;
    GenOptimizedCode( front, tree );
    front.cvars.CurFunc = save.func;
    SymGet( front, front.cvars.CurFunc, save.func_handle );  // must be done before changing CurFuncHandle
    front.cvars.CurFuncHandle = save.func_handle;
    front.cgen.FuncNodePtr = save.funcnode;
    front.cgen.CGLabelHandles = save.cglabel_handles;
    front.cvars.LabelIndex = save.labelindex;
    --front.cgen.InLineDepth;
}

static TREEPTR FindFuncStmtTree( FrontRun& front, SYM_HANDLE sym_handle )
{
    SYMPTR      symptr;

    symptr = SymGetPtr( front, sym_handle );
    return( symptr->u.func.start_of_func );
}

void GenInLineFunc( FrontRun& front, SYM_HANDLE sym_handle )
{
    TREEPTR     tree;

    tree = FindFuncStmtTree(front, sym_handle);
    if( tree != NULL ) {
        DoInLineFunction( front, tree );
    }
}

bool IsInLineFunc( FrontRun& front, SYM_HANDLE sym_handle )
{
    bool        ret;
    TREEPTR     tree;
    TREEPTR     right;

    ret = false;
    if( front.cgen.InLineDepth < MAX_INLINE_DEPTH ) {
        tree = FindFuncStmtTree(front, sym_handle);
        if( tree != NULL ) {
           right = tree->right;
           if( (right->op.u2.func.flags & FUNC_INUSE) == 0 ) {
               ret = ( (right->op.u2.func.flags & FUNC_OK_TO_INLINE) != 0 );
           }
        }
    }
    return( ret );
}

static int ScanFunction( FrontRun& front, TREEPTR tree, int inline_depth )
/********************************************************
 * This function recursively checks if a function is really used and
 * needs to be emitted.
 * A function is not inlined and generated if it calls itself recursively,
 * or if the inline depth goes over the limit.
 * In that case, and for normal function calls, the function is marked
 * as FUNC_USED and can't be skipped by the GenOptimizedCode().
 */
{
    TREEPTR             right;
    struct func_info   *f;
    int                 marked;

    if( tree == NULL || tree->right == NULL )
        return( 0 );
    f = &tree->right->op.u2.func;

    if( inline_depth == -1 ) {
        /*
         * non-recursive call
         */
        inline_depth = 0;
    } else if( (f->flags & FUNC_OK_TO_INLINE) && (f->flags & FUNC_INUSE) == 0 &&
        inline_depth < MAX_INLINE_DEPTH ) {
        /*
         * simulate inlining when appropriate
         */
        inline_depth++;
    } else {
        /*
         * if already examined no need to do it again
         */
        if( f->flags & FUNC_USED )
            return 0;
        f->flags |= FUNC_USED | FUNC_MARKED;
        return( 1 );
    }

    f->flags |= FUNC_INUSE;
    marked = 0;
    for( ; tree != NULL; tree = tree->left ) {
        for( right = LinearizeTree( front, tree->right ); right != NULL; right = right->u.thread ) {
            if( right->op.opr == OPR_FUNCNAME ) {
                marked += ScanFunction( front, FindFuncStmtTree( front, right->op.u2.sym_handle ), inline_depth );
            }
        }
        if( tree->right->op.opr == OPR_FUNCEND ) {
            break;
        }
    }
    f->flags &= ~FUNC_INUSE;
    return( marked );
}

static void PruneFunctions( FrontRun& front )
/*********************************
 * This function scans the source file tree for functions. Any non-static
 * function or static function whose address is taken is scanned for
 * any functions called, and not inlined.
 * These functions are marked as used.
 */
{
    TREEPTR     tree;
    SYM_ENTRY   sym;
    int         marked;

    marked = 0;
    for( tree = front.cvars.FirstStmt; tree != NULL; tree = tree->left ) {
        if( tree->right->op.opr == OPR_FUNCTION ) {
            SymGet( front, &sym, tree->right->op.u2.func.sym_handle );
            if( sym.attribs.stg_class != SC_STATIC || ( sym.flags & SYM_ADDR_TAKEN ) ) {
                tree->right->op.u2.func.flags |= FUNC_MARKED | FUNC_USED;
                marked++;
            }
        }
    }
    while( marked ) {
        marked = 0;
        for( tree = front.cvars.FirstStmt; tree != NULL; tree = tree->left ) {
            if( tree->right->op.opr == OPR_FUNCTION ) {
                if (tree->right->op.u2.func.flags & FUNC_MARKED) {
                    tree->right->op.u2.func.flags &= ~FUNC_MARKED;
                    marked += ScanFunction( front, tree, -1 );
                }
            }
        }
    }
}

static void GenModuleCode( FrontRun& front )
{
    TREEPTR     tree;

    front.cgen.InLineDepth = 0;
//    InLineFuncStack = NULL;
    for( tree = front.cvars.FirstStmt; tree != NULL; tree = tree->left ) {
        tree = GenOptimizedCode( front, tree );
    }
}

static void NoCodeGenDLL( FrontRun& front )
{
    FEMessage( front, FEMSG_FATAL, const_cast<pointer>(static_cast<const void *>("Unable to load code generator DLL")) );
}

static void EmitSyms( FrontRun& front )
{
    SYM_HANDLE          sym_handle;
    SYM_ENTRY           sym;

    for( sym_handle = front.cvars.GlobalSym; sym_handle != SYM_NULL; sym_handle = sym.handle ) {
        SymGet( front, &sym, sym_handle );
        EmitSym( front, &sym, sym_handle );
        if( (front.cvars.GenSwitches & CGSW_GEN_DBG_LOCALS )
          && ( sym.sym_type->decl_type != TYP_FUNCTION )
          && ( (sym.flags & SYM_TEMP) == 0 )
          && ( sym.attribs.stg_class != SC_TYPEDEF ) ) {
            DBModSym( *front.back, (CGSYM_HANDLE)sym_handle, TY_DEFAULT );
        }
    }
}


static void FreeGblVars( FrontRun& front, SYM_HANDLE sym_handle )
{
    SYMPTR      sym;

    for( ; sym_handle != SYM_NULL; sym_handle = sym->handle ) {
        sym = SymGetPtr( front, sym_handle );
        if( sym->u1.backinfo != NULL ) {
//            BEFiniBack( sym->u1.backinfo );
            BEFreeBack( *front.back, sym->u1.backinfo );
        }
    }
}

static void RelExtVars( FrontRun& front, SYM_HANDLE sym_handle )
{
    SYMPTR      sym;

    for( ; sym_handle != SYM_NULL; sym_handle = sym->handle ) {
        sym = SymGetPtr( front, sym_handle );
        if( (sym->flags & SYM_FUNC_RETURN_VAR) == 0 ) {
            if( sym->attribs.stg_class == SC_EXTERN
              || sym->attribs.stg_class == SC_STATIC
              || sym->sym_type->decl_type == TYP_VOID ) {
                if( sym->u1.backinfo != NULL ) {
                    BEFreeBack( *front.back, sym->u1.backinfo );
                }
            }
        }
    }
}

static void FreeExtVars( FrontRun& front )
{
    SYM_LIST_HEADS   *sym_list_head;
    SYM_LIST_HEADS   *next_sym_list_head;

    for( sym_list_head = front.cvars.SymListHeads; sym_list_head != NULL; sym_list_head = next_sym_list_head ) {
        RelExtVars( front, sym_list_head->sym_head );
        next_sym_list_head = sym_list_head->next;
        CMemFree( front, sym_list_head );
    }
}

void DoCompile( FrontRun& front )
{
    jmp_buf         *old_env;
    jmp_buf         env;
    cg_init_info    cg_info;

    old_env = front.cvars.Environment;
    if( setjmp( env ) == 0 ) {
        front.cvars.Environment = JMPBUF_PTR( env );
        if( BELoad( NULL ) ) {
            if( ! front.cvars.CompFlags.zu_switch_used ) {
                front.cvars.TargetSwitches &= ~ CGSW_X86_FLOATING_SS;
            }
            if( front.cvars.CompFlags.rent ) {
                front.cvars.GenSwitches |= CGSW_GEN_POSITION_INDEPENDANT;
            }
            cg_info = BEInit( *front.back, front.cvars.GenSwitches, front.cvars.TargetSwitches, front.cvars.OptSize, static_cast<proc_revision>(front.cvars.ProcRevision) );
            if( cg_info.revision != 0 || cg_info.target != 0 ) {
#if _CPU == 386
                if( front.cvars.TargetSwitches & (CGSW_X86_P5_PROFILING | CGSW_X86_NEW_P5_PROFILING) ) {
                    front.cvars.FunctionProfileSegId = AddSegName( front, TS_SEG_TI, "DATA", SEGTYPE_INITFINI );
                }
#endif
                SetSegs(front);
                BEStart(*front.back);
                EmitSegLabels(front);
                if( front.cvars.GenSwitches & CGSW_GEN_DBG_TYPES )
                    EmitDBType(front);
                EmitSyms(front);
                EmitCS_Strings(front);
                front.cvars.SrcLoc.line = 0;
                front.cvars.SrcLoc.column = 0;
                front.cvars.SrcLoc.fno = 0;
                EmitDataQuads(front);
                FreeDataQuads(front);
#ifdef __SEH__
                front.cgen.TryRefno = NewRefno(front);
                BEDefType( *front.back, static_cast<cg_type>(front.cgen.TryRefno), 1, TRY_BLOCK_SIZE );
                front.cgen.TryTableBackHandles = NULL;
#endif
                PruneFunctions(front);
                GenModuleCode(front);
                FreeStrings(front);
                FiniSegLabels(front);
                if( front.cvars.ErrCount != 0 ) {
                    BEAbort(*front.back);
                }
                BEStop(*front.back);
                FiniSegBacks(front);
                FreeGblVars( front, front.cvars.GlobalSym );
                FreeGblVars( front, front.cvars.SpecialSyms );
                FreeExtVars(front);
#ifdef __SEH__
                FreeTryTableBackHandles(front);
#endif
                BEFini(*front.back);
                BEUnload();
                SegFini(front);
            }
        } else {
            NoCodeGenDLL(front);
        }
    }
    front.cvars.Environment = old_env;
}


cg_type CGenType( FrontRun& front, TYPEPTR typ )
{
    cg_type         dtype;
    type_modifiers  flags;
    align_type      align;

    SKIP_TYPEDEFS( typ );
    switch( typ->decl_type ) {

    case TYP_FCOMPLEX:
    case TYP_DCOMPLEX:
    case TYP_LDCOMPLEX:
    case TYP_STRUCT:
    case TYP_UNION:
        if( typ->object != NULL ) {
            /*
             * structure has a zero length array as last field
             */
            dtype = static_cast<cg_type>(NewRefno(front));
            align = GetTypeAlignment( front, typ );
            BEDefType( *front.back, dtype, align, TypeSize(front, typ) );
        } else if( typ->u.tag->refno == 0 ) {
            dtype = static_cast<cg_type>(NewRefno(front));
            align = GetTypeAlignment( front, typ );
            BEDefType( *front.back, dtype, align, TypeSize(front, typ) );
            typ->u.tag->refno = dtype;
        } else {
            dtype = static_cast<cg_type>(typ->u.tag->refno);
        }
        break;
    case TYP_ARRAY:
        if( typ->u.array->refno == 0 ) {
            dtype = static_cast<cg_type>(NewRefno(front));
            align = GetTypeAlignment( front, typ );
            BEDefType( *front.back, dtype, align, SizeOfArg( front, typ ) );
            typ->u.array->refno = dtype;
        }
        dtype = static_cast<cg_type>(typ->u.array->refno);
        break;
    case TYP_FIELD:
    case TYP_UFIELD:
        dtype = static_cast<cg_type>(CGDataType[typ->u.f.field_type]);
        break;
    case TYP_FUNCTION:
        dtype = CodePtrType( FLAG_NONE );
        break;
    case TYP_POINTER:
        flags = typ->u.p.decl_flags;
        dtype = PtrType( typ->object, flags );
        break;
    case TYP_ENUM:
        typ = typ->object;
        /* fall through */
    default:
        dtype = static_cast<cg_type>(CGDataType[typ->decl_type]);
    }
    return( dtype );
}


cg_type PtrType( TYPEPTR typ, type_modifiers flags )
{
    cg_type     dtype;

    SKIP_TYPEDEFS( typ );
    if( typ->decl_type == TYP_FUNCTION ) {
        dtype = CodePtrType( flags );
    } else {
        if( flags & FLAG_FAR ) {
            dtype = TY_LONG_POINTER;
        } else if( flags & FLAG_HUGE ) {
            dtype = TY_HUGE_POINTER;
        } else if( flags & FLAG_NEAR ) {
            dtype = TY_NEAR_POINTER;
        } else {
            dtype = TY_POINTER;
        }
    }
    return( dtype );
}


void EmitStrPtr( FrontRun& front, STR_HANDLE str_handle, cg_type pointer_type )
{
    str_handle->ref_count++;
    Emit1String( front, str_handle );
    DGBackPtr( *front.back, str_handle->back_handle, StringSegment( front, str_handle ), 0, pointer_type );
}
