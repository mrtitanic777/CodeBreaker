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
#include "cerrs.h"
#include "cmsg.h"
#include "cgswitch.h"
#include "pragdefn.h"


#define MAX_LEVEL       1024
#define USER_LIB_PRIO   '9'
#define ERRLIMIT_NOMAX  ((unsigned)-1)

#define MSGIDX_INVALID  (-1)
#define IS_MSGIDX_INVALID(m)  ((int)(m) == MSGIDX_INVALID)

#define CheckEqSymLevel(p)  ((p)->level == (id_level_type)cvars_state().SymLevel)
#define CheckLtSymLevel(p)  ((p)->level < (id_level_type)cvars_state().SymLevel)

#ifdef DEVBUILD
    #define __xstr(x)   #x
    #define __location  " (" __FILE__ "," __xstr(__LINE__) ")"
#endif

/* Macros to skip all typedefs and arrive at the underlying type */
#define SKIP_TYPEDEFS( typeptr )                    \
    while( typeptr->decl_type == TYP_TYPEDEF ) {    \
        typeptr = typeptr->object;                  \
    }
#define SKIP_DUMMY_TYPEDEFS( typeptr )              \
    while( typeptr->decl_type == TYP_TYPEDEF && (typeptr->type_flags & TF2_DUMMY_TYPEDEF) ) { \
        typeptr = typeptr->object;                  \
    }
#define SKIP_ENUM( typeptr )                        \
    if( typeptr->decl_type == TYP_ENUM ) {          \
        typeptr = typeptr->object;                  \
    }
#define SKIP_ARRAYS( typeptr )                      \
    while( typeptr->decl_type == TYP_ARRAY ) {      \
        typeptr = typeptr->object;                  \
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

#if _CPU == 370
global char         *AuxName;
#endif


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
    bool_ Check_global_prototype;
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
#if _INTEL_CPU
    target_size DataThreshold;   /* sizeof(obj) > this ==> separate segment */
#endif
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
    int (*NextChar)( void );
    void (*UnGetChar)( int c );
    int (*GetCharCheck)( int );
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
    bool_ PrintWhiteSpace;   // also refered from cmac2.c
    call_list *CallNodeList;
    LABEL_INDEX LabelIndex;
    SYM_LIST_HEADS *SymListHeads;
    unsigned SymTypedef;
    TAGPTR TagHash[ID_HASH_SIZE + 1];
    unsigned NodeCount;
};
inline CvarsState& cvars_state() { return cbrun_part<CvarsState>(); }

#endif

//================= Function Prototypes ========================

/* casian.c */
extern void         SetDBChar(int);

/* ccheck.c */
extern bool_         CheckCompatibleFunction( TYPEPTR typ1, TYPEPTR typ2, bool_ topLevelCheck );
extern bool_         CheckCompatibleLanguage( type_modifiers typ1, type_modifiers typ2 );
extern void         CheckTernary( TYPEPTR typ1, TYPEPTR typ2 );
extern void         CheckCallParms( void );
extern void         ParmCountMismatch( void );
extern void         CheckRetType( TREEPTR );
extern void         CompatiblePtrType( TYPEPTR, TYPEPTR, TOKEN );
extern bool_         CheckIdenticalType( TYPEPTR, TYPEPTR );
extern bool_         VerifyType( TYPEPTR, TYPEPTR, SYMPTR );
extern TYPEPTR      SkipTypeFluff( TYPEPTR typ );
extern bool_         CheckZeroConstant( TREEPTR tree );
extern bool_         CheckZero( TREEPTR tree );
extern bool_         CheckAssignBits( uint64 *val, unsigned width, bool_ mask );
extern bool_         CheckAssignRange( TYPEPTR typ1, TREEPTR opnd2 );
extern void         CheckParmAssign( TYPEPTR typ1, TREEPTR opnd2, int parm_num, bool_ asgn_check );

