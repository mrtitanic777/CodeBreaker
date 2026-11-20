/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2021 The Open Watcom Contributors. All Rights Reserved.
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


#include <assert.h>

#include "omfflhn.h"
#include "omforl.h"
#include "orlhash.h"


static orl_return       freeFileHandle( omf_file_handle ofh )
{
    omf_sec_handle      sh;
    omf_quantity        x;
    omf_tmp_fixup       tf;

    while( (sh = ofh->first_sec) != nullptr ) {
        switch( sh->type ) {
        case ORL_SEC_TYPE_NOTE:
            if( sh->assoc.comment.comments != nullptr ) {
                for( x = 0; x < sh->assoc.comment.num; x++ ) {
                    _ClientFree( ofh, sh->assoc.comment.comments[x] );
                }
                _ClientFree( ofh, sh->assoc.comment.comments );
                sh->assoc.comment.comments = nullptr;
                sh->assoc.comment.num = 0;
            }
            break;
        case ORL_SEC_TYPE_RELOCS:
            if( sh->assoc.reloc.relocs != nullptr ) {
                for( x = 0; x < sh->assoc.reloc.num; x++ ) {
                    _ClientFree( ofh, const_cast<orl_reloc_struct *>( sh->assoc.reloc.relocs[x] ) );
                }
                _ClientFree( ofh, sh->assoc.reloc.relocs );
                sh->assoc.reloc.relocs = nullptr;
                sh->assoc.reloc.num = 0;
            }
            break;
        case ORL_SEC_TYPE_SYM_TABLE:
            if( sh->assoc.sym.syms != nullptr ) {
                for( x = 0; x < sh->assoc.sym.num; x++ ) {
                    _ClientFree( ofh, sh->assoc.sym.syms[x] );
                }
                _ClientFree( ofh, sh->assoc.sym.syms );
                sh->assoc.sym.syms = nullptr;
                sh->assoc.sym.num = 0;
            }
            if( sh->assoc.sym.hash_tab != nullptr ) {
                ORLHashTableFree( sh->assoc.sym.hash_tab );
            }
            break;
        case ORL_SEC_TYPE_STR_TABLE:
            if( sh->assoc.string.strings != nullptr ) {
                for( x = 0; x < sh->assoc.string.num; x++ ) {
                    _ClientFree( ofh, sh->assoc.string.strings[x] );
                }
                _ClientFree( ofh, sh->assoc.string.strings );
                sh->assoc.string.strings = nullptr;
                sh->assoc.string.num = 0;
            }
            break;
        case ORL_SEC_TYPE_NO_BITS:
        case ORL_SEC_TYPE_PROG_BITS:
            if( sh->contents != nullptr ) {
                _ClientFree( ofh, sh->contents );
                sh->contents = nullptr;
            }
            if( sh->assoc.seg.lines != nullptr && ( sh->assoc.seg.num_lines > 0 ) ) {
                _ClientFree( ofh, const_cast<orl_linnum_struct *>( sh->assoc.seg.lines ) );
                sh->assoc.seg.lines = nullptr;
                sh->assoc.seg.num_lines = 0;
            }
            break;
        default:
            assert( 0 );
        }
        ofh->first_sec = sh->next;
        _ClientFree( ofh, sh );
    }

    if( ofh->segs != nullptr ) {
        _ClientFree( ofh, ofh->segs );
        ofh->segs = nullptr;
        ofh->num_segs = 0;
    }

    if( ofh->comdats != nullptr ) {
        _ClientFree( ofh, ofh->comdats );
        ofh->comdats = nullptr;
        ofh->num_comdats = 0;
    }

    if( ofh->groups != nullptr ) {
        for( x = 0; x < ofh->num_groups; x++ ) {
            assert( ofh->groups[x] );
            _ClientFree( ofh, ofh->groups[x]->segs );
            _ClientFree( ofh, ofh->groups[x] );
        }
        _ClientFree( ofh, ofh->groups );
        ofh->groups = nullptr;
        ofh->num_groups = 0;
    }

    if( ofh->lidata != nullptr ) {
        while( ofh->lidata->first_fixup != nullptr ) {
            tf = ofh->lidata->first_fixup;
            ofh->lidata->first_fixup = tf->next;
            _ClientFree( ofh, tf );
        }
        _ClientFree( ofh, ofh->lidata );
        ofh->lidata = nullptr;
    }

    _ClientFree( ofh, ofh );

    return( ORL_OKAY );
}

void OmfAddFileLinks( omf_handle oh, omf_file_handle ofh )
{
    assert( oh );
    assert( ofh );

    ofh->next = oh->first_file_hnd;
    ofh->omf_hnd = oh;
    oh->first_file_hnd = ofh;
}

orl_return OmfRemoveFileLinks( omf_file_handle ofh )
{
    omf_handle                          oh;
    omf_file_handle                     curr;

    assert( ofh );
    oh = ofh->omf_hnd;
    assert( oh );

    if( oh->first_file_hnd == ofh ) {
        oh->first_file_hnd = ofh->next;
    } else {
        for( curr = oh->first_file_hnd; curr->next != nullptr; curr = curr->next ) {
            if( curr->next == ofh ) {
                curr->next = ofh->next;
                break;
            }
        }
    }
    return( freeFileHandle( ofh ) );
}
