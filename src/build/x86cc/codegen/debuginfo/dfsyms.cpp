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
* Description:  Emit DWARF symbol information.
*
****************************************************************************/


#include "_cgstd.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "coderep.h"
#include "cgmem.h"
#include "zoiks.h"
#include "cgauxcc.h"
#include "cgauxinf.h"
#include "dw.h"
#include "dwarf.h"
#include "dfdbg.h"
#include "data.h"
#include "types.h"
#include "objout.h"
#include "dbsyms.h"
#include "dfsyms.h"
#include "dfsupp.h"
#include "dftypes.h"
#include "targetdb.h"
#include "namelist.h"
#include "felang.h"
#include "cgprotos.h"
#include "feprotos.h"
#include "backrun.hpp"



#define MAX_LANG    (sizeof( LangNames ) / sizeof( LangNames[0] ))








namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct DfsymsFileState: dfsyms_state.h) */

}  // namespace



static const struct dwarf_lang_map LangNames[] = {
    {DW_LANG_C89,           FE_LANG_C        },
    {DW_LANG_C_plus_plus,   FE_LANG_CPP      },
    {DW_LANG_Fortran77,     FE_LANG_FORTRAN  },
};

static void CLIWrite( BackRun& back, dw_sectnum sect, const void *block, size_t size )
/*********************************************************************/
{
    sect_info           *curr;
//    long_offset         off;

    curr = &back.dfsyms_file.DwarfSegs[sect];
    { segment_id old_segid = ChangeOP( back, curr->segid );
//        off = AskBigLocation();
        DataBytes( back, size, block );
    SetOP( back, old_segid ); }
}

static dw_out_offset CLITell( BackRun& back, dw_sectnum sect )
/*********************************************/
{
    sect_info           *curr;
    long_offset         off;

    curr = &back.dfsyms_file.DwarfSegs[sect];
    { segment_id old_segid = ChangeOP( back, curr->segid );
        off = AskBigLocation(back);
    SetOP( back, old_segid ); }
    return( off );
}

static void CLISeek( BackRun& back, dw_sectnum sect, dw_out_offset offset, int type )
/********************************************************************/
{
    sect_info           *curr;
    dw_out_offset       new_offset;

    curr = &back.dfsyms_file.DwarfSegs[sect];
    { segment_id old_segid = ChangeOP( back, curr->segid );
        new_offset = offset;
        switch( type ) {
        case DW_SEEK_CUR:
            new_offset = AskBigLocation(back) + offset;
            break;
        case DW_SEEK_SET:
            break;
        case DW_SEEK_END:
            new_offset = AskBigMaxSize(back) + offset;
            break;
        }
        SetBigLocation( back, new_offset );
    SetOP( back, old_segid ); }
}

static void DoReloc( BackRun& back, dw_sym_handle sym, dw_addr_offset disp )
/***********************************************************/
{
    const type_def      *ptr_type;

    ptr_type = TypeAddress( back, TY_NEAR_POINTER );
    FEPtr( back, (cg_sym_handle)sym, ptr_type, disp );
}


static void DoSegReloc( BackRun& back, dw_sym_handle sym )
/*****************************************/
{
    FEPtrBase( back, (cg_sym_handle)sym );
}

static void DoLblReloc( BackRun& back, back_handle bck, int disp )
/*************************************************/
{
    const type_def  *ptr_type;
    segment_id      segid;

    segid = AskSegID( back, bck, CG_BACK );
    ptr_type = TypeAddress( back, TY_NEAR_POINTER );
    BackPtr( back, bck, segid, disp, ptr_type );
}

static void DoSegLblReloc( BackRun& back, back_handle bck )
/******************************************/
{
    segment_id      segid;

    segid = AskSegID( back, bck, CG_BACK );
    BackPtrBase( back, bck, segid );
}

static void DoSectOffset( BackRun& back, dw_sectnum sect )
/*****************************************/
{
    back_handle bck;
    long        pos;
    segment_id  segid;

    pos = CLITell( back, sect );
    bck = back.dfsyms_file.DwarfSegs[sect].bck;
    segid = back.dfsyms_file.DwarfSegs[sect].segid;
    BackPtrBigOffset( back, bck, segid, pos );

}

