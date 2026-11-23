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
* Description:  C compiler global variables.
*
****************************************************************************/


#pragma once
#define BY_C_FRONT_END

#ifndef global
    #define global  extern
#endif

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "target.h"
#include "cconst.h"
#include "dw.h"
#include "ctypes.h"
#include "macro.h"
#include "cfcb.h"
#include "csegid.h"
#include "ctokens.h"

inline unsigned char &GetMtoken( const char *p ) { return (*(TYPE_MTOKEN *)(p)); }

inline void SetMtoken( char *p, TOKEN v ) { *(TYPE_MTOKEN *)(p)=(v); }

inline unsigned char &GetMparm( const char *p ) { return (*(TYPE_MPARM *)(p)); }

inline int GetMacroParmCount( macro_entry *m ) { return ((m)->u.parm_count - 1); }

inline bool MacroWithParenthesis( macro_entry *m ) { return ((m)->u.parm_count > 0); }

inline bool MacroIsSpecial( macro_entry *m ) { return ((m)->macro_defn == 0); }

inline bool MacroHasVarArgs( macro_entry *m ) { return (((m)->macro_flags & MFLAG_HAS_VAR_ARGS) != 0); }
#include "cerrs.h"
#include "cmsg.h"
#include "cgswitch.h"
#include "pragdefn.h"

inline bool IsKeyword( TOKEN t ) { return (t >= FIRST_KEYWORD && t <= LAST_KEYWORD); }

inline bool IsIdOrKeyword( TOKEN t ) { return (t == T_ID || t >= FIRST_KEYWORD && t <= LAST_KEYWORD); }


#define MAX_LEVEL       1024
#define USER_LIB_PRIO   '9'
#define ERRLIMIT_NOMAX  ((unsigned)-1)

#define MSGIDX_INVALID  (-1)
inline bool IsMsgidxInvalid( unsigned int m ) { return ((int)(m) == MSGIDX_INVALID); }

/* whether a symbol, tag or hash entry is of the current lex level of the run (the bodies are in frontrun.hpp) */
struct FrontRun;
template< class Entry > inline bool CheckEqSymLevel( const FrontRun& front, const Entry *entry );
template< class Entry > inline bool CheckLtSymLevel( const FrontRun& front, const Entry *entry );


/* Macros to skip all typedefs and arrive at the underlying type */
inline void SkipTypedefs( type_definition *&typeptr )
{
    while( typeptr->decl_type == TYP_TYPEDEF ) { typeptr = typeptr->object; }
}
inline void SkipDummyTypedefs( type_definition *&typeptr )
{
    while( typeptr->decl_type == TYP_TYPEDEF && (typeptr->type_flags & TF2_DUMMY_TYPEDEF) ) { typeptr = typeptr->object; }
}
inline void SkipEnum( type_definition *&typeptr )
{
    if( typeptr->decl_type == TYP_ENUM ) { typeptr = typeptr->object; }
}
inline void SkipArrays( type_definition *&typeptr )
{
    while( typeptr->decl_type == TYP_ARRAY ) { typeptr = typeptr->object; }
}

typedef enum {
    SEGTYPE_CODE       = 1,             /* #pragma code_seg("segname","class") */
    SEGTYPE_DATA       = 2,             /* #pragma data_seg("segname","class") */
    SEGTYPE_BASED      = 3,             /* __based(__segname("segname")) */
    SEGTYPE_INITFINI   = 4,             /* "TI", "XI" or "YI" segment */
    SEGTYPE_INITFINITR = 5,             /* thread data */
} seg_type;

typedef struct library_list {
    struct  library_list    *next;      // used by precompiled header
    char                    priority;   // priority '1'-'9'
    char                    libname[1]; // library name
} library_list;

typedef struct alias_list {
    struct  alias_list      *next;
    const char              *name;      /* one of 'name', 'a_sym' is valid */
    SYM_HANDLE              a_sym;
    const char              *subst;
    SYM_HANDLE              s_sym;      /* one of 'subst', 's_sym' is valid */
    char                    names[1];
} alias_list;

typedef struct nested_parm_lists {
    struct nested_parm_lists    *prev_list;
    TYPEPTR                     *next_parm_type;
} nested_parm_lists;

typedef struct extref_info {
    struct extref_info  *next;
    SYM_HANDLE          symbol;
    char                name[1];
} extref_info;

typedef struct undef_names {
    struct undef_names  *next;
    char                *name;
} undef_names;

typedef struct {
    int64     value;
    DATA_TYPE type;
} const_val;



/* The ValueStack array is also used by CGEN for saving _try block info */


#ifndef CB_STATE_CVARS_H
#define CB_STATE_CVARS_H

