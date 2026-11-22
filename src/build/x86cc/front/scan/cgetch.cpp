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
* Description:  Get a character from source stream.
*
****************************************************************************/


#include "cvars.h"
#include <errno.h>
#include "wio.h"
#include "scan.h"
#include "frontrun.hpp"


#define MAX_INC_DEPTH   255

/*
 * the following table is used to map three character sequences
 * beginning with ?? followed by 'tri' into the single character 'new'
 */



static const unsigned char  notFilled[] = { '\n', '\0' };


namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct CgetchState: cgetch_state.h) */

}  // namespace



void InitIncFile( FrontRun& front )
{
    front.cgetch.IncFileDepth = MAX_INC_DEPTH;
}

static bool ReadBuffer( FrontRun& front, FCB *fcb )
{
    size_t      read_amount;

    if( fcb->fp == NULL ) {
        /*
         * null device or in-memory buffer
         */
        CloseSrcFile( front, fcb );
        return( true );
    }
    if( fcb->src_end == notFilled + 1 ) {
        /*
         * no data read yet
         */
        if( fcb->typ == FT_HEADER_FORCED ) {
            front.cvars.InitialMacroFlags = MFLAG_NONE;
            if( front.cvars.PCH_FileName != NULL && front.cvars.CompFlags.make_precompiled_header == 0 ) {
                if( front.cvars.CompFlags.ok_to_use_precompiled_hdr ) {
                    front.cvars.CompFlags.use_precompiled_header = true;
                }
            }
            if( front.cvars.CompFlags.use_precompiled_header ) {
                InitBuildPreCompiledHeader(front);
            }
            if( front.cvars.CompFlags.use_precompiled_header ) {
                front.cvars.CompFlags.use_precompiled_header = false;
                if( UsePreCompiledHeader( front, fcb->src_name ) ) {
                    return( true );
                }
            }
        }
    }
    /*
     * if end of file was read already then close file and no other reading from file
     */
    if( fcb->eof ) {
        CloseSrcFile( front, fcb );
        return( true );
    }
    /* copy last byte to begining of next data */
    fcb->src_buf[0] = fcb->src_end[-1];
    fcb->src_end = fcb->src_ptr = fcb->src_buf + 1;
    read_amount = fread( fcb->src_buf + 1, 1, SRC_BUF_SIZE, fcb->fp );
    fcb->src_end += read_amount;            // mark end of buffer
    fcb->src_buf[1 + read_amount] = '\0';
    if( fcb->src_end[-1] == DOS_EOF_CHAR ) {
        read_amount--;
        fcb->src_end--;
        fcb->src_buf[1 + read_amount] = '\0';
    } else if( read_amount == SRC_BUF_SIZE ) {
        /*
         * full buffer was read, no EOL check
         *
         * returned false indicates CurrChar is invalid and requires
         * read new character
         */
        return( false );
    }
    fcb->eof = true;
    /*
     * check for file read error
     */
    if( ferror( fcb->fp ) ) {
        CErr3p( front, ERR_IO_ERR, fcb->src_name, strerror( errno ) );
        CloseSrcFile( front, fcb );
        return( true );
    }
    /*
     * ANSI/ISO C says a non-empty source file must be terminated
     * with a newline. If it's not, we insert one, otherwise
     * whatever comes next will be tacked onto that unterminated
     * line, possibly confusing the hell out of the user.
     */
    if( fcb->src_end[-1] != '\n' ) {
        /*
         * set no_eol flag to emit warning later so line # is right
         */
        fcb->no_eol = true;
        /*
         * append missing newline character and correct end of buffer pointer
         */
        read_amount += 1;
        fcb->src_buf[read_amount++] = '\n';
        fcb->src_buf[read_amount] = '\0';
        fcb->src_end++;
        /*
         * returned false indicates CurrChar is invalid and
         * requires read new character
         */
        return( false );
    }
    if( read_amount == 0 ) {
        CloseSrcFile( front, fcb );
        return( true );
    }
    /*
     * returned false indicates CurrChar is invalid and
     * requires read new character
     */
    return( false );
}


