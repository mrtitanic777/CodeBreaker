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
* Description:  Generic (target independent) pragma processing.
*
****************************************************************************/


#include <ctype.h>
#include "cvars.h"
#include "i64.h"
#include "cgswitch.h"
#include "pdefn2.h"
#include "caux.h"
#include "cfeinfo.h"
#include "asmstmt.h"
#include "scan.h"
#include "toggles.h"
#ifdef DEVBUILD
    #include "togglesd.h"
#endif

#include "clibext.h"


#define pragmaNameRecog(what)   (strcmp(cvars_state().Buffer, what) == 0)

typedef enum {
    #define pick(a,b,c,d)   a,
    #include "auxinfo.h"
    #undef pick
    M_SIZE
} magic_words;

typedef struct prag_stack {
    struct prag_stack   *next;
    unsigned            value;
} prag_stack;

#ifdef DEVBUILD
pragma_dbg_toggles      PragmaDbgToggles;
#endif


struct magic_words_data {
    const char      *name;
    aux_info        *info;
};
namespace {

/* the statics of this file, one set for each run of the compiler */
struct CpragmaState {
#define pick( x ) prag_stack *TOGGLE_STK( x );
#include "togdef.h"
#undef pick
    prag_stack *TOGGLE_STK( pack );
    prag_stack *TOGGLE_STK( enum );
    prag_stack *FreePrags;
    int AsmFuncNum;
    struct magic_words_data magicWords[10] = {
    #define pick(a,b,c,d) { b, d },
    #include "auxinfo.h"
    #undef pick
    };
};

}  // namespace

static inline CpragmaState& cpragma_state() { return cbrun_part<CpragmaState>(); }

/* the stacks of pushed toggles are members of the state, which the macro that named the statics now names */
#undef TOGGLE_STK
#define TOGGLE_STK( x )     cpragma_state().HeadToggles_tog_##x


static prag_stack *stackPush( prag_stack **header, prag_stack *element )
/**********************************************************************/
{
    element->next = *header;
    *header = element;
    return( element );
}

static prag_stack *stackPop( prag_stack **header )
/************************************************/
{
    prag_stack  *element;

    element = *header;
    if( element != NULL ) {
        *header = element->next;
    }
    return( element );
}

static void freeStack( prag_stack **header )
/******************************************/
{
    prag_stack  *element;

    while( (element = *header) != NULL ) {
        *header = element->next;
        CMemFree( element );
    }
}

static void pushPrag( prag_stack **header, unsigned value )
/*********************************************************/
{
    prag_stack *stack_entry;

    stack_entry = stackPop( &cpragma_state().FreePrags );
    if( stack_entry == NULL ) {
        stack_entry = static_cast<prag_stack *>(CMemAlloc( sizeof( *stack_entry ) ));
    }
    stack_entry->value = value;
    stackPush( header, stack_entry );
}

static bool popPrag( prag_stack **header, unsigned *pvalue )
/**********************************************************/
{
    prag_stack *stack_entry;

    stack_entry = stackPop( header );
    if( stack_entry != NULL ) {
        if( pvalue != NULL ) {
            *pvalue = stack_entry->value;
        }
        stackPush( &cpragma_state().FreePrags, stack_entry );
        return( true );
    }
    return( false );
}

void CPragmaInit( void )
/**********************/
{
    #define pick( x ) TOGGLE_STK( x ) = NULL;
    #include "togdef.h"
    #undef pick
    TOGGLE_STK( pack ) = NULL;
    TOGGLE_STK( enum ) = NULL;
    cpragma_state().FreePrags = NULL;
    cpragma_state().AsmFuncNum = 0;

    cvars_state().TextSegList = NULL;
    cvars_state().AliasHead = NULL;
    cvars_state().HeadLibs = NULL;
    cvars_state().ExtrefInfo = NULL;

    PragmaAuxInit();

    PragmaInit();
}

void CPragmaFini( void )
/**********************/
{
    void    *junk;

    PragmaFini();

    PragmaAuxFini();

    #define pick( x )   freeStack( &TOGGLE_STK( x ) );
    #include "togdef.h"
    #undef pick
    freeStack( &TOGGLE_STK( pack ) );
    freeStack( &TOGGLE_STK( enum ) );
    freeStack( &cpragma_state().FreePrags );

    while( (junk = cvars_state().TextSegList) != NULL ) {
        cvars_state().TextSegList = cvars_state().TextSegList->next;
        CMemFree( junk );
    }

    while( (junk = cvars_state().HeadLibs) != NULL ) {
        cvars_state().HeadLibs = cvars_state().HeadLibs->next;
        CMemFree( junk );
    }

    while( (junk = cvars_state().AliasHead) != NULL ) {
        cvars_state().AliasHead = cvars_state().AliasHead->next;
        CMemFree( junk );
    }

    while( (junk = cvars_state().ExtrefInfo) != NULL ) {
        cvars_state().ExtrefInfo = cvars_state().ExtrefInfo->next;
        CMemFree( junk );
    }
}

static void endOfPragma( bool check_end )
/***************************************/
{
    if( check_end ) {
        if( cvars_state().CurToken == T_SEMI_COLON )
            PPNextToken();
        if( cvars_state().CurToken == T_PRAGMA_END )
            return;
        ExpectingToken( T_NULL );
        while( cvars_state().CurToken != T_NULL && cvars_state().CurToken != T_EOF ) {
            PPNextToken();
        }
    }
}

const char *SkipUnderscorePrefix( const char *str )
/*************************************************/
{
    if( cvars_state().CompFlags.non_iso_compliant_names_enabled ) {
        if( *str == '_' ) {
            str++;
            if( *str == '_' ) {
                str++;
            }
        }
    } else if( str[0] == '_' && str[1] == '_' ) {
        str += 2;
    } else {
        str = NULL;
    }
    return( str );
}

bool PragRecogId( const char *what )
/**********************************/
{
    bool        ok;
    const char  *p;

    ok = IS_ID_OR_KEYWORD( cvars_state().CurToken );
    if( ok ) {
        p = SkipUnderscorePrefix( cvars_state().Buffer );
        ok = ( p != NULL && stricmp( p, what ) == 0 );
        if( ok ) {
            PPNextToken();
        }
    }
    return( ok );
}

bool PragRecogName( const char *what )
/************************************/
{
    bool    ok;

    ok = IS_ID_OR_KEYWORD( cvars_state().CurToken );
    if( ok ) {
        ok = pragmaNameRecog( what );
        if( ok ) {
            PPNextToken();
        }
    }
    return( ok );
}

static void advanceToken( void )
/******************************/
{
    CMemFree( cvars_state().SavedId );
    cvars_state().SavedId = NULL;
    cvars_state().CurToken = cvars_state().LAToken;
}

