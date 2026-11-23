/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2023 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  WHEN YOU FIGURE OUT WHAT THIS FILE DOES, PLEASE
*               DESCRIBE IT HERE!
*
****************************************************************************/


#include "_cgstd.h"
#include "coderep.h"
#include "cgauxcc.h"
#include "cgauxinf.h"
#include "zoiks.h"
#include "data.h"
#include "objout.h"
#include "namelist.h"
#include "x86objd.h"
#include "x86obj.h"
#include "split.h"
#include "generate.h"
#include "makeaddr.h"
#include "x86segs.h"
#include "regalloc.h"
#include "feprotos.h"
#include "backrun.hpp"


void    InitSegment( void ) {
/*****************************/
}


void    FiniSegment( void ) {
/*****************************/
}


hw_reg_set      CalcSegment( BackRun& back, cg_sym_handle sym, cg_class class_ ) {
/*********************************************************************/

    fe_attr     attr;
    segment_id  segid;
    hw_reg_set  *reg;

    if( class_ == CG_TBL || class_ == CG_VTB || class_ == CG_CLB ) {
        if( IsTargetModel( back, CGSW_X86_FLAT_MODEL ) )
            return( HW_DS );
        return( HW_CS );
    }
    if( class_ == CG_FE ) {
        attr = FEAttr( *back.front, sym );
    } else {
        attr = static_cast<fe_attr>(EMPTY);
    }
    segid = AskSegID( back, sym, class_ );
    reg = static_cast<hw_reg_set *>(FEAuxInfo( *back.front, (pointer)(pointer_uint)segid, FEINF_PEGGED_REGISTER ));
    if( reg != NULL && !HW_CEqual( *reg, HW_EMPTY ) ) {
        if( HW_COvlap( *reg, HW_SEGS ) )
            return( *reg );
        FEMessage( *back.front, FEMSG_BAD_PEG_REG, AskForLblSym( back, static_cast<label_handle>(back.data.CurrProc->label) ) );
    }
    if( class_ == CG_FE && (attr & FE_PROC) && (attr & FE_DLLIMPORT) == 0 ) {
#if _TARGET & _TARG_80386
        if( IsTargetModel( back, CGSW_X86_FLAT_MODEL ) )
            return( HW_CS ); /* all have same CS */
        if( AskCodeSeg(back) == segid ) {
            return( HW_CS );
        }
#endif
    } else {
        if( IsTargetModel( back, CGSW_X86_FLAT_MODEL )
          && IsntTargetModel( back, CGSW_X86_FLOATING_DS ) )
            return(HW_DS);
        if( AskCodeSeg(back) == segid ) {
            /* COMDAT's might be allocated by some other module */
            if( attr & FE_COMMON )
                return( HW_EMPTY );
            return( HW_CS );
        }
        if( AskSegIsPrivate( back, segid ) )
            return( HW_EMPTY );
        if( IsntTargetModel( back, CGSW_X86_FLOATING_DS ) )
            return( HW_DS );
        if( IsntTargetModel( back, CGSW_X86_FLOATING_SS ) ) {
            return( HW_SS );
        }
    }
    return( HW_EMPTY );
}


name    *AddrConst( BackRun& back, name *value, segment_id segid, constant_type_class const_type )
/*********************************************************************************/
{
    hw_reg_set  reg;

    switch( const_type ) {
    case CONS_SEGMENT:
        HW_CAsgn( reg, HW_EMPTY );
        if( value != NULL ) {
            reg = CalcSegment( back, static_cast<cg_sym_handle>(value->v.symbol), value->m.memory_type );
        }
        if( HW_CEqual( reg, HW_EMPTY ) ) {
            return( AllocAddrConst( back, value, segid, const_type, U2 ) );
        } else {
            return( AllocRegName( back, reg ) );
        }
    case CONS_OFFSET:
        return( AllocAddrConst( back, value, segid, const_type, WD ) );
    case CONS_ADDRESS:
        return( AllocAddrConst( back, value, segid, const_type, CP ) );
    default:
        Zoiks( back, ZOIKS_115 );
        return( NULL );
    }
}

bool    SegIsCS( BackRun& back, name *op ) {
/*************************************/

    hw_reg_set  segreg;

    segreg = CalcSegment( back, static_cast<cg_sym_handle>(op->v.symbol), op->m.memory_type );
    if( HW_CEqual( segreg, HW_CS ) )
        return( true );
    return( false );
}


