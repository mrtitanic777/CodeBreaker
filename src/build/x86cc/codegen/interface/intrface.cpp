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
#include "feprotos.h"
#include "cgprotos.h"
#include "_cfloat.h"

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
    InitCG(back);
}

void _CGAPI     BEStop( BackRun& back )
/****************************/
{
    FiniCG(back);
}

void _CGAPI     BEAbort( BackRun& back )
/*****************************/
{
    AbortCG(back);
}

pointer _CGAPI  BEMemAlloc( BackRun& back, unsigned size )
/*****************************************/
{
    return( CGAlloc( back, size ) );
}

void _CGAPI     BEMemFree( BackRun& back, pointer ptr )
/**************************************/
{
    CGFree( back, ptr );
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
    return( AskOP(back) );
}

segment_id _CGAPI   BESetSeg( BackRun& back, segment_id segid )
/**********************************************/
{
    return( ChangeOP( back, segid ) );
}

void _CGAPI BEDefSeg( BackRun& back, segment_id segid, seg_attr attr, cchar_ptr str, uint algn )
/*******************************************************************************/
{
    DefSegment( back, segid, attr, str, algn, false );
}

void _CGAPI     BEFlushSeg( BackRun& back, segment_id segid )
/********************************************/
{
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
    return( AskForNewLabel(back) );
}

void _CGAPI     BEFiniLabel( BackRun& back, label_handle lbl )
/*********************************************/
{

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
    return( BGUnrollCount( back, unroll_count ) );
}


/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%                                              %%*/
/*%   Tree patch creation/use/deletion           %%*/
/*%                                              %%*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

patch_handle _CGAPI     BEPatch( BackRun& back )
/*************************************/
{
    return( BGNewPatch(back) );
}

cg_name _CGAPI          CGPatchNode( BackRun& back, patch_handle patch, cg_type tipe )
/*********************************************************************/
{
    return( BGPatchNode( back, patch, TypeAddress( back, tipe ) ) );
}

void _CGAPI     BEPatchInteger( BackRun& back, patch_handle patch, int_32 value )
/****************************************************************/
{
    BGPatchInteger( back, patch, value );
}

