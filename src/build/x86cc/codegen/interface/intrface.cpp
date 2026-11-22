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
* Description:  Code generator public interface.
*
****************************************************************************/


#include "_cgstd.h"
#include <ctype.h>
#include "coderep.h"
#include "seldef.h"
#include "memcheck.h"
#include "cgmem.h"
#include "tree.h"
#include "zoiks.h"
#include "cgauxinf.h"
#include "data.h"
#include "types.h"
#include "bldins.h"
#include "utils.h"
#include "objout.h"
#include "treeprot.h"
#include "makeaddr.h"
#include "patch.h"
#include "dbsyms.h"
#include "objio.h"
#include "blips.h"
#include "bldsel.h"
#include "intrface.h"
#include "opttell.h"
#include "typemap.h"
#include "bldcall.h"
#include "generate.h"
#include "bckptr.h"
#include "inline.h"
#include "memout.h"
#include "memmgt.h"
#include "bgcall.h"
#include "savings.h"
#include "regsave.h"
#include "i64.h"
#include "trmem.h"
#include "feprotos.h"
#include "cgprotos.h"
#include "_cfloat.h"

#ifdef DEVBUILD
#include "echoapi.h"
#include <stdlib.h>
#endif
#include "backrun.hpp"


#define MAX_BCK_INFO    1000    // number of bck_info's per carve block

typedef struct uback_info {
    union {
        bck_info            bck;
        struct uback_info   *link;
    } u;
} uback_info;

typedef struct bck_info_block {
    struct bck_info_block       *next;
    uback_info                  bck_infos[MAX_BCK_INFO];
} bck_info_block;

#ifdef QNX_FLAKEY
unsigned        OrigModel;
#endif


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%                                              %%*/
/*%   Module interface                           %%*/
/*%                                              %%*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/


namespace {

/* the statics of this file, one set for each run of the compiler */
}  // namespace
struct IntrfaceState {
    uback_info *BckInfoHead;   // linked list of available bck_info's
    bck_info_block *BckInfoCarveHead;   // list of big blocks of bck_info's
    bool memStarted = false;
    back_handle NewBackReturn = NULL;
    cg_name CGDuplicateArray[2];
};
namespace {

}  // namespace


void _CGAPI     BEMemInit( BackRun& back, pointer trmemhdl )
/*******************************************/
{
    back.intrface->BckInfoHead = NULL;
    back.intrface->BckInfoCarveHead = NULL;
    InitBlip();
    CGMemInit( back, trmemhdl );
    back.cfloat.cgh.alloc = CGAlloc;
    back.cfloat.cgh.free = CGFree;
    CFInit( &back.cfloat.cgh );
    back.intrface->memStarted = true;
}

cg_init_info _CGAPI     BEInitCg( BackRun& back, cg_switches switches,
                                    cg_target_switches platform,
                                    uint optsize,
                                    proc_revision proc )
/**************************************************************/
{
    cg_init_info        cg_info;

    back.data.OptForSize = (byte)optsize;
    back.model.CGProcessorVersion = proc; /* so _CPULevel works */
#if _TARGET & _TARG_8086
    /* if it ain't a 386 or better, FS and GS aren't there */
    if( !_CPULevel( CPU_386 ) ) {
        platform &= ~(CGSW_X86_FLOATING_FS | CGSW_X86_FLOATING_GS);
    }
#elif  _TARGET & _TARG_80386
    if( !_CPULevel( CPU_386 ) ) {
        SET_CPU( proc, CPU_386 );
    }
    proc &= ~FPU_EMU;   /* don't need funny fixups for 386 */
#endif
    back.model.Model = switches;
    back.model.TargetModel = platform;
#ifdef QNX_FLAKEY
    OrigModel = switches;
#endif
#ifdef DEVBUILD
    EchoAPIInit();
    EchoAPI( "BEInit( %x, %x, %i, %x )\n", switches, platform, optsize, proc );
    // must be after Model is set and before InitDBGInfo call
#endif
    back.model.CGProcessorVersion = proc;
    InitWeights( back, optsize );
    InitSegDefs(back);
    InitDbgInfo(back);
    TypeInit(back);
    TInit(back);
    if( !CGOpenf(back) ) {
        cg_info.revision = 0;
        cg_info.target = 0;
    } else {
        cg_info.revision = II_REVISION;
#if _TARGET & _TARG_8086
        cg_info.target = II_TARG_8086;
#elif _TARGET & _TARG_80386
        cg_info.target = II_TARG_80386;
#elif _TARGET & _TARG_370
        cg_info.target = II_TARG_370; /* NYI -- for now */
#elif _TARGET & _TARG_AXP
        cg_info.target = II_TARG_AXP;
#elif _TARGET & _TARG_PPC
        cg_info.target = II_TARG_PPC;
#elif _TARGET & _TARG_MIPS
        cg_info.target = II_TARG_MIPS;
#endif
    }
    return( cg_info );
}

cg_init_info _CGAPI     BEInit( BackRun& back, cg_switches switches,
                                cg_target_switches platform,
                                uint optsize, proc_revision proc )
/****************************************************************/
{
    BEMemInit( back, NULL );
    return( BEInitCg( back, switches, platform, optsize, proc ) );
}

void _CGAPI     BEStart( BackRun& back )
/*****************************/
{
#ifdef DEVBUILD
    EchoAPI( "BEStart()\n" );
#endif
    InitCG(back);
}

void _CGAPI     BEStop( BackRun& back )
/****************************/
{
#ifdef DEVBUILD
    EchoAPI( "BEStop()\n" );
#endif
    FiniCG(back);
}

void _CGAPI     BEAbort( BackRun& back )
/*****************************/
{
#ifdef DEVBUILD
    EchoAPI( "BEAbort()\n" );
#endif
    AbortCG(back);
}

TRMEMAPI( BEMemAlloc )
pointer _CGAPI  BEMemAlloc( BackRun& back, unsigned size )
/*****************************************/
{
#if defined( TRMEM )
    return( _CGAlloc( size, _TRMEM_WHO( 1 ) ) );
#else
    return( CGAlloc( back, size ) );
#endif
}

TRMEMAPI( BEMemFree )
void _CGAPI     BEMemFree( BackRun& back, pointer ptr )
/**************************************/
{
#if defined( TRMEM )
    _CGFree( ptr, _TRMEM_WHO( 2 ) );
#else
    CGFree( back, ptr );
#endif
}

void _CGAPI     BEMemFini( BackRun& back )
/*******************************/
{
    CFFini( back, &back.cfloat.cgh );
    CGMemFini(back);
}

static void FreeBckInfoCarveBlocks( BackRun& back )
{
    bck_info_block      *carve_block;

    for( ; (carve_block = back.intrface->BckInfoCarveHead) != NULL; ) {
        back.intrface->BckInfoCarveHead = carve_block->next;
        CGFree( back, carve_block );
    }
    back.intrface->BckInfoHead = NULL;
}

void _CGAPI     BEFiniCg( BackRun& back )
/******************************/
{
#ifdef DEVBUILD
    EchoAPI( "BEFiniCg()\n" );
    EchoAPIFini();
#endif
    FreeBckInfoCarveBlocks(back);
    FiniDbgInfo(back);
    TypeFini(back);
    TFini(back);
    FiniBlip();
}

