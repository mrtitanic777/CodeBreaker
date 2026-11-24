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

#ifndef X86_EMIT_INCLUDED
#define X86_EMIT_INCLUDED

#include "x86objd.h"
#include "fppatch.h"
#include "rtrtn.h"

/*
 * Format-neutral object writer used by the Intel backends.  The existing
 * implementation is the OMF provider.  The 386 backend can also select the
 * OWL provider for COFF or ELF output.
 */
typedef struct x86_obj_emitter {
    void            (*init_seg_defs)( BackRun& );
    bool            (*free_obj_cache)( BackRun& );
    segment_id      (*ask_op)( BackRun& );
    segment_id      (*change_op)( BackRun&, segment_id );
    void            (*set_op)( BackRun&, segment_id );
    void            (*def_segment)( BackRun&, segment_id, seg_attr, const char *, uint, bool );
    void            (*empty_queue)( BackRun& );
    char            (*get_mem_model)( BackRun& );
    segment_id      (*dbg_seg_def)( BackRun&, const char *, const char *, int );
    void            (*obj_init)( BackRun& );
    offset          (*ask_location)( BackRun& );
    void            (*check_dbg_size)( BackRun&, offset, bool );
    bool            (*use_import_form)( BackRun&, fe_attr );
    bool            (*seg_is_near)( BackRun&, segment_id );
    bool            (*seg_is_blank)( BackRun&, segment_id );
    bool            (*seg_is_private)( BackRun&, segment_id );
    bool            (*seg_is_rom)( BackRun&, segment_id );
    segment_id      (*ask_back_seg)( BackRun& );
    segment_id      (*ask_code_seg)( BackRun& );
    bool            (*have_code_seg)( BackRun& );
    segment_id      (*ask_alt_code_seg)( BackRun& );
    segment_id      (*ask_code16_seg)( BackRun& );
    void            (*out_select)( BackRun&, bool );
    void            (*set_up_obj)( BackRun&, bool );
    void            (*flush_op)( BackRun&, segment_id );
    void            (*obj_fini)( BackRun& );
    void            (*out_dll_export)( BackRun&, uint, cg_sym_handle );
    void            (*out_label)( BackRun&, label_handle );
    void            (*abs_patch)( BackRun&, abspatch_handle, offset );
    void            *(*init_patches)( BackRun& );
    void            (*set_big_location)( BackRun&, long_offset );
    void            (*inc_location)( BackRun&, offset );
    void            (*set_location)( BackRun&, offset );
    void            (*out_fp_patch)( BackRun&, fp_patches );
    void            (*out_patch)( BackRun&, label_handle, patch_attr );
    struct abs_patch *(*new_abs_patch)( BackRun& );
    void            (*out_data_byte)( BackRun&, byte );
    void            (*out_data_short)( BackRun&, uint_16 );
    void            (*out_data_long)( BackRun&, uint_32 );
    void            (*out_abs_patch)( BackRun&, struct abs_patch *, patch_attr );
    void            (*out_reloc)( BackRun&, segment_id, fix_class, bool );
    void            (*out_special_common)( BackRun&, import_handle, fix_class, bool );
    void            (*out_import)( BackRun&, cg_sym_handle, fix_class, bool );
    void            (*out_rt_import_rel)( BackRun&, rt_class, fix_class, bool );
    void            (*out_rt_import)( BackRun&, rt_class, fix_class );
    void            (*out_bck_export)( BackRun&, const char *, bool );
    void            (*out_bck_import)( BackRun&, const char *, back_handle, fix_class );
    void            (*out_line_num)( BackRun&, cg_linenum, bool );
    unsigned        (*save_pending_line)( BackRun&, unsigned );
    void            (*out_bytes)( BackRun&, unsigned, const byte * );
    void            (*out_iterated)( BackRun&, byte, offset );
    bool            (*need_base_set)( BackRun& );
    offset          (*ask_max_size)( BackRun& );
    long_offset     (*ask_big_location)( BackRun& );
    long_offset     (*ask_big_max_size)( BackRun& );
    void            (*tell_new_label)( BackRun&, cg_sym_handle );
    void            (*tell_new_proc)( BackRun&, cg_sym_handle );
    void            (*tell_virt_func_ref)( BackRun&, void * );
    segment_id      (*ask_seg_id)( BackRun&, pointer, cg_class );
    bool            (*name_is_code)( BackRun&, pointer, cg_class );
    bool            (*name_is_rom)( BackRun&, pointer, cg_class );
    bool            (*sym_is_exported)( BackRun&, cg_sym_handle );
} x86_obj_emitter;

extern const x86_obj_emitter X86OmfEmitter;
#if _TARGET & _TARG_80386
extern const x86_obj_emitter X86OwlEmitter;
#endif

#endif
