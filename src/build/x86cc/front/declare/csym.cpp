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
* Description:  C compiler symbol table management.
*
****************************************************************************/


#include "cvars.h"
#include "cgswitch.h"
#include "caux.h"
#include "cmacadd.h"
#include "roundmac.h"
#include "frontrun.hpp"


#define SYM_BUF_SIZE_DEF    _1K
#define SYM_SEG_SIZE_DEF    _32K
#define SYM_MAX_INDEX_DEF   0xFFFFF

#define SYM_PER_BUF         (SYM_BUF_SIZE_DEF / sizeof( SYM_ENTRY ))
#define SYM_BUF_SIZE        (SYM_PER_BUF * sizeof( SYM_ENTRY ))
#define SYM_BUF_PER_SEG     (SYM_SEG_SIZE_DEF / SYM_BUF_SIZE_DEF)
#define SYM_SEG_SIZE        (SYM_BUF_SIZE * SYM_BUF_PER_SEG)
#define SYM_PER_SEG         (SYM_SEG_SIZE / sizeof( SYM_ENTRY ))
#define SYM_SEG_MAX         (SYM_MAX_INDEX_DEF / SYM_PER_SEG + 1)
#define SYM_MAX_INDEX       (SYM_SEG_MAX * SYM_PER_SEG)

#define CURR_SYM_HANDLE()   ((SYM_HANDLE)(pointer_uint)front.csym.NextSymHandle)



 /* contains actual pointer to block of memory */




namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct CsymState: csym_state.h) */

}  // namespace



static SEGADDR_T AllocSegment( FrontRun& front, seg_info *si )
{
    /*
     * FEmalloc never returns NULL - it either allocates the memory
     * or kills the compiler.
     */
    si->index = (SEGADDR_T)FEmalloc( front, SYM_SEG_SIZE );
    si->allocated = true;
    return( si->index );
}

static SEGADDR_T AccessSegment( FrontRun& front, seg_info *si )
{
    if( !si->allocated ) {
        AllocSegment( front, si );
    }
    return( si->index );
}

static void NewSym( FrontRun& front )
{
    ++front.csym.NextSymHandle;
    if( front.csym.NextSymHandle >= SYM_MAX_INDEX ) {
        CErr1( front, ERR_INTERNAL_LIMIT_EXCEEDED );
        CSuicide(front);
    }
}

void SymInit( FrontRun& front )
{
    id_hash_idx     hash;
    unsigned        seg_num;

    front.csym.NextSymHandle = 0;
    front.csym.Cached_sym_num = ~0u;
    front.cvars.SymLevel = 0;
    front.cvars.GblSymCount = 0;
    front.cvars.LclSymCount = 0;
    front.cvars.HashTab = static_cast<SYM_HASHPTR *>(PermMemAlloc( front, ID_HASH_SIZE * sizeof( SYM_HASHPTR ) ));
    for( hash = 0; hash < ID_HASH_SIZE; hash++ ) {
        front.cvars.HashTab[hash] = NULL;
    }
    front.cvars.TagHead = NULL;
    front.cvars.DeadTags = NULL;
    front.cvars.LabelHead = NULL;
    front.cvars.HashFreeList = NULL;
    front.csym.NextSymHandle = 0;
    front.cvars.ErrSym = (FIELDPTR)CPermAlloc( front, sizeof( FIELD_ENTRY ) );
    front.cvars.ErrSym->field_type = TypeDefault(front);
    front.cvars.SymBufNum = 0;
    front.csym.FirstSymInBuf = 0;
    front.cvars.LastSymBuf = 0;
    front.cvars.SymBufDirty = 0;
    front.cvars.CurFuncHandle = SYM_NULL;
    for( seg_num = 0; seg_num < SYM_SEG_MAX; ++seg_num ) {
        front.csym.SymBufSegs[seg_num].allocated = false;
    }
    front.csym.SymBufSegment = AllocSegment( front, &front.csym.SymBufSegs[0] );
    front.csym.SymBufPtr = (char *)front.csym.SymBufSegment;
    EnumInit(front);
}


void SymFini( FrontRun& front )
{
    unsigned    seg_num;
    seg_info    *si;

    for( seg_num = 0; seg_num < SYM_SEG_MAX; ++seg_num ) {
        si = &front.csym.SymBufSegs[seg_num];
        if( si->allocated ) {
            FEfree( front, si->index );
        }
    }
    if( front.cvars.CompFlags.extra_stats_wanted ) {
        printf( "SymStats: get = %u, rep = %u, read = %u, write = %u"
            ", typedef = %u\n",
        front.csym.SymStats.get, front.csym.SymStats.replace, front.csym.SymStats.read, front.csym.SymStats.write,
        front.cvars.SymTypedef );
    }
}

unsigned SymGetNumSyms( FrontRun& front )
{
    return( front.csym.NextSymHandle + 1 );
}

unsigned SymGetNumSpecialSyms( FrontRun& front )
{
    return( (unsigned)(pointer_uint)front.cvars.SpecialSyms );
}

SYM_HANDLE SymGetFirst( void )
{
    return( (SYM_HANDLE)0 );
}

