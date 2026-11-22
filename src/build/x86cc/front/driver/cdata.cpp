/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2025 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  InitGlobalVars() initialises all global variables from cfe;
*               Variables are, unfortunately, declared all over the joint.
*
****************************************************************************/


#define global
#include "cvars.h"
#include "i64.h"
#include "cmacadd.h"


void InitGlobalVars( void )
{
    cvars_state().PCH_Start               = NULL;     /* start of precompiled memory block */
    cvars_state().PCH_End                 = NULL;     /* end of precompiled memory block */
    cvars_state().PCH_Macros              = NULL;     /* macros loaded from pre-compiled header */
    cvars_state().PCH_FileName            = NULL;     /* name to use for pre-compiled header */
    cvars_state().IncFileList             = NULL;     /* list of primary include files for PCH */
    cvars_state().PCH_SymArray            = NULL;     /* array of symbol table pointers from PCH */
    cvars_state().PCH_MaxSymHandle        = 0;        /* number of symbols in PCH_SymArray */
    cvars_state().DebugFlag               = 0;
    cvars_state().CurToken                = T_NULL;
    cvars_state().BadTokenInfo            = ERR_NONE;
    cvars_state().TokenLen                = 0;
    cvars_state().TokenLoc.line           = 0;
    cvars_state().TokenLoc.column         = 0;
    cvars_state().TokenLoc.fno            = 0;
    cvars_state().SrcFileLoc.line         = 0;
    cvars_state().SrcFileLoc.column       = 0;
    cvars_state().SrcFileLoc.fno          = 0;
    cvars_state().CommentLoc.line         = 0;
    cvars_state().CommentLoc.column       = 0;
    cvars_state().CommentLoc.fno          = 0;
    cvars_state().CurrChar                = 0;
    cvars_state().ConstType               = static_cast<DATA_TYPE>(0);
    cvars_state().MainSrcFile             = NULL;     /* primary source file being compiled */
    cvars_state().SrcFiles                = NULL;
    cvars_state().SrcFName                = NULL;     /* source file name without suffix */
    cvars_state().DefFName                = NULL;     /* .def file name (prototypes) */
    cvars_state().WholeFName              = NULL;     /* whole file name with suffix */
    cvars_state().ForceInclude            = NULL;
    cvars_state().ForcePreInclude         = NULL;
#if _CPU == 370
    AuxName                 = NULL;
#endif
    cvars_state().FNames                  = NULL;     /* list of file names processed */
    cvars_state().ErrFile                 = NULL;     /* error file */
    cvars_state().DefFile                 = NULL;     /* output for func prototypes */
    cvars_state().CppFile                 = NULL;     /* output for preprocessor */
    cvars_state().CppStack                = NULL;     /* #if structure control stack */
    cvars_state().IncPathList             = NULL;     /* list of path names to try for include files */
    cvars_state().SrcLoc.line             = 0;
    cvars_state().SrcLoc.column           = 0;
    cvars_state().SrcLoc.fno              = 0;
    cvars_state().SrcLineCount            = 0;        /* # of lines in primary source file */
    cvars_state().IncLineCount            = 0;        /* # of lines in all included files  */
    cvars_state().ErrCount                = 0;        /* total # of errors encountered     */
    cvars_state().WngCount                = 0;        /* total # of warnings encountered   */
    cvars_state().WngLevel                = 0;        /* warning severity level */
    cvars_state().TypeCount               = 0;        /* total # of type nodes allocated   */
    cvars_state().GblSymCount             = 0;        /* total # of global symbols */
    cvars_state().LclSymCount             = 0;        /* total # of local and temporary symbols */
    cvars_state().FuncCount               = 0;        /* total # of functions defined in module */
    cvars_state().SizeOfCount             = 0;        /* # of nested sizeof() expressions  */
    cvars_state().SymLevel                = 0;        /* current lex level (# of nested {) */
    cvars_state().SavedId                 = NULL;     /* saved id when doing look ahead */
    cvars_state().SavedTokenLoc.line      = 0;        /* value of TokenLine when id saved */
    cvars_state().SavedTokenLoc.column    = 0;        /* value of TokenColumn when id saved */
    cvars_state().SavedTokenLoc.fno       = 0;        /* value of TokenFno when id saved */
    cvars_state().LAToken                 = T_NULL;   /* look ahead token */
    cvars_state().LabelHead               = NULL;     /* list of all labels defined in function */
    cvars_state().TagHead                 = NULL;     /* list of all struct, union, enum tags */
    cvars_state().DeadTags                = NULL;     /* list of all tags that are out of scope */
    cvars_state().CurFunc                 = NULL;     /* current function being worked on */
    cvars_state().ParmList                = NULL;     /* list of parms for function */
    cvars_state().GlobalSym               = SYM_NULL; /* global symbol table list head */
#if _CPU == 386
    cvars_state().SymSTOD                 = SYM_NULL; /* builtin symbol for 'rep stosd' */
    cvars_state().SymSTOSB                = SYM_NULL; /* builtin symbol for '__STOSB' */
    cvars_state().SymSTOSD                = SYM_NULL; /* builtin symbol for '__STOSD' */
#endif
#ifdef __SEH__
    cvars_state().SymTryInit              = SYM_NULL; /* builtin symbol for '__TryInit' */
    cvars_state().SymTryFini              = SYM_NULL; /* builtin symbol for '__TryFini' */
    cvars_state().SymExcept               = SYM_NULL; /* builtin symbol for '__Except' */
    cvars_state().SymFinally              = SYM_NULL; /* builtin symbol for '__Finally' */
    cvars_state().SymTryUnwind            = SYM_NULL; /* builtin symbol for '__TryUnwind' */
    cvars_state().TrySymHandle            = SYM_NULL; /* builtin symbol for local try block */
    cvars_state().TryScope                = 0;        /* current scope of _try blocks */
    cvars_state().TryCount                = 0;        /* current number of _try blocks */
#endif
    cvars_state().SymSTOW                 = SYM_NULL; /* builtin symbol for 'rep stosw' */
    cvars_state().SymSTOWB                = SYM_NULL; /* builtin symbol for 'rep stosw, stosb' */
    cvars_state().SymMIN                  = SYM_NULL; /* builtin symbol for 'min(a,b)' */
    cvars_state().SymMAX                  = SYM_NULL; /* builtin symbol for 'max(a,b)' */
    cvars_state().SymMEMCMP               = SYM_NULL; /* builtin symbol for 'memcmp' func */
    cvars_state().SpecialSyms             = SYM_NULL; /* builtin symbols (thread linked) */
    cvars_state().CharSymHandle           = SYM_NULL; /* sym handle for "char" typedef */
    cvars_state().Sym_CS                  = SYM_NULL; /* sym handle for __segname("_CODE") ie. CS */
    cvars_state().Sym_SS                  = SYM_NULL; /* sym handle for __segname("_STACK")ie. SS */
    cvars_state().SymCover                = SYM_NULL; /* sym handle for '__COVERAGE' */
    cvars_state().SymDFAbbr               = SYM_NULL; /* sym handle for '__DFABBREV' */
    cvars_state().SymChipBug              = SYM_NULL; /* sym handle for '__chipbug' */
    cvars_state().ErrSym                  = NULL;

#if _CPU == 386
    cvars_state().FunctionProfileBlock    = NULL;     /* handle for profiling data block */
    cvars_state().FunctionProfileSegId    = SEG_NULL; /* segment for profiling data block */
#endif

    cvars_state().MacroDepth              = 0;
    cvars_state().HashTab                 = NULL;

    cvars_state().GenSwitches             = static_cast<cg_switches>(0);        /* target independant switches for code generator */
    cvars_state().TargetSwitches          = static_cast<cg_target_switches>(0);        /* target specific code generator switches */

    cvars_state().ProcRevision            = 0;        /* processor revision for c.g. */
    cvars_state().GenCodeGroup            = NULL;     /* pointer to code group name */
    cvars_state().ProEpiDataSize          = 0;        /* data to be alloc'd for pro/epi hook */
    cvars_state().ErrLimit                = 0;

    cvars_state().Inline_Threshold        = 0;        /* -oe=num for function inlining */

    cvars_state().DataPtrSize             = 0;
    cvars_state().CodePtrSize             = 0;
    cvars_state().DeadCode                = 0;        /* non-zero => next stmt is unreachable */
    cvars_state().TmpSymCount             = 0;
    cvars_state().LitCount                = 0;
    cvars_state().LitPoolSize             = 0;
    cvars_state().MacroSize               = 0;
    cvars_state().SegmentNum              = 0;        /* next PRIVATE segment number to use */
    cvars_state().FarStringSegId          = SEG_NULL;

    cvars_state().Environment             = NULL;     /* var for Suicide() */


    /* The following 3 arrays are also used by CGEN for saving _try block info */
    cvars_state().ExprLevel               = 0;

    cvars_state().SegListHead             = NULL;

    cvars_state().ScopeStruct             = 0;
    cvars_state().ScopeUnion              = 0;
    cvars_state().ScopeEnum               = 0;

    cvars_state().B_Int_1                 = 0;
    cvars_state().B_UInt_1                = 0;
    cvars_state().B_Short                 = 0;
    cvars_state().B_UShort                = 0;
    cvars_state().B_Int                   = 0;
    cvars_state().B_UInt                  = 0;
    cvars_state().B_Int32                 = 0;
    cvars_state().B_UInt32                = 0;
    cvars_state().B_Int64                 = 0;
    cvars_state().B_UInt64                = 0;
    cvars_state().B_Bool                  = 0;

    cvars_state().OptSize                 = 0;        /* 100 => make pgm small as possible */
    cvars_state().LoopDepth               = 0;        /* current nesting of loop constructs */
    cvars_state().HeadLibs                = NULL;     /* list of library search records */
    cvars_state().AliasHead               = NULL;     /* list of symbol alias records */
    cvars_state().CurFuncHandle           = SYM_NULL; /* sym_handle for current function */
    cvars_state().LastFuncOutOfMem        = NULL;     /* cinfo: */
    cvars_state().HashFreeList            = NULL;     /* list of available hash entries */

    cvars_state().SymBufSegNum            = 0;        /* segment # containing buffer */

    cvars_state().StringType              = NULL;     /* "unsigned char *" for use by literals */
    cvars_state().ConstCharType           = NULL;     /* "const char" type */
    cvars_state().StringArrayType         = NULL;     /* "unsigned char []" used by literals */
    cvars_state().NestedParms             = NULL;

    cvars_state().TextSegName             = NULL;     /* name of the text segment */
    cvars_state().DataSegName             = NULL;     /* name of the data segment */
    cvars_state().CodeClassName           = NULL;     /* name of the code class */
    cvars_state().ModuleName              = NULL;     /* name of module */
    cvars_state().ObjectFileName          = NULL;     /* name of object file */
    cvars_state().DependFileName          = NULL;     /* Name of make style auto depend file */
    cvars_state().DependHeaderPath        = NULL;     /* If no path is part of an included file, use this */
    cvars_state().DependForceSlash        = 0;
    cvars_state().PackAmount              = 0;        /* current packing alignment */
    cvars_state().GblPackAmount           = 0;        /* packing alignment given on command line */
    cvars_state().Column                  = 0;        /* skip to Column when reading */
    cvars_state().Trunc                   = 0;        /* stop at Trunc when reading  */
    cvars_state().PrevProtoType           = NULL;     /* prev func prototype */

    cvars_state().TargetSystem            = TS_OTHER;
    cvars_state().DefDataSegment          = SEG_NULL; /* #pragma data_seg("segname","class") */
    cvars_state().DefCodeSegment          = NULL;     /* #pragma code_seg("seg","c") */

    cvars_state().UnrollCount             = 0;        /* #pragma unroll(#); */
    cvars_state().InitialMacroFlags       = MFLAG_NONE;
    cvars_state().Stack87                 = 0;
    cvars_state().ErrorFileName           = NULL;

    cvars_state().UndefNames              = NULL;
    cvars_state().Check_global_prototype  = 0;

    memset( &cvars_state().CompFlags, 0, sizeof( cvars_state().CompFlags ) );
    memset( &cvars_state().CompVars, 0, sizeof( cvars_state().CompVars ) );

    Set64ValZero( cvars_state().Constant64 );

    InitMacroSegments();                /* initialize pointer to list of macro segments */
    InitStmt();
    InitErrLoc();
}
