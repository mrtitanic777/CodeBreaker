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
* Description:  Manage type information.
*
****************************************************************************/


#include "dwpriv.h"
#include "dwinfo.h"
#include "dwhandle.h"
#include "dwloc.h"
#include "dwtype.h"
#include "dwrefer.h"
#include "backrun.hpp"

void DW_EmitTypeRef( BackRun& back, dw_client cli, dw_handle hdl )
{
    DW_InfoHandleReference( back, cli, hdl );
}

dw_handle DWENTRY DWHandle( BackRun& back, dw_client cli, dw_struct_type kind )
{
    dw_handle                   new_hdl;

    /* unused parameters */ (void)kind;

    new_hdl = DW_NewHandle( back, cli );
    return( new_hdl );
}


void DWENTRY DWHandleSet( dw_client cli, dw_handle set_hdl )
{

    _Assert( cli->defset == 0 );
    cli->defset = set_hdl;
}

dw_handle DWENTRY DWFundamental( BackRun& back, dw_client cli, char const *name, dw_ftype fund_idx, unsigned size )
{
    dw_handle                   new_hdl;
    abbrev_code                 abbrev;

    _Validate( fund_idx >= DW_FT_MIN && fund_idx < DW_FT_MAX );
    new_hdl = DW_GetHandle( back, cli );
    abbrev = AB_BASE_TYPE;
    if( name != NULL )
        abbrev |= AB_NAME;
    DW_StartDIE( back, cli, abbrev );
    if( name != NULL ) {
        DW_InfoString( back, cli, name );
    }
    DW_Info8( back, cli, fund_idx );
    DW_Info8( back, cli, size );
    DW_EndDIE( cli );
    return( new_hdl );
}


dw_handle DWENTRY DWModifier( BackRun& back, dw_client cli, dw_handle hdl, uint modifiers )
{
    dw_handle                   new_hdl = 0;
    abbrev_code                 abbrev;

    _ValidateHandle( hdl );
    while( modifiers != 0 ) {
        if( modifiers & DW_MOD_CONSTANT ) {
            abbrev = AB_CONST_TYPE;
            modifiers &= ~DW_MOD_CONSTANT;
        } else if( modifiers & DW_MOD_VOLATILE ) {
            abbrev = AB_VOLATILE_TYPE;
            modifiers &= ~DW_MOD_VOLATILE;
        } else {
            abbrev = AB_ADDR_CLASS_TYPE;
        }
        new_hdl = DW_GetHandle( back, cli );
        DW_StartDIE( back, cli, abbrev );
        DW_InfoHandleWriteOffset( back, cli, hdl );
        if( abbrev == AB_ADDR_CLASS_TYPE ) {
            DW_Info8( back, cli, modifiers );
            modifiers = 0;
        }
        DW_EndDIE( cli );
        hdl = new_hdl;
    }
    return( new_hdl );
}


void DW_EmitAccessFlags( BackRun& back, dw_client cli, dw_flags access_flags )
{
    access_flags &= DW_FLAG_ACCESS_MASK;
    switch( access_flags ) {
    case DW_FLAG_PRIVATE:
        DW_Info8( back, cli, DW_ACCESS_private );
        break;
    case DW_FLAG_PROTECTED:
        DW_Info8( back, cli, DW_ACCESS_protected );
        break;
    default:
        DW_Info8( back, cli, DW_ACCESS_public );
    }
}


static void emitCommonTypeInfo( BackRun& back, dw_client cli, abbrev_code abbrev, const char *name, dw_flags access_flags )
{
    if( name != NULL )
        abbrev |= AB_NAME;
    DW_StartDIE( back, cli, abbrev );
    DW_EmitAccessFlags( back, cli, access_flags );
    if( name != NULL ) {
        /* AT_name */
        DW_InfoString( back, cli, name );
    }
}


