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
* Description:  processing input line data and line queueing for macros
*
****************************************************************************/


#include "asmglob.h"
#include <ctype.h>
#include "asminlin.h"
#if defined( _STANDALONE_ )
#include "directiv.h"
#include "asmalloc.h"
#include "condasm.h"
#include "asmexpnd.h"
#include "macro.h"
#include "asminput.h"
#include "iopath.h"
#include "pathlist.h"
#include "standalo.h"
#include "pathgrp2.h"

#include "clibext.h"

#endif
#include "asmrun.hpp"

#if defined( _STANDALONE_ )

typedef struct line_list {
    struct line_list    *next;
    char                *line;
} line_list;

/*
 * NOTE: the line queue is now a STACK of line queues
 *       when a macro is being expanded, we push a new line queue on the stack
 *       thus there is 1 queue on the stack for every level of nesting in macros
 */
typedef struct input_queue {
    struct {
        line_list       *head;
        line_list       *tail;
    }                   lines;
    struct input_queue  *next;
} input_queue;

typedef struct file_list {
    struct file_list    *next;
    union {
        FILE            *fp;
        input_queue     *data;
    } u;
    const FNAME         *srcfile;   /* name of include file */
    unsigned long       line_num;   /* current line in parent file */
    bool                is_a_file;
    bool                hidden;
} file_list;

extern void             FreeForceInclude( void );

extern bool             in_prologue;
extern bool             CheckSeg;
extern bool             DefineProc;             // true if the definition of procedure
                                                // has not ended
extern unsigned long    PassTotal;

unsigned                BufSize;                // size of CodeBuffer

static input_queue      *line_queue  = NULL;    // line queue
static file_list        *file_stack  = NULL;    // top of included file stack
static char             *IncludePath = NULL;

#else


#endif


#if defined( _STANDALONE_ )

static bool get_asmline( char *ptr, unsigned max, FILE *fp )
/**********************************************************/
{
    int         quote = '\0';
    bool        skip;
    int         c;
    bool        got_something;
    /*
     * blow away any comments
     * look for ';'
     * note that ';' are ok in quoted strings
     */
    skip = false;
    got_something = false;
    for( ;; ) {
        c = getc( fp );
        /*
         * copy the line until we hit a NULL, newline, or ';' not in a quote
         */
        switch( c ) {
        case '\t':
        case ' ':
            if( (Options.mode & MODE_TASM)
              && ( quote == '\0' )
              && got_something
              && ( *(ptr - 1) == '\\' ) ) {
                skip = true;
            }
            break;
        case '\'':
        case '"':
        case '`':
            if( quote == '\0' ) {
                quote = c;
            } else if( c == quote ) {
                quote = '\0';
            }
            break;
        case '<':
            if( quote == '\0' ) {
                quote = '>';
            }
            break;
        case '>':
            if( c == quote ) {
                quote = '\0';
            }
            break;
        case ';':
            if( quote != '\0' ) {
                break;
            }
            skip = true;
            break;
        case '\r':
            /*
             * don't store '\r' character in string
             */
            continue;
        case '\n':
            /*
             * if continuation character found, pass over newline
             */
            if( got_something
              && ( *(ptr - 1) == '\\' ) ) {
                ptr--;
                max++;
                LineNumber++;
                skip = false;
                /*
                 * does not save the '\n' character after the continuation
                 * character '\\' in the string
                 */
                continue;
            }
            *ptr = '\0';
            /* fall through */
        case '\0':
            /*
             * we have found the end of the line
             */
            return( true );
        case 0x1A:
            /*
             * DOS text files may use Ctrl-Z as an end-of-file marker.
             * It can appear before the physical end of file (CP/M-era
             * files), so seek to physical EOF to stop all subsequent
             * reads from returning data past the logical end.
             */
            fseek( fp, 0, SEEK_END );
            /* fall through */
        case EOF:
            *ptr = '\0';
            return( got_something );
        }
        if( !skip ) {
            *ptr++ = (char)c;
            if( --max <= 1 )
                skip = true;
            got_something = true;
        }
    }
}

void PushLineQueue( void )
/************************/
{
    input_queue *new_;

    DebugMsg(( "PUSHLINEQUEUE\n" ));

    new_ = MemAllocSafe( sizeof( input_queue ) );
    new_->next = line_queue;
    new_->lines.head = NULL;
    new_->lines.tail = NULL;
    line_queue = new_;
}

