/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2023      The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Print module compilation statistics.
*
****************************************************************************/


#include "cvars.h"


void InitStats( void )
{
    cvars_state().CompFlags.stats_printed = false;
    cvars_state().CompFlags.extra_stats_wanted = false;
    cvars_state().SrcLineCount = 0;
    cvars_state().IncLineCount = 0;
    cvars_state().WngCount = 0;
    cvars_state().ErrCount = 0;
    cvars_state().GblSymCount = 0;
    cvars_state().LclSymCount = 0;
    cvars_state().MacroCount = 0;
    cvars_state().MacroSize = 0;
    cvars_state().EnumCount = 0;
    cvars_state().TagCount = 0;
    cvars_state().FieldCount = 0;
    cvars_state().TypeCount = 0;
    cvars_state().TmpSymCount = 0;
    cvars_state().LitPoolSize = 0;
    cvars_state().LitCount = 0;
    /*
     * still useful for internal debugging
     */
    cvars_state().FuncCount = 0;
}

void PrintStats( void )
{
    FCB         *nest_fcb;
    char        *fname;
    char        msgbuf[MAX_MSG_LEN];
    int         len;

    if( !cvars_state().CompFlags.stats_printed ) {
        len = 0;
        fname = cvars_state().WholeFName;
        if( fname == NULL )
            fname = const_cast<char *>("");
        len += sprintf( &msgbuf[len], "%s: ", fname );
        if( cvars_state().SrcLineCount == 0 ) {
            nest_fcb = cvars_state().SrcFiles;
            if( nest_fcb != NULL ) {
                while( nest_fcb->prev_file != NULL ) {
                    cvars_state().IncLineCount += nest_fcb->src_line_cnt;
                    nest_fcb = nest_fcb->prev_file;
                }
                cvars_state().SrcLineCount = nest_fcb->src_line_cnt;
            }
        }
        len += sprintf( &msgbuf[len], "%u lines, ", cvars_state().SrcLineCount );
        if( cvars_state().IncLineCount != 0 ) {
            len += sprintf( &msgbuf[len], "included %u, ", cvars_state().IncLineCount );
        }
        len += sprintf( &msgbuf[len], "%u warnings, ", cvars_state().WngCount );
        len += sprintf( &msgbuf[len], "%u errors", cvars_state().ErrCount );
        BannerMsg( msgbuf );
        cvars_state().CompFlags.stats_printed = true;
    }
}