SYM_HANDLE SymGetNext( FrontRun& front, SYM_HANDLE sym_handle )
{
    unsigned    handle = (unsigned)(pointer_uint)sym_handle;

    if( handle < front.csym.NextSymHandle ) {
        return( (SYM_HANDLE)(pointer_uint)( handle + 1 ) );
    } else {
        return( SYM_INVALID );
    }
}

// This is a temporary function, to be deleted later
void SetNextSymHandle( FrontRun& front, unsigned val )
{
    front.csym.NextSymHandle = val;
}

SYM_HANDLE SegSymbol( FrontRun& front, const char *name, segment_id segid )
{
    SYM_ENTRY       sym;
    SYM_HANDLE      handle;
    size_t          len;

    NewSym(front);
    handle = CURR_SYM_HANDLE();
    memset( &sym, 0, sizeof( SYM_ENTRY ) );
    len = strlen( name ) + 1;
    sym.name = static_cast<char *>(CMemAlloc( front, len + 1 ));
    sym.name[0] = '.';
    memcpy( &(sym.name[1]), name, len );
    sym.sym_type = GetType( front, TYP_USHORT );
    sym.attribs.stg_class = SC_STATIC;
    sym.level = 1;  // make invisible
    SymReplace( front, &sym, handle );
    if( segid != SEG_NULL ) {
        SetSegSymHandle( front, handle, segid );
    }
    return( handle );
}

SYM_HANDLE SpcSymbol( FrontRun& front, const char *name, TYPEPTR typ, stg_classes stg_class )
{
    SYM_ENTRY       sym;

    NewSym(front);
    memset( &sym, 0, sizeof( SYM_ENTRY ) );
    sym.name = CMemStrdup( front, name );
    sym.sym_type = typ;
    sym.attribs.stg_class = stg_class;
    SymReplace( front, &sym, CURR_SYM_HANDLE() );
    return( CURR_SYM_HANDLE() );
}

void SpcSymInit( FrontRun& front )
{
    TYPEPTR     ptr2char;
    TYPEPTR     typ;
    SYM_ENTRY   sym;

    /*
     * first entry into SpecialSyms
     */
    memset( &sym, 0, sizeof( SYM_ENTRY ) );
    sym.sym_type = GetType( front, TYP_VOID );
    SymReplace( front, &sym, 0 );
#ifdef __SEH__
    /*
     * create special _try sym
     */
    front.cvars.TrySymHandle = SpcSymbol( front, ".try", GetType( front, TYP_VOID ), SC_AUTO );
#endif
    /*
     * create special symbol for "extern unsigned int __near __chipbug"
     */
    front.cvars.SymChipBug = SpcSymbol( front, "__chipbug", GetType( front, TYP_UINT ), SC_EXTERN );
    /*
     * create special symbol table entry for __segname("_CODE")
     */
    front.cvars.Sym_CS = SegSymbol( front, "CS", SEG_CODE );
    /*
     * create special symbol table entry for __segname("_STACK")
     */
    front.cvars.Sym_SS = SegSymbol( front, "SS", SEG_STACK );
    /*
     * create special symbol table entry for __segname("_CONST")
     */
    SegSymbol( front, "CONST", SEG_CONST );
    /*
     * create special symbol table entry for __segname("_DATA")
     */
    SegSymbol( front, "DS", SEG_DATA );

    front.cvars.SpecialSyms = CURR_SYM_HANDLE();
    /*
     * Create special symbol table entries for use by stosw, stosb pragmas
     * This should be a TYP_FUNCTION returning pointer to char
     */
    ptr2char = PtrNode( front, GetType( front, TYP_CHAR ), FLAG_NONE, SEG_DATA );
    typ = TypeNode( front, TYP_FUNCTION, ptr2char );
    /*
     * The ".stosw" functions are done through internal AUX entries
     */
    front.cvars.SymSTOW  = MakeFunction( front, "_inline_.stosw", GetType( front, TYP_VOID ) );
    front.cvars.SymSTOWB = MakeFunction( front, "_inline_.stoswb", GetType( front, TYP_VOID ) );
    front.cvars.SymMIN   = MakeFunction( front, "_inline_.min", GetType( front, TYP_UINT ) );
    front.cvars.SymMAX   = MakeFunction( front, "_inline_.max", GetType( front, TYP_UINT ) );
    front.cvars.SymMEMCMP= MakeFunction( front, "_inline_memcmp", GetType( front, TYP_INT ) );
    typ = TypeNode( front, TYP_FUNCTION, GetType( front, TYP_INT ) );
#if _CPU == 386
    front.cvars.SymSTOD  = MakeFunction( front, "_inline_.stosd", GetType( front, TYP_VOID ) );
    front.cvars.SymSTOSB = MakeFunction( front, "__STOSB", GetType( front, TYP_VOID ) );
    front.cvars.SymSTOSD = MakeFunction( front, "__STOSD", GetType( front, TYP_VOID ) );
#endif
#ifdef __SEH__
    front.cvars.SymTryInit = MakeFunction( front, "__TryInit", typ );
    front.cvars.SymTryFini = MakeFunction( front, "__TryFini", typ );
    front.cvars.SymExcept  = MakeFunction( front, "__Except", typ );
    front.cvars.SymFinally = MakeFunction( front, "__Finally", typ );
    front.cvars.SymTryUnwind = MakeFunction( front, "__TryUnwind", typ );
#endif
    front.cvars.SymCover = MakeFunction( front, "__COVERAGE", typ );
    MakeFunction( front, "__C", typ );
    SymGet( front, &sym, CURR_SYM_HANDLE() );
    sym.attribs.stg_class = SC_STATIC;
    SymReplace( front, &sym, CURR_SYM_HANDLE() );
}