/* the variables of cvars.h, one set for each run of the compiler (run/cbrun.hpp) */
struct CvarsState {
    char *PCH_Start;   // start of precompiled memory block
    char *PCH_End;   // end of precompiled memory block
    char *PCH_Macros;   // macros loaded from pre-compiled header
    char *PCH_FileName;   // name to use for pre-compiled header
    INCFILE *IncFileList;   // list of primary include files for PCH
    SYMPTR *PCH_SymArray;   // array of symbol table pointers from PCH
    unsigned PCH_MaxSymHandle;   // number of symbols in PCH_SymArray
    int DebugFlag;
    TOKEN CurToken;
    msg_codes BadTokenInfo;
    size_t TokenLen;
    source_loc TokenLoc;
    source_loc SrcFileLoc;   /* duplicate of SrcFile->src_line */
    source_loc CommentLoc;
    int CurrChar;
    DATA_TYPE ConstType;
    uint64 Constant64;
    FCB *MainSrcFile;   /* primary source file being compiled */
    FCB *SrcFiles;
    char *SrcFName;   /* source file name without suffix */
    char *DefFName;   /* .def file name (prototypes) */
    char *WholeFName;   /* whole file name with suffix */
    char *ForceInclude;
    char *ForcePreInclude;
    FNAMEPTR FNames;   /* list of file names processed */
    RDIRPTR RDirNames;   /* list of read only directorys */
    IALIASPTR IAliasNames;   /* list of include aliases */
    FILE *ErrFile;   /* error file */
    FILE *DefFile;   /* output for func prototypes */
    FILE *CppFile;   /* output for preprocessor */
    FILE *DepFile;   /* make style auto depend file */
    struct cpp_info *CppStack;   /* #if structure control stack */
    char *IncPathList;   /* list of path names to try for include files */
    source_loc SrcLoc;
    unsigned SrcLineCount;   /* # of lines in primary source file */
    unsigned IncLineCount;   /* # of lines in all included files  */
    unsigned ErrCount;   /* total # of errors encountered     */
    unsigned WngCount;   /* total # of warnings encountered   */
    int WngLevel;   /* warning severity level */
    int TypeCount;   /* total # of type nodes allocated   */
    int GblSymCount;   /* total # of global symbols */
    int LclSymCount;   /* total # of local and temporary symbols */
    int FuncCount;   /* total # of functions defined in module */
    int MacroCount;   /* total # of macros defined */
    int TagCount;   /* total # of tag entries */
    int FieldCount;   /* total # of struct field names */
    int EnumCount;   /* total # of enumerated symbols */
    int SizeOfCount;   /* # of nested sizeof() expressions  */
    int NestLevel;   /* pre-processing level of #if */
    int SkipLevel;   /* pre-processing level of #if to skip to */
    id_level_stype SymLevel;   /* current lex level (# of nested {) */
    bool Check_global_prototype;
    char *SavedId;   /* saved id when doing look ahead */
    source_loc SavedTokenLoc;   /* value of TokenLine when id saved */
    TOKEN LAToken;   /* look ahead token */
    LABELPTR LabelHead;   /* list of all labels defined in function */
    TAGPTR TagHead;   /* list of all struct, union, enum tags */
    TAGPTR DeadTags;   /* list of all tags that are out of scope */
    TYPEPTR TypeHead;   /* list of all type nodes allocated */
    TYPEPTR FuncTypeHead[MAX_PARM_LIST_HASH_SIZE + 1];
    TYPEPTR VoidParmList[2];   /* function with void parm */
    PARMPTR ParmList;   /* list of parms for function */
    SYM_HANDLE GlobalSym;   /* global symbol table list head */
#if _CPU == 386
    SYM_HANDLE SymSTOD;   /* builtin symbol for 'rep stosd' */
#endif
#if _CPU == 386
    SYM_HANDLE SymSTOSB;   /* builtin symbol for '__STOSB' */
#endif
#if _CPU == 386
    SYM_HANDLE SymSTOSD;   /* builtin symbol for '__STOSD' */
#endif
#ifdef __SEH__
    SYM_HANDLE SymTryInit;   /* builtin symbol for '__TryInit' */
#endif
#ifdef __SEH__
    SYM_HANDLE SymTryFini;   /* builtin symbol for '__TryFini' */
#endif
#ifdef __SEH__
    SYM_HANDLE SymExcept;   /* builtin symbol for '__Except' */
#endif
#ifdef __SEH__
    SYM_HANDLE SymFinally;   /* builtin symbol for '__Finally' */
#endif
#ifdef __SEH__
    SYM_HANDLE SymTryUnwind;   /* builtin symbol for '__TryUnwind' */
#endif
#ifdef __SEH__
    SYM_HANDLE TrySymHandle;   /* builtin symbol for local try block */
#endif
#ifdef __SEH__
    tryindex_t TryScope;   /* current scope of _try blocks */
#endif
#ifdef __SEH__
    tryindex_t TryCount;   /* current number of _try blocks */
#endif
    SYM_HANDLE SymSTOW;   /* builtin symbol for 'rep stosw' */
    SYM_HANDLE SymSTOWB;   /* builtin symbol for 'rep stosw, stosb' */
    SYM_HANDLE SymMIN;   /* builtin symbol for 'min(a,b)' */
    SYM_HANDLE SymMAX;   /* builtin symbol for 'max(a,b)' */
    SYM_HANDLE SymMEMCMP;   /* builtin symbol for 'memcmp' func */
    SYM_HANDLE SpecialSyms;   /* builtin symbols (thread linked) */
    SYM_HANDLE CharSymHandle;   /* sym handle for "char" typedef */
    SYM_HANDLE Sym_CS;   /* sym handle for __segname("_CODE") ie. CS */
    SYM_HANDLE Sym_SS;   /* sym handle for __segname("_STACK")ie. SS */
    SYM_HANDLE SymCover;   /* sym handle for '__COVERAGE' */
    SYM_HANDLE SymDFAbbr;   /* sym handle for '__DFABBREV' */
    SYM_HANDLE SymChipBug;   /* sym handle for '__chipbug' */
    FIELDPTR ErrSym;
#if _CPU == 386
    BACK_HANDLE FunctionProfileBlock;   /* handle for profiling data block */
#endif
#if _CPU == 386
    segment_id FunctionProfileSegId;   /* segment for profiling data block */
#endif
    int MacroDepth;
    char *MacroPtr;
    MEPTR UndefMacroList;
    MEPTR *MacHash;   /* [MACRO_HASH_SIZE] */
    ENUMPTR EnumTable[ID_HASH_SIZE];
    SYM_HASHPTR *HashTab;
    TYPEPTR BaseTypes[DATA_TYPE_SIZE];
    unsigned CTypeCounts[DATA_TYPE_SIZE];
    char *Buffer;
    char *TokenBuf;
    cg_switches GenSwitches;   /* target independant switches for code generator */
    cg_target_switches TargetSwitches;   /* target specific code generator switches */
    unsigned ProcRevision;   /* processor revision for c.g. */
    char *GenCodeGroup;   /* pointer to code group name */
    unsigned ProEpiDataSize;   /* data to be alloc'd for pro/epi hook */
    unsigned ErrLimit;
    target_size DataThreshold;   /* sizeof(obj) > this ==> separate segment */
    unsigned Inline_Threshold;   /* -oe=num for function inlining */
    unsigned DataPtrSize;
    unsigned CodePtrSize;
    int DeadCode;   /* non-zero => next stmt is unreachable */
    int TmpSymCount;
    int LitCount;
    target_size LitPoolSize;
    size_t MacroSize;
    ppctl_t PPControl;
    comp_flags CompFlags;
    global_comp_flags GlobalCompFlags;
    comp_vars CompVars;
    segment_id SegmentNum;   /* next PRIVATE segment number to use */
    segment_id FarStringSegId;
    jmp_buf *Environment;   /* var for Suicide() */
    TREEPTR ValueStack[MAX_LEVEL];
    TOKEN Token[MAX_LEVEL];
    token_class Class[MAX_LEVEL];
    expr_level_type ExprLevel;
    segment_list *SegListHead;
    TREEPTR FirstStmt;   /* root of expression tree */
    dbug_type ScopeStruct;
    dbug_type ScopeUnion;
    dbug_type ScopeEnum;
    dbug_type B_Int_1;
    dbug_type B_UInt_1;
    dbug_type B_Short;
    dbug_type B_UShort;
    dbug_type B_Int;
    dbug_type B_UInt;
    dbug_type B_Int32;
    dbug_type B_UInt32;
    dbug_type B_Int64;
    dbug_type B_UInt64;
    dbug_type B_Bool;
    int OptSize;   /* 100 => make pgm small as possible */
    char __Time[9];   /* "HH:MM:SS" for __TIME__ macro */
    char __Date[12];   /* "MMM DD YYYY" for __DATE__ macro */
    int LoopDepth;   /* current nesting of loop constructs */
    char CLIB_Name[10];   /* "1CLIBMT3x" */
    char MATHLIB_Name[10];   /* "5MATHx" or "8MATH87x" */
    char *EmuLib_Name;   /* "9emu87" for -fpi, "9noemu87" for -fpi */
    library_list *HeadLibs;
    alias_list *AliasHead;
    SYMPTR CurFunc;   /* current function being worked on */
    SYM_ENTRY CurFuncSym;   /* for contents of current function symbol */
    SYM_HANDLE CurFuncHandle;   /* sym_handle for current function */
    CGPOINTER LastFuncOutOfMem;   /* cinfo: */
    SYM_HASHPTR HashFreeList;   /* list of available hash entries */
    unsigned SymBufNum;   /* current buffer in memory */
    unsigned SymBufSegNum;   /* segment # containing buffer */
    unsigned LastSymBuf;   /* # of last symbol table buffer */
    unsigned SymBufDirty;   /* 1 => buffer has been changed */
    TYPEPTR StringType;   /* "unsigned char *" for use by literals */
    TYPEPTR ConstCharType;   /* "const char" type */
    TYPEPTR StringArrayType;   /* "unsigned char []" used by literals */
    nested_parm_lists *NestedParms;
    STR_HANDLE StringHash[STRING_HASH_SIZE];   /* string literals */
    char *TextSegName;   /* name of the text segment */
    char *DataSegName;   /* name of the data segment */
    char *CodeClassName;   /* name of the code class */
    char *ModuleName;   /* name of module */
    char *ObjectFileName;   /* name of object file */
    char *DependFileName;   /* name of make style auto depend file */
    char *DependHeaderPath;   /* path to prepend if included header has none */
    char *DependTarget;   /* name of target in make style autodep */
    char *SrcDepName;   /* name of first depend (sourcefile)*/
    char DependForceSlash;   /* type of slash to force to in depend output */
    align_type PackAmount;   /* current packing alignment */
    align_type GblPackAmount;   /* packing alignment given on command line */
    textsegment *TextSegList;   /* list of #pragma alloc_text segs*/
    extref_info *ExtrefInfo;
    int (*NextChar)( FrontRun& );
    void (*UnGetChar)( FrontRun&, int c );
    int (*GetCharCheck)( FrontRun&, int );
    struct debug_fwd_types *DebugNameList;
    unsigned Column;   /* skip to Column when reading */
    unsigned Trunc;   /* stop at Trunc when reading  */
    size_t EnumRecSize;   /* size of largest enum entry */
    int PreProcChar;
    TYPEPTR PrevProtoType;   /* prev func prototype */
    align_type SegAlignment[FIRST_PRIVATE_SEGMENT];
    unsigned TargetSystem;
    segment_id DefDataSegment;   /* #pragma data_seg("segname","class") */
    textsegment *DefCodeSegment;   /* #pragma code_seg("seg","c") */
    unroll_type UnrollCount;   /* #pragma unroll(#); */
    macro_flags InitialMacroFlags;
    unsigned char Stack87;
    char *ErrorFileName;
    undef_names *UndefNames;
    TREEPTR CurFuncNode;
    bool PrintWhiteSpace;   // also refered from cmac2.c
    call_list *CallNodeList;
    LABEL_INDEX LabelIndex;
    SYM_LIST_HEADS *SymListHeads;
    unsigned SymTypedef;
    TAGPTR TagHash[ID_HASH_SIZE + 1];
    unsigned NodeCount;
};

