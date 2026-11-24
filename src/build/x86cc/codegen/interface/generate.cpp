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
* Description:  High level code generation routines. Lots of action here.
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "memcheck.h"
#include "data.h"
#include "types.h"
#include "addrcnst.h"
#include "fpu.h"
#include "makeins.h"
#include "foldins.h"
#include "rtrtn.h"
#include "objout.h"
#include "makeaddr.h"
#include "objio.h"
#include "dominate.h"
#include "inssched.h"
#include "peepopt.h"
#include "memlimit.h"
#include "blips.h"
#include "optmain.h"
#include "opttell.h"
#include "object.h"
#include "regalloc.h"
#include "targetin.h"
#include "makeblk.h"
#include "indvars.h"
#include "loopopts.h"
#include "nullprop.h"
#include "rgtbl.h"
#include "expand.h"
#include "insutil.h"
#include "insdead.h"
#include "namelist.h"
#include "typemap.h"
#include "blktrim.h"
#include "optimize.h"
#include "index.h"
#include "fixindex.h"
#include "conflict.h"
#include "scmain.h"
#include "generate.h"
#include "inline.h"
#include "cse.h"
#include "dataflo.h"
#include "flograph.h"
#include "multiply.h"
#include "splitvar.h"
#include "temps.h"
#include "varusage.h"
#include "liveinfo.h"
#include "regtree.h"
#include "breakrtn.h"
#include "condcode.h"
#include "trecurse.h"
#include "feprotos.h"
#include "backrun.hpp"


namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct GenerateState: generate_state.h) */

}  // namespace


void    InitCG( BackRun& back )
/********************/
{
    back.optmac.InOptimizer = 0;
    back.data.InsId = 0;
    back.data.CurrProc = NULL;
    back.data.CurrBlock = NULL;
    back.data.BlockList = NULL;
    back.data.HeadBlock = NULL;
    back.data.BlockByBlock = false;
    back.generate.abortCG = false;
    FPInit(back);/* must be before InitRegTbl */
    InitRegTbl(back);
    ScoreInit(back);
    InitQueue(back);
    InitMakeAddr(back);
    RegTreeInit(back);
    InitIns(back);
    InitConflict(back);
    InitRT(back);
    InitNames(back);
    ObjInit(back);
    back.data.ClassPointer = TypeClass( back, back.data.TypePtr );
    InitSegment();
}


void    AbortCG( BackRun& back )
/*********************/
{
    back.generate.abortCG = true;
}


void    FiniCG( BackRun& back )
/********************/
{
    FiniQueue(back);
    FiniSegment();
    if( back.generate.abortCG ) {
        AbortObj(back);
    }
    ObjFini(back);
    TellFreeAllLabels(back);
    BlowAwayFreeLists(back);
    ScoreFini();
}


static  void    AddANop( BackRun& back )
/*****************************/
{
    AddAnIns( back, back.data.BlockList, MakeNop(back) );
}


static  void            PreOptimize( BackRun& back )
/*****************************************/
{
    bool        change;

    if( IsntModel( back, CGSW_GEN_NO_OPTIMIZATION ) ) {
//        CharsAndShortsToInts();
        MakeMovAddrConsts(back);
        PushPostOps(back);
        DeadTemps(back);
        InsDead(back);
        MakeFlowGraph(back);
        BlockTrim(back);
        CommonSex( back, IsModel( back, CGSW_GEN_LOOP_OPTIMIZATION ) );
        SetOnCondition(back);
        BlockTrim(back);
        AddANop(back);
        if( IsModel( back, CGSW_GEN_LOOP_OPTIMIZATION ) ) {
            change = false;
            if( TransLoops( back, false ) ) {
                change = true;
            }
            if( LoopInvariant(back) ) {
                change = true;
            }
            if( change ) {
                CommonSex(back, true);
                InsDead(back);
                CommonInvariant(back);
            }
            if( IndVars(back) ) {
                CommonSex(back, false);
                InsDead(back);
                change = true;
            }
            BlockTrim(back);
            if( TransLoops( back, true ) ) {
                BlockTrim(back);
                CommonSex( back, false );
                change = true;
            }
            if( change ) {
                ReConstFold(back);
            }
            LoopEnregister(back);
            if( change ) {
                BlockTrim(back);
            }
        }
        MulToShiftAdd(back);
        KillMovAddrConsts(back);
        FindReferences(back);
    } else {
        MakeFlowGraph(back);
        AddANop(back);
        FindReferences(back);
    }
}


