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


#define MAX_INC_DEPTH   255

/*
 * the following table is used to map three character sequences
 * beginning with ?? followed by 'tri' into the single character 'new'
 */
struct tri_graph {
    char        tri;
    char        new_;
};


static const unsigned char  notFilled[] = { '\n', '\0' };


namespace {

/* the statics of this file, one set for each run of the compiler */
struct CgetchState {
    struct tri_graph TriGraphs[10] = {
        { '=', '#' },
        { '(', '[' },
        { '/', '\\'},
        { ')', ']' },
        { '\'','^' },
        { '<', '{' },
        { '!', '|' },
        { '>', '}' },
        { '-', '~' },
        { '\0','\0'}
    };
    int LastChar;
    int Blank1Count;
    int Blank2Count;
    int Tab1Count;
    int IncFileDepth;
};

}  // namespace

static inline CgetchState& cgetch_state() { return cbrun_part<CgetchState>(); }


void InitIncFile( void )
{
    cgetch_state().IncFileDepth = MAX_INC_DEPTH;
}

static bool_ ReadBuffer( FCB *fcb )
{
    size_t      read_amount;

    if( fcb->fp == NULL ) {
        /*
         * null device or in-memory buffer
         */
        CloseSrcFile( fcb );
        return( true );
    }
    if( fcb->src_end == notFilled + 1 ) {
        /*
         * no data read yet
         */
        if( fcb->typ == FT_HEADER_FORCED ) {
            cvars_state().InitialMacroFlags = MFLAG_NONE;
            if( cvars_state().PCH_FileName != NULL && cvars_state().CompFlags.make_precompiled_header == 0 ) {
                if( cvars_state().CompFlags.ok_to_use_precompiled_hdr ) {
                    cvars_state().CompFlags.use_precompiled_header = true;
                }
            }
            if( cvars_state().CompFlags.use_precompiled_header ) {
                InitBuildPreCompiledHeader();
            }
            if( cvars_state().CompFlags.use_precompiled_header ) {
                cvars_state().CompFlags.use_precompiled_header = false;
                if( UsePreCompiledHeader( fcb->src_name ) ) {
                    return( true );
                }
            }
        }
    }
    /*
     * if end of file was read already then close file and no other reading from file
     */
    if( fcb->eof ) {
        CloseSrcFile( fcb );
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
        CErr3p( ERR_IO_ERR, fcb->src_name, strerror( errno ) );
        CloseSrcFile( fcb );
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
        CloseSrcFile( fcb );
        return( true );
    }
    /*
     * returned false indicates CurrChar is invalid and
     * requires read new character
     */
    return( false );
}


int GetNextChar( void )
{
    int c;

    c = *cvars_state().SrcFiles->src_ptr++;
    if(( scan_state().CharSet[c] & C_EX ) == 0 ) {
//        SrcFiles->column++;
        cvars_state().CurrChar = c;
        return( c );
    }
    return( cvars_state().GetCharCheck( c ) );
}

static int getCharAfterOneQuestion( void )
{
    int c;

    /*
     * column for 'LastChar' has not been set yet
     */
    cvars_state().NextChar = GetNextChar;
    c = cgetch_state().LastChar;
    if( c == '?' ) {
//        SrcFiles->column++;
        cvars_state().CurrChar = c;
        return( c );
    }
    return( cvars_state().GetCharCheck( c ) );
}

static int getCharAfterTwoQuestion( void )
{
//    SrcFiles->column++;
    cvars_state().CurrChar = '?';
    cvars_state().NextChar = getCharAfterOneQuestion;
    return( cvars_state().CurrChar );
}

int getCharAfterBackSlash( void )
{
    if( cgetch_state().Blank1Count != 0 ) {
        --cgetch_state().Blank1Count;
        return( cvars_state().CurrChar = ' ' );
    }
    if( cgetch_state().Tab1Count != 0 ) {
        --cgetch_state().Tab1Count;
        return( cvars_state().CurrChar = '\t' );
    }
    if( cgetch_state().Blank2Count != 0 ) {
        --cgetch_state().Blank2Count;
        return( cvars_state().CurrChar = ' ' );
    }
    cvars_state().NextChar = GetNextChar;
    return( cvars_state().GetCharCheck( cgetch_state().LastChar ) );
}