bool    SegIsSS( BackRun& back, name *op ) {
/*************************************/

    hw_reg_set  segreg;

    segreg = CalcSegment( back, static_cast<cg_sym_handle>(op->v.symbol), op->m.memory_type );
    if( HW_CEqual( segreg, HW_SS ) )
        return( true );
    if( HW_CEqual( segreg, HW_DS )
      && IsntTargetModel( back, CGSW_X86_FLOATING_DS )
      && IsntTargetModel( back, CGSW_X86_FLOATING_SS ) )
        return( true );
    return( false );
}


name    *GetSegment( BackRun& back, name *op ) {
/*****************************************/

    hw_reg_set  reg;
    segment_id  segid;

    reg = CalcSegment( back, static_cast<cg_sym_handle>(op->v.symbol), op->m.memory_type );
    if( !HW_CEqual( reg, HW_EMPTY ) )
        return( AllocRegName( back, reg ) );
    segid = AskSegID( back, op->v.symbol, op->m.memory_type );
    if( AskSegIsNear( back, segid ) )
        return( AddrConst( back, NULL, AskBackSeg(back), CONS_SEGMENT ) );
    return( AddrConst( back, op, segid, CONS_SEGMENT ) );
}

name    *NearSegment( BackRun& back ) {
/******************************/

    if( IsntTargetModel( back, CGSW_X86_FLOATING_DS ) )
        return( AllocRegName( back, HW_DS ) );
    if( IsTargetModel( back, CGSW_X86_FLOATING_SS ) )
        return( AddrConst( back, NULL, AskBackSeg(back), CONS_SEGMENT ) );/*means DGROUP*/
    return( AllocRegName( back, HW_SS ) );
}


name    *SegName( BackRun& back, name *op ) {
/**************************************/

    name        *idx;
    hw_reg_set  tmp;

    if( op->n.class_ == N_REGISTER ) {
        tmp = op->r.reg;
        HW_COnlyOn( tmp, HW_DS_ES_SS_CS );
        return( AllocRegName( back, tmp ) );
    }
    if( op->n.class_ == N_TEMP )
        return( AllocRegName( back, HW_SS ) );
    if( op->n.class_ == N_MEMORY )
        return( GetSegment( back, op ) );
    if( op->n.class_ == N_INDEXED ) {
        idx = op->i.index;
        if( op->i.base != NULL && idx->n.type_class != PT ) {
            // even though it's based - huge arrays cannot assume segment
            // of base is same as segment of the huge pointer - BBB Sept 14, 1995
            return( SegName( back, op->i.base ) );
        }
        if( idx->n.class_ == N_REGISTER ) {
            if( HW_CEqual( idx->r.reg, HW_xSP )
             || HW_CEqual( idx->r.reg, HW_xBP ) ) {
                return( AllocRegName( back, HW_SS ) );
            } else {
                Zoiks( back, ZOIKS_022 );
                return( op );
            }
        } else if( idx->n.type_class == CP || idx->n.type_class == PT ) {
            return( SegmentPart( back, idx ) );
        } else {
            return( NearSegment(back) );
        }
    }
    return( op );
}


