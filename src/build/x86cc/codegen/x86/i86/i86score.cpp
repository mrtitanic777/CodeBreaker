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
* Description:  Scoreboarding routines specific to the i86.
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "score.h"
#include "data.h"
#include "makeins.h"
#include "namelist.h"
#include "rgtbl.h"
#include "x86segs.h"
#include "backrun.hpp"


#define S_MAX   3
#define S_DS    0
#define S_SS    1
#define S_ES    2

#define I_MAX   3
#define I_BX    0
#define I_DI    1
#define I_SI    2

namespace {

/* the statics of this file, one set for each run of the compiler */
}  // namespace
struct ScoreFileState {
    name *PtrRegs[S_MAX][I_MAX];
    name *DS;
    name *SS;
    name *ES;
};
namespace {

}  // namespace



void    ScInitRegs( BackRun& back, score_hdl scoreboard )
/*****************************************
 * Add some register equality "truths" to the scoreboard "sc"
 */
{
    int     ss;
    int     ds;

    if( _IsntTargetModel( CGSW_X86_FLOATING_DS )
      && _IsntTargetModel( CGSW_X86_FLOATING_SS ) ) {
        ss = AllocRegName( back, HW_SS )->r.reg_index;
        ds = AllocRegName( back, HW_DS )->r.reg_index;
        if( !RegsEqual( scoreboard, ss, ds ) ) {
            RegInsert( back, scoreboard, ss, ds );
        }
    }
}


static  name    *NewRegName( BackRun& back, hw_reg_set reg )
/********************************************
 * Allocate a new "far pointer" register an set its class
 */
{
    name        *reg_name;

    reg_name = AllocRegName( back, reg );
    reg_name->n.type_class = CP;
    return( reg_name );
}


void    AddRegs( BackRun& back )
/******************************
 * Add some registers to the N_REGISTER list, so that we can do
 * scoreboarding on them
 */
{
    hw_reg_set  lo_part;
    name        *reg_name;
    int         i;
    int         j;

    back.score_file->DS = AllocRegName( back, HW_DS );
    back.score_file->SS = AllocRegName( back, HW_SS );
    back.score_file->ES = AllocRegName( back, HW_ES );
    for( i = S_MAX; i-- > 0; ) {
        for( j = I_MAX; j-- > 0; ) {
            back.score_file->PtrRegs[i][j] = NULL;
        }
    }
    for( reg_name = back.data.Names[N_REGISTER]; reg_name != NULL; reg_name = reg_name->n.next_name ) {
        if( IsIndexReg( back, reg_name->r.reg, CP, false ) ) {
            if( HW_COvlap( reg_name->r.reg, HW_DS )
             || HW_COvlap( reg_name->r.reg, HW_ES )
             || HW_COvlap( reg_name->r.reg, HW_SS ) ) {
                lo_part = LowReg( back, reg_name->r.reg );
                if( HW_COvlap( lo_part, HW_BX ) ) {
                    back.score_file->PtrRegs[S_DS][I_BX] = NewRegName( back, HW_DS_BX );
                    back.score_file->PtrRegs[S_SS][I_BX] = NewRegName( back, HW_SS_BX );
                    back.score_file->PtrRegs[S_ES][I_BX] = NewRegName( back, HW_ES_BX );
                } else if( HW_COvlap( lo_part, HW_SI ) ) {
                    back.score_file->PtrRegs[S_DS][I_SI] = NewRegName( back, HW_DS_SI );
                    back.score_file->PtrRegs[S_SS][I_SI] = NewRegName( back, HW_SS_SI );
                    back.score_file->PtrRegs[S_ES][I_SI] = NewRegName( back, HW_ES_SI );
                } else if( HW_COvlap( lo_part, HW_DI ) ) {
                    back.score_file->PtrRegs[S_DS][I_DI] = NewRegName( back, HW_DS_DI );
                    back.score_file->PtrRegs[S_SS][I_DI] = NewRegName( back, HW_SS_DI );
                    back.score_file->PtrRegs[S_ES][I_DI] = NewRegName( back, HW_ES_DI );
                }
            }
        }
    }
}