bool PopLineQueue( void )
/************************
 * remove a line queue from the top of the
 * stack & throw it away
 */
{
    input_queue *tmp;

    /*
     * pop the line_queue stack
     */
    tmp = line_queue;
    in_prologue = false;
    if( tmp == NULL )
        return( false );
    line_queue = line_queue->next;
    MemFree( tmp );
    return( true );
}

bool GetQueueMacroHidden( void )
/******************************/
{
    if(( file_stack != NULL )
      && !file_stack->is_a_file ) {
        return( file_stack->hidden );
    } else {
        return( false );
    }
}

static line_list *enqueue( void )
/*******************************/
{
    line_list   *new_;

    if( line_queue == NULL ) {
        line_queue = MemAllocSafe( sizeof( input_queue ) );
        line_queue->next = NULL;
        line_queue->lines.tail = NULL;
        line_queue->lines.head = NULL;
    }
    new_ = MemAllocSafe( sizeof( line_list ) );
    /*
     * add new item to the end of list
     */
    new_->next = NULL;
    if( line_queue->lines.head == NULL ) {
        line_queue->lines.head = new_;
    } else {
        line_queue->lines.tail->next = new_;
    }
    line_queue->lines.tail = new_;

    return( new_ );
}

static file_list *push_flist( const char *name, bool is_a_file )
/**************************************************************/
{
    file_list   *new_;

    new_ = MemAllocSafe( sizeof( file_list ) );
    new_->next = file_stack;
    file_stack = new_;
    new_->line_num = LineNumber;
    new_->is_a_file = is_a_file;
    new_->hidden = false;
    if( !is_a_file ) {
        dir_node_handle dir;

        dir = (dir_node_handle)AsmGetSymbol( name );
        LineNumber = dir->line_num;
        new_->srcfile = dir->e.macroinfo->srcfile;
    } else {
        new_->srcfile = AddFlist( name );
        LineNumber = 0;
    }
    return( new_ );
}

const FNAME *get_curr_srcfile( void )
/***********************************/
{
    return( ( file_stack == NULL ) ? ModuleInfo.srcfile : file_stack->srcfile );
}

void print_include_file_nesting_structure( void )
/***********************************************/
{
    file_list       *tmp;
    unsigned        tab = 1;

    tmp = file_stack;
    if( tmp == NULL )
        return;

    for( ; tmp != NULL ; ) {
        if( tmp->next == NULL )
            break;
        if( !tmp->hidden ) {
            if( tmp->is_a_file ) {
                _AsmNote( 0, NOTE_INCLUDED_BY, tmp->next->srcfile->name, tmp->line_num );
            } else {
                _AsmNote( 0, NOTE_MACRO_CALLED_FROM, tmp->next->srcfile->name, tmp->line_num );
            }
        }
        tmp = tmp->next;
        tab++;
    }

    if( !tmp->hidden ) {
        if( tmp->is_a_file ) {
            _AsmNote( 0, NOTE_INCLUDED_BY, ModuleInfo.srcfile->name, tmp->line_num );
        } else {
            _AsmNote( 0, NOTE_MACRO_CALLED_FROM, ModuleInfo.srcfile->name, tmp->line_num );
        }
    }
}

void InputQueueLine( const char *line )
/*************************************/
{
    line_list   *new_;

    DebugMsg(( "QUEUELINE: %s  ( line %lu ) \n", line, LineNumber ));
    new_ = enqueue();
    new_->line = MemStrdupSafe( line );
}

static FILE *open_file_in_include_path( const char *name, char *fullpath )
/************************************************************************/
{
    char            *path_list;
    char            *p;
    FILE            *fp = NULL;
    char            c;

    while( isspace( *name ) )
        name++;

    path_list = IncludePath;
    if( path_list != NULL ) {
        while( (c = *path_list) != '\0' ) {
            p = fullpath;
            do {
                ++path_list;
                if( IS_PATH_LIST_SEP( c ) ) {
                    break;
                }
                *p++ = c;
            } while( (c = *path_list) != '\0' );
            c = p[-1];
            if( !IS_PATH_SEP( c ) ) {
                *p++ = DIR_SEP;
            }
            strcpy( p, name );
            fp = fopen( fullpath, "r" );
            if( fp != NULL ) {
                break;
            }
        }
    }
    return( fp );
}