void _CGAPI     BEFini( BackRun& back )
/****************************/
{
    BEFiniCg(back);
    BEMemFini(back);
}

segment_id _CGAPI   BEGetSeg( BackRun& back )
/**********************************/
{
#ifdef DEVBUILD
    segment_id  segid;

    EchoAPI( "BEGetSeg()" );
    segid = AskOP();
    return EchoAPIHexReturn( segid );
#else
    return( AskOP(back) );
#endif
}

segment_id _CGAPI   BESetSeg( BackRun& back, segment_id segid )
/**********************************************/
{
#ifdef DEVBUILD
    EchoAPI( "BESetSeg( %x )", segid );
    segid = ChangeOP( segid );
    return EchoAPIHexReturn( segid );
#else
    return( ChangeOP( back, segid ) );
#endif
}

void _CGAPI BEDefSeg( BackRun& back, segment_id segid, seg_attr attr, cchar_ptr str, uint algn )
/*******************************************************************************/
{
#ifdef DEVBUILD
    EchoAPI( "BEDefSeg( %x, %x, %c, %i )\n", segid, attr, str, algn );
#endif
    DefSegment( back, segid, attr, str, algn, false );
}

void _CGAPI     BEFlushSeg( BackRun& back, segment_id segid )
/********************************************/
{
#ifdef DEVBUILD
    EchoAPI( "BEFlushSeg( %x )\n", segid );
#endif
    FlushOP( back, segid );
}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%                                              %%*/
/*%   Handle creation/deletion                   %%*/
/*%                                              %%*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

label_handle _CGAPI BENewLabel( BackRun& back )
/************************************/
{
#ifdef DEVBUILD
    label_handle    retn;

    EchoAPI( "BENewLabel()" );
    retn = AskForNewLabel();
    EchoAPI( " -> %L\n", retn );
    hdlAdd( LABEL_HANDLE, retn );
    return retn;
#else
    return( AskForNewLabel(back) );
#endif
}

void _CGAPI     BEFiniLabel( BackRun& back, label_handle lbl )
/*********************************************/
{
#ifdef DEVBUILD
    EchoAPI( "BEFiniLabel( %L )\n", lbl );
    hdlExists( LABEL_HANDLE, lbl );
    hdlUseOnce( LABEL_HANDLE, lbl );
#endif

    BGFiniLabel( back, lbl );
}


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%                                              %%*/
/*%   Miscellaneous stuff                        %%*/
/*%                                              %%*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/


uint_32 _CGAPI      BEUnrollCount( BackRun& back, uint_32 unroll_count )
/*******************************************************/
{
#ifdef DEVBUILD
    uint_32 retn;
    EchoAPI( "BEUnrollCount( %i )", unroll_count );
    retn = BGUnrollCount( unroll_count );
    return EchoAPIIntReturn( retn );
#else
    return( BGUnrollCount( back, unroll_count ) );
#endif
}


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%                                              %%*/
/*%   Tree patch creation/use/deletion           %%*/
/*%                                              %%*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

patch_handle _CGAPI     BEPatch( BackRun& back )
/*************************************/
{
#ifdef DEVBUILD
    patch_handle patch;

    EchoAPI( "BEPatch()" );
    patch = BGNewPatch();
    EchoAPI( " -> %P\n", patch );
    hdlAdd( PATCH_HANDLE, patch );
    return( patch );
#else
    return( BGNewPatch(back) );
#endif
}

cg_name _CGAPI          CGPatchNode( BackRun& back, patch_handle patch, cg_type tipe )
/*********************************************************************/
{
#ifdef DEVBUILD
    cg_name     retn;

    EchoAPI( "CGPatchNode( %P, %t )", patch, tipe );
    hdlExists( PATCH_HANDLE, patch );
    retn = BGPatchNode( patch, TypeAddress( tipe ) );
    hdlAdd( CG_NAMES, retn );
    return EchoAPICgnameReturn( retn );
#else
    return( BGPatchNode( back, patch, TypeAddress( back, tipe ) ) );
#endif
}

void _CGAPI     BEPatchInteger( BackRun& back, patch_handle patch, int_32 value )
/****************************************************************/
{
#ifdef DEVBUILD
    EchoAPI( "BEPatchInteger( %P, %x )\n", patch, value );
    hdlExists( PATCH_HANDLE, patch );
#endif
    BGPatchInteger( back, patch, value );
}

void _CGAPI     BEFiniPatch( BackRun& back, patch_handle patch )
/***********************************************/
{
#ifdef DEVBUILD
    EchoAPI( "BEFiniPatch( %P )\n", patch );
    hdlUseOnce( PATCH_HANDLE, patch );
#endif

    BGFiniPatch( back, patch );
}


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%                                              %%*/
/*%   Back Handles                               %%*/
/*%                                              %%*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/


name * cg_internal LkAddBackUserTemp( BackRun& back, cg_sym_handle sym, name *temp )
/*******************************************************************/
{
    back_handle bck;

    back.intrface->NewBackReturn = TO_FAKE_BACK( (back_handle)temp );
    bck = FEBack( *back.front, sym );
    back.intrface->NewBackReturn = NULL;
    return( (name *)TO_REAL_BACK( bck ) );
}

name * cg_internal SymBackUserTemp( BackRun& back, cg_sym_handle sym )
/*****************************************************/
{
    back_handle bck;

    bck = FEBack( *back.front, sym );
    return( (name *)TO_REAL_BACK( bck ) );
}

static void AllocMoreBckInfo( BackRun& back )
/**********************************/
{
    uback_info          *p;
    bck_info_block      *carve_block;
    int                 i;

    carve_block = static_cast<bck_info_block *>(CGAlloc( back, sizeof( bck_info_block ) ));
    if( carve_block != NULL ) {
        carve_block->next = back.intrface->BckInfoCarveHead;   // link into big list
        back.intrface->BckInfoCarveHead = carve_block;
        p = &carve_block->bck_infos[0];
        back.intrface->BckInfoHead = p;
        for( i = 0; i < (MAX_BCK_INFO - 1); i++ ) {
            p->u.link = p + 1;
            ++p;
        }
        p->u.link = NULL;
    }
}

back_handle _CGAPI      BENewBack( BackRun& back, cg_sym_handle sym )
/****************************************************/
{
    back_handle bck;

#ifdef DEVBUILD
    EchoAPI( "BENewBack( %s )", sym );
#endif
    if( back.intrface->NewBackReturn == FAKE_NULL ) {
#ifdef DEVBUILD
        EchoAPI( " -> %B\n", NULL );
#endif
        return( NULL );
    }
    if( back.intrface->NewBackReturn != NULL ) {
#ifdef DEVBUILD
    EchoAPI( " -> %B\n", NewBackReturn );
#endif
        return( back.intrface->NewBackReturn );
    }
//    bck = CGAlloc( sizeof( bck_info ) );
    if( back.intrface->BckInfoHead == NULL ) {
        AllocMoreBckInfo(back);
    }
    bck = &back.intrface->BckInfoHead->u.bck;
    back.intrface->BckInfoHead = back.intrface->BckInfoHead->u.link;
    bck->lbl = AskForLabel( back, sym );
    bck->imp = NOT_IMPORTED;
    bck->imp_alt = NOT_IMPORTED;
    if( sym == 0 ) {
        bck->segid = AskBackSeg(back);
    } else {
        bck->segid = FESegID( *back.front, sym );
    }
    if( !IS_REAL_BACK( bck ) )
        _Zoiks( ZOIKS_067 );
#ifdef DEVBUILD
    EchoAPI( " -> %B\n", bck );
#endif
    return( bck );
}

