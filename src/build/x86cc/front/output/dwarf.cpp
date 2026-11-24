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
* Description:  DWARF browsing information support.
*
****************************************************************************/


#include "cvars.h"
#include "browsio.h"
#include "cg.h"
#include "cgdefs.h"
#include "cgswitch.h"
#include "cgprotos.h"
#include "i64.h"
#include "frontrun.hpp"


extern  void    InitDebugTypes( FrontRun& front );         /* from pchdr.c */



static dw_handle dwarfType( FrontRun& front, TYPEPTR, DC_CONTROL );
static void dwarfEmitVariables( FrontRun& front, SYM_HANDLE sym_handle );

static void type_update( TYPEPTR typ, int mask, dw_handle dh )
/************************************************************/
{
    typ->type_flags = static_cast<enum type_state>((typ->type_flags & ~TF2_DWARF) | mask);
    typ->u1.dwarf_type = dh;
}
namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct DwarfState: dwarf_state.h) */

}  // namespace



static void dwarfFile( FrontRun& front, unsigned filenum )
/***************************************/
{
    FNAMEPTR    flist;
    char       *fname;

    /*
     * if current file changed, call DWDeclFile
     */
    if( filenum != front.dwarf.dwarfFile_Current_File_Index ) {
        flist = FileIndexToFName(front, filenum);
        fname = FNameFullPath( front, flist );
        DWDeclFile( *front.back, front.dwarf.Client, fname );
        front.dwarf.dwarfFile_Current_File_Index = filenum;
    }
}

static void dwarfLocation( FrontRun& front, source_loc *src_loc )
/**********************************************/
{
    dwarfFile( front, src_loc->fno );
    DWDeclPos( front.dwarf.Client, src_loc->line, 0 );
}

static void dwarfReference( FrontRun& front, dw_handle dh, source_loc *src_loc )
/*************************************************************/
{
    dwarfFile( front, src_loc->fno );
    DWReference( *front.back, front.dwarf.Client, src_loc->line, 0, dh );
}

static void dwarfStructInfo( FrontRun& front, TAGPTR tag )
/***************************************/
{
    dw_handle   dh;
    dw_handle   fld_dh;
    TYPEPTR     typ;
    FIELDPTR    field;
    XREFPTR     xref;

    for( field = tag->u.field_list; field != NULL; field = field->next_field ) {
        xref = field->xref;
        typ = field->field_type;
        if( typ->decl_type == TYP_FIELD || typ->decl_type == TYP_UFIELD ) {
            fld_dh = dwarfType( front, GetType( front, typ->u.f.field_type ), DC_DEFAULT );
            if( xref != NULL ) {    //stupid struct { int x; int y[] ) = init thing
                /*
                 * Also watch for side effects with the DWDeclPos and a dwtype
                 */
                dwarfLocation( front, &xref->src_loc );
            }
            dh = DWAddBitField( *front.back, front.dwarf.Client,
                        fld_dh,
                        front.dwarf.dummyLoc,
                        TypeSize( front, typ ),
                        typ->u.f.field_start,
                        typ->u.f.field_width,
                        field->name,
                        DW_FLAG_PUBLIC );
        } else {
            fld_dh =  dwarfType( front, typ, DC_DEFAULT );
            xref = field->xref;     // re-get in case the struct was freed during recursion
            if( xref != NULL ) {    //stupid struct { int x; int y[] ) = init thing
                dwarfLocation( front, &xref->src_loc );
            }
            dh = DWAddField( *front.back, front.dwarf.Client,
                        fld_dh,
                        front.dwarf.dummyLoc,
                        field->name,
                        DW_FLAG_PUBLIC );
        }
        if( xref != NULL ) {
            for( ; (xref = xref->next_xref) != NULL; ) {
                dwarfReference( front, dh, &xref->src_loc );
            }
            FreeXrefs( front, field->xref );
            field->xref = NULL;
        }
    }
}

