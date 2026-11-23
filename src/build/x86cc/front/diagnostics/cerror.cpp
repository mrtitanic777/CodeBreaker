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
* Description:  Error and warning message output.
*
****************************************************************************/


#include "cvars.h"
#include <stdarg.h>

#include "clibext.h"
#include "cerrs.h"
#include "frontrun.hpp"


/*
 * Types of post-processing messages (informational notes)
 */





namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct CerrorState: cerror_state.h) */

}  // namespace



void OpenErrFile( FrontRun& front )
/**********************/
{
    char        *name;

    if( front.cvars.SrcFName != NULL ) {
        name = ErrFileName(front);
        if( name != NULL ) {
            front.cvars.ErrFile = front.held->fopen( name, "w" );
            if( front.cvars.ErrFile != NULL ) {
                setvbuf( front.cvars.ErrFile, static_cast<char *>(CPermAlloc( front, 32 )), _IOFBF, 32 );
            }
        }
    }
}

static bool okToPrintMsg( FrontRun& front, msg_codes msgnum, int *plevel )
/********************************************************
 * See if OK to print message
 */
{
    bool        ok;
    int         level;
    unsigned    msg_index;

    msg_index = GetMsgIndex( msgnum );
    if( IS_MSGIDX_INVALID( msg_index ) )
        return( false );
    ok = front.cerrs.msg_level[msg_index].enabled;
    level = front.cerrs.msg_level[msg_index].level;
    switch( front.cerrs.msg_level[msg_index].type ) {
    case MSG_TYPE_INFO :
        level = WLEVEL_NOTE;
        break;
    case MSG_TYPE_ANSIERR :
    case MSG_TYPE_ANSIWARN :
        ok = !front.cvars.CompFlags.extensions_enabled;
        break;
    case MSG_TYPE_ANSI :
        if( !front.cvars.CompFlags.extensions_enabled ) {
            level = WLEVEL_ERROR;
        }
        break;
    case MSG_TYPE_WARNING :
    case MSG_TYPE_ERROR :       /* Error messages are used for Warning */
        break;
    }
    *plevel = level;
    if( level == WLEVEL_DISABLED )
        return( false );
    return( ok );
}

static void CMsgInfo( FrontRun& front, cmsg_info *info, int parm_num, msg_codes msgnum, va_list args )
/************************************************************************************
 * fill cmsg_info struct
 */
{
    char        *fname;
    unsigned    line;
    unsigned    column;
    char const  *msgstr;
    int         prefix_len;

    info->msgnum = msgnum;
//    CMsgSetClass( info, msgnum );
    switch( msgnum ) {
    case ERR_INVALID_MEMORY_MODEL:
    case ERR_INVALID_OPTION:
    case ERR_INVALID_OPTIMIZATION:
        /*
         * no location for error message
         */
        line = 0;
        column = 0;
        fname = NULL;
        break;
    default:
        if( front.cerror.error_fname != NULL ) {
            fname = front.cerror.error_fname;
            line = front.cerror.error_line;
            column = front.cerror.error_column;
        } else {
            fname = FileIndexToCorrectName( front, front.cvars.TokenLoc.fno );
            line = front.cvars.TokenLoc.line;
            column = front.cvars.TokenLoc.column;
        }
    }
    prefix_len = 0;
    if( parm_num ) {
        msgstr = CGetMsgStr( front, PHRASE_PARAM );
        prefix_len = sprintf( info->msgtxt, msgstr, parm_num );
        info->msgtxt[prefix_len++] = ' ';
    }
    msgstr = CGetMsgStr( front, msgnum );
    vsnprintf( info->msgtxt + prefix_len, MAX_MSG_LEN - prefix_len, msgstr, args );
    info->msgtxt[MAX_MSG_LEN - 1] = '\0';
    info->line = line;
    info->column = column;
    info->fname = fname;
}

static char const *MsgClassPhrase( FrontRun& front, cmsg_class class_ )
{
    msg_codes       msgnum = PHRASE_ERROR;     // just for init.
    char const      *phrase;

    switch( class_ ) {
    case CMSG_INFO:
        msgnum = PHRASE_NOTE;
        break;
    case CMSG_WARN:
        msgnum = PHRASE_WARNING;
        break;
    case CMSG_ERRO:
        msgnum = PHRASE_ERROR;
        break;
    }
    phrase = CGetMsgStr( front, msgnum );
    return( phrase );
}

void FmtCMsg( FrontRun& front, char *buff, cmsg_info *info )
/******************************************
 * format message with line & file int buff
 */
{
    size_t      len;
    char const  *phrase;
    char const  *code_prefix;

    buff[0] = '\0';
    if( info->line != 0 ) {
        if( info->fname != NULL ) {
            snprintf( buff, MAX_MSG_LEN, "%s(%u): ", info->fname, info->line );
        }
    }
    code_prefix = CGetMsgPrefix( info->msgnum );
    phrase = MsgClassPhrase( front, info->class_ );
    len = strlen( buff );
    snprintf( buff + len, MAX_MSG_LEN - len, "%s %s%03d: ", phrase, code_prefix, info->msgnum );
}