static void CLIReloc( BackRun& back, dw_sectnum sect, dw_reloc_type reloc_type, ... )
/********************************************************************/
{
    static uint_32 const    zero  = 0;
    sect_info               *curr;
    dw_sym_handle           sym;
    dw_sym_handle           bck;
    loc_range               *low;
    loc_range               *high;
    dw_sectnum              sect_no;
    va_list                 args;
//    long_offset             off;

    va_start( args, reloc_type );
    curr = &back.dfsyms_file.DwarfSegs[sect];
    { segment_id old_segid = ChangeOP( back, curr->segid );
//        off = AskBigLocation();
        switch( reloc_type ) {
        case DW_W_LOW_PC:
            if( back.dfsyms_file.Pc_Low != NULL ) {
                DoLblReloc( back, back.dfsyms_file.Pc_Low, 0 );
            } else {
                Zoiks( back, ZOIKS_107 ); /* No Low PC */
            }
            break;
        case DW_W_HIGH_PC:
            if( back.dfsyms_file.Pc_High != NULL ) {
                DoLblReloc( back, back.dfsyms_file.Pc_High, 0 );
            } else {
                Zoiks( back, ZOIKS_107 ); /* No High PC */
            }
            break;
        case DW_W_STATIC:
            sym = va_arg( args, dw_sym_handle );
            DoReloc( back, sym, 0  );
            break;
        case DW_W_LABEL:
            bck = va_arg( args, dw_sym_handle );
            DoLblReloc( back, (back_handle)bck, 0 );
            break;
        case DW_W_SEGMENT:
            sym = va_arg( args, dw_sym_handle );
            DoSegReloc( back, sym );
            break;
        case DW_W_LABEL_SEG:
            bck = va_arg( args, dw_sym_handle );
            DoSegLblReloc( back, (back_handle)bck );
            break;
        case DW_W_LOC_RANGE:
            low =  va_arg( args, loc_range* );
            high = va_arg( args, loc_range* );
            DoLblReloc( back, low->bck, low->disp );
            DoLblReloc( back, high->bck, high->disp );
            break;
        case DW_W_DEFAULT_FUNCTION:
            break;
        case DW_W_ARANGE_ADDR:
            DoLblReloc( back, back.dfsyms_file.ARange, 0 );
    #if (_TARGET & _TARG_8086)
            DoSegLblReloc( back, back.dfsyms_file.ARange );
    #elif (_TARGET & _TARG_80386)
            if( IsntTargetModel( back, CGSW_X86_FLAT_MODEL ) ) {
                DoSegLblReloc( back, back.dfsyms_file.ARange );
            }
    #endif
            break;
        case DW_W_UNIT_SIZE:
            back.dfsyms_file.UnitSize->segid = curr->segid;
            back.dfsyms_file.UnitSize->offset =  AskBigLocation(back);
            DataBytes( back, sizeof( zero ), &zero );
            break;
        case DW_W_SECTION_POS:
            sect_no = static_cast<dw_sectnum>( va_arg( args, int ) );   /* (an enum passed through ... arrives as int) */
            DoSectOffset( back, sect_no );
            break;
        case DW_W_EXT_REF:
          {
            long_offset disp;

            sym = va_arg( args, dw_sym_handle );
            disp = va_arg( args, dw_addr_offset );
            DoReloc( back, sym, disp );
//          DFFEPtrRef( (cg_sym_handle)sym, disp );
            break;
          }
        default:
            Zoiks( back, ZOIKS_107 ); /* Unknown reloc */
            break;
        }
    SetOP( back, old_segid ); }
    va_end( args );
}


static void *CLIAlloc( BackRun& back, size_t size )
{
    void        *p;

    p = CGAlloc( back, size );
    return( p );
}


static void CLIFree( BackRun& back, void *p )
{
    CGFree( back, p );
}

static back_handle  MakeLabel( BackRun& back )
{
    back_handle bck;

    bck = BENewBack( back, NULL );
    bck->segid = AskOP(back);
    return( bck );
}

void    DFInitDbgInfo( BackRun& back )
/****************************
 * called after ObjInit
 */
{
    back.dfsyms_file.CurrFNo = 0;
    back.dfsyms_file.CcuDef = false;
    back.dfsyms.Client = NULL;
}

static dw_langnum SetDwarfLang( BackRun& back )
{
    dw_langnum  ret;
    const char  *name;
    int         index;

    ret = DW_LANG_C89;
    name = (const char *)FEAuxInfo( *back.front, NULL, FEINF_SOURCE_LANGUAGE );
    for( index = 0; index < MAX_LANG; ++index ) {
        if( strcmp( name, LangNames[index].name ) == 0 ) {
            ret = LangNames[index].dwarf_lang;
            break;
        }
    }
    return( ret );
}