static  block           *NextBlock( BackRun& back, block *blk, void *parm )
/**********************************************************/
{
    /* unused parameters */ (void)parm;

    return( blk->next_block );
}


static  void            PostOptimize( BackRun& back )
/******************************************/
{
    if( IsntModel( back, CGSW_GEN_NO_OPTIMIZATION ) ) {
        /*
         * Run peephole optimizer again. Important: It is critical that the
         * new instructions can be directly generated because RegAlloc is
         * done by now. PeepOpt() is responsible for verifying that.
         */
        if( PeepOpt( back, back.data.HeadBlock, NextBlock, NULL, true ) ) {
            LiveInfoUpdate(back);
        }
        /*
         * this is important as BuildIndex cannot handle instructions with no operands
         */
        DeadInstructions(back);
        BuildIndex(back);
        DeadInstructions(back);
    }
    MergeIndex(back);
    if( IsntModel( back, CGSW_GEN_NO_OPTIMIZATION ) ) {
        /*
         * Calling Conditions() at this point has nice optimization effect,
         * but doesn't working correctly right now. It optimizes conditions
         * making them dependent from previous conditions codes, but riscifier
         * generates XOR's which will trash cond. codes. Either Conditions()
         * either riscifier must be fixed to handle this situation.
         */
//        OptCloseMoves();  /* todo: merge constant moves before riscifier */
        LdStAlloc(back);
        Score(back);
        DeadInstructions(back); /* cleanup junk after Score() */
        /*
         * deRISCify before LoopRegInvariant() or Shedule() are run:
         * they're moving RISCified pair.
         */
        LdStCompress(back);
        /*
         * Reuse registers freed by deriscifier
         */
        Score(back);
        DeadInstructions(back); /* cleanup junk after Score() */
        if( !back.data.BlockByBlock )
            LoopRegInvariant(back);
        /*
         * Get rid of remaining unused conditions on register level.
         */
        if( IsntTargetModel( back, CGSW_X86_STATEMENT_COUNTING ) ) {
            Conditions(back);
        }
    }
    FPExpand(back);
    if( IsntModel( back, CGSW_GEN_NO_OPTIMIZATION ) ) {
        DeadInstructions(back);   /* cleanup junk after Conditions() */
        /*
         * Run scheduler last, when all instructions are stable
         */
        if( IsModel( back, CGSW_GEN_INS_SCHEDULING ) ) {
            back.data.HaveLiveInfo = false;
            Schedule(back); /* NOTE: Schedule messes up live information */
            LiveInfoUpdate(back);
            back.data.HaveLiveInfo = true;
        }
        /*
         * run this again in case Scheduler messed around with indices
         */
        if( PeepOpt( back, back.data.HeadBlock, NextBlock, NULL, true ) ) {
            LiveInfoUpdate(back);
        }
    }
    FPOptimize(back);
}


static  void    FreeExtraTemps( BackRun& back, name *last, block_id blk_id )
/***********************************************************/
{
    name        **owner;
    name        *temp;

    owner = &back.data.Names[N_TEMP];
    for( ;; ) {
        temp = *owner;
        if( temp == last )
            break;
        if( (temp->v.usage & USE_IN_ANOTHER_BLOCK) == 0
          && !FrontEndTmp( temp )
          && temp->t.u.blk_id == blk_id ) {
            *owner = temp->n.next_name;
            FreeAName( back, temp );
        } else {
            owner = &temp->n.next_name;
        }
    }
}


static  void    ForceTempsMemory( BackRun& back )
/**************************************/
{
    name        *op;
    name        *next;

    ParmPropagate(back);
    for( op = back.data.Names[N_TEMP]; op != back.data.LastTemp; op = next ) {
        next = op->n.next_name;
        if( (op->v.usage & USE_IN_ANOTHER_BLOCK)
          || FrontEndTmp( op ) ) {
            op = DeAlias( op );
            op->v.usage |= NEEDS_MEMORY | USE_MEMORY;
            while( op->v.conflict != NULL ) {
                FreeAConflict( back, op->v.conflict );
            }
        }
    }
    AssignOtherLocals(back);
    for( op = back.data.Names[N_MEMORY]; op != NULL; op = op->n.next_name ) {
        op->v.usage |= USE_IN_ANOTHER_BLOCK | USE_MEMORY;
        if( op->v.conflict != NULL ) {
            FreeAConflict( back, op->v.conflict );
            op->v.conflict = NULL;
        }
    }
    back.data.LastTemp = back.data.Names[N_TEMP];
}