int GetNextChar( FrontRun& front )
{
    int c;

    c = *front.cvars.SrcFiles->src_ptr++;
    if(( front.scan.CharSet[c] & C_EX ) == 0 ) {
//        SrcFiles->column++;
        front.cvars.CurrChar = c;
        return( c );
    }
    return( front.cvars.GetCharCheck( front, c ) );
}

static int getCharAfterOneQuestion( FrontRun& front )
{
    int c;

    /*
     * column for 'LastChar' has not been set yet
     */
    front.cvars.NextChar = GetNextChar;
    c = front.cgetch.LastChar;
    if( c == '?' ) {
//        SrcFiles->column++;
        front.cvars.CurrChar = c;
        return( c );
    }
    return( front.cvars.GetCharCheck( front, c ) );
}

static int getCharAfterTwoQuestion( FrontRun& front )
{
//    SrcFiles->column++;
    front.cvars.CurrChar = '?';
    front.cvars.NextChar = getCharAfterOneQuestion;
    return( front.cvars.CurrChar );
}

int getCharAfterBackSlash( FrontRun& front )
{
    if( front.cgetch.Blank1Count != 0 ) {
        --front.cgetch.Blank1Count;
        return( front.cvars.CurrChar = ' ' );
    }
    if( front.cgetch.Tab1Count != 0 ) {
        --front.cgetch.Tab1Count;
        return( front.cvars.CurrChar = '\t' );
    }
    if( front.cgetch.Blank2Count != 0 ) {
        --front.cgetch.Blank2Count;
        return( front.cvars.CurrChar = ' ' );
    }
    front.cvars.NextChar = GetNextChar;
    return( front.cvars.GetCharCheck( front, front.cgetch.LastChar ) );
}

static int getTestCharFromFile( FrontRun& front )
{
    int c;

    for( ; (c = *front.cvars.SrcFiles->src_ptr++) == '\0'; ) {
        /*
         * check to make sure the NUL character we just found is at the
         * end of the buffer, and not an embedded NUL character in the
         * source file.
         */
        if( front.cvars.SrcFiles->src_ptr != front.cvars.SrcFiles->src_end + 1 )
            break;
        if( ReadBuffer( front, front.cvars.SrcFiles ) ) {
            return( front.cvars.CurrChar );
        }
    }
    return( c );
}

static int getSecondMultiByte( FrontRun& front )
{
    int c;

    c = getTestCharFromFile(front);
    /*
     * should we do this for a multi-byte char?
     */
    front.cvars.NextChar = GetNextChar;
    front.cvars.CurrChar = c;
    return( c );
}

static char translateTriGraph( FrontRun& front, char c )
{
    struct tri_graph *p;

    for( p = front.cgetch.TriGraphs; p->tri != '\0'; ++p ) {
        if( c == p->tri ) {
            if( front.cvars.CompFlags.extensions_enabled ) {
                if( front.cvars.SkipLevel == front.cvars.NestLevel ) {
                    front.cvars.CompFlags.trigraph_alert = true;
                }
            }
            return( p->new_ );
        }
    }
    return( c );
}

static int tryBackSlashNewLine( FrontRun& front )
{
    int nc;

    /*
     * CurrChar is '\\' and SrcFiles->column is up to date
     */
    front.cgetch.Blank1Count = 0;
    front.cgetch.Blank2Count = 0;
    front.cgetch.Tab1Count = 0;
    nc = getTestCharFromFile(front);
    if( front.cvars.CompFlags.extensions_enabled ) {
        while( nc == ' ' ) {
            ++front.cgetch.Blank1Count;
            nc = getTestCharFromFile(front);
        }
        while( nc == '\t' ) {
            ++front.cgetch.Tab1Count;
            nc = getTestCharFromFile(front);
        }
        while( nc == ' ' ) {
            ++front.cgetch.Blank2Count;
            nc = getTestCharFromFile(front);
        }
    }
    if( nc == '\r' ) {
        nc = getTestCharFromFile(front);
    }
    if( nc == '\n' ) {
        if( front.cvars.CompFlags.scanning_cpp_comment && front.cvars.SkipLevel == front.cvars.NestLevel ) {
            CWarn1( front, ERR_SPLICE_IN_CPP_COMMENT );
        }
        if( front.cvars.CompFlags.cpp_mode ) {
            if( front.cvars.CompFlags.in_pragma ) {
                CppPrtChar( front, '\\' );
                CppPrtChar( front, '\n' );
            } else if( front.cvars.CompFlags.cpp_line_wanted ) {
                CppPrtChar( front, '\n' );
            }
        }
        NewLineStartPos( front.cvars.SrcFiles );
        front.cvars.SrcFileLoc = front.cvars.SrcFiles->src_loc;
//        SrcFiles->column = 0;
        return( GetNextChar(front) );
    }
    front.cgetch.LastChar = nc;
    front.cvars.NextChar = getCharAfterBackSlash;
    return( '\\' );
}