static void OutMsg( FrontRun& front, cmsg_info  *info )
/*************************************
 * print message to streams
 */
{
    char        pre[MAX_MSG_LEN]; //actual message text

    if( front.cvars.ErrFile == NULL )
        OpenErrFile(front);
    if( !front.cvars.CompFlags.eq_switch_used )
        ConsErrMsg( front, info );
    if( front.cvars.ErrFile != NULL ) {
        FmtCMsg( front, pre, info );
        fputs( pre, front.cvars.ErrFile );
        fputs( info->msgtxt, front.cvars.ErrFile );
        fputc( '\n', front.cvars.ErrFile );
        front.cvars.CompFlags.errfile_written = true;
    }
}

static void PrintType( FrontRun& front, msg_codes msgnum, TYPEPTR typ )
{
    char    *text;

    if( typ == NULL )
        return;

    text = DiagGetTypeName( front, typ );
    CInfoMsg( front, msgnum, text );
    CMemFree( front, text );
}

static void PrintPostNotes( FrontRun& front )
{
    while( front.cerror.PostList != NULL ) {
        switch( front.cerror.PostList->type ) {
        case POSTLIST_SYMBOL:
            CInfoMsg( front, INFO_SYMBOL_DECLARATION, front.cerror.PostList->u.s.sym_name, front.cerror.PostList->u.s.sym_file, front.cerror.PostList->u.s.sym_line );
            break;
        case POSTLIST_TWOTYPES_1:
            PrintType( front, INFO_SRC_CNV_TYPE, front.cerror.PostList->u.o.types[0] );
            PrintType( front, INFO_TGT_CNV_TYPE, front.cerror.PostList->u.o.types[1] );
            break;
        case POSTLIST_TWOTYPES_2:
            CInfoMsg( front, INFO_OPERATOR, TokenString[front.cerror.PostList->u.o.opr] );
            PrintType( front, INFO_FIRST_OPND_TYPE, front.cerror.PostList->u.o.types[0] );
            PrintType( front, INFO_SECOND_OPND_TYPE, front.cerror.PostList->u.o.types[1] );
            break;
        }
        SetDiagPop(front);
    }
}

static void CErr( FrontRun& front, int parm_num, msg_codes msgnum, ... )
/******************************************************
 * Output error message
 */
{
    va_list     args1;
    cmsg_info   info;

    info.class_ = CMSG_ERRO;
    if( front.cvars.ErrLimit == ERRLIMIT_NOMAX || front.cvars.ErrCount < front.cvars.ErrLimit ) {
        va_start( args1, msgnum );
        CMsgInfo( front, &info, parm_num, msgnum, args1 );
        va_end( args1 );
        OutMsg( front, &info );
        ++front.cvars.ErrCount;
        PrintPostNotes(front);
    } else {
        va_start( args1, msgnum );
        CMsgInfo( front, &info, 0, ERR_TOO_MANY_ERRORS, args1 );
        va_end( args1 );
        OutMsg( front, &info );
        CSuicide(front);
    }
}

void CErr1( FrontRun& front, msg_codes msgnum )
{
    CErr( front, 0, msgnum );
}

void CErr2( FrontRun& front, msg_codes msgnum, int p1 )
{
    CErr( front, 0, msgnum, p1 );
}

void CErr2p( FrontRun& front, msg_codes msgnum, const char *p1 )
{
    CErr( front, 0, msgnum, p1 );
}

void CErr3p( FrontRun& front, msg_codes msgnum, const char *p1, const char *p2 )
{
    CErr( front, 0, msgnum, p1, p2 );
}

void CErr4p( FrontRun& front, msg_codes msgnum, const char *p1, const char *p2, const char *p3 )
{
    CErr( front, 0, msgnum, p1, p2, p3 );
}

void CErrP1( FrontRun& front, int parm_num, msg_codes msgnum )
{
    CErr( front, parm_num, msgnum );
}


static void CWarn( FrontRun& front, int parm_num, msg_codes msgnum, ... )
/*******************************************************
 * Out warning message
 */
{
    va_list     args1;
    cmsg_info   info;
    int         level;

    if( okToPrintMsg( front, msgnum, &level ) ) {
        if( level <= front.cvars.WngLevel ) {
            info.class_ = CMSG_WARN;
            va_start( args1, msgnum );
            CMsgInfo( front, &info, parm_num, msgnum, args1 );
            OutMsg( front, &info );
            ++front.cvars.WngCount;
            PrintPostNotes(front);
            va_end( args1 );
        }
    }
}

void CWarn1( FrontRun& front, msg_codes msgnum )
{
    CWarn( front, 0, msgnum );
}