static  void    BlockToCode( BackRun& back, bool partly_done )
/*********************************************/
{
    block_num           inputs;
    block_num           targets;
    block_edge          *input_edges;
    conflict_node       *curr;
    conflict_node       **owner;
    conflict_node       *conflist;
    block_id            blk_id;


    /* try to get back some memory*/
    MemLow( back );

    /* make the block look like an entire procedure*/

    back.data.HeadBlock = back.data.CurrBlock;
    back.data.BlockList = back.data.CurrBlock;
    back.data.HeadBlock->prev_block = NULL;
    if( back.data.HeadBlock->next_block != NULL ) {
        back.data.HeadBlock->next_block->prev_block = NULL;
    }
    /* Kludge - need a pointer to the next block for CALL_LABEL - puke! */
    if( IsBlkAttr( back.data.HeadBlock, BLK_CALL_LABEL ) ) {
        back.data.HeadBlock->u2.next = back.data.HeadBlock->next_block;
    }
    back.data.HeadBlock->next_block = NULL;

    /* force anything that spans blocks to memory */

    back.data.HeadBlock->u.partition = back.data.HeadBlock;
    ConstFold( back, back.data.HeadBlock );
    back.data.HeadBlock->u.partition = NULL;

    ForceTempsMemory(back);
    if( !partly_done ) {
        FixMemRefs(back);
        back.data.HaveLiveInfo = false;
        if( IsntModel( back, CGSW_GEN_NO_OPTIMIZATION )
          && IsntModel( back, CGSW_GEN_FORTRAN_ALIASING ) ) {
            FindReferences(back);
            CommonSex(back, false);
            PushPostOps(back);
        }
        FindReferences(back);
        DeadTemps(back);
        if( IsModel( back, CGSW_GEN_NO_OPTIMIZATION ) ) {
            SetInOut( back, back.data.HeadBlock );
        } else {
            MakeConflicts(back);
        }
        MakeLiveInfo(back);
        back.data.HaveLiveInfo = true;
        AxeDeadCode(back);
        FixIndex(back);
        FixSegments(back);
        FPRegAlloc(back);
        RegAlloc( back, true );
        FreeConflicts(back);
        back.data.HaveLiveInfo = false;
    } else {
        conflist = NULL;
        owner = &back.data.ConfList;
        for( ;; ) {
            curr = *owner;
            if( curr == NULL )
                break;
            if( curr->start_block == back.data.HeadBlock ) {
                *owner = curr->next_conflict;
                curr->next_conflict = conflist;
                conflist = curr;
            } else {
                owner = &curr->next_conflict;
            }
        }
        curr = back.data.ConfList;
        back.data.ConfList = conflist;
        RegAlloc( back, true );
        FreeConflicts(back);
        back.data.ConfList = curr;
    }
    input_edges = back.data.HeadBlock->input_edges;
    inputs = back.data.HeadBlock->inputs;
    targets = back.data.HeadBlock->targets;
    back.data.HeadBlock->inputs = 0;
    back.data.HeadBlock->input_edges = NULL;
    back.data.HeadBlock->targets = 0;
    FPParms(back);
    PostOptimize(back);
    back.data.HeadBlock->input_edges = input_edges;
    back.data.HeadBlock->inputs = inputs;
    back.data.HeadBlock->targets = targets;

    /* generate a prolog that saves all registers*/

    if( (back.data.CurrProc->prolog_state & PST_PROLOG_GENERATED) == 0 ) {
        back.data.CurrProc->state.used = AllCacheRegs(back);
        GenProlog(back);
    }

    blk_id = back.data.CurrBlock->blk_id;
    AssgnMoreTemps( back, blk_id );
    OptSegs(back);

    /* generate the code for the block*/

    if( IsBlkAttr( back.data.CurrBlock, BLK_RETURN ) ) {
        GenObject(back);
        FiniStackMap(back);
        FreeProc(back);
    } else {
        GenObject(back);
    }
    if( partly_done ) {
        FreeExtraTemps( back, NULL, blk_id );
    } else {
        FreeExtraTemps( back, back.data.LastTemp, blk_id );
    }
}