/* ccmain.c */
extern void         FreeRDir( void );
extern void         FrontEndInit( bool_ reuse );
extern bool_         FrontEnd( char ** );
extern void         FrontEndFini( void );
extern void         CppComment( int );
extern bool_         CppPrinting( void );
extern void         CppPuts( const char * );
extern void         CppPutsQuoted( const char * );
extern void         SetPPWidth( unsigned );
extern void         CppPrtChar( int );
extern void         CppPrtToken( TOKEN );
extern bool_         OpenSrcFile( const char *, src_file_type );
extern void         CloseSrcFile( FCB * );
extern void         OpenDefFile( void );
extern FILE         *OpenBrowseFile( void );
extern void         CloseFiles( void );
extern void         FreeFNames( void );
extern char         *ErrFileName( void );
extern char         *DepFileName( void );
extern char         *ObjFileName( void );
extern char         *CppFileName( void );
extern char         *ForceSlash( char *, char );
extern char         *GetSourceDepName( void );
extern FNAMEPTR     NextDependency( FNAMEPTR );
extern void         PrtfFilenameErr( const char *filename, src_file_type typ, bool_ print_error );

extern FNAMEPTR     AddFlist( char const * );
extern FNAMEPTR     FileIndexToFName( unsigned );
extern char         *FNameFullPath( FNAMEPTR flist );
extern char         *FileIndexToCorrectName( unsigned file_index );
extern void         SrcFileReadOnlyDir( char const *dirs );
extern void         SrcFileReadOnlyFile( char const *file );
extern bool_         SrcFileInRDir( FNAMEPTR flist );
extern void         SrcFileIncludeAlias( const char *alias_name, const char *real_name, bool_ is_lib );
extern int          SrcFileTime( char const *,time_t * );
extern void         SetSrcFNameOnce( void );
extern TOKEN        GetNextToken( void );
extern void         CppEmitPoundLine( unsigned, const char *, bool_ );

extern void         AddIncFile( INCFILE * );
extern void         AddIncFileList( const char *filename );
extern void         FreeIncFileList( void );

/* cdata.c */
extern void         InitGlobalVars( void );

/* cdebug.c */
extern dbug_type    DBType( TYPEPTR );
extern void         EmitDBType( void );

extern void         ParsePgm( void );
extern bool_         CheckFuncMain( const char *name );
extern void         AdjParmType( SYMPTR sym );
extern void         Check_Struct_Union_Enum( TYPEPTR );
extern void         Declarator( SYMPTR sym, type_modifiers mod, TYPEPTR typ, decl_state state );
extern bool_         DeclList( SYM_HANDLE * );
extern FIELDPTR     FieldDecl( TYPEPTR typ, type_modifiers mod, decl_state state );
extern TYPEPTR      TypeName( void );

/* cdecl2.c */
extern void         InvDecl( void );
extern parm_list    *NewParm( TYPEPTR, parm_list * );
extern TYPEPTR      *MakeParmList( parm_list *, bool_ );
extern FIELDPTR     FieldCreate( const char *name );
extern bool_         LoopDecl( SYM_HANDLE *sym_head );

/* cdinit */
extern void         InitDataQuads( void );
extern void         FreeDataQuads( void );
extern bool_         DataQuadsAvailable( void );
extern void         *StartDataQuadAccess( void );
extern void         EndDataQuadAccess( void * );
extern DATA_QUAD    *NextDataQuad( void );
extern void         InitSymData( TYPEPTR, TYPEPTR, int );
extern void         StaticInit( SYMPTR, SYM_HANDLE );
extern void         VarDeclEquals( SYMPTR, SYM_HANDLE );

/* cdump */
extern void         DumpFuncDefn( void );
extern void         SymDump( void );
extern char         *DiagGetTypeName( TYPEPTR typ );

/* cenum */
extern TYPEPTR      EnumDecl( type_modifiers );
extern ENUMPTR      EnumLookup( id_hash_idx, const char * );
extern void         EnumInit( void );
extern void         FreeEnums( void );