bool InputQueueFile( const char *path )
/*************************************/
{
    FILE        *fp;
    file_list   *new_;
    char        fullpath[ _MAX_PATH ];
    pgroup2     pg;

    _splitpath2( path, pg.buffer, &pg.drive, &pg.dir, &pg.fname, &pg.ext );
    _makepath( fullpath, pg.drive, pg.dir, pg.fname, pg.ext );
    fp = fopen( fullpath, "r" );
    if( fp == NULL ) {
        if( IncludePath != NULL ) {
            fp = open_file_in_include_path( path, pg.buffer );
        }
        if( fp == NULL ) {
            AsmErr( CANNOT_OPEN_INCLUDE_FILE, fullpath );
            return( RC_ERROR );
        }
        new_ = push_flist( pg.buffer, true );
    } else {
        new_ = push_flist( path, true );
    }
    new_->u.fp = fp;
    return( RC_OK );
}

static char *input_get( char *string )
/************************************/
{
    line_list   *inputline;
    file_list   *inputfile;

    /*
     * Check the line_queue first; then the file_stack
     */
    Globals.expand_count = 0;
    while( line_queue != NULL ) {
        if( line_queue->lines.head != NULL ) {
            inputline = line_queue->lines.head;
            strcpy( string, inputline->line );
            line_queue->lines.head = inputline->next;
            if( line_queue->lines.head == NULL )
                line_queue->lines.tail = NULL;
            MemFree( inputline->line );
            MemFree( inputline );
            return( string );
        }
        PopLineQueue();
    }

    while( file_stack != NULL ) {
        inputfile = file_stack;
        if( inputfile->is_a_file ) {
            if( get_asmline( string, MAX_LINE_LEN, inputfile->u.fp ) ) {
                LineNumber++;
                return( string );
            }
            /*
             * EOF is reached
             */
            fclose( inputfile->u.fp );
        } else {
            /*
             * this "file" is just a line queue for a macro
             */
            inputline = inputfile->u.data->lines.head;
            LineNumber++;
            if( inputline != NULL ) {
                strcpy( string, inputline->line );
                inputfile->u.data->lines.head = inputline->next;
                MemFree( inputline->line );
                MemFree( inputline );
                return( string );
            }
            MacroEnd( false );
            MemFree( inputfile->u.data );
        }
        file_stack = inputfile->next;
        LineNumber = inputfile->line_num;
        MemFree( inputfile );
    }
    return( NULL );
}

char *ReadTextLine( char *string )
/*********************************
 * get a new line, first checking the line queue & the file stack,
 * then looking in the assembly file itself
 * put it into string and return a pointer to the front
 */
{

    char *line;

    string[0] = '\0';
    line = input_get( string );
    if( line != NULL ) {
        return( line );
    }
    if( !get_asmline( string, MAX_LINE_LEN, AsmFiles.fp[ASM] ) ) {
        return( NULL );
    }
    LineNumber++;
    return( string );
}

static char *join_multiline_cmds( char *line, size_t max_len )
/************************************************************/
{
    size_t  linelen;
    size_t  i;

    /*
     * Trailing-comma line continuation is accepted in WATCOM and MASM 6
     * compatible modes. TASM MASM mode follows MASM 5 behavior and treats
     * the comma as a blank trailing argument (`mymacro 1,`).
     */
    if( (Options.mode & (MODE_TASM | MODE_MASM5)) == (MODE_TASM | MODE_MASM5) )
        return( line );
    linelen = strlen( line );
    /*
     * if the last non-whitespace character is a comma
     * join the next line on
     */
    for( i = linelen; i; --i ) {
        if( !isspace( line[ i - 1 ] ) ) {
            if( line[ i - 1 ] == ',' )
                ScanLine( line + linelen, max_len - linelen );
            break;
        }
    }
    return( line );
}

bool    ScanLine( char *string, size_t max_len )
/**********************************************/
{
    char        *line;
    char        buffer[MAX_LINE_LEN];
    size_t      len;

    line = ( max_len < MAX_LINE_LEN ) ? buffer : string;
    line = ReadTextLine( line );
    if( line != NULL ) {
        prep_line_for_conditional_assembly( line );
        if( line != string ) {          /* comparing the pointers */
            len = strlen( line ) + 1;
            if( len <= max_len ) {
                memcpy( string, line, len );
            } else {
                AsmError( ASM_CODE_TOO_LONG );
                return( false );
            }
        }
        join_multiline_cmds( string, max_len );
    }
    return( line != NULL );
}

