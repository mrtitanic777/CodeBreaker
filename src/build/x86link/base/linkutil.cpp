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
* Description:  Utility routines for wlink.
*
****************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <setjmp.h>
#include <sys/types.h>
#include <array>
#include <cstddef>
#include <string>
#include <string_view>
#include "build/x86link/link/linkstd.h"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include "pcobj.h"
#include "build/x86link/base/newmem.h"
#include "build/x86link/base/fileio.h"
#include "build/x86link/driver/ideentry.h"
#include "build/x86link/base/ring.h"
#include "build/x86link/layout/overlays.h"
#include "build/x86link/base/strtab.h"
#include "build/x86link/formats/output.h"
#include "build/x86link/link/permdata.h"
#include "build/x86link/objects/objio.h"
#include "build/x86link/map/map.h"
#include "pathlist.h"
#include "build/x86link/link/run_state.hpp"

#include "clibext.h"


namespace {

/* what a walk over the sections hands to its per-section callback */
struct mods_walk_data {
    mods_walk_fn    *cbfn;
};

struct class_walk_data {
    class_walk_fn   *cbfn;
};

/* what the file keeps between its calls in one run */
struct LinkUtilState {
    seg_leader      *find_seg = nullptr;        /* FindSegment's place: the segment... */
    class_entry     *find_class = nullptr;      /* ...and its class */
    jmp_buf         *spawn_stack = nullptr;     /* Spawn's innermost jump target */
};

LinkUtilState &State()
{
    return cb_wlink::RunState::part<LinkUtilState>();
}

/* Default File Extension array, see ldefext.h */

constexpr std::string_view DefExt[] = {
    #define pick(enum,text) std::string_view( text ),
    #include "build/x86link/link/ldefext.h"
    #undef pick
};

constexpr std::size_t MAXDEPTH = sizeof( std::size_t ) * 8;

} // namespace

void WriteNulls( f_handle file, size_t len, const char *name )
/************************************************************/
/* copy nulls for uninitialized data */
{
    static const std::array<unsigned, 16> NullArray{};

    for( ; len > sizeof( NullArray ); len -= sizeof( NullArray ) ) {
        QWrite( file, NullArray.data(), sizeof( NullArray ), name );
    }
    if( len > 0 ) {
        QWrite( file, NullArray.data(), len, name );
    }
}

void CheckErr(Link& link)
/**************************/
{
    if( link.LinkState & ( LS_LINK_ERROR | LS_STOP_WORKING ) ) {
        if( link.MapFile != nullptr ) {
            WriteMapLibsUsed(link);
        }
        Suicide();
    }
}

void CheckStop(Link& link)
/***************************/
{
    if( link.LinkState & LS_STOP_WORKING ) {
        Suicide();
    }
}

void LnkFatal( const char *msg )
/******************************/
{
    LnkMsg( FTL+MSG_INTERNAL, "s", msg );
}

bool TestBit( byte *array, unsigned num )
/***********************************************/
/* return true if the specified bit is on */
{
    const byte  mask = static_cast<byte>( 1 << (num % 8) );

    return( (array[num / 8] & mask) != 0 );
}

void ClearBit( byte *array, unsigned num )
/************************************************/
/* make sure a bit is turned off */
{
    const byte  mask = static_cast<byte>( 1 << (num % 8) );

    array[num / 8] &= ~mask;
}

static void WalkModsList( mod_entry *list, mods_walk_fn *cbfn )
/*************************************************************/
{
    for( ; list != nullptr; list = list->u.next ) {
        cbfn( list );
    }
}

static void WalkSectModsList( section *sect, void *mods_walk_cb )
/***************************************************************/
{
    Link& link = wlink();
    link.CurrSect = sect;
    WalkModsList( sect->mods, static_cast<mods_walk_data *>( mods_walk_cb )->cbfn );
}

void WalkAllSects(Link& link,  void (*rtn)( section * ) )
/*******************************************/
{
    rtn( link.Root );
#ifdef _EXE
    if( link.FmtData.type & MK_OVERLAYS ) {
        WalkAreas( link.Root->areas, rtn );
    }
#endif
}