#endif

//================= Function Prototypes ========================

/* casian.c */
extern void         SetDBChar(FrontRun& front, int);

/* ccheck.c */
extern bool         CheckCompatibleFunction( FrontRun& front, TYPEPTR typ1, TYPEPTR typ2, bool topLevelCheck );
extern bool         CheckCompatibleLanguage( FrontRun& front, type_modifiers typ1, type_modifiers typ2 );
extern void         CheckTernary( FrontRun& front, TYPEPTR typ1, TYPEPTR typ2 );
extern void         CheckCallParms( FrontRun& front );
extern void         ParmCountMismatch( FrontRun& front );
extern void         CheckRetType( FrontRun& front, TREEPTR );
extern void         CompatiblePtrType( FrontRun& front, TYPEPTR, TYPEPTR, TOKEN );
extern bool         CheckIdenticalType( FrontRun& front, TYPEPTR, TYPEPTR );
extern bool         VerifyType( FrontRun& front, TYPEPTR, TYPEPTR, SYMPTR );
extern TYPEPTR      SkipTypeFluff( TYPEPTR typ );
extern bool         CheckZeroConstant( FrontRun& front, TREEPTR tree );
extern bool         CheckZero( FrontRun& front, TREEPTR tree );
extern bool         CheckAssignBits( uint64 *val, unsigned width, bool mask );
extern bool         CheckAssignRange( FrontRun& front, TYPEPTR typ1, TREEPTR opnd2 );
extern void         CheckParmAssign( FrontRun& front, TYPEPTR typ1, TREEPTR opnd2, int parm_num, bool asgn_check );