static  void    InitSegBck( BackRun& back )
/********************************/
{
    dw_sectnum  i;
    segment_id  old_segid;
    back_handle bck;

    old_segid = AskOP(back);
    for( i = static_cast<dw_sectnum>(0); i < DW_DEBUG_MAX; ++i ) {
        SetOP( back, back.dfsyms_file.DwarfSegs[i].segid );
        bck = MakeLabel(back);
        bck->segid = back.dfsyms_file.DwarfSegs[i].segid;
        back.dfsyms_file.DwarfSegs[i].bck = bck;
        DataLabel( back, bck->lbl );
    }
    SetOP( back, old_segid );
}

static  void    InitLineSegBck( BackRun& back )
/************************************/
{
    back_handle bck;

    { segment_id old_segid = ChangeOP( back, back.dfsyms_file.DwarfSegs[DW_DEBUG_LINE].segid );
        bck = MakeLabel(back);
        bck->segid = back.dfsyms_file.DwarfSegs[DW_DEBUG_LINE].segid;
        back.dfsyms_file.DwarfSegs[DW_DEBUG_LINE].bck = bck;
        DataLabel( back, bck->lbl );
    SetOP( back, old_segid ); }
}

static  void    FiniSegBck( BackRun& back )
/********************************/
{
    int         i;
    back_handle bck;

    for( i = 0; i < DW_DEBUG_MAX; ++i ) {
        bck = back.dfsyms_file.DwarfSegs[i].bck;
        BEFreeBack( back, bck );
    }
}

static  void    FiniLineSegBck( BackRun& back )
/************************************/
{
    back_handle bck;

    bck = back.dfsyms_file.DwarfSegs[DW_DEBUG_LINE].bck;
    BEFreeBack( back, bck );
}

static int InitCU( BackRun& back, dw_cu_info *cu )
{
    const type_def  *tipe_addr;

    cu->source_filename = static_cast<const char *>(FEAuxInfo( *back.front, NULL, FEINF_SOURCE_NAME ));
    cu->directory = "";
    cu->inc_list = NULL;
    cu->inc_list_len = 0;
    tipe_addr = TypeAddress( back, TY_NEAR_POINTER );
    cu->offset_size = tipe_addr->length;
#if (_TARGET & _TARG_8086)
    cu->segment_size = 2;
#elif (_TARGET & _TARG_80386)
    cu->segment_size = ( IsntTargetModel( back, CGSW_X86_FLAT_MODEL ) ) ? 2 : 0;
#endif
    switch( GetMemModel(back) ) {
    case 'h':
        cu->model = DW_MEM_MODEL_huge;
        break;
    case 'l':
        cu->model = DW_MEM_MODEL_large;
        break;
    case 'f':
        cu->model = DW_MEM_MODEL_flat;
        break;
    case 's':
        cu->model = DW_MEM_MODEL_small;
        break;
    case 'c':
        cu->model = DW_MEM_MODEL_compact;
        break;
    case 'm':
        cu->model = DW_MEM_MODEL_medium;
        break;
    default:
        cu->model = DW_MEM_MODEL_none;
        break;
    }
    return( 0 );
}

void    DFSymRange( BackRun& back, cg_sym_handle sym, offset size )
/***************************************************
 * I don't see what this is good for. The aranges for any
 * comdat symbols will be taken care of by DFSegRange().
 * Running this code may produce overlapping aranges that
 * confuse the hell out of the debugger. However, not running
 * this may cause debug information to be missing... call it
 * a FIXME
 */
{
    if( IsModel( back, CGSW_GEN_DBG_LOCALS )
      || IsModel( back, CGSW_GEN_DBG_TYPES ) ) {
        back.dfsyms_file.ARange = FEBack( *back.front, sym );
        DWAddress( back, back.dfsyms.Client, size );
    }
}

void    DFSegRange( BackRun& back )
/*************************
 * do arange for the current segment
 */
{
    back_handle bck;
    offset      off;
    offset      size;

    if( IsModel( back, CGSW_GEN_DBG_LOCALS )
      || IsModel( back, CGSW_GEN_DBG_TYPES ) ) {
        size = AskMaxSize(back);
        if( size > 0 ) {
            bck = MakeLabel(back);
            off = AskLocation(back);
            SetLocation( back, 0 );
            DataLabel( back, bck->lbl );
            SetLocation( back, off );
            back.dfsyms_file.ARange = bck;
            DWAddress( back, back.dfsyms.Client, size );
            BEFreeBack( back, bck );
        }
    }
}