static dw_handle dwarfStructUnion( FrontRun& front, TYPEPTR typ, DC_CONTROL control )
/******************************************************************/
{
    dw_handle   dh;
    TAGPTR      tag;
    char        *name;
    char        defined;

    /* unused parameters */ (void)control;

    if( typ->type_flags & TF2_DWARF ) {
        dh = typ->u1.dwarf_type;
    } else {
        if( typ->decl_type == TYP_STRUCT ) {
            dh = DWStruct( *front.back, front.dwarf.Client, DW_ST_STRUCT );
        } else {
            dh = DWStruct( *front.back, front.dwarf.Client, DW_ST_UNION );
        }
        type_update( typ, TF2_DWARF_FWD, dh );
    }
    tag = typ->u.tag;
    if( typ->type_flags & TF2_DWARF_FWD ) {
        type_update( typ, TF2_DWARF_DEF, dh );
        if( tag->name[0] == '\0' ) {
            name = NULL;
        } else {
            name = tag->name;
        }
        defined = (tag->size != 0);
        DWBeginStruct( *front.back, front.dwarf.Client,
                       dh,
                       tag->size,
                       name,
                       0,
                       (defined ? DW_FLAG_NONE : DW_FLAG_DECLARATION ) );
        if( defined ) {
            dwarfStructInfo( front, tag );
        }
        DWEndStruct( *front.back, front.dwarf.Client );
    }
    return( dh );
}

static ENUMPTR ReverseEnums( ENUMPTR esym )    /* reverse order of enums */
{
    ENUMPTR     prev_enum;
    ENUMPTR     next_enum;

    prev_enum = NULL;
    while( esym != NULL ) {
        next_enum = esym->thread;
        esym->thread = prev_enum;
        prev_enum = esym;
        esym = next_enum;
    }
    return( prev_enum );
}

static dw_handle dwarfEnum( FrontRun& front, TYPEPTR typ )
/***************************************/
{
    dw_handle   dh;
    ENUMPTR     esym;
    ENUMPTR     enum_list;

    dh = DWBeginEnumeration( *front.back, front.dwarf.Client,
                             TypeSize( front, typ->object ),
                             typ->u.tag->name,
                             0,
                             static_cast<dw_flags>(0) );
    enum_list = ReverseEnums( typ->u.tag->u.enum_list );
    for( esym = enum_list; esym != NULL; esym = esym->thread ) {
        DWAddEnumerationConstant( *front.back, front.dwarf.Client, U64Low( esym->value ), esym->name );
    }
    ReverseEnums( enum_list );
    DWEndEnumeration( *front.back, front.dwarf.Client );
    type_update( typ, TF2_DWARF_DEF, dh );
    return( dh );
}

static dw_handle dwarfTypeArray( FrontRun& front, TYPEPTR typ )
/********************************************/
{
    dw_handle           dh;

    if( typ->type_flags & TF2_DWARF )
        return( typ->u1.dwarf_type );
    dh = DWSimpleArray( *front.back, front.dwarf.Client,
                dwarfType( front, typ->object, DC_DEFAULT ),
                typ->u.array->dimension );
    type_update( typ, TF2_DWARF_DEF, dh );
    return( dh );
}

static dw_handle dwarfTypeFunction( FrontRun& front, TYPEPTR typ, char *name )
/***********************************************************/
{
    dw_handle   dh;
    TYPEPTR     *parm_types;

    if( typ->type_flags & TF2_DWARF )
        return( typ->u1.dwarf_type );
    dh = dwarfType( front, typ->object, DC_RETURN );
    dh = DWBeginSubroutineType( *front.back, front.dwarf.Client,
                                dh,
                                name,
                                0,
                                DW_FLAG_DECLARATION | DW_FLAG_PROTOTYPED );
    type_update( typ, TF2_DWARF_DEF, dh );
    if( typ->u.fn.parms != NULL ) {
        for( parm_types = typ->u.fn.parms; (typ = *parm_types) != NULL; ++parm_types ) {
            if( typ->decl_type == TYP_DOT_DOT_DOT ) {
                DWAddEllipsisToSubroutineType( *front.back, front.dwarf.Client );
            } else {
                DWAddParmToSubroutineType( *front.back, front.dwarf.Client,
                                       dwarfType( front, typ, DC_DEFAULT ),
                                       front.dwarf.dummyLoc,
                                       front.dwarf.dummyLoc,
                                       NULL );
            }
        }
    }
    DWEndSubroutineType( *front.back, front.dwarf.Client );
    return( dh );
}