void ParmWalkAllSects(Link& link,  void (*rtn)( section *, void * ), void *parm )
/*******************************************************************/
{
    rtn( link.Root, parm );
#ifdef _EXE
    if( link.FmtData.type & MK_OVERLAYS ) {
        ParmWalkAreas( link.Root->areas, rtn, parm );
    }
#endif
}

void WalkMods(Link& link,  mods_walk_fn *cbfn )
/*********************************/
{
    mods_walk_data      mods_walk_cb{ cbfn };

    ParmWalkAllSects(link,  WalkSectModsList, &mods_walk_cb );
    link.CurrSect = link.Root;
    WalkModsList( link.LibModules, cbfn );
}

static void WalkClass( class_entry *class_, class_walk_fn *cbfn )
/**************************************************************/
{
    /* the ring hands each element over as a void *; the callback takes it as a seg_leader * */
    RingWalk( class_->segs, reinterpret_cast<ringwalk_fn *>( cbfn ) );
}

void SectWalkClass(Link& link,  section *sect, class_walk_fn *cbfn )
/******************************************************/
{
    link.CurrSect = sect;
    for( class_entry *class_ = sect->classes; class_ != nullptr; class_ = class_->next ) {
        WalkClass( class_, cbfn );
    }
}

static void _SectWalkClass( section *sect, void *class_walk_cb )
/**************************************************************/
{
    Link& link = wlink();
    link.CurrSect = sect;
    for( class_entry *class_ = sect->classes; class_ != nullptr; class_ = class_->next ) {
        WalkClass( class_, static_cast<class_walk_data *>( class_walk_cb )->cbfn );
    }
}

void WalkLeaders(Link& link,  class_walk_fn *cbfn )
/*************************************/
{
    class_walk_data     class_walk_cb{ cbfn };

    ParmWalkAllSects(link,  _SectWalkClass, &class_walk_cb );
}

seg_leader *FindSegment( section *sect, const char *name )
/********************************************************/
/* NOTE: this does not work for overlays!
 *
 * sect != NULL then it works as FindFirstSegment
 * sect == NULL then it works as FindNextSegment
 *
 */
{
    LinkUtilState   &st = State();

    if( sect != nullptr ) {
        st.find_class = sect->classes;
        st.find_seg = nullptr;
    }
    for( ; st.find_class != nullptr; st.find_class = st.find_class->next ) {
        while( (st.find_seg = static_cast<seg_leader *>( RingStep( st.find_class->segs, st.find_seg ) )) != nullptr ) {
            if( stricmp( st.find_seg->segname.u.ptr, name ) == 0 ) {
                return( st.find_seg );
            }
        }
    }
    return( st.find_seg );
}

void AppendList( void **in_head, void *newnode )
/**********************************************
 * Link a new node into a linked list (new node goes at the end of the list)
 */
{
    node    **owner;

    for( owner = reinterpret_cast<node **>( in_head ); *owner != nullptr; owner = reinterpret_cast<node **>( &(*owner)->next ) )
        /* nothing */;
    static_cast<node *>( newnode )->next = nullptr;
    *owner = static_cast<node *>( newnode );
}

void FreeList( void *_curr )
/**************************/
/* Free a list of nodes. */
{
    node        *next;

    for( node *curr = static_cast<node *>( _curr ); curr != nullptr; curr = next ) {
        next = static_cast<node *>( curr->next );
        MemFree( curr );
    }
}

obj_name_list *AddNameTable(Link& link,  const char *name, size_t len, bool is_mod, obj_name_list **owner )
/*********************************************************************************************/
{
    obj_name_list   *imp;
    unsigned_32     off;
    unsigned_16     index;

    index = 1;
    off = 1;
    for( imp = *owner; imp != nullptr; imp = imp->next ) {
        if( len == imp->len
          && memcmp( imp->name.u.ptr, name, len ) == 0 )
            break;
        off += imp->len + 1;
        ++index;
        owner = &imp->next;
    }
    if( imp == nullptr ) {
        imp = static_cast<obj_name_list *>( _PermAlloc( sizeof( obj_name_list ) ) );
        imp->next = nullptr;
        imp->len = len;
        imp->name.u.ptr = AddSymbolStringTable( &link.PermStrings, name, len );
        imp->num = is_mod ? index : off;
        *owner = imp;
    }
    return( imp );
}

