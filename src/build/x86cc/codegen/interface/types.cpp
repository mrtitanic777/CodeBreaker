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
* Description:  Code generator type system.
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "cgmem.h"
#include "data.h"
#include "types.h"
#include "backrun.hpp"










namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct TypesFileState: types_state.h) */

}  // namespace



/*                              refno           length  attributes */
/*                              ======          ======  ========== */

static const type_def TUInt1      = {  TY_UINT_1,       1,      static_cast<type_attr>(0) };
static const type_def TInt1       = {  TY_INT_1,        1,      TYPE_SIGNED };
static const type_def TUInt2      = {  TY_UINT_2,       2,      static_cast<type_attr>(0) };
static const type_def TInt2       = {  TY_INT_2,        2,      TYPE_SIGNED };
static const type_def TUInt4      = {  TY_UINT_4,       4,      static_cast<type_attr>(0) };
static const type_def TInt4       = {  TY_INT_4,        4,      TYPE_SIGNED };
static const type_def TUInt8      = {  TY_UINT_8,       8,      static_cast<type_attr>(0) };
static const type_def TInt8       = {  TY_INT_8,        8,      TYPE_SIGNED };
static const type_def TSingle     = {  TY_SINGLE,       4,      static_cast<type_attr>(TYPE_SIGNED + TYPE_FLOAT) };
static const type_def TDouble     = {  TY_DOUBLE,       8,      static_cast<type_attr>(TYPE_SIGNED + TYPE_FLOAT) };
static const type_def TLongDouble = {  TY_LONG_DOUBLE,  10,     static_cast<type_attr>(TYPE_SIGNED + TYPE_FLOAT) };
static const type_def TBool       = {  TY_BOOLEAN,      0,      static_cast<type_attr>(0) };
static const type_def TNull       = {  TY_DEFAULT,      0,      static_cast<type_attr>(0) };
static const type_def TPascal     = {  TY_PROC_PARM,    4,      static_cast<type_attr>(0) };

const type_def  *TypeAddress( BackRun& back, cg_type tipe )
/*******************************************
    given a type refno, "tipe" which is known to the front end, return a
    pointer to the appropriate "type_def".
*/
{
    type_list   *list;

    switch( tipe ) {

    case TY_UINT_1:
        return( &TUInt1 );
    case TY_INT_1:
        return( &TInt1 );
    case TY_UINT_2:
        return( &TUInt2 );
    case TY_INT_2:
        return( &TInt2 );
    case TY_UINT_4:
        return( &TUInt4 );
    case TY_INT_4:
        return( &TInt4 );
    case TY_UINT_8:
        return( &TUInt8 );
    case TY_INT_8:
        return( &TInt8 );
    case TY_NEAR_CODE_PTR:
        return( &TNearCP );
    case TY_LONG_CODE_PTR:
        return( &TLongCP );
    case TY_NEAR_POINTER:
        return( &TNearP );
    case TY_LONG_POINTER:
        return( &TLongP );
    case TY_HUGE_POINTER:
        return( &THugeP );
    case TY_SINGLE:
        return( &TSingle );
    case TY_DOUBLE:
        return( &TDouble );
    case TY_LONG_DOUBLE:
        return( &TLongDouble );
    case TY_BOOLEAN:
        return( &TBool );
    case TY_DEFAULT:
        return( &TNull );
    case TY_PROC_PARM:
        return( &TPascal );
    case TY_INTEGER:
        return( back.types.PTInteger );
    case TY_UNSIGNED:
        return( back.types.PTUnsigned );
    case TY_POINTER:
        return( back.types.PTPointer );
    case TY_CODE_PTR:
        return( back.types.PTCodePointer );
    default:
        for( list = back.types_file.TypeList; list != NULL; list = list->link ) {
            if( list->format == TYPE_DEFINITION ) {
                if( list->tipe.u.type_def.refno == tipe ) {
                    return( &list->tipe.u.type_def );
                }
            } else {    /* TYPE_ALIAS */
                if( list->tipe.u.alias.refno == tipe ) {
                    return( list->tipe.u.alias.tptr );
                }
            }
        }
        return( NULL );
    }
}


type_length     TypeLength( BackRun& back, cg_type tipe )
/*****************************************
    Return the size of a given type "tipe".
*/
{
    const type_def  *t;

    t = TypeAddress( back, tipe );
    return( t->length );
}


const type_def  *TypeAlias( BackRun& back, cg_type define, cg_type existing )
/*************************************************************
    cause refno "define" to become an alias for existing type "existing"
*/
{
    const type_def  *t;
    type_list       *list;

    t = TypeAddress( back, existing );
    if( t->refno == TY_DEFAULT )
        return( t );
    list = static_cast<type_list *>(CGAlloc( back, sizeof( type_list ) ));
    list->link = back.types_file.TypeList;
    back.types_file.TypeList = list;
    list->format = TYPE_ALIAS;
    list->tipe.u.alias.refno = define;
    list->tipe.u.alias.tptr  = t;
    return( t );
}


const type_def  *TypeDef( BackRun& back, cg_type refno, type_length length, type_length align )
/*******************************************************************************
    Define a structure/array type which will have handle "refno".  The
    type will occupy "length" bytes of storage.
*/
{
    type_list   *list;

    /* unused parameters */ (void)align;

    list = static_cast<type_list *>(CGAlloc( back, sizeof( type_list ) ));
    list->link = back.types_file.TypeList;
    back.types_file.TypeList = list;
    list->format = TYPE_DEFINITION;
    list->tipe.u.type_def.refno  =  refno;
    list->tipe.u.type_def.length = length;
    list->tipe.u.type_def.attr   = static_cast<type_attr>(0);
    return( &list->tipe.u.type_def );
}


void    TypeFini( BackRun& back )
/***********************
    Finish up the typeing stuff
*/
{
    type_list   *type;
    type_list   *next;

    for( type = back.types_file.TypeList; type != NULL; type = next ) {
        next = type->link;
        CGFree( back, type );
    }
}

void    TypeInit( BackRun& back )
/**********************/
{
    back.types_file.TypeList = NULL;

    TargTypeInit(back);
    back.data.TypeProcParm = TypeAddress( back, TY_PROC_PARM );
    back.data.TypeInteger = TypeAddress( back, TY_INTEGER );
    back.data.TypeHugeInteger = TypeAddress( back, TY_HUGE_INTEGER );
    back.data.TypeLongInteger = TypeAddress( back, TY_LONG_INTEGER );
    back.data.TypeLongLongInteger = TypeAddress( back, TY_INT_8 );   // should be something else perhaps?
    back.data.TypeNearInteger = TypeAddress( back, TY_NEAR_INTEGER );
    back.data.TypeUnsigned = TypeAddress( back, TY_UNSIGNED );
    back.data.TypeBoolean = TypeAddress( back, TY_BOOLEAN );
    back.data.TypeNone = TypeAddress( back, TY_DEFAULT );
    back.data.TypePtr = TypeAddress( back, TY_POINTER );
}