SYM_HANDLE MakeFunction( FrontRun& front, const char *id, TYPEPTR typ )
{
    SYM_ENTRY   sym;

    memset( &sym, 0, sizeof( SYM_ENTRY ) );
    sym.name = CMemStrdup( front, id );
    sym.attribs.stg_class = SC_EXTERN;
    sym.flags = SYM_FUNCTION;
    sym.handle = front.cvars.SpecialSyms;
    sym.sym_type = typ;
    NewSym(front);
    front.cvars.SpecialSyms = CURR_SYM_HANDLE();
    SymReplace( front, &sym, CURR_SYM_HANDLE() );
    return( CURR_SYM_HANDLE() );
}


void SymCreate( FrontRun& front, SYMPTR sym, const char *id )
{
    memset( sym, 0, sizeof( SYM_ENTRY ) );
    sym->name = CMemStrdup( front, id );
    sym->src_loc = front.cvars.TokenLoc;
}


static void SymAccess( FrontRun& front, unsigned sym_num )
{
    unsigned    buf_num;
    unsigned    seg_num;
    seg_info    *si;

    front.csym.Cached_sym_num = sym_num;
    if( sym_num < front.csym.FirstSymInBuf || sym_num >= front.csym.FirstSymInBuf + SYM_PER_BUF ) {
        buf_num = sym_num / SYM_PER_BUF;
        if( front.cvars.SymBufDirty ) {
            front.csym.SymStats.write++;
            si = &front.csym.SymBufSegs[front.cvars.SymBufSegNum];
            if( !si->allocated ) {
                front.csym.SymBufSegment = AllocSegment( front, si );
            }
            front.cvars.SymBufDirty = 0;
        }
        seg_num = buf_num / SYM_BUF_PER_SEG;
        si = &front.csym.SymBufSegs[seg_num];
        front.csym.SymBufSegment = AccessSegment( front, si );
        front.csym.SymBufPtr = (char *)front.csym.SymBufSegment + (buf_num % SYM_BUF_PER_SEG) * SYM_BUF_SIZE;
        front.cvars.SymBufNum = buf_num;
        front.cvars.SymBufSegNum = seg_num;
        front.csym.FirstSymInBuf = buf_num * SYM_PER_BUF;
    }
    front.csym.Cached_sym_addr = front.csym.SymBufPtr + (sym_num - front.csym.FirstSymInBuf) * sizeof( SYM_ENTRY );
}

SYMPTR SymGetPtr( FrontRun& front, SYM_HANDLE sym_handle )
{
    unsigned    handle = (unsigned)(pointer_uint)sym_handle;
    SYMPTR      symptr;

    front.csym.SymStats.getptr++;
    if( sym_handle == front.cvars.CurFuncHandle ) {
        symptr = &front.cvars.CurFuncSym;
    } else if( handle < front.cvars.PCH_MaxSymHandle ) {
        symptr = front.cvars.PCH_SymArray[handle];
    } else {
        if( handle != front.csym.Cached_sym_num ) {
            SymAccess( front, handle );
        }
        symptr = static_cast<SYMPTR>(front.csym.Cached_sym_addr);
    }
    return( symptr );
}

void SymGet( FrontRun& front, SYMPTR sym, SYM_HANDLE sym_handle )
{
    unsigned    handle = (unsigned)(pointer_uint)sym_handle;
    SYMPTR      symptr;

    front.csym.SymStats.get++;
    if( sym_handle == front.cvars.CurFuncHandle ) {
        symptr = &front.cvars.CurFuncSym;
    } else if( handle < front.cvars.PCH_MaxSymHandle ) {
        symptr = front.cvars.PCH_SymArray[handle];
    } else {
        if( handle != front.csym.Cached_sym_num ) {
            SymAccess( front, handle );
        }
        symptr = static_cast<SYMPTR>(front.csym.Cached_sym_addr);
    }
    memcpy( sym, symptr, sizeof( SYM_ENTRY ) );
}


void SymReplace( FrontRun& front, SYMPTR sym, SYM_HANDLE sym_handle )
{
    unsigned    handle = (unsigned)(pointer_uint)sym_handle;

    front.csym.SymStats.replace++;
    if( sym_handle == front.cvars.CurFuncHandle ) {
        memcpy( &front.cvars.CurFuncSym, sym, sizeof( SYM_ENTRY ) );
    }
    if( handle < front.cvars.PCH_MaxSymHandle ) {
        memcpy( front.cvars.PCH_SymArray[handle], sym, sizeof( SYM_ENTRY ) );
    } else {
        if( handle != front.csym.Cached_sym_num ) {
            SymAccess( front, handle );
        }
        memcpy( front.csym.Cached_sym_addr, sym, sizeof( SYM_ENTRY ) );
        front.cvars.SymBufDirty = 1;
    }
}


