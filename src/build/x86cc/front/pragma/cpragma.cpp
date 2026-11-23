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

#include "clibext.h"
#include "frontrun.hpp"


static inline bool pragmaNameRecog( FrontRun& front, const char *what ) { return strcmp( front.cvars.Buffer, what ) == 0; }

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



struct magic_words_data {
    const char      *name;
    aux_info        *info;
};
namespace {

/* the statics of this file, one set for each run of the compiler */
}  // namespace
struct CpragmaState {
#define pick( x ) prag_stack *TOGGLE_STK( x );
#include "togdef.h"
#undef pick
    prag_stack *TOGGLE_STK( pack );
    prag_stack *TOGGLE_STK( enum );
    prag_stack *FreePrags;
    int AsmFuncNum;
    struct magic_words_data magicWords[10];
};

/* (the words name the built-in calling conventions of the run: set when it makes this part) */
static void cpragma_start( FrontRun& front, CpragmaState& part )
{
    const struct magic_words_data words[10] = {
    #define pick(a,b,c,d) { b, &d( front ) },
    #include "auxinfo.h"
    #undef pick
    };

    for( int i = 0; i < 10; ++i ) {
        part.magicWords[i] = words[i];
    }
}
namespace {

}  // namespace


/* the stacks of pushed toggles are members of the state, which the macro that named the statics now names */
#undef TOGGLE_STK
#define TOGGLE_STK( front, x )     front.cpragma->HeadToggles_tog_##x


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

static void freeStack( FrontRun& front, prag_stack **header )
/******************************************/
{
    prag_stack  *element;

    while( (element = *header) != NULL ) {
        *header = element->next;
        CMemFree( front, element );
    }
}

static void pushPrag( FrontRun& front, prag_stack **header, unsigned value )
/*********************************************************/
{
    prag_stack *stack_entry;

    stack_entry = stackPop( &front.cpragma->FreePrags );
    if( stack_entry == NULL ) {
        stack_entry = static_cast<prag_stack *>(CMemAlloc( front, sizeof( *stack_entry ) ));
    }
    stack_entry->value = value;
    stackPush( header, stack_entry );
}

static bool popPrag( FrontRun& front, prag_stack **header, unsigned *pvalue )
/**********************************************************/
{
    prag_stack *stack_entry;

    stack_entry = stackPop( header );
    if( stack_entry != NULL ) {
        if( pvalue != NULL ) {
            *pvalue = stack_entry->value;
        }
        stackPush( &front.cpragma->FreePrags, stack_entry );
        return( true );
    }
    return( false );
}

void CPragmaInit( FrontRun& front )
/**********************/
{
    #define pick( x ) TOGGLE_STK( front, x ) = NULL;
    #include "togdef.h"
    #undef pick
    TOGGLE_STK( front, pack ) = NULL;
    TOGGLE_STK( front, enum ) = NULL;
    front.cpragma->FreePrags = NULL;
    front.cpragma->AsmFuncNum = 0;

    front.cvars.TextSegList = NULL;
    front.cvars.AliasHead = NULL;
    front.cvars.HeadLibs = NULL;
    front.cvars.ExtrefInfo = NULL;

    PragmaAuxInit(front);

    PragmaInit(front);
}

void CPragmaFini( FrontRun& front )
/**********************/
{
    void    *junk;

    PragmaFini();

    PragmaAuxFini(front);

    #define pick( x )   freeStack( front, &TOGGLE_STK( front, x ) );
    #include "togdef.h"
    #undef pick
    freeStack( front, &TOGGLE_STK( front, pack ) );
    freeStack( front, &TOGGLE_STK( front, enum ) );
    freeStack( front, &front.cpragma->FreePrags );

    while( (junk = front.cvars.TextSegList) != NULL ) {
        front.cvars.TextSegList = front.cvars.TextSegList->next;
        CMemFree( front, junk );
    }

    while( (junk = front.cvars.HeadLibs) != NULL ) {
        front.cvars.HeadLibs = front.cvars.HeadLibs->next;
        CMemFree( front, junk );
    }

    while( (junk = front.cvars.AliasHead) != NULL ) {
        front.cvars.AliasHead = front.cvars.AliasHead->next;
        CMemFree( front, junk );
    }

    while( (junk = front.cvars.ExtrefInfo) != NULL ) {
        front.cvars.ExtrefInfo = front.cvars.ExtrefInfo->next;
        CMemFree( front, junk );
    }
}

static void endOfPragma( FrontRun& front, bool check_end )
/***************************************/
{
    if( check_end ) {
        if( front.cvars.CurToken == T_SEMI_COLON )
            PPNextToken(front);
        if( front.cvars.CurToken == T_PRAGMA_END )
            return;
        ExpectingToken( front, T_NULL );
        while( front.cvars.CurToken != T_NULL && front.cvars.CurToken != T_EOF ) {
            PPNextToken(front);
        }
    }
}