static  void    FlushBlocks( BackRun& back, bool partly_done )
/*********************************************/
/* we're in deep trouble. Try to get out of it using as little memory as*/
/* possible*/
{
    block       *blk;
    block       *next;
    block       *curr;
    block_class classes;

    if( !back.data.BlockByBlock ) {
        FEMessageCurrProc( back, FEMSG_REGALLOC_DIED );
    }
    if( !partly_done ) {
        Renumber(back);
    }
    curr = back.data.CurrBlock;
    back.data.BlockByBlock = true;
    classes = static_cast<block_class>(0);
    for( blk = back.data.HeadBlock; blk != NULL; blk = next ) {
        next = blk->next_block;
        classes |= blk->class_;
        back.data.CurrBlock = blk;
        BlockToCode( back, partly_done );
        FlushOpt(back);
    }
    back.data.CurrBlock = curr;
    back.data.HeadBlock = NULL;
    back.data.BlockList = NULL;
}


static  void    FreeExtraSyms( BackRun& back, name *last )
/*****************************************/
{
    name        **owner;
    name        *temp;
    name        *next;

    owner = &back.data.Names[N_TEMP];
    for( ;; ) {
        temp = *owner;
        if( temp == last )
            break;
        if( (temp->v.usage & USE_IN_ANOTHER_BLOCK) == 0
          && !FrontEndTmp( temp ) ) {
            *owner = temp->n.next_name;
            FreeAName( back, temp );
        } else {
            owner = &temp->n.next_name;
        }
    }
    for( temp = back.data.Names[N_CONSTANT]; temp != NULL; temp = next ) {
        next = temp->n.next_name;
        FreeAName( back, temp );
    }
    back.data.Names[N_CONSTANT] = NULL;
    for( temp = back.data.Names[N_INDEXED]; temp != NULL; temp = next ) {
        next = temp->n.next_name;
        FreeAName( back, temp );
    }
    back.data.Names[N_INDEXED] = NULL;
}


static  void    FinishIndex( BackRun& back )
/*********************************/
{
    block       *blk;
    instruction *ins;
    instruction *next;

    for( blk = back.data.HeadBlock; blk != NULL; blk = blk->next_block ) {
        for( ins = blk->ins.head.next; ins->head.opcode != OP_BLOCK; ins = next ) {
            next = NeedIndex( back, ins );
            if( next == ins ) {
                next = ins->head.next;
            }
        }
    }
}


static  void    ForceConflictsMemory( BackRun& back )
/******************************************/
{
    conflict_node       *conf;
    conflict_node       *next;

    ParmPropagate(back);
    for( conf = back.data.ConfList; conf != NULL; conf = conf->next_conflict ) {
        SetFalse( conf, CST_NEEDS_INDEX_SPLIT | CST_NEEDS_SEGMENT_SPLIT );
    }
    for( conf = back.data.ConfList; conf != NULL; conf = next ) {
        next = conf->next_conflict;
        if( !GBitEmpty( conf->id.out_of_block )
          || ( conf->name->n.class_ == N_TEMP && FrontEndTmp( conf->name ) )
          || ( conf->name->n.class_ == N_MEMORY && conf->name->v.symbol != NULL )
          || (conf->name->v.usage & USE_IN_ANOTHER_BLOCK) ) {
            next = InMemory( back, conf );
        }
    }
}


static  void    Panic( BackRun& back, bool partly_done )
/***************************************/
{
    proc_def    *curr_proc;

    if( partly_done ) {
        FinishIndex(back);
    }
    ForceConflictsMemory(back);
    SortBlocks(back);
    UnFixEdges(back);
    InitStackMap(back);
    AssgnMoreTemps( back, BLK_ID_NONE );
    curr_proc = back.data.CurrProc;
    FlushBlocks( back, partly_done );
    if( curr_proc == back.data.CurrProc /* if not freed (dummy!) */
      && (back.data.CurrProc->prolog_state & PST_EPILOG_GENERATED) == 0 ) {
        GenEpilog(back);
        FiniStackMap(back);
        FreeProc(back);
    }
}


static  void            GenPartialRoutine( BackRun& back, bool routine_done )
/************************************************************/
{
    while( routine_done || MemLow( back ) ) {
        if( CreateBreak(back) ) {
            back.data.BlockByBlock = true;
            BlockTrim(back);
            FindReferences(back);
            PreOptimize(back);
            FixBreak(back);
            SortBlocks(back);
            /* so the front gets told we're panicking */
            back.data.BlockByBlock = false;
            FlushBlocks( back, false );
            RemoveBreak(back);
            if( back.data.HeadBlock == NULL ) {
                FreeExtraSyms( back, NULL );
            }
        } else {
            FlushBlocks( back, false );
            FreeExtraSyms( back, NULL );
            break;
        }
    }
}