static SYM_HASHPTR SymHash( FrontRun& front, SYMPTR sym, SYM_HANDLE sym_handle )
{
    SYM_HASHPTR     hsym;
    TYPEPTR         typ;

    hsym = static_cast<SYM_HASHPTR>(PermMemAlloc( front, offsetof( id_hash_entry, name ) + strlen( sym->name ) + 1 ));
    hsym->sym_type = NULL;
    if( sym->attribs.stg_class == SC_TYPEDEF ) {
        typ = sym->sym_type;
        typ = TypeNode( front, TYP_TYPEDEF, typ );
        typ->u.typedefn = sym_handle;
        sym->sym_type = typ;
        hsym->sym_type = typ;
    }
    hsym->level = sym->level;
    strcpy( hsym->name, sym->name );
    CMemFree( front, sym->name );
    sym->name = NULL;
    hsym->handle = sym_handle;
    return( hsym );
}


SYM_HANDLE SymAdd( FrontRun& front, id_hash_idx hash, SYMPTR sym )
{
    SYM_HASHPTR     hsym;
    SYM_HASHPTR     *head;

    if( sym == NULL )
        return( SYM_NULL );
    if( front.cvars.SymLevel == 0 ) {
        ++front.cvars.GblSymCount;
    } else {
        ++front.cvars.LclSymCount;
    }
    NewSym(front);
    sym->level = (id_level_type)front.cvars.SymLevel;
    hsym = SymHash( front, sym, CURR_SYM_HANDLE() );
    sym->u1.hash = hash;
    /*
     * add name to head of list
     */
    for( head = &front.cvars.HashTab[hash]; *head != NULL; head = &(*head)->next_sym ) {
        if( CheckLtSymLevel( *head ) || CheckEqSymLevel( *head ) ) {
            break;
        }
    }
    hsym->next_sym = *head;     /* add name to head of list */
    *head = hsym;
    return( hsym->handle );
}


SYM_HANDLE SymAddL0( FrontRun& front, id_hash_idx hash, SYMPTR new_sym )
/******************************************************
 * add symbol to level 0
 */
{
    SYM_HASHPTR     hsym;
    SYM_HASHPTR     new_hsym;

    if( new_sym == NULL )
        return( SYM_NULL );
    ++front.cvars.GblSymCount;
    NewSym(front);
    new_sym->level = 0;
    new_hsym = SymHash( front, new_sym, CURR_SYM_HANDLE() );
    new_hsym->next_sym = NULL;
    new_sym->u1.hash = hash;
    hsym = front.cvars.HashTab[hash];
    if( hsym == NULL ) {
        front.cvars.HashTab[hash] = new_hsym;
    } else {
        while( hsym->next_sym != NULL ) {
            hsym = hsym->next_sym;
        }
        hsym->next_sym = new_hsym;      /* add new symbol to end of list */
    }
    return( new_hsym->handle );
}


static SYM_HANDLE GetNewDotSym( FrontRun& front, SYMPTR sym, char id, TYPEPTR typ, stg_classes stg_class )
{
    char            name[3];
    SYM_HANDLE      sym_handle;

    name[0] = '.';
    name[1] = id;
    name[2] = '\0';
    SymCreate( front, sym, name );
    sym_handle = SymAdd( front, 0, sym );
    sym->sym_type = typ;
    sym->attribs.stg_class = stg_class;
    sym->flags |= SYM_REFERENCED | SYM_TEMP;
    return( sym_handle );
}


SYM_HANDLE MakeNewDotSym( FrontRun& front, SYMPTR sym, char id, TYPEPTR typ, stg_classes stg_class )
{
    SYM_HANDLE  sym_handle;

    sym_handle = GetNewDotSym( front, sym, id, typ, stg_class );
    if( front.cvars.SymLevel != 0 ) {
        sym->handle = front.cvars.CurFunc->u.func.locals;
        front.cvars.CurFunc->u.func.locals = sym_handle;
    }
    ++front.cvars.TmpSymCount;
    return( sym_handle );
}


SYM_HANDLE SymLook( FrontRun& front, id_hash_idx hash, const char *id )
{
    SYM_HASHPTR     hsym;

    for( hsym = front.cvars.HashTab[hash]; hsym != NULL; hsym = hsym->next_sym ) {
        if( strcmp( hsym->name, id ) == 0 ) {
            return( hsym->handle );
        }
    }
    return( SYM_NULL );
}