bool GetPragmaAuxAliasInfo( void )
/********************************/
{
    if( cvars_state().CurToken != T_LEFT_PAREN )          /* #pragma aux symbol ..... */
        return( IS_ID_OR_KEYWORD( cvars_state().CurToken ) );
    PPNextToken();
    if( !IS_ID_OR_KEYWORD( cvars_state().CurToken ) )     /* error */
        return( false );
    LookAhead();
    if( cvars_state().LAToken == T_RIGHT_PAREN ) {        /* #pragma aux (alias) symbol ..... */
        pragdefn_state().CurrAlias = PragmaAuxAlias( cvars_state().SavedId );
        advanceToken();
        PPNextToken();
        return( IS_ID_OR_KEYWORD( cvars_state().CurToken ) );
    } else if( cvars_state().LAToken == T_COMMA ) {       /* #pragma aux (symbol, alias) */
        SetCurrInfo( cvars_state().SavedId );
        advanceToken();
        PPNextToken();
        if( !IS_ID_OR_KEYWORD( cvars_state().CurToken ) )
            return( false );                /* error */
        GetPragmaAuxAlias();
        return( false );                    /* process no more! */
    } else {                                /* error */
        advanceToken();
        return( false );                    /* shut up the compiler */
    }
}

void *AsmQuerySymbol( const char *name )
/**************************************/
{
    return( SymLook( CalcHashID( name ), name ) );
}

enum sym_state AsmQueryState( void *handle )
/******************************************/
{
    SYM_HANDLE  sym_handle = (SYM_HANDLE)handle;
    SYM_ENTRY   sym;

    if( sym_handle == SYM_NULL )
        return( SYM_UNDEFINED );
    SymGet( &sym, sym_handle );
    if( (sym.flags & SYM_REFERENCED) == 0 ) {
        sym.flags |= SYM_REFERENCED;
        SymReplace( &sym, sym_handle );
    }
    switch( sym.attribs.stg_class ) {
    case SC_AUTO:
    case SC_REGISTER:
        return( SYM_STACK );
    }
    return( SYM_EXTERNAL );
}

static aux_info *lookupMagicKeyword( const char *name )
/*****************************************************/
{
    magic_words     mword;

    if( (name = SkipUnderscorePrefix( name )) != NULL ) {
        for( mword = static_cast<magic_words>(0); mword < M_SIZE; mword++ ) {
            if( strcmp( name, cpragma_state().magicWords[mword].name + 2 ) == 0 ) {
                return( cpragma_state().magicWords[mword].info );
            }
        }
    }
    return( NULL );
}


void CreateAux( const char *id )
/******************************/
{
    pragdefn_state().CurrEntry = (aux_entry *)CMemAlloc( offsetof( aux_entry, name ) + strlen( id ) + 1 );
    strcpy( pragdefn_state().CurrEntry->name, id );
#if _CPU == 370
    CurrEntry->offset = -1;
#endif
}


void CreateAuxInlineFunc( bool too_many_bytes )
/*********************************************/
{
    char        name[10];

    sprintf( name, "F.%d", cpragma_state().AsmFuncNum );
    ++cpragma_state().AsmFuncNum;
    CreateAux( name );
    pragdefn_state().CurrInfo = (aux_info *)CMemAlloc( sizeof( aux_info ) );
    pragdefn_state().CurrEntry->info = pragdefn_state().CurrInfo;
    *pragdefn_state().CurrInfo = WatcallInfo;
    pragdefn_state().CurrInfo->use = 1;
    pragdefn_state().CurrInfo->save = pdefn2_state().AsmRegsSaved;  // indicate no registers saved
    if( !too_many_bytes ) {
        if( AsmInsertFixups( pragdefn_state().CurrInfo ) ) {
            AsmUsesAuto( pragdefn_state().CurrInfo );
        }
    }
    pragdefn_state().CurrEntry->next = pragdefn_state().AuxList;
    pragdefn_state().AuxList = pragdefn_state().CurrEntry;
}


void SetCurrInfo( const char *name )
/**********************************/
{
    SYM_HANDLE      sym_handle;
    SYM_ENTRY       sym;
    type_modifiers  sym_attrib = FLAG_NONE;

    pragdefn_state().CurrInfo = lookupMagicKeyword( name );
    if( pragdefn_state().CurrInfo == NULL ) {
        if( pragdefn_state().CurrAlias == NULL ) {
            sym_handle = SymLook( CalcHashID( name ), name );
            if( sym_handle != SYM_NULL ) {
                SymGet( &sym, sym_handle );
                sym_attrib = sym.mods;
            }
            pragdefn_state().CurrAlias = GetLangInfo( sym_attrib );
        }
        CreateAux( name );
    } else {
        if( pragdefn_state().CurrAlias == NULL ) {
            pragdefn_state().CurrAlias = pragdefn_state().CurrInfo;
        }
    }
}


aux_info *PragmaAuxAlias( const char *name )
/******************************************/
{
    aux_entry   *aux;
    aux_info    *info;

    info = lookupMagicKeyword( name );
    if( info == NULL ) {
        aux = AuxLookup( name );
        if( aux != NULL ) {
            info = aux->info;
        }
    }
    return( info );
}


void XferPragInfo( const char *from, const char *to )
/***************************************************/
{
    aux_entry *ent;

    if( AuxLookup( to ) != NULL )
        return;
    ent = AuxLookup( from );
    if( ent == NULL )
        return;
    CreateAux( to );
    pragdefn_state().CurrEntry->info = ent->info;
    ent->info->use++;
    pragdefn_state().CurrEntry->next = pragdefn_state().AuxList;
    pragdefn_state().AuxList = pragdefn_state().CurrEntry;
}


static void CopyLinkage( void )
/*****************************/
{
#if _CPU == 370
    linkage_regs *regs;

    if( CurrInfo->linkage == &DefaultLinkage )
        return;
    if( CurrInfo->linkage == &OSLinkage )
        return;
    if( CurrInfo->linkage != CurrAlias->linkage )
        return;
    regs = (linkage_regs *)CMemAlloc( sizeof( linkage_regs ) );
    memcpy( regs, CurrInfo->linkage, sizeof( linkage_regs ) );
    CurrInfo->linkage = regs;
#endif
}


static void CopyParms( void )
/***************************/
{
    size_t      size;
    hw_reg_set  *regs;

    if( pragdefn_state().CurrInfo->parms != pragdefn_state().CurrAlias->parms )
        return;
    if( IsAuxParmsBuiltIn( pragdefn_state().CurrInfo->parms ) )
        return;
    size = sizeof( hw_reg_set );
    for( regs = pragdefn_state().CurrInfo->parms; !HW_CEqual( *regs, HW_EMPTY ); ++regs ) {
        size += sizeof( hw_reg_set );
    }
    regs = (hw_reg_set *)CMemAlloc( size );
    memcpy( regs, pragdefn_state().CurrInfo->parms, size );
    pragdefn_state().CurrInfo->parms = regs;
}

static void CopyCode( void )
/**************************/
{
    byte_seq    *code;
    unsigned    size;

    /*
     * TODO deal with reloc list
     */
    if( pragdefn_state().CurrInfo->code == NULL )
        return;
    if( pragdefn_state().CurrInfo->code != pragdefn_state().CurrAlias->code )
        return;
    size = offsetof( byte_seq, data ) + pragdefn_state().CurrInfo->code->length;
    code = (byte_seq *)CMemAlloc( size );
    memcpy( code, pragdefn_state().CurrInfo->code, size );
    pragdefn_state().CurrInfo->code = code;
}

