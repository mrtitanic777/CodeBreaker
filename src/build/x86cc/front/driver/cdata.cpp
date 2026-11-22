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


#include "cvars.h"
#include "i64.h"
#include "cmacadd.h"
#include "frontrun.hpp"


void InitGlobalVars( FrontRun& front )
{
    front.cvars.PCH_Start               = NULL;     /* start of precompiled memory block */
    front.cvars.PCH_End                 = NULL;     /* end of precompiled memory block */
    front.cvars.PCH_Macros              = NULL;     /* macros loaded from pre-compiled header */
    front.cvars.PCH_FileName            = NULL;     /* name to use for pre-compiled header */
    front.cvars.IncFileList             = NULL;     /* list of primary include files for PCH */
    front.cvars.PCH_SymArray            = NULL;     /* array of symbol table pointers from PCH */
    front.cvars.PCH_MaxSymHandle        = 0;        /* number of symbols in PCH_SymArray */
    front.cvars.DebugFlag               = 0;
    front.cvars.CurToken                = T_NULL;
    front.cvars.BadTokenInfo            = ERR_NONE;
    front.cvars.TokenLen                = 0;
    front.cvars.TokenLoc.line           = 0;
    front.cvars.TokenLoc.column         = 0;
    front.cvars.TokenLoc.fno            = 0;
    front.cvars.SrcFileLoc.line         = 0;
    front.cvars.SrcFileLoc.column       = 0;
    front.cvars.SrcFileLoc.fno          = 0;
    front.cvars.CommentLoc.line         = 0;
    front.cvars.CommentLoc.column       = 0;
    front.cvars.CommentLoc.fno          = 0;
    front.cvars.CurrChar                = 0;
    front.cvars.ConstType               = static_cast<DATA_TYPE>(0);
    front.cvars.MainSrcFile             = NULL;     /* primary source file being compiled */
    front.cvars.SrcFiles                = NULL;
    front.cvars.SrcFName                = NULL;     /* source file name without suffix */
    front.cvars.DefFName                = NULL;     /* .def file name (prototypes) */
    front.cvars.WholeFName              = NULL;     /* whole file name with suffix */
    front.cvars.ForceInclude            = NULL;
    front.cvars.ForcePreInclude         = NULL;
#if _CPU == 370
    AuxName                 = NULL;
#endif
    front.cvars.FNames                  = NULL;     /* list of file names processed */
    front.cvars.ErrFile                 = NULL;     /* error file */
    front.cvars.DefFile                 = NULL;     /* output for func prototypes */
    front.cvars.CppFile                 = NULL;     /* output for preprocessor */
    front.cvars.CppStack                = NULL;     /* #if structure control stack */
    front.cvars.IncPathList             = NULL;     /* list of path names to try for include files */
    front.cvars.SrcLoc.line             = 0;
    front.cvars.SrcLoc.column           = 0;
    front.cvars.SrcLoc.fno              = 0;
    front.cvars.SrcLineCount            = 0;        /* # of lines in primary source file */
    front.cvars.IncLineCount            = 0;        /* # of lines in all included files  */
    front.cvars.ErrCount                = 0;        /* total # of errors encountered     */
    front.cvars.WngCount                = 0;        /* total # of warnings encountered   */
    front.cvars.WngLevel                = 0;        /* warning severity level */
    front.cvars.TypeCount               = 0;        /* total # of type nodes allocated   */
    front.cvars.GblSymCount             = 0;        /* total # of global symbols */
    front.cvars.LclSymCount             = 0;        /* total # of local and temporary symbols */
    front.cvars.FuncCount               = 0;        /* total # of functions defined in module */
    front.cvars.SizeOfCount             = 0;        /* # of nested sizeof() expressions  */
    front.cvars.SymLevel                = 0;        /* current lex level (# of nested {) */
    front.cvars.SavedId                 = NULL;     /* saved id when doing look ahead */
    front.cvars.SavedTokenLoc.line      = 0;        /* value of TokenLine when id saved */
    front.cvars.SavedTokenLoc.column    = 0;        /* value of TokenColumn when id saved */
    front.cvars.SavedTokenLoc.fno       = 0;        /* value of TokenFno when id saved */
    front.cvars.LAToken                 = T_NULL;   /* look ahead token */
    front.cvars.LabelHead               = NULL;     /* list of all labels defined in function */
    front.cvars.TagHead                 = NULL;     /* list of all struct, union, enum tags */
    front.cvars.DeadTags                = NULL;     /* list of all tags that are out of scope */
    front.cvars.CurFunc                 = NULL;     /* current function being worked on */
    front.cvars.ParmList                = NULL;     /* list of parms for function */
    front.cvars.GlobalSym               = SYM_NULL; /* global symbol table list head */
#if _CPU == 386
    front.cvars.SymSTOD                 = SYM_NULL; /* builtin symbol for 'rep stosd' */
    front.cvars.SymSTOSB                = SYM_NULL; /* builtin symbol for '__STOSB' */
    front.cvars.SymSTOSD                = SYM_NULL; /* builtin symbol for '__STOSD' */
#endif
#ifdef __SEH__
    front.cvars.SymTryInit              = SYM_NULL; /* builtin symbol for '__TryInit' */
    front.cvars.SymTryFini              = SYM_NULL; /* builtin symbol for '__TryFini' */
    front.cvars.SymExcept               = SYM_NULL; /* builtin symbol for '__Except' */
    front.cvars.SymFinally              = SYM_NULL; /* builtin symbol for '__Finally' */
    front.cvars.SymTryUnwind            = SYM_NULL; /* builtin symbol for '__TryUnwind' */
    front.cvars.TrySymHandle            = SYM_NULL; /* builtin symbol for local try block */
    front.cvars.TryScope                = 0;        /* current scope of _try blocks */
    front.cvars.TryCount                = 0;        /* current number of _try blocks */
#endif
    front.cvars.SymSTOW                 = SYM_NULL; /* builtin symbol for 'rep stosw' */
    front.cvars.SymSTOWB                = SYM_NULL; /* builtin symbol for 'rep stosw, stosb' */
    front.cvars.SymMIN                  = SYM_NULL; /* builtin symbol for 'min(a,b)' */
    front.cvars.SymMAX                  = SYM_NULL; /* builtin symbol for 'max(a,b)' */
    front.cvars.SymMEMCMP               = SYM_NULL; /* builtin symbol for 'memcmp' func */
    front.cvars.SpecialSyms             = SYM_NULL; /* builtin symbols (thread linked) */
    front.cvars.CharSymHandle           = SYM_NULL; /* sym handle for "char" typedef */
    front.cvars.Sym_CS                  = SYM_NULL; /* sym handle for __segname("_CODE") ie. CS */
    front.cvars.Sym_SS                  = SYM_NULL; /* sym handle for __segname("_STACK")ie. SS */
    front.cvars.SymCover                = SYM_NULL; /* sym handle for '__COVERAGE' */
    front.cvars.SymDFAbbr               = SYM_NULL; /* sym handle for '__DFABBREV' */
    front.cvars.SymChipBug              = SYM_NULL; /* sym handle for '__chipbug' */
    front.cvars.ErrSym                  = NULL;

#if _CPU == 386
    front.cvars.FunctionProfileBlock    = NULL;     /* handle for profiling data block */
    front.cvars.FunctionProfileSegId    = SEG_NULL; /* segment for profiling data block */
#endif

    front.cvars.MacroDepth              = 0;
    front.cvars.HashTab                 = NULL;

    front.cvars.GenSwitches             = static_cast<cg_switches>(0);        /* target independant switches for code generator */
    front.cvars.TargetSwitches          = static_cast<cg_target_switches>(0);        /* target specific code generator switches */

    front.cvars.ProcRevision            = 0;        /* processor revision for c.g. */
    front.cvars.GenCodeGroup            = NULL;     /* pointer to code group name */
    front.cvars.ProEpiDataSize          = 0;        /* data to be alloc'd for pro/epi hook */
    front.cvars.ErrLimit                = 0;

    front.cvars.Inline_Threshold        = 0;        /* -oe=num for function inlining */

    front.cvars.DataPtrSize             = 0;
    front.cvars.CodePtrSize             = 0;
    front.cvars.DeadCode                = 0;        /* non-zero => next stmt is unreachable */
    front.cvars.TmpSymCount             = 0;
    front.cvars.LitCount                = 0;
    front.cvars.LitPoolSize             = 0;
    front.cvars.MacroSize               = 0;
    front.cvars.SegmentNum              = 0;        /* next PRIVATE segment number to use */
    front.cvars.FarStringSegId          = SEG_NULL;

    front.cvars.Environment             = NULL;     /* var for Suicide() */


    /* The following 3 arrays are also used by CGEN for saving _try block info */
    front.cvars.ExprLevel               = 0;

    front.cvars.SegListHead             = NULL;

    front.cvars.ScopeStruct             = 0;
    front.cvars.ScopeUnion              = 0;
    front.cvars.ScopeEnum               = 0;

    front.cvars.B_Int_1                 = 0;
    front.cvars.B_UInt_1                = 0;
    front.cvars.B_Short                 = 0;
    front.cvars.B_UShort                = 0;
    front.cvars.B_Int                   = 0;
    front.cvars.B_UInt                  = 0;
    front.cvars.B_Int32                 = 0;
    front.cvars.B_UInt32                = 0;
    front.cvars.B_Int64                 = 0;
    front.cvars.B_UInt64                = 0;
    front.cvars.B_Bool                  = 0;

    front.cvars.OptSize                 = 0;        /* 100 => make pgm small as possible */
    front.cvars.LoopDepth               = 0;        /* current nesting of loop constructs */
    front.cvars.HeadLibs                = NULL;     /* list of library search records */
    front.cvars.AliasHead               = NULL;     /* list of symbol alias records */
    front.cvars.CurFuncHandle           = SYM_NULL; /* sym_handle for current function */
    front.cvars.LastFuncOutOfMem        = NULL;     /* cinfo: */
    front.cvars.HashFreeList            = NULL;     /* list of available hash entries */

    front.cvars.SymBufSegNum            = 0;        /* segment # containing buffer */

    front.cvars.StringType              = NULL;     /* "unsigned char *" for use by literals */
    front.cvars.ConstCharType           = NULL;     /* "const char" type */
    front.cvars.StringArrayType         = NULL;     /* "unsigned char []" used by literals */
    front.cvars.NestedParms             = NULL;

    front.cvars.TextSegName             = NULL;     /* name of the text segment */
    front.cvars.DataSegName             = NULL;     /* name of the data segment */
    front.cvars.CodeClassName           = NULL;     /* name of the code class */
    front.cvars.ModuleName              = NULL;     /* name of module */
    front.cvars.ObjectFileName          = NULL;     /* name of object file */
    front.cvars.DependFileName          = NULL;     /* Name of make style auto depend file */
    front.cvars.DependHeaderPath        = NULL;     /* If no path is part of an included file, use this */
    front.cvars.DependForceSlash        = 0;
    front.cvars.PackAmount              = 0;        /* current packing alignment */
    front.cvars.GblPackAmount           = 0;        /* packing alignment given on command line */
    front.cvars.Column                  = 0;        /* skip to Column when reading */
    front.cvars.Trunc                   = 0;        /* stop at Trunc when reading  */
    front.cvars.PrevProtoType           = NULL;     /* prev func prototype */

    front.cvars.TargetSystem            = TS_OTHER;
    front.cvars.DefDataSegment          = SEG_NULL; /* #pragma data_seg("segname","class") */
    front.cvars.DefCodeSegment          = NULL;     /* #pragma code_seg("seg","c") */

    front.cvars.UnrollCount             = 0;        /* #pragma unroll(#); */
    front.cvars.InitialMacroFlags       = MFLAG_NONE;
    front.cvars.Stack87                 = 0;
    front.cvars.ErrorFileName           = NULL;

    front.cvars.UndefNames              = NULL;
    front.cvars.Check_global_prototype  = 0;

    memset( &front.cvars.CompFlags, 0, sizeof( front.cvars.CompFlags ) );
    memset( &front.cvars.CompVars, 0, sizeof( front.cvars.CompVars ) );

    Set64ValZero( front.cvars.Constant64 );

    InitMacroSegments(front);                /* initialize pointer to list of macro segments */
    InitStmt(front);
    InitErrLoc(front);
}