SYM_HANDLE SymLookTypedef( FrontRun& front, id_hash_idx hash, const char *id, SYMPTR sym )
{
    SYM_HASHPTR     hsym;

    for( hsym = front.cvars.HashTab[hash]; hsym != NULL; hsym = hsym->next_sym ) {
        if( strcmp( hsym->name, id ) == 0 ) {
            if( hsym->sym_type == NULL )
                break;
            sym->sym_type = hsym->sym_type;
            sym->attribs.stg_class = SC_TYPEDEF;
            sym->level = hsym->level;
            return( hsym->handle );
        }
    }
    return( SYM_NULL );
}


SYM_HANDLE Sym0Look( FrontRun& front, id_hash_idx hash, const char *id )
/******************************************************
 * look for symbol on level 0
 */
{
    SYM_HASHPTR     hsym;

    for( hsym = front.cvars.HashTab[hash]; hsym != NULL; hsym = hsym->next_sym ) {
        if( strcmp( hsym->name, id ) == 0 ) {  /* name matches */
            if( hsym->level == 0 ) {
                return( hsym->handle );
            }
        }
    }
    return( SYM_NULL );
}


static void CheckReference( FrontRun& front, SYMPTR sym, SYM_NAMEPTR name )
{
    TYPEPTR     typ;

    if( sym->flags & SYM_DEFINED ) {
        if( sym->attribs.stg_class != SC_EXTERN ) {
            if( (sym->flags & SYM_REFERENCED) == 0 ) {
                if( (sym->flags & SYM_IGNORE_UNREFERENCE) == 0 ) {
                    if( sym->attribs.is_parm ) {
                        CWarn2p( front, ERR_PARM_NOT_REFERENCED, name );
                    } else {
                        CWarn2p( front, ERR_SYM_NOT_REFERENCED, name );
                    }
                }
            } else if( (sym->flags & SYM_ASSIGNED) == 0 ) {
                typ = sym->sym_type;
                SKIP_TYPEDEFS( typ );
                if( sym->attribs.stg_class != SC_STATIC && typ->decl_type != TYP_ARRAY ) {
                    CWarn2p( front, ERR_SYM_NOT_ASSIGNED, name );
                }
            }
        }
    }
}


static void CheckIncomplete( FrontRun& front, SYMPTR sym, SYM_NAMEPTR name )
{
    TYPEPTR     typ;

    if( sym->attribs.stg_class != SC_TYPEDEF ) {
        if( (sym->flags & (SYM_FUNCTION | SYM_TEMP)) == 0 ) {
            if( (sym->flags & SYM_REFERENCED) == 0 ) {
                /*
                 * if it wasn't referenced, don't worry
                 */
                if( sym->attribs.stg_class == SC_EXTERN ) {
                    return;
                }
            }
            typ = sym->sym_type;
            SKIP_TYPEDEFS( typ );
            if( SizeOfArg( front, typ ) == 0 && typ->decl_type != TYP_FUNCTION
              && typ->decl_type != TYP_DOT_DOT_DOT ) {
                if( sym->attribs.stg_class != SC_EXTERN ) {
                    CErr2p( front, ERR_INCOMPLETE_TYPE, name );
                }
            }
        }
    }
}


static void CheckDefined( FrontRun& front, SYMPTR sym, SYM_NAMEPTR name )
{
    if( sym->flags & SYM_DEFINED ) {
        if( sym->attribs.stg_class == SC_STATIC ) {
            if( (sym->flags & SYM_REFERENCED) == 0 ) {
                if( (sym->flags & SYM_IGNORE_UNREFERENCE) == 0 ) {
                    CWarn2p( front, ERR_SYM_NOT_REFERENCED, name );
                }
            }
        }
    } else {    /* not defined */
        if( sym->flags & SYM_REFERENCED ) {
            if( sym->attribs.stg_class == SC_STATIC ) {
                if( sym->flags & SYM_FUNCTION ) {
                    /*
                     * Check to see if we have a matching aux entry with code attached
                     */
                    aux_entry   *paux = AuxLookup( front, name );
                    if( !paux || !paux->info || !paux->info->code ) {
                        CErr2p( front, ERR_FUNCTION_NOT_DEFINED, name );
                    }
                }
            } else if( sym->attribs.stg_class == SC_FORWARD ) {
                sym->attribs.stg_class = SC_EXTERN;
            }
        }
    }
}

static void CheckFunction( FrontRun& front, SYMPTR sym, SYM_NAMEPTR name )
{
#if _INTEL_CPU
    if( sym->attribs.stg_class == SC_STATIC ) {
        if( sym->flags & SYM_ADDR_TAKEN ) {
            if( front.cvars.CompFlags.using_overlays ) {
                CWarn2p( front, ERR_ADDR_OF_STATIC_FUNC_TAKEN, name );
            }
        } else {
            if( (sym->mods & (FLAG_FAR | FLAG_NEAR)) == 0
              && (front.cvars.TargetSwitches & CGSW_X86_BIG_CODE)
              && !front.cvars.CompFlags.multiple_code_segments ) {
                sym->mods |= FLAG_NEAR;
            }
        }
    }
#else /* _RISC_CPU */

    /* unused parameters */ (void)sym; (void)name;

#endif
}

#if _CPU == 370
/*** External name control for IBM 370 restrictions ***/
struct xlist {
    struct xlist    *next;
    char            xname[8+1];
};

