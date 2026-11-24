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
* Description:  Generate calls to runtime support routines.
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "zoiks.h"
#include "seldef.h"
#include "cgauxcc.h"
#include "cgauxinf.h"
#include "makeins.h"
#include "data.h"
#include "rtrtn.h"
#include "objout.h"
#include "namelist.h"
#include "makeblk.h"
#include "insutil.h"
#include "rgtbl.h"
#include "rtcall.h"
#include "inssegs.h"
#include "conflict.h"
#include "x86segs.h"
#include "liveinfo.h"
#include "x86rtrn.h"
#include "bldselco.h"
#include "_x86rtrn.h"
#include "backrun.hpp"


/*
 * If you add a new routine, add it to the debugger's symbol list
 * so the debugger can recognize it.
 */
const rtn_info RTInfo[] = {
    #define PICK(e,name,op,class_,left,right,result) {name, op, class_, left, right, result},
    #define PICK1(e,name,op,class_,left,right,result) __FP80BIT(PICK(e,name,op,class_,left,right,result),)
    #include "_rtinfo.h"
    #undef PICK1
    #undef PICK
};


namespace {

/* the statics of this file, one set for each run of the compiler */
}  // namespace
struct X386rtrtnState {
    const struct STRUCT_BYTE_SEQ( 2 ) Scn1 = {
         2, false,
        {0xF2,           /*       repne     */
         0xAE}           /*       scasb     */
    };
    const struct STRUCT_BYTE_SEQ( 6 ) Scn1ES = {
         6, false,
        {0x06,          /*      push    es  */
         0x0e,          /*      push    cs  */
         0x07,          /*      pop     es  */
         0xF2,          /*      repne       */
         0xAE,          /*      scasb       */
         0x07}          /*      pop     es  */
    };
    const struct STRUCT_BYTE_SEQ( 3 ) Scn2 = {    /* or Scn4 in USE16 */
         3, false,
        {0xF2,          /*      repne       */
         0x66, 0xAF}    /*      scasw       */
    };
    const struct STRUCT_BYTE_SEQ( 7 ) Scn2ES = {  /* or Scn4 in USE16 */
         7, false,
        {0x06,          /*      push    es  */
         0x0e,          /*      push    cs  */
         0x07,          /*      pop     es  */
         0xF2,          /*      repne       */
         0x66, 0xAF,    /*      scasw       */
         0x07}          /*      pop     es  */
    };
    const struct STRUCT_BYTE_SEQ( 2 ) Scn4 = {    /* or Scn2 in USE16 */
         2, false,
        {0xF2,          /*      repne       */
         0xAF}          /*      scasd       */
    };
    const struct STRUCT_BYTE_SEQ( 6 ) Scn4ES = {  /* or Scn2 in USE16 */
         6, false,
        {0x06,          /*      push    es  */
         0x0e,          /*      push    cs  */
         0x07,          /*      pop     es  */
         0xF2,          /*      repne       */
         0xAF,          /*      scasd       */
         0x07}          /*      pop     es  */
    };
};
namespace {

}  // namespace



const char  *AskRTName( BackRun& back, rt_class rtindex )
/****************************************/
{
    if( IsTargetModel( back, CGSW_X86_INDEXED_GLOBALS ) ) {
        switch( rtindex ) {
        case RT_FDA:
            return( "__FXA" );
        case RT_FDS:
            return( "__FXS" );
        case RT_FDM:
            return( "__FXM" );
        case RT_FDD:
            return( "__FXD" );
        case RT_FDC:
            return( "__FXC" );
        case RT_DPOW:
            return( "RT@XPOW" );
        case RT_DATAN2:
            return( "IF@XATAN2" );
        case RT_DFMOD:
            return( "IF@XFMOD" );
        }
    }
    return( RTInfo[rtindex].name );
}


bool    RTLeaveOp2( BackRun& back, instruction *ins )
/*************************************
 * return true if it's a bad idea to put op2 into a temporary since we're
 * gonna take the bugger's address in rMAKECALL.
 */
{
    /* unused parameters */ (void)ins;

    return( false );
}