static dw_handle dwarfType( FrontRun& front, TYPEPTR typ, DC_CONTROL control )
/***********************************************************/
{
    dw_handle   dh = 0;
    SYMPTR      sym;

    /* unused parameters */ (void)control;

    if( typ->type_flags & TF2_DWARF )
        return( typ->u1.dwarf_type );

    switch( typ->decl_type ) {
    case TYP_BOOL:
        dh = DWFundamental( *front.back, front.dwarf.Client, "_Bool", DW_FT_UNSIGNED_CHAR, TypeSize( front, typ ) );
        type_update( typ, TF2_DWARF_DEF, dh );
        break;
    case TYP_CHAR:
        dh = DWFundamental( *front.back, front.dwarf.Client, "char", DW_FT_SIGNED_CHAR, TypeSize( front, typ ) );
        type_update( typ, TF2_DWARF_DEF, dh );
        break;
    case TYP_UCHAR:
        dh = DWFundamental( *front.back, front.dwarf.Client, "unsigned char", DW_FT_UNSIGNED_CHAR, TypeSize( front, typ ) );
        type_update( typ, TF2_DWARF_DEF, dh );
        break;
    case TYP_WCHAR:
        dh = DWFundamental( *front.back, front.dwarf.Client, "wchar_t", DW_FT_UNSIGNED_CHAR, TypeSize( front, typ ) );
        type_update( typ, TF2_DWARF_DEF, dh );
        break;
    case TYP_SHORT:
        dh = DWFundamental( *front.back, front.dwarf.Client, "signed short", DW_FT_SIGNED, TypeSize( front, typ ) );
        type_update( typ, TF2_DWARF_DEF, dh );
        break;
    case TYP_USHORT:
        dh = DWFundamental( *front.back, front.dwarf.Client, "unsigned short", DW_FT_UNSIGNED, TypeSize( front, typ ) );
        type_update( typ, TF2_DWARF_DEF, dh );
        break;
    case TYP_INT:
        dh = DWFundamental( *front.back, front.dwarf.Client, "signed int", DW_FT_SIGNED, TypeSize( front, typ ) );
        type_update( typ, TF2_DWARF_DEF, dh );
        break;
    case TYP_UINT:
        dh = DWFundamental( *front.back, front.dwarf.Client, "unsigned int", DW_FT_UNSIGNED, TypeSize( front, typ ) );
        type_update( typ, TF2_DWARF_DEF, dh );
        break;
    case TYP_LONG:
        dh = DWFundamental( *front.back, front.dwarf.Client, "signed long", DW_FT_SIGNED, TypeSize( front, typ ) );
        type_update( typ, TF2_DWARF_DEF, dh );
        break;
    case TYP_ULONG:
        dh = DWFundamental( *front.back, front.dwarf.Client, "unsigned long", DW_FT_UNSIGNED, TypeSize( front, typ ) );
        type_update( typ, TF2_DWARF_DEF, dh );
        break;
    case TYP_LONG64:
        dh = DWFundamental( *front.back, front.dwarf.Client, "__int64", DW_FT_UNSIGNED, TypeSize( front, typ ) );
        type_update( typ, TF2_DWARF_DEF, dh );
        break;
    case TYP_ULONG64:
        dh = DWFundamental( *front.back, front.dwarf.Client, "unsigned __int64", DW_FT_UNSIGNED, TypeSize( front, typ ) );
        type_update( typ, TF2_DWARF_DEF, dh );
        break;
    case TYP_FLOAT:
        dh = DWFundamental( *front.back, front.dwarf.Client, "float", DW_FT_FLOAT, TypeSize( front, typ ) );
        type_update( typ, TF2_DWARF_DEF, dh );
        break;
    case TYP_DOUBLE:
        dh = DWFundamental( *front.back, front.dwarf.Client, "double", DW_FT_FLOAT, TypeSize( front, typ ) );
        type_update( typ, TF2_DWARF_DEF, dh );
        break;
    case TYP_LONG_DOUBLE:
        dh = DWFundamental( *front.back, front.dwarf.Client, "long double", DW_FT_FLOAT, TypeSize( front, typ ) );
        type_update( typ, TF2_DWARF_DEF, dh );
        break;
    case TYP_ENUM:
        dh = dwarfEnum( front, typ );
        break;
    case TYP_POINTER:
        dh = DWPointer( *front.back, front.dwarf.Client, dwarfType( front, typ->object, DC_DEFAULT ), static_cast<dw_flags>(0) );
        type_update( typ, TF2_DWARF_DEF, dh );
        break;
    case TYP_TYPEDEF:
        dh = dwarfType( front, typ->object, DC_DEFAULT );
        sym = SymGetPtr( front, typ->u.typedefn );
        dwarfLocation( front, &sym->src_loc );
        dh = DWTypedef( *front.back, front.dwarf.Client,
                        dh,
                        sym->name,
                        0,
                        static_cast<dw_flags>(0) );
        type_update( typ, TF2_DWARF_DEF, dh );
        break;
    case TYP_STRUCT:
    case TYP_UNION:
        dh = dwarfStructUnion( front, typ, DC_DEFAULT );
        break;
    case TYP_FUNCTION:
        dh = dwarfTypeFunction( front, typ, NULL );
        break;
    case TYP_ARRAY:
        dh = dwarfTypeArray( front, typ );
        break;
    case TYP_VOID:
        dh = DWFundamental( *front.back, front.dwarf.Client, "void", DW_FT_UNSIGNED, 0 );
        type_update( typ, TF2_DWARF_DEF, dh );
        break;
    default:
        break;
    }
    return( dh );
}