static  void InitExtName( struct xlist **where  )
/*** Init extern name list***/
{
    *where = NULL;
}

static void CheckExtName( struct xlist **link, SYMPTR sym, SYM_NAMEPTR name )
/****************************************************************************
 * Restricted extern names i.e 8 char upper check
 */
{
    struct xlist    *new_;
    struct xlist    *curr;
    int             i;
    int             c;

    new_ =  CMemAlloc( sizeof ( struct xlist ) );
    for( i = 0; i < 8 && (c = name[i]) != 0; i++ ) {
        new_->xname[i] = (char)tolower( c );
    }
    new_->xname[i} = '\0';
    for( ; (curr = *link) != NULL; link = &curr->next ) {
        int cmp;

        cmp = strcmp( new_->xname, curr->xname );
        if( cmp == 0 ) {
            SetErrLoc( &sym->src_loc );
            CErr3p( ERR_DUPLICATE_ID, name, new_->xname );
            InitErrLoc();
            CMemFree( new_ );
            return;
        } else if( cmp < 0 ) {
            break;
        }
    }
    new_->next = *link;
    *link = new_;
}

static void FiniExtName( struct xlist *head )
/*** Free xname list **********************/
{
    struct xlist    *next;

    while( head != NULL ) {
        next = head->next;
        CMemFree( head );
        head = next;
    }
}
#endif /* IBM370 names */

#define BUCKETS 7

static int SymBucket( FrontRun& front, SYMPTR sym )
/*********************************
 * determine bucket # for symbol
 *
 * divide all the global symbols into buckets based on size of the item
 * 0 - functions
 * 1 - 1-byte items
 * 2 - odd-length items
 * 3 - 2-byte items
 * 4 - even-length items (that are not a multiple of 4 in size)
 * 5 - 4-byte items (or multiple of 4, but not a multiple of 8)
 * 6 - 8-byte items (or multiple of 8)
 */
{
    int             bucket;
    unsigned        size;

    bucket = 0; /* assume its a function */
    if( (sym->flags & SYM_FUNCTION) == 0 ) {    /* if variable */
        size = SizeOfArg( front, sym->sym_type );
        switch( size % 8 ) {
        case 0:                 /* multiple of 8 */
            bucket = 6;
            break;
        case 4:                 /* multiple of 4 */
            bucket = 5;
            break;
        case 1:
        case 3:
        case 5:
        case 7:
            bucket = 2;             /* odd length objects */
            if( size == 1 )
                bucket = 1;
            break;
        case 2:
        case 6:
            bucket = 4;             /* even length objects */
            if( size == 2 )
                bucket = 3;
            break;
        }
    }
    return( bucket );
}

static SYM_HASHPTR GetSymList( FrontRun& front )
{
    SYM_HASHPTR     hsym;
    SYM_HASHPTR     next_hsymptr;
    SYM_HASHPTR     sym_list;
    SYM_HASHPTR     sym_tail;
    id_hash_idx     hash;
    unsigned        i;
    unsigned        j;
    SYM_HASHPTR     sym_seglist[SYM_SEG_MAX];
    SYM_HASHPTR     sym_buflist[SYM_BUF_PER_SEG];
    SYM_HASHPTR     sym_buftail[SYM_BUF_PER_SEG];

    sym_list = NULL;
    for( hash = 0; hash < ID_HASH_SIZE; hash++ ) {
        for( hsym = front.cvars.HashTab[hash]; hsym != NULL; hsym = next_hsymptr ) {
            if( !CheckEqSymLevel( hsym ) )
                break;
            next_hsymptr = hsym->next_sym;
            hsym->next_sym = sym_list;
            sym_list = hsym;
        }
        front.cvars.HashTab[hash] = hsym;
    }
    // if SymLevel == 0 then should sort the sym_list so that we don't do
    // a lot of page thrashing.
    if( front.cvars.SymLevel == 0 ) {
        for( i = 0; i < SYM_SEG_MAX; i++ ) {
            sym_seglist[i] = NULL;
        }
        for( hsym = sym_list; hsym != NULL; hsym = next_hsymptr ) {
            next_hsymptr = hsym->next_sym;
            i = (unsigned)(pointer_uint)hsym->handle / (SYM_PER_BUF * SYM_BUF_PER_SEG);
            hsym->next_sym = sym_seglist[i];
            sym_seglist[i] = hsym;
        }
        sym_list = NULL;
        sym_tail = NULL;
        for( i = 0; i < SYM_SEG_MAX; i++ ) {
            for( j = 0; j < SYM_BUF_PER_SEG; j++ ) {
                sym_buflist[j] = NULL;
                sym_buftail[j] = NULL;
            }
            for( hsym = sym_seglist[i]; hsym != NULL; hsym = next_hsymptr ) {
                next_hsymptr = hsym->next_sym;
                j = ((unsigned)(pointer_uint)hsym->handle / SYM_PER_BUF) % SYM_BUF_PER_SEG;
                hsym->next_sym = sym_buflist[j];
                sym_buflist[j] = hsym;
                if( sym_buftail[j] == NULL ) {
                    sym_buftail[j] = hsym;
                }
            }
            for( j = 0; j < SYM_BUF_PER_SEG; j++ ) {
                hsym = sym_buflist[j];
                if( hsym != NULL ) {
                    if( sym_list == NULL )
                        sym_list = hsym;
                    if( sym_tail != NULL )
                        sym_tail->next_sym = hsym;
                    sym_tail = sym_buftail[j];
                }
            }
        }
    }
    return( sym_list );
}

