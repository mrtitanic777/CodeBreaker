/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2026      The Open Watcom Contributors. All Rights Reserved.
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
****************************************************************************/

#include "_cgstd.h"
#include "coderep.h"
#include "model.h"
#include "x86objd.h"
#include "objout.h"
#include "x86obj.h"
#include "x86emit.h"
#include "backrun.hpp"

namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct X86objState: x86obj_state.h) */

}  // namespace



static void selectObjEmitter( BackRun& back )
{
#if _TARGET & _TARG_80386
    if( IsModel( back, CGSW_GEN_OBJ_OWL ) ) {
        back.x86obj.Emitter = &X86OwlEmitter;
    } else
#endif
    {
        back.x86obj.Emitter = &X86OmfEmitter;
    }
}

void InitSegDefs( BackRun& back ) { selectObjEmitter(back); back.x86obj.Emitter->init_seg_defs(back); }
bool FreeObjCache( BackRun& back ) { return( back.x86obj.Emitter->free_obj_cache(back) ); }
segment_id AskOP( BackRun& back ) { return( back.x86obj.Emitter->ask_op(back) ); }
segment_id ChangeOP( BackRun& back, segment_id s ) { return( back.x86obj.Emitter->change_op( back, s ) ); }
void SetOP( BackRun& back, segment_id s ) { back.x86obj.Emitter->set_op( back, s ); }
void DefSegment( BackRun& back, segment_id s, seg_attr a, const char *n, uint l, bool u ) { back.x86obj.Emitter->def_segment( back, s, a, n, l, u ); }
void DoEmptyQueue( BackRun& back ) { back.x86obj.Emitter->empty_queue(back); }
char GetMemModel( BackRun& back ) { return( back.x86obj.Emitter->get_mem_model(back) ); }
segment_id DbgSegDef( BackRun& back, const char *n, const char *c, int m ) { return( back.x86obj.Emitter->dbg_seg_def( back, n, c, m ) ); }
void ObjInit( BackRun& back ) { back.x86obj.Emitter->obj_init(back); }
offset AskLocation( BackRun& back ) { return( back.x86obj.Emitter->ask_location(back) ); }
void ChkDbgSegSize( BackRun& back, offset m, bool t ) { back.x86obj.Emitter->check_dbg_size( back, m, t ); }
bool UseImportForm( BackRun& back, fe_attr a ) { return( back.x86obj.Emitter->use_import_form( back, a ) ); }
bool AskSegIsNear( BackRun& back, segment_id s ) { return( back.x86obj.Emitter->seg_is_near( back, s ) ); }
bool AskSegIsBlank( BackRun& back, segment_id s ) { return( back.x86obj.Emitter->seg_is_blank( back, s ) ); }
bool AskSegIsPrivate( BackRun& back, segment_id s ) { return( back.x86obj.Emitter->seg_is_private( back, s ) ); }
bool AskSegIsROM( BackRun& back, segment_id s ) { return( back.x86obj.Emitter->seg_is_rom( back, s ) ); }
segment_id AskBackSeg( BackRun& back ) { return( back.x86obj.Emitter->ask_back_seg(back) ); }
segment_id AskCodeSeg( BackRun& back ) { return( back.x86obj.Emitter->ask_code_seg(back) ); }
bool HaveCodeSeg( BackRun& back ) { return( back.x86obj.Emitter->have_code_seg(back) ); }
segment_id AskAltCodeSeg( BackRun& back ) { return( back.x86obj.Emitter->ask_alt_code_seg(back) ); }
segment_id AskCode16Seg( BackRun& back ) { return( back.x86obj.Emitter->ask_code16_seg(back) ); }
void OutSelect( BackRun& back, bool s ) { back.x86obj.Emitter->out_select( back, s ); }
void SetUpObj( BackRun& back, bool d ) { back.x86obj.Emitter->set_up_obj( back, d ); }
void FlushOP( BackRun& back, segment_id s ) { back.x86obj.Emitter->flush_op( back, s ); }
void ObjFini( BackRun& back ) { back.x86obj.Emitter->obj_fini(back); }
void OutDLLExport( BackRun& back, uint w, cg_sym_handle s ) { back.x86obj.Emitter->out_dll_export( back, w, s ); }
void OutLabel( BackRun& back, label_handle l ) { back.x86obj.Emitter->out_label( back, l ); }
void AbsPatch( BackRun& back, abspatch_handle p, offset l ) { back.x86obj.Emitter->abs_patch( back, p, l ); }
void *InitPatches( BackRun& back ) { return( back.x86obj.Emitter->init_patches(back) ); }
void SetBigLocation( BackRun& back, long_offset l ) { back.x86obj.Emitter->set_big_location( back, l ); }
void IncLocation( BackRun& back, offset l ) { back.x86obj.Emitter->inc_location( back, l ); }
void SetLocation( BackRun& back, offset l ) { back.x86obj.Emitter->set_location( back, l ); }
void OutFPPatch( BackRun& back, fp_patches p ) { back.x86obj.Emitter->out_fp_patch( back, p ); }
void OutPatch( BackRun& back, label_handle l, patch_attr a ) { back.x86obj.Emitter->out_patch( back, l, a ); }
abs_patch *NewAbsPatch( BackRun& back ) { return( back.x86obj.Emitter->new_abs_patch(back) ); }
void OutDataByte( BackRun& back, byte v ) { back.x86obj.Emitter->out_data_byte( back, v ); }
void OutDataShort( BackRun& back, uint_16 v ) { back.x86obj.Emitter->out_data_short( back, v ); }
void OutDataLong( BackRun& back, uint_32 v ) { back.x86obj.Emitter->out_data_long( back, v ); }
void OutAbsPatch( BackRun& back, abs_patch *p, patch_attr a ) { back.x86obj.Emitter->out_abs_patch( back, p, a ); }
void OutReloc( BackRun& back, segment_id s, fix_class c, bool r ) { back.x86obj.Emitter->out_reloc( back, s, c, r ); }
void OutSpecialCommon( BackRun& back, import_handle h, fix_class c, bool r ) { back.x86obj.Emitter->out_special_common( back, h, c, r ); }
void OutImport( BackRun& back, cg_sym_handle s, fix_class c, bool r ) { back.x86obj.Emitter->out_import( back, s, c, r ); }
void OutRTImportRel( BackRun& back, rt_class i, fix_class c, bool r ) { back.x86obj.Emitter->out_rt_import_rel( back, i, c, r ); }
void OutRTImport( BackRun& back, rt_class i, fix_class c ) { back.x86obj.Emitter->out_rt_import( back, i, c ); }
void OutBckExport( BackRun& back, const char *n, bool e ) { back.x86obj.Emitter->out_bck_export( back, n, e ); }
void OutBckImport( BackRun& back, const char *n, back_handle b, fix_class c ) { back.x86obj.Emitter->out_bck_import( back, n, b, c ); }
void OutLineNum( BackRun& back, cg_linenum l, bool b ) { back.x86obj.Emitter->out_line_num( back, l, b ); }
unsigned SavePendingLine( BackRun& back, unsigned l ) { return( back.x86obj.Emitter->save_pending_line( back, l ) ); }
void OutDBytes( BackRun& back, unsigned l, const byte *s ) { back.x86obj.Emitter->out_bytes( back, l, s ); }
void OutIBytes( BackRun& back, byte p, offset l ) { back.x86obj.Emitter->out_iterated( back, p, l ); }
bool NeedBaseSet( BackRun& back ) { return( back.x86obj.Emitter->need_base_set(back) ); }
offset AskMaxSize( BackRun& back ) { return( back.x86obj.Emitter->ask_max_size(back) ); }
long_offset AskBigLocation( BackRun& back ) { return( back.x86obj.Emitter->ask_big_location(back) ); }
long_offset AskBigMaxSize( BackRun& back ) { return( back.x86obj.Emitter->ask_big_max_size(back) ); }
void TellObjNewLabel( BackRun& back, cg_sym_handle l ) { back.x86obj.Emitter->tell_new_label( back, l ); }
void TellObjNewProc( BackRun& back, cg_sym_handle p ) { back.x86obj.Emitter->tell_new_proc( back, p ); }
void TellObjVirtFuncRef( BackRun& back, void *c ) { back.x86obj.Emitter->tell_virt_func_ref( back, c ); }
segment_id AskSegID( BackRun& back, pointer h, cg_class c ) { return( back.x86obj.Emitter->ask_seg_id( back, h, c ) ); }
bool AskNameIsCode( BackRun& back, pointer h, cg_class c ) { return( back.x86obj.Emitter->name_is_code( back, h, c ) ); }
bool AskNameIsROM( BackRun& back, pointer h, cg_class c ) { return( back.x86obj.Emitter->name_is_rom( back, h, c ) ); }
bool SymIsExported( BackRun& back, cg_sym_handle s ) { return( back.x86obj.Emitter->sym_is_exported( back, s ) ); }