void    DFBegCCU( BackRun& back, segment_id code_segid, dw_sym_handle dbg_pch )
/***************************************************************
 * Call when codeseg has been defined
 */
{
    dw_cu_info      cu;
    back_handle     bck;

    /* unused parameters */ (void)code_segid;

    if( IsntModel( back, CGSW_GEN_DBG_LOCALS )
      && IsntModel( back, CGSW_GEN_DBG_TYPES ) ) {
        return;
    }
    if( back.dfsyms_file.CcuDef ) {
        InitCU( back, &cu );
        cu.dbg_pch = dbg_pch;
        cu.flags = DW_CU_FLAG_NONE;
        back.dfsyms_file.Pc_Low = NULL;
        back.dfsyms_file.Pc_High = NULL;
        bck = NULL;
        back.dfsyms_file.Comp_High = back.dfsyms_file.Pc_High;
        DWBeginCompileUnit( back, back.dfsyms.Client, &cu );
        if( cu.flags & DW_CU_FLAG_CONTIGUOUS ) {
            BEFreeBack( back, bck );
        }
    } else {
        back.dfsyms_file.CcuDef = true;
    }
}

static const char *SetDwarfProducer( void )
{
    const char  *name;

    name = "";
    return( name );
}

void    DFObjInitDbgInfo( BackRun& back )
/*******************************
 * called by objinit to init segments and dwarf writing library
 */
{
    static const dw_funcs cli_funcs = {
        CLIReloc,
        CLIWrite,
        CLISeek,
        CLITell,
        CLIAlloc,
        CLIFree
    };
    dw_init_info    info;
    cg_sym_handle   abbrev_sym;
    cg_sym_handle   debug_pch;
    fe_attr         attr;

    if( IsntModel( back, CGSW_GEN_DBG_LOCALS )
      && IsntModel( back, CGSW_GEN_DBG_TYPES ) ) {
        return;
    }
    info.big_endian = IsModel( back, CGSW_GEN_OBJ_ENDIAN_BIG );
    info.compiler_options = DW_CM_DEBUGGER;
    info.abbrev_sym = 0;
    info.producer_name = SetDwarfProducer();
    info.language = SetDwarfLang(back);
    if( setjmp( info.exception_handler ) == 0 ) {
        info.funcs = cli_funcs;
        InitSegBck(back); // start each seg with a ref label
        if( IsModel( back, CGSW_GEN_DBG_PREDEF ) ) {
            abbrev_sym = static_cast<cg_sym_handle>(FEAuxInfo( *back.front, NULL, FEINF_DBG_PREDEF_SYM ));
            info.abbrev_sym = (dw_sym_handle)abbrev_sym;
            attr = FEAttr( *back.front, abbrev_sym );
            if( (attr & FE_IMPORT) ) {
                info.compiler_options |= DW_CM_ABBREV_PRE;
            } else {
                back_handle bck;

                info.compiler_options |= DW_CM_ABBREV_GEN;
                bck = FEBack( *back.front, abbrev_sym ); // dump out export label
                bck->segid = back.dfsyms_file.DwarfSegs[DW_DEBUG_ABBREV].segid;
                { segment_id old_segid = ChangeOP( back, back.dfsyms_file.DwarfSegs[DW_DEBUG_ABBREV].segid );
                    DataLabel( back, bck->lbl );
                SetOP( back, old_segid ); }
            }
        }
        debug_pch = static_cast<cg_sym_handle>(FEAuxInfo( *back.front, NULL, FEINF_DBG_PCH_SYM ));
        if( debug_pch != NULL ) {
            attr = FEAttr( *back.front, debug_pch );
            if( (attr & FE_IMPORT) == 0 ) {
                back_handle bck;

                bck = FEBack( *back.front, debug_pch );
                bck->segid = back.dfsyms_file.DwarfSegs[DW_DEBUG_INFO].segid;
                { segment_id old_segid = ChangeOP( back, back.dfsyms_file.DwarfSegs[DW_DEBUG_INFO].segid );
                    DataLabel( back, bck->lbl );
                SetOP( back, old_segid ); }
                debug_pch = NULL;
            }
        }
        back.dfsyms.Client = DWInit( back, &info );
        if( back.dfsyms.Client == NULL ) {
            Zoiks( back, ZOIKS_107 ); /* Bad */
        }
        DFBegCCU( back, AskCodeSeg(back), (dw_sym_handle)debug_pch );
    } else {
        Zoiks( back, ZOIKS_107 ); /* Big Error */
    }
}