/* ccmain.c */
extern void         FreeRDir( FrontRun& front );
extern void         FrontEndInit( FrontRun& front, bool reuse );
extern bool         FrontEnd( FrontRun& front, char ** );
extern void         FrontEndFini( FrontRun& front );
extern void         CppComment( FrontRun& front, int );
extern bool         CppPrinting( FrontRun& front );
extern void         CppPuts( FrontRun& front, const char * );
extern void         CppPutsQuoted( FrontRun& front, const char * );
extern void         SetPPWidth( FrontRun& front, unsigned );
extern void         CppPrtChar( FrontRun& front, int );
extern void         CppPrtToken( FrontRun& front, TOKEN );
extern bool         OpenSrcFile( FrontRun& front, const char *, src_file_type );
extern void         CloseSrcFile( FrontRun& front, FCB * );
extern void         OpenDefFile( FrontRun& front );
extern FILE         *OpenBrowseFile( FrontRun& front );
extern void         CloseFiles( FrontRun& front );
extern void         FreeFNames( FrontRun& front );
extern char         *ErrFileName( FrontRun& front );
extern char         *DepFileName( FrontRun& front );
extern char         *ObjFileName( FrontRun& front );
extern char         *CppFileName( FrontRun& front );
extern char         *ForceSlash( char *, char );
extern char         *GetSourceDepName( FrontRun& front );
extern FNAMEPTR     NextDependency( FrontRun& front, FNAMEPTR );
extern void         PrtfFilenameErr( FrontRun& front, const char *filename, src_file_type typ, bool print_error );

extern FNAMEPTR     AddFlist( FrontRun& front, char const * );
extern FNAMEPTR     FileIndexToFName( FrontRun& front, unsigned );
extern char         *FNameFullPath( FrontRun& front, FNAMEPTR flist );
extern char         *FileIndexToCorrectName( FrontRun& front, unsigned file_index );
extern void         SrcFileReadOnlyDir( FrontRun& front, char const *dirs );
extern void         SrcFileReadOnlyFile( FrontRun& front, char const *file );
extern bool         SrcFileInRDir( FrontRun& front, FNAMEPTR flist );
extern void         SrcFileIncludeAlias( FrontRun& front, const char *alias_name, const char *real_name, bool is_lib );
extern int          SrcFileTime( char const *,time_t * );
extern void         SetSrcFNameOnce( FrontRun& front );
extern TOKEN        GetNextToken( FrontRun& front );
extern void         CppEmitPoundLine( FrontRun& front, unsigned, const char *, bool );

extern void         AddIncFile( FrontRun& front, INCFILE * );
extern void         AddIncFileList( FrontRun& front, const char *filename );
extern void         FreeIncFileList( FrontRun& front );

/* cdata.c */
extern void         InitGlobalVars( FrontRun& front );

/* cdebug.c */
extern dbug_type    DBType( FrontRun& front, TYPEPTR );
extern void         EmitDBType( FrontRun& front );

extern void         ParsePgm( FrontRun& front );
extern bool         CheckFuncMain( const char *name );
extern void         AdjParmType( FrontRun& front, SYMPTR sym );
extern void         Check_Struct_Union_Enum( FrontRun& front, TYPEPTR );
extern void         Declarator( FrontRun& front, SYMPTR sym, type_modifiers mod, TYPEPTR typ, decl_state state );
extern bool         DeclList( FrontRun& front, SYM_HANDLE * );
extern FIELDPTR     FieldDecl( FrontRun& front, TYPEPTR typ, type_modifiers mod, decl_state state );
extern TYPEPTR      TypeName( FrontRun& front );

/* cdecl2.c */
extern void         InvDecl( FrontRun& front );
extern parm_list    *NewParm( FrontRun& front, TYPEPTR, parm_list * );
extern TYPEPTR      *MakeParmList( FrontRun& front, parm_list *, bool );
extern FIELDPTR     FieldCreate( FrontRun& front, const char *name );
extern bool         LoopDecl( FrontRun& front, SYM_HANDLE *sym_head );