/* cerror.c */
extern void         CErr1( msg_codes msgnum );
extern void         CErr2( msg_codes msgnum, int );
extern void         CErr2p( msg_codes msgnum, const char * );
extern void         CErr3p( msg_codes msgnum, const char *, const char * );
extern void         CErr4p( msg_codes msgnum, const char *, const char *, const char * );
extern void         CErrP1( int parm_num, msg_codes msgnum );
extern void         SetErrLoc( source_loc * );
extern void         InitErrLoc( void );
extern void         CWarn1( msg_codes msgnum );
extern void         CWarn2( msg_codes msgnum, int );
extern void         CWarn2p( msg_codes msgnum, const char * );
extern void         CWarnP1( int parm_num, msg_codes msgnum );
extern void         PCHNote( msg_codes msgnum, ... );
extern void         CInfoMsg( msg_codes msgnum, ... );
extern void         CSuicide( void );
extern void         OpenErrFile( void );
extern void         FmtCMsg( char *buff, cmsg_info *info );
extern void         SetDiagSymbol( SYMPTR sym, SYM_HANDLE handle );
extern void         SetDiagEnum( ENUMPTR );
extern void         SetDiagMacro( MEPTR );
extern void         SetDiagType1( TYPEPTR typ_source );
extern void         SetDiagType2( TYPEPTR typ_target, TYPEPTR typ_source );
extern void         SetDiagType3( TYPEPTR typ_first, TYPEPTR typ_second, TOKEN opr );
extern void         SetDiagPop( void );

/* cexpr.c */
extern void         ExprInit( void );
extern void         CheckCallNode( TREEPTR tree );
extern TREEPTR      Expr( void );
extern TREEPTR      AddrExpr( void );
extern TREEPTR      BoolExpr( TREEPTR );
extern TREEPTR      CommaExpr( void );
extern bool_         ConstExprAndType( const_val * );
extern TREEPTR      SingleExpr( void );
extern TREEPTR      IntLeaf( target_int );
extern TREEPTR      RValue( TREEPTR );
extern TREEPTR      LongLeaf( target_long );
extern TREEPTR      UIntLeaf( target_uint );
extern TREEPTR      VarLeaf( SYMPTR, SYM_HANDLE );
extern TREEPTR      BasedPtrNode( TYPEPTR, TREEPTR );
extern bool_         IsLValue( TREEPTR );
extern op_flags     OpFlags( type_modifiers  flags );
extern type_modifiers FlagOps( op_flags ops );
extern FIELDPTR     SearchFields( TYPEPTR *class_typ, target_size *field_offset, const char *name );

/* cfold.c */
extern int64        LongValue64( TREEPTR leaf );
extern void         CastConstValue( TREEPTR, DATA_TYPE );
extern void         CastConstNode( TREEPTR leaf, TYPEPTR newtyp );
extern void         DoConstFold( TREEPTR );
extern void         FoldExprTree( TREEPTR );

/* cgen.c */
extern void         DoCompile( void );
extern void         EmitInit( void );
extern void         EmitAbort( void );
extern target_size  EmitBytes( STR_HANDLE );
extern void         GenInLineFunc( SYM_HANDLE sym_handle );
extern bool_         IsInLineFunc( SYM_HANDLE sym_handle );

/* cgendata */
extern target_size  GetDQuadPointerSize( enum quad_flags dq_flags );
extern void         EmitDataQuads( void );
extern void         EmitZeros( target_size );
extern void         AlignIt( TYPEPTR );

/* cgetch */
extern int          GetNextChar( void );
extern void         GetNextCharUndo( int );
extern int          GetCharCheckFile( int );
extern int          getCharAfterBackSlash( void );
extern bool_         OpenFCB( FILE *fp, const char *filename, src_file_type typ );
extern void         CloseFCB( FCB * );
extern void         SrcPurge( void );
extern void         InitIncFile( void );

/* cinfo.c */
extern void         SegInit( void );
extern void         SegFini( void );
extern segment_id   AddSegName( const char *, const char *, int );
extern segment_id   DefThreadSeg( void );
extern void         EmitSegLabels( void );
extern void         FiniSegLabels( void );
extern void         FiniSegBacks( void );
extern void         SetSegs( void );
extern SYM_HANDLE   SegSymHandle( segment_id );
extern void         SetFuncSegment( SYMPTR, segment_id );
extern void         SetFarHuge( SYMPTR, bool_ );
extern char         *SegClassName( segment_id );
#if _INTEL_CPU
extern hw_reg_set   *SegPeggedReg( segment_id );
#endif
extern void         SetSegment( SYMPTR );
extern void         SetSegAlign( SYMPTR );
extern void         AssignSeg( SYMPTR );
extern segment_id   SymSegId( SYMPTR sym );
extern void         SetSegSymHandle( SYM_HANDLE sym_handle, segment_id segid );
extern void         ImportNearSegIdInit( void );
extern void         ImportSegIdInit( void );