dw_handle DWENTRY DWTypedef( BackRun& back, dw_client cli, dw_handle base_type, const char *name, dw_addr_offset start_scope, dw_flags flags )
{
    dw_handle                   new_hdl;
    abbrev_code                 abbrev;

    /* unused parameters */ (void)start_scope;

    _Validate( name != NULL );
    _ValidateHandle( base_type );
    new_hdl = DW_GetHandle( back, cli );
    abbrev = AB_TYPEDEF_TYPE | AB_START_REF;
    emitCommonTypeInfo( back, cli, abbrev , name, flags );
    DW_EmitTypeRef( back, cli, base_type );
    DW_EndDIE( cli );
    DW_EndRef( back, cli );
    return( new_hdl );
}

dw_handle DWENTRY DWRefPCH( BackRun& back, dw_client cli, uint_32 ref )
{
    dw_handle                   new_hdl;

    _Validate( ref != 0 );
    _Validate( cli->dbg_pch != NULL );
    new_hdl = DW_GetHandle( back, cli );
    DW_StartDIE( back, cli, AB_INDIRECT_TYPE );
    CLIReloc4( cli, DW_DEBUG_INFO, DW_W_EXT_REF, cli->dbg_pch, ref  );
    DW_EndDIE( cli );
    return( new_hdl );
}

void DW_WriteAddressClass( BackRun& back, dw_client cli, dw_flags flags )
{
    if( GET_FLAG_PTR_TYPE( flags ) ) {
        DW_Info8( back, cli, GET_FLAG_PTR_TYPE( flags ) );
    }
}


dw_handle DWENTRY DWPointer( BackRun& back, dw_client cli, dw_handle base_type, dw_flags flags )
{
    dw_handle                   new_hdl;
    abbrev_code                 abbrev;

    _ValidateHandle( base_type );
    new_hdl = DW_GetHandle( back, cli );
    abbrev = AB_POINTER_TYPE;
    if( flags & DW_FLAG_REFERENCE )
        abbrev = AB_REFERENCE_TYPE;
    flags &= ~DW_FLAG_REFERENCE;
    if( flags )
        abbrev |= AB_ADDRESS_CLASS;
    DW_StartDIE( back, cli, abbrev );
    DW_WriteAddressClass( back, cli, flags );
    DW_InfoHandleWriteOffset( back, cli, base_type );
    DW_EndDIE( cli );
    return( new_hdl );
}


dw_handle DWENTRY DWBasedPointer( BackRun& back, dw_client cli, dw_handle base_type, dw_loc_handle seg, dw_flags flags )
{
    dw_handle                   new_hdl;
    abbrev_code                 abbrev;

    _ValidateHandle( base_type );
    new_hdl = DW_GetHandle( back, cli );
    abbrev = AB_POINTER_TYPE;
    if( seg )
        abbrev |= AB_SEGMENT;
    if( flags )
        abbrev |= AB_ADDRESS_CLASS;
    DW_StartDIE( back, cli, abbrev );
    if( seg ) {
        DW_InfoEmitLocExpr( back, cli, sizeof( uint_8 ), seg );
    }
    DW_WriteAddressClass( back, cli, flags );
    DW_InfoHandleWriteOffset( back, cli, base_type );
    DW_EndDIE( cli );
    return( new_hdl );
}

dw_handle DWENTRY DWSimpleArray( BackRun& back, dw_client cli, dw_handle elt_type, int elt_count )
{
    dw_handle                   new_hdl;
    abbrev_code                 abbrev;

    new_hdl = DW_GetHandle( back, cli );
    _ValidateHandle( elt_type );
    abbrev = AB_SIMPLE_ARRAY_TYPE;
    DW_StartDIE( back, cli, abbrev );
    DW_InfoULEB128( back, cli, elt_count );
    DW_EmitTypeRef( back, cli, elt_type );
    DW_EndDIE( cli );
    return( new_hdl );
}

