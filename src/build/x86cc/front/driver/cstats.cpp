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
#include "frontrun.hpp"


void InitStats( FrontRun& front )
{
    front.cvars.CompFlags.stats_printed = false;
    front.cvars.CompFlags.extra_stats_wanted = false;
    front.cvars.SrcLineCount = 0;
    front.cvars.IncLineCount = 0;
    front.cvars.WngCount = 0;
    front.cvars.ErrCount = 0;
    front.cvars.GblSymCount = 0;
    front.cvars.LclSymCount = 0;
    front.cvars.MacroCount = 0;
    front.cvars.MacroSize = 0;
    front.cvars.EnumCount = 0;
    front.cvars.TagCount = 0;
    front.cvars.FieldCount = 0;
    front.cvars.TypeCount = 0;
    front.cvars.TmpSymCount = 0;
    front.cvars.LitPoolSize = 0;
    front.cvars.LitCount = 0;
    /*
     * still useful for internal debugging
     */
    front.cvars.FuncCount = 0;
}

void PrintStats( FrontRun& front )
{
    FCB         *nest_fcb;
    char        *fname;
    char        msgbuf[MAX_MSG_LEN];
    int         len;

    if( !front.cvars.CompFlags.stats_printed ) {
        len = 0;
        fname = front.cvars.WholeFName;
        if( fname == NULL )
            fname = const_cast<char *>("");
        len += sprintf( &msgbuf[len], "%s: ", fname );
        if( front.cvars.SrcLineCount == 0 ) {
            nest_fcb = front.cvars.SrcFiles;
            if( nest_fcb != NULL ) {
                while( nest_fcb->prev_file != NULL ) {
                    front.cvars.IncLineCount += nest_fcb->src_line_cnt;
                    nest_fcb = nest_fcb->prev_file;
                }
                front.cvars.SrcLineCount = nest_fcb->src_line_cnt;
            }
        }
        len += sprintf( &msgbuf[len], "%u lines, ", front.cvars.SrcLineCount );
        if( front.cvars.IncLineCount != 0 ) {
            len += sprintf( &msgbuf[len], "included %u, ", front.cvars.IncLineCount );
        }
        len += sprintf( &msgbuf[len], "%u warnings, ", front.cvars.WngCount );
        len += sprintf( &msgbuf[len], "%u errors", front.cvars.ErrCount );
        BannerMsg( front, msgbuf );
        front.cvars.CompFlags.stats_printed = true;
    }
}
