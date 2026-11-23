/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2024 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Routines to emit type debugging information.
*
****************************************************************************/


#include "cvars.h"
#include "cgdefs.h"
#include "cgswitch.h"
#include "cgprotos.h"
#include "feprotos.h"
#include "cgen.h"
#include "frontrun.hpp"


static dbug_type DBTypeStruct( FrontRun& front, TYPEPTR typ );
static dbug_type DBTypeEnum( FrontRun& front, TYPEPTR typ );
static void InitDBType( FrontRun& front );

//void RevTypeList();

static void InitDBType( FrontRun& front )
{
    TYPEPTR     typ;

    front.cvars.ScopeStruct = DBScope( *front.back, "struct" );
    front.cvars.ScopeUnion = DBScope( *front.back, "union" );
    front.cvars.ScopeEnum = DBScope( *front.back, "enum" );
    typ = GetType( front, TYP_PLAIN_CHAR );
    if( typ->decl_type == TYP_UCHAR ) {
        typ->u1.debug_type = DBScalar( *front.back, "char", TY_UINT_1 );
    } else {
        typ->u1.debug_type = DBScalar( *front.back, "char", TY_INT_1 );
    }
    front.cvars.B_Int_1  = DBScalar( *front.back, "signed char", TY_INT_1 );
    front.cvars.B_UInt_1 = DBScalar( *front.back, "unsigned char", TY_UINT_1 );
    front.cvars.B_Short  = DBScalar( *front.back, "short", TY_INT_2 );
    front.cvars.B_UShort = DBScalar( *front.back, "unsigned short", TY_UINT_2 );
#if TARGET_INT == 4
    front.cvars.B_Int    = DBScalar( *front.back, "int", TY_INT_4 );
    front.cvars.B_UInt   = DBScalar( *front.back, "unsigned int", TY_UINT_4 );
#else
    front.cvars.B_Int    = DBScalar( *front.back, "int", TY_INT_2 );
    front.cvars.B_UInt   = DBScalar( *front.back, "unsigned int", TY_UINT_2 );
#endif
    front.cvars.B_Int32  = DBScalar( *front.back, "long", TY_INT_4 );
    front.cvars.B_UInt32 = DBScalar( *front.back, "unsigned long", TY_UINT_4 );
    front.cvars.B_Int64  = DBScalar( *front.back, "__int64", TY_INT_8 );
    front.cvars.B_UInt64 = DBScalar( *front.back, "unsigned __int64", TY_UINT_8 );
    front.cvars.B_Bool   = DBScalar( *front.back, "_Bool", TY_UINT_1 );
    front.cvars.DebugNameList = NULL;
}


static void EmitADBType( FrontRun& front, TYPEPTR typ )
{
    switch( typ->decl_type ) {
    case TYP_STRUCT:
    case TYP_UNION:
    case TYP_ENUM:
        if( !front.cvars.CompFlags.dump_types_with_names ) break;
        if( typ->u.tag->name[0] == '\0' ) break;
        goto dump_type;
    case TYP_TYPEDEF:
        if( !front.cvars.CompFlags.dump_types_with_names ) break;
        if( front.cvars.CompFlags.no_debug_type_names ) break;
    dump_type:
        DBType( front, typ );
        break;
    default:
        break;
    }
}

void EmitDBType( FrontRun& front )
{
//    RevTypeList();
    InitDBType(front);
    WalkTypeList( front, EmitADBType );
}

static dbug_type DBIntegralType( FrontRun& front, DATA_TYPE decl_type )
{
    dbug_type   ret_val;

    switch( decl_type ) {
    case TYP_CHAR:
        ret_val = front.cvars.B_Int_1;
        break;
    case TYP_UCHAR:
        ret_val = front.cvars.B_UInt_1;
        break;
    case TYP_SHORT:
        ret_val = front.cvars.B_Short;
        break;
    case TYP_USHORT:
        ret_val = front.cvars.B_UShort;
        break;
    case TYP_INT:
    default:
        ret_val = front.cvars.B_Int;
        break;
    case TYP_UINT:
        ret_val = front.cvars.B_UInt;
        break;
    case TYP_LONG:
        ret_val = front.cvars.B_Int32;
        break;
    case TYP_ULONG:
        ret_val = front.cvars.B_UInt32;
        break;
    case TYP_LONG64:
        ret_val = front.cvars.B_Int64;
        break;
    case TYP_ULONG64:
        ret_val = front.cvars.B_UInt64;
        break;
    case TYP_BOOL:
        ret_val = front.cvars.B_Bool;
        break;
    }
    return( ret_val );
}

