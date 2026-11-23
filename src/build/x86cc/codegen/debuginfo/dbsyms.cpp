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
* Description:  Emit debug information for debugging locals.
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "cgmem.h"
#include "optmain.h"
#include "zoiks.h"
#include "cgaux.h"
#include "data.h"
#include "types.h"
#include "makeins.h"
#include "objout.h"
#include "dbsyms.h"
#include "wvsyms.h"
#include "dw.h"
#include "dfsyms.h"
#include "cvdbg.h"
#include "cvsyms.h"
#include "namelist.h"
#include "makeblk.h"
#include "dbsupp.h"
#include "feprotos.h"
#include "cgprotos.h"
#include "backrun.hpp"


static const opcode_entry     DbgInfo[] = {
/*           op1   op2   res   eq      verify          reg           gen             fu  */
_OE( NONE, V_NO,           RG_,          G_DEBUG,        FU_NO )
};

static  void    SrcFileNoInit( BackRun& back )
/***********************************/
{
    back.dbsyms.DBFiles.lst = NULL;
    back.dbsyms.DBFiles.count = 0;
}

static  void    DBSrcFileFini( BackRun& back )
/***********************************/
{
    fname_lst   *curr;

    while( (curr = back.dbsyms.DBFiles.lst) != NULL ) {
        back.dbsyms.DBFiles.lst = curr->next;
        CGFree( back, curr );
    }
}

uint    _CGAPI DBSrcFile( BackRun& back, cchar_ptr fname )
/*****************************************/
{
    uint        index;
    size_t      len;
    fname_lst   *curr;
    fname_lst   **lnk;

    index = 0;
    for( lnk = &back.dbsyms.DBFiles.lst; (curr = *lnk) != NULL; lnk = &curr->next ) {
        if( strcmp( fname, curr->fname ) == 0 ) {
            return( index );
        }
        ++index;
    }
    len = strlen( fname ) + 1;
    curr = static_cast<fname_lst *>(CGAlloc( back, sizeof( *curr ) - 1 + len ));
    curr->len = len;
    curr->next = NULL;
    strcpy( curr->fname, fname );
    ++back.dbsyms.DBFiles.count;
    *lnk = curr;
    return( index );
}

const char *SrcFNoFind( BackRun& back, uint fno )
/********************************/
{
    uint        index;
    fname_lst   *curr;

    index = 0;
    for( curr  = back.dbsyms.DBFiles.lst; curr != NULL; curr = curr->next ) {
        if( index == fno ) {
            return( curr->fname );
        }
        ++index;
    }
    return( "unknown" );
}


static void  AddCueBlk( BackRun& back, cue_ctl *ctl )
/************************************/
{
    cue_blk *new_cb;

    new_cb = static_cast<cue_blk *>(CGAlloc( back, sizeof( *new_cb ) ));
    new_cb->next = NULL;
    *ctl->lnk = new_cb;
    ctl->curr = new_cb;
    ctl->lnk = &new_cb->next;
    ctl->next = &new_cb->info[0];
    ctl->end = &new_cb->info[CUES_PER_BLK];
}


static void  SourceCueInit( cue_ctl *ctl )
/****************************************/
{
    ctl->head = NULL;
    ctl->lnk  = &ctl->head;
    ctl->next = &ctl->start[0];
    ctl->end  = &ctl->start[1];
    ctl->state.cue = 0;
    ctl->state.fno = -1; /* force change */
    ctl->state.line = 0;
    ctl->state.col =  0;
    ctl->count = 0;
}

cue_idx CueAdd( BackRun& back, int fno, int line, int col )
/******************************************/
{
    enum {
        EQUAL    = 0x00,
        LINE_NO  = 0x01,
        COL_NO   = 0x02,
        FNO_NO   = 0x04,
    } cmp;
    cue_ctl     *ctl;
    int         diff;

    if( fno == 0 && col == 1 && line < PRIMARY_RANGE ) {
        return( line );
    }
    ctl = &back.dbsyms.LineInfo;
    cmp = EQUAL;
    if( fno != ctl->state.fno ) {
        cmp |= FNO_NO;
    }
    if( col != ctl->state.col ) {
        cmp |= COL_NO;
    }
    diff = line - ctl->state.line;
    if( diff != 0 ) {
       cmp |= LINE_NO;
    }
    if( cmp == LINE_NO && 0 <= diff && diff < MAX_LINE_DELTA  ) {
        ctl->state.line = line;
        ctl->state.cue += diff;
        cmp = EQUAL;
    }
    if( cmp ) {
        if( ctl->next >= ctl->end ) {
            AddCueBlk( back, ctl );
        }
        ++ctl->state.cue;
        ctl->state.fno = fno;
        ctl->state.line = line;
        ctl->state.col = col;
        *ctl->next = ctl->state;
        ++ctl->next;
        ++ctl->count;
    }
    return( ctl->state.cue + PRIMARY_RANGE );
}

