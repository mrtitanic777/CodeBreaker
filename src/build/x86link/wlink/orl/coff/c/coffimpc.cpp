/**************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2024      The Open Watcom Contributors. All Rights Reserved.
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
* Description:  COFF import library symbols definitions.
*
****************************************************************************/


#include <cstdlib>
#include "coffimpc.h"


// the names an import library's members are made of, each with its length (declared in coffimpc.h)
namespace {

template <std::size_t N>
constexpr name_len make_name( const char ( &text )[N] )
{
    return name_len{ N - 1, text };
}

}  // namespace

name_len str_coff_idata2 = make_name( ".idata$2" );
name_len str_coff_idata3 = make_name( ".idata$3" );
name_len str_coff_idata4 = make_name( ".idata$4" );
name_len str_coff_idata5 = make_name( ".idata$5" );
name_len str_coff_idata6 = make_name( ".idata$6" );
name_len str_coff_text = make_name( ".text" );
name_len str_coff_pdata = make_name( ".pdata" );
name_len str_coff_reldata = make_name( ".reldata" );
name_len str_coff_toc = make_name( ".toc" );
name_len str_coff_ppc_prefix = make_name( ".." );
name_len str_coff_imp_prefix = make_name( "__imp_" );
name_len str_coff_null_thunk_data = make_name( "_NULL_THUNK_DATA" );
name_len str_coff_import_descriptor = make_name( "__IMPORT_DESCRIPTOR_" );
name_len str_coff_null_import_descriptor = make_name( "__NULL_IMPORT_DESCRIPTOR" );