/* cintmain.c */
extern void         ConsErrMsg( cmsg_info *info );
extern void         ConsErrMsgVerbatim( char const *line );
extern void         BannerMsg( char const *line );
extern void         DebugMsg( char const *line );
extern void         NoteMsg( char const *line );
extern void         ConBlip( void );
extern void         MyExit( int ret );

/* cmac1.c */
extern void         InitTokenBuf( size_t );
extern void         MacroInit( void );
extern void         MacroAddComp( void );
extern void         MacroFini( void );
extern void         MacroPurge( void );
extern void         DumpAllMacros( void );
extern TOKEN        GetMacroToken( void );
extern TOKEN        SpecialMacro( MEPTR );
extern void         DoMacroExpansion( MEPTR );
extern void         InsertReScanPragmaTokens( const char *pragma );
extern void         InsertToken( TOKEN token, const char *str );

/* cmac2.c */
extern TOKEN        CheckControl( void );
extern bool_         MacroDel( const char *name );
extern void         CppStackInit( void );
extern void         CppStackFini( void );
extern MEPTR        MacroScan( void );
extern TOKEN        Process_Pragma( void );

/* cmath.c */
extern TREEPTR      AddOp( TREEPTR, TOKEN, TREEPTR );
extern TREEPTR      InitAsgn( TYPEPTR, TREEPTR );
extern TREEPTR      AsgnOp( TREEPTR, TOKEN, TREEPTR );
extern TREEPTR      BinOp( TREEPTR, TOKEN, TREEPTR );
extern bool_         IsPtrConvSafe( TREEPTR, TYPEPTR, TYPEPTR );
extern TREEPTR      CnvOp( TREEPTR, TYPEPTR, bool_ );
extern TREEPTR      FlowOp( TREEPTR, opr_code, TREEPTR );
extern void         CheckConst( TREEPTR );
extern TREEPTR      IntOp( TREEPTR, TOKEN, TREEPTR );
extern TREEPTR      RelOp( TREEPTR, TOKEN, TREEPTR );
extern TREEPTR      ShiftOp( TREEPTR, TOKEN, TREEPTR );
extern TYPEPTR      TernType( TREEPTR, TREEPTR );
extern TYPEPTR      TypeOf( TREEPTR );
extern TREEPTR      UComplement( TREEPTR );
extern TREEPTR      UMinus( TREEPTR );
extern DATA_TYPE    BinExprType( TYPEPTR, TYPEPTR );
extern DATA_TYPE    DataTypeOf( TYPEPTR );
extern bool_         IsFuncPtr( TYPEPTR );
extern TREEPTR      FixupAss( TREEPTR opnd, TYPEPTR newtyp );
extern pointer_class ExprTypeClass( TYPEPTR typ );
extern TREEPTR      LCastAdj( TREEPTR tree );

/* cmodel */
extern char         *Define_Macro( const char * );
extern char         *Define_UserMacro( const char *, bool_ );
extern void         PreDefine_Macro( const char * );
extern void         AddUndefMacro( const char * );
extern void         InitModInfo( void );
extern void         MiscMacroDefs( void );

/* cmsg.c */
extern char const   *CGetMsgStr( msg_codes msgnum );
extern void         CGetMsg( char *msgbuf, msg_codes msgnum );
extern void         InitMsg( void );
extern void         FiniMsg( void );
extern char const   *UsageText( void );   // GET INTERNATIONAL USAGE TEXT
//extern msg_type     CGetMsgType( msg_codes msgnum );
extern char const   *CGetMsgPrefix( msg_codes msgnum );
extern unsigned     GetMsgIndex( msg_codes msgnum );

/* coptions */
extern void         GenCOptions( char ** );
extern bool_         MergeIncludeFromEnv( const char *env );