/* cdinit */
extern void         InitDataQuads( FrontRun& front );
extern void         FreeDataQuads( FrontRun& front );
extern bool         DataQuadsAvailable( FrontRun& front );
extern void         *StartDataQuadAccess( FrontRun& front );
extern void         EndDataQuadAccess( FrontRun& front, void * );
extern DATA_QUAD    *NextDataQuad( FrontRun& front );
extern void         InitSymData( FrontRun& front, TYPEPTR, TYPEPTR, int );
extern void         StaticInit( FrontRun& front, SYMPTR, SYM_HANDLE );
extern void         VarDeclEquals( FrontRun& front, SYMPTR, SYM_HANDLE );

/* cdump */
extern void         DumpFuncDefn( FrontRun& front );
extern void         SymDump( void );
extern char         *DiagGetTypeName( FrontRun& front, TYPEPTR typ );

/* cenum */
extern TYPEPTR      EnumDecl( FrontRun& front, type_modifiers );
extern ENUMPTR      EnumLookup( FrontRun& front, id_hash_idx, const char * );
extern void         EnumInit( FrontRun& front );
extern void         FreeEnums( FrontRun& front );

/* cerror.c */
extern void         CErr1( FrontRun& front, msg_codes msgnum );
extern void         CErr2( FrontRun& front, msg_codes msgnum, int );
extern void         CErr2p( FrontRun& front, msg_codes msgnum, const char * );
extern void         CErr3p( FrontRun& front, msg_codes msgnum, const char *, const char * );
extern void         CErr4p( FrontRun& front, msg_codes msgnum, const char *, const char *, const char * );
extern void         CErrP1( FrontRun& front, int parm_num, msg_codes msgnum );
extern void         SetErrLoc( FrontRun& front, source_loc * );
extern void         InitErrLoc( FrontRun& front );
extern void         CWarn1( FrontRun& front, msg_codes msgnum );
extern void         CWarn2( FrontRun& front, msg_codes msgnum, int );
extern void         CWarn2p( FrontRun& front, msg_codes msgnum, const char * );
extern void         CWarnP1( FrontRun& front, int parm_num, msg_codes msgnum );
extern void         PCHNote( FrontRun& front, msg_codes msgnum, ... );
extern void         CInfoMsg( FrontRun& front, msg_codes msgnum, ... );
extern void         CSuicide( FrontRun& front );
extern void         OpenErrFile( FrontRun& front );
extern void         FmtCMsg( FrontRun& front, char *buff, cmsg_info *info );
extern void         SetDiagSymbol( FrontRun& front, SYMPTR sym, SYM_HANDLE handle );
extern void         SetDiagEnum( FrontRun& front, ENUMPTR );
extern void         SetDiagMacro( FrontRun& front, MEPTR );
extern void         SetDiagType1( FrontRun& front, TYPEPTR typ_source );
extern void         SetDiagType2( FrontRun& front, TYPEPTR typ_target, TYPEPTR typ_source );
extern void         SetDiagType3( FrontRun& front, TYPEPTR typ_first, TYPEPTR typ_second, TOKEN opr );
extern void         SetDiagPop( FrontRun& front );

/* cexpr.c */
extern void         ExprInit( FrontRun& front );
extern void         CheckCallNode( FrontRun& front, TREEPTR tree );
extern TREEPTR      Expr( FrontRun& front );
extern TREEPTR      AddrExpr( FrontRun& front );
extern TREEPTR      BoolExpr( FrontRun& front, TREEPTR );
extern TREEPTR      CommaExpr( FrontRun& front );
extern bool         ConstExprAndType( FrontRun& front, const_val * );
extern TREEPTR      SingleExpr( FrontRun& front );
extern TREEPTR      IntLeaf( FrontRun& front, target_int );
extern TREEPTR      RValue( FrontRun& front, TREEPTR );
extern TREEPTR      LongLeaf( FrontRun& front, target_long );
extern TREEPTR      UIntLeaf( FrontRun& front, target_uint );
extern TREEPTR      VarLeaf( FrontRun& front, SYMPTR, SYM_HANDLE );
extern TREEPTR      BasedPtrNode( FrontRun& front, TYPEPTR, TREEPTR );
extern bool         IsLValue( TREEPTR );
extern op_flags     OpFlags( type_modifiers  flags );
extern type_modifiers FlagOps( op_flags ops );
extern FIELDPTR     SearchFields( TYPEPTR *class_typ, target_size *field_offset, const char *name );

/* cfold.c */
extern int64        LongValue64( FrontRun& front, TREEPTR leaf );
extern void         CastConstValue( FrontRun& front, TREEPTR, DATA_TYPE );
extern void         CastConstNode( FrontRun& front, TREEPTR leaf, TYPEPTR newtyp );
extern void         DoConstFold( FrontRun& front, TREEPTR );
extern void         FoldExprTree( FrontRun& front, TREEPTR );

/* cgen.c */
extern void         DoCompile( FrontRun& front );
extern void         EmitInit( FrontRun& front );
extern void         EmitAbort( void );
extern target_size  EmitBytes( FrontRun& front, STR_HANDLE );
extern void         GenInLineFunc( FrontRun& front, SYM_HANDLE sym_handle );
extern bool         IsInLineFunc( FrontRun& front, SYM_HANDLE sym_handle );

/* cgendata */
extern target_size  GetDQuadPointerSize( FrontRun& front, enum quad_flags dq_flags );
extern void         EmitDataQuads( FrontRun& front );
extern void         EmitZeros( FrontRun& front, target_size );
extern void         AlignIt( FrontRun& front, TYPEPTR );