static void dwarfFunctionDefine( FrontRun& front, SYMPTR func_sym )
/************************************************/
{
    TYPEPTR     typ;
    dw_handle   return_dh;
    dw_handle   func_dh;
    dw_handle   dh;
    uint        call_type;
    dw_flags    flags;
    SYMPTR      sym;
    SYM_HANDLE  sym_handle;

    call_type = 0;
    typ = func_sym->sym_type;
    if( func_sym->mods & FLAG_NEAR ) {
        call_type = DW_SB_NEAR_CALL;
    } else if( func_sym->mods &  FLAG_FAR ) {
        call_type = DW_SB_FAR_CALL;
    } else if( func_sym->mods & FLAG_FAR16 ) {
        call_type = DW_SB_FAR16_CALL;
    }
    flags = DW_FLAG_PROTOTYPED;
    if( func_sym->attribs.stg_class == SC_STATIC ) {
        flags |= DW_FLAG_SUB_STATIC;
    }
    return_dh = dwarfType( front, typ->object, DC_RETURN );
    func_dh = func_sym->dwarf_handle;
    if( func_dh != 0 ) {    // was forward ref'd
        DWHandleSet( front.dwarf.Client, func_dh );
    }
    dwarfLocation( front, &func_sym->src_loc );
    func_dh = DWBeginSubroutine( *front.back, front.dwarf.Client,
                   call_type,
                   return_dh,
                   front.dwarf.dummyLoc,
                   front.dwarf.dummyLoc,
                   front.dwarf.dummyLoc,
                   0,
                   front.dwarf.dummyLoc,
                   func_sym->name,
                   0,
                   flags );
    func_sym->dwarf_handle = func_dh;
    for( sym_handle = func_sym->u.func.parms; sym_handle != SYM_NULL; sym_handle = sym->handle ) {
        sym = SymGetPtr( front, sym_handle );
        dh = DWFormalParameter( *front.back, front.dwarf.Client,
                        dwarfType( front, sym->sym_type, DC_DEFAULT ),
                        NULL,
                        NULL,
                        sym->name,
                        DW_DEFAULT_NONE );
        sym->dwarf_handle = dh;
    }
    dwarfEmitVariables( front, func_sym->u.func.locals );
}