static dbug_type DoBasedPtr( FrontRun& front, TYPEPTR typ, cg_type cgtype )
{
    dbug_type       ret_val;
    dbg_loc         dl;
    SYM_HANDLE      sym_handle;
    SYM_ENTRY       sym;
    bool            have_retval;

    have_retval = false;
    ret_val = 0;
    dl = DBLocInit();
    sym_handle = typ->u.p.based_sym;
    if( sym_handle == SYM_NULL ) {
        dl = DBLocConst( *front.back, dl, 0 );
        dl = DBLocOp( *front.back, dl, DB_OP_MK_FP, 0 );
    } else {
        if( sym_handle == front.cvars.Sym_CS ) {
            ret_val = DBPtr( *front.back, cgtype, DBType( front, typ->object ) );
            have_retval = true;
        } else if( sym_handle == front.cvars.Sym_SS ) {
            ret_val = DBPtr( *front.back, cgtype, DBType( front, typ->object ) );
            have_retval = true;
        } else {
            SymGet( front, &sym, sym_handle );
            if( sym.name[0] == '.' ) {  /* if segment label */
                ret_val = DBPtr( *front.back, cgtype, DBType( front, typ->object ) );
                have_retval = true;
            } else {
                dl = DBLocSym( *front.back, dl, (CGSYM_HANDLE)sym_handle );
                dl = DBLocOp( *front.back, dl, DB_OP_POINTS, TY_UINT_2 );
                dl = DBLocConst( *front.back, dl, 0 );
                dl = DBLocOp( *front.back, dl, DB_OP_MK_FP, 0 );
            }
        }
    }
    if (!have_retval) {
        ret_val = DBBasedPtr( *front.back, cgtype, DBType( front, typ->object ), dl );
    }
    DBLocFini( *front.back, dl );
    return( ret_val );
}

dbug_type DBType( FrontRun& front, TYPEPTR typ )
{
    dbug_type       ret_val;
    dbg_proc        pr;
    TYPEPTR         *parm_types;
    TYPEPTR         parm_type;
    target_size     size;
    SYM_ENTRY       sym;
    struct debug_fwd_types fwd_info, *fip;
    cg_type         cgtype;

    if( typ->u1.debug_type == DBG_FWD_TYPE ) {
        fip = front.cvars.DebugNameList;
        while( fip->typ != typ )  fip = fip->next;
        if( fip->debug_name == NULL ) {
            fip->debug_name = DBBegName( *front.back, "", fip->scope );
        }
        typ->u1.debug_type = DBForward( *front.back, fip->debug_name );
    }
    if( typ->u1.debug_type != DBG_NIL_TYPE )  return( typ->u1.debug_type );

    fwd_info.next = front.cvars.DebugNameList;
    fwd_info.typ = typ;
    fwd_info.debug_name = NULL;
    fwd_info.scope = DBG_NIL_TYPE;
    /*
     * default is INT
     */
    ret_val = front.cvars.B_Int;
    switch( typ->decl_type ) {
    case TYP_VOID:
        ret_val = DBScalar( *front.back, "void", TY_DEFAULT );
        break;
    case TYP_FLOAT:
        ret_val = DBScalar( *front.back, "float", TY_SINGLE );
        break;
    case TYP_DOUBLE:
        ret_val = DBScalar( *front.back, "double", TY_DOUBLE );
        break;
    case TYP_LONG_DOUBLE:
        ret_val = DBScalar( *front.back, "long double", TY_LONG_DOUBLE );
        break;
    case TYP_ARRAY:
        size = TypeSize( front, typ );
        if( size != 0 ) {
            --size;
        }
        ret_val = DBIntArrayCG( *front.back, CGenType(front, typ), size, DBType( front, typ->object ) );
        break;
    case TYP_POINTER:
        cgtype = PtrType( typ->object, typ->u.p.decl_flags );
        if( typ->u.p.decl_flags & FLAG_BASED ) {
            ret_val = DoBasedPtr( front, typ, cgtype );
        } else {
            ret_val = DBPtr( *front.back, cgtype, DBType( front, typ->object ) );
        }
        break;
    case TYP_STRUCT:
    case TYP_UNION:
        fwd_info.scope = (typ->decl_type == TYP_STRUCT)
                                    ? front.cvars.ScopeStruct : front.cvars.ScopeUnion;
        if( typ->u.tag->name[0] != '\0' ) {
            fwd_info.debug_name = DBBegName( *front.back, typ->u.tag->name,
                                        fwd_info.scope );
        }
        front.cvars.DebugNameList = &fwd_info;
        typ->u1.debug_type = DBG_FWD_TYPE;
        ret_val = DBTypeStruct( front, typ );
        if( fwd_info.debug_name != NULL ) {
            ret_val = DBEndName( *front.back, fwd_info.debug_name, ret_val );
        }
        front.cvars.DebugNameList = fwd_info.next;
        break;
    case TYP_FUNCTION:
        cgtype = TY_CODE_PTR;
        pr = DBBegProc( *front.back, cgtype, DBType( front, typ->object ) );
        if( typ->u.fn.parms != NULL ) {
            for( parm_types = typ->u.fn.parms; (parm_type = *parm_types) != NULL; ++parm_types ) {
                if( parm_type->decl_type == TYP_DOT_DOT_DOT ) {
                    break;
                }
                DBAddParm( *front.back, pr, DBType( front, parm_type ) );
            }
        }
        ret_val = DBEndProc( *front.back, pr );
        break;
    case TYP_TYPEDEF:
        if( typ->type_flags & TF2_DUMMY_TYPEDEF ) {
            ret_val = DBType( front, typ->object );
        } else {
            SymGet( front, &sym, typ->u.typedefn );
            if( !front.cvars.CompFlags.no_debug_type_names ) {
                fwd_info.debug_name = DBBegName( *front.back, sym.name, DBG_NIL_TYPE );
            }
            typ->u1.debug_type = DBG_FWD_TYPE;
            front.cvars.DebugNameList = &fwd_info;
            ret_val = DBType( front, typ->object );
            if( fwd_info.debug_name != NULL ) {
                ret_val = DBEndName( *front.back, fwd_info.debug_name, ret_val );
                if( front.cvars.GenSwitches & CGSW_GEN_DBG_CV ) {
                    DBTypeDef( *front.back, sym.name, ret_val ); //get codeview typedef out
                }
            }
            front.cvars.DebugNameList = fwd_info.next;
        }
        break;
    case TYP_ENUM:
        ret_val = DBTypeEnum( front, typ );
        break;
    default:
        ret_val = DBIntegralType( front, typ->decl_type );
        break;
    }
    typ->u1.debug_type = ret_val;
    return( ret_val );
}