void CWarn2( FrontRun& front, msg_codes msgnum, int p1 )
{
    CWarn( front, 0, msgnum, p1 );
}

void CWarn2p( FrontRun& front, msg_codes msgnum, const char *p1 )
{
    CWarn( front, 0, msgnum, p1 );
}

void CWarnP1( FrontRun& front, int parm_num, msg_codes msgnum )
{
    CWarn( front, parm_num, msgnum );
}


void CInfoMsg( FrontRun& front, msg_codes msgnum, ... )
{
    va_list     args1;
    cmsg_info   info;
    int         level;

    if( okToPrintMsg( front, msgnum, &level ) ) {
        if( level <= front.cvars.WngLevel ) {
            info.class_ = CMSG_INFO;
            va_start( args1, msgnum );
            CMsgInfo( front, &info, 0, msgnum, args1 );
            OutMsg( front, &info );
            va_end( args1 );
        }
    }
}


void PCHNote( FrontRun& front, msg_codes msgnum, ... )
/************************************
 * Output pre-compiled header Note
 */
{
    va_list     args1;
    char        msgbuf[MAX_MSG_LEN];
    char const  *msgstr;

    if( !front.cvars.CompFlags.no_pch_warnings ) {
        msgstr = CGetMsgStr( front, msgnum );
        va_start( args1, msgnum );
        vsnprintf( msgbuf, MAX_MSG_LEN, msgstr, args1 );
        va_end( args1 );
        NoteMsg( front, msgbuf );
    }
}


void SetErrLoc( FrontRun& front, source_loc *src_loc )
{
    front.cerror.error_fname = FileIndexToCorrectName( front, src_loc->fno );
    front.cerror.error_line = src_loc->line;
    front.cerror.error_column = src_loc->column;
}


void InitErrLoc( FrontRun& front )
{
    front.cerror.error_fname = NULL;
}


void CSuicide( FrontRun& front )
{
    if( front.cvars.Environment != NULL ) {
        longjmp( *front.cvars.Environment, 1 );
    }
    MyExit(front, 1);
}



static struct ErrPostList *NewPostList( FrontRun& front, postlist_type type )
/***********************************************************
 * post-processing messages (informational notes)
 */
{
    struct ErrPostList  *np;

    np = static_cast<struct ErrPostList *>(CMemAlloc( front, sizeof( *np ) ));
    np->next = front.cerror.PostList;
    np->type = type;
    front.cerror.PostList = np;
    return( np );
}

void SetDiagSymbol( FrontRun& front, SYMPTR sym, SYM_HANDLE handle )
{
    struct ErrPostList  *np;

    np = NewPostList( front, POSTLIST_SYMBOL );
    np->u.s.sym_name = SymName( front, sym, handle );
    if( np->u.s.sym_name == NULL )
        np->u.s.sym_name = "???";
    np->u.s.sym_file = FileIndexToCorrectName( front, sym->src_loc.fno );
    np->u.s.sym_line = sym->src_loc.line;
}

void SetDiagEnum( FrontRun& front, ENUMPTR ep )
{
    struct ErrPostList  *np;

    np = NewPostList( front, POSTLIST_SYMBOL );
    np->u.s.sym_name = ep->name;
    np->u.s.sym_file = FileIndexToCorrectName( front, ep->src_loc.fno );
    np->u.s.sym_line = ep->src_loc.line;
}

void SetDiagMacro( FrontRun& front, MEPTR mp )
{
    struct ErrPostList  *np;

    np = NewPostList( front, POSTLIST_SYMBOL );
    np->u.s.sym_name = mp->macro_name;
    np->u.s.sym_file = FileIndexToCorrectName( front, mp->src_loc.fno );
    np->u.s.sym_line = mp->src_loc.line;
}

void SetDiagType1( FrontRun& front, TYPEPTR typ_source )
{
    SetDiagType2( front, NULL, typ_source );
}

void SetDiagType2( FrontRun& front, TYPEPTR typ_target, TYPEPTR typ_source )
{
    struct ErrPostList  *np;

    np = NewPostList( front, POSTLIST_TWOTYPES_1 );
    np->u.o.opr = static_cast<TOKEN>(0);
    np->u.o.types[0] = typ_source;
    np->u.o.types[1] = typ_target;
}

void SetDiagType3( FrontRun& front, TYPEPTR typ_first, TYPEPTR typ_second, TOKEN opr )
{
    struct ErrPostList  *np;

    np = NewPostList( front, POSTLIST_TWOTYPES_2 );
    np->u.o.opr = opr;
    np->u.o.types[0] = typ_first;
    np->u.o.types[1] = typ_second;
}

void SetDiagPop( FrontRun& front )
{
    struct ErrPostList  *np;

    np = front.cerror.PostList;
    if( np != NULL ) {
        front.cerror.PostList = np->next;
        CMemFree( front, np );
    }
}