bool CueFind( BackRun& back, cue_idx cue, cue_state *ret )
/*****************************************/
{
    cue_ctl     *ctl;
    cue_blk     *blk;
    cue_state   *hi;
    int         diff;

    ctl = &back.dbsyms.LineInfo;
    if( ctl->count == 0 || cue < PRIMARY_RANGE ) {
        ret->fno = 0;
        ret->line = cue;
        ret->col = 1;
        return( true );
    }
    cue -= PRIMARY_RANGE;
    if( cue < ctl->start[0].cue ) {
        Zoiks( back, ZOIKS_078 ); /* lower than low */
        return( false );
    }
    hi = &ctl->start[1];
    for( blk = ctl->head; blk != NULL; blk = blk->next ) {
        if( cue < blk->info[0].cue )
            break;
        hi = &blk->info[CUES_PER_BLK];
    }
    if( blk == NULL ) {
        /* if ctl->head == NULL then next is == ctl->start[1] */
        /* if cue in last blk next is end of entries in blk */
        hi = ctl->next;
        if( cue > ctl->state.cue ) { /* high than high */
            Zoiks( back, ZOIKS_078 );
            return( false );
        }
    }
    do {
        --hi;
    } while( ( diff = cue - hi->cue ) < 0 );
    *ret = *hi;
    ret->line += diff;
    return( true );
}

static void SourceCueFini( BackRun& back, cue_ctl *ctl )
/***************************************/
{
    cue_blk *list;
    cue_blk *next;

    for( list = ctl->head; list != NULL; list = next ) {
        next = list->next;
        CGFree( back, list );
    }
    ctl->head = NULL;
}

void    InitDbgInfo( BackRun& back )
/*************************/
{
//    cue_idx     idx;
    uint        fno;

    SrcFileNoInit(back);
    DBNested( back, false ); /* set nesting */
    SourceCueInit( &back.dbsyms.LineInfo );
    fno = DBSrcFile( back, static_cast<cchar_ptr>(FEAuxInfo( *back.front, NULL, FEINF_SOURCE_NAME )) );
//    idx = CueAdd( fno, 1, 1 );
    CueAdd( back, fno, 1, 1 );
    back.data.SrcLine = 1;
    if( IsModel( back, CGSW_GEN_DBG_DF ) ) {
        DFInitDbgInfo(back);
    } else if( IsModel( back, CGSW_GEN_DBG_CV ) ) {
        CVInitDbgInfo(back);
    } else {
        WVInitDbgInfo(back);
    }
}


void    FiniDbgInfo( BackRun& back )
/*************************/
{
    DBSrcFileFini(back);
    SourceCueFini( back, &back.dbsyms.LineInfo );
    if( IsModel( back, CGSW_GEN_DBG_DF ) ) {
        DFFiniDbgInfo();
    } else if( IsModel( back, CGSW_GEN_DBG_CV ) ) {
        CVFiniDbgInfo();
    } else {
        WVFiniDbgInfo();
    }
}


void    _CGAPI DBLineNum( BackRun& back, uint no )
/*********************************/
{
    back.data.SrcLine = no;
}

void _CGAPI     DBSrcCue( BackRun& back, uint fno, uint line, uint col )
/*******************************************************/
{
    cue_idx     idx;
    bool        hasxcue;
//    const char  *fname;

//  fname = SrcFNoFind( fno );
//  printf( "in %s %d %d\n", fname, line, col );
    hasxcue =  IsntModel( back, CGSW_GEN_DBG_TYPES ); // Just OMF line nums
    if( hasxcue ) {
        if( fno == 0 && col == 1 ) {
            DBLineNum( back, line );
        }
    } else {
        idx = CueAdd( back, fno, line, col );
        back.data.SrcLine = idx;
    }
}

void _CGAPI DBGenStMem( BackRun& back, cg_sym_handle sym, dbg_loc loc )
/******************************************************/
{
    if( IsModel( back, CGSW_GEN_DBG_DF ) ) {
        DFGenStatic( back, sym, loc );
    } else if( IsModel( back, CGSW_GEN_DBG_CV ) ) {
        CVGenStatic( back, sym, loc, true );
    } else {
    }
}

static  void    AddLocal( dbg_local **owner, dbg_local *lcl )
/***********************************************************/
{
    dbg_local *curr;

    while( (curr = *owner) != NULL ) {
        owner = &curr->link;
    }
    lcl->link = NULL;
    *owner = lcl;
}