/* cgetch */
extern int          GetNextChar( FrontRun& front );
extern void         GetNextCharUndo( FrontRun& front, int );
extern int          GetCharCheckFile( FrontRun& front, int );
extern int          getCharAfterBackSlash( FrontRun& front );
extern bool         OpenFCB( FrontRun& front, FILE *fp, const char *filename, src_file_type typ );
extern void         CloseFCB( FrontRun& front, FCB * );
extern void         SrcPurge( FrontRun& front );
extern void         InitIncFile( FrontRun& front );

/* cinfo.c */
extern void         SegInit( FrontRun& front );
extern void         SegFini( FrontRun& front );
extern segment_id   AddSegName( FrontRun& front, const char *, const char *, int );
extern segment_id   DefThreadSeg( FrontRun& front );
extern void         EmitSegLabels( FrontRun& front );
extern void         FiniSegLabels( FrontRun& front );
extern void         FiniSegBacks( FrontRun& front );
extern void         SetSegs( FrontRun& front );
extern SYM_HANDLE   SegSymHandle( FrontRun& front, segment_id );
extern void         SetFuncSegment( FrontRun& front, SYMPTR, segment_id );
extern void         SetFarHuge( FrontRun& front, SYMPTR, bool );
extern char         *SegClassName( FrontRun& front, segment_id );
extern hw_reg_set   *SegPeggedReg( FrontRun& front, segment_id );
extern void         SetSegment( FrontRun& front, SYMPTR );
extern void         SetSegAlign( FrontRun& front, SYMPTR );
extern void         AssignSeg( FrontRun& front, SYMPTR );
extern segment_id   SymSegId( FrontRun& front, SYMPTR sym );
extern void         SetSegSymHandle( FrontRun& front, SYM_HANDLE sym_handle, segment_id segid );
extern void         ImportNearSegIdInit( FrontRun& front );
extern void         ImportSegIdInit( FrontRun& front );

/* cintmain.c */
extern void         ConsErrMsg( FrontRun& front, cmsg_info *info );
extern void         ConsErrMsgVerbatim( FrontRun& front, char const *line );
extern void         BannerMsg( FrontRun& front, char const *line );
extern void         DebugMsg( FrontRun& front, char const *line );
extern void         NoteMsg( FrontRun& front, char const *line );
extern void         ConBlip( FrontRun& front );
extern void         MyExit( FrontRun& front, int ret );

/* cmac1.c */
extern void         InitTokenBuf( FrontRun& front, size_t );
extern void         MacroInit( FrontRun& front );
extern void         MacroAddComp( FrontRun& front );
extern void         MacroFini( FrontRun& front );
extern void         MacroPurge( void );
extern void         DumpAllMacros( FrontRun& front );
extern TOKEN        GetMacroToken( FrontRun& front );
extern TOKEN        SpecialMacro( FrontRun& front, MEPTR );
extern void         DoMacroExpansion( FrontRun& front, MEPTR );
extern void         InsertReScanPragmaTokens( FrontRun& front, const char *pragma );
extern void         InsertToken( FrontRun& front, TOKEN token, const char *str );

/* cmac2.c */
extern TOKEN        CheckControl( FrontRun& front );
extern bool         MacroDel( FrontRun& front, const char *name );
extern void         CppStackInit( FrontRun& front );
extern void         CppStackFini( FrontRun& front );
extern MEPTR        MacroScan( FrontRun& front );
extern TOKEN        Process_Pragma( FrontRun& front );

/* cmath.c */
extern TREEPTR      AddOp( FrontRun& front, TREEPTR, TOKEN, TREEPTR );
extern TREEPTR      InitAsgn( FrontRun& front, TYPEPTR, TREEPTR );
extern TREEPTR      AsgnOp( FrontRun& front, TREEPTR, TOKEN, TREEPTR );
extern TREEPTR      BinOp( FrontRun& front, TREEPTR, TOKEN, TREEPTR );
extern bool         IsPtrConvSafe( FrontRun& front, TREEPTR, TYPEPTR, TYPEPTR );
extern TREEPTR      CnvOp( FrontRun& front, TREEPTR, TYPEPTR, bool );
extern TREEPTR      FlowOp( FrontRun& front, TREEPTR, opr_code, TREEPTR );
extern void         CheckConst( FrontRun& front, TREEPTR );
extern TREEPTR      IntOp( FrontRun& front, TREEPTR, TOKEN, TREEPTR );
extern TREEPTR      RelOp( FrontRun& front, TREEPTR, TOKEN, TREEPTR );
extern TREEPTR      ShiftOp( FrontRun& front, TREEPTR, TOKEN, TREEPTR );
extern TYPEPTR      TernType( FrontRun& front, TREEPTR, TREEPTR );
extern TYPEPTR      TypeOf( TREEPTR );
extern TREEPTR      UComplement( FrontRun& front, TREEPTR );
extern TREEPTR      UMinus( FrontRun& front, TREEPTR );
extern DATA_TYPE    BinExprType( TYPEPTR, TYPEPTR );
extern DATA_TYPE    DataTypeOf( TYPEPTR );
extern bool         IsFuncPtr( TYPEPTR );
extern TREEPTR      FixupAss( FrontRun& front, TREEPTR opnd, TYPEPTR newtyp );
extern pointer_class ExprTypeClass( FrontRun& front, TYPEPTR typ );
extern TREEPTR      LCastAdj( FrontRun& front, TREEPTR tree );

/* cmodel */
extern char         *Define_Macro( FrontRun& front, const char * );
extern char         *Define_UserMacro( FrontRun& front, const char *, bool );
extern void         PreDefine_Macro( FrontRun& front, const char * );
extern void         AddUndefMacro( FrontRun& front, const char * );
extern void         InitModInfo( FrontRun& front );
extern void         MiscMacroDefs( FrontRun& front );