void    DFObjLineInitDbgInfo( BackRun& back )
/***********************************
 * called by objinit to init segments and dwarf writing library
 */
{
    static const dw_funcs cli_funcs = {
        CLIReloc,
        CLIWrite,
        CLISeek,
        CLITell,
        CLIAlloc,
        CLIFree
    };
    dw_init_info    info;
    dw_cu_info      cu;

    info.big_endian = IsModel( back, CGSW_GEN_OBJ_ENDIAN_BIG );
    info.compiler_options = DW_CM_DEBUGGER;
    info.abbrev_sym = 0;
    info.producer_name = SetDwarfProducer();
    info.language = SetDwarfLang(back);
    if( setjmp( info.exception_handler ) == 0 ) {
        info.funcs = cli_funcs;
        InitLineSegBck(back); // start each seg with a ref label
        back.dfsyms.Client = DWInit( back, &info );
        if( back.dfsyms.Client == NULL ) {
            Zoiks( back, ZOIKS_107 ); /* Bad */
        }
        InitCU( back, &cu );
        cu.dbg_pch = NULL;
        cu.flags = DW_CU_FLAG_NONE;
        DWInitDebugLine( back, back.dfsyms.Client, &cu );
    } else {
        Zoiks( back, ZOIKS_107 ); /* Big Error */
    }
}


pointer _CGAPI DFClient( BackRun& back )
/******************************
 * return the client handle
 */
{
    return( back.dfsyms.Client );
}

void _CGAPI DFDwarfLocal( BackRun& back, pointer client, pointer locid, pointer sym )
/*********************************************************************
 * TODO: maybe this should be some sort of call back
 * add to location expr where local sym is
 */
{
    name        *tmp;
    type_length offset;

    tmp = DeAlias( AllocUserTemp( back, sym, XX ) );
    offset = NewBase( back, tmp );
    DWLocOp( back, static_cast<dw_client>(client), static_cast<dw_loc_id>(locid), DW_LOC_fbreg, offset );
}

void    DFFiniDbgInfo( void )
{
}


void    DFObjFiniDbgInfo( BackRun& back, offset codesize )
/*****************************************/
{
    offset          here;
    back_handle     bck;

    if( IsModel( back, CGSW_GEN_DBG_LOCALS )
      || IsModel( back, CGSW_GEN_DBG_TYPES ) ) {
        bck = back.dfsyms_file.Comp_High;
        if( bck != NULL ) {
            { segment_id old_segid = ChangeOP( back, AskCodeSeg(back) );
                OutLabel( back, bck->lbl );
            SetOP( back, old_segid ); }
            BEFreeBack( back, bck );
            back.dfsyms_file.Comp_High = NULL;
        }
        DWEndCompileUnit( back, back.dfsyms.Client );
        DWFini( back, back.dfsyms.Client );
        { segment_id old_segid = ChangeOP( back, back.dfsyms_file.UnitSize->segid );
            here = AskLocation(back);
            SetLocation( back, back.dfsyms_file.UnitSize->offset );
            DataLong( back, codesize );
            SetLocation( back, here );
        SetOP( back, old_segid ); }
        FiniSegBck(back);
    }
}

void    DFObjLineFiniDbgInfo( BackRun& back )
/**********************************/
{
    DWFiniDebugLine( back, back.dfsyms.Client );
    DWFini( back, back.dfsyms.Client );
    FiniLineSegBck(back);
}

void     DFLineNum( BackRun& back, cue_state *state, offset lc )
/***********************************************/
{
    const char  *fname;

    if( NeedBaseSet(back) ) {
        back_handle bck;

        bck = MakeLabel(back);
        OutLabel( back, bck->lbl );
        DWLineAddr( back, back.dfsyms.Client, (dw_sym_handle)bck, lc );
#if (_TARGET & _TARG_8086)
        DWLineSeg( back, back.dfsyms.Client, (dw_sym_handle)bck );
#elif (_TARGET & _TARG_80386)
        if( IsntTargetModel( back, CGSW_X86_FLAT_MODEL ) ) {
            DWLineSeg( back, back.dfsyms.Client, (dw_sym_handle)bck );
        }
#endif
        BEFreeBack( back, bck );
    }
    if( state->fno != back.dfsyms_file.CurrFNo ) {
        fname = SrcFNoFind( back, state->fno );
        DWSetFile( back, back.dfsyms.Client, fname );
        back.dfsyms_file.CurrFNo = state->fno;
    }
    DWLineNum( back, back.dfsyms.Client, DW_LN_STMT, state->line, state->col, lc );
}


static  dw_loc_handle   SegLoc( BackRun& back, cg_sym_handle sym )
/*************************************************/
{
    dw_loc_id       locid;
    dw_loc_handle   df_loc;

    locid = DWLocInit( back, back.dfsyms.Client );
    DWLocSegment( back, back.dfsyms.Client, locid, (dw_sym_handle)sym );
    df_loc = DWLocFini( back, back.dfsyms.Client, locid );
    return( df_loc );
}