unsigned_16 log2_16( unsigned_16 value )
/**************************************/
// This calculates the binary log of value, truncating decimals.
{
    unsigned_16 log;

    if( value == 0 ) {
        return( 0 );
    }
    log = 15;
    for( ; ; ) {
        if( value & 0x8000 ) {  // done if high bit on
            break;
        }
        value = static_cast<unsigned_16>( value << 1 );     // shift left and decrease possible log.
        log--;
    }
    return( log );
}

unsigned_16 log2_32( unsigned_32 value )
/**************************************/
// This calculates the binary log of a 32-bit value, truncating decimals.
{
    unsigned_16 log;

    if( value == 0 ) {
        return( 0 );
    }
    log = 31;
    for( ; ; ) {
        if( value & 0x80000000 ) {  // done if high bit on
            break;
        }
        value <<= 1;            // shift left and decrease possible log.
        log--;
    }
    return( log );
}

const char *GetBaseName( const char *namestart, size_t len, size_t *lenp )
/************************************************************************/
/* parse name as a filename, "removing" the path and the extension */
/* returns a pointer to the "base" of the filename, and a length without
 * the extension */
{
    const char  *dotpoint;
    const char  *string;

    if( len == 0 )
        len = strlen( namestart );
    // ignore path & extension in module name.
    dotpoint = nullptr;
    for( string = namestart; len-- > 0; string++ ) {
        const char ch = *string;
        if( ch == '.' ) {
            dotpoint = string;
            continue;
        }
        if( IS_PATH_SEP( ch ) ) {
            namestart = string + 1;
            dotpoint = nullptr;
        }
    }
    if( dotpoint != nullptr ) {
        *lenp = dotpoint - namestart;
    } else {
        *lenp = string - namestart;
    }
    return( namestart );
}

void VMemQSort( virt_mem base, size_t n, size_t width,
                        void (*swapfn)( virt_mem, virt_mem ),
                        int (*cmpfn)( virt_mem, virt_mem ) )
/***********************************************************/
// qsort stolen from clib, and suitably modified since we need to be able
// to swap parallel arrays.
{
    virt_mem        p1;
    virt_mem        p2;
    virt_mem        mid;
    int             comparison;
    size_t          last_non_equal_count;
    size_t          i;
    size_t          count;
    size_t          sp;
    std::array<virt_mem, MAXDEPTH>  base_stack;
    std::array<size_t, MAXDEPTH>    n_stack;

    sp = 0;
    for( ; ; ) {
        while( n > 1 ) {
            p1 = base + width;
            if( n == 2 ) {
                if( cmpfn( base, p1 ) > 0 ) {
                    swapfn( base, p1 );
                }
                break;
            } else {
                /* store mid element at base for pivot */
                /* this will speed up sorting of a sorted list */
                mid = base + ( n >> 1 ) * width;
                swapfn( base, mid );
                p2 = base;
                count = 0;
                last_non_equal_count = 0;
                for( i = 1; i < n; ++i ) {
                    comparison = cmpfn( p1, base );
                    if( comparison <= 0 ) {
                        p2 += width;
                        count++;
                        if( i != count ) {              /* p1 != p2 */
                            swapfn( p1, p2 );
                        }
                    }
                    if( comparison != 0 )
                        last_non_equal_count = count;
                    p1 += width;
                }
                /* special check to see if all values compared are equal */
                if( ( count == n - 1 )
                  && ( last_non_equal_count == 0 ) )
                    break;
                if( count != 0 ) {  /* store pivot in right spot */
                    swapfn( base, p2 );
                }
                n = n - count - 1;          /* calc. size of right part */

                /* The pivot is at p2. It is in its final position.
                   There are count items to the left of the pivot.
                   There are n items to the right of the pivot.
                */

                if( count != last_non_equal_count ) {   /* 18-jul-90 */
                    /*
                       There are last_non_equal_count+1 items to the left
                       of the pivot that still need to be checked.
                       There are (count - (last_non_equal_count+1)) items
                       immediately to the left of the pivot that are
                       equal to the pivot. They are in their final position.
                    */
                    count = last_non_equal_count + 1;
                }
                if( count < n ) {           /* if left part is shorter */
                    base_stack[sp] = p2 + width;  /* - stack right part */
                    n_stack[sp] = n;
                    n = count;
                } else {                    /* right part is shorter */
                    base_stack[sp] = base;  /* - stack left part */
                    n_stack[sp] = count;
                    base = p2 + width;
                }
                ++sp;
            }
        }
        if( sp == 0 )
            break;
        --sp;
        base = base_stack[sp];
        n    = n_stack[sp];
    }
}