static SYM_HASHPTR FreeSym( FrontRun& front )
{
    SYM_HASHPTR     hsym;
    SYM_HASHPTR     next_hsymptr;
    SYM_HASHPTR     sym_list;
    int             bucket;
    SYM_HANDLE      prev_tail;
    SYM_ENTRY       sym;
    SYM_HANDLE      head[BUCKETS];
    SYM_HANDLE      tail[BUCKETS];
    TYPEPTR         typ;

  #if _CPU == 370
    void            *xlist;
    InitExtName( &xlist );
  #endif

    for( bucket = 0; bucket < BUCKETS; ++bucket ) {
        head[bucket] = SYM_NULL;
        tail[bucket] = SYM_NULL;
    }
    sym_list = GetSymList(front);
    for( hsym = sym_list; hsym != NULL; hsym = next_hsymptr ) {
        SymGet( front, &sym, hsym->handle );
        next_hsymptr = hsym->next_sym;
        if( (sym.attribs.stg_class == SC_TYPEDEF
          || (front.cvars.SymLevel != 0)
          || (sym.flags & (SYM_REFERENCED | SYM_DEFINED)))
          || (sym.attribs.stg_class == SC_NONE) ) {
            if( front.cvars.SymLevel == 0 ) {
                bucket = SymBucket( front, &sym );
                sym.handle = head[bucket];
                if( ( sym.flags & SYM_FUNCTION ) == 0 ) {
                    /*
                     * VARIABLE
                     */
                    if( sym.attribs.stg_class == SC_NONE ) {
                        typ = sym.sym_type;
                        SKIP_TYPEDEFS( typ );
                        if( typ->decl_type == TYP_ARRAY ) {
                            if( typ->u.array->dimension == 0 ) {
                                typ->u.array->dimension = 1;
                            }
                        }
                    }
                    AssignSeg( front, &sym );
                } else {
                    /*
                     * FUNCTION
                     */
                    CheckFunction( front, &sym, hsym->name );
                }
                if( tail[bucket] == SYM_NULL ) {
                    tail[bucket] = hsym->handle;
                }
                head[bucket] = hsym->handle;
            }
            /*
             * keep static names so that we can output static pubdefs and get nicer
             * -d1 debugging
             */
            sym.name = static_cast<char *>(CPermAlloc( front, strlen( hsym->name ) + 1 ));
            strcpy( sym.name, hsym->name );
            sym.u1.backinfo = NULL;
            SymReplace( front, &sym, hsym->handle );
        }
        SetErrLoc( front, &sym.src_loc );
        CheckIncomplete( front, &sym, hsym->name );
        if( front.cvars.SymLevel == 0 ) {
            CheckDefined( front, &sym, hsym->name );
        } else {
            CheckReference( front, &sym, hsym->name );
            if( sym.attribs.stg_class == SC_STATIC && (sym.flags & SYM_FUNCTION) == 0 ) {
                front.cvars.CurFuncNode->op.u2.func.flags &= ~FUNC_OK_TO_INLINE;
                SymReplace( front, front.cvars.CurFunc, front.cvars.CurFuncHandle );
            }
        }
        InitErrLoc(front);
    }
    if( front.cvars.SymLevel == 0 ) {
        front.cvars.GlobalSym = SYM_NULL;
        prev_tail = SYM_NULL;
        for( bucket = BUCKETS - 1; bucket >= 0; --bucket ) {
            if( head[bucket] != SYM_NULL ) {
                if( front.cvars.GlobalSym == SYM_NULL ) {
                    front.cvars.GlobalSym = head[bucket];
                }
                if( prev_tail != SYM_NULL ) {
                    SymGet( front, &sym, prev_tail );
                    sym.handle = head[bucket];
                    SymReplace( front, &sym, prev_tail );
                }
                prev_tail = tail[bucket];
            }
        }
    }
    return( sym_list );
}


static void AsgnSegs( FrontRun& front, SYM_HANDLE sym_handle )
{
    SYM_ENTRY   sym;

    for( ; sym_handle != SYM_NULL; sym_handle = sym.handle ) {
        SymGet( front, &sym, sym_handle );
        if( ( sym.flags & SYM_FUNCTION ) == 0 ) {  /* if variable */
            AssignSeg( front, &sym );
            SymReplace( front, &sym, sym_handle );
        }
    }
}