dw_handle DWENTRY DWBeginArray( BackRun& back, dw_client cli, dw_handle elt_type, uint_32 stride_size, const char *name, dw_addr_offset start_scope, dw_flags access_flags )
{
    dw_handle                   new_hdl;
    abbrev_code                 abbrev;

    /* unused parameters */ (void)start_scope;

    new_hdl = DW_GetHandle( back, cli );
    _ValidateHandle( elt_type );
    abbrev = AB_SIBLING;
    if( stride_size != 0 ) {
        abbrev |= AB_ARRAY_TYPE_WITH_STRIDE;
    } else {
        abbrev |= AB_ARRAY_TYPE;
    }
    emitCommonTypeInfo( back, cli, abbrev, name, access_flags );
    /* AT_stride_size */
    if( stride_size ) {
        DW_InfoULEB128( back, cli, stride_size );
    }
    DW_EmitTypeRef( back, cli, elt_type );
    DW_EndDIE( cli );
    DW_StartChildren( back, cli );
    return( new_hdl );
}


void DWENTRY DWArrayDimension( BackRun& back, dw_client cli, const dw_dim_info *dim_info )
{
    abbrev_code abbrev;

    abbrev = AB_ARRAY_BOUND;
    if( dim_info->index_type != 0 ) {
        abbrev |= AB_TYPE;
    }
    DW_StartDIE( back, cli, abbrev );
    if( dim_info->index_type != 0 ) {
        DW_EmitTypeRef( back, cli, dim_info->index_type );
    }
    /* AT_lower_bound */
    DW_InfoULEB128( back, cli, dim_info->lo_data );
    /* AT_upper_bound */
    DW_InfoULEB128( back, cli, dim_info->hi_data );
    DW_EndDIE( cli );
}

void DWENTRY DWArrayVarDim( BackRun& back, dw_client cli, const dw_vardim_info *dim_info )
{
    abbrev_code abbrev;

    abbrev = AB_ARRAY_VARDIM;
    if( dim_info->index_type != 0 ) {
        abbrev |= AB_TYPE;
    }
    DW_StartDIE( back, cli, abbrev );
    if( dim_info->index_type != 0 ) {
        DW_EmitTypeRef( back, cli, dim_info->index_type );
    }
    /* AT_lower_bound */
    DW_EmitTypeRef( back, cli, dim_info->lo_data );
    /* AT_count */
    DW_EmitTypeRef( back, cli, dim_info->count_data );
    DW_EndDIE( cli );
}

void DWENTRY DWEndArray( BackRun& back, dw_client cli )
{
    DW_EndChildren( back, cli );
}

dw_handle DWENTRY DWStruct( BackRun& back, dw_client cli, dw_struct_type kind )
{
    dw_handle                   new_hdl;

    new_hdl = DW_NewHandle( back, cli );
    DW_CreateExtra( back, cli, new_hdl )->structure.kind = kind;
    return( new_hdl );
}


void DWENTRY DWBeginStruct( BackRun& back, dw_client cli, dw_handle struct_hdl, dw_size_t size, const char *name, dw_addr_offset start_scope, dw_flags flags )
{
    abbrev_code                 abbrev;
    dw_struct_type              kind;

    /* unused parameters */ (void)start_scope;

    DW_SetHandleLocation( back, cli, struct_hdl );
    kind = static_cast<dw_struct_type>(DW_GetExtra( cli, struct_hdl )->structure.kind);
    abbrev = 0;
    switch( kind ){
    case DW_ST_CLASS:
        abbrev = AB_CLASS_TYPE;
        break;
    case DW_ST_STRUCT:
        abbrev = AB_STRUCT_TYPE;
        break;
    case DW_ST_UNION:
        abbrev = AB_UNION_TYPE;
        break;
    }
    DW_DestroyExtra( cli, struct_hdl );
    abbrev |= AB_SIBLING | AB_START_REF;
    emitCommonTypeInfo( back, cli, abbrev , name, flags );
    /* AT_byte_size */
    DW_InfoULEB128( back, cli, size );
    DW_EndDIE( cli );
    DW_StartChildren( back, cli );
}