static int getTestCharFromFile( void )
{
    int c;

    for( ; (c = *cvars_state().SrcFiles->src_ptr++) == '\0'; ) {
        /*
         * check to make sure the NUL character we just found is at the
         * end of the buffer, and not an embedded NUL character in the
         * source file.
         */
        if( cvars_state().SrcFiles->src_ptr != cvars_state().SrcFiles->src_end + 1 )
            break;
        if( ReadBuffer( cvars_state().SrcFiles ) ) {
            return( cvars_state().CurrChar );
        }
    }
    return( c );
}

static int getSecondMultiByte( void )
{
    int c;

    c = getTestCharFromFile();
    /*
     * should we do this for a multi-byte char?
     */
    cvars_state().NextChar = GetNextChar;
    cvars_state().CurrChar = c;
    return( c );
}

static char translateTriGraph( char c )
{
    struct tri_graph *p;

    for( p = cgetch_state().TriGraphs; p->tri != '\0'; ++p ) {
        if( c == p->tri ) {
            if( cvars_state().CompFlags.extensions_enabled ) {
                if( cvars_state().SkipLevel == cvars_state().NestLevel ) {
                    cvars_state().CompFlags.trigraph_alert = true;
                }
            }
            return( p->new_ );
        }
    }
    return( c );
}

static int tryBackSlashNewLine( void )
{
    int nc;

    /*
     * CurrChar is '\\' and SrcFiles->column is up to date
     */
    cgetch_state().Blank1Count = 0;
    cgetch_state().Blank2Count = 0;
    cgetch_state().Tab1Count = 0;
    nc = getTestCharFromFile();
    if( cvars_state().CompFlags.extensions_enabled ) {
        while( nc == ' ' ) {
            ++cgetch_state().Blank1Count;
            nc = getTestCharFromFile();
        }
        while( nc == '\t' ) {
            ++cgetch_state().Tab1Count;
            nc = getTestCharFromFile();
        }
        while( nc == ' ' ) {
            ++cgetch_state().Blank2Count;
            nc = getTestCharFromFile();
        }
    }
    if( nc == '\r' ) {
        nc = getTestCharFromFile();
    }
    if( nc == '\n' ) {
        if( cvars_state().CompFlags.scanning_cpp_comment && cvars_state().SkipLevel == cvars_state().NestLevel ) {
            CWarn1( ERR_SPLICE_IN_CPP_COMMENT );
        }
        if( cvars_state().CompFlags.cpp_mode ) {
            if( cvars_state().CompFlags.in_pragma ) {
                CppPrtChar( '\\' );
                CppPrtChar( '\n' );
            } else if( cvars_state().CompFlags.cpp_line_wanted ) {
                CppPrtChar( '\n' );
            }
        }
        NewLineStartPos( cvars_state().SrcFiles );
        cvars_state().SrcFileLoc = cvars_state().SrcFiles->src_loc;
//        SrcFiles->column = 0;
        return( GetNextChar() );
    }
    cgetch_state().LastChar = nc;
    cvars_state().NextChar = getCharAfterBackSlash;
    return( '\\' );
}

static int tryTrigraphAgain( void )
{
    int c;
    int xc;

//    SrcFiles->column++;
    c = getTestCharFromFile();
    if( c != '?' ) {
        xc = translateTriGraph( c );
        if( c != xc ) {
//            SrcFiles->column += 2;
            cvars_state().CurrChar = xc;
            cvars_state().NextChar = GetNextChar;
            if( xc == '\\' ) {
                return( tryBackSlashNewLine() );
            }
            return( xc );
        }
        cgetch_state().LastChar = c;
        cvars_state().CurrChar = '?';
        cvars_state().NextChar = getCharAfterTwoQuestion;
        return( '?' );
    }
    cvars_state().CurrChar = c;
    /*
     * leave NextChar set here because it could still be a trigraph
     */
    return( c );
}

int GetCharCheckFile( int c )
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
            cvars_state().CurrChar = '\0';
            if( cvars_state().SrcFiles->no_eol ) {
                cvars_state().CurrChar = '\n';
            }
            if( cvars_state().SrcFiles->src_ptr == cvars_state().SrcFiles->src_end + 1 ) {
                if( !ReadBuffer( cvars_state().SrcFiles ) ) {
                    return( GetNextChar() );
                }
            }
            return( cvars_state().CurrChar );
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
            cvars_state().CurrChar = c;
            return( tryBackSlashNewLine() );
        case '\r':
            break;
        default:
//            SrcFiles->column++;
            if( c > 0x7f && (scan_state().CharSet[c] & C_DB) ) {
                /*
                 * we should not process the second byte through
                 * normal channels since its value is meaningless
                 * out of context
                 */
                cvars_state().NextChar = getSecondMultiByte;
            }
            break;
        }
        cvars_state().CurrChar = c;
        return( c );
    }
    /*
     * we have one '?'
     */
//    SrcFiles->column++;
    nc = getTestCharFromFile();
    if( nc != '?' ) {
        cgetch_state().LastChar = nc;
        cvars_state().CurrChar = c;
        cvars_state().NextChar = getCharAfterOneQuestion;
        return( c );
    }
    /*
     * we have two '?'s
     */
    nnc = getTestCharFromFile();
    xc = translateTriGraph( nnc );
    if( nnc != xc ) {
//        SrcFiles->column += 2;
        cvars_state().CurrChar = xc;
        if( xc == '\\' ) {
            return( tryBackSlashNewLine() );
        }
        return( xc );
    }
    cgetch_state().LastChar = nnc;
    cvars_state().CurrChar = c;
    if( nnc == '?' ) {
        /*
         * the next char after this may be a valid trigraph!
         */
        cvars_state().NextChar = tryTrigraphAgain;
        return( c );
    }
    cvars_state().NextChar = getCharAfterTwoQuestion;
    return( c );
}

static int regetUngotCharAfterOneQuestion( void )
{
    int c;

    /*
     * column for 'LastChar' has been set
     */
    cvars_state().NextChar = GetNextChar;
    c = cgetch_state().LastChar;
    cvars_state().CurrChar = c;
    return( c );
}

static int regetUngotCharAfterTwoQuestion( void )
{
    int c;

    cvars_state().NextChar = regetUngotCharAfterOneQuestion;
    c = '?';
    cvars_state().CurrChar = c;
    return( c );
}

static int restartDetectOneQuestion( void )
{
    int c;

    cvars_state().NextChar = regetUngotCharAfterOneQuestion;
    c = '?';
    cvars_state().CurrChar = c;
    return( c );
}

static int restartDetectTwoQuestion( void )
{
    int c;

    cvars_state().NextChar = regetUngotCharAfterTwoQuestion;
    c = '?';
    cvars_state().CurrChar = c;
    return( c );
}

static int restartBackSlashWhiteSpace( void )
{
    int c;

    cvars_state().NextChar = getCharAfterBackSlash;
    c = '\\';
    cvars_state().CurrChar = c;
    return( c );
}

void GetNextCharUndo( int c )
{
    if( cvars_state().NextChar == GetNextChar ) {
        /*
         * will return 'LastChar' and reset back to 'GetNextChar'
         */
        cvars_state().NextChar = regetUngotCharAfterOneQuestion;
        cgetch_state().LastChar = c;
        return;
    }
    if( cvars_state().NextChar == getCharAfterOneQuestion ) {
        /*
         * LastChar already has character saved in it
         * c should be '?'
         */
        cvars_state().NextChar = restartDetectOneQuestion;
        return;
    }
    if( cvars_state().NextChar == getCharAfterTwoQuestion ) {
        /*
         * LastChar already has character saved in it
         * c should be '?'
         */
        cvars_state().NextChar = restartDetectTwoQuestion;
        return;
    }
    if( cvars_state().NextChar == getCharAfterBackSlash ) {
        /*
         * LastChar already has character saved in it
         * c should be '\\'
         */
        cvars_state().NextChar = restartBackSlashWhiteSpace;
        return;
    }
#ifdef FDEBUG
    CFatal( "more than one GetNextCharUndo executed" );
#endif
}