void    DFGenStatic( BackRun& back, cg_sym_handle sym, dbg_loc loc )
/***************************************************/
{
    dw_flags        flags;
    fe_attr         attr;
    const char      *name;
    dw_loc_handle   dw_loc;
    dw_loc_handle   dw_segloc;
    dw_handle       obj;
    dbg_type        dbtype;

    attr = FEAttr( *back.front, sym );
    if( attr & FE_GLOBAL ) {
        flags = DW_FLAG_GLOBAL;
    } else {
        flags = DW_FLAG_NONE;
    }
    name = FEName( *back.front, sym );
    dw_segloc = NULL;
#if (_TARGET & _TARG_8086)
    if( attr & FE_STATIC ) {
        dw_segloc = SegLoc( back, sym );
    }
#elif (_TARGET & _TARG_80386)
    if( attr & FE_STATIC ) {
        if( IsntTargetModel( back, CGSW_X86_FLAT_MODEL ) ) {
            dw_segloc = SegLoc( back, sym );
        }
    }
#endif
    dbtype = FEDbgType( *back.front, sym ); /* causes name side effects */
    dw_loc = DBGLoc2DF( back, loc );
    obj = DWVariable( back, back.dfsyms.Client, dbtype, dw_loc, 0, dw_segloc, name, 0, flags );
    if( attr &  FE_GLOBAL ) {
        name = FEName( *back.front, sym );
        DWPubname( back, back.dfsyms.Client, obj, name );
    }
    if( dw_loc != NULL ) {
        DWLocTrash( back, back.dfsyms.Client, dw_loc );
    }
    if( dw_segloc != NULL ) {
        DWLocTrash( back, back.dfsyms.Client, dw_segloc );
    }
}

void    DFTypedef( BackRun& back, const char *nm, dbg_type tipe )
/*************************************************
 * emit a user typedef
 */
{
     DWTypedef( back, back.dfsyms.Client, tipe, nm, 0, static_cast<dw_flags>(0) );
}

static  void    GenRetSym( BackRun& back, dbg_loc loc, dbg_type tipe )
/*****************************************************/
{
    dw_loc_handle   dw_loc;

    dw_loc = DBGLoc2DF( back, loc );
    if( dw_loc != NULL ) {
        DWVariable( back, back.dfsyms.Client, tipe, dw_loc,
                   0, NULL, ".return", 0, DW_FLAG_ARTIFICIAL );
        DWLocTrash( back, back.dfsyms.Client, dw_loc );
    }
}

static void    SymParm( BackRun& back, cg_sym_handle sym, dw_loc_handle loc, dw_loc_handle entry )
/*********************************************************************************/
{
//    fe_attr         attr;
    const char      *name;
//    dw_handle       obj;
    dbg_type        dbtype;

//    attr = FEAttr( sym );
    dbtype = FEDbgType( *back.front, sym ); /* causes FEName side effects */
    name = FEName( *back.front, sym );
//    obj = DWFormalParameter( Client, dbtype, loc, entry,  name, DW_DEFAULT_NONE );
    DWFormalParameter( back, back.dfsyms.Client, dbtype, loc, entry,  name, DW_DEFAULT_NONE );
}

/*
 * Coming out of optimizer queue
 */

static dw_loc_handle  RetLoc( BackRun& back, uint_32 ret_offset )
/*************************************************
 * make a loc for return address
 */
{
    dw_loc_id       locid;
    dw_loc_handle   df_loc;

    locid = DWLocInit( back, back.dfsyms.Client );
    DWLocOp( back, back.dfsyms.Client, locid, DW_LOC_fbreg, ret_offset );
    df_loc = DWLocFini( back, back.dfsyms.Client, locid );
    return( df_loc );
}

static dw_loc_handle  FrameLoc( BackRun& back )
/*
 * make a loc for frame address
 */
{
    uint            dsp;
    dw_loc_id       locid;
    dw_loc_handle   df_loc;

    locid = DWLocInit( back, back.dfsyms.Client );
    dsp = DFDisplayReg(back);
    DWLocReg( back, back.dfsyms.Client, locid, dsp );
    df_loc = DWLocFini( back, back.dfsyms.Client, locid );
    return( df_loc );
}
static dw_loc_id StkLoc( BackRun& back, uint_32 stk_offset, dw_loc_id locid )
/*************************************************************
 * make a loc for stack  address
 */
{
    uint            stk;

    stk = DFStkReg(back);
    DWLocOp( back, back.dfsyms.Client, locid, DW_LOC_breg, stk, stk_offset );
    return( locid );
}