name    *ScanCall( BackRun& back, tbl_control *table, name *value, type_class_def type_class )
/******************************************************************************
 * generates a fake call to a runtime routine that looks up "value" in a table
 * and jumps to the appropriate case, using either a pointer or index
 * returned by the "routine". The "routine" will be generated inline later.
 * See BEAuxInfo for the code sequences generated. That will explain
 * how the jump destination is determined as well.
 */
{
    instruction *new_ins;
    name        *reg_name;
    name        *result;
    name        *label;
    hw_reg_set  tmp;
    rt_class    rtindex;

    switch( type_class ) {
    case U1:
        rtindex = RT_SCAN1;
        break;
    case U2:
        rtindex = RT_SCAN2;
        break;
    case U4:
        rtindex = RT_SCAN4;
        break;
    default:
        rtindex = RT_NOP;
        break;
    }

    reg_name = AllocRegName( back, FirstReg( back, RTInfo[rtindex].left ) );
    new_ins = MakeConvert( back, value, reg_name, type_class, value->n.type_class );
    AddIns( back, new_ins );

    reg_name = AllocRegName( back, HW_ECX );
    new_ins = MakeMove( back, AllocIntConst( back, table->size + 1 ), reg_name, U4 );
    AddIns( back, new_ins );

    reg_name = AllocRegName( back, HW_EDI );
    label = AllocMemory( back, table, 0, CG_VTB, U4 );
    label = AddrConst( back, label, AskBackSeg(back), CONS_OFFSET );
    new_ins = MakeMove( back, label, reg_name, U4 );
    AddIns( back, new_ins );

    new_ins = NewIns( back, 3 );
    new_ins->head.opcode = OP_CALL;
    new_ins->type_class = U2;
    tmp = FirstReg( back, RTInfo[rtindex].left );
    HW_CTurnOn( tmp, HW_ECX );
    HW_CTurnOn( tmp, HW_EDI );
    HW_CTurnOn( tmp, HW_ES );
    new_ins->operands[CALL_OP_USED] = AllocRegName( back, tmp );
    new_ins->operands[CALL_OP_USED2] = new_ins->operands[CALL_OP_USED];
    new_ins->operands[CALL_OP_ADDR] = AllocMemory( back, RTLabel( back, rtindex ), 0, CG_LBL, U4 );
    new_ins->result = NULL;
    new_ins->num_operands = 2;
    AddIns( back, new_ins );

    result = AllocMemory( back, table, 0, CG_TBL, U4 ); /* so table gets freed!*/
    if( type_class == U4 ) {
        HW_CAsgn( tmp, HW_ECX );
        HW_CTurnOn( tmp, HW_EDI );
        new_ins->zap = &AllocRegName( back, tmp )->r;
        new_ins->result = AllocRegName( back, HW_EDI );
        HW_CAsgn( tmp, HW_CS );
        HW_CTurnOn( tmp, HW_EDI );
        result = AllocRegName( back, tmp );
        result = AllocIndex( back, result, NULL, ( table->size - 1 ) * 4, U4 );
        new_ins = MakeMove( back, result, AllocTemp( back, WD ), WD );
        AddIns( back, new_ins );
        result = new_ins->result;
    } else {
        HW_CAsgn( tmp, HW_ECX );
        HW_CTurnOn( tmp, HW_EDI );
        new_ins->zap = &AllocRegName( back, tmp )->r;
        new_ins->result = AllocRegName( back, HW_ECX );
        new_ins = MakeMove( back, new_ins->result, AllocTemp( back, WD ), WD );
        AddIns( back, new_ins );
        result = AllocIndex( back, new_ins->result, result, 0, U4 );
        result->i.scale = SCALE_4; /* 2**2 == 4 */
    }
    return( result );
}


name    *Addressable( BackRun& back, name *cons, type_class_def type_class )
/************************************************************
 * make sure a floating point constant is addressable (dropped
 * it into memory if it isnt)
 */
{
    if( cons->n.class_ == N_CONSTANT )
        return( GenFloat( back, cons, type_class ) );
    return( cons );
}


pointer BEAuxInfo( BackRun& back, pointer hdl, aux_class request )
/**************************************************
 * see ScanCall for explanation
 */
{
    switch( request ) {
    case FEINF_AUX_LOOKUP:
        switch( FindRTLabel( back, static_cast<label_handle>(hdl) ) ) {
        case RT_SCAN1:
            if( IsntTargetModel( back, CGSW_X86_FLAT_MODEL ) )
                return( const_cast<pointer>(static_cast<const void *>(&back.x386rtrtn->Scn1ES)) );
            return( const_cast<pointer>(static_cast<const void *>(&back.x386rtrtn->Scn1)) );
        case RT_SCAN2:
            if( IsntTargetModel( back, CGSW_X86_USE_32 ) ) {
                if( IsntTargetModel( back, CGSW_X86_FLAT_MODEL ) )
                    return( const_cast<pointer>(static_cast<const void *>(&back.x386rtrtn->Scn4ES)) );
                return( const_cast<pointer>(static_cast<const void *>(&back.x386rtrtn->Scn4)) );
            } else {
                if( IsntTargetModel( back, CGSW_X86_FLAT_MODEL ) )
                    return( const_cast<pointer>(static_cast<const void *>(&back.x386rtrtn->Scn2ES)) );
                return( const_cast<pointer>(static_cast<const void *>(&back.x386rtrtn->Scn2)) );
            }
        case RT_SCAN4:
            if( IsntTargetModel( back, CGSW_X86_USE_32 ) ) {
                if( IsntTargetModel( back, CGSW_X86_FLAT_MODEL ) )
                    return( const_cast<pointer>(static_cast<const void *>(&back.x386rtrtn->Scn2ES)) );
                return( const_cast<pointer>(static_cast<const void *>(&back.x386rtrtn->Scn2)) );
            } else {
                if( IsntTargetModel( back, CGSW_X86_FLAT_MODEL ) )
                    return( const_cast<pointer>(static_cast<const void *>(&back.x386rtrtn->Scn4ES)) );
                return( const_cast<pointer>(static_cast<const void *>(&back.x386rtrtn->Scn4)) );
            }
        default:
            return( NULL );
        }
    case FEINF_CALL_CLASS:
        return( reinterpret_cast<pointer>(FECALL_GEN_NONE) );
    case FEINF_CALL_CLASS_TARGET:
        return( reinterpret_cast<pointer>(FECALL_X86_NONE) );
    case FEINF_CALL_BYTES:
        return( hdl );
    default:
        Zoiks( back, ZOIKS_128 );
        return( NULL );
    }
}

instruction     *rMAKEFNEG( BackRun& back, instruction *ins )
/*********************************************
 * this is intentionally a stub for the 386.
 */
{
    return( ins );
}

/* the run makes and frees its copy of this file's variables (run/ files: see the run header) */
X386rtrtnState* x386rtrtn_make() { return new X386rtrtnState(); }
void x386rtrtn_free(X386rtrtnState* part) { delete part; }