static  dbg_block *MkBlock( BackRun& back )
/********************************/
{
    dbg_block   *blk;

    blk = static_cast<dbg_block *>(CGAlloc( back, sizeof( dbg_block ) ));
    blk->parent = back.data.CurrProc->targ.debug->blk;
    back.data.CurrProc->targ.debug->blk = blk;
    blk->locals = NULL;
    blk->bpatches = NULL;
    return( blk );
}


void _CGAPI DBGenSym( BackRun& back, cg_sym_handle sym, dbg_loc loc, int scoped )
/****************************************************************/
{
    fe_attr     attr;
    dbg_local   *lcl;

    if( IsModel( back, CGSW_GEN_DBG_LOCALS ) ) {
        attr = FEAttr( *back.front, sym );
        if( (attr & FE_IMPORT) == 0 ) {
            if( attr & FE_PROC ) {
                back.data.CurrProc->state.attr |= ROUTINE_WANTS_DEBUGGING;
                back.data.CurrProc->targ.debug = static_cast<dbg_rtn *>(CGAlloc( back, sizeof( dbg_rtn ) ));
                memset( back.data.CurrProc->targ.debug, 0, sizeof( dbg_rtn ) );
                back.data.CurrProc->targ.debug->parms = NULL;
                back.data.CurrProc->targ.debug->return_loc = LocDupl( loc );
                back.data.CurrProc->targ.debug->obj_type = DBG_NIL_TYPE;
                back.data.CurrProc->targ.debug->obj_loc = NULL;
                back.data.CurrProc->targ.debug->rtn_blk = MkBlock(back);
            } else if( scoped ) {
                lcl = static_cast<dbg_local *>(CGAlloc( back, sizeof( dbg_local ) ));
                lcl->sym = sym;
                lcl->loc = LocDupl( loc );
                lcl->kind = DBG_SYM_VAR;
                AddLocal( &back.data.CurrProc->targ.debug->blk->locals, lcl );
            } else {
                if( IsModel( back, CGSW_GEN_DBG_DF ) ) {
                    DFGenStatic( back, sym, loc );
                } else if( IsModel( back, CGSW_GEN_DBG_CV ) ) {
                    CVGenStatic( back, sym, loc, false );
                } else {
                    WVGenStatic( back, sym , loc );
                }
            }
        }
    }
}


void    _CGAPI DBModSym( BackRun& back, cg_sym_handle sym, cg_type indirect )
/************************************************************/
{
    fe_attr     attr;
    dbg_loc     loc;

    /* unused parameters */ (void)indirect;

    if( IsModel( back, CGSW_GEN_DBG_LOCALS ) ) {
        attr = FEAttr( *back.front, sym );
        if( (attr & FE_IMPORT) == 0 ) {
            if( attr & FE_PROC ) {
                loc = NULL;
            } else {
                loc = DBLocInit();
                loc = DBLocSym( back, loc, sym );
            }
            DBGenSym( back, sym, loc, false );
            DBLocFini( back, loc );
        }
    }
}


void _CGAPI DBObject( BackRun& back, dbg_type tipe, dbg_loc loc, cg_type ptr_type )
/******************************************************************/
{
    /* unused parameters */ (void)ptr_type;

    back.data.CurrProc->targ.debug->obj_type = tipe;
    back.data.CurrProc->targ.debug->obj_loc = LocDupl( loc );
    if( IsModel( back, CGSW_GEN_DBG_DF ) ) {
        //
    } else if( IsModel( back, CGSW_GEN_DBG_CV ) ) {
        //
    } else {
        WVObjectPtr( back, ptr_type );
    }
}


void    DBAllocReg( name *reg, name *temp )
/*****************************************/
{
    /* unused parameters */ (void)reg; (void)temp;
}

void _CGAPI DBTypeDef( BackRun& back, cchar_ptr nm, dbg_type tipe )
/**************************************************/
{
    if( IsModel( back, CGSW_GEN_DBG_DF ) ) {
        DFTypedef( back, nm, tipe );
    } else if( IsModel( back, CGSW_GEN_DBG_CV ) ) {
        CVTypedef( back, nm, tipe );
    } else {
    }
}

void    _CGAPI DBLocalSym( BackRun& back, cg_sym_handle sym, cg_type indirect )
/**************************************************************/
{
    fe_attr     attr;
    dbg_loc     loc;

    /* unused parameters */ (void)indirect;

    if( back.data.CurrProc->targ.debug != NULL ) {
        attr = FEAttr( *back.front, sym );
        if( (attr & FE_IMPORT) == 0 ) {
            loc = DBLocInit();
            loc = DBLocSym( back, loc, sym );
            DBGenSym( back, sym, loc, true );
            DBLocFini( back, loc );
        }
    }
}