static  dbg_local *UnLinkLoc( dbg_local **owner, cg_sym_handle sym )
/*******************************************************************
 * unlink dbg_local with sym from owner
 */
{
    dbg_local           *curr;

    for( ; (curr = *owner) != NULL; owner = &(*owner)->link ) {
        if( curr->sym == sym ) {
            *owner = curr->link;
            break;
        }
    }
    return( curr );
}


static  void GenParmLoc( BackRun& back, dbg_local *parm, dbg_local **locals )
{
    dbg_local      *alt;
    dw_loc_handle   dw_loc;
    dw_loc_handle   dw_entry;
    dw_loc_id       df_locid;

    df_locid = DWLocInit( back, back.dfsyms.Client );
    if( parm->loc->class_ == LOC_CONST_4 ) {
        df_locid = StkLoc( back, parm->loc->u.val, df_locid );
    } else {
        df_locid = DBGLoc2DFCont( back, parm->loc, df_locid );
    }
    alt = UnLinkLoc( locals, static_cast<cg_sym_handle>(parm->sym) );
    if( alt != NULL ) {
        dbg_loc     alt_loc;
        dbg_loc     *alt_lnk;

        alt_loc = alt->loc;      // skip down to loc base
        alt_lnk = &alt_loc->next;
        if( *alt_lnk != NULL ) {
            for( ;; ) {
                alt_loc = *alt_lnk;
                if( alt_loc->next == NULL )
                    break;
                alt_lnk = &alt_loc->next;
            }
            *alt_lnk = NULL;   // don't use base
            df_locid = DBGLoc2DFCont( back, alt->loc, df_locid );
            *alt_lnk = alt_loc;
        }
        dw_loc = DBGLoc2DF( back, alt->loc );
        DBLocFini( back, alt->loc );
        CGFree( back, alt );
    } else {
        dw_loc = DBGLoc2DF( back, NULL );
    }
    dw_entry = DWLocFini( back, back.dfsyms.Client, df_locid );
    SymParm( back, static_cast<cg_sym_handle>(parm->sym), dw_loc, dw_entry );
    DWLocTrash( back, back.dfsyms.Client, dw_loc );
    DWLocTrash( back, back.dfsyms.Client, dw_entry );
}


static  void    DumpLocals( BackRun& back, dbg_local *local )
/********************************************/
{
    dbg_local   *next;
    dbg_type    tipe;

    for( ; local != NULL; local = next ) {
        next = local->link;
        switch( local->kind ) {
        case DBG_SYM_VAR:
            DFGenStatic( back, static_cast<cg_sym_handle>(local->sym), local->loc );
            DBLocFini( back, local->loc );
            break;
        case DBG_SYM_TYPE:
            tipe = FEDbgType( *back.front, static_cast<cg_sym_handle>(local->sym) );
            break;
        case DBG_SYM_TYPEDEF:
            tipe = FEDbgType( *back.front, static_cast<cg_sym_handle>(local->sym) );
            DFTypedef( back, FEName( *back.front, static_cast<cg_sym_handle>(local->sym) ), tipe );
            break;
        }
        CGFree( back, local );
    }
}