void _CGAPI     BEFiniPatch( BackRun& back, patch_handle patch )
/***********************************************/
{

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

    if( back.intrface->NewBackReturn == FAKE_NULL ) {
        return( NULL );
    }
    if( back.intrface->NewBackReturn != NULL ) {
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
    return( bck );
}

void _CGAPI     BEFiniBack( BackRun& back, back_handle bck )
/*******************************************/
{
    if( IS_REAL_BACK( bck ) ) {
        TellNoSymbol( back, bck->lbl );
    }
}

void _CGAPI     BEFreeBack( BackRun& back, back_handle bck )
/*******************************************/
{
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
    TypeDef( back, what, len, align );
}

void _CGAPI     BEAliasType( BackRun& back, cg_type what, cg_type to )
/*****************************************************/
{
    TypeAlias( back, what, to );
}

uint_32 _CGAPI      BETypeLength( BackRun& back, cg_type tipe )
/**********************************************/
{
    return( TypeLength( back, tipe ) );
}

uint _CGAPI     BETypeAlign( cg_type tipe )
/*****************************************/
{
    /* unused parameters */ (void)tipe;

    return( 1 );
}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%                                              %%*/
/*%   Procedure/parms/autos declaration          %%*/
/*%                                              %%*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

void _CGAPI     CGProcDecl( BackRun& back, cg_sym_handle sym, cg_type tipe )
/***********************************************************/
{
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
    BGAutoDecl( back, sym, TypeAddress( back, tipe ) );
}

void _CGAPI     CGReturn( BackRun& back, cg_name name, cg_type tipe )
/****************************************************/
{
    const type_def  *new_tipe;
    an              retv;

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
    return( TGLeaf( back, BGVarargsBasePtr( back, TypeAddress( back, tipe ) ) ) );
}

cg_name _CGAPI CGInteger( BackRun& back, int_32 val, cg_type tipe )
/**************************************************/
{
    return( TGLeaf( back, BGInteger( back, val, TypeAddress( back, tipe ) ) ) );
}

cg_name _CGAPI CGInt64( BackRun& back, signed_64 val, cg_type tipe )
/***************************************************/
{
    return( TGLeaf( back, BGInt64( back, val, TypeAddress( back, tipe ) ) ) );
}

cg_name _CGAPI CGFloat( BackRun& back, cchar_ptr num, cg_type tipe )
/***************************************************/
{
    return( TGLeaf( back, BGFloat( back, num, TypeAddress( back, tipe ) ) ) );
}


cg_name _CGAPI CGFEName( BackRun& back, cg_sym_handle sym, cg_type tipe )
/********************************************************/
{
    cg_name     leaf;


    if( (FEAttr( *back.front, sym ) & FE_DLLIMPORT) && ( FindAuxInfoSym( sym, FEINF_CALL_BYTES ) == NULL ) ) {
        leaf = TGLeaf( back, BGName( back, CG_FE, sym, TypeAddress( back, TY_POINTER ) ) );
        leaf = CGUnary( back, O_POINTS, leaf, TY_POINTER );
    } else {
        leaf = TGLeaf( back, BGName( back, CG_FE, sym, TypeAddress( back, tipe ) ) );
    }
    return( leaf );
}

cg_name _CGAPI CGBackName( BackRun& back, back_handle bck, cg_type tipe )
/********************************************************/
{

    if( !IS_REAL_BACK( bck ) )
        _Zoiks( ZOIKS_068 );

    return( TGLeaf( back, BGName( back, CG_BACK, bck, TypeAddress( back, tipe ) ) ) );
}

cg_name _CGAPI CGTempName( BackRun& back, temp_handle temp, cg_type tipe )
/*********************************************************/
{
    /* unused parameters */ (void)tipe;

    return( TGLeaf( back, BGTempName( back, (name *)temp, TypeAddress( back, TY_POINTER ) ) ) );
}

temp_handle _CGAPI      CGTemp( BackRun& back, cg_type tipe )
/********************************************/
{
    return( (temp_handle)BGGlobalTemp( back, TypeAddress( back, tipe ) ) );
}

/**/
/* Assignment operations*/
/**/

cg_name _CGAPI CGAssign( BackRun& back, cg_name dest, cg_name source, cg_type tipe )
/*******************************************************************/
{
    return( TGAssign( back, dest, source, TypeAddress( back, tipe ) ) );
}


cg_name _CGAPI CGLVAssign( BackRun& back, cg_name dest, cg_name source, cg_type tipe )
/*********************************************************************/
{
    return( TGLVAssign( back, dest, source, TypeAddress( back, tipe ) ) );
}


cg_name _CGAPI CGPostGets( BackRun& back, cg_op opcode, cg_name dest, cg_name src, cg_type tipe )
/********************************************************************************/
{
    return( TGPostGets( back, opcode, dest, src, TypeAddress( back, tipe ) ) );
}

cg_name _CGAPI CGPreGets( BackRun& back, cg_op opcode, cg_name dest, cg_name src, cg_type tipe )
/*******************************************************************************/
{
    return( TGPreGets( back, opcode, dest, src, TypeAddress( back, tipe ) ) );
}

cg_name _CGAPI CGLVPreGets( BackRun& back, cg_op opcode, cg_name dest, cg_name src, cg_type tipe )
/*********************************************************************************/
{
    return( TGLVPreGets( back, opcode, dest, src, TypeAddress( back, tipe ) ) );
}

/**/
/* Arithmetic/logical operations*/
/**/

cg_name _CGAPI CGBinary( BackRun& back, cg_op opcode, cg_name name1, cg_name name2, cg_type tipe )
/*********************************************************************************/
{
    if( opcode == O_COMMA ) {
        name1 = TGTrash( name1 );
    } else if( opcode == O_SIDE_EFFECT ) {
        name2 = TGTrash( name2 );
    }
    return( TGBinary( back, opcode, name1, name2, TypeAddress( back, tipe ) ) );
}

cg_name _CGAPI CGUnary( BackRun& back, cg_op opcode, cg_name name, cg_type tipe )
/****************************************************************/
{
    return( TGUnary( back, opcode, name, TypeAddress( back, tipe ) ) );
}

cg_name _CGAPI CGIndex( BackRun& back, cg_name name, cg_name by, cg_type tipe, cg_type ptipe )
/*****************************************************************************/
{
    return( TGIndex( back, name, by, TypeAddress( back, tipe ), TypeAddress( back, ptipe ) ) );
}

/**/
/* Routine calling*/
/**/

call_handle _CGAPI  CGInitCall( BackRun& back, cg_name name, cg_type tipe, cg_sym_handle sym )
/*****************************************************************************/
{
    return( TGInitCall( back, name, TypeAddress( back, tipe ), sym ) );
}

void _CGAPI     CGAddParm( BackRun& back, call_handle call, cg_name name, cg_type tipe )
/***********************************************************************/
{
    TGAddParm( back, call, name, TypeAddress( back, tipe ) );
}

cg_name _CGAPI CGCall( call_handle call )
/***************************************/
{
    return( TGCall( call ) );
}

/**/
/* Comparison/short-circuit operations*/
/**/

cg_name _CGAPI CGCompare( BackRun& back, cg_op opcode, cg_name name1, cg_name name2, cg_type tipe )
/**********************************************************************************/
{
    return( TGCompare( back, opcode, name1, name2, TypeAddress( back, tipe ) ) );
}

cg_name _CGAPI CGFlow( BackRun& back, cg_op opcode, cg_name name1, cg_name name2 )
/*****************************************************************/
{
    return( TGFlow( back, opcode, name1, name2 ) );
}

/**/
/* Control flow operations*/
/**/

cg_name _CGAPI CGChoose( BackRun& back, cg_name sel, cg_name n1, cg_name n2, cg_type tipe )
/**************************************************************************/
{
    return( TGQuestion( back, sel, n1, n2, TypeAddress( back, tipe ) ) );
}

cg_name _CGAPI CGWarp( BackRun& back, cg_name before, label_handle label, cg_name after )
/************************************************************************/
{
    return( TGWarp( back, before, label, after ) );
}


cg_name _CGAPI  CGCallback( BackRun& back, cg_callback func, callback_handle parm )
/******************************************************************/
{
    return( TGCallback( back, func, parm ) );
}


void _CGAPI     CG3WayControl( BackRun& back, cg_name expr, label_handle lt,
                               label_handle eq, label_handle gt )
/***************************************************************/
{
    TG3WayControl( back, expr, lt, eq, gt );  /* special TGen()*/
}

void _CGAPI     CGControl( BackRun& back, cg_op opcode, cg_name expr, label_handle lbl )
/***********************************************************************/
{
    TGControl( back, opcode, expr, lbl );  /* special TGen()*/
}

void _CGAPI     CGBigLabel( BackRun& back, back_handle bck )
/*******************************************/
{
    BGBigLabel( back, bck );
}

void _CGAPI     CGBigGoto( BackRun& back, label_handle lbl, int level )
/******************************************************/
{
    BGBigGoto( back, lbl, level );
}

sel_handle _CGAPI       CGSelInit( BackRun& back )
/***************************************/
{
    return( BGSelInit(back) );
}

void _CGAPI     CGSelCase( BackRun& back, sel_handle s, label_handle lbl, signed_64 val )
/************************************************************************/
{
    BGSelCase( back, s, lbl, &val );
}

void _CGAPI     CGSelRange( BackRun& back, sel_handle s, signed_64 lo,
                        signed_64 hi, label_handle lbl )
/******************************************************/
{
    BGSelRange( back, s, &lo, &hi, lbl );
}

void _CGAPI     CGSelOther( sel_handle s, label_handle lbl )
/**********************************************************/
{
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

    CGSelectRestricted( back, s, expr, CG_SWITCH_ALL );
}

/**/
/* Misc. operations*/
/**/

cg_name _CGAPI CGEval( BackRun& back, cg_name name )
/***********************************/
{
    return( TGTmpLeaf( back, TGen( back, name, TypeAddress( back, TY_DEFAULT ) ) ) );
}

void _CGAPI     CGTrash( BackRun& back, cg_name name )
/*************************************/
{
    BGTrash( back, TGen( back, TGTrash( name ), TypeAddress( back, TY_DEFAULT ) ) );
}

void _CGAPI     CGDone( BackRun& back, cg_name name )
/************************************/
{
    BGTrash( back, TGen( back, TGTrash( name ), TypeAddress( back, TY_DEFAULT ) ) );
    BGStartBlock(back);
}

cg_type _CGAPI CGType( cg_name name )
/***********************************/
{
    return( TGType( name ) );
}

cg_name _CGAPI CGBitMask( BackRun& back, cg_name left, byte start, byte len, cg_type tipe )
/**************************************************************************/
{
    return( TGBitMask( back, left, start, len, TypeAddress( back, tipe ) ) );
}

cg_name _CGAPI CGVolatile( cg_name name )
/***************************************/
{
    return( TGVolatile( name ) );
}

cg_name _CGAPI CGAttr( cg_name name, cg_sym_attr attr )
/*****************************************************/
{
    return( TGAttr( name, attr ) );
}



cg_name * _CGAPI CGDuplicate( BackRun& back, cg_name name )
/******************************************/
{
    an          addr;

    addr = TGen( back, name, TypeAddress( back, TY_DEFAULT ) );
    back.intrface->CGDuplicateArray[0] = TGReLeaf( back, BGCopy( back, addr ) );
    back.intrface->CGDuplicateArray[1] = TGReLeaf( back, addr );

    return( back.intrface->CGDuplicateArray );
}

/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/
/*%                                              %%*/
/*%   Data generator actions                     %%*/
/*%                                              %%*/
/*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

void _CGAPI     DGLabel( BackRun& back, back_handle bck )
/****************************************/
{
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
    if( !IS_REAL_BACK( bck ) )
        _Zoiks( ZOIKS_068 );
    DGBlip();
    BackPtr( back, bck, segid, offset, TypeAddress( back, tipe ) );
}

void _CGAPI DGFEPtr( BackRun& back, cg_sym_handle sym, cg_type tipe, int_32 offset )
/*******************************************************************/
{
    DGBlip();
    FEPtr( back, sym, TypeAddress( back, tipe ), offset );
}

void _CGAPI DGFEPtrBase( BackRun& back, cg_sym_handle sym )
/******************************************/
/* CodeBreaker: the selector of the symbol's segment, a word the loader fills in */
{
    DGBlip();
    FEPtrBase( back, sym );
}

void _CGAPI     DGBytes( BackRun& back, uint_32 len, const void *src )
/*****************************************************/
{
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

    len = TypeLength( back, tipe );
    i = 0;
    form = buff;
    while( i < len ) {
        *form++ = value & 0xff;
        value /= 256;
        ++i;
    }
    form = buff;
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

    data.u.val = value;
    form = data.u.buff;
    len = TypeLength( back, tipe );
    DGBytes( back, len, form );
}

void _CGAPI     DGFloat( BackRun& back, cchar_ptr value, cg_type tipe )
/******************************************************/
{
    float_handle    cf;
    flt             buff;

    cf = CFCnvSF( back, &back.cfloat.cgh, value );
    CFCnvTarget( cf, &buff, TypeLength( back, tipe ) );
    CFFree( back, &back.cfloat.cgh, cf );
    DGBytes( back, TypeLength( back, tipe ), &buff );
}

void _CGAPI     DGIBytes( BackRun& back, uint_32 len, byte pat )
/***********************************************/
{
    DGBlip();
    IterBytes( back, len, pat );
}

void _CGAPI     DGChar( BackRun& back, char value )
/**********************************/
{
    DGIBytes( back, 1, value );
}

void _CGAPI DGString( BackRun& back, cchar_ptr value, uint len )
/***********************************************/
{

    DGBytes( back, len, value );
}

void _CGAPI     DGUBytes( BackRun& back, uint_32 len )
/*************************************/
{
    DGBlip();
    IncLocation( back, len );
}

void _CGAPI     DGAlign( BackRun& back, uint align )
/***********************************/
{
    DGBlip();
    DataAlign( back, align );
}


uint_32 _CGAPI  DGSeek( BackRun& back, uint_32 where )
/*************************************/
{
    uint        old;


    DGBlip();
    old = AskLocation(back);
    SetLocation( back, where );

    return( old );
}

uint_32 _CGAPI      DGTell( BackRun& back )
/********************************/
{
    return( AskLocation(back) );
}


uint_32 _CGAPI      DGBackTell( BackRun& back, back_handle bck )
/***********************************************/
{

    if( !IS_REAL_BACK( bck ) )
        _Zoiks( ZOIKS_068 );

    return( AskAddress( back, bck->lbl ) );
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
