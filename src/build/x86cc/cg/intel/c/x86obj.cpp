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

namespace {

/* the statics of this file, one set for each run of the compiler */
struct X86objState {
    const x86_obj_emitter *Emitter = &X86OmfEmitter;
};

}  // namespace

static inline X86objState& x86obj_state() { return cbrun_part<X86objState>(); }


static void selectObjEmitter( void )
{
#if _TARGET & _TARG_80386
    if( _IsModel( CGSW_GEN_OBJ_OWL ) ) {
        x86obj_state().Emitter = &X86OwlEmitter;
    } else
#endif
    {
        x86obj_state().Emitter = &X86OmfEmitter;
    }
}

void InitSegDefs( void ) { selectObjEmitter(); x86obj_state().Emitter->init_seg_defs(); }
bool_ FreeObjCache( void ) { return( x86obj_state().Emitter->free_obj_cache() ); }
segment_id AskOP( void ) { return( x86obj_state().Emitter->ask_op() ); }
segment_id ChangeOP( segment_id s ) { return( x86obj_state().Emitter->change_op( s ) ); }
void SetOP( segment_id s ) { x86obj_state().Emitter->set_op( s ); }
void DefSegment( segment_id s, seg_attr a, const char *n, uint l, bool_ u ) { x86obj_state().Emitter->def_segment( s, a, n, l, u ); }
void DoEmptyQueue( void ) { x86obj_state().Emitter->empty_queue(); }
char GetMemModel( void ) { return( x86obj_state().Emitter->get_mem_model() ); }
segment_id DbgSegDef( const char *n, const char *c, int m ) { return( x86obj_state().Emitter->dbg_seg_def( n, c, m ) ); }
void ObjInit( void ) { x86obj_state().Emitter->obj_init(); }
offset AskLocation( void ) { return( x86obj_state().Emitter->ask_location() ); }
void ChkDbgSegSize( offset m, bool_ t ) { x86obj_state().Emitter->check_dbg_size( m, t ); }
bool_ UseImportForm( fe_attr a ) { return( x86obj_state().Emitter->use_import_form( a ) ); }
bool_ AskSegIsNear( segment_id s ) { return( x86obj_state().Emitter->seg_is_near( s ) ); }
bool_ AskSegIsBlank( segment_id s ) { return( x86obj_state().Emitter->seg_is_blank( s ) ); }
bool_ AskSegIsPrivate( segment_id s ) { return( x86obj_state().Emitter->seg_is_private( s ) ); }
bool_ AskSegIsROM( segment_id s ) { return( x86obj_state().Emitter->seg_is_rom( s ) ); }
segment_id AskBackSeg( void ) { return( x86obj_state().Emitter->ask_back_seg() ); }
segment_id AskCodeSeg( void ) { return( x86obj_state().Emitter->ask_code_seg() ); }
bool_ HaveCodeSeg( void ) { return( x86obj_state().Emitter->have_code_seg() ); }
segment_id AskAltCodeSeg( void ) { return( x86obj_state().Emitter->ask_alt_code_seg() ); }
segment_id AskCode16Seg( void ) { return( x86obj_state().Emitter->ask_code16_seg() ); }
void OutSelect( bool_ s ) { x86obj_state().Emitter->out_select( s ); }
void SetUpObj( bool_ d ) { x86obj_state().Emitter->set_up_obj( d ); }
void FlushOP( segment_id s ) { x86obj_state().Emitter->flush_op( s ); }
void ObjFini( void ) { x86obj_state().Emitter->obj_fini(); }
void OutDLLExport( uint w, cg_sym_handle s ) { x86obj_state().Emitter->out_dll_export( w, s ); }
void OutLabel( label_handle l ) { x86obj_state().Emitter->out_label( l ); }
void AbsPatch( abspatch_handle p, offset l ) { x86obj_state().Emitter->abs_patch( p, l ); }
void *InitPatches( void ) { return( x86obj_state().Emitter->init_patches() ); }
void SetBigLocation( long_offset l ) { x86obj_state().Emitter->set_big_location( l ); }
void IncLocation( offset l ) { x86obj_state().Emitter->inc_location( l ); }
void SetLocation( offset l ) { x86obj_state().Emitter->set_location( l ); }
void OutFPPatch( fp_patches p ) { x86obj_state().Emitter->out_fp_patch( p ); }
void OutPatch( label_handle l, patch_attr a ) { x86obj_state().Emitter->out_patch( l, a ); }
abs_patch *NewAbsPatch( void ) { return( x86obj_state().Emitter->new_abs_patch() ); }
void OutDataByte( byte v ) { x86obj_state().Emitter->out_data_byte( v ); }
void OutDataShort( uint_16 v ) { x86obj_state().Emitter->out_data_short( v ); }
void OutDataLong( uint_32 v ) { x86obj_state().Emitter->out_data_long( v ); }
void OutAbsPatch( abs_patch *p, patch_attr a ) { x86obj_state().Emitter->out_abs_patch( p, a ); }
void OutReloc( segment_id s, fix_class c, bool_ r ) { x86obj_state().Emitter->out_reloc( s, c, r ); }
void OutSpecialCommon( import_handle h, fix_class c, bool_ r ) { x86obj_state().Emitter->out_special_common( h, c, r ); }
void OutImport( cg_sym_handle s, fix_class c, bool_ r ) { x86obj_state().Emitter->out_import( s, c, r ); }
void OutRTImportRel( rt_class i, fix_class c, bool_ r ) { x86obj_state().Emitter->out_rt_import_rel( i, c, r ); }
void OutRTImport( rt_class i, fix_class c ) { x86obj_state().Emitter->out_rt_import( i, c ); }
void OutBckExport( const char *n, bool_ e ) { x86obj_state().Emitter->out_bck_export( n, e ); }
void OutBckImport( const char *n, back_handle b, fix_class c ) { x86obj_state().Emitter->out_bck_import( n, b, c ); }
void OutLineNum( cg_linenum l, bool_ b ) { x86obj_state().Emitter->out_line_num( l, b ); }
unsigned SavePendingLine( unsigned l ) { return( x86obj_state().Emitter->save_pending_line( l ) ); }
void OutDBytes( unsigned l, const byte *s ) { x86obj_state().Emitter->out_bytes( l, s ); }
void OutIBytes( byte p, offset l ) { x86obj_state().Emitter->out_iterated( p, l ); }
bool_ NeedBaseSet( void ) { return( x86obj_state().Emitter->need_base_set() ); }
offset AskMaxSize( void ) { return( x86obj_state().Emitter->ask_max_size() ); }
long_offset AskBigLocation( void ) { return( x86obj_state().Emitter->ask_big_location() ); }
long_offset AskBigMaxSize( void ) { return( x86obj_state().Emitter->ask_big_max_size() ); }
void TellObjNewLabel( cg_sym_handle l ) { x86obj_state().Emitter->tell_new_label( l ); }
void TellObjNewProc( cg_sym_handle p ) { x86obj_state().Emitter->tell_new_proc( p ); }
void TellObjVirtFuncRef( void *c ) { x86obj_state().Emitter->tell_virt_func_ref( c ); }
segment_id AskSegID( pointer h, cg_class c ) { return( x86obj_state().Emitter->ask_seg_id( h, c ) ); }
bool_ AskNameIsCode( pointer h, cg_class c ) { return( x86obj_state().Emitter->name_is_code( h, c ) ); }
bool_ AskNameIsROM( pointer h, cg_class c ) { return( x86obj_state().Emitter->name_is_rom( h, c ) ); }
bool_ SymIsExported( cg_sym_handle s ) { return( x86obj_state().Emitter->sym_is_exported( s ) ); }