void    DFProEnd( BackRun& back, dbg_rtn *rtn, offset lc )
/*****************************************/
{
    cg_sym_handle       sym;
    dbg_type            tipe;
    fe_attr             attr;
    const char          *name;
    dw_flags            flags;
    dw_loc_handle       dw_retloc;
    dw_loc_handle       dw_frameloc;
    dw_loc_handle       dw_segloc;
    dbg_local           *parm;
    dbg_local           *next;
    dw_handle           obj;
    sym_access          *access;

    /* unused parameters */ (void)lc;

    sym = AskForLblSym( back, static_cast<label_handle>(back.data.CurrProc->label) );
    tipe = FEDbgRetType( *back.front, sym );
    flags = DW_FLAG_NONE;
    if( (call_class_target)(pointer_uint)FindAuxInfoSym( back, sym, FEINF_CALL_CLASS_TARGET ) & FECALL_X86_FAR_CALL ) {
  #if _TARGET & _TARG_8086
        flags = DW_FLAG_PTR_TYPE_FAR16;
  #elif _TARGET & _TARG_80386
        flags = DW_FLAG_PTR_TYPE_FAR32;
  #endif
    }
    attr = FEAttr( *back.front, sym );
    name = FEName( *back.front, sym );
    if( attr & FE_GLOBAL ) {
        flags |= DW_FLAG_GLOBAL;
    } else {
        flags |= DW_FLAG_SUB_STATIC;
    }
    if( attr & FE_COMPILER ) {
        flags |= DW_FLAG_ARTIFICIAL;
    }
    flags |= DW_FLAG_PROTOTYPED;
    access = static_cast<sym_access *>(FEAuxInfo( *back.front, sym, FEINF_DBG_SYM_ACCESS ));
    if( access != NULL ) {
        if( *access == SYM_ACC_PUBLIC ) {
            flags |= DW_FLAG_PUBLIC;
        } else if( *access == SYM_ACC_PROTECTED ) {
            flags |= DW_FLAG_PROTECTED;
        } else if( *access == SYM_ACC_PRIVATE ) {
            flags |= DW_FLAG_PRIVATE;
        }
    }
    DBLocFini( back, rtn->obj_loc );
#if (_TARGET & _TARG_8086)
    dw_retloc = RetLoc( back, rtn->ret_offset );
    dw_frameloc = FrameLoc(back);
    dw_segloc = SegLoc( back, sym );
#elif (_TARGET & _TARG_80386)
    dw_retloc = RetLoc( back, rtn->ret_offset );
    dw_frameloc = FrameLoc(back);
    dw_segloc = ( IsntTargetModel( back, CGSW_X86_FLAT_MODEL ) ) ? SegLoc( back, sym ) : NULL;
#endif
    rtn->end_lbl = MakeLabel(back);
    back.dfsyms_file.Pc_Low  = FEBack( *back.front, sym );
    back.dfsyms_file.Pc_High = rtn->end_lbl;
    obj = DWBeginSubroutine( back, back.dfsyms.Client, 0, tipe, dw_retloc,
                     dw_frameloc, NULL, rtn->obj_type,
                     dw_segloc, name, rtn->pro_size, flags );
    if( attr &  FE_GLOBAL ) {
        if( rtn->obj_type != DBG_NIL_TYPE ) {
            name = static_cast<const char *>(FEAuxInfo( *back.front, sym, FEINF_CLASS_APPENDED_NAME ));
        } else {
            name = FEName( *back.front, sym );
        }
        DWPubname( back, back.dfsyms.Client, obj, name );
    }
    if( dw_retloc != NULL ) {
        DWLocTrash( back, back.dfsyms.Client, dw_retloc );
    }
    if( dw_frameloc != NULL ) {
        DWLocTrash( back, back.dfsyms.Client, dw_frameloc );
    }
    if( dw_segloc != NULL ) {
        DWLocTrash( back, back.dfsyms.Client, dw_segloc );
    }
    for( parm = rtn->parms; parm != NULL; parm = next ) {
        next = parm->link;
        if( parm->sym != NULL ) {
            GenParmLoc( back, parm, &rtn->rtn_blk->locals );
        }
        DBLocFini( back, parm->loc );
        CGFree( back, parm );
    }
    if( rtn->return_loc != NULL ) {
        GenRetSym( back, rtn->return_loc, tipe );
        DBLocFini( back, rtn->return_loc );
    }
    DFBlkBeg( back, rtn->rtn_blk, lc );
//   DumpLocals( rtn->blk->locals );
}

void    DFBlkBeg( BackRun& back, dbg_block *blk, offset lc )
/*******************************************/
{
    back_handle bck;

    /* unused parameters */ (void)lc;

    bck = MakeLabel(back);
    blk->end_lbl = MakeLabel(back);
    OutLabel( back, bck->lbl );
    back.dfsyms_file.Pc_Low  = bck;
    back.dfsyms_file.Pc_High = blk->end_lbl;
    DWBeginLexicalBlock( back, back.dfsyms.Client, NULL, NULL );
    BEFreeBack( back, bck );
    DumpLocals( back, blk->locals );
}

void    DFBlkEnd( BackRun& back, dbg_block *blk, offset lc )
/*******************************************/
{
    back_handle bck;

    /* unused parameters */ (void)lc;

    bck = blk->end_lbl;
    OutLabel( back, bck->lbl );
    BEFreeBack( back, bck );
    DWEndLexicalBlock( back, back.dfsyms.Client );
}

void    DFEpiBeg( BackRun& back, dbg_rtn *rtn, offset lc )
/*****************************************/
{
    DFBlkEnd( back, rtn->rtn_blk, lc );
}

void    DFRtnEnd( BackRun& back, dbg_rtn *rtn, offset lc )
/*****************************************/
{
    back_handle bck;

    /* unused parameters */ (void)lc;

    bck = rtn->end_lbl;
    OutLabel( back, bck->lbl );
    BEFreeBack( back, bck );
    DWEndSubroutine( back, back.dfsyms.Client );
}

void    DFSetSection( BackRun& back, dw_sectnum sect, back_handle bck, segment_id segid )
/************************************************************************/
{
    back.dfsyms_file.DwarfSegs[sect].segid = segid;
    back.dfsyms_file.DwarfSegs[sect].bck = bck;
}