int Spawn( void (*fn)( void ) )
/*****************************/
{
    jmp_buf *save_env;
    jmp_buf env;
    int     status;

    save_env = State().spawn_stack;
    State().spawn_stack = &env;
    status = setjmp( env );
    if( status == 0 ) {
        (*fn)();
    }
    State().spawn_stack = save_env;  /* unwind */
    return( status );
}

void Suicide( void )
/******************/
{
    if( State().spawn_stack != nullptr ) {
        longjmp( *State().spawn_stack, 1 );
    }
}

/* the directories a path list (an environment variable) names, as wlink keeps them: one allocation, the elements
   separated by PATH_LIST_SEP; an empty list when the variable is not set or is empty */
static char *CopyPathList( const char *path_list )
{
    char        *list;
    char        *p;

    if( path_list != nullptr
      && *path_list != '\0' ) {
        list = static_cast<char *>( MemAllocSafe( strlen( path_list ) + 1 ) );
        p = list;
        do {
            if( p != list )
                *p++ = PATH_LIST_SEP;
            path_list = GetPathElement( path_list, nullptr, &p );
        } while( *path_list != '\0' );
        *p = '\0';
    } else {
        list = static_cast<char *>( MemAllocSafe( 1 ) );
        *list = '\0';
    }
    return( list );
}

void InitEnvVars(Link& link)
/**********************/
{
    if( link.ExePath == nullptr ) {
#if defined( __QNX__ )
        const char  *path_list = "/usr/watcom";
#else
        const char  *path_list = GetEnvString( "PATH" );
#endif
        link.ExePath = CopyPathList( path_list );
    }
    if( link.LibPath == nullptr ) {
        link.LibPath = CopyPathList( GetEnvString( "LIB" ) );
    }
}

void FiniEnvVars(Link& link)
/**********************/
{
    if( link.ExePath != nullptr ) {
        MemFree( link.ExePath );
        link.ExePath = nullptr;
    }
    if( link.LibPath != nullptr ) {
        MemFree( link.LibPath );
        link.LibPath = nullptr;
    }
}

f_handle FindPath(Link& link,  const char *name, char *fullname )
/***************************************************/
{
    const char                  *path_list;
    f_handle                    file;
    std::array<char, PATH_MAX>  fullpath;

    strcpy( fullpath.data(), name );
    file = QObjOpen( fullpath.data() );
    if( file == NIL_FHANDLE
      && link.ExePath != nullptr ) {
        for( path_list = link.ExePath; *path_list != '\0'; ) {
            strcpy( MakePath( fullpath.data(), &path_list ), name );
            file = QObjOpen( fullpath.data() );
            if( file != NIL_FHANDLE ) {
                break;
            }
        }
    }
    if( file != NIL_FHANDLE
      && fullname != nullptr ) {
        strcpy( fullname, fullpath.data() );
    }
    return( file );
}

group_entry *FindGroup(Link& link,  segment seg )
/***********************************/
{
    group_entry *group;

    for( group = link.Groups; group != nullptr; group = group->next ) {
        if( group->addr.seg == seg ) {
            break;
        }
    }
    return( group );
}