static int tryTrigraphAgain( FrontRun& front )
{
    int c;
    int xc;

//    SrcFiles->column++;
    c = getTestCharFromFile(front);
    if( c != '?' ) {
        xc = translateTriGraph( front, c );
        if( c != xc ) {
//            SrcFiles->column += 2;
            front.cvars.CurrChar = xc;
            front.cvars.NextChar = GetNextChar;
            if( xc == '\\' ) {
                return( tryBackSlashNewLine(front) );
            }
            return( xc );
        }
        front.cgetch.LastChar = c;
        front.cvars.CurrChar = '?';
        front.cvars.NextChar = getCharAfterTwoQuestion;
        return( '?' );
    }
    front.cvars.CurrChar = c;
    /*
     * leave NextChar set here because it could still be a trigraph
     */
    return( c );
}

int GetCharCheckFile( FrontRun& front, int c )
{
    int xc;
    int nc;
    int nnc;

    /*
     * column has not being changed for 'c' yet
     */
    if( c != '?' ) {
        switch( c ) {
        case '\0':
            /*
             * check to make sure the NUL character we just found is at the
             * end of the buffer, and not an embedded NUL character in the
             * source file.
             */
            front.cvars.CurrChar = '\0';
            if( front.cvars.SrcFiles->no_eol ) {
                front.cvars.CurrChar = '\n';
            }
            if( front.cvars.SrcFiles->src_ptr == front.cvars.SrcFiles->src_end + 1 ) {
                if( !ReadBuffer( front, front.cvars.SrcFiles ) ) {
                    return( GetNextChar(front) );
                }
            }
            return( front.cvars.CurrChar );
        case '\n':
//            NewLineStartPos( SrcFiles );
//            SrcFiles->column = 0;
            break;
        case '\t':
//            SrcFiles->column +=
//              ( ( TAB_WIDTH + 1 ) - (( SrcFiles->column + 1 ) % TAB_WIDTH ) );
            break;
        case '\\':
//            SrcFiles->column++;
            front.cvars.CurrChar = c;
            return( tryBackSlashNewLine(front) );
        case '\r':
            break;
        default:
//            SrcFiles->column++;
            if( c > 0x7f && (front.scan.CharSet[c] & C_DB) ) {
                /*
                 * we should not process the second byte through
                 * normal channels since its value is meaningless
                 * out of context
                 */
                front.cvars.NextChar = getSecondMultiByte;
            }
            break;
        }
        front.cvars.CurrChar = c;
        return( c );
    }
    /*
     * we have one '?'
     */
//    SrcFiles->column++;
    nc = getTestCharFromFile(front);
    if( nc != '?' ) {
        front.cgetch.LastChar = nc;
        front.cvars.CurrChar = c;
        front.cvars.NextChar = getCharAfterOneQuestion;
        return( c );
    }
    /*
     * we have two '?'s
     */
    nnc = getTestCharFromFile(front);
    xc = translateTriGraph( front, nnc );
    if( nnc != xc ) {
//        SrcFiles->column += 2;
        front.cvars.CurrChar = xc;
        if( xc == '\\' ) {
            return( tryBackSlashNewLine(front) );
        }
        return( xc );
    }
    front.cgetch.LastChar = nnc;
    front.cvars.CurrChar = c;
    if( nnc == '?' ) {
        /*
         * the next char after this may be a valid trigraph!
         */
        front.cvars.NextChar = tryTrigraphAgain;
        return( c );
    }
    front.cvars.NextChar = getCharAfterTwoQuestion;
    return( c );
}