static void CopyObjName( void )
/*****************************/
{
    if( pragdefn_state().CurrInfo->objname == NULL )
        return;
    if( pragdefn_state().CurrInfo->objname != pragdefn_state().CurrAlias->objname )
        return;
    pragdefn_state().CurrInfo->objname = CMemStrdup( pragdefn_state().CurrInfo->objname );
}

#if _CPU == _AXP
static void CopyExceptRtn( void )
/*******************************/
{
    if( CurrInfo->except_rtn == NULL )
        return;
    if( CurrInfo->except_rtn != CurrAlias->except_rtn )
        return;
    CurrInfo->except_rtn = CMemStrdup( CurrInfo->except_rtn );
}
#endif

void PragmaAuxEnding( void )
/**************************/
{
    if( pragdefn_state().CurrEntry == NULL )
        return;
    pragdefn_state().CurrInfo->use = pragdefn_state().CurrAlias->use; /* for compare */
    if( memcmp( pragdefn_state().CurrAlias, pragdefn_state().CurrInfo, sizeof( aux_info ) ) == 0 ) {
        pragdefn_state().CurrEntry->info = pragdefn_state().CurrAlias;
        pragdefn_state().CurrAlias->use++;
        CMemFree( pragdefn_state().CurrInfo );
    } else {
        CopyParms();
        CopyLinkage();
        CopyCode();
        CopyObjName();
#if _CPU == _AXP
        CopyExceptRtn();
#endif
        pragdefn_state().CurrInfo->use = 1;
        pragdefn_state().CurrEntry->info = pragdefn_state().CurrInfo;
    }
    /*
     * If this pragma defines code, check to see if we already have a function body
     */
    if( pragdefn_state().CurrEntry != NULL && pragdefn_state().CurrEntry->info != NULL && pragdefn_state().CurrEntry->info->code != NULL ) {
        SYM_HANDLE  sym_handle;
        SYM_ENTRY   sym;

        if( SYM_NULL != (sym_handle = SymLook( CalcHashID( pragdefn_state().CurrEntry->name ), pragdefn_state().CurrEntry->name )) ) {
            SymGet( &sym, sym_handle );
            if( ( sym.flags & SYM_DEFINED ) && ( sym.flags & SYM_FUNCTION ) ) {
                CErr2p( ERR_SYM_ALREADY_DEFINED, pragdefn_state().CurrEntry->name );
            }
        }
    }

    pragdefn_state().CurrEntry->next = pragdefn_state().AuxList;
    pragdefn_state().AuxList = pragdefn_state().CurrEntry;
}


void PragObjNameInfo( char **objname )
/************************************/
{
    if( cvars_state().CurToken == T_STRING ) {
        *objname = CMemStrdup( cvars_state().Buffer );
        PPNextToken();
    }
}

TOKEN PragRegSet( void )
/**********************/
{
    if( cvars_state().CurToken == T_LEFT_BRACKET )
        return( T_RIGHT_BRACKET );
    if( cvars_state().CurToken == T_LEFT_BRACE )
        return( T_RIGHT_BRACE );
    return( T_NULL );
}

int PragRegIndex( const char *registers, const char *name, bool ignorecase )
/**************************************************************************/
{
    int             index;
    const char      *p;
    unsigned char   c;
    unsigned char   c2;
    size_t          i;
    size_t          len;

    len = strlen( name );
    index = 0;
    for( p = registers; *p != '\0'; ) {
        i = 0;
        while( (c = *p++) != '\0' ) {
            if( i > len )
                continue;
            if( i < len ) {
                c2 = name[i++];
                if( c == c2 || ignorecase && tolower( c ) == tolower( c2 ) ) {
                    continue;
                }
            }
            i = len + 1;
        }
        if( i == len )
            return( index );
        ++index;
    }
    return( -1 );
}

int PragRegNumIndex( const char *str, int max_reg )
/*************************************************/
{
    int         index;

    /*
     * decode regular register index, max 2 digit
     */
    if( isdigit( (unsigned char)str[0] ) ) {
        index = str[0] - '0';
        if( isdigit( (unsigned char)str[1] ) ) {
            index = index * 10 + ( str[1] - '0' );
            if( str[2] == '\0' && index < max_reg ) {
                return( index );
            }
        } else if( str[1] == '\0' && index < max_reg ) {
            return( index );
        }
    }
    return( -1 );
}

void PragRegNameErr( const char *regname )
/****************************************/
{
    CErr2p( ERR_BAD_REGISTER_NAME, regname );
}

hw_reg_set PragRegList( void )
/****************************/
{
    hw_reg_set  res, reg;
    TOKEN       close;

    HW_CAsgn( res, HW_EMPTY );
    HW_CAsgn( reg, HW_EMPTY );
    close = PragRegSet();
    if( close != T_NULL ) {
        while( PPNextToken() != close ) {
            reg = PragReg();
            HW_TurnOn( res, reg );
            if( cvars_state().CurToken == close ) {
                break;
            }
        }
        MustRecog( close );
    }
    return( res );
}

hw_reg_set *PragManyRegSets( void )
/*********************************/
{
    int         i;
    hw_reg_set  list;
    hw_reg_set  *sets;
    hw_reg_set  buff[MAXIMUM_PARMSETS + 1];

    for( i = 0, list = PragRegList(); !HW_CEqual( list, HW_EMPTY ); list = PragRegList(), ++i ) {
        if( i == MAXIMUM_PARMSETS ) {
            CErr1( ERR_TOO_MANY_PARM_SETS );
            break;
        }
        buff[i] = list;
    }
    HW_CAsgn( buff[i], HW_EMPTY );
    i = ( i + 1 ) * sizeof( hw_reg_set );
    sets = (hw_reg_set *)CMemAlloc( i );
    memcpy( sets, buff, i );
    return( sets );
}

/*******************************************************
 *
 *  #pragma .... processing
 *
 *******************************************************/