static dw_handle dwarfFunctionDecl( FrontRun& front, SYMPTR func_sym )
/***************************************************/
{
    TYPEPTR     typ;
    dw_handle   return_dh;
    dw_handle   func_dh;
    uint        call_type;
    dw_flags    flags;

    call_type = 0;
    typ = func_sym->sym_type;
    if( func_sym->mods & FLAG_NEAR ) {
        call_type = DW_SB_NEAR_CALL;
    } else if( func_sym->mods &  FLAG_FAR ) {
        call_type = DW_SB_FAR_CALL;
    } else if( func_sym->mods & FLAG_FAR16 ) {
        call_type = DW_SB_FAR16_CALL;
    }
    flags = DW_FLAG_PROTOTYPED | DW_FLAG_DECLARATION;
    if( func_sym->attribs.stg_class == SC_STATIC ) {
        flags |= DW_FLAG_SUB_STATIC;
    }
    return_dh = dwarfType( front, typ->object, DC_RETURN );
    dwarfLocation( front, &func_sym->src_loc );
    func_dh = DWBeginSubroutine( *front.back, front.dwarf.Client,
                   call_type,
                   return_dh,
                   front.dwarf.dummyLoc,
                   front.dwarf.dummyLoc,
                   front.dwarf.dummyLoc,
                   0,
                   front.dwarf.dummyLoc,
                   func_sym->name,
                   0,
                   flags );
   DWEndSubroutine( *front.back, front.dwarf.Client );
   return( func_dh );
}

static dw_handle dwarfVariable( FrontRun& front, SYMPTR sym )
/******************************************/
{
    dw_handle   dh;
    dw_flags    flags;

    flags = DW_FLAG_NONE;
    if( sym->attribs.stg_class == SC_NONE ) {
        flags = DW_FLAG_GLOBAL;
    }
    dh = dwarfType( front, sym->sym_type, DC_DEFAULT );
    dwarfLocation( front, &sym->src_loc );
    dh = DWVariable( *front.back, front.dwarf.Client,
                dh,
                front.dwarf.dummyLoc,
                0,
                front.dwarf.dummyLoc,
                sym->name,
                0,
                flags );
    return( dh );
}


static void dwarfEmitVariables( FrontRun& front, SYM_HANDLE sym_handle )
/*****************************************************/
{
    SYMPTR      sym;
    TYPEPTR     typ;
    dw_handle   dh;

    for( ; sym_handle != SYM_NULL; sym_handle = sym->handle ) {
        sym = SymGetPtr( front, sym_handle );
        if( (sym->flags & SYM_TEMP) == 0 ) {
            typ = sym->sym_type;
            dh = 0;
            if( sym->flags & SYM_FUNCTION ) {
                if( (sym->flags & SYM_DEFINED) == 0 ) {
                    dh = dwarfFunctionDecl( front, sym );
                }
//                printf( "func: %s", sym->name );
            } else if( sym->attribs.stg_class == SC_TYPEDEF ) {
                dh = dwarfType( front, typ, DC_DEFAULT );
//                printf( "typedef: %s", sym->name );
            } else {
                dh = dwarfVariable( front, sym );
//                printf( "var:: %s", sym->name );
            }
            sym->dwarf_handle = dh;
//            printf( " defined on line %u\n", sym->d.defn_line );
        }
    }
}