static int regetUngotCharAfterOneQuestion( FrontRun& front )
{
    int c;

    /*
     * column for 'LastChar' has been set
     */
    front.cvars.NextChar = GetNextChar;
    c = front.cgetch.LastChar;
    front.cvars.CurrChar = c;
    return( c );
}

static int regetUngotCharAfterTwoQuestion( FrontRun& front )
{
    int c;

    front.cvars.NextChar = regetUngotCharAfterOneQuestion;
    c = '?';
    front.cvars.CurrChar = c;
    return( c );
}

static int restartDetectOneQuestion( FrontRun& front )
{
    int c;

    front.cvars.NextChar = regetUngotCharAfterOneQuestion;
    c = '?';
    front.cvars.CurrChar = c;
    return( c );
}

static int restartDetectTwoQuestion( FrontRun& front )
{
    int c;

    front.cvars.NextChar = regetUngotCharAfterTwoQuestion;
    c = '?';
    front.cvars.CurrChar = c;
    return( c );
}

static int restartBackSlashWhiteSpace( FrontRun& front )
{
    int c;

    front.cvars.NextChar = getCharAfterBackSlash;
    c = '\\';
    front.cvars.CurrChar = c;
    return( c );
}

void GetNextCharUndo( FrontRun& front, int c )
{
    if( front.cvars.NextChar == GetNextChar ) {
        /*
         * will return 'LastChar' and reset back to 'GetNextChar'
         */
        front.cvars.NextChar = regetUngotCharAfterOneQuestion;
        front.cgetch.LastChar = c;
        return;
    }
    if( front.cvars.NextChar == getCharAfterOneQuestion ) {
        /*
         * LastChar already has character saved in it
         * c should be '?'
         */
        front.cvars.NextChar = restartDetectOneQuestion;
        return;
    }
    if( front.cvars.NextChar == getCharAfterTwoQuestion ) {
        /*
         * LastChar already has character saved in it
         * c should be '?'
         */
        front.cvars.NextChar = restartDetectTwoQuestion;
        return;
    }
    if( front.cvars.NextChar == getCharAfterBackSlash ) {
        /*
         * LastChar already has character saved in it
         * c should be '\\'
         */
        front.cvars.NextChar = restartBackSlashWhiteSpace;
        return;
    }
#ifdef FDEBUG
    CFatal( "more than one GetNextCharUndo executed" );
#endif
}

static bool FCB_Alloc( FrontRun& front, FILE *fp, const char *filename, src_file_type typ )
{
    FCB             *fcb;
    unsigned char   *src_buffer;
    FNAMEPTR        flist;

    --front.cgetch.IncFileDepth;
    fcb = (FCB *)CMemAlloc( front, sizeof( FCB ) );
    src_buffer = static_cast<unsigned char *>(FEmalloc( front, SRC_BUF_SIZE + 3 ));
    if( fcb != NULL ) {
        src_buffer[0] = '\n';
        src_buffer[1] = '\0';
        fcb->src_buf = src_buffer;
        fcb->src_ptr = fcb->src_end = src_buffer + 1;
        flist = AddFlist( front, filename );
        FNameFullPath( front, flist );
        fcb->src_name = flist->name;
        fcb->src_line_cnt = 0;
        fcb->src_loc.line = 0;
        fcb->src_loc.column = 0;
        fcb->src_loc.fno = flist->index;
        front.cvars.SrcFileLoc = fcb->src_loc;
        fcb->src_flist = flist;
        fcb->fp = fp;
        fcb->prev_file = front.cvars.SrcFiles;
        fcb->prev_currchar = front.cvars.CurrChar;
#if _CPU == 370
        fcb->column = 0;     /* init colum, trunc info */
        fcb->trunc = 0;
        fcb->prevcount = 0;
#endif
        if( front.cvars.SrcFiles != NULL ) {
            if( front.cvars.SrcFiles == front.cvars.MainSrcFile ) {
                /*
                 * remember name of included file
                 */
                AddIncFileList( front, filename );
            }
        }
        fcb->rseekpos = 0;
        fcb->typ = typ;
        fcb->no_eol = false;
        fcb->eof = false;
        front.cvars.SrcFiles = fcb;
        front.cvars.CurrChar = '\n';    /* set next character to newline */
        return( true );
    }
    return( false );
}