static void DumpFieldList( FrontRun& front, dbg_struct st, target_size bias,
                          FIELDPTR pfield, TYPEPTR field_obj )
{
    TYPEPTR field_typ;

    for( ; pfield != NULL; pfield = pfield->next_field ) {
        field_typ = pfield->field_type;
        if( pfield->name[0] == '\0' ) {
            /*
             * anonymous struct/union -- suck up to this level
             */
            while( field_typ->decl_type == TYP_TYPEDEF ) {
                field_typ = field_typ->object;
            }
            DumpFieldList( front, st, bias + pfield->offset,
                        field_typ->u.tag->u.field_list, NULL );
        } else if(( field_typ->decl_type == TYP_FIELD ) ||
            ( field_typ->decl_type == TYP_UFIELD ) ) {
            field_typ->u1.debug_type = DBIntegralType( front, field_typ->u.f.field_type );
            DBAddBitField( *front.back, st, bias + pfield->offset,
                field_typ->u.f.field_start,
                field_typ->u.f.field_width, pfield->name,
                field_typ->u1.debug_type );
        } else if( field_obj != NULL
                && field_typ->decl_type == TYP_ARRAY
                && field_typ->u.array->dimension == 0 ) {
            DBAddField( *front.back, st, bias + pfield->offset,
                pfield->name, DBType( front, field_obj ));
        } else {
            DBAddField( *front.back, st, bias + pfield->offset,
                pfield->name, DBType( front, pfield->field_type ));
        }
    }
}

static dbug_type DBTypeStruct( FrontRun& front, TYPEPTR typ )
{
    dbug_type   ret_val;
    dbg_struct  st;
    TYPEPTR     obj;

    if( typ->object != NULL ) {
        /*
         * structure has a zero length array as last field
         */
        obj = typ->object;
    } else {
        obj = NULL;
    }
    st = DBBegNameStruct( *front.back, typ->u.tag->name, CGenType( front, typ ),
                                ( typ->decl_type == TYP_STRUCT ) );
    ret_val = DBStructForward( st );
    if( ret_val != DBG_NIL_TYPE ) {
         typ->u1.debug_type = ret_val;
    }
    DumpFieldList( front, st, 0, typ->u.tag->u.field_list, obj );
    ret_val = DBEndStruct( *front.back, st );
    return( ret_val );
}


static dbug_type DBTypeEnum( FrontRun& front, TYPEPTR typ )
{
    dbug_type   ret_val;
    dbg_enum    e;
    ENUMPTR     ep;

    e = DBBegEnum( *front.back, CGenType( front, typ->object ) );
    for( ep = typ->u.tag->u.enum_list; ep != NULL; ep = ep->thread ) {
        DBAddConst64( *front.back, e, ep->name, ep->value );
    }
    ret_val = DBEndEnum( *front.back, e );
    if( typ->u.tag->name[0] != '\0' ) {
        DBEndName( *front.back, DBBegName( *front.back, typ->u.tag->name, front.cvars.ScopeEnum ), ret_val );
    }
    return( ret_val );
}

dbug_type FEDbgType( FrontRun& front, CGSYM_HANDLE cgsym_handle )
{
    SYM_HANDLE     sym_handle = (SYM_HANDLE)cgsym_handle;

    return( DBType( front, SymGetPtr( front, sym_handle )->sym_type ) );
}

dbug_type FEDbgRetType( FrontRun& front, CGSYM_HANDLE cgsym_handle )
{
    SYM_HANDLE     sym_handle = (SYM_HANDLE)cgsym_handle;
    TYPEPTR        typ;

    typ = SymGetPtr( front, sym_handle )->sym_type;
    if( typ->decl_type == TYP_FUNCTION ) {
        return( DBType( front, typ->object ) );
    } else {
        return( DBG_NIL_TYPE );
    }
}