void _CGAPI     BEFiniBack( BackRun& back, back_handle bck )
/*******************************************/
{
#ifdef DEVBUILD
    EchoAPI( "BEFiniBack( %L )\n", bck );
#endif
    if( IS_REAL_BACK( bck ) ) {
        TellNoSymbol( back, bck->lbl );
    }
}

void _CGAPI     BEFreeBack( BackRun& back, back_handle bck )
/*******************************************/
{
#ifdef DEVBUILD
    EchoAPI( "BEFreeBack( %L )\n", bck );
#endif
    if( IS_REAL_BACK( bck ) ) {
//        CGFree( bck );
        uback_info      *p;

        p = (uback_info *)bck;
        p->u.link = back.intrface->BckInfoHead;
        back.intrface->BckInfoHead = p;
    }
}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%                                              %%*/
/*%   Typing information                         %%*/
/*%                                              %%*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

void _CGAPI     BEDefType( BackRun& back, cg_type what, uint align, uint_32 len )
/****************************************************************/
{
#ifdef DEVBUILD
    EchoAPI( "BEDefType( %t, %x, %i )\n", what, align, len );
#endif
    TypeDef( back, what, len, align );
}

void _CGAPI     BEAliasType( BackRun& back, cg_type what, cg_type to )
/*****************************************************/
{
#ifdef DEVBUILD
    EchoAPI( "BEAliasType( %t, %t )\n", what, to );
#endif
    TypeAlias( back, what, to );
}

uint_32 _CGAPI      BETypeLength( BackRun& back, cg_type tipe )
/**********************************************/
{
#ifdef DEVBUILD
    uint_32 retn;
    EchoAPI( "BETypeLength( %t )\n", tipe );
    retn = TypeLength( tipe) ;
    return EchoAPIIntReturn( retn );
#else
    return( TypeLength( back, tipe ) );
#endif
}

uint _CGAPI     BETypeAlign( cg_type tipe )
/*****************************************/
{
#ifdef DEVBUILD
    uint retn;
    EchoAPI( "BETypeAlign( %t )\n", tipe );
    retn = 1;
    return EchoAPIIntReturn( retn );
#else
    /* unused parameters */ (void)tipe;

    return( 1 );
#endif
}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%                                              %%*/
/*%   Procedure/parms/autos declaration          %%*/
/*%                                              %%*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

void _CGAPI     CGProcDecl( BackRun& back, cg_sym_handle sym, cg_type tipe )
/***********************************************************/
{
#ifdef DEVBUILD
    EchoAPI( "\n" );
    EchoAPI( "CGProcDecl( %s, %t )\n", sym, tipe );
#endif
    if( BGInInline(back) ) {
        BGProcInline( back, sym, TypeAddress( back, tipe ) );
    } else {
        TellObjNewProc( back, sym );
        BGProcDecl( back, sym, TypeAddress( back, tipe ) );
    }
}

void _CGAPI     CGParmDecl( BackRun& back, cg_sym_handle sym, cg_type tipe )
/***********************************************************/
{
#ifdef DEVBUILD
    EchoAPI( "CGParmDecl( %s, %t )\n", sym, tipe );
#endif
    if( BGInInline(back) ) {
        BGParmInline( back, sym, TypeAddress( back, tipe ) );
    } else {
        BGParmDecl( back, sym, TypeAddress( back, tipe ) );
    }
}

label_handle _CGAPI CGLastParm( BackRun& back )
/************************************/
{
    label_handle    top;

#ifdef DEVBUILD
    EchoAPI( "CGLastParm()\n" );
#endif
    top = NULL;
    if( _IsntModel( CGSW_GEN_NO_OPTIMIZATION ) ) {
        if( !BGInInline(back) ) {
            top = BENewLabel(back);
            BGGenCtrl( back, O_LABEL, NULL, top, true );
        }
    }
    return( top );
}

void _CGAPI     CGAutoDecl( BackRun& back, cg_sym_handle sym, cg_type tipe )
/***********************************************************/
{
#ifdef DEVBUILD
    EchoAPI( "CGAutoDecl( %s, %t )\n", sym, tipe );
#endif
    BGAutoDecl( back, sym, TypeAddress( back, tipe ) );
}

void _CGAPI     CGReturn( BackRun& back, cg_name name, cg_type tipe )
/****************************************************/
{
    const type_def  *new_tipe;
    an              retv;

#ifdef DEVBUILD
    EchoAPI( "CGReturn( %n, %t )\n\n", name, tipe );
    if( NULL != name ) {
        hdlUseOnce( CG_NAMES, name );
        hdlAllUsed( CG_NAMES );
    }
#endif
    new_tipe = TypeAddress( back, tipe );
    retv = NULL;
    if( name != NULL ) {
        retv = TGReturn( back, name, new_tipe ); /* special TGen()*/
    }
    if( BGInInline(back) ) {
        BGRetInline( back, retv, new_tipe );
    } else {
        BGReturn( back, retv, new_tipe );
    }
}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%                                              %%*/
/*%   Code generator actions                     %%*/
/*%                                              %%*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

/**/
/* Leaf nodes*/
/**/

cg_name _CGAPI CGVarargsBasePtr( BackRun& back, cg_type tipe )
/*********************************************/
{
#ifdef DEVBUILD
    cg_name     retn;

    EchoAPI( "CGVarargsBasePtr( %t )", tipe );
    retn = TGLeaf( BGVarargsBasePtr( TypeAddress( tipe ) ) );
    hdlAdd( CG_NAMES, retn );
    return EchoAPICgnameReturn( retn );
#else
    return( TGLeaf( back, BGVarargsBasePtr( back, TypeAddress( back, tipe ) ) ) );
#endif
}

cg_name _CGAPI CGInteger( BackRun& back, int_32 val, cg_type tipe )
/**************************************************/
{
#ifdef DEVBUILD
    cg_name     retn;

    EchoAPI( "CGInteger( %i, %t )", val, tipe );
    verifyNotUserType( tipe );
    retn = TGLeaf( BGInteger( val, TypeAddress( tipe ) ) );
    hdlAdd( CG_NAMES, retn );
    return EchoAPICgnameReturn( retn );
#else
    return( TGLeaf( back, BGInteger( back, val, TypeAddress( back, tipe ) ) ) );
#endif
}