void    _CGAPI DBLocalType( BackRun& back, cg_sym_handle sym, bool kind )
/********************************************************/
{
    dbg_local   *lcl;

    if( IsModel( back, CGSW_GEN_DBG_LOCALS ) ) {
        if( IsModel( back, CGSW_GEN_DBG_CV )
          || IsModel( back, CGSW_GEN_DBG_DF ) ) {
            lcl = static_cast<dbg_local *>(CGAlloc( back, sizeof( dbg_local ) ));
            lcl->sym = sym;
            lcl->loc = NULL;
            if( kind ) {
                lcl->kind = DBG_SYM_TYPE;
            } else {
                lcl->kind = DBG_SYM_TYPEDEF;
            }
            AddLocal( &back.data.CurrProc->targ.debug->blk->locals, lcl );
        }
    }
}

static  void    AddBlockInfo( BackRun& back, dbg_block *blk, bool start )
/********************************************************/
{
    instruction *ins;

    ins = MakeNop(back);
    ins->table = DbgInfo;
    ins->u.gen_table = ins->table;
    ins->flags.u.nop_flags = NOP_DBGINFO;
    if( start )
        ins->flags.u.nop_flags |= NOP_DBGINFO_START;
    ins->operands[0] = (name *)blk;
    AddIns( back, ins );
}


dbg_block *DoDBBegBlock( BackRun& back, int fast_codegen )
/*****************************************/
{
    dbg_block   *blk;

    if( back.data.CurrProc->targ.debug == NULL )
        return( NULL );
    blk = MkBlock(back);
    if( !fast_codegen ) {
        /*%%%% stick a NOP in the instruction stream, point it at block*/
        AddBlockInfo( back, blk, true );
    }
    return( blk );
}

void _CGAPI     DBBegBlock( BackRun& back )
/********************************/
{
    DoDBBegBlock( back, 0 );
}


void    DoDBEndBlock( BackRun& back, int fast_codegen )
/**************************************/
{
    dbg_block   *blk;

    if( back.data.CurrProc->targ.debug != NULL ) {
        blk = back.data.CurrProc->targ.debug->blk;
        if( !fast_codegen ) {
            AddBlockInfo( back, blk, false );
        }
        back.data.CurrProc->targ.debug->blk = blk->parent;
    }
}

void _CGAPI     DBEndBlock( BackRun& back )
/********************************/
{
    DoDBEndBlock( back, 0 );
}


void    DbgSetBase( BackRun& back )
/************************/
{
    if( IsModel( back, CGSW_GEN_DBG_DF ) ) {
        /* nothing */
    } else if( IsModel( back, CGSW_GEN_DBG_CV ) ) {
        CVSetBase();
    } else {
        WVSetBase(back);
    }
}


void    DbgParmLoc( BackRun& back, name *parm, cg_sym_handle sym )
/*************************************************/
// sym is NULL if no front end sym
{
    dbg_local       *lcl;
    dbg_loc         loc;

    if( IsntModel( back, CGSW_GEN_DBG_DF ) ) {
        if( parm->n.class_ != N_REGISTER  ) {
            return;
        }
    }
    lcl = static_cast<dbg_local *>(CGAlloc( back, sizeof( dbg_local ) ));
    loc = DBLocInit();
    loc = LocParm( back, loc, parm );
    lcl->loc = loc;
    lcl->sym = sym;
    lcl->kind = DBG_SYM_VAR;
    AddLocal( &back.data.CurrProc->targ.debug->parms, lcl );
}


void    DbgRetLoc( BackRun& back )
/***********************/
{
    dbg_loc loc;

    if( back.data.CurrProc->targ.debug->return_loc == NULL ) {
        loc = DBLocInit();
        loc = LocReg( back, loc, AllocRegName( back, back.data.CurrProc->state.return_reg ) );
        if( back.data.CurrProc->targ.return_points == NULL ) {
            /* nothing to do */
        } else if( back.data.CurrProc->state.attr & ROUTINE_ALLOCS_RETURN ) {
            loc->class_ = LOC_IND_REG + IND_RALLOC_NEAR;
        } else {
            loc->class_ = LOC_IND_REG + IND_CALLOC_NEAR;
        }
        back.data.CurrProc->targ.debug->return_loc = loc;
    }
}


/**/
/* Going into optimizer queue*/
/**/