void DWENTRY DWAddFriend( BackRun& back, dw_client cli, dw_handle friend_ )
{
    DW_StartDIE( back, cli, AB_FRIEND );
    DW_InfoHandleReference( back, cli, friend_ );
    DW_EndDIE( cli );
}


dw_handle DWENTRY DWAddInheritance( BackRun& back, dw_client cli, dw_handle ancestor_hdl, dw_loc_handle loc, dw_flags flags )
{
    dw_handle                   new_hdl;
    abbrev_code                 abbrev;

    new_hdl = DW_LabelNewHandle( back, cli );
    abbrev = AB_INHERITANCE;
    DW_StartDIE( back, cli, abbrev );
    DW_EmitAccessFlags( back, cli, flags );
    /* AT_location */
    if( loc ) {
        DW_InfoEmitLocExpr( back, cli, sizeof( uint_8 ), loc );
    } else {
        DW_InfoEmitLocExprNull( back, cli, sizeof( uint_8 ) );
    }
    /* AT_virtual */
    if( flags & DW_FLAG_VIRTUAL ) {
        DW_Info8( back, cli, DW_VIRTUALITY_virtual );
    } else {
        DW_Info8( back, cli, DW_VIRTUALITY_none );
    }
    /* AT_type */
    DW_InfoHandleReference( back, cli, ancestor_hdl );
    DW_EndDIE( cli );
    return( new_hdl );
}



dw_handle DWENTRY DWAddField( BackRun& back, dw_client cli, dw_handle field_hdl, dw_loc_handle loc, const char *name, dw_flags flags )
{
    dw_handle                   new_hdl;
    abbrev_code                 abbrev;

    _ValidateHandle( field_hdl );
    _Validate( (flags & DW_FLAG_STATIC) == 0 ); // use DWVariable dummy
    new_hdl = DW_LabelNewHandle( back, cli );
    abbrev = AB_FIELD;
    if( (flags & DW_FLAG_ACCESS_MASK) != DW_FLAG_PUBLIC ){ // not the default
        abbrev |= AB_OPACCESS;
    }
    if( flags & DW_FLAG_ARTIFICIAL ) {
        abbrev |= AB_ARTIFICIAL;
    }
    DW_StartDIE( back, cli, abbrev );
    if( abbrev & AB_OPACCESS ) {
        /* AT_accessibility */
        DW_EmitAccessFlags( back, cli, flags );
    }
    /* AT_artificial */
    if( flags & DW_FLAG_ARTIFICIAL ){
        DW_Info8( back, cli, 1 );
    }
    if( name == NULL ) {
        name  = "";
    }
    /* AT_name */
    DW_InfoString( back, cli, name );
    /* AT_data_member_location */
    if( loc ) {
        DW_InfoEmitLocExpr( back, cli, sizeof( uint_8 ), loc );
    } else {
        DW_InfoEmitLocExprNull( back, cli, sizeof( uint_8 ) );
    }
    /* AT_type */
    DW_EmitTypeRef( back, cli, field_hdl );
    DW_EndDIE( cli );
    return( new_hdl );
}