cg_name _CGAPI CGInt64( BackRun& back, signed_64 val, cg_type tipe )
/***************************************************/
{
#ifdef DEVBUILD
    cg_name     retn;

    EchoAPI( "CGInt64( %x %x, %t )"
           , U64Low( val )
           , U64High( val )
           , tipe );
    verifyNotUserType( tipe );
    retn = TGLeaf( BGInt64( val, TypeAddress( tipe ) ) );
    hdlAdd( CG_NAMES, retn );
    return EchoAPICgnameReturn( retn );
#else
    return( TGLeaf( back, BGInt64( back, val, TypeAddress( back, tipe ) ) ) );
#endif
}

cg_name _CGAPI CGFloat( BackRun& back, cchar_ptr num, cg_type tipe )
/***************************************************/
{
#ifdef DEVBUILD
    cg_name     retn;

    EchoAPI( "CGFloat( %c, %t )", num, tipe );
    verifyNotUserType( tipe );
    retn = TGLeaf( BGFloat( num, TypeAddress( tipe ) ) );
    hdlAdd( CG_NAMES, retn );
    return EchoAPICgnameReturn( retn );
#else
    return( TGLeaf( back, BGFloat( back, num, TypeAddress( back, tipe ) ) ) );
#endif
}

#if 0
cg_name _CGAPI CGBigInt( float_handle f, cg_type tipe )
/*****************************************************/
{
#ifdef DEVBUILD
    cg_name     retn;

    EchoAPI( "CGBigInt( %x, %t )", f, tipe );
    verifyNotUserType( tipe );
    retn = TGLeaf( MakeConst( f, TypeAddress( tipe ) ) );
    hdlAdd( CG_NAMES, retn );
    return EchoAPICgnameReturn( retn );
#else
    return( TGLeaf( MakeConst( f, TypeAddress( tipe ) ) ) );
#endif
}
#endif

cg_name _CGAPI CGFEName( BackRun& back, cg_sym_handle sym, cg_type tipe )
/********************************************************/
{
    cg_name     leaf;


    if( (FEAttr( *back.front, sym ) & FE_DLLIMPORT) && ( FindAuxInfoSym( sym, FEINF_CALL_BYTES ) == NULL ) ) {
        leaf = TGLeaf( back, BGName( back, CG_FE, sym, TypeAddress( back, TY_POINTER ) ) );
#ifdef DEVBUILD
        EchoAPI( "CGFEName( %s, %t ) declspec(dllimport)", sym, tipe );
        hdlAdd( CG_NAMES, leaf );
#endif
        leaf = CGUnary( back, O_POINTS, leaf, TY_POINTER );
    } else {
        leaf = TGLeaf( back, BGName( back, CG_FE, sym, TypeAddress( back, tipe ) ) );
#ifdef DEVBUILD
        EchoAPI( "CGFEName( %s, %t )", sym, tipe );
        hdlAdd( CG_NAMES, leaf );
#endif
    }
#ifdef DEVBUILD
    return EchoAPICgnameReturn( leaf );
#else
    return( leaf );
#endif
}

cg_name _CGAPI CGBackName( BackRun& back, back_handle bck, cg_type tipe )
/********************************************************/
{
#ifdef DEVBUILD
    cg_name     retn;

    EchoAPI( "CGBackName( %B, %t )", bck, tipe );
#endif

    if( !IS_REAL_BACK( bck ) )
        _Zoiks( ZOIKS_068 );

#ifdef DEVBUILD
    retn = TGLeaf( BGName( CG_BACK, bck, TypeAddress( tipe ) ) );
    hdlAdd( CG_NAMES, retn );
    return EchoAPICgnameReturn( retn );
#else
    return( TGLeaf( back, BGName( back, CG_BACK, bck, TypeAddress( back, tipe ) ) ) );
#endif
}

cg_name _CGAPI CGTempName( BackRun& back, temp_handle temp, cg_type tipe )
/*********************************************************/
{
#ifdef DEVBUILD
    cg_name     retn;

    EchoAPI( "CGTempName( %T, %t )", temp, tipe );
    retn = TGLeaf( BGTempName( (name *)temp, TypeAddress( TY_POINTER ) ) );
    hdlAdd( CG_NAMES, retn );
    return EchoAPICgnameReturn( retn );
#else
    /* unused parameters */ (void)tipe;

    return( TGLeaf( back, BGTempName( back, (name *)temp, TypeAddress( back, TY_POINTER ) ) ) );
#endif
}

temp_handle _CGAPI      CGTemp( BackRun& back, cg_type tipe )
/********************************************/
{
#ifdef DEVBUILD
    temp_handle     retn;

    EchoAPI( "CGTemp( %t )", tipe );
    retn = (temp_handle)BGGlobalTemp( TypeAddress( tipe ) );
    return EchoAPITempHandleReturn( retn );
#else
    return( (temp_handle)BGGlobalTemp( back, TypeAddress( back, tipe ) ) );
#endif
}

/**/
/* Assignment operations*/
/**/

cg_name _CGAPI CGAssign( BackRun& back, cg_name dest, cg_name source, cg_type tipe )
/*******************************************************************/
{
#ifdef DEVBUILD
    cg_name     retn;

    EchoAPI( "CGAssign( %n, %n, %t )", dest, source, tipe );
    hdlUseOnce( CG_NAMES, dest );
    hdlUseOnce( CG_NAMES, source );
    retn = TGAssign( dest, source, TypeAddress( tipe ) );
    hdlAddBinary( CG_NAMES, retn, dest, source );
    return EchoAPICgnameReturn( retn );
#else
    return( TGAssign( back, dest, source, TypeAddress( back, tipe ) ) );
#endif
}


cg_name _CGAPI CGLVAssign( BackRun& back, cg_name dest, cg_name source, cg_type tipe )
/*********************************************************************/
{
#ifdef DEVBUILD
    cg_name     retn;

    EchoAPI( "CGLVAssign( %n, %n, %t )", dest, source, tipe );
    hdlUseOnce( CG_NAMES, dest );
    hdlUseOnce( CG_NAMES, source );
    retn = TGLVAssign( dest, source, TypeAddress( tipe ) );
    hdlAddBinary( CG_NAMES, retn, dest, source );
    return EchoAPICgnameReturn( retn );
#else
    return( TGLVAssign( back, dest, source, TypeAddress( back, tipe ) ) );
#endif
}


cg_name _CGAPI CGPostGets( BackRun& back, cg_op opcode, cg_name dest, cg_name src, cg_type tipe )
/********************************************************************************/
{
#ifdef DEVBUILD
    cg_name     retn;

    EchoAPI( "CGPostGets( %o, %n, %n, %t )", opcode, dest, src, tipe );
    hdlUseOnce( CG_NAMES, dest );
    hdlUseOnce( CG_NAMES, src );
    retn = TGPostGets( opcode, dest, src, TypeAddress( tipe ) );
    hdlAddBinary( CG_NAMES, retn, dest, src );
    return EchoAPICgnameReturn( retn );
#else
    return( TGPostGets( back, opcode, dest, src, TypeAddress( back, tipe ) ) );
#endif
}