static bool_ FCB_Alloc( FILE *fp, const char *filename, src_file_type typ )
{
    FCB             *fcb;
    unsigned char   *src_buffer;
    FNAMEPTR        flist;

    --cgetch_state().IncFileDepth;
    fcb = (FCB *)CMemAlloc( sizeof( FCB ) );
    src_buffer = static_cast<unsigned char *>(FEmalloc( SRC_BUF_SIZE + 3 ));
    if( fcb != NULL ) {
        src_buffer[0] = '\n';
        src_buffer[1] = '\0';
        fcb->src_buf = src_buffer;
        fcb->src_ptr = fcb->src_end = src_buffer + 1;
        flist = AddFlist( filename );
        FNameFullPath( flist );
        fcb->src_name = flist->name;
        fcb->src_line_cnt = 0;
        fcb->src_loc.line = 0;
        fcb->src_loc.column = 0;
        fcb->src_loc.fno = flist->index;
        cvars_state().SrcFileLoc = fcb->src_loc;
        fcb->src_flist = flist;
        fcb->fp = fp;
        fcb->prev_file = cvars_state().SrcFiles;
        fcb->prev_currchar = cvars_state().CurrChar;
#if _CPU == 370
        fcb->column = 0;     /* init colum, trunc info */
        fcb->trunc = 0;
        fcb->prevcount = 0;
#endif
        if( cvars_state().SrcFiles != NULL ) {
            if( cvars_state().SrcFiles == cvars_state().MainSrcFile ) {
                /*
                 * remember name of included file
                 */
                AddIncFileList( filename );
            }
        }
        fcb->rseekpos = 0;
        fcb->typ = typ;
        fcb->no_eol = false;
        fcb->eof = false;
        cvars_state().SrcFiles = fcb;
        cvars_state().CurrChar = '\n';    /* set next character to newline */
        return( true );
    }
    return( false );
}

static void FCB_Free( FCB *fcb )
{
    ++cgetch_state().IncFileDepth;
    FEfree( fcb->src_buf );
    CMemFree( fcb );
}

bool_ OpenFCB( FILE *fp, const char *filename, src_file_type typ )
{
    if( cgetch_state().IncFileDepth == 0 ) {
        CErr2( ERR_INCDEPTH, MAX_INC_DEPTH );
        CSuicide();
        return( false );
    }
    if( cvars_state().CompFlags.track_includes ) {
        /*
         * Don't track the top level file (any semi-intelligent user should
         * have no trouble tracking *that* down)
         */
        if( cgetch_state().IncFileDepth < MAX_INC_DEPTH ) {
            CInfoMsg( INFO_INCLUDING_FILE, filename );
        }
    }

    if( FCB_Alloc( fp, filename, typ ) ) {
        return( true );
    }
    CErr1( ERR_OUT_OF_MEMORY );
    return( false );
}

void CloseFCB( FCB *fcb )
{
    if( cvars_state().CompFlags.scanning_comment ) {
        CErr2( ERR_INCOMPLETE_COMMENT, cvars_state().CommentLoc.line );
    }
    if( fcb->no_eol ) {
        source_loc  err_loc;

        err_loc.line = fcb->src_line_cnt - 1;
        err_loc.column = fcb->src_loc.column;
        err_loc.fno = fcb->src_flist->index;
        SetErrLoc( &err_loc );
        CWarn1( ERR_NO_EOL_BEFORE_EOF );
        InitErrLoc();
    }
    cvars_state().SrcFiles = fcb->prev_file;
    cvars_state().CurrChar = fcb->prev_currchar;
    if( cvars_state().SrcFiles != NULL ) {
        if( cvars_state().SrcFiles->fp == NULL ) {
            /*
             * physical file name must be used, not logical
             */
            cvars_state().SrcFiles->fp = fopen( cvars_state().SrcFiles->src_flist->name, "rb" );
            fseek( cvars_state().SrcFiles->fp, cvars_state().SrcFiles->rseekpos, SEEK_SET );
        }
        cvars_state().SrcFileLoc = cvars_state().SrcFiles->src_loc;
        cvars_state().IncLineCount += fcb->src_line_cnt;
        if( cvars_state().SrcFiles == cvars_state().MainSrcFile ) {
            if( cvars_state().CompFlags.make_precompiled_header ) {
                cvars_state().CompFlags.make_precompiled_header = false;
                if( cvars_state().ErrCount == 0 ) {
                    BuildPreCompiledHeader( cvars_state().PCH_FileName );
                }
            }
        }
        if( cvars_state().CompFlags.cpp_mode ) {
            CppEmitPoundLine( cvars_state().SrcFiles->src_loc.line, cvars_state().SrcFiles->src_name, true );
        }
    } else {
        cvars_state().SrcLineCount = fcb->src_line_cnt;
        cvars_state().CurrChar = LCHR_EOF;
    }
    FCB_Free( fcb );
}

void SrcPurge( void )
/*******************/
{
    FCB *fcb;

    while( (fcb = cvars_state().SrcFiles) != NULL ) {
        cvars_state().SrcFiles = fcb->prev_file;
        if( fcb->fp != NULL && fcb->fp != stdin )
            fclose( fcb->fp );
        FCB_Free( fcb );
    }
}