void AsmCodeByte( unsigned char byte )
/************************************/
{
    if( CurrSeg != NULL ) {
        if( CurrSeg->seg->e.seginfo->iscode == SEGTYPE_UNDEF ) {
            CurrSeg->seg->e.seginfo->iscode = SEGTYPE_ISCODE;
        }
    }
    OutSelect( false );
    AsmByte( byte );
}

void AsmDataByte( unsigned char byte )
/************************************/
{
    OutSelect( true );
    AsmByte( byte );
}

static bool CheckHaveSeg( void )
/******************************/
{
    if( CurrSeg != NULL )
        return( true );

    if( CheckSeg ) {
        AsmError( DATA_EMITTED_WITH_NO_SEGMENT );
        write_to_file = false;
        CheckSeg = false;
    }
    return( false );
}

void AddItemToIncludePath( const char *path_list, const char *end )
/*****************************************************************/
{
    size_t      len;
    size_t      old_len;
    char        *old_list;
    char        *p;

    if( end == NULL ) {
        len = strlen( path_list );
        end = path_list + len;
    } else {
        len = end - path_list;
    }
    if( len > 0 ) {
        if( IncludePath == NULL ) {
            p = IncludePath = MemAllocSafe( len + 1 );
        } else {
            old_list = IncludePath;
            old_len = strlen( old_list );
            IncludePath = MemAllocSafe( old_len + 1 + len + 1 );
            strcpy( IncludePath, old_list );
            MemFree( old_list );
            p = IncludePath + old_len;
        }
        do {
            if( p != IncludePath )
                *p++ = PATH_LIST_SEP;
            path_list = GetPathElement( path_list, end, &p );
        } while( path_list != end );
        *p = '\0';
    }
}

void FreeIncludePath( void )
/**************************/
{
    MemFree( IncludePath );
    FreeForceInclude();
}

void PushMacro( const char *name, bool hidden )
/*********************************************/
{
    file_list *new_;

    DebugMsg(( "PUSH_MACRO\n" ));
    new_ = push_flist( name, false );
    new_->u.data = line_queue;
    new_->hidden = hidden;
    line_queue = line_queue->next;
}

#ifdef DEBUG_OUT
static void dbg_output( token_buffer *tokbuf )
/*********************************************
 * For debugging use only; print out a simplied version of the source line
 * after it is parsed and the expression is evaluated
 */
{
    if( Options.int_debug ) {
        token_idx   i;

        DebugMsg(("Line: %lu ", LineNumber ));
        DebugMsg(("Output :"));
        for( i = 0; i < tokbuf->count; i++ ) {
            switch( tokbuf->tokens[i].class_ ) {
            case TC_NUM:
                DebugMsg(( " %d ", tokbuf->tokens[i].u.value ));
                break;
            case TC_STRING:
            case TC_STRING_SQUOTE:
            case TC_STRING_DQUOTE:
            case TC_STRING_ANGLE:
            case TC_STRING_BRACE:
                DebugMsg(( " '%s' ", tokbuf->tokens[i].string_ptr));
                break;
            case TC_OP_SQ_BRACKET:
                DebugMsg(( " %s ", "[" ));
                break;
            case TC_CL_SQ_BRACKET:
                DebugMsg(( " %s ", "]" ));
                break;
            case TC_COLON:
                DebugMsg(( " %s ", ":" ));
                break;
            case TC_RES_ID:
                switch( tokbuf->tokens[i].u.token ) {
                case T_PTR:
                    DebugMsg(( " %s ", "Ptr" ));
                    break;
                case T_NEAR:
                    DebugMsg(( " %s ", "Near" ));
                    break;
                case T_FAR:
                    DebugMsg(( " %s ", "Far" ));
                    break;
                case T_PROC:
                    DebugMsg(( " %s ", "Proc" ));
                    break;
                case T_BYTE:
                case T_SBYTE:
                    DebugMsg(( " %s ", "Byte" ));
                    break;
                case T_WORD:
                case T_SWORD:
                    DebugMsg(( " %s ", "Word" ));
                    break;
                case T_DWORD:
                case T_SDWORD:
                    DebugMsg(( " %s ", "DWord" ));
                    break;
                case T_PWORD:
                case T_FWORD:
                    DebugMsg(( " %s ", "FWord" ));
                    break;
                case T_QWORD:
                    DebugMsg(( " %s ", "QWord" ));
                    break;
                case T_TBYTE:
                    DebugMsg(( " %s ", "TByte" ));
                    break;
                case T_OWORD:
                    DebugMsg(( " %s ", "OWord" ));
                    break;
                default:
                    DebugMsg((" %s ", tokbuf->tokens[i].string_ptr ));
                    break;
                }
                break;
            default:
                DebugMsg((" %s ", tokbuf->tokens[i].string_ptr ));
                break;
            }
        }
        DebugMsg(("\n"));
    } /* if Options.int_debug */
}
#endif
#endif