cg_name _CGAPI CGPreGets( BackRun& back, cg_op opcode, cg_name dest, cg_name src, cg_type tipe )
/*******************************************************************************/
{
#ifdef DEVBUILD
    cg_name     retn;

    EchoAPI( "CGPreGets( %o, %n, %n, %t )", opcode, dest, src, tipe );
    hdlUseOnce( CG_NAMES, dest );
    hdlUseOnce( CG_NAMES, src );
    retn = TGPreGets( opcode, dest, src, TypeAddress( tipe ) );
    hdlAddBinary( CG_NAMES, retn, dest, src );
    return EchoAPICgnameReturn( retn );
#else
    return( TGPreGets( back, opcode, dest, src, TypeAddress( back, tipe ) ) );
#endif
}

cg_name _CGAPI CGLVPreGets( BackRun& back, cg_op opcode, cg_name dest, cg_name src, cg_type tipe )
/*********************************************************************************/
{
#ifdef DEVBUILD
    cg_name     retn;

    EchoAPI( "CGLVPreGets( %o, %n, %n, %t )", opcode, dest, src, tipe );
    hdlUseOnce( CG_NAMES, dest );
    hdlUseOnce( CG_NAMES, src );
    retn = TGLVPreGets( opcode, dest, src, TypeAddress( tipe ) );
    hdlAddBinary( CG_NAMES, retn, dest, src );
    return EchoAPICgnameReturn( retn );
#else
    return( TGLVPreGets( back, opcode, dest, src, TypeAddress( back, tipe ) ) );
#endif
}

/**/
/* Arithmetic/logical operations*/
/**/

cg_name _CGAPI CGBinary( BackRun& back, cg_op opcode, cg_name name1, cg_name name2, cg_type tipe )
/*********************************************************************************/
{
#ifdef DEVBUILD
    cg_name     retn;

    EchoAPI( "CGBinary( %o, %n, %n, %t )", opcode, name1, name2, tipe );
    hdlUseOnce( CG_NAMES, name1 );
    hdlUseOnce( CG_NAMES, name2 );
    if( opcode != O_COMMA ) {
        verifyNotUserType( tipe );
    }
#endif
    if( opcode == O_COMMA ) {
        name1 = TGTrash( name1 );
    } else if( opcode == O_SIDE_EFFECT ) {
        name2 = TGTrash( name2 );
    }
#ifdef DEVBUILD
    retn = TGBinary( opcode, name1, name2, TypeAddress(tipe) );
    hdlAddBinary( CG_NAMES, retn, name1, name2 );
    return EchoAPICgnameReturn( retn );
#else
    return( TGBinary( back, opcode, name1, name2, TypeAddress( back, tipe ) ) );
#endif
}

cg_name _CGAPI CGUnary( BackRun& back, cg_op opcode, cg_name name, cg_type tipe )
/****************************************************************/
{
#ifdef DEVBUILD
    cg_name     retn;

    EchoAPI( "CGUnary( %o, %n, %t )", opcode, name, tipe );
    hdlUseOnce( CG_NAMES, name );
    if( opcode != O_POINTS ) {
        verifyNotUserType( tipe );
    }
    retn = TGUnary( opcode, name, TypeAddress( tipe ) );
    hdlAddUnary( CG_NAMES, retn, name );
    return EchoAPICgnameReturn( retn );
#else
    return( TGUnary( back, opcode, name, TypeAddress( back, tipe ) ) );
#endif
}

cg_name _CGAPI CGIndex( BackRun& back, cg_name name, cg_name by, cg_type tipe, cg_type ptipe )
/*****************************************************************************/
{
#ifdef DEVBUILD
    EchoAPI( "CGIndex( %n, %n, %t, %t )", name, by, tipe, ptipe );
#endif
    return( TGIndex( back, name, by, TypeAddress( back, tipe ), TypeAddress( back, ptipe ) ) );
}

/**/
/* Routine calling*/
/**/

call_handle _CGAPI  CGInitCall( BackRun& back, cg_name name, cg_type tipe, cg_sym_handle sym )
/*****************************************************************************/
{
#ifdef DEVBUILD
    call_handle  call;

    EchoAPI( "CGInitCall( %n, %t, %s )", name, tipe, sym );
    hdlUseOnce( CG_NAMES, name );
    call = TGInitCall( name, TypeAddress( tipe ), sym );
    hdlAddUnary( CG_NAMES, call, name );
    return( EchoAPICallHandleReturn( call ) );
#else
    return( TGInitCall( back, name, TypeAddress( back, tipe ), sym ) );
#endif
}

void _CGAPI     CGAddParm( BackRun& back, call_handle call, cg_name name, cg_type tipe )
/***********************************************************************/
{
#ifdef DEVBUILD
    EchoAPI( "CGAddParm( %C, %n, %t )\n", call, name, tipe );
    hdlExists( CG_NAMES, call );
    hdlUseOnce( CG_NAMES, name );
#endif
    TGAddParm( back, call, name, TypeAddress( back, tipe ) );
}

cg_name _CGAPI CGCall( call_handle call )
/***************************************/
{
#ifdef DEVBUILD
    cg_name     retn;

    EchoAPI( "CGCall( %C )\n", call );
    hdlUseOnce( CG_NAMES, call );
    retn = TGCall( call );
    hdlAddUnary( CG_NAMES, retn, call );
    return( retn );
#else
    return( TGCall( call ) );
#endif
}

/**/
/* Comparison/short-circuit operations*/
/**/

cg_name _CGAPI CGCompare( BackRun& back, cg_op opcode, cg_name name1, cg_name name2, cg_type tipe )
/**********************************************************************************/
{
#ifdef DEVBUILD
    cg_name     retn;

    EchoAPI( "CGCompare( %o, %n, %n, %t )", opcode, name1, name2, tipe );
    hdlUseOnce( CG_NAMES, name1 );
    hdlUseOnce( CG_NAMES, name2 );
    retn = TGCompare( opcode, name1, name2, TypeAddress( tipe ) );
    hdlAddBinary( CG_NAMES, retn, name1, name2 );
    return EchoAPICgnameReturn( retn );
#else
    return( TGCompare( back, opcode, name1, name2, TypeAddress( back, tipe ) ) );
#endif
}

cg_name _CGAPI CGFlow( BackRun& back, cg_op opcode, cg_name name1, cg_name name2 )
/*****************************************************************/
{
#ifdef DEVBUILD
    cg_name     retn;

    EchoAPI( "CGFlow( %o, %n, %n )", opcode, name1, name2 );
    hdlUseOnce( CG_NAMES, name1 );
    if( NULL != name2 ) {
        hdlUseOnce( CG_NAMES, name2 );
    }
    retn = TGFlow( opcode, name1, name2 );
    hdlAddBinary( CG_NAMES, retn, name1, name2 );
    return EchoAPICgnameReturn( retn );
#else
    return( TGFlow( back, opcode, name1, name2 ) );
#endif
}

/**/
/* Control flow operations*/
/**/