/* cmsg.c */
extern char const   *CGetMsgStr( FrontRun& front, msg_codes msgnum );
extern void         CGetMsg( FrontRun& front, char *msgbuf, msg_codes msgnum );
extern void         InitMsg( FrontRun& front );
extern void         FiniMsg( FrontRun& front );
extern char const   *UsageText( FrontRun& front );   // GET INTERNATIONAL USAGE TEXT
//extern msg_type     CGetMsgType( msg_codes msgnum );
extern char const   *CGetMsgPrefix( msg_codes msgnum );
extern unsigned     GetMsgIndex( msg_codes msgnum );

/* coptions */
extern void         GenCOptions( FrontRun& front, char ** );
extern bool         MergeIncludeFromEnv( FrontRun& front, const char *env );

/* cpragma */
extern void         CPragmaInit( FrontRun& front );
extern void         CPragmaFini( FrontRun& front );
extern void         SetToggleFlag( FrontRun& front, char const *name, int func, bool push );
extern void         CPragma( FrontRun& front );
extern textsegment  *LkSegName( FrontRun& front, const char *, const char * );
extern textsegment  *NewTextSeg( FrontRun& front, const char *, const char *, const char * );
extern void         PragmaInit( FrontRun& front );
extern void         PragmaFini( void );
extern void         PragmaAuxEnding( FrontRun& front );
extern void         PragObjNameInfo( FrontRun& front, char ** );
extern bool         PragRecogId( FrontRun& front, const char * );
extern bool         PragRecogName( FrontRun& front, const char * );
extern hw_reg_set   PragRegList( FrontRun& front );
extern int          PragRegIndex( const char *, const char *, bool );
extern int          PragRegNumIndex( const char *name, int max_reg );
extern void         PragRegNameErr( FrontRun& front, const char *regname );
extern hw_reg_set   *PragManyRegSets( FrontRun& front );
extern TOKEN        PragRegSet( FrontRun& front );
extern void         CheckPragmas( void );
extern void         CreateAux( FrontRun& front, const char * );
extern void         CreateAuxInlineFunc( FrontRun& front, bool too_many_bytes );
extern void         SetCurrInfo( FrontRun& front, const char * );
extern void         XferPragInfo( FrontRun& front, const char *, const char * );
extern bool         GetMsgNum( const char *str, msg_codes *val );
extern void         WarnEnableDisable( FrontRun& front, bool enabled, msg_codes msgnum );
extern void         AddLibraryName( FrontRun& front, const char *, char );
extern void         AddExtRefN( FrontRun& front, const char * );
extern void         AddExtRefS( FrontRun& front, SYM_HANDLE );
extern unsigned     VerifyPackAmount( unsigned amount );
extern void         SetPackAmount( FrontRun& front, unsigned amount );
extern bool         GetPragmaAuxAliasInfo( FrontRun& front );
extern aux_info     *PragmaAuxAlias( FrontRun& front, const char *name );
extern bool         GetPragmaAuxAlias( FrontRun& front );
extern const char   *SkipUnderscorePrefix( FrontRun& front, const char *str );

/* cprag??? */
extern void         AsmStmt( FrontRun& front );
extern bool         AsmInsertFixups( FrontRun& front, aux_info *info );
extern void         AsmUsesAuto( aux_info *info );
extern void         PragAux( FrontRun& front );
extern hw_reg_set   PragRegName( FrontRun& front, const char * );
extern hw_reg_set   PragReg( FrontRun& front );

/* cpurge */
extern void         InitPurge( void );
extern void         PurgeMemory( FrontRun& front );

/* cscan */
extern void         NewLineStartPos( FCB *srcfile );
extern void         ScanInit( FrontRun& front );
extern bool         InitPPScan( FrontRun& front );
extern void         FiniPPScan( FrontRun& front, bool );
extern id_hash_idx  CalcHashID( const char * );
extern mac_hash_idx CalcHashMacro( const char * );
extern str_hash_idx CalcStringHash( STR_HANDLE lit );
extern void         SkipAhead( FrontRun& front );
extern TOKEN        ScanToken( FrontRun& front );
extern void         ReScanInit( FrontRun& front, const char * );
extern TOKEN        ReScanToken( FrontRun& front );
extern const char   *ReScanPos( FrontRun& front );
extern TOKEN        KwLookup( FrontRun& front, const char *, size_t );
extern TOKEN        NextToken( FrontRun& front );
extern TOKEN        PPNextToken( FrontRun& front );

/* csizeof */
extern target_size  SizeOfArg( FrontRun& front, TYPEPTR );

/* cstats */
extern void         InitStats( FrontRun& front );
extern void         PrintStats( FrontRun& front );

/* cstmt */
extern void         LookAhead( FrontRun& front );
extern void         Statement( FrontRun& front );
extern void         AddStmt( FrontRun& front, TREEPTR );
extern void         GenFunctionNode( FrontRun& front, SYM_HANDLE );
extern LABEL_INDEX  NextLabel( FrontRun& front );
extern void         StmtInit( FrontRun& front );

/* cstring */
extern void         FreeLiteral( FrontRun& front, STR_HANDLE );
extern STR_HANDLE   GetLiteral( FrontRun& front );
extern void         LoadUnicodeTable( FrontRun& front, unsigned );
extern void         StringInit( FrontRun& front );
extern TREEPTR      StringLeaf( FrontRun& front, string_flags );