void EndBlock( FrontRun& front )
{
/*    SYM_HASHPTR sym_list; */

    FreeEnums(front);
    FreeTags(front);
    FreeSym(front);  /* sym_list = FreeSym(); */
    if( front.cvars.SymLevel != 0 ) {
//        CurFunc->u.func.locals = FreeVars( sym_list );
        if( front.cvars.SymLevel == 1 ) {
            AsgnSegs( front, front.cvars.CurFunc->u.func.locals );
//            DumpWeights( CurFunc->u.func.locals );
        } else {
            AsgnSegs( front, GetBlockSymList(front) );
        }
    }
    --front.cvars.SymLevel;
}


#if 0
static void DumpWeights( SYMPTR sym )
{
    if( DebugFlag > 0 ) {
        for( ; sym != NULL; sym = sym->thread ) {
            printf( "%4x: %s\n", sym->u.var.offset, sym->name );
        }
    }
}
#endif


LABELPTR LkLabel( FrontRun& front, const char *name )
{
    LABELPTR    label;

    for( label = front.cvars.LabelHead; label != NULL; label = label->next_label ) {
        if( strcmp( name, label->name ) == 0 ) {
            return( label );
        }
    }
    label = (LABELPTR)CMemAlloc( front, sizeof( LABELDEFN ) + strlen( name ) );
    if( label != NULL ) {
        label->next_label = front.cvars.LabelHead;
        front.cvars.LabelHead = label;
        strcpy( label->name, name );
        label->defined = false;
        label->referenced = false;
        label->ref_list = NextLabel(front);
    }
    return( label );
}


void FreeLabels( FrontRun& front )
{
    LABELPTR    label;

    for( ; (label = front.cvars.LabelHead) != NULL; ) {
        front.cvars.LabelHead = label->next_label;
        if( !label->defined ) {
            CErr2p( front, ERR_UNDEFINED_LABEL, label->name );
        } else if( !label->referenced ) {
            CWarn2p( front, ERR_UNREFERENCED_LABEL, label->name );
        }
        CMemFree( front, label );
    }
}


#if 0
static void DoSymPurge( SYMPTR sym )
{
    SYMPTR  curr;
    SYMPTR  temp;

    if( (sym->flags & SYM_FUNCTION) && (sym != CurFunc) ) {
        curr = sym->u.func.parms;
        while( curr != NULL ) {
            temp = curr->thread;
            CMemFree( curr );
            curr = temp;
        }
        curr = sym->u.func.locals;
        while( curr != NULL ) {
            temp = curr->thread;
            CMemFree( curr );
            curr = temp;
        }
    }
    CMemFree( sym );
}
#endif


void SymsPurge( FrontRun& front )
{
    LABELPTR    label;

    while( (label = front.cvars.LabelHead) != NULL ) {
        front.cvars.LabelHead = label->next_label;
        CMemFree( front, label );
    }
//      EnumInit();
//      PurgeTags( TagHead );
    front.cvars.TagHead = NULL;
//      PurgeTags( DeadTags );
    front.cvars.DeadTags = NULL;

#if 0
    {
        id_hash_idx     hash;
        SYMPTR          tmp_sym, sym;

        for( hash = 0; hash < ID_HASH_SIZE; hash++ ) {
            sym = HashTab[hash];
            HashTab[hash] = NULL;
            while( sym != NULL ) {
                tmp_sym = sym->next_sym;
                DoSymPurge( sym );
                sym = tmp_sym;
            }
        }

        while( (sym = GlobalSym) != SYM_NULL ) {
            GlobalSym = sym->thread;
            DoSymPurge( sym );
        }

        while( (sym = SpecialSyms) != SYM_NULL ) {
            SpecialSyms = sym->thread;
            DoSymPurge( sym );
        }
    }
#endif
    TypesPurge();
}

#if 0           /* can't free these because they are in CPermArea */

static void PurgeTags( TAGPTR tag_head )
{
    TAGPTR      tag;
    FIELDPTR    field;
    DATA_TYPE   tag_type;

    for( ; (tag = tag_head) != NULL; ) {
        tag_head = tag->next_tag;
        tag_type = tag->sym_type->decl_type;
        if( tag_type == TYP_STRUCT || tag_type == TYP_UNION ) {
            for( ; (field = tag->u.field_list) != NULL; ) {
                tag->u.field_list = field->next_field;
                CMemFree( field );
            }
        } else {    /* tag_type == TYP_ENUM */
            PurgeEnums( tag->u.enum_list );
        }
        CMemFree( tag );
    }
}


static void PurgeEnums( ENUM_HANDLE list_head )
{
    ENUMPTR     ep;

    for( ; (ep = list_head) != NULL; ) {
        list_head = ep->thread;
        CMemFree( ep );
    }
}

#endif

XREFPTR NewXref( FrontRun& front, XREFPTR next_xref )
{
    XREFPTR     xref;

    xref = (XREFPTR)CMemAlloc( front, sizeof( XREF_ENTRY ) );
    xref->next_xref = next_xref;
    xref->src_loc = front.cvars.TokenLoc;
    return( xref );
}

void FreeXrefs( FrontRun& front, XREFPTR xref_list )
{
    XREFPTR     xref;

    while( (xref = xref_list) != NULL ) {
        xref_list = xref->next_xref;
        CMemFree( front, xref );
    }
}