const char *SkipUnderscorePrefix( FrontRun& front, const char *str )
/*************************************************/
{
    if( front.cvars.CompFlags.non_iso_compliant_names_enabled ) {
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

bool PragRecogId( FrontRun& front, const char *what )
/**********************************/
{
    bool        ok;
    const char  *p;

    ok = IsIdOrKeyword( front.cvars.CurToken );
    if( ok ) {
        p = SkipUnderscorePrefix( front, front.cvars.Buffer );
        ok = ( p != NULL && stricmp( p, what ) == 0 );
        if( ok ) {
            PPNextToken(front);
        }
    }
    return( ok );
}

bool PragRecogName( FrontRun& front, const char *what )
/************************************/
{
    bool    ok;

    ok = IsIdOrKeyword( front.cvars.CurToken );
    if( ok ) {
        ok = pragmaNameRecog( front, what );
        if( ok ) {
            PPNextToken(front);
        }
    }
    return( ok );
}

static void advanceToken( FrontRun& front )
/******************************/
{
    CMemFree( front, front.cvars.SavedId );
    front.cvars.SavedId = NULL;
    front.cvars.CurToken = front.cvars.LAToken;
}

bool GetPragmaAuxAliasInfo( FrontRun& front )
/********************************/
{
    if( front.cvars.CurToken != T_LEFT_PAREN )          /* #pragma aux symbol ..... */
        return( IsIdOrKeyword( front.cvars.CurToken ) );
    PPNextToken(front);
    if( !IsIdOrKeyword( front.cvars.CurToken ) )     /* error */
        return( false );
    LookAhead(front);
    if( front.cvars.LAToken == T_RIGHT_PAREN ) {        /* #pragma aux (alias) symbol ..... */
        front.pragdefn.CurrAlias = PragmaAuxAlias( front, front.cvars.SavedId );
        advanceToken(front);
        PPNextToken(front);
        return( IsIdOrKeyword( front.cvars.CurToken ) );
    } else if( front.cvars.LAToken == T_COMMA ) {       /* #pragma aux (symbol, alias) */
        SetCurrInfo( front, front.cvars.SavedId );
        advanceToken(front);
        PPNextToken(front);
        if( !IsIdOrKeyword( front.cvars.CurToken ) )
            return( false );                /* error */
        GetPragmaAuxAlias(front);
        return( false );                    /* process no more! */
    } else {                                /* error */
        advanceToken(front);
        return( false );                    /* shut up the compiler */
    }
}

void *AsmQuerySymbol( FrontRun& front, const char *name )
/**************************************/
{
    return( SymLook( front, CalcHashID( name ), name ) );
}

enum sym_state AsmQueryState( FrontRun& front, void *handle )
/******************************************/
{
    SYM_HANDLE  sym_handle = (SYM_HANDLE)handle;
    SYM_ENTRY   sym;

    if( sym_handle == SYM_NULL )
        return( SYM_UNDEFINED );
    SymGet( front, &sym, sym_handle );
    if( (sym.flags & SYM_REFERENCED) == 0 ) {
        sym.flags |= SYM_REFERENCED;
        SymReplace( front, &sym, sym_handle );
    }
    switch( sym.attribs.stg_class ) {
    case SC_AUTO:
    case SC_REGISTER:
        return( SYM_STACK );
    }
    return( SYM_EXTERNAL );
}

static aux_info *lookupMagicKeyword( FrontRun& front, const char *name )
/*****************************************************/
{
    magic_words     mword;

    if( (name = SkipUnderscorePrefix( front, name )) != NULL ) {
        for( mword = static_cast<magic_words>(0); mword < M_SIZE; mword++ ) {
            if( strcmp( name, front.cpragma->magicWords[mword].name + 2 ) == 0 ) {
                return( front.cpragma->magicWords[mword].info );
            }
        }
    }
    return( NULL );
}


void CreateAux( FrontRun& front, const char *id )
/******************************/
{
    front.pragdefn.CurrEntry = (aux_entry *)CMemAlloc( front, offsetof( aux_entry, name ) + strlen( id ) + 1 );
    strcpy( front.pragdefn.CurrEntry->name, id );
}


void CreateAuxInlineFunc( FrontRun& front, bool too_many_bytes )
/*********************************************/
{
    char        name[10];

    sprintf( name, "F.%d", front.cpragma->AsmFuncNum );
    ++front.cpragma->AsmFuncNum;
    CreateAux( front, name );
    front.pragdefn.CurrInfo = (aux_info *)CMemAlloc( front, sizeof( aux_info ) );
    front.pragdefn.CurrEntry->info = front.pragdefn.CurrInfo;
    *front.pragdefn.CurrInfo = WatcallInfo( front );
    front.pragdefn.CurrInfo->use = 1;
    front.pragdefn.CurrInfo->save = front.pdefn2.AsmRegsSaved;  // indicate no registers saved
    if( !too_many_bytes ) {
        if( AsmInsertFixups( front, front.pragdefn.CurrInfo ) ) {
            AsmUsesAuto( front.pragdefn.CurrInfo );
        }
    }
    front.pragdefn.CurrEntry->next = front.pragdefn.AuxList;
    front.pragdefn.AuxList = front.pragdefn.CurrEntry;
}


void SetCurrInfo( FrontRun& front, const char *name )
/**********************************/
{
    SYM_HANDLE      sym_handle;
    SYM_ENTRY       sym;
    type_modifiers  sym_attrib = FLAG_NONE;

    front.pragdefn.CurrInfo = lookupMagicKeyword( front, name );
    if( front.pragdefn.CurrInfo == NULL ) {
        if( front.pragdefn.CurrAlias == NULL ) {
            sym_handle = SymLook( front, CalcHashID( name ), name );
            if( sym_handle != SYM_NULL ) {
                SymGet( front, &sym, sym_handle );
                sym_attrib = sym.mods;
            }
            front.pragdefn.CurrAlias = GetLangInfo( front, sym_attrib );
        }
        CreateAux( front, name );
    } else {
        if( front.pragdefn.CurrAlias == NULL ) {
            front.pragdefn.CurrAlias = front.pragdefn.CurrInfo;
        }
    }
}


aux_info *PragmaAuxAlias( FrontRun& front, const char *name )
/******************************************/
{
    aux_entry   *aux;
    aux_info    *info;

    info = lookupMagicKeyword( front, name );
    if( info == NULL ) {
        aux = AuxLookup( front, name );
        if( aux != NULL ) {
            info = aux->info;
        }
    }
    return( info );
}


void XferPragInfo( FrontRun& front, const char *from, const char *to )
/***************************************************/
{
    aux_entry *ent;

    if( AuxLookup( front, to ) != NULL )
        return;
    ent = AuxLookup( front, from );
    if( ent == NULL )
        return;
    CreateAux( front, to );
    front.pragdefn.CurrEntry->info = ent->info;
    ent->info->use++;
    front.pragdefn.CurrEntry->next = front.pragdefn.AuxList;
    front.pragdefn.AuxList = front.pragdefn.CurrEntry;
}


static void CopyLinkage( void )
/*****************************/
{
}


static void CopyParms( FrontRun& front )
/***************************/
{
    size_t      size;
    hw_reg_set  *regs;

    if( front.pragdefn.CurrInfo->parms != front.pragdefn.CurrAlias->parms )
        return;
    if( IsAuxParmsBuiltIn( front, front.pragdefn.CurrInfo->parms ) )
        return;
    size = sizeof( hw_reg_set );
    for( regs = front.pragdefn.CurrInfo->parms; !HW_CEqual( *regs, HW_EMPTY ); ++regs ) {
        size += sizeof( hw_reg_set );
    }
    regs = (hw_reg_set *)CMemAlloc( front, size );
    memcpy( regs, front.pragdefn.CurrInfo->parms, size );
    front.pragdefn.CurrInfo->parms = regs;
}

static void CopyCode( FrontRun& front )
/**************************/
{
    byte_seq    *code;
    unsigned    size;

    /*
     * TODO deal with reloc list
     */
    if( front.pragdefn.CurrInfo->code == NULL )
        return;
    if( front.pragdefn.CurrInfo->code != front.pragdefn.CurrAlias->code )
        return;
    size = offsetof( byte_seq, data ) + front.pragdefn.CurrInfo->code->length;
    code = (byte_seq *)CMemAlloc( front, size );
    memcpy( code, front.pragdefn.CurrInfo->code, size );
    front.pragdefn.CurrInfo->code = code;
}

static void CopyObjName( FrontRun& front )
/*****************************/
{
    if( front.pragdefn.CurrInfo->objname == NULL )
        return;
    if( front.pragdefn.CurrInfo->objname != front.pragdefn.CurrAlias->objname )
        return;
    front.pragdefn.CurrInfo->objname = CMemStrdup( front, front.pragdefn.CurrInfo->objname );
}


void PragmaAuxEnding( FrontRun& front )
/**************************/
{
    if( front.pragdefn.CurrEntry == NULL )
        return;
    front.pragdefn.CurrInfo->use = front.pragdefn.CurrAlias->use; /* for compare */
    if( memcmp( front.pragdefn.CurrAlias, front.pragdefn.CurrInfo, sizeof( aux_info ) ) == 0 ) {
        front.pragdefn.CurrEntry->info = front.pragdefn.CurrAlias;
        front.pragdefn.CurrAlias->use++;
        CMemFree( front, front.pragdefn.CurrInfo );
    } else {
        CopyParms(front);
        CopyLinkage();
        CopyCode(front);
        CopyObjName(front);
        front.pragdefn.CurrInfo->use = 1;
        front.pragdefn.CurrEntry->info = front.pragdefn.CurrInfo;
    }
    /*
     * If this pragma defines code, check to see if we already have a function body
     */
    if( front.pragdefn.CurrEntry != NULL && front.pragdefn.CurrEntry->info != NULL && front.pragdefn.CurrEntry->info->code != NULL ) {
        SYM_HANDLE  sym_handle;
        SYM_ENTRY   sym;

        if( SYM_NULL != (sym_handle = SymLook( front, CalcHashID( front.pragdefn.CurrEntry->name ), front.pragdefn.CurrEntry->name )) ) {
            SymGet( front, &sym, sym_handle );
            if( ( sym.flags & SYM_DEFINED ) && ( sym.flags & SYM_FUNCTION ) ) {
                CErr2p( front, ERR_SYM_ALREADY_DEFINED, front.pragdefn.CurrEntry->name );
            }
        }
    }

    front.pragdefn.CurrEntry->next = front.pragdefn.AuxList;
    front.pragdefn.AuxList = front.pragdefn.CurrEntry;
}


void PragObjNameInfo( FrontRun& front, char **objname )
/************************************/
{
    if( front.cvars.CurToken == T_STRING ) {
        *objname = CMemStrdup( front, front.cvars.Buffer );
        PPNextToken(front);
    }
}

TOKEN PragRegSet( FrontRun& front )
/**********************/
{
    if( front.cvars.CurToken == T_LEFT_BRACKET )
        return( T_RIGHT_BRACKET );
    if( front.cvars.CurToken == T_LEFT_BRACE )
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

void PragRegNameErr( FrontRun& front, const char *regname )
/****************************************/
{
    CErr2p( front, ERR_BAD_REGISTER_NAME, regname );
}

hw_reg_set PragRegList( FrontRun& front )
/****************************/
{
    hw_reg_set  res, reg;
    TOKEN       close;

    HW_CAsgn( res, HW_EMPTY );
    HW_CAsgn( reg, HW_EMPTY );
    close = PragRegSet(front);
    if( close != T_NULL ) {
        while( PPNextToken(front) != close ) {
            reg = PragReg(front);
            HwTurnOn( res, reg );
            if( front.cvars.CurToken == close ) {
                break;
            }
        }
        MustRecog( front, close );
    }
    return( res );
}

hw_reg_set *PragManyRegSets( FrontRun& front )
/*********************************/
{
    int         i;
    hw_reg_set  list;
    hw_reg_set  *sets;
    hw_reg_set  buff[MAXIMUM_PARMSETS + 1];

    for( i = 0, list = PragRegList(front); !HW_CEqual( list, HW_EMPTY ); list = PragRegList(front), ++i ) {
        if( i == MAXIMUM_PARMSETS ) {
            CErr1( front, ERR_TOO_MANY_PARM_SETS );
            break;
        }
        buff[i] = list;
    }
    HW_CAsgn( buff[i], HW_EMPTY );
    i = ( i + 1 ) * sizeof( hw_reg_set );
    sets = (hw_reg_set *)CMemAlloc( front, i );
    memcpy( sets, buff, i );
    return( sets );
}

/*******************************************************
 *
 *  #pragma .... processing
 *
 *******************************************************/

void SetToggleFlag( FrontRun& front, char const *name, int func, bool push )
/*********************************************************/
{
    #define pick( x ) \
        if( strcmp( name, #x ) == 0 ) { \
            if( func == -1 ) { \
                unsigned    value; \
                if( popPrag( front, &TOGGLE_STK( front, x ), &value ) ) { \
                    Toggles( front ).tog_##x = ( value != 0 ); \
                } \
            } else { \
                if( push ) { \
                    pushPrag( front, &TOGGLE_STK( front, x ), Toggles( front ).tog_##x ); \
                } \
                Toggles( front ).tog_##x = ( func != 0 ); \
            } \
            return; \
        }
    #include "togdef.h"
    #undef pick
}

static void pragOptions( FrontRun& front, int func )
/**********************************
 * forms:
 *
 *  #pragma on (<toggle name>)
 *  #pragma off (<toggle name>)
 *  #pragma pop (<toggle name>)
 */
{
    PPCtlEnableMacros( front );
    PPNextToken(front);
    if( ExpectingToken( front, T_LEFT_PAREN ) ) {
        for( PPNextToken(front); IsIdOrKeyword( front.cvars.CurToken ); PPNextToken(front) ) {
            SetToggleFlag( front, front.cvars.Buffer, func, true );
        }
        MustRecog( front, T_RIGHT_PAREN );
    }
    PPCtlDisableMacros( front );
}

void AddLibraryName( FrontRun& front, const char *libname, char priority )
/*******************************************************/
{
    library_list    **owner;
    library_list    *lib;

    for( owner = &front.cvars.HeadLibs; (lib = *owner) != NULL; owner = &lib->next ) {
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
        lib = static_cast<library_list *>(CMemAlloc( front, sizeof( library_list ) + strlen( libname ) ));
        strcpy( lib->libname, libname );
    }
    /*
     * set priority and insert library entry to proper position
     */
    lib->priority = priority;
    lib->next = *owner;
    *owner = lib;
}

static void GetLibraryNames( FrontRun& front )
/********************************/
{
    while( IsIdOrKeyword( front.cvars.CurToken ) || front.cvars.CurToken == T_STRING ) {
        AddLibraryName( front, front.cvars.Buffer, USER_LIB_PRIO );
        PPNextToken(front);
    }
}

static void pragLibs( FrontRun& front )
/***************************
 * forms:
 *
 *  #pragma library
 *  #pragma library (<libraries name list>)
 */
{
    PPCtlEnableMacros( front );
    PPNextToken(front);
    if( front.cvars.CurToken == T_LEFT_PAREN ) {
        PPNextToken(front);
        GetLibraryNames(front);
        MustRecog( front, T_RIGHT_PAREN );
    } else {
        front.cvars.CompFlags.pragma_library = true;
    }
    PPCtlDisableMacros( front );
}

static void pragComment( FrontRun& front )
/******************************
 * forms:
 *
 *  #pragma comment ( comment_type [, "comment_string"] )
 */
{
    PPCtlEnableMacros( front );
    PPNextToken(front);
    if( ExpectingToken( front, T_LEFT_PAREN ) ) {
        PPNextToken(front);
        if( PragRecogId( front, "lib" ) ) {
            if( ExpectingToken( front, T_COMMA ) ) {
                PPNextToken(front);
            }
            GetLibraryNames(front);
        }
        MustRecog( front, T_RIGHT_PAREN );
    }
    PPCtlDisableMacros( front );
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

void SetPackAmount( FrontRun& front, unsigned amount )
/***********************************/
{
    front.cvars.PackAmount = VerifyPackAmount( amount );
}

static void getPackArgs( FrontRun& front )
/****************************/
{
    /*
     * check to make sure it is a numeric token
     */
    if( PragRecogId( front, "push" ) ) {
        pushPrag( front, &TOGGLE_STK( front, pack ), (unsigned)front.cvars.PackAmount );
        if( front.cvars.CurToken == T_COMMA ) {
            PPNextToken(front);
            if( ExpectingConstant(front) ) {
                SetPackAmount( front, U32FetchTrunc( front.cvars.Constant64 ) );
            }
            PPNextToken(front);
        }
    } else if( PragRecogId( front, "pop" ) ) {
        unsigned    value;

        if( popPrag( front, &TOGGLE_STK( front, pack ), &value ) ) {
            front.cvars.PackAmount = (align_type)value;
        }
    } else {
        CErr1( front, ERR_NOT_A_CONSTANT_EXPR );
    }
}

static void pragPack( FrontRun& front )
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
    PPCtlEnableMacros( front );
    PPNextToken(front);
    if( ExpectingToken( front, T_LEFT_PAREN ) ) {
        PPNextToken(front);
        if( front.cvars.CurToken == T_CONSTANT ) {
            SetPackAmount( front, U32FetchTrunc( front.cvars.Constant64 ) );
            PPNextToken(front);
        } else if( IsIdOrKeyword( front.cvars.CurToken ) ) {
            getPackArgs(front);
        } else if( front.cvars.CurToken == T_RIGHT_PAREN ) {
            front.cvars.PackAmount = front.cvars.GblPackAmount;
        }
        MustRecog( front, T_RIGHT_PAREN );
    }
    PPCtlDisableMacros( front );
}

textsegment *NewTextSeg( FrontRun& front, const char *name, const char *suffix, const char *classname )
/************************************************************************************/
{
    textsegment     *tseg;
    size_t          len1;
    size_t          len2;
    size_t          len3;

    len1 = strlen( name );
    len2 = strlen( suffix );
    len3 = strlen( classname );
    tseg = static_cast<textsegment *>(CMemAlloc( front, offsetof( textsegment, segname ) + len1 + len2 + 1 + len3 + 1 ));
    tseg->class_ = len1 + len2 + 1;
    strcpy( tseg->segname, name );
    strcpy( tseg->segname + len1, suffix );
    strcpy( tseg->segname + tseg->class_, classname );
    tseg->next = front.cvars.TextSegList;
    front.cvars.TextSegList = tseg;
    return( tseg );
}

textsegment *LkSegName( FrontRun& front, const char *segname, const char *classname )
/******************************************************************/
{
    textsegment     *tseg;

    for( tseg = front.cvars.TextSegList; tseg != NULL; tseg = tseg->next ) {
        if( strcmp( tseg->segname, segname ) == 0 ) {
            if( strcmp( tseg->segname + tseg->class_, classname ) == 0 ) {
                return( tseg );
            }
        }
    }
    return( NewTextSeg( front, segname, "", classname ) );
}

static void pragAllocText( FrontRun& front )
/********************************
 * forms:
 *
 *  #pragma alloc_text ( seg_name, fn [, fn] )
 */
{
    struct textsegment  *tseg;
    SYM_HANDLE          sym_handle;
    SYM_ENTRY           sym;

    PPCtlEnableMacros( front );
    PPNextToken(front);
    if( ExpectingToken( front, T_LEFT_PAREN ) ) {
        PPNextToken(front);
        /*
         * current token can be an T_ID or a T_STRING
         */
        tseg = LkSegName( front, front.cvars.Buffer, "" );
        PPNextToken(front);
        for( ;; ) {
            MustRecog( front, T_COMMA );
            /*
             * current token can be an T_ID or a T_STRING
             */
            sym_handle = Sym0Look( front, CalcHashID( front.cvars.Buffer ), front.cvars.Buffer );
            if( sym_handle == SYM_NULL ) {
                /*
                 * error
                 */
            } else {
                SymGet( front, &sym, sym_handle );
                if( sym.flags & SYM_FUNCTION ) {
                    sym.seginfo = tseg;
                    SymReplace( front, &sym, sym_handle );
                } else {
                    /*
                     * error, must be function
                     */
                }
            }
            PPNextToken(front);
            if( front.cvars.CurToken == T_RIGHT_PAREN )
                break;
            if( front.cvars.CurToken == T_EOF )
                break;
            if( front.cvars.CurToken == T_NULL ) {
                break;
            }
        }
        front.cvars.CompFlags.multiple_code_segments = true;
        MustRecog( front, T_RIGHT_PAREN );
    }
    PPCtlDisableMacros( front );
}

static void changeLevel( FrontRun& front, unsigned level, int msg_index )
/******************************************************/
{
    if( front.cerrs.msg_level[msg_index].level != level ) {
        front.cerrs.msg_level[msg_index].level = level;
        if( level == WLEVEL_DISABLED ) {
            /*
             * disable message
             */
            if( front.cerrs.msg_level[msg_index].enabled ) {
                front.cerrs.msg_level[msg_index].enabled = false;
            }
        } else {
            /*
             * enable message
             */
            if( !front.cerrs.msg_level[msg_index].enabled ) {
                front.cerrs.msg_level[msg_index].enabled = true;
            }
        }
    }
}

static void changeStatus( FrontRun& front, bool enabled, int msg_index )
/*****************************************************/
{
    if( front.cerrs.msg_level[msg_index].enabled != enabled ) {
        front.cerrs.msg_level[msg_index].enabled = enabled;
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

static bool getMessageNum( FrontRun& front, msg_codes *val )
/*****************************************/
{
    if( front.cvars.CurToken == T_CONSTANT ) {
        *val = static_cast<msg_codes>(U32FetchTrunc( front.cvars.Constant64 ));
        return( true );
    } else if( front.cvars.CurToken == T_ID ) {
        return( GetMsgNum( front.cvars.Buffer, val ) );
    }
    return( false );
}

static void warnChangeLevel( FrontRun& front, unsigned level, msg_codes msgnum )
/**************************************************************
 * CHANGE WARNING LEVEL FOR A MESSAGE
 */
{
    unsigned    msg_index;

    if( msgnum == 0 )
        return;
    msg_index = GetMsgIndex( msgnum );
    if( IsMsgidxInvalid( msg_index ) ) {
        CWarn2( front, ERR_PRAG_WARNING_BAD_MESSAGE, msgnum );
        return;
    }
    switch( front.cerrs.msg_level[msg_index].type ) {
//    case MSG_TYPE_ERROR :       /* Error messages are used for Warning */
    case MSG_TYPE_INFO :
    case MSG_TYPE_ANSIERR :
        CWarn2( front, ERR_PRAG_WARNING_BAD_MESSAGE, msgnum );
        break;
    case MSG_TYPE_ERROR :       /* Error messages are used for Warning */
    case MSG_TYPE_WARNING :
    case MSG_TYPE_ANSI :
    case MSG_TYPE_ANSIWARN :
        changeLevel( front, level, msg_index );
        break;
    }
}

static void warnChangeLevels( FrontRun& front, unsigned level )
/*********************************************
 * CHANGE WARNING LEVELS FOR ALL MESSAGES
 */
{
    int     msg_index;          // - index for number

    for( msg_index = 0; msg_index < MESSAGE_COUNT; msg_index++ ) {
        switch( front.cerrs.msg_level[msg_index].type ) {
        case MSG_TYPE_ERROR :       /* Error messages are used for Warning */
        case MSG_TYPE_WARNING :
        case MSG_TYPE_ANSI :
        case MSG_TYPE_ANSIWARN :
            changeLevel( front, level, msg_index );
            break;
        }
    }
}

void WarnEnableDisable( FrontRun& front, bool enabled, msg_codes msgnum )
/******************************************************/
{
    unsigned    msg_index;

    if( msgnum == 0 )
        return;
    msg_index = GetMsgIndex( msgnum );
    if( IsMsgidxInvalid( msg_index ) ) {
        CWarn2( front, ERR_PRAG_WARNING_BAD_MESSAGE, msgnum );
        return;
    }
    switch( front.cerrs.msg_level[msg_index].type ) {
//    case MSG_TYPE_ERROR :       /* Error messages are used for Warning */
    case MSG_TYPE_INFO :
    case MSG_TYPE_ANSIERR :
        CWarn2( front, ERR_PRAG_WARNING_BAD_MESSAGE, msgnum );
        break;
    case MSG_TYPE_ERROR :       /* Error messages are used for Warning */
    case MSG_TYPE_WARNING :
    case MSG_TYPE_ANSI :
    case MSG_TYPE_ANSIWARN :
        changeStatus( front, enabled, msg_index );
        break;
    }
}

static bool pragWarning( FrontRun& front )
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
    PPCtlEnableMacros( front );
    NextToken(front);
    if( front.cvars.CurToken == T_TIMES ) {
        change_all = true;
    } else if( !getMessageNum( front, &msgnum ) ) {
        /*
         * ignore; MS or other vendor's #pragma
         */
        ignore = true;
    }
    if( !ignore ) {
        NextToken(front);
        if( front.cvars.CurToken == T_CONSTANT ) {
            level = U32FetchTrunc( front.cvars.Constant64 );
            NextToken(front);
            if( change_all ) {
                warnChangeLevels( front, level );
            } else {
                warnChangeLevel( front, level, msgnum );
            }
        } else {
            CWarn1( front, ERR_PRAG_WARNING_BAD_LEVEL );
            NextToken(front);
        }
    }
    PPCtlDisableMacros( front );
    return( ignore );
}

static void pragEnableDisableMessage( FrontRun& front, bool enabled )
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

    PPCtlEnableMacros( front );
    PPNextToken(front);
    if( ExpectingToken( front, T_LEFT_PAREN ) ) {
        PPNextToken(front);
        while( getMessageNum( front, &msgnum ) ) {
            WarnEnableDisable( front, enabled, msgnum );
            PPNextToken(front);
            if( front.cvars.CurToken == T_COMMA ) {
                PPNextToken(front);
            }
        }
        MustRecog( front, T_RIGHT_PAREN );
    }
    PPCtlDisableMacros( front );
}

static char *collectStrings( FrontRun& front, char *message )
{
    size_t  len;
    size_t  new_len;

    len = 0;
    while( front.cvars.CurToken == T_STRING ) {
        new_len = len + strlen( front.cvars.Buffer );
        message = static_cast<char *>(CMemRealloc( front, message, new_len + 1 ));
        strcpy( message + len, front.cvars.Buffer );
        len = new_len;
        PPNextToken(front);
    }
    return( message );
}

static void pragMessage( FrontRun& front )
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

    PPCtlEnableMacros( front );
    PPNextToken(front);
    if( ExpectingToken( front, T_LEFT_PAREN ) ) {
        PPNextToken(front);
        if( front.cvars.CurToken == T_STRING ) {
            message = static_cast<char *>(CMemAlloc( front, 1 ));
            message[0] = '\0';
            message = collectStrings( front, message );
            BannerMsg( front, message );
            CMemFree( front, message );
        }
        MustRecog( front, T_RIGHT_PAREN );
    }
    PPCtlDisableMacros( front );
}

static void pragEnum( FrontRun& front )
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
    PPCtlEnableMacros( front );
    PPNextToken(front);
    if( PragRecogId( front, "int" ) ) {
        pushPrag( front, &TOGGLE_STK( front, enum ), front.cvars.CompFlags.make_enums_an_int );
        front.cvars.CompFlags.make_enums_an_int = true;
    } else if( PragRecogId( front, "minimum" ) ) {
        pushPrag( front, &TOGGLE_STK( front, enum ), front.cvars.CompFlags.make_enums_an_int );
        front.cvars.CompFlags.make_enums_an_int = false;
    } else if( PragRecogId( front, "original" ) ) {
        pushPrag( front, &TOGGLE_STK( front, enum ), front.cvars.CompFlags.make_enums_an_int );
        front.cvars.CompFlags.make_enums_an_int = front.cvars.CompFlags.original_enum_setting;
    } else if( PragRecogId( front, "pop" ) ) {
        unsigned    value;

        if( popPrag( front, &TOGGLE_STK( front, enum ), &value ) ) {
            front.cvars.CompFlags.make_enums_an_int = ( value != 0 );
        }
    }
    PPCtlDisableMacros( front );
}

static void pragIntrinsic( FrontRun& front, int intrinsic )
/*****************************************
 * forms:
 *
 *  #pragma intrinsic ( fn [, fn] )
 */
{
    SYM_HANDLE  sym_handle;
    SYM_ENTRY   sym;

    PPCtlEnableMacros( front );
    PPNextToken(front);
    if( ExpectingToken( front, T_LEFT_PAREN ) ) {
        for( PPNextToken(front); IsIdOrKeyword( front.cvars.CurToken ); PPNextToken(front) ) {
            sym_handle = SymLook( front, CalcHashID( front.cvars.Buffer ), front.cvars.Buffer );
            if( sym_handle != SYM_NULL ) {
                SymGet( front, &sym, sym_handle );
                sym.flags &= ~ SYM_INTRINSIC;
                if( intrinsic )
                    sym.flags |= SYM_INTRINSIC;
                SymReplace( front, &sym, sym_handle );
            }
            PPNextToken(front);
            if( front.cvars.CurToken != T_COMMA ) {
                break;
            }
        }
        MustRecog( front, T_RIGHT_PAREN );
    }
    PPCtlDisableMacros( front );
}

static void pragCodeSeg( FrontRun& front )
/******************************
 * forms:
 *
 *  #pragma code_seg ( seg_name [, class_name] )
 */
{
    textsegment     *tseg;
    char            *segname;
    char            *classname;

    PPCtlEnableMacros( front );
    PPNextToken(front);
    if( front.cvars.CurToken == T_LEFT_PAREN ) {
        tseg = NULL;
        PPNextToken(front);
        if( ( front.cvars.CurToken == T_STRING ) || ( front.cvars.CurToken == T_ID ) ) {
            segname = CMemStrdup( front, front.cvars.Buffer );
            classname = CMemStrdup( front, "" );
            PPNextToken(front);
            if( front.cvars.CurToken == T_COMMA ) {
                PPNextToken(front);
                if( ( front.cvars.CurToken == T_STRING ) || ( front.cvars.CurToken == T_ID ) ) {
                    CMemFree( front, classname );
                    classname = CMemStrdup( front, front.cvars.Buffer );
//                    CodeClassName = CMemStrdup( Buffer ); */
                    PPNextToken(front);
                }
            }
            tseg = LkSegName( front, segname, classname );
            CMemFree( front, segname );
            CMemFree( front, classname );
        }
        MustRecog( front, T_RIGHT_PAREN );
        front.cvars.DefCodeSegment = tseg;
        front.cvars.CompFlags.multiple_code_segments = true;
    }
    PPCtlDisableMacros( front );
}

static void pragDataSeg( FrontRun& front )
/******************************
 * forms:
 *
 *  #pragma data_seg ( seg_name [, class_name] )
 */
{
    char        *segname;
    segment_id  segid;

    PPCtlEnableMacros( front );
    PPNextToken(front);
    if( front.cvars.CurToken == T_LEFT_PAREN ) {
        segid = SEG_NULL;
        PPNextToken(front);
        if( ( front.cvars.CurToken == T_STRING ) || ( front.cvars.CurToken == T_ID ) ) {
            segname = CMemStrdup( front, front.cvars.Buffer );
            PPNextToken(front);
            if( front.cvars.CurToken == T_COMMA ) {
                PPNextToken(front);
                if( ( front.cvars.CurToken == T_STRING ) || ( front.cvars.CurToken == T_ID ) ) {
                    segid = AddSegName( front, segname, front.cvars.Buffer, SEGTYPE_DATA );
                    PPNextToken(front);
                } else {
                    segid = AddSegName( front, segname, NULL, SEGTYPE_DATA );
                }
            } else {
                segid = AddSegName( front, segname, NULL, SEGTYPE_DATA );
            }
            CMemFree( front, segname );
        }
        MustRecog( front, T_RIGHT_PAREN );
        front.cvars.DefDataSegment = segid;
    }
    PPCtlDisableMacros( front );
}

static void pragUnroll( FrontRun& front )
/*****************************
 * forms:
 *
 *  #pragma unroll ( n )
 */
{
    unroll_type unroll_count;

    PPCtlEnableMacros( front );
    PPNextToken(front);
    if( ExpectingToken( front, T_LEFT_PAREN ) ) {
        unroll_count = 0;
        PPNextToken(front);
        if( front.cvars.CurToken == T_CONSTANT ) {
            if( U64CmpU32( front.cvars.Constant64, 255 ) > 0 ) {
                unroll_count = (unroll_type)255;
            } else {
                unroll_count = (unroll_type)U8FetchTrunc( front.cvars.Constant64 );
            }
            PPNextToken(front);
        }
        front.cvars.UnrollCount = unroll_count;
        MustRecog( front, T_RIGHT_PAREN );
    }
    PPCtlDisableMacros( front );
}

static void pragReadOnlyFile( FrontRun& front )
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
    PPCtlEnableMacros( front );
    PPNextToken(front);
    if( front.cvars.CurToken == T_STRING ) {
        do {
            SrcFileReadOnlyFile( front, front.cvars.Buffer );
            PPNextToken(front);
            if( front.cvars.CurToken == T_SEMI_COLON ) {
                PPNextToken(front);
            }
        } while( front.cvars.CurToken == T_STRING );
    } else {
        SrcFileReadOnlyFile( front, NULL );
    }
    PPCtlDisableMacros( front );
}


static void pragReadOnlyDir( FrontRun& front )
/**********************************
 * forms:
 *
 *  #pragma read_only_directory "directory"*
 *
 * causes all files within directory to be marked read-only
 */
{
    PPCtlEnableMacros( front );
    PPNextToken(front);
    while( front.cvars.CurToken == T_STRING ) {
        SrcFileReadOnlyDir( front, front.cvars.Buffer );
        PPNextToken(front);
        if( front.cvars.CurToken == T_SEMI_COLON ) {
            PPNextToken(front);
        }
    }
    PPCtlDisableMacros( front );
}

static void pragIncludeAlias( FrontRun& front )
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
    PPCtlEnableMacros( front );
    PPNextToken(front);
    if( ExpectingToken( front, T_LEFT_PAREN ) ) {
        PPNextToken(front);
        if( front.cvars.CurToken == T_STRING ) {
            char    *alias_name;

            alias_name = CMemStrdup( front, front.cvars.Buffer );
            PPNextToken(front);
            MustRecog( front, T_COMMA );
            if( front.cvars.CurToken == T_STRING ) {
                SrcFileIncludeAlias( front, alias_name, front.cvars.Buffer, false );
                PPNextToken(front);
            }
            CMemFree( front, alias_name );
        } else if( front.cvars.CurToken == T_LT ) {
            char    a_buf[82];    /* same size as CInclude() in cmac2.c */
            char    r_buf[82];

            a_buf[0] = '\0';
            for( ; PPNextToken(front) != T_NULL; ) {
                if( front.cvars.CurToken == T_GT ) {
                    PPNextToken(front);
                    break;
                }
                strncat( a_buf, front.cvars.Buffer, sizeof( a_buf ) - 2 );
            }
            MustRecog( front, T_COMMA );
            if( front.cvars.CurToken == T_LT ) {
                r_buf[0] = '\0';
                for( ; PPNextToken(front) != T_NULL; ) {
                    if( front.cvars.CurToken == T_GT ) {
                        PPNextToken(front);
                        break;
                    }
                    strncat( r_buf, front.cvars.Buffer, sizeof( r_buf ) - 2 );
                }
                SrcFileIncludeAlias( front, a_buf, r_buf, true );
            }
        }
        MustRecog( front, T_RIGHT_PAREN );
    }
    PPCtlDisableMacros( front );
}

static void pragOnce( FrontRun& front )
/***************************
 * forms:
 *
 *  #pragma once
 *
 * include file once
 */
{
    PPCtlEnableMacros( front );
    PPNextToken(front);
    SetSrcFNameOnce(front);
    PPCtlDisableMacros( front );
}

static void OptionPragSTDC( FrontRun& front )
/********************************/
{
    if( PragRecogName( front, "ON" ) ) {
    } else if( PragRecogName( front, "OFF" ) ) {
    } else if( PragRecogName( front, "DEFAULT" ) ) {
    }
}

static void pragSTDC( FrontRun& front )
/***************************
 * forms:
 *
 *  #pragma STDC (FP_CONTRACT|FENV_ACCESS|CX_LIMITED_RANGE) (ON|OFF|DEFAULT)
 */
{
    PPCtlDisableMacros( front );
    PPNextToken(front);
    if( PragRecogName( front, "FP_CONTRACT" ) ) {
        OptionPragSTDC(front);
    } else if( PragRecogName( front, "FENV_ACCESS" ) ) {
        OptionPragSTDC(front);
    } else if( PragRecogName( front, "CX_LIMITED_RANGE" ) ) {
        OptionPragSTDC(front);
    }
}

void AddExtRefN ( FrontRun& front, const char *name )
/**********************************/
{
    extref_info     **extref;
    extref_info     *new_extref;

    for( extref = &front.cvars.ExtrefInfo; *extref != NULL; extref = &(*extref)->next )
        ; /* nothing to do */
    new_extref = static_cast<extref_info *>(CMemAlloc( front, sizeof( extref_info ) + strlen( name ) ));
    strcpy( new_extref->name, name );
    new_extref->symbol = NULL;
    new_extref->next = NULL;
    *extref = new_extref;
}

void AddExtRefS ( FrontRun& front, SYM_HANDLE sym )
/********************************/
{
    extref_info  **extref;
    extref_info  *new_extref;

    for( extref = &front.cvars.ExtrefInfo; *extref != NULL; extref = &(*extref)->next )
        ; /* nothing to do */
    new_extref = static_cast<extref_info *>(CMemAlloc( front, sizeof( extref_info ) ));
    new_extref->symbol = sym;
    new_extref->name[0] = '\0';
    new_extref->next = NULL;
    *extref = new_extref;
}

static void parseExtRef ( FrontRun& front )
/******************************/
{
    SYM_HANDLE   extref_sym;

    if( front.cvars.CurToken == T_STRING ) {
        AddExtRefN( front, front.cvars.Buffer );
    } else {
        extref_sym = SymLook( front, CalcHashID( front.cvars.Buffer ), front.cvars.Buffer );
        if( extref_sym != NULL ) {
            AddExtRefS( front, extref_sym );
        } else {
            CErr2p( front, ERR_UNDECLARED_SYM, front.cvars.Buffer );
        }
    }
}

static void pragExtRef( FrontRun& front )
/*****************************
 * forms:
 *
 *  #pragma extref ( symbolid [, ...] )
 *  #pragma extref ( "symbolname" [, ...] )
 */
{
    PPCtlEnableMacros( front );
    PPNextToken(front);
    if( front.cvars.CurToken == T_LEFT_PAREN ) {
        do {
            PPNextToken(front);
            if( !IsIdOrKeyword( front.cvars.CurToken ) && front.cvars.CurToken != T_STRING )
                break;
            parseExtRef(front);
            PPNextToken(front);
        } while( front.cvars.CurToken == T_COMMA );
        MustRecog( front, T_RIGHT_PAREN );
    } else if( IsIdOrKeyword( front.cvars.CurToken ) || front.cvars.CurToken == T_STRING ) {
        parseExtRef(front);
        PPNextToken(front);
    }
    PPCtlDisableMacros( front );
}

static void pragAlias( FrontRun& front )
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

    PPCtlEnableMacros( front );
    PPNextToken(front);
    if( ExpectingToken( front, T_LEFT_PAREN ) ) {
        PPNextToken(front);
        if( front.cvars.CurToken == T_ID ) {
            alias_sym = SymLook( front, CalcHashID( front.cvars.Buffer ), front.cvars.Buffer );
            if( alias_sym == SYM_NULL ) {
                CErr2p( front, ERR_UNDECLARED_SYM, front.cvars.Buffer );
            }
        } else if( front.cvars.CurToken == T_STRING ) {
            alias_name = CMemStrdup( front, front.cvars.Buffer );
        }
        PPNextToken(front);
        MustRecog( front, T_COMMA );
        if( front.cvars.CurToken == T_ID ) {
            subst_sym = SymLook( front, CalcHashID( front.cvars.Buffer ), front.cvars.Buffer );
            if( subst_sym == 0 ) {
                CErr2p( front, ERR_UNDECLARED_SYM, front.cvars.Buffer );
            }
        } else if( front.cvars.CurToken == T_STRING ) {
            subst_name = CMemStrdup( front, front.cvars.Buffer );
        }
        PPNextToken(front);
        MustRecog( front, T_RIGHT_PAREN );
    }
    PPCtlDisableMacros( front );
    /*
     * Add a new alias record - if it's valid - to the list
     */
    if( ( alias_name != NULL || alias_sym != SYM_NULL ) && ( subst_name != NULL || subst_sym != SYM_NULL ) ) {
        for( alias = &front.cvars.AliasHead; *alias != NULL; alias = &(*alias)->next )
            ; /* nothing to do */
        new_alias = static_cast<alias_list *>((void *)CMemAlloc( front, sizeof( alias_list ) ));
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

void CPragma( FrontRun& front )
/******************/
{
    bool    check_end;

    /*
     * Note that the include_alias pragma must always be processed
     * because it's intended for the preprocessor, not the compiler.
     */
    front.cvars.CompFlags.in_pragma = true;
    check_end = true;
    PPNextToken(front);
    if( IsIdOrKeyword( front.cvars.CurToken ) && pragmaNameRecog( front, "include_alias" ) ) {
        pragIncludeAlias(front);
    } else if( front.cvars.CompFlags.cpp_mode ) {
        if( front.cvars.CompFlags.cpp_output ) {
            CppPuts( front, "#pragma " );
        }
        if( front.cvars.CurToken != T_NULL ) {
            CppPrtToken( front, front.cvars.CurToken );
            PPCtlEnableMacros( front );
            for( front.cvars.CurToken = GetNextToken(front); front.cvars.CurToken != T_NULL && front.cvars.CurToken != T_PRAGMA_END; front.cvars.CurToken = GetNextToken(front) ) {
                CppPrtToken( front, front.cvars.CurToken );
            }
            PPCtlDisableMacros( front );
        }
    } else if( IsIdOrKeyword( front.cvars.CurToken ) ) {
        if( pragmaNameRecog( front, "on" ) ) {
            pragOptions( front, 1 );
        } else if( pragmaNameRecog( front, "off" ) ) {
            pragOptions( front, 0 );
        } else if( pragmaNameRecog( front, "pop" ) ) {
            pragOptions( front, -1 );
        } else if( pragmaNameRecog( front, "aux" ) || pragmaNameRecog( front, "linkage" ) ) {
            PragAux(front);
        } else if( pragmaNameRecog( front, "library" ) ) {
            pragLibs(front);
        } else if( pragmaNameRecog( front, "comment" ) ) {
            pragComment(front);
        } else if( pragmaNameRecog( front, "pack" ) ) {
            pragPack(front);
        } else if( pragmaNameRecog( front, "alloc_text" ) ) {
            pragAllocText(front);
        } else if( pragmaNameRecog( front, "code_seg" ) ) {
            pragCodeSeg(front);
        } else if( pragmaNameRecog( front, "data_seg" ) ) {
            pragDataSeg(front);
        } else if( pragmaNameRecog( front, "warning" ) ) {
            if( pragWarning(front) ) {
                /*
                 * ignore #pragma warning
                 * skip rest of line
                 */
                check_end = false;
            }
        } else if( pragmaNameRecog( front, "disable_message" ) ) {
            pragEnableDisableMessage( front, false );
        } else if( pragmaNameRecog( front, "enable_message" ) ) {
            pragEnableDisableMessage( front, true );
        } else if( pragmaNameRecog( front, "message" ) ) {
            pragMessage(front);
        } else if( pragmaNameRecog( front, "intrinsic" ) ) {
            pragIntrinsic( front, 1 );
        } else if( pragmaNameRecog( front, "function" ) ) {
            pragIntrinsic( front, 0 );
        } else if( pragmaNameRecog( front, "enum" ) ) {
            pragEnum(front);
        } else if( pragmaNameRecog( front, "read_only_file" ) ) {
            pragReadOnlyFile(front);
        } else if( pragmaNameRecog( front, "read_only_directory" ) ) {
            pragReadOnlyDir(front);
        } else if( pragmaNameRecog( front, "once" ) ) {
            pragOnce(front);
        } else if( pragmaNameRecog( front, "unroll" ) ) {
            pragUnroll(front);
        } else if( pragmaNameRecog( front, "STDC" ) ) {
            pragSTDC(front);
        } else if( pragmaNameRecog( front, "extref" ) ) {
            pragExtRef(front);
        } else if( pragmaNameRecog( front, "alias" ) ) {
            pragAlias(front);
        } else {
            check_end = false;
        }
    } else {
        check_end = false;
    }
    endOfPragma( front, check_end );
    front.cvars.CompFlags.in_pragma = false;
}

/* the run makes and frees its copy of this file's variables (run/ files: see the run header) */
CpragmaState* cpragma_make(FrontRun& front) { CpragmaState* part = new CpragmaState(); cpragma_start(front, *part); return part; }
void cpragma_free(CpragmaState* part) { delete part; }