dw_handle DWENTRY DWAddBitField( BackRun& back, dw_client cli, dw_handle field_hdl, dw_loc_handle loc, dw_size_t byte_size, uint bit_offset, uint bit_size, const char *name, dw_flags flags )
{
    abbrev_code                 abbrev;
    dw_handle                   new_hdl;

    _ValidateHandle( field_hdl );
    new_hdl = DW_LabelNewHandle( back, cli );
    abbrev = AB_BITFIELD;
    if( byte_size )
        abbrev |= AB_BYTE_SIZE;
    DW_StartDIE( back, cli, abbrev );
    /* AT_accessibility */
    DW_EmitAccessFlags( back, cli, flags );
    if( byte_size ) {
        /* AT_byte_size */
        DW_InfoULEB128( back, cli, byte_size );
    }
    /* AT_bit_offset */
    DW_InfoULEB128( back, cli, bit_offset );
    /* AT_bit_size  */
    DW_InfoULEB128( back, cli, bit_size );
    /* AT_artificial */
    DW_Info8( back, cli, (flags & DW_FLAG_ARTIFICIAL) != 0 );
    if( name == NULL ) {
        name  = "";
    }
    /* AT_name */
    DW_InfoString( back, cli, name );
    /* AT_data_member_location */
    if( loc ) {
        DW_InfoEmitLocExpr( back, cli, sizeof( uint_8 ), loc );
    } else {
        DW_InfoEmitLocExprNull( back, cli, sizeof( uint_8 ) );
    }
    /* AT_type  */
    DW_EmitTypeRef( back, cli, field_hdl );
    DW_EndDIE( cli );
    return( new_hdl );
}


void DWENTRY DWEndStruct( BackRun& back, dw_client cli )
{
    DW_EndChildren( back, cli );
    DW_EndRef( back, cli );
}


dw_handle DWENTRY DWBeginEnumeration( BackRun& back, dw_client cli, dw_size_t byte_size, const char *name, dw_addr_offset start_scope, dw_flags flags )
{
    dw_handle                   new_hdl;
    abbrev_code                 abbrev;

    /* unused parameters */ (void)start_scope;

    new_hdl = DW_GetHandle( back, cli );
    abbrev = AB_ENUMERATION | AB_SIBLING;
    emitCommonTypeInfo( back, cli, abbrev, name, flags );
    /* AT_byte_size */
    DW_InfoULEB128( back, cli, byte_size );
    DW_EndDIE( cli );
    DW_StartChildren( back, cli );
    return( new_hdl );
}


void DWENTRY DWAddEnumerationConstant( BackRun& back, dw_client cli, dw_uconst value, const char *name )
{
    DW_StartDIE( back, cli, AB_ENUMERATOR );
    DW_InfoULEB128( back, cli, value );
    DW_InfoString( back, cli, name );
    DW_EndDIE( cli );
}


void DWENTRY DWEndEnumeration( BackRun& back, dw_client cli )
{
    DW_EndChildren( back, cli );
}


dw_handle DWENTRY DWBeginSubroutineType( BackRun& back, dw_client cli, dw_handle return_type, const char *name, dw_addr_offset start_scope, dw_flags flags )
{
    dw_handle                   new_hdl;
    abbrev_code                 abbrev;

    /* unused parameters */ (void)start_scope;

    _ValidateHandle( return_type );
    new_hdl = DW_GetHandle( back, cli );
    abbrev = AB_SUBROUTINE_TYPE | AB_SIBLING;
    if( flags & DW_FLAG_PTR_TYPE_MASK )
        abbrev |= AB_ADDRESS_CLASS;
    emitCommonTypeInfo( back, cli, abbrev, name, flags );
    if( abbrev & AB_ADDRESS_CLASS ) {
        DW_WriteAddressClass( back, cli, flags );
    }
    /* AT_prototyped */
    DW_Info8( back, cli, ( flags & DW_FLAG_PROTOTYPED ) != 0 );
    DW_EmitTypeRef( back, cli, return_type );
    DW_EndDIE( cli );
    DW_StartChildren( back, cli );
    return( new_hdl );
}