/* cpragma */
extern void         CPragmaInit( void );
extern void         CPragmaFini( void );
extern void         SetToggleFlag( char const *name, int func, bool_ push );
extern void         CPragma( void );
extern textsegment  *LkSegName( const char *, const char * );
extern textsegment  *NewTextSeg( const char *, const char *, const char * );
extern void         PragmaInit( void );
extern void         PragmaFini( void );
extern void         PragmaAuxEnding( void );
extern void         PragObjNameInfo( char ** );
extern bool_         PragRecogId( const char * );
extern bool_         PragRecogName( const char * );
extern hw_reg_set   PragRegList( void );
extern int          PragRegIndex( const char *, const char *, bool_ );
extern int          PragRegNumIndex( const char *name, int max_reg );
extern void         PragRegNameErr( const char *regname );
extern hw_reg_set   *PragManyRegSets( void );
extern TOKEN        PragRegSet( void );
extern void         CheckPragmas( void );
extern void         CreateAux( const char * );
extern void         CreateAuxInlineFunc( bool_ too_many_bytes );
extern void         SetCurrInfo( const char * );
extern void         XferPragInfo( const char *, const char * );
extern bool_         GetMsgNum( const char *str, msg_codes *val );
extern void         WarnEnableDisable( bool_ enabled, msg_codes msgnum );
extern void         AddLibraryName( const char *, char );
extern void         AddExtRefN( const char * );
extern void         AddExtRefS( SYM_HANDLE );
extern unsigned     VerifyPackAmount( unsigned amount );
extern void         SetPackAmount( unsigned amount );
extern bool_         GetPragmaAuxAliasInfo( void );
extern aux_info     *PragmaAuxAlias( const char *name );
extern bool_         GetPragmaAuxAlias( void );
extern const char   *SkipUnderscorePrefix( const char *str );

/* cprag??? */
extern void         AsmStmt( void );
extern bool_         AsmInsertFixups( aux_info *info );
extern void         AsmUsesAuto( aux_info *info );
extern void         PragAux( void );
extern hw_reg_set   PragRegName( const char * );
extern hw_reg_set   PragReg( void );

/* cpurge */
extern void         InitPurge( void );
extern void         PurgeMemory( void );

/* cscan */
extern void         NewLineStartPos( FCB *srcfile );
extern void         ScanInit( void );
extern bool_         InitPPScan( void );
extern void         FiniPPScan( bool_ );
extern id_hash_idx  CalcHashID( const char * );
extern mac_hash_idx CalcHashMacro( const char * );
extern str_hash_idx CalcStringHash( STR_HANDLE lit );
extern void         SkipAhead( void );
extern TOKEN        ScanToken( void );
extern void         ReScanInit( const char * );
extern TOKEN        ReScanToken( void );
extern const char   *ReScanPos( void );
extern TOKEN        KwLookup( const char *, size_t );
extern TOKEN        NextToken( void );
extern TOKEN        PPNextToken( void );

/* csizeof */
extern target_size  SizeOfArg( TYPEPTR );

/* cstats */
extern void         InitStats( void );
extern void         PrintStats( void );

/* cstmt */
extern void         LookAhead( void );
extern void         Statement( void );
extern void         AddStmt( TREEPTR );
extern void         GenFunctionNode( SYM_HANDLE );
extern LABEL_INDEX  NextLabel( void );
extern void         StmtInit( void );

/* cstring */
extern void         FreeLiteral( STR_HANDLE );
extern STR_HANDLE   GetLiteral( void );
extern void         LoadUnicodeTable( unsigned );
extern void         StringInit( void );
extern TREEPTR      StringLeaf( string_flags );