static void dwarfDumpNode( FrontRun& front, TREEPTR node )
/***************************************/
{
    SYMPTR      sym;

    switch( node->op.opr ) {
    case OPR_FUNCTION:          // start of function
        sym = SymGetPtr( front, node->op.u2.func.sym_handle );
        dwarfFunctionDefine( front, sym );
        break;
    case OPR_FUNCEND:           // end of function
        DWEndSubroutine( *front.back, front.dwarf.Client );
        break;
    case OPR_NEWBLOCK:          // start of new block { vars; }
        DWBeginLexicalBlock( *front.back, front.dwarf.Client, NULL, NULL );
        dwarfEmitVariables( front, node->op.u2.sym_handle );
        break;
    case OPR_ENDBLOCK:          // end of new block { vars; }
        DWEndLexicalBlock( *front.back, front.dwarf.Client );
        break;
    case OPR_PUSHSYM:           // push sym_handle
    case OPR_PUSHADDR:          // push address of sym_handle
    case OPR_FUNCNAME:          // function name
        sym = SymGetPtr( front, node->op.u2.sym_handle );
        if( (sym->flags & SYM_TEMP) == 0 ) {
            dw_handle   dh;

            dh = sym->dwarf_handle;
            if( dh == 0 ) {     // forward ref
                dh = DWHandle( *front.back, front.dwarf.Client, DW_ST_NONE );
                sym->dwarf_handle = dh;
            }
            dwarfReference( front, dh, &front.dwarf.CurLoc );
        }
        break;
    default:
        break;
    }
}


static void dwarfEmitFunctions( FrontRun& front )
/************************************/
{
    TREEPTR     tree;

    for( tree = front.cvars.FirstStmt; tree != NULL; tree = tree->left ) {
        front.dwarf.CurLoc = tree->op.u2.src_loc;
        WalkExprTree( front, tree->right, dwarfDumpNode, NoOp, NoOp, dwarfDumpNode );
    }
}

static void SetDwarfType( FrontRun& front, TYPEPTR typ )
{
    typ->type_flags &= ~TF2_DWARF;
    typ->u1.dwarf_type = 0;
}

static void SetFuncDwarfType( FrontRun& front, TYPEPTR typ, int index )
{
    /* unused parameters */ (void)index;

    typ->type_flags &= ~TF2_DWARF;
    typ->u1.dwarf_type = 0;
}

static void EmitAType( FrontRun& front, TYPEPTR typ )
{
    dwarfType( front, typ, DC_DEFAULT );
}

static void dwarfInitTypes( FrontRun& front )
/********************************/
{
    WalkTypeList( front, SetDwarfType );
    WalkFuncTypeList( front, SetFuncDwarfType );
}

static void dwarfEmit( FrontRun& front )
/***************************/
{
    DATA_TYPE   i;

    dwarfInitTypes(front);
    for( i = static_cast<DATA_TYPE>(0); i < DATA_TYPE_BOOL_TO_LONG_DOUBLE; i++ ) {
        dwarfType( front, GetType( front, i ), DC_DEFAULT );
    }
    WalkTypeList( front, EmitAType );
    dwarfEmitVariables( front, front.cvars.GlobalSym );
    dwarfEmitFunctions(front);
    InitDebugTypes(front);
}

extern void DwarfBrowseEmit( FrontRun& front )
/*********************************/
{
    front.dwarf.Client = DwarfInit(front);
    front.dwarf.dummyLoc = DWLocFini( *front.back, front.dwarf.Client, DWLocInit( *front.back, front.dwarf.Client ) );
    front.dwarf.CurLoc.fno = 0;
    front.dwarf.CurLoc.line = 0;
    front.dwarf.CurLoc.column = 0;
    dwarfEmit(front);
    DWLocTrash( *front.back, front.dwarf.Client, front.dwarf.dummyLoc );
    DwarfFini( front, front.dwarf.Client );
}