void AsmByte( AsmRun& assembler, unsigned char byte )
/*********************************
 * Write a byte to the object file
 */
{
#if defined( _STANDALONE_ )
    if( CheckHaveSeg() ) {
        (CurrSeg->seg->e.seginfo->current_loc)++;
        if( CurrSeg->seg->e.seginfo->length < CurrSeg->seg->e.seginfo->current_loc  ) {
            CurrSeg->seg->e.seginfo->length = CurrSeg->seg->e.seginfo->current_loc;
        }
        if( Parse_Pass != PASS_1
          && write_to_file ) {
            AsmCodeBuffer[BufSize++] = byte;
            if( BufSize >= MAX_LEDATA_THRESHOLD ) {
                FlushCurrSeg();
            }
        }
    }
    PassTotal++;
#else
    if( assembler.asminlin->AsmCodeAddress <= assembler.asminlin->AsmCodeLimit ) {
        assembler.asminlin->AsmCodeBuffer[assembler.asminlin->AsmCodeAddress++] = byte;
    }
#endif
}

#if defined( _STANDALONE_ )

void AsmLine( const char *string )
/********************************/
{
    token_idx       count;
    token_buffer    tokbuf;

    if( AsmScan( &tokbuf, string ) ) {
        write_to_file = false;
        return;
    }
    /*
     * A lone text equate used as a statement ( name EQU <text> ; then  name )
     * must be substituted before the macro/statement is decided -- the VMM DDK's
     * EndProc invokes  ??_ends  whose text is the matching segment-close macro
     * name (e.g. VxD_LOCKED_CODE_ENDS).  ExpandMacro only recognises a macro
     * NAME token, so expand the leading const first, then let ExpandMacro run.
     * Guarded to count==1 so a definition ( name EQU ... ) LHS is never expanded.
     */
    if( tokbuf.count == 1 && tokbuf.tokens[0].class_ == TC_ID ) {
        bool expanded;
        if( ExpandSymbol( &tokbuf, 0, false, &expanded ) ) {
            write_to_file = false;
            return;
        }
    }
    if( ExpandMacro( &tokbuf ) ) {
        write_to_file = false;
        return;
    }
    if( tokbuf.count > 0 ) {
        if( tokbuf.count > 1 ) {
            if( ExpandTheWorld( &tokbuf, 1, true, true ) ) {
                write_to_file = false;
                return;
            }
        }
        if( CurrProc != NULL
          && !DefineProc ) {
            for( count = 0; count < tokbuf.count; count++ ) {
                bool expanded;
                if( ExpandProcString( &tokbuf, count, &expanded ) ) {
                    write_to_file = false;
                    return;
                }
                if( expanded ) {
                    return;
                }
            }
        }
        if( AsmParse( &tokbuf, string ) ) {
            write_to_file = false;
        }
  #ifdef DEBUG_OUT
        dbg_output( &tokbuf );               // for debuggin only
  #endif
    }
}

#else

void AsmLine( AsmRun& assembler, const char *string, bool use_emu )
/**********************************************/
{
    enum fpe        old_floating_point;
    token_buffer    tokbuf;

    old_floating_point = static_cast<enum fpe>(assembler.asmglob.SWData.fpt);
    if( old_floating_point != NO_FP_ALLOWED ) {
        assembler.asmglob.SWData.fpt = ( use_emu ) ? DO_FP_EMULATION : NO_FP_EMULATION;
    }
    if( !AsmScan( assembler, &tokbuf, string ) ) {
        if( tokbuf.count > 0 ) {
            AsmParse( assembler, &tokbuf, string );
        }
    }
    assembler.asmglob.SWData.fpt = old_floating_point;
}

#endif