void    FEMessageCurrProc( BackRun& back, fe_msg femsg )
/***************************************/
{

    if( IsntModel( back, CGSW_GEN_NO_OPTIMIZATION )
      && back.generate.FEMessageCurrProc_lastproc != back.data.CurrProc ) {
        FEMessage( *back.front, femsg, AskForLblSym( back, static_cast<label_handle>(back.data.CurrProc->label) ) );
        back.generate.FEMessageCurrProc_lastproc = back.data.CurrProc;
    }
}


void    Generate( BackRun& back, bool routine_done )
/***********************************/
/* The big one - here's where most of code generation happens.
 * Follow this routine to see the transformation of code unfold.
 */
{
    if( BGInInline(back) )
        return;
    back.data.HaveLiveInfo = false;
    back.data.HaveDominatorInfo = false;
    /*
     * if we want to go fast, generate statement at a time
     */
    if( IsModel( back, CGSW_GEN_NO_OPTIMIZATION ) ) {
        if( !back.data.BlockByBlock ) {
            InitStackMap(back);
            back.data.BlockByBlock = true;
        }
        LNBlip( back.data.SrcLine );
        FlushBlocks( back, false );
        FreeExtraSyms( back, back.data.LastTemp );
        if( MemLow( back ) ) {
            BlowAwayFreeLists(back);
        }
        return;
    }
    /*
     * if we couldn't get the whole procedure in memory, generate part of it
     */
    if( back.data.BlockByBlock ) {
        if( MemLow( back )
          || routine_done ) {
            GenPartialRoutine( back, routine_done );
        } else {
            BlkTooBig(back);
        }
        return;
    }
    /*
     * if we're low on memory, go into BlockByBlock mode
     */
    if( MemLow( back ) ) {
        InitStackMap(back);
        GenPartialRoutine( back, routine_done );
        BlowAwayFreeLists(back);
        return;
    }
    /*
     * check to see that no basic block gets too unwieldy
     */
    if( !routine_done ) {
        BlkTooBig(back);
        return;
    }
    /*
     * The routine is all in memory. Optimize and generate it
     */
    FixReturns(back);
    FixEdges(back);
    Renumber(back);
    BlockTrim(back);
    FindReferences(back);
    TailRecursion(back);
    NullConflicts( back, USE_IN_ANOTHER_BLOCK );
    InsDead(back);
    FixMemRefs(back);
    FindReferences(back);
    PreOptimize(back);
    PropNullInfo(back);
    MemtoBaseTemp();
    if( MemCritical( back ) ) {
        Panic( back, false );
        return;
    }
    MakeConflicts(back);
    if( IsModel( back, CGSW_GEN_LOOP_OPTIMIZATION ) ) {
        SplitVars(back);
    }
    AddCacheRegs(back);
    MakeLiveInfo(back);
    back.data.HaveLiveInfo = true;
    AxeDeadCode(back);
    /*
     * AxeDeadCode() may have emptied some blocks. Run BlockTrim() to get rid
     * of useless conditionals, then redo conflicts etc. if any blocks died.
     */
    if( BlockTrim(back) ) {
        FreeConflicts(back);
        NullConflicts( back, static_cast<var_usage>(EMPTY) );
        FindReferences(back);
        MakeConflicts(back);
        MakeLiveInfo(back);
    }
    FixIndex(back);
    FixSegments(back);
    FPRegAlloc(back);
    if( !RegAlloc( back, false ) ) {
        Panic( back, true );
        back.data.HaveLiveInfo = false;
        return;
    }
    FPParms(back);
    FixMemBases();
    PostOptimize(back);
    InitStackMap(back);
    AssignTemps(back);
    FiniStackMap(back);
    FreeConflicts(back);
    SortBlocks(back);
    if( CalcDominatorInfo(back) ) {
        back.data.HaveDominatorInfo = true;
    }
    GenProlog(back);
    UnFixEdges(back);
    OptSegs(back);
    GenObject(back);
    if( (back.data.CurrProc->prolog_state & PST_EPILOG_GENERATED) == 0 ) {
        GenEpilog(back);
    }
    FreeProc(back);
    back.data.HaveLiveInfo = false;
    if( IsTargetModel( back, CGSW_X86_NEW_P5_PROFILING ) ) {
        FlushQueue(back);
    }
}