void    DbgRetOffset( BackRun& back, type_length offset )
/****************************************/
{
    back.data.CurrProc->targ.debug->ret_offset = offset;
}


static  void    EmitDbg( BackRun& back, oc_class class_, pointer ptr )
/****************************************************/
{
    any_oc      oc;

    oc.oc_debug.hdr.class_ = static_cast<oc_class>(OC_INFO + class_);
    oc.oc_debug.hdr.reclen = sizeof( oc_debug );
    oc.oc_debug.hdr.objlen = 0;
    oc.oc_debug.ptr = ptr;
    InputOC( back, &oc );
}


void    EmitRtnBeg( BackRun& back )
/************************/
{
    EmitDbg( back, static_cast<oc_class>(OC_INFO_DBG_RTN_BEG), back.data.CurrProc->targ.debug );
}


void    EmitProEnd( BackRun& back )
/************************/
{
    EmitDbg( back, static_cast<oc_class>(OC_INFO_DBG_PRO_END), back.data.CurrProc->targ.debug );
}


void    EmitDbgInfo( BackRun& back, instruction *ins )
/*************************************/
{
    if( ins->flags.u.nop_flags & NOP_DBGINFO_START ) {
        EmitDbg( back, static_cast<oc_class>(OC_INFO_DBG_BLK_BEG), ins->operands[0] );
    } else {
        EmitDbg( back, static_cast<oc_class>(OC_INFO_DBG_BLK_END), ins->operands[0] );
    }
}


void    EmitEpiBeg( BackRun& back )
/************************/
{
    EmitDbg( back, static_cast<oc_class>(OC_INFO_DBG_EPI_BEG), back.data.CurrProc->targ.debug );
}


void    EmitRtnEnd( BackRun& back )
/************************/
{
    EmitDbg( back, static_cast<oc_class>(OC_INFO_DBG_RTN_END), back.data.CurrProc->targ.debug );
    { segment_id old_segid = ChangeOP( back, AskCodeSeg(back) );
        EmptyQueue(back);
    SetOP( back, old_segid ); }
}


/**/
/* Coming out of optimizer queue*/
/**/


void    DbgRtnBeg( BackRun& back, dbg_rtn *rtn,  offset lc )
/*******************************************/
{
    rtn->rtn_blk->start = lc;
    if( IsModel( back, CGSW_GEN_DBG_CV ) ) {
        CVRtnBeg( rtn, lc );
    }
}


void    DbgProEnd( BackRun& back, dbg_rtn *rtn, offset lc )
/******************************************/
{
    rtn->pro_size = lc - rtn->rtn_blk->start;
    if( IsModel( back, CGSW_GEN_DBG_DF ) ) {
        DFProEnd( back, rtn, lc );
    } else if( IsModel( back, CGSW_GEN_DBG_CV ) ) {
        CVProEnd( back, rtn, lc );
    }
}


void    DbgBlkBeg( BackRun& back, dbg_block *blk, offset lc )
/********************************************/
{
    blk->start = lc;
    if( IsModel( back, CGSW_GEN_DBG_DF ) ) {
        DFBlkBeg( back, blk, lc );
    } else if( IsModel( back, CGSW_GEN_DBG_CV ) ) {
        CVBlkBeg( back, blk, lc );
    }
}

void    DbgBlkEnd( BackRun& back, dbg_block *blk, offset lc )
/********************************************/
{
    if( IsModel( back, CGSW_GEN_DBG_DF ) ) {
        DFBlkEnd( back, blk, lc );
    } else if( IsModel( back, CGSW_GEN_DBG_CV ) ) {
        CVBlkEnd( back, blk, lc );
    } else {
        WVBlkEnd( back, blk, lc );
    }
    CGFree( back, blk );
}


void    DbgEpiBeg( BackRun& back, dbg_rtn *rtn, offset lc )
/******************************************/
{
    rtn->epi_start = lc;
    if( IsModel( back, CGSW_GEN_DBG_DF ) ) {
        DFEpiBeg( back, rtn, lc );
    } else if( IsModel( back, CGSW_GEN_DBG_CV ) ) {
        CVEpiBeg( rtn, lc );
    }
}


void    DbgRtnEnd( BackRun& back, dbg_rtn *rtn, offset lc )
/******************************************/
{
    if( IsModel( back, CGSW_GEN_DBG_DF ) ) {
        DFRtnEnd( back, rtn, lc );
    } else if( IsModel( back, CGSW_GEN_DBG_CV ) ) {
        CVRtnEnd( back, rtn, lc );
    } else {
        WVRtnEnd( back, rtn, lc );
    }
    CGFree( back, rtn->blk );
    CGFree( back, rtn );
}