void SetToggleFlag( char const *name, int func, bool push )
/*********************************************************/
{
#ifdef DEVBUILD
    #define pick( x ) \
        if( strcmp( name, #x ) == 0 ) { \
            if( func == -1 ) { \
            } else { \
                TOGGLEDBG( x ) = ( func != 0 ); \
            } \
            return; \
        }
    #include "togdefd.h"
    #undef pick
#endif
    #define pick( x ) \
        if( strcmp( name, #x ) == 0 ) { \
            if( func == -1 ) { \
                unsigned    value; \
                if( popPrag( &TOGGLE_STK( x ), &value ) ) { \
                    TOGGLE( x ) = ( value != 0 ); \
                } \
            } else { \
                if( push ) { \
                    pushPrag( &TOGGLE_STK( x ), TOGGLE( x ) ); \
                } \
                TOGGLE( x ) = ( func != 0 ); \
            } \
            return; \
        }
    #include "togdef.h"
    #undef pick
}

static void pragOptions( int func )
/**********************************
 * forms:
 *
 *  #pragma on (<toggle name>)
 *  #pragma off (<toggle name>)
 *  #pragma pop (<toggle name>)
 */
{
    PPCTL_ENABLE_MACROS();
    PPNextToken();
    if( ExpectingToken( T_LEFT_PAREN ) ) {
        for( PPNextToken(); IS_ID_OR_KEYWORD( cvars_state().CurToken ); PPNextToken() ) {
            SetToggleFlag( cvars_state().Buffer, func, true );
        }
        MustRecog( T_RIGHT_PAREN );
    }
    PPCTL_DISABLE_MACROS();
}

void AddLibraryName( const char *libname, char priority )
/*******************************************************/
{
    library_list    **owner;
    library_list    *lib;

    for( owner = &cvars_state().HeadLibs; (lib = *owner) != NULL; owner = &lib->next ) {
        if( lib->priority < priority ) {
            library_list    **tmp_owner;
            /*
             * search library entry with lower priority
             */
            for( tmp_owner = owner; (lib = *tmp_owner) != NULL; tmp_owner = &lib->next ) {
                if( FNAMECMPSTR( lib->libname, libname ) == 0 ) {
                    /*
                     * remove library entry from linked list
                     */
                    *tmp_owner = lib->next;
                    break;
                }
            }
            break;
        }
        if( FNAMECMPSTR( lib->libname, libname ) == 0 ) {
            /*
             * library entry already exists with higher or equal priority
             * no change required
             */
            return;
        }
    }
    /*
     * if library entry not found then create new one
     */
    if( lib == NULL ) {
        lib = static_cast<library_list *>(CMemAlloc( sizeof( library_list ) + strlen( libname ) ));
        strcpy( lib->libname, libname );
    }
    /*
     * set priority and insert library entry to proper position
     */
    lib->priority = priority;
    lib->next = *owner;
    *owner = lib;
}

static void GetLibraryNames( void )
/********************************/
{
    while( IS_ID_OR_KEYWORD( cvars_state().CurToken ) || cvars_state().CurToken == T_STRING ) {
        AddLibraryName( cvars_state().Buffer, USER_LIB_PRIO );
        PPNextToken();
    }
}

static void pragLibs( void )
/***************************
 * forms:
 *
 *  #pragma library
 *  #pragma library (<libraries name list>)
 */
{
    PPCTL_ENABLE_MACROS();
    PPNextToken();
    if( cvars_state().CurToken == T_LEFT_PAREN ) {
        PPNextToken();
        GetLibraryNames();
        MustRecog( T_RIGHT_PAREN );
    } else {
        cvars_state().CompFlags.pragma_library = true;
    }
    PPCTL_DISABLE_MACROS();
}

static void pragComment( void )
/******************************
 * forms:
 *
 *  #pragma comment ( comment_type [, "comment_string"] )
 */
{
    PPCTL_ENABLE_MACROS();
    PPNextToken();
    if( ExpectingToken( T_LEFT_PAREN ) ) {
        PPNextToken();
        if( PragRecogId( "lib" ) ) {
            if( ExpectingToken( T_COMMA ) ) {
                PPNextToken();
            }
            GetLibraryNames();
        }
        MustRecog( T_RIGHT_PAREN );
    }
    PPCTL_DISABLE_MACROS();
}

unsigned VerifyPackAmount( unsigned amount )
/******************************************/
{
    if( amount <= 1 ) {
        amount = 1;
    } else if( amount <= 2 ) {
        amount = 2;
    } else if( amount <= 4 ) {
        amount = 4;
    } else if( amount <= 8 ) {
        amount = 8;
    } else {
        amount = 16;
    }
    return( amount );
}

void SetPackAmount( unsigned amount )
/***********************************/
{
    cvars_state().PackAmount = VerifyPackAmount( amount );
}

static void getPackArgs( void )
/****************************/
{
    /*
     * check to make sure it is a numeric token
     */
    if( PragRecogId( "push" ) ) {
        pushPrag( &TOGGLE_STK( pack ), (unsigned)cvars_state().PackAmount );
        if( cvars_state().CurToken == T_COMMA ) {
            PPNextToken();
            if( ExpectingConstant() ) {
                SetPackAmount( U32FetchTrunc( cvars_state().Constant64 ) );
            }
            PPNextToken();
        }
    } else if( PragRecogId( "pop" ) ) {
        unsigned    value;

        if( popPrag( &TOGGLE_STK( pack ), &value ) ) {
            cvars_state().PackAmount = (align_type)value;
        }
    } else {
        CErr1( ERR_NOT_A_CONSTANT_EXPR );
    }
}

static void pragPack( void )
/***************************
 * forms:
 *
 *  #pragma pack ()
 *  #pragma pack ( n )
 *  #pragma pack ( push )
 *  #pragma pack ( pop )
 *  #pragma pack ( push, n )
 */
{
    PPCTL_ENABLE_MACROS();
    PPNextToken();
    if( ExpectingToken( T_LEFT_PAREN ) ) {
        PPNextToken();
        if( cvars_state().CurToken == T_CONSTANT ) {
            SetPackAmount( U32FetchTrunc( cvars_state().Constant64 ) );
            PPNextToken();
        } else if( IS_ID_OR_KEYWORD( cvars_state().CurToken ) ) {
            getPackArgs();
        } else if( cvars_state().CurToken == T_RIGHT_PAREN ) {
            cvars_state().PackAmount = cvars_state().GblPackAmount;
        }
        MustRecog( T_RIGHT_PAREN );
    }
    PPCTL_DISABLE_MACROS();
}

textsegment *NewTextSeg( const char *name, const char *suffix, const char *classname )
/************************************************************************************/
{
    textsegment     *tseg;
    size_t          len1;
    size_t          len2;
    size_t          len3;

    len1 = strlen( name );
    len2 = strlen( suffix );
    len3 = strlen( classname );
    tseg = static_cast<textsegment *>(CMemAlloc( offsetof( textsegment, segname ) + len1 + len2 + 1 + len3 + 1 ));
    tseg->class_ = len1 + len2 + 1;
    strcpy( tseg->segname, name );
    strcpy( tseg->segname + len1, suffix );
    strcpy( tseg->segname + tseg->class_, classname );
    tseg->next = cvars_state().TextSegList;
    cvars_state().TextSegList = tseg;
    return( tseg );
}

textsegment *LkSegName( const char *segname, const char *classname )
/******************************************************************/
{
    textsegment     *tseg;

    for( tseg = cvars_state().TextSegList; tseg != NULL; tseg = tseg->next ) {
        if( strcmp( tseg->segname, segname ) == 0 ) {
            if( strcmp( tseg->segname + tseg->class_, classname ) == 0 ) {
                return( tseg );
            }
        }
    }
    return( NewTextSeg( segname, "", classname ) );
}

static void pragAllocText( void )
/********************************
 * forms:
 *
 *  #pragma alloc_text ( seg_name, fn [, fn] )
 */
{
    struct textsegment  *tseg;
    SYM_HANDLE          sym_handle;
    SYM_ENTRY           sym;

    PPCTL_ENABLE_MACROS();
    PPNextToken();
    if( ExpectingToken( T_LEFT_PAREN ) ) {
        PPNextToken();
        /*
         * current token can be an T_ID or a T_STRING
         */
        tseg = LkSegName( cvars_state().Buffer, "" );
        PPNextToken();
        for( ;; ) {
            MustRecog( T_COMMA );
            /*
             * current token can be an T_ID or a T_STRING
             */
            sym_handle = Sym0Look( CalcHashID( cvars_state().Buffer ), cvars_state().Buffer );
            if( sym_handle == SYM_NULL ) {
                /*
                 * error
                 */
            } else {
                SymGet( &sym, sym_handle );
                if( sym.flags & SYM_FUNCTION ) {
                    sym.seginfo = tseg;
                    SymReplace( &sym, sym_handle );
                } else {
                    /*
                     * error, must be function
                     */
                }
            }
            PPNextToken();
            if( cvars_state().CurToken == T_RIGHT_PAREN )
                break;
            if( cvars_state().CurToken == T_EOF )
                break;
            if( cvars_state().CurToken == T_NULL ) {
                break;
            }
        }
#if _INTEL_CPU
        cvars_state().CompFlags.multiple_code_segments = true;
#endif
        MustRecog( T_RIGHT_PAREN );
    }
    PPCTL_DISABLE_MACROS();
}

static void changeLevel( unsigned level, int msg_index )
/******************************************************/
{
    if( cerrs_state().msg_level[msg_index].level != level ) {
        cerrs_state().msg_level[msg_index].level = level;
        if( level == WLEVEL_DISABLED ) {
            /*
             * disable message
             */
            if( cerrs_state().msg_level[msg_index].enabled ) {
                cerrs_state().msg_level[msg_index].enabled = false;
            }
        } else {
            /*
             * enable message
             */
            if( !cerrs_state().msg_level[msg_index].enabled ) {
                cerrs_state().msg_level[msg_index].enabled = true;
            }
        }
    }
}

static void changeStatus( bool enabled, int msg_index )
/*****************************************************/
{
    if( cerrs_state().msg_level[msg_index].enabled != enabled ) {
        cerrs_state().msg_level[msg_index].enabled = enabled;
    }
}

bool GetMsgNum( const char *str, msg_codes *val )
/***********************************************/
{
    char            c;

    /*
     * skip, C++ compiler messages, prefixed by 'P' character
     */
    c = *str;
    if( c == 'P' || c == 'p' ) {
        *val = static_cast<msg_codes>(0);
        return( true );
    }
    /*
     * process C compiler messages, prefixed by 'C' character
     * or old messages without prefix which can be C or C++ message
     * it is for backward compatibility
     */
    if( c == 'C' || c == 'c' )
        str++;
    if( isdigit( *(unsigned char *)str ) ) {
        *val = static_cast<msg_codes>(atol( str ));
        return( true );
    }
    return( false );
}

static bool getMessageNum( msg_codes *val )
/*****************************************/
{
    if( cvars_state().CurToken == T_CONSTANT ) {
        *val = static_cast<msg_codes>(U32FetchTrunc( cvars_state().Constant64 ));
        return( true );
    } else if( cvars_state().CurToken == T_ID ) {
        return( GetMsgNum( cvars_state().Buffer, val ) );
    }
    return( false );
}

static void warnChangeLevel( unsigned level, msg_codes msgnum )
/**************************************************************
 * CHANGE WARNING LEVEL FOR A MESSAGE
 */
{
    unsigned    msg_index;

    if( msgnum == 0 )
        return;
    msg_index = GetMsgIndex( msgnum );
    if( IS_MSGIDX_INVALID( msg_index ) ) {
        CWarn2( ERR_PRAG_WARNING_BAD_MESSAGE, msgnum );
        return;
    }
    switch( cerrs_state().msg_level[msg_index].type ) {
//    case MSG_TYPE_ERROR :       /* Error messages are used for Warning */
    case MSG_TYPE_INFO :
    case MSG_TYPE_ANSIERR :
        CWarn2( ERR_PRAG_WARNING_BAD_MESSAGE, msgnum );
        break;
    case MSG_TYPE_ERROR :       /* Error messages are used for Warning */
    case MSG_TYPE_WARNING :
    case MSG_TYPE_ANSI :
    case MSG_TYPE_ANSIWARN :
        changeLevel( level, msg_index );
        break;
    }
}

static void warnChangeLevels( unsigned level )
/*********************************************
 * CHANGE WARNING LEVELS FOR ALL MESSAGES
 */
{
    int     msg_index;          // - index for number

    for( msg_index = 0; msg_index < MESSAGE_COUNT; msg_index++ ) {
        switch( cerrs_state().msg_level[msg_index].type ) {
        case MSG_TYPE_ERROR :       /* Error messages are used for Warning */
        case MSG_TYPE_WARNING :
        case MSG_TYPE_ANSI :
        case MSG_TYPE_ANSIWARN :
            changeLevel( level, msg_index );
            break;
        }
    }
}

void WarnEnableDisable( bool enabled, msg_codes msgnum )
/******************************************************/
{
    unsigned    msg_index;

    if( msgnum == 0 )
        return;
    msg_index = GetMsgIndex( msgnum );
    if( IS_MSGIDX_INVALID( msg_index ) ) {
        CWarn2( ERR_PRAG_WARNING_BAD_MESSAGE, msgnum );
        return;
    }
    switch( cerrs_state().msg_level[msg_index].type ) {
//    case MSG_TYPE_ERROR :       /* Error messages are used for Warning */
    case MSG_TYPE_INFO :
    case MSG_TYPE_ANSIERR :
        CWarn2( ERR_PRAG_WARNING_BAD_MESSAGE, msgnum );
        break;
    case MSG_TYPE_ERROR :       /* Error messages are used for Warning */
    case MSG_TYPE_WARNING :
    case MSG_TYPE_ANSI :
    case MSG_TYPE_ANSIWARN :
        changeStatus( enabled, msg_index );
        break;
    }
}

static bool pragWarning( void )
/******************************
 * forms:
 *
 *  #pragma warning # level   (change message # to have level "level)
 *  #pragma warning * level   (change all messages to have level "level)
 *
 * "level" must be digit (0-5)
 * "level==0" implies warning will be treated as an error
 */
{
    msg_codes msgnum;           // - message number
    unsigned level;             // - new level
    bool change_all;            // - true ==> change all levels
    bool ignore;

    ignore = false;
    change_all = false;
    msgnum = static_cast<msg_codes>(0);
    PPCTL_ENABLE_MACROS();
    NextToken();
    if( cvars_state().CurToken == T_TIMES ) {
        change_all = true;
    } else if( !getMessageNum( &msgnum ) ) {
        /*
         * ignore; MS or other vendor's #pragma
         */
        ignore = true;
    }
    if( !ignore ) {
        NextToken();
        if( cvars_state().CurToken == T_CONSTANT ) {
            level = U32FetchTrunc( cvars_state().Constant64 );
            NextToken();
            if( change_all ) {
                warnChangeLevels( level );
            } else {
                warnChangeLevel( level, msgnum );
            }
        } else {
            CWarn1( ERR_PRAG_WARNING_BAD_LEVEL );
            NextToken();
        }
    }
    PPCTL_DISABLE_MACROS();
    return( ignore );
}

static void pragEnableDisableMessage( bool enabled )
/***************************************************
 * forms:
 *
 *  #pragma enable_message( messageNo )
 *  #pragma disable_message( messageNo )
 *
 * disable/enable display of selected message number
 */
{
    msg_codes msgnum;

    PPCTL_ENABLE_MACROS();
    PPNextToken();
    if( ExpectingToken( T_LEFT_PAREN ) ) {
        PPNextToken();
        while( getMessageNum( &msgnum ) ) {
            WarnEnableDisable( enabled, msgnum );
            PPNextToken();
            if( cvars_state().CurToken == T_COMMA ) {
                PPNextToken();
            }
        }
        MustRecog( T_RIGHT_PAREN );
    }
    PPCTL_DISABLE_MACROS();
}

static char *collectStrings( char *message )
{
    size_t  len;
    size_t  new_len;

    len = 0;
    while( cvars_state().CurToken == T_STRING ) {
        new_len = len + strlen( cvars_state().Buffer );
        message = static_cast<char *>(CMemRealloc( message, new_len + 1 ));
        strcpy( message + len, cvars_state().Buffer );
        len = new_len;
        PPNextToken();
    }
    return( message );
}

static void pragMessage( void )
/******************************
 * form:
 *
 *  #pragma message ("one or more " "long message " "strings")
 *
 * output these strings to stdout
 * this output is _not_ dependent on setting
 * of #pragma enable_message or disable_message.
 */
{
    char    *message;

    PPCTL_ENABLE_MACROS();
    PPNextToken();
    if( ExpectingToken( T_LEFT_PAREN ) ) {
        PPNextToken();
        if( cvars_state().CurToken == T_STRING ) {
            message = static_cast<char *>(CMemAlloc( 1 ));
            message[0] = '\0';
            message = collectStrings( message );
            BannerMsg( message );
            CMemFree( message );
        }
        MustRecog( T_RIGHT_PAREN );
    }
    PPCTL_DISABLE_MACROS();
}

static void pragEnum( void )
/***************************
 * forms:
 *
 *  #pragma enum int        (1)
 *  #pragma enum minimum    (2)
 *  #pragma enum original   (3)
 *  #pragma enum pop        (4)
 *
 * The pragma affects the underlying storage-definition for subsequent
 * enum declarations.
 *
 * (1) make int the underlying storage definition (same as -ei)
 * (2) minimize the underlying storage definition (same as no -ei)
 * (3) reset back to result of command-line parsing
 * (4) restore previous value
 *
 * 1-3 all push previous value before affecting value
 */
{
    PPCTL_ENABLE_MACROS();
    PPNextToken();
    if( PragRecogId( "int" ) ) {
        pushPrag( &TOGGLE_STK( enum ), cvars_state().CompFlags.make_enums_an_int );
        cvars_state().CompFlags.make_enums_an_int = true;
    } else if( PragRecogId( "minimum" ) ) {
        pushPrag( &TOGGLE_STK( enum ), cvars_state().CompFlags.make_enums_an_int );
        cvars_state().CompFlags.make_enums_an_int = false;
    } else if( PragRecogId( "original" ) ) {
        pushPrag( &TOGGLE_STK( enum ), cvars_state().CompFlags.make_enums_an_int );
        cvars_state().CompFlags.make_enums_an_int = cvars_state().CompFlags.original_enum_setting;
    } else if( PragRecogId( "pop" ) ) {
        unsigned    value;

        if( popPrag( &TOGGLE_STK( enum ), &value ) ) {
            cvars_state().CompFlags.make_enums_an_int = ( value != 0 );
        }
    }
    PPCTL_DISABLE_MACROS();
}

static void pragIntrinsic( int intrinsic )
/*****************************************
 * forms:
 *
 *  #pragma intrinsic ( fn [, fn] )
 */
{
    SYM_HANDLE  sym_handle;
    SYM_ENTRY   sym;

    PPCTL_ENABLE_MACROS();
    PPNextToken();
    if( ExpectingToken( T_LEFT_PAREN ) ) {
        for( PPNextToken(); IS_ID_OR_KEYWORD( cvars_state().CurToken ); PPNextToken() ) {
            sym_handle = SymLook( CalcHashID( cvars_state().Buffer ), cvars_state().Buffer );
            if( sym_handle != SYM_NULL ) {
                SymGet( &sym, sym_handle );
                sym.flags &= ~ SYM_INTRINSIC;
                if( intrinsic )
                    sym.flags |= SYM_INTRINSIC;
                SymReplace( &sym, sym_handle );
            }
            PPNextToken();
            if( cvars_state().CurToken != T_COMMA ) {
                break;
            }
        }
        MustRecog( T_RIGHT_PAREN );
    }
    PPCTL_DISABLE_MACROS();
}

static void pragCodeSeg( void )
/******************************
 * forms:
 *
 *  #pragma code_seg ( seg_name [, class_name] )
 */
{
    textsegment     *tseg;
    char            *segname;
    char            *classname;

    PPCTL_ENABLE_MACROS();
    PPNextToken();
    if( cvars_state().CurToken == T_LEFT_PAREN ) {
        tseg = NULL;
        PPNextToken();
        if( ( cvars_state().CurToken == T_STRING ) || ( cvars_state().CurToken == T_ID ) ) {
            segname = CMemStrdup( cvars_state().Buffer );
            classname = CMemStrdup( "" );
            PPNextToken();
            if( cvars_state().CurToken == T_COMMA ) {
                PPNextToken();
                if( ( cvars_state().CurToken == T_STRING ) || ( cvars_state().CurToken == T_ID ) ) {
                    CMemFree( classname );
                    classname = CMemStrdup( cvars_state().Buffer );
//                    CodeClassName = CMemStrdup( Buffer ); */
                    PPNextToken();
                }
            }
            tseg = LkSegName( segname, classname );
            CMemFree( segname );
            CMemFree( classname );
        }
        MustRecog( T_RIGHT_PAREN );
        cvars_state().DefCodeSegment = tseg;
#if _INTEL_CPU
        cvars_state().CompFlags.multiple_code_segments = true;
#endif
    }
    PPCTL_DISABLE_MACROS();
}

static void pragDataSeg( void )
/******************************
 * forms:
 *
 *  #pragma data_seg ( seg_name [, class_name] )
 */
{
    char        *segname;
    segment_id  segid;

    PPCTL_ENABLE_MACROS();
    PPNextToken();
    if( cvars_state().CurToken == T_LEFT_PAREN ) {
        segid = SEG_NULL;
        PPNextToken();
        if( ( cvars_state().CurToken == T_STRING ) || ( cvars_state().CurToken == T_ID ) ) {
            segname = CMemStrdup( cvars_state().Buffer );
            PPNextToken();
            if( cvars_state().CurToken == T_COMMA ) {
                PPNextToken();
                if( ( cvars_state().CurToken == T_STRING ) || ( cvars_state().CurToken == T_ID ) ) {
                    segid = AddSegName( segname, cvars_state().Buffer, SEGTYPE_DATA );
                    PPNextToken();
                } else {
                    segid = AddSegName( segname, NULL, SEGTYPE_DATA );
                }
            } else {
                segid = AddSegName( segname, NULL, SEGTYPE_DATA );
            }
            CMemFree( segname );
        }
        MustRecog( T_RIGHT_PAREN );
        cvars_state().DefDataSegment = segid;
    }
    PPCTL_DISABLE_MACROS();
}

static void pragUnroll( void )
/*****************************
 * forms:
 *
 *  #pragma unroll ( n )
 */
{
    unroll_type unroll_count;

    PPCTL_ENABLE_MACROS();
    PPNextToken();
    if( ExpectingToken( T_LEFT_PAREN ) ) {
        unroll_count = 0;
        PPNextToken();
        if( cvars_state().CurToken == T_CONSTANT ) {
            if( U64CmpU32( cvars_state().Constant64, 255 ) > 0 ) {
                unroll_count = (unroll_type)255;
            } else {
                unroll_count = (unroll_type)U8FetchTrunc( cvars_state().Constant64 );
            }
            PPNextToken();
        }
        cvars_state().UnrollCount = unroll_count;
        MustRecog( T_RIGHT_PAREN );
    }
    PPCTL_DISABLE_MACROS();
}

static void pragReadOnlyFile( void )
/***********************************
 * forms:
 *
 *  #pragma read_only_file          (1)
 *  #pragma read_only_file "file"*  (2)
 *
 * (1) causes current file to be marked read-only
 * (2) causes indicated file to be marked read-only
 *      - file must have started inclusion (may have completed)
 */
{
    PPCTL_ENABLE_MACROS();
    PPNextToken();
    if( cvars_state().CurToken == T_STRING ) {
        do {
            SrcFileReadOnlyFile( cvars_state().Buffer );
            PPNextToken();
            if( cvars_state().CurToken == T_SEMI_COLON ) {
                PPNextToken();
            }
        } while( cvars_state().CurToken == T_STRING );
    } else {
        SrcFileReadOnlyFile( NULL );
    }
    PPCTL_DISABLE_MACROS();
}


static void pragReadOnlyDir( void )
/**********************************
 * forms:
 *
 *  #pragma read_only_directory "directory"*
 *
 * causes all files within directory to be marked read-only
 */
{
    PPCTL_ENABLE_MACROS();
    PPNextToken();
    while( cvars_state().CurToken == T_STRING ) {
        SrcFileReadOnlyDir( cvars_state().Buffer );
        PPNextToken();
        if( cvars_state().CurToken == T_SEMI_COLON ) {
            PPNextToken();
        }
    }
    PPCTL_DISABLE_MACROS();
}

static void pragIncludeAlias( void )
/***********************************
 * forms:
 *
 *  #pragma include_alias ( "alias_name", "real_name" )
 *  #pragma include_alias ( <alias_name>, <real_name> )
 *
 * causes include directives referencing alias_name to be refer
 * to real_name instead
 */
{
    PPCTL_ENABLE_MACROS();
    PPNextToken();
    if( ExpectingToken( T_LEFT_PAREN ) ) {
        PPNextToken();
        if( cvars_state().CurToken == T_STRING ) {
            char    *alias_name;

            alias_name = CMemStrdup( cvars_state().Buffer );
            PPNextToken();
            MustRecog( T_COMMA );
            if( cvars_state().CurToken == T_STRING ) {
                SrcFileIncludeAlias( alias_name, cvars_state().Buffer, false );
                PPNextToken();
            }
            CMemFree( alias_name );
        } else if( cvars_state().CurToken == T_LT ) {
            char    a_buf[82];    /* same size as CInclude() in cmac2.c */
            char    r_buf[82];

            a_buf[0] = '\0';
            for( ; PPNextToken() != T_NULL; ) {
                if( cvars_state().CurToken == T_GT ) {
                    PPNextToken();
                    break;
                }
                strncat( a_buf, cvars_state().Buffer, sizeof( a_buf ) - 2 );
            }
            MustRecog( T_COMMA );
            if( cvars_state().CurToken == T_LT ) {
                r_buf[0] = '\0';
                for( ; PPNextToken() != T_NULL; ) {
                    if( cvars_state().CurToken == T_GT ) {
                        PPNextToken();
                        break;
                    }
                    strncat( r_buf, cvars_state().Buffer, sizeof( r_buf ) - 2 );
                }
                SrcFileIncludeAlias( a_buf, r_buf, true );
            }
        }
        MustRecog( T_RIGHT_PAREN );
    }
    PPCTL_DISABLE_MACROS();
}

static void pragOnce( void )
/***************************
 * forms:
 *
 *  #pragma once
 *
 * include file once
 */
{
    PPCTL_ENABLE_MACROS();
    PPNextToken();
    SetSrcFNameOnce();
    PPCTL_DISABLE_MACROS();
}

static void OptionPragSTDC( void )
/********************************/
{
    if( PragRecogName( "ON" ) ) {
    } else if( PragRecogName( "OFF" ) ) {
    } else if( PragRecogName( "DEFAULT" ) ) {
    }
}

static void pragSTDC( void )
/***************************
 * forms:
 *
 *  #pragma STDC (FP_CONTRACT|FENV_ACCESS|CX_LIMITED_RANGE) (ON|OFF|DEFAULT)
 */
{
    PPCTL_DISABLE_MACROS();
    PPNextToken();
    if( PragRecogName( "FP_CONTRACT" ) ) {
        OptionPragSTDC();
    } else if( PragRecogName( "FENV_ACCESS" ) ) {
        OptionPragSTDC();
    } else if( PragRecogName( "CX_LIMITED_RANGE" ) ) {
        OptionPragSTDC();
    }
}

void AddExtRefN ( const char *name )
/**********************************/
{
    extref_info     **extref;
    extref_info     *new_extref;

    for( extref = &cvars_state().ExtrefInfo; *extref != NULL; extref = &(*extref)->next )
        ; /* nothing to do */
    new_extref = static_cast<extref_info *>(CMemAlloc( sizeof( extref_info ) + strlen( name ) ));
    strcpy( new_extref->name, name );
    new_extref->symbol = NULL;
    new_extref->next = NULL;
    *extref = new_extref;
}

void AddExtRefS ( SYM_HANDLE sym )
/********************************/
{
    extref_info  **extref;
    extref_info  *new_extref;

    for( extref = &cvars_state().ExtrefInfo; *extref != NULL; extref = &(*extref)->next )
        ; /* nothing to do */
    new_extref = static_cast<extref_info *>(CMemAlloc( sizeof( extref_info ) ));
    new_extref->symbol = sym;
    new_extref->name[0] = '\0';
    new_extref->next = NULL;
    *extref = new_extref;
}

static void parseExtRef ( void )
/******************************/
{
    SYM_HANDLE   extref_sym;

    if( cvars_state().CurToken == T_STRING ) {
        AddExtRefN( cvars_state().Buffer );
    } else {
        extref_sym = SymLook( CalcHashID( cvars_state().Buffer ), cvars_state().Buffer );
        if( extref_sym != NULL ) {
            AddExtRefS( extref_sym );
        } else {
            CErr2p( ERR_UNDECLARED_SYM, cvars_state().Buffer );
        }
    }
}

static void pragExtRef( void )
/*****************************
 * forms:
 *
 *  #pragma extref ( symbolid [, ...] )
 *  #pragma extref ( "symbolname" [, ...] )
 */
{
    PPCTL_ENABLE_MACROS();
    PPNextToken();
    if( cvars_state().CurToken == T_LEFT_PAREN ) {
        do {
            PPNextToken();
            if( !IS_ID_OR_KEYWORD( cvars_state().CurToken ) && cvars_state().CurToken != T_STRING )
                break;
            parseExtRef();
            PPNextToken();
        } while( cvars_state().CurToken == T_COMMA );
        MustRecog( T_RIGHT_PAREN );
    } else if( IS_ID_OR_KEYWORD( cvars_state().CurToken ) || cvars_state().CurToken == T_STRING ) {
        parseExtRef();
        PPNextToken();
    }
    PPCTL_DISABLE_MACROS();
}

static void pragAlias( void )
/****************************
 * forms:
 *
 *  #pragma alias(id1/"name1", id2/"name2")
 *
 * Causes linker to replace references to id1/name1 with references
 * to id2/name2. Both the alias and the substituted symbol may be defined
 * either as a string name or an id of existing symbol.
 */
{
    SYM_HANDLE      alias_sym;
    SYM_HANDLE      subst_sym;
    const char      *alias_name;
    const char      *subst_name;
    alias_list      **alias;
    alias_list      *new_alias;

    alias_name = NULL;
    subst_name = NULL;
    alias_sym = SYM_NULL;
    subst_sym = SYM_NULL;

    PPCTL_ENABLE_MACROS();
    PPNextToken();
    if( ExpectingToken( T_LEFT_PAREN ) ) {
        PPNextToken();
        if( cvars_state().CurToken == T_ID ) {
            alias_sym = SymLook( CalcHashID( cvars_state().Buffer ), cvars_state().Buffer );
            if( alias_sym == SYM_NULL ) {
                CErr2p( ERR_UNDECLARED_SYM, cvars_state().Buffer );
            }
        } else if( cvars_state().CurToken == T_STRING ) {
            alias_name = CMemStrdup( cvars_state().Buffer );
        }
        PPNextToken();
        MustRecog( T_COMMA );
        if( cvars_state().CurToken == T_ID ) {
            subst_sym = SymLook( CalcHashID( cvars_state().Buffer ), cvars_state().Buffer );
            if( subst_sym == 0 ) {
                CErr2p( ERR_UNDECLARED_SYM, cvars_state().Buffer );
            }
        } else if( cvars_state().CurToken == T_STRING ) {
            subst_name = CMemStrdup( cvars_state().Buffer );
        }
        PPNextToken();
        MustRecog( T_RIGHT_PAREN );
    }
    PPCTL_DISABLE_MACROS();
    /*
     * Add a new alias record - if it's valid - to the list
     */
    if( ( alias_name != NULL || alias_sym != SYM_NULL ) && ( subst_name != NULL || subst_sym != SYM_NULL ) ) {
        for( alias = &cvars_state().AliasHead; *alias != NULL; alias = &(*alias)->next )
            ; /* nothing to do */
        new_alias = static_cast<alias_list *>((void *)CMemAlloc( sizeof( alias_list ) ));
        new_alias->next = NULL;
        if( alias_name ) {
            new_alias->name = alias_name;
        } else {
            new_alias->a_sym = alias_sym;
        }
        if( subst_name ) {
            new_alias->subst = subst_name;
        } else {
            new_alias->s_sym = subst_sym;
        }
        *alias = new_alias;
    }
}

void CPragma( void )
/******************/
{
    bool    check_end;

    /*
     * Note that the include_alias pragma must always be processed
     * because it's intended for the preprocessor, not the compiler.
     */
    cvars_state().CompFlags.in_pragma = true;
    check_end = true;
    PPNextToken();
    if( IS_ID_OR_KEYWORD( cvars_state().CurToken ) && pragmaNameRecog( "include_alias" ) ) {
        pragIncludeAlias();
    } else if( cvars_state().CompFlags.cpp_mode ) {
        if( cvars_state().CompFlags.cpp_output ) {
            CppPuts( "#pragma " );
        }
        if( cvars_state().CurToken != T_NULL ) {
            CppPrtToken( cvars_state().CurToken );
            PPCTL_ENABLE_MACROS();
            for( cvars_state().CurToken = GetNextToken(); cvars_state().CurToken != T_NULL && cvars_state().CurToken != T_PRAGMA_END; cvars_state().CurToken = GetNextToken() ) {
                CppPrtToken( cvars_state().CurToken );
            }
            PPCTL_DISABLE_MACROS();
        }
    } else if( IS_ID_OR_KEYWORD( cvars_state().CurToken ) ) {
        if( pragmaNameRecog( "on" ) ) {
            pragOptions( 1 );
        } else if( pragmaNameRecog( "off" ) ) {
            pragOptions( 0 );
        } else if( pragmaNameRecog( "pop" ) ) {
            pragOptions( -1 );
        } else if( pragmaNameRecog( "aux" ) || pragmaNameRecog( "linkage" ) ) {
            PragAux();
        } else if( pragmaNameRecog( "library" ) ) {
            pragLibs();
        } else if( pragmaNameRecog( "comment" ) ) {
            pragComment();
        } else if( pragmaNameRecog( "pack" ) ) {
            pragPack();
        } else if( pragmaNameRecog( "alloc_text" ) ) {
            pragAllocText();
        } else if( pragmaNameRecog( "code_seg" ) ) {
            pragCodeSeg();
        } else if( pragmaNameRecog( "data_seg" ) ) {
            pragDataSeg();
        } else if( pragmaNameRecog( "warning" ) ) {
            if( pragWarning() ) {
                /*
                 * ignore #pragma warning
                 * skip rest of line
                 */
                check_end = false;
            }
        } else if( pragmaNameRecog( "disable_message" ) ) {
            pragEnableDisableMessage( false );
        } else if( pragmaNameRecog( "enable_message" ) ) {
            pragEnableDisableMessage( true );
        } else if( pragmaNameRecog( "message" ) ) {
            pragMessage();
        } else if( pragmaNameRecog( "intrinsic" ) ) {
            pragIntrinsic( 1 );
        } else if( pragmaNameRecog( "function" ) ) {
            pragIntrinsic( 0 );
        } else if( pragmaNameRecog( "enum" ) ) {
            pragEnum();
        } else if( pragmaNameRecog( "read_only_file" ) ) {
            pragReadOnlyFile();
        } else if( pragmaNameRecog( "read_only_directory" ) ) {
            pragReadOnlyDir();
        } else if( pragmaNameRecog( "once" ) ) {
            pragOnce();
        } else if( pragmaNameRecog( "unroll" ) ) {
            pragUnroll();
        } else if( pragmaNameRecog( "STDC" ) ) {
            pragSTDC();
        } else if( pragmaNameRecog( "extref" ) ) {
            pragExtRef();
        } else if( pragmaNameRecog( "alias" ) ) {
            pragAlias();
        } else {
            check_end = false;
        }
    } else {
        check_end = false;
    }
    endOfPragma( check_end );
    cvars_state().CompFlags.in_pragma = false;
}