cg_name _CGAPI CGChoose( BackRun& back, cg_name sel, cg_name n1, cg_name n2, cg_type tipe )
/**************************************************************************/
{
#ifdef DEVBUILD
    cg_name     retn;

    EchoAPI( "CGChoose( %n, %n, %n, %t )", sel, n1, n2, tipe );
    hdlUseOnce( CG_NAMES, sel );
    hdlUseOnce( CG_NAMES, n1 );
    hdlUseOnce( CG_NAMES, n2 );
    retn = TGQuestion( sel, n1, n2, TypeAddress( tipe ) );
    hdlAddTernary( CG_NAMES, retn, sel, n1, n2 );
    return EchoAPICgnameReturn( retn );
#else
    return( TGQuestion( back, sel, n1, n2, TypeAddress( back, tipe ) ) );
#endif
}

cg_name _CGAPI CGWarp( BackRun& back, cg_name before, label_handle label, cg_name after )
/************************************************************************/
{
#ifdef DEVBUILD
    cg_name     retn;

    EchoAPI( "CGWarp( %n, %L, %n )", before, label, after );
    if( before != NULL ) { // Fortran calls with NULL first parm
        hdlUseOnce( CG_NAMES, before );
    }
    hdlUseOnce( CG_NAMES, after );
    retn = TGWarp( before, label, after );
    hdlAddUnary( CG_NAMES, retn, before );
    return EchoAPICgnameReturn( retn );
#else
    return( TGWarp( back, before, label, after ) );
#endif
}


cg_name _CGAPI  CGCallback( BackRun& back, cg_callback func, callback_handle parm )
/******************************************************************/
{
#ifdef DEVBUILD
    cg_name     retn;

    EchoAPI( "CGCallback( %x, %x )", func, parm );
    retn = TGCallback( func, parm );
    hdlAdd( CG_NAMES, retn );
    EchoAPI( " -> %n\n", retn );
    return retn;
#else
    return( TGCallback( back, func, parm ) );
#endif
}


void _CGAPI     CG3WayControl( BackRun& back, cg_name expr, label_handle lt,
                               label_handle eq, label_handle gt )
/***************************************************************/
{
#ifdef DEVBUILD
    EchoAPI( "CG3WayControl( %n, %L, %L, %L )\n", expr, lt, eq, gt );
#endif
    TG3WayControl( back, expr, lt, eq, gt );  /* special TGen()*/
}

void _CGAPI     CGControl( BackRun& back, cg_op opcode, cg_name expr, label_handle lbl )
/***********************************************************************/
{
#ifdef DEVBUILD
    EchoAPI( "CGControl( %o, %n, %L )\n", opcode, expr, lbl );
    if( NULL != expr ) {
        hdlUseOnce( CG_NAMES, expr );
        hdlAllUsed( CG_NAMES );
    }
    if( lbl != 0 ) {
        hdlExists( LABEL_HANDLE, lbl );
    }
#endif
    TGControl( back, opcode, expr, lbl );  /* special TGen()*/
}

void _CGAPI     CGBigLabel( BackRun& back, back_handle bck )
/*******************************************/
{
#ifdef DEVBUILD
    EchoAPI( "CGBigLabel( %B )\n", bck );
#endif
    BGBigLabel( back, bck );
}

void _CGAPI     CGBigGoto( BackRun& back, label_handle lbl, int level )
/******************************************************/
{
#ifdef DEVBUILD
    EchoAPI( "CGBigGoto( %L, %i )\n", lbl, level );
#endif
    BGBigGoto( back, lbl, level );
}

sel_handle _CGAPI       CGSelInit( BackRun& back )
/***************************************/
{
#ifdef DEVBUILD
    sel_handle  retn;

    EchoAPI( "CGSelInit()" );
    retn = BGSelInit();
    hdlAdd( SEL_HANDLE, retn );
    return EchoAPISelHandleReturn( retn );
#else
    return( BGSelInit(back) );
#endif
}

void _CGAPI     CGSelCase( BackRun& back, sel_handle s, label_handle lbl, signed_64 val )
/************************************************************************/
{
#ifdef DEVBUILD
    EchoAPI( "CGSelCase( %S, %L, %l )\n", s, lbl, val.u._64[0] );
    hdlExists( SEL_HANDLE, s );
    hdlExists( LABEL_HANDLE, lbl );
#endif
    BGSelCase( back, s, lbl, &val );
}

void _CGAPI     CGSelRange( BackRun& back, sel_handle s, signed_64 lo,
                        signed_64 hi, label_handle lbl )
/******************************************************/
{
#ifdef DEVBUILD
    EchoAPI( "CGSelRange( %S, %L, %l, %l )\n", s, lbl, lo.u._64[0], hi.u._64[0] );
    hdlExists( SEL_HANDLE, s );
    hdlExists( LABEL_HANDLE, lbl );
#endif
    BGSelRange( back, s, &lo, &hi, lbl );
}

void _CGAPI     CGSelOther( sel_handle s, label_handle lbl )
/**********************************************************/
{
#ifdef DEVBUILD
    EchoAPI( "CGSelOther( %S, %L )\n", s, lbl );
    hdlExists( SEL_HANDLE, s );
    hdlExists( LABEL_HANDLE, lbl );
#endif
    BGSelOther( s, lbl );
}

void _CGAPI     CGSelectRestricted( BackRun& back, sel_handle s, cg_name expr, cg_switch_type allowed )
/**************************************************************************************/
{
    an  selv;

    selv = TGen( back, expr, TypeAddress( back, TY_DEFAULT ) );
    BGSelect( back, s, selv, allowed );
}

void _CGAPI     CGSelect( BackRun& back, sel_handle s, cg_name expr )
/****************************************************/
{
#ifdef DEVBUILD
    EchoAPI( "CGSelect( %S, %n )\n", s, expr );
    hdlExists( SEL_HANDLE, s );
    hdlUseOnce( CG_NAMES, expr );
    hdlUseOnce( SEL_HANDLE, s );
#endif

    CGSelectRestricted( back, s, expr, CG_SWITCH_ALL );
}

/**/
/* Misc. operations*/
/**/

cg_name _CGAPI CGEval( BackRun& back, cg_name name )
/***********************************/
{
#ifdef DEVBUILD
    cg_name     retn;

    EchoAPI( "CGEval( %n )", name );
    hdlUseOnce( CG_NAMES, name );
    retn = TGTmpLeaf( TGen( name, TypeAddress( TY_DEFAULT ) ) );
    hdlAddUnary( CG_NAMES, retn, name );
    return EchoAPICgnameReturn( retn );
#else
    return( TGTmpLeaf( back, TGen( back, name, TypeAddress( back, TY_DEFAULT ) ) ) );
#endif
}

void _CGAPI     CGTrash( BackRun& back, cg_name name )
/*************************************/
{
#ifdef DEVBUILD
    EchoAPI( "CGTrash( %n )\n", name );
    hdlUseOnce( CG_NAMES, name );
    hdlAllUsed( CG_NAMES );
#endif
    BGTrash( back, TGen( back, TGTrash( name ), TypeAddress( back, TY_DEFAULT ) ) );
}

void _CGAPI     CGDone( BackRun& back, cg_name name )
/************************************/
{
#ifdef DEVBUILD
    EchoAPI( "CGDone( %n )\n", name );
    hdlUseOnce( CG_NAMES, name );
    hdlAllUsed( CG_NAMES );
#endif
    BGTrash( back, TGen( back, TGTrash( name ), TypeAddress( back, TY_DEFAULT ) ) );
    BGStartBlock(back);
}