dw_handle DWENTRY DWAddParmToSubroutineType( BackRun& back, dw_client cli, dw_handle parm_type, dw_loc_handle loc, dw_loc_handle seg, const char *name )
{
    dw_handle                   new_hdl;
    abbrev_code                 abbrev;

    /* unused parameters */ (void)loc; (void)seg;

    //TODO: change interface

    _ValidateHandle( parm_type );
    new_hdl = DW_LabelNewHandle( back, cli );
    abbrev = AB_FORMAL_PARM_TYPE;
    if( name != NULL )
        abbrev |= AB_NAME;
    DW_StartDIE( back, cli, abbrev );
    /* AT_name */
    if( name != NULL ) {
        DW_InfoString( back, cli, name );
    }
    DW_EmitTypeRef( back, cli, parm_type );
    DW_EndDIE( cli );
    return( new_hdl );
}


dw_handle DWENTRY DWAddEllipsisToSubroutineType( BackRun& back, dw_client cli )
{
    dw_handle                   new_hdl;

    new_hdl = DW_LabelNewHandle( back, cli );
    DW_StartDIE( back, cli, AB_ELLIPSIS );
    DW_EndDIE( cli );
    return( new_hdl );
}


void DWENTRY DWEndSubroutineType( BackRun& back, dw_client cli )
{
    DW_EndChildren( back, cli );
}


dw_handle DWENTRY DWString( BackRun& back, dw_client cli, dw_loc_handle string_length, dw_size_t byte_size, const char *name, dw_addr_offset start_scope, dw_flags flags )
{
    dw_handle                   new_hdl;
    abbrev_code                 abbrev;

    /* unused parameters */ (void)start_scope;

    new_hdl = DW_GetHandle( back, cli );
    abbrev = string_length ? AB_STRING_WITH_LOC : AB_STRING;
    if( byte_size )
        abbrev |= AB_BYTE_SIZE;
    emitCommonTypeInfo( back, cli, abbrev, name, flags );
    /* AT_byte_size */
    if( byte_size ) {
        DW_InfoULEB128( back, cli, byte_size );
    }
    /* AT_string_length */
    if( string_length ) {
        DW_InfoEmitLocExpr( back, cli, sizeof( uint_8 ), string_length );
    }
    DW_EndDIE( cli );
    return( new_hdl );
}


dw_handle DWENTRY DWMemberPointer( BackRun& back, dw_client cli, dw_handle struct_type, dw_loc_handle loc, dw_handle base_type, const char *name, dw_flags flags )
{
    abbrev_code                 abbrev;
    dw_handle                   new_hdl;

    new_hdl = DW_LabelNewHandle( back, cli );
    abbrev = AB_MEMBER_POINTER;
    if( name != NULL )
        abbrev |= AB_NAME;
    if( flags & DW_FLAG_PTR_TYPE_MASK )
        abbrev |= AB_ADDRESS_CLASS;
    DW_StartDIE( back, cli, abbrev );
    /* AT_name */
    if( name != NULL ) {
        DW_InfoString( back, cli, name );
    }
    DW_WriteAddressClass( back, cli, flags );
    /* AT_type */
    DW_EmitTypeRef( back, cli, base_type );
    /* AT_containing_type */
    DW_InfoHandleReference( back, cli, struct_type );
    /* AT_use_location */
    DW_InfoEmitLocExpr( back, cli, sizeof( uint_16 ), loc );
    DW_EndDIE( cli );
    return( new_hdl );
}

dw_handle DWENTRY DWBeginNameSpace( BackRun& back, dw_client cli, const char *name )
{
    dw_handle                   new_hdl;
    abbrev_code                 abbrev;

    new_hdl = DW_LabelNewHandle( back, cli );
    abbrev = AB_NAMESPACE | AB_SIBLING | AB_START_REF;
    if( name != NULL )
        abbrev |= AB_NAME;
    DW_StartDIE( back, cli, abbrev );
    if( name != NULL ) {
        /* AT_name */
        DW_InfoString( back, cli, name );
    }
    DW_EndDIE( cli );
    DW_StartChildren( back, cli );
    return( new_hdl );
}

void DWENTRY DWEndNameSpace( BackRun& back, dw_client cli )
{
    DW_EndChildren( back, cli );
    DW_EndRef( back, cli );
}