cg_type NamePtrType( BackRun& back, name *op ) {
/*************************************/

    cg_sym_handle sym;

    if( op->n.class_ == N_MEMORY ) {
        if( op->m.memory_type == CG_FE || op->m.memory_type == CG_BACK ) {
            sym = static_cast<cg_sym_handle>(op->v.symbol);
            if( AskNameIsCode( back, sym, op->m.memory_type ) ) {
                if( op->m.memory_type == CG_FE ) {
                    if( FEAttr( *back.front, sym ) & FE_PROC ) {
                        if( (call_class_target)(pointer_uint)FindAuxInfoSym( back, sym, FEINF_CALL_CLASS_TARGET ) & FECALL_X86_FAR_CALL )
                            return( TY_LONG_CODE_PTR );
                        return( TY_NEAR_CODE_PTR );
                    } else {
                        return( TY_HUGE_POINTER );
                    }
                } else {
                    return( TY_HUGE_POINTER );
                }
            } else if( AskSegIsPrivate( back, AskSegID( back, sym, op->m.memory_type ) )
              || IsTargetModel( back, CGSW_X86_FLOATING_DS ) ) {
                if( op->m.memory_type == CG_FE &&
                    ( FEAttr( *back.front, sym ) & FE_ONESEG ) ) {
                    return( TY_POINTER );
                } else {
                    return( TY_HUGE_POINTER );
                }
            } else {
                return( TY_POINTER );
            }
        } else if( IsTargetModel( back, CGSW_X86_FLOATING_DS )
          || op->m.memory_type == CG_CLB ) {
            return( TY_HUGE_POINTER );
        } else {
            return( TY_POINTER );
        }
    } else if( op->n.class_ == N_TEMP ) {
        if( IsTargetModel( back, CGSW_X86_FLOATING_SS )
          || IsTargetModel( back, CGSW_X86_FLOATING_DS ) ) {
            // can't have a stack > 64K - BBB 06/02/94
            return( TY_LONG_POINTER );
        } else {
            return( TY_POINTER );
        }
    } else if( IsTargetModel( back, CGSW_X86_FLOATING_DS ) ) {
        return( TY_HUGE_POINTER );
    } else {
        return( TY_POINTER );
    }
}


  bool    SegOver( BackRun& back, name *op ) {
/*************************************/

    segment_id  segid;

    if( op->n.class_ == N_INDEXED ) {
        if( op->i.index->n.class_ != N_CONSTANT
          && op->i.index->n.size != WORD_SIZE )
            return( true );
        if( op->i.base != NULL )
            return( SegOver( back, op->i.base ) );
        if( IsTargetModel( back, CGSW_X86_FLOATING_DS ) ) {
            return( true );
        }
    } else if( op->n.class_ == N_MEMORY ) {
        switch( op->m.memory_type ) {
        case CG_FE:
        case CG_BACK:
        case CG_LBL:
            segid = AskSegID( back, op->v.symbol, op->m.memory_type );
            if( AskSegIsPrivate( back, segid ) ) {
                return( true );
            }
            if( segid == AskCodeSeg(back)
              && IsntTargetModel( back, CGSW_X86_FLAT_MODEL ) ) {
                return( true );
            }
            if( IsTargetModel( back, CGSW_X86_FLOATING_SS )
              && IsTargetModel( back, CGSW_X86_FLOATING_DS ) ) {
                return( true );
            }
            return( false );
        case CG_CLB:
            return( false );
        default:
            break;
        }
    }
    return( false );
}


bool    LoadAToMove( BackRun& back, instruction *ins ) {
/***********************************************/

    segment_id  segid;
    name        *op;

    if( IsTargetModel( back, CGSW_X86_INDEXED_GLOBALS ) )
        return( false );
    if( ins->head.opcode != OP_LA && ins->head.opcode != OP_CAREFUL_LA )
        return( false );
    op = ins->operands[0];
    if( op->n.class_ != N_MEMORY )
        return( false );
    ins->head.opcode = OP_MOV;
    segid = AskSegID( back, op->v.symbol, op->m.memory_type );
    if( AskSegIsNear( back, segid ) ) {
        segid = AskBackSeg(back);
    }
    if( ins->type_class == WD || ins->type_class == SW ) {
        ins->operands[0] = AddrConst( back, op, segid, CONS_OFFSET );
    } else { /* must be a far pointer */
        ins->operands[0] = AddrConst( back, op, segid, CONS_ADDRESS );
    }
    return( true );
}

bool                     IsUncacheableMemory( BackRun& back, name *opnd )
/*
    Return true if we don't want to cache the global piece of memory
    represented by opnd in a register - this could be dangerous if the
    variable is not in DGROUP or if DS is not pegged - we would need to
    load a segment register before and after every call and no one has
    necessarily reserved one.
*/
{
    hw_reg_set          segreg;

    if( opnd->n.class_ != N_MEMORY )
        return( false );
    segreg = CalcSegment( back, static_cast<cg_sym_handle>(opnd->v.symbol), opnd->m.memory_type );
    if( HW_CEqual( segreg, HW_SS )
      && IsntTargetModel( back, CGSW_X86_FLOATING_SS ) )
        return( false );
    if( HW_CEqual( segreg, HW_DS )
      && IsntTargetModel( back, CGSW_X86_FLOATING_DS ) )
        return( false );
    if( HW_CEqual( segreg, HW_CS ) )
        return( false );
    return( true );
}