/* csym */
extern void         SymInit( FrontRun& front );
extern void         SpcSymInit( FrontRun& front );
extern void         SymFini( FrontRun& front );
extern void         SymCreate( FrontRun& front, SYMPTR, const char * );
extern SYM_HANDLE   SegSymbol( FrontRun& front, const char *, segment_id );
extern SYM_HANDLE   SpcSymbol( FrontRun& front, const char *, TYPEPTR, stg_classes );
extern SYM_HANDLE   SymAdd( FrontRun& front, id_hash_idx, SYMPTR );
extern SYM_HANDLE   SymAddL0( FrontRun& front, id_hash_idx, SYMPTR );
extern SYM_HANDLE   SymLook( FrontRun& front, id_hash_idx, const char * );
extern SYM_HANDLE   Sym0Look( FrontRun& front, id_hash_idx, const char * );
extern SYM_HANDLE   SymLookTypedef( FrontRun& front, id_hash_idx, const char *, SYMPTR );
extern void         SymGet( FrontRun& front, SYMPTR, SYM_HANDLE );
extern SYMPTR       SymGetPtr( FrontRun& front, SYM_HANDLE );
extern void         SymReplace( FrontRun& front, SYMPTR, SYM_HANDLE );
extern void         EndBlock( FrontRun& front );
extern SYM_HANDLE   MakeFunction( FrontRun& front, const char *, TYPEPTR );
extern SYM_HANDLE   MakeNewDotSym( FrontRun& front, SYMPTR, char, TYPEPTR, stg_classes );
extern LABELPTR     LkLabel( FrontRun& front, const char * );
extern void         FreeLabels( FrontRun& front );
extern XREFPTR      NewXref( FrontRun& front, XREFPTR );
extern void         FreeXrefs( FrontRun& front, XREFPTR );
extern unsigned     SymGetNumSyms( FrontRun& front );
extern unsigned     SymGetNumSpecialSyms( FrontRun& front );
extern SYM_HANDLE   SymGetFirst( void );
extern SYM_HANDLE   SymGetNext( FrontRun& front, SYM_HANDLE sym_handle );
extern void         SymsPurge( FrontRun& front );
extern void         SetNextSymHandle( FrontRun& front, unsigned val );

/* ctimepc */
extern void         TimeInit( FrontRun& front );

/* ctype.c */
extern void         CTypeInit( FrontRun& front );
extern void         InitTypeHashTables( FrontRun& front );
extern void         SetSignedChar( FrontRun& front );
extern TYPEPTR      GetType( FrontRun& front, DATA_TYPE );
extern TYPEPTR      ArrayNode( FrontRun& front, TYPEPTR );
extern TYPEPTR      FuncNode( FrontRun& front, TYPEPTR, type_modifiers, TYPEPTR * );
extern TYPEPTR      TypeDefault( FrontRun& front );
extern TYPEPTR      PtrNode( FrontRun& front, TYPEPTR, type_modifiers, segment_id );
extern TYPEPTR      BPtrNode( FrontRun& front, TYPEPTR, type_modifiers, segment_id, SYM_HANDLE, BASED_KIND );
extern TYPEPTR      TypeNode( FrontRun& front, DATA_TYPE, TYPEPTR );
extern type_modifiers TypeQualifier( FrontRun& front );
extern void         TypeSpecifier( FrontRun& front, decl_info *info );
extern void         FullDeclSpecifier( FrontRun& front, decl_info *info );
extern TAGPTR       NullTag( FrontRun& front );
extern TAGPTR       TagLookup( FrontRun& front, const char *name );
extern void         FreeTags( FrontRun& front );
extern target_size  TypeSize( FrontRun& front, TYPEPTR );
extern target_size  TypeSizeEx( FrontRun& front, TYPEPTR, bitfield_width *pFieldWidth );
extern TYPEPTR      GetIntTypeBySize( FrontRun& front, target_size size, bool sign, bool exact );
extern TAGPTR       VfyNewTag( FrontRun& front, TAGPTR, DATA_TYPE );
extern void         VfyNewSym( FrontRun& front, id_hash_idx, const char * );
extern align_type   GetTypeAlignment( FrontRun& front, TYPEPTR );
extern void         TypesPurge( void );
extern void         AddTypeHash( FrontRun& front, TYPEPTR );
extern void         AddPtrTypeHash( FrontRun& front, TYPEPTR );
extern void         AdjModsTypeNode( FrontRun& front, TYPEPTR *ptyp, type_modifiers decl_mod, SYMPTR sym );

/* cusage */
extern void         CUsage( FrontRun& front );

/* cutil */
extern void         CErrSymName( FrontRun& front, int, SYMPTR, SYM_HANDLE );
extern void         Expecting( FrontRun& front, const char * );
extern bool         ExpectingToken( FrontRun& front, TOKEN );
extern void         ExpectingAfter( FrontRun& front, TOKEN, TOKEN );
extern bool         ExpectingConstant( FrontRun& front );
extern void         ExpectEndOfLine( FrontRun& front );
extern void         ExpectIdentifier( FrontRun& front );
extern void         ExpectString( FrontRun& front );
extern void         ExpectStructUnionTag( FrontRun& front );
extern void         MustRecog( FrontRun& front, TOKEN );
extern SYM_NAMEPTR  SymName( FrontRun& front, SYMPTR, SYM_HANDLE );

/* dwarf */
extern void         DwarfBrowseEmit( FrontRun& front );

/* ftoa */
extern char         *ftoa( FrontRun& front, FLOATVAL * );

/* jis2unic */
extern unsigned     JIS2Unicode( unsigned );

/* pchdr.c */
extern bool         UsePreCompiledHeader( FrontRun& front, const char * );
extern void         InitBuildPreCompiledHeader( FrontRun& front );
extern void         BuildPreCompiledHeader( FrontRun& front, const char * );
extern void         FreePreCompiledHeader( FrontRun& front );

/* watcom */
extern void         CBanner( FrontRun& front );

/* cstmt.c */
extern SYM_HANDLE   GetBlockSymList( FrontRun& front );
extern void         InitStmt( FrontRun& front );
extern void         SwitchPurge( FrontRun& front );