static void FCB_Free( FrontRun& front, FCB *fcb )
{
    ++front.cgetch.IncFileDepth;
    FEfree( front, fcb->src_buf );
    CMemFree( front, fcb );
}

bool OpenFCB( FrontRun& front, FILE *fp, const char *filename, src_file_type typ )
{
    if( front.cgetch.IncFileDepth == 0 ) {
        CErr2( front, ERR_INCDEPTH, MAX_INC_DEPTH );
        CSuicide(front);
        return( false );
    }
    if( front.cvars.CompFlags.track_includes ) {
        /*
         * Don't track the top level file (any semi-intelligent user should
         * have no trouble tracking *that* down)
         */
        if( front.cgetch.IncFileDepth < MAX_INC_DEPTH ) {
            CInfoMsg( front, INFO_INCLUDING_FILE, filename );
        }
    }

    if( FCB_Alloc( front, fp, filename, typ ) ) {
        return( true );
    }
    CErr1( front, ERR_OUT_OF_MEMORY );
    return( false );
}

void CloseFCB( FrontRun& front, FCB *fcb )
{
    if( front.cvars.CompFlags.scanning_comment ) {
        CErr2( front, ERR_INCOMPLETE_COMMENT, front.cvars.CommentLoc.line );
    }
    if( fcb->no_eol ) {
        source_loc  err_loc;

        err_loc.line = fcb->src_line_cnt - 1;
        err_loc.column = fcb->src_loc.column;
        err_loc.fno = fcb->src_flist->index;
        SetErrLoc( front, &err_loc );
        CWarn1( front, ERR_NO_EOL_BEFORE_EOF );
        InitErrLoc(front);
    }
    front.cvars.SrcFiles = fcb->prev_file;
    front.cvars.CurrChar = fcb->prev_currchar;
    if( front.cvars.SrcFiles != NULL ) {
        if( front.cvars.SrcFiles->fp == NULL ) {
            /*
             * physical file name must be used, not logical
             */
            front.cvars.SrcFiles->fp = front.held->fopen( front.cvars.SrcFiles->src_flist->name, "rb" );
            fseek( front.cvars.SrcFiles->fp, front.cvars.SrcFiles->rseekpos, SEEK_SET );
        }
        front.cvars.SrcFileLoc = front.cvars.SrcFiles->src_loc;
        front.cvars.IncLineCount += fcb->src_line_cnt;
        if( front.cvars.SrcFiles == front.cvars.MainSrcFile ) {
            if( front.cvars.CompFlags.make_precompiled_header ) {
                front.cvars.CompFlags.make_precompiled_header = false;
                if( front.cvars.ErrCount == 0 ) {
                    BuildPreCompiledHeader( front, front.cvars.PCH_FileName );
                }
            }
        }
        if( front.cvars.CompFlags.cpp_mode ) {
            CppEmitPoundLine( front, front.cvars.SrcFiles->src_loc.line, front.cvars.SrcFiles->src_name, true );
        }
    } else {
        front.cvars.SrcLineCount = fcb->src_line_cnt;
        front.cvars.CurrChar = LCHR_EOF;
    }
    FCB_Free( front, fcb );
}

void SrcPurge( FrontRun& front )
/*******************/
{
    FCB *fcb;

    while( (fcb = front.cvars.SrcFiles) != NULL ) {
        front.cvars.SrcFiles = fcb->prev_file;
        if( fcb->fp != NULL && fcb->fp != stdin )
            front.held->fclose( fcb->fp );
        FCB_Free( front, fcb );
    }
}