cg_type _CGAPI CGType( cg_name name )
/***********************************/
{
#ifdef DEVBUILD
    cg_type     retn;

    EchoAPI( "CGType( %n )", name );
    retn = TGType( name );
    return EchoAPICgtypeReturn( retn );
#else
    return( TGType( name ) );
#endif
}

cg_name _CGAPI CGBitMask( BackRun& back, cg_name left, byte start, byte len, cg_type tipe )
/**************************************************************************/
{
#ifdef DEVBUILD
    cg_name     retn;

    EchoAPI( "CGBitMask( %n, %x, %x, %t )", left, start, len, tipe );
    hdlUseOnce( CG_NAMES, left );
    retn = TGBitMask( left, start, len, TypeAddress( tipe ) );
    hdlAddUnary( CG_NAMES, retn, left );
    return EchoAPICgnameReturn( retn );
#else
    return( TGBitMask( back, left, start, len, TypeAddress( back, tipe ) ) );
#endif
}

cg_name _CGAPI CGVolatile( cg_name name )
/***************************************/
{
#ifdef DEVBUILD
    cg_name     retn;

    EchoAPI( "CGVolatile( %n )", name );
    hdlUseOnce( CG_NAMES, name );
    retn = TGVolatile( name );
    hdlAddUnary( CG_NAMES, retn, name );
    return EchoAPICgnameReturn( retn );
#else
    return( TGVolatile( name ) );
#endif
}

cg_name _CGAPI CGAttr( cg_name name, cg_sym_attr attr )
/*****************************************************/
{
#ifdef DEVBUILD
    cg_name     retn;

    EchoAPI( "CGAttr( %n, %i )", name, attr );
    hdlUseOnce( CG_NAMES, name );
    retn = TGAttr( name, attr );
    hdlAddUnary( CG_NAMES, retn, name );
    return EchoAPICgnameReturn( retn );
#else
    return( TGAttr( name, attr ) );
#endif
}

#if 0
cg_name _CGAPI CGAlign( cg_name name, uint alignment )
/****************************************************/
{
#ifdef DEVBUILD
    cg_name     retn;

    EchoAPI( "CGAlign( %n, %i )", name, alignment );
    hdlUseOnce( CG_NAMES, name );
    retn = TGAlign( name, alignment );
    hdlAddUnary( CG_NAMES, retn, name );
    return EchoAPICgnameReturn( retn );
#else
    return( TGAlign( name, alignment ) );
#endif
}
#endif


cg_name * _CGAPI CGDuplicate( BackRun& back, cg_name name )
/******************************************/
{
    an          addr;
#ifdef DEVBUILD
    cg_name     *retn;

    EchoAPI( "CGDuplicate( %n )", name );
    hdlExists( CG_NAMES, name );
#endif

    addr = TGen( back, name, TypeAddress( back, TY_DEFAULT ) );
    back.intrface->CGDuplicateArray[0] = TGReLeaf( back, BGCopy( back, addr ) );
    back.intrface->CGDuplicateArray[1] = TGReLeaf( back, addr );

#ifdef DEVBUILD
    retn = CGDuplicateArray;
    EchoAPI( " -> %n %n\n", retn[0], retn[1] );
    hdlAdd( CG_NAMES, retn[0] );
    hdlAdd( CG_NAMES, retn[1] );
    return retn;
#else
    return( back.intrface->CGDuplicateArray );
#endif
}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%                                              %%*/
/*%   Data generator actions                     %%*/
/*%                                              %%*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

void _CGAPI     DGLabel( BackRun& back, back_handle bck )
/****************************************/
{
#ifdef DEVBUILD
    EchoAPI( "DGLabel( %B )\n", bck );
#endif
    if( !IS_REAL_BACK( bck ) )
        _Zoiks( ZOIKS_068 );
    DGBlip();
    DataLabel( back, bck->lbl );
    bck->segid = AskOP(back);
}

void _CGAPI     DGBackPtr( BackRun& back, back_handle bck, segment_id segid,
                            int_32 offset, cg_type tipe )
/***********************************************************/
{
#ifdef DEVBUILD
    EchoAPI( "DGBackPtr( %B, %S, %i, %t )\n", bck, segid, offset, tipe );
#endif
    if( !IS_REAL_BACK( bck ) )
        _Zoiks( ZOIKS_068 );
    DGBlip();
    BackPtr( back, bck, segid, offset, TypeAddress( back, tipe ) );
}

void _CGAPI DGFEPtr( BackRun& back, cg_sym_handle sym, cg_type tipe, int_32 offset )
/*******************************************************************/
{
#ifdef DEVBUILD
    EchoAPI( "DGFEPtr( %S, %t, %i )\n", sym, tipe, offset );
#endif
    DGBlip();
    FEPtr( back, sym, TypeAddress( back, tipe ), offset );
}

void _CGAPI DGFEPtrBase( BackRun& back, cg_sym_handle sym )
/******************************************/
/* CodeBreaker: the selector of the symbol's segment, a word the loader fills in */
{
#ifdef DEVBUILD
    EchoAPI( "DGFEPtrBase( %S )\n", sym );
#endif
    DGBlip();
    FEPtrBase( back, sym );
}

void _CGAPI     DGBytes( BackRun& back, uint_32 len, const void *src )
/*****************************************************/
{
#ifdef DEVBUILD
    EchoAPI( "DGBytes( %x, %x )\n", len, src );
#endif
    DGBlip();
    DataBytes( back, len, src );
}

void _CGAPI     DGInteger( BackRun& back, uint_32 value, cg_type tipe )
/******************************************************/
{
    type_length len;
    int         i;
    byte        buff[6];
    byte        *form;

#ifdef DEVBUILD
    EchoAPI( "DGInteger( %i, %t )\n", value, tipe );
#endif
    len = TypeLength( back, tipe );
    i = 0;
#if _TARG_MEMORY & _TARG_LOW_FIRST
    form = buff;
    while( i < len ) {
        *form++ = value & 0xff;
        value /= 256;
        ++i;
    }
    form = buff;
#else
    form = buff + 6;
    while( i < len ) {
        *--form = value & 0xff;
        value /= 256;
        ++i;
    }
#endif
    DGBytes( back, len, form );
}

void _CGAPI     DGInteger64( BackRun& back, unsigned_64 value, cg_type tipe )
/************************************************************/
{
    type_length len;
    struct {
        union {
            uint_32     vall;
            unsigned_64 val;
            byte        buff[8];
        } u;
    } data;
    byte        *form;

#if ( ( _TARG_MEMORY & _TARG_LOW_FIRST ) == 0 ) == defined( __BIG_ENDIAN__ )
    data.u.val = value;
#else
    {  // reverse them
        struct {
            union {
                unsigned_64 val;
                byte        buff[8];
            } u;
        } temp;
        int  i;

        temp.u.val = value;
        for( i = 0; i <= 7; ++i ) {
            data.u.buff[i] = temp.u.buff[7 - i];
        }
    }
#endif
#ifdef DEVBUILD
// fix this up when we get printf support for int64
    EchoAPI( "DGInteger64( %x %x, %t )\n"
           , data.u.val.u._32[0]
           , data.u.val.u._32[1]
           , tipe );
#endif
    form = data.u.buff;
    len = TypeLength( back, tipe );
    DGBytes( back, len, form );
}