offset FindLinearAddr(Link& link,  addr_type *addr )
/**************************************/
{
    group_entry *group;

    group = FindGroup(link,  addr->seg );
    if( group != nullptr ) {
        return( addr->off + ( group->linear - group->addr.off ) );
    }
    return( addr->off );
}

offset FindLinearAddr2(Link& link,  addr_type *addr )
/***************************************/
{
    group_entry *group;

    group = FindGroup(link,  addr->seg );
    if( group != nullptr ) {
        return( addr->off + group->linear + link.FmtData.base );
    }
    return( addr->off );
}

file_list *AllocNewFile(Link& link,  member_list *member )
/********************************************/
{
    file_list       *file;

    file = static_cast<file_list *>( _PermAlloc( sizeof( file_list ) ) );
    file->next = nullptr;
    file->flags_dbi = link.DBIFlag;
    file->flags_file = static_cast<file_flags>( 0 );
    file->strtab = nullptr;
    file->u.member = member;
    if( member != nullptr ) {
        file->flags_file |= STAT_HAS_MEMBER;
    }
    return( file );
}

char *FileName( const char *buff, size_t len, file_defext etype, bool force )
/***************************************************************************/
{
    Link& link = wlink();
    const char  *namptr;
    const char  *namstart;
    char        *ptr;
    size_t      cnt;
    size_t      namelen;
    size_t      extlen;

    for( namptr = buff + len; namptr != buff; --namptr ) {
        if( IS_PATH_SEP( namptr[-1] ) ) {
            break;
        }
    }
    namstart = namptr;
    cnt = len - ( namptr - buff );
    if( cnt == 0 ) {
        const std::string name( buff, len );
        LnkMsg( FTL+LOC+LINE+MSG_INV_FILENAME, "s", name.c_str() );
    }
    namelen = cnt;
    namptr = buff + len - 1;
    while( --cnt > 0 && *namptr != '.' ) {
        namptr--;
    }
    if( !force
      && ( etype == E_NONE
      || *namptr == '.' ) ) {
        /*
         * duplicate name if no extension change is necessary
         */
        ptr = MemToStringSafe(link,  buff, len );
    } else {
        if( force
          && etype == E_MAP ) {         // op map goes in current dir.
            buff = namstart;
            len = namelen;
        }
        /*
         * remove extension if it exists
         */
        if( cnt != 0 ) {
            len = namptr - buff;
        }
        /*
         * if some extension is required then add it
         */
        if( etype == E_NONE ) {
            ptr = MemToStringSafe(link,  buff, len );
        } else {
            const std::string_view  ext = DefExt[etype];

            extlen = ext.size();
            ptr = static_cast<char *>( MemAllocSafe( len + 1 + extlen + 1 ) );
            memcpy( ptr, buff, len );
            if( extlen > 0 ) {
                ptr[len++] = '.';
                memcpy( ptr + len, ext.data(), extlen );
                len += extlen;
            }
            ptr[len] = '\0';
        }
    }
    return( ptr );
}

static int stricmp_wrapper( const void *s1, const void *s2 )
/**********************************************************/
{
    return( stricmp( static_cast<const char *>( s1 ), static_cast<const char *>( s2 ) ) );
}

section *NewSection( void )
/*************************/
{
    section             *sect;

    sect = static_cast<section *>( MemAllocSafe( sizeof( section ) ) );
    sect->next = nullptr;
    sect->classes = nullptr;
    sect->orderlist = nullptr;
    sect->areas = nullptr;
    sect->files = nullptr;
    sect->modFilesHashed = CreateHTable( 256, StringiHashFunc, stricmp_wrapper, MemAllocSafe, MemFree );
    sect->mods = nullptr;
    sect->reloclist = nullptr;
    SET_ADDR_UNDEFINED( sect->sect_addr );
    sect->ovlref = 0;
    sect->parent = nullptr;
    sect->relocs = 0;
    sect->size = 0;
    sect->outfile = nullptr;
    sect->u.dist_mods = nullptr;
    sect->dbg_info = nullptr;
    return( sect );
}