/* csym */
extern void         SymInit( void );
extern void         SpcSymInit( void );
extern void         SymFini( void );
extern void         SymCreate( SYMPTR, const char * );
extern SYM_HANDLE   SegSymbol( const char *, segment_id );
extern SYM_HANDLE   SpcSymbol( const char *, TYPEPTR, stg_classes );
extern SYM_HANDLE   SymAdd( id_hash_idx, SYMPTR );
extern SYM_HANDLE   SymAddL0( id_hash_idx, SYMPTR );
extern SYM_HANDLE   SymLook( id_hash_idx, const char * );
extern SYM_HANDLE   Sym0Look( id_hash_idx, const char * );
extern SYM_HANDLE   SymLookTypedef( id_hash_idx, const char *, SYMPTR );
extern void         SymGet( SYMPTR, SYM_HANDLE );
extern SYMPTR       SymGetPtr( SYM_HANDLE );
extern void         SymReplace( SYMPTR, SYM_HANDLE );
extern void         EndBlock( void );
extern SYM_HANDLE   MakeFunction( const char *, TYPEPTR );
extern SYM_HANDLE   MakeNewDotSym( SYMPTR, char, TYPEPTR, stg_classes );
extern LABELPTR     LkLabel( const char * );
extern void         FreeLabels( void );
extern XREFPTR      NewXref( XREFPTR );
extern void         FreeXrefs( XREFPTR );
extern unsigned     SymGetNumSyms( void );
extern unsigned     SymGetNumSpecialSyms( void );
extern SYM_HANDLE   SymGetFirst( void );
extern SYM_HANDLE   SymGetNext( SYM_HANDLE sym_handle );
extern void         SymsPurge( void );
extern void         SetNextSymHandle( unsigned val );

/* ctimepc */
extern void         TimeInit( void );

/* ctype.c */
extern void         CTypeInit( void );
extern void         InitTypeHashTables( void );
extern void         SetSignedChar( void );
extern TYPEPTR      GetType( DATA_TYPE );
extern TYPEPTR      ArrayNode( TYPEPTR );
extern TYPEPTR      FuncNode( TYPEPTR, type_modifiers, TYPEPTR * );
extern TYPEPTR      TypeDefault( void );
extern TYPEPTR      PtrNode( TYPEPTR, type_modifiers, segment_id );
extern TYPEPTR      BPtrNode( TYPEPTR, type_modifiers, segment_id, SYM_HANDLE, BASED_KIND );
extern TYPEPTR      TypeNode( DATA_TYPE, TYPEPTR );
extern type_modifiers TypeQualifier( void );
extern void         TypeSpecifier( decl_info *info );
extern void         FullDeclSpecifier( decl_info *info );
extern TAGPTR       NullTag( void );
extern TAGPTR       TagLookup( const char *name );
extern void         FreeTags( void );
extern target_size  TypeSize( TYPEPTR );
extern target_size  TypeSizeEx( TYPEPTR, bitfield_width *pFieldWidth );
extern TYPEPTR      GetIntTypeBySize( target_size size, bool_ sign, bool_ exact );
extern TAGPTR       VfyNewTag( TAGPTR, DATA_TYPE );
extern void         VfyNewSym( id_hash_idx, const char * );
extern align_type   GetTypeAlignment( TYPEPTR );
extern void         TypesPurge( void );
extern void         AddTypeHash( TYPEPTR );
extern void         AddPtrTypeHash( TYPEPTR );
extern void         AdjModsTypeNode( TYPEPTR *ptyp, type_modifiers decl_mod, SYMPTR sym );

/* cusage */
extern void         CUsage( void );

/* cutil */
extern void         CErrSymName( int, SYMPTR, SYM_HANDLE );
extern void         Expecting( const char * );
extern bool_         ExpectingToken( TOKEN );
extern void         ExpectingAfter( TOKEN, TOKEN );
extern bool_         ExpectingConstant( void );
extern void         ExpectEndOfLine( void );
extern void         ExpectIdentifier( void );
extern void         ExpectString( void );
extern void         ExpectStructUnionTag( void );
extern void         MustRecog( TOKEN );
extern SYM_NAMEPTR  SymName( SYMPTR, SYM_HANDLE );

/* dwarf */
extern void         DwarfBrowseEmit( void );

/* ftoa */
extern char         *ftoa( FLOATVAL * );

/* jis2unic */
extern unsigned     JIS2Unicode( unsigned );

/* pchdr.c */
extern bool_         UsePreCompiledHeader( const char * );
extern void         InitBuildPreCompiledHeader( void );
extern void         BuildPreCompiledHeader( const char * );
extern void         FreePreCompiledHeader( void );

/* watcom */
extern void         CBanner( void );

/* cstmt.c */
extern SYM_HANDLE   GetBlockSymList( void );
extern void         InitStmt( void );
extern void         SwitchPurge( void );