void _CGAPI     DGFloat( BackRun& back, cchar_ptr value, cg_type tipe )
/******************************************************/
{
    float_handle    cf;
    flt             buff;

#ifdef DEVBUILD
    EchoAPI( "DGFloat( %c, %t )\n", value, tipe );
#endif
    cf = CFCnvSF( back, &back.cfloat.cgh, value );
    CFCnvTarget( cf, &buff, TypeLength( back, tipe ) );
    CFFree( back, &back.cfloat.cgh, cf );
    DGBytes( back, TypeLength( back, tipe ), &buff );
}

void _CGAPI     DGIBytes( BackRun& back, uint_32 len, byte pat )
/***********************************************/
{
#ifdef DEVBUILD
    EchoAPI( "DGIBytes( %x, %x )\n", len, pat );
#endif
    DGBlip();
    IterBytes( back, len, pat );
}

void _CGAPI     DGChar( BackRun& back, char value )
/**********************************/
{
#ifdef DEVBUILD
    EchoAPI( "DGChar( %x )\n", value );
#endif
    DGIBytes( back, 1, value );
}

void _CGAPI DGString( BackRun& back, cchar_ptr value, uint len )
/***********************************************/
{
#ifdef DEVBUILD
    char        data[40];
    unsigned    slen = len;
    char        *d = data;
    char        *dt = &data[sizeof( data ) - 1];
    const char  *s = value;
    char        *hex = "0123456789abcdef";
    char        c;

    for( ;; ) {
        assert( d <= dt );
        if( slen == 0 )
            break;
        --slen;
        c = *s++;
        if( !iscntrl(c) && isascii(c)) {
            if(( d + (1+1)) >= dt )
                break;
            *d++ = c;
        } else {
            if(( d + (4+1) ) >= dt )
                break;
            *d++ = '\\';
            *d++ = 'x';
            *d++ = hex[( c >> 4 ) & 0x0f];
            *d++ = hex[( c >> 0 ) & 0x0f];
        }
    }
    *d = '\0';
    assert( strlen( data ) < sizeof( data ) );
    if( slen != 0 ) {
        EchoAPI( "DGString( \"%c...\", %x )\n", data, len );
    } else {
        EchoAPI( "DGString( \"%c\", %x )\n", data, len );
    }
#endif

#if 1
    DGBytes( back, len, value );
#else
    // this code is only useful if we do ASCII<->EBCDIC translation
    // but since we don't do conversions; it's only slow
    while( len > 0 ) {
        DGChar( *value++ );
        --len;
    }
#endif
}

void _CGAPI     DGUBytes( BackRun& back, uint_32 len )
/*************************************/
{
#ifdef DEVBUILD
    EchoAPI( "DGUBytes( %x )\n", len );
#endif
    DGBlip();
    IncLocation( back, len );
}

void _CGAPI     DGAlign( BackRun& back, uint align )
/***********************************/
{
#ifdef DEVBUILD
    EchoAPI( "DGAlign( %i )\n", align );
#endif
    DGBlip();
    DataAlign( back, align );
}


uint_32 _CGAPI  DGSeek( BackRun& back, uint_32 where )
/*************************************/
{
    uint        old;

#ifdef DEVBUILD
    EchoAPI( "DGSeek( %x )", where );
#endif

    DGBlip();
    old = AskLocation(back);
    SetLocation( back, where );

#ifdef DEVBUILD
    return( EchoAPIHexReturn( old ) );
#else
    return( old );
#endif
}

uint_32 _CGAPI      DGTell( BackRun& back )
/********************************/
{
#ifdef DEVBUILD
    uint_32     retn;

    EchoAPI( "DGTell()" );
    retn = AskLocation();
    return( EchoAPIIntReturn( retn ) );
#else
    return( AskLocation(back) );
#endif
}


uint_32 _CGAPI      DGBackTell( BackRun& back, back_handle bck )
/***********************************************/
{
#ifdef DEVBUILD
    uint_32 retn;
    EchoAPI( "DGBackTell( %B )", bck );
#endif

    if( !IS_REAL_BACK( bck ) )
        _Zoiks( ZOIKS_068 );

#ifdef DEVBUILD
    retn = AskAddress( bck->lbl );
    return EchoAPIIntReturn( retn );
#else
    return( AskAddress( back, bck->lbl ) );
#endif
}


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%                                              %%*/
/*%   Internal routines                          %%*/
/*%                                              %%*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

void _CGAPI     DGCFloat( BackRun& back, pointer cf, cg_type tipe )
/**************************************************/
{
    flt        buff;

#ifdef DEVBUILD
    EchoAPI( "DGCFloat( %x, %t )\n", cf, tipe );
#endif
    CFCnvTarget( static_cast<cfloat *>(cf), &buff, TypeLength( back, tipe ) );
    DGBytes( back, TypeLength( back, tipe ), &buff );
}

const char * cg_internal AskName( BackRun& back, pointer hdl, cg_class class_ )
/*************************************************************/
{
    switch( class_ ) {
    case CG_FE:
        return( FEName( *back.front, static_cast<cg_sym_handle>(hdl) ) );
    case CG_LBL:
        return( "(LBL)" );
    case CG_CLB:
        return( "(CLB)" );
    case CG_BACK:
        return( "(BCK)" );
    case CG_TBL:
        return( "(TBL)" );
    case CG_VTB:
        return( "(VTB)" );
    default:
        _Zoiks( ZOIKS_130 );
        return( "" );
    }
}

label_handle cg_internal AskForSymLabel( BackRun& back, pointer hdl, cg_class class_ )
/********************************************************************/
{
    switch( class_ ) {
    case CG_FE:
        return( FEBack( *back.front, static_cast<cg_sym_handle>(hdl) )->lbl );
    case CG_LBL:
        return( (label_handle)hdl );
    case CG_CLB:
        return( (label_handle)hdl );
    case CG_BACK:
        return( ((back_handle)hdl)->lbl );
    case CG_TBL:
        return( ((tbl_control*)hdl)->lbl );
    case CG_VTB:
        return( ((tbl_control*)hdl)->value_lbl );
    default:
        _Zoiks( ZOIKS_130 );
        return( NULL );
    }
}

import_handle   cg_internal AskImportHandle( BackRun& back, cg_sym_handle sym )
/**************************************************************/
{
    return( FEBack( *back.front, sym )->imp );
}

void    cg_internal TellImportHandle( BackRun& back, cg_sym_handle sym, import_handle imphdl )
/*****************************************************************************/
{
    FEBack( *back.front, sym )->imp = imphdl;
}

/* the run makes and frees its copy of this file's variables (run/ files: see the run header) */
IntrfaceState* intrface_make() { return new IntrfaceState(); }
void intrface_free(IntrfaceState* part) { delete part; }