void    ScoreSegments( BackRun& back, score_hdl scoreboard )
/********************************************
 * Do special scoreboarding on segment registers.  Given that BX = DI,
 * for example, we know that SS:BX = SS:DI, and DS:BX = DS:DI.
 */
{
    score_hdl   ds;
    score_hdl   xs;
    int         i;
    name        *dst;
    name        *src;

    ds = &scoreboard[back.score_file->DS->r.reg_index];
    for( xs = ds->next_reg; xs != ds; xs = xs->next_reg ) {
        if( xs->index == back.score_file->SS->r.reg_index ) {
            for( i = I_MAX; i-- > 0; ) {
                dst = back.score_file->PtrRegs[S_SS][i];
                src = back.score_file->PtrRegs[S_DS][i];
                if( dst != NULL && src != NULL ) {
                    if( !RegsEqual( scoreboard, dst->r.reg_index, src->r.reg_index ) ) {
                        RegInsert( back, scoreboard, dst->r.reg_index, src->r.reg_index );
                    }
                }
            }
        } else if( xs->index == back.score_file->ES->r.reg_index ) {
            for( i = I_MAX; i-- > 0; ) {
                dst = back.score_file->PtrRegs[S_ES][i];
                src = back.score_file->PtrRegs[S_DS][i];
                if( dst != NULL && src != NULL ) {
                    if( !RegsEqual( scoreboard, dst->r.reg_index, src->r.reg_index ) ) {
                        RegInsert( back, scoreboard, dst->r.reg_index, src->r.reg_index );
                    }
                }
            }
        }
    }
}


bool    ScAddOk( BackRun& back, hw_reg_set reg1, hw_reg_set reg2 )
/**********************************************************
 * Is it ok to say that "reg1" = "reg2"?  This is not ok for
 * unalterable registers since there may be hidden modifications of
 * these registers.
 */
{
    if( HW_Ovlap( reg1, back.data.CurrProc->state.unalterable ) ) {
        if( !HW_CEqual( reg1, HW_DS ) && !HW_CEqual( reg1, HW_SS ) ) {
            return( false );
        }
    }
    if( HW_Ovlap( reg2, back.data.CurrProc->state.unalterable ) ) {
        if( !HW_CEqual( reg2, HW_DS ) && !HW_CEqual( reg2, HW_SS ) ) {
            return( false );
        }
    }
    return( true );
}


bool    ScConvert( BackRun& back, instruction *ins )
/********************************************
 * Get rid of instructions like CBW if the high part is not used in the
 * next instruction.
 */
{
    hw_reg_set  tmp;

    if( G( ins ) == G_SIGNEX ) {
        tmp = HighReg( back, ins->result->r.reg );
        if( !HW_Ovlap( ins->head.next->head.live.regs, tmp ) ) {
            FreeIns( back, ins ); /* get rid of the pesky cwd or cbw instruction!*/
            return( true );
        }
    }
    return( false );
}


bool    CanReplace( instruction *ins )
/*********************************************
 * Are we allowed to replace any of the operands of "ins" with
 * different registers?  For long shifts the answer is no since CX is a
 * must for the loop counter.
 */
{
    if( ( ins->head.opcode == OP_LSHIFT
       || ins->head.opcode == OP_RSHIFT )
      && ( ins->type_class == U4
       || ins->type_class == I4 ) ) return( false );
    return( true );
}


bool    ScRealRegister( name *reg )
/******************************************
 * Return "true" if "reg" is a real machine register and not some
 * monstrosity like AX:DX:BX used for calls.
 */
{
    return( reg->n.type_class != XX );
}

/* the run makes and frees its copy of this file's variables (run/ files: see the run header) */
#if _TARGET & _TARG_8086
ScoreFileState* score_file_make() { return new ScoreFileState(); }
void score_file_free(ScoreFileState* part) { delete part; }
#endif
