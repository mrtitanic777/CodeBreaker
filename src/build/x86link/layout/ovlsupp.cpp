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
* Description:  OVLSUPP -- overlay support routines
*
****************************************************************************/

#include <cstring>
#include <cstddef>
#include "build/x86link/link/linkstd.h"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include "pcobj.h"
#include "build/x86link/base/newmem.h"
#include "build/x86link/fixups/reloc.h"
#include "build/x86link/base/virtmem.h"
#include "build/x86link/base/fileio.h"
#include "build/x86link/map/map.h"
#include "build/x86link/objects/procfile.h"
#include "build/x86link/layout/objcalc.h"
#include "build/x86link/objects/objfree.h"
#include "build/x86link/debuginfo/dbgall.h"
#include "build/x86link/symbols/objpass1.h"
#include "build/x86link/formats/output.h"
#include "build/x86link/formats/dos.h"
#include "build/x86link/objects/objnode.h"
#include "build/x86link/layout/distrib.h"
#include "build/x86link/layout/overlays.h"
#include "build/x86link/base/ring.h"
#include "build/x86link/link/specials.h"
#include "build/x86link/link/run_state.hpp"

#include "clibext.h"


#ifdef _EXE


static void         AllocAreas( AREASECT *area );

namespace {

// what ovlsupp.cpp keeps for one run of the linker
struct OvlState {
    vect_state  vect;               /* structure with overlay state members */
    segdata     *ovlSegData = nullptr;
    addr_type   vecAddr;            /* address of overlay vectors */
    addr_type   stashAddr;
    group_entry *group = nullptr;   /* pointer to group for overlay table   */
    addr_type   tabAddr;            /* address of overlay tables            */
    unsigned    tabSize = 0;        /* size of overlay tables               */
    int         vecNum = 0;         /* number of vectors so far             */
};

OvlState &ovl()
{
    return cb_wlink::RunState::part<OvlState>();
}

}

void ResetOverlaySupp( void )
/***************************/
{
    wlink().OvlAreaSize = 0xFFFF;
    ovl().vect.OvlVectors = nullptr;
    /* OvlSectNum value 0 is reserved for Root */
    /* Overlayed sections start at 1 */
    wlink().OvlSectNum = 1;
    wlink().OvlClasses = nullptr;
    ovl().vecNum = 0;
    ResetDistribSupp();
}

static void ParmWalkSections( section *sect, void (*rtn)( section *, void * ), void *parm )
/*****************************************************************************************/
{
    for( ; sect != nullptr; sect = sect->next ) {
        rtn( sect, parm );
        ParmWalkAreas( sect->areas, rtn, parm );
    }
}

void ParmWalkAreas( AREASECT *area, void (*rtn)( section *, void * ), void *parm )
/********************************************************************************/
{
    for( ; area != nullptr; area = area->next ) {
        ParmWalkSections( area->sections, rtn, parm );
    }
}

static void WalkSections( section *sect, void (*rtn)( section * ) )
/*****************************************************************/
{
    for( ; sect != nullptr; sect = sect->next ) {
        rtn( sect );
        WalkAreas( sect->areas, rtn );
    }
}

void WalkAreas( AREASECT *area, void (*rtn)( section * ) )
/********************************************************/
{
    for( ; area != nullptr; area = area->next ) {
        WalkSections( area->sections, rtn );
    }
}

static void DoSecPubs( section *sect )
/************************************/
{
    if( wlink().MapFile != nullptr ) {
        WriteMapOvlPubHead( sect );
        WriteMapPubStart();
    }
    ProcPubsSect( sect->mods, sect );
    OvlProcPubsSect( sect );
    if( wlink().MapFile != nullptr ) {
        WriteMapPubEnd();
    }
}

void OvlProcPubsSect( section *sect )
/***********************************/
{
    for( wlink().CurrMod = sect->u.dist_mods; wlink().CurrMod != nullptr; wlink().CurrMod = wlink().CurrMod->u2.next ) {
        DoPubsSect( sect );
    }
}

void OvlProcPubs( void )
/**********************/
{
    if( wlink().MapFile != nullptr ) {
        WriteMapOvlVectHead( &ovl().vect );
    }
    WalkAreas( wlink().Root->areas, DoSecPubs );
}

static void AllocSections( section *first_sect )
/**********************************************/
/* Allocate parallel overlay sections */
{
    addr_type           save_addr;
    addr_type           max_addr;
    section             *sect;
    unsigned long       result;
    unsigned            ovl_size;
    unsigned            min_size;
    bool                area_specified;

    wlink().CurrSect = first_sect;    /* NormalizeAddr() needs to know the section */
    MAlign( wlink().FmtData.SegShift );     /* as do some DBI routines. */
    NormalizeAddr();
    if( first_sect == wlink().NonSect ) {
        if( wlink().FmtData.u.dos.dynamic ) {
            ovl_size = wlink().CurrLoc.seg - ovl().stashAddr.seg;
            /* OvlSectNum value 0 is reserved for Root */
            min_size = ovl_size + wlink().OvlSectNum + 1;  // need at least 1 para per sect. + 1
            if( min_size < 64 ) {
                min_size = 64;      // reserve 1 K for the stack
            }
            area_specified = true;
            if( wlink().OvlAreaSize == 0xFFFF ) {   // default is twice the memory size
                wlink().OvlAreaSize = ovl_size * 2;
                area_specified = false;
            } else if( wlink().OvlAreaSize == 0 ) {
                area_specified = false;
            }
            if( wlink().OvlAreaSize < min_size ) {
                wlink().OvlAreaSize = min_size;
                if( area_specified ) {
                    LnkMsg( WRN+MSG_AREA_TOO_SMALL, "l", static_cast<unsigned long>( min_size ) << wlink().FmtData.SegShift );
                }
            }
            result = wlink().CurrLoc.seg + static_cast<unsigned long>( wlink().OvlAreaSize - ovl_size );
            if( result > 0xFFFF ) {
                LnkMsg( WRN+MSG_CANT_RESERVE_SPACE, "l", static_cast<unsigned long>( wlink().OvlAreaSize ) << wlink().FmtData.SegShift );
            } else {
                wlink().CurrLoc.seg = static_cast<segment>( result );
            }
        }
        wlink().OvlAreaSize = wlink().CurrLoc.seg - ovl().stashAddr.seg;
    }
    save_addr = wlink().CurrLoc;
    max_addr.off = 0;
    max_addr.seg = 0;
    for( sect = first_sect; sect != nullptr; sect = sect->next ) {
        wlink().CurrSect = sect;
        sect->sect_addr = save_addr;
        AllocClasses( sect );
        if( sect->areas != nullptr ) {
            AllocAreas( sect->areas );
        }
        NormalizeAddr();        /* avoid any overflow messages */
        AddSize( 2 );   /* reserve some space for the overlay manager */
        NormalizeAddr();        /*  get canonical form */
        if( ( wlink().CurrLoc.seg > max_addr.seg )
          || ( wlink().CurrLoc.seg == max_addr.seg )
          && ( wlink().CurrLoc.off > max_addr.off ) ) {
            max_addr = wlink().CurrLoc;
        }
        wlink().CurrLoc = save_addr;
    }
    wlink().CurrLoc = max_addr;
}

static void AllocAreas( AREASECT *area )
/**************************************/
{
    /* now CurrLoc is beginning of overlay area */
    DEBUG(( DBG_OLD, "Overlay area start %a", &wlink().CurrLoc ));
    for( ; area != nullptr; area = area->next ) {
        AllocSections( area->sections );
    }
}

void OvlCalc( void )
/******************/
{
    unsigned        temp;
    outfilelist     *fnode;

    wlink().CurrSect = wlink().Root;
    /* record starting address of overlay table */
    Align( 2 ); // for overlay table speed.
    SET_SYM_ADDR( ovl().vect.OverlayTable, wlink().CurrLoc.off, wlink().CurrLoc.seg );
    DEBUG(( DBG_OLD, "Overlay table address %a", &wlink().CurrLoc ));
    ovl().tabAddr = wlink().CurrLoc;
    /* calculate size of overlay table proper */
    /* OvlSectNum value 0 is reserved for Root, Root entry is not emited */
    temp = sizeof( ovltab_prolog ) + ( wlink().OvlSectNum - 1 ) * sizeof( ovltab_entry );
    SET_SYM_ADDR( ovl().vect.OverlayTableEnd, wlink().CurrLoc.off + temp, wlink().CurrLoc.seg );
    temp += sizeof( unsigned_16 );      /* add Overlays table terminator */
    for( fnode = wlink().OutFiles; fnode != nullptr; fnode = fnode->next ) {
        fnode->ovlfnoff = temp;
        temp += strlen( fnode->fname ) + 1;
    }
    wlink().Root->outfile->ovlfnoff |= OVE_EXE_FILENAME;    // indicate the .exe file.
    /* calculate starting address of overlay vectors, record */
    ovl().tabSize = temp;   /*  incl. NULLCHAR */
    wlink().CurrentSeg = nullptr;
    AddSize( ovl().tabSize );
    SET_SYM_ADDR( ovl().vect.OvlVecStart, wlink().CurrLoc.off, wlink().CurrLoc.seg );
    DEBUG(( DBG_OLD, "Overlay vector start %a", &wlink().CurrLoc ));
    ovl().vecAddr = wlink().CurrLoc;
    /* calculate start of overlay area */
    if( wlink().FmtData.u.dos.ovl_short ) {
        temp = ovl().vecNum * sizeof( svector );
    } else {
        temp = ovl().vecNum * sizeof( lvector );
    }
    AddSize( temp );
    ovl().tabSize += temp;
    SET_SYM_ADDR( ovl().vect.OvlVecEnd, wlink().CurrLoc.off, wlink().CurrLoc.seg );
    AddSize( 2 );       // reserve some space for the ovl. manager.
    Align( 4 );
    NormalizeAddr();
    ovl().stashAddr = wlink().CurrLoc;
    AllocAreas( wlink().Root->areas );
    MAlign( 4 );          /*    don't add to section size */
    NormalizeAddr();        /*  now CurrLoc.seg points to next free para */

    ovl().group = AllocGroup( AutoGrpName, &wlink().Groups );
    ovl().group->addr = ovl().tabAddr;
    ovl().group->section = wlink().Root;
    ovl().group->size = ovl().tabSize;
    ovl().group->totalsize = ovl().tabSize;
    ovl().group->num = 0;
    ovl().group->linear = 0;
    Ring2Append( &ovl().group->leaders, wlink().OvlSeg );

    wlink().OvlSeg->group = ovl().group;
    wlink().OvlSeg->seg_addr = ovl().tabAddr;
    wlink().OvlSeg->size = ovl().tabSize;

    ovl().ovlSegData->u1.vm_ptr = AllocStg( ovl().tabSize );
    ovl().ovlSegData->length = ovl().tabSize;
    RingAppend( &ovl().ovlSegData->u.leader->pieces, ovl().ovlSegData );
}

void FreeOverlaySupp( void )
/**************************/
{
    wlink().OvlClasses = nullptr;
    ovl().vect.OvlVectors = nullptr;
    if( wlink().OvlSeg != nullptr ) {
        FreeLeader( wlink().OvlSeg );
    }
    FreeDistribSupp();
}

static bool IsAncestor( overlay_ref elder_ovlref, section *ceorl )
/****************************************************************/
/* Is overlay section # "elder" an ancestor of section "ceorl" */
{
    for( ; ; ) {
        if( ceorl->ovlref == elder_ovlref )
            return( true );
        if( ceorl->parent == nullptr )
            return( false );
        ceorl = ceorl->parent;
    }
}

#define NO_VECTOR( sym ) ( ( IS_SYM_COMMUNAL( sym ) ) \
                            || ( (sym)->u.d.ovlstate & OVL_FORCE ) )

void OvlDefVector( symbol *sym )
/******************************/
{
    segdata     *sdata;
    overlay_ref ovlref;

    if( NO_VECTOR( sym ) )
        return;
    sdata = sym->p.seg;
    if( sdata == nullptr ) {
        sym->u.d.ovlstate |= (OVL_FORCE | OVL_NO_VECTOR);
        return;         /* NOTE: <--- premature return <----------- */
    }
    ovlref = sdata->u.leader->class_->section->ovlref;
    if( !sdata->iscode
      || ( ovlref == 0 ) ) {      // not code or in root
        sym->u.d.ovlstate |= (OVL_FORCE | OVL_NO_VECTOR);
    } else {
        if( sym->info & SYM_REFERENCED ) {
            if( sym->u.d.ovlref != ovlref ) {
                /* first reference must have been a proper ancestor or cousin */
                OvlVectorize( sym );
            }
        } else {
            sym->u.d.ovlstate |= OVL_REF;
            sym->u.d.ovlref = ovlref;
        }
    }
}

void OvlVectorize( symbol *sym )
/******************************/
/* allocate an overlay vector for a symbol */
{
    vecnode         *vectnode;

    sym->u.d.ovlref = ++ovl().vecNum;
    sym->u.d.ovlstate &= ~OVL_NO_VECTOR;
    sym->u.d.ovlstate |= OVL_FORCE;
    vectnode = static_cast<vecnode *>( _PermAlloc( sizeof( vecnode ) ) );
    vectnode->sym = sym;
    AppendList( reinterpret_cast<void **>( &ovl().vect.OvlVectors ), vectnode );
    DEBUG(( DBG_OLD, "Vectorize %d %S", ovl().vecNum, sym ));
}

static void OvlRefVector( symbol *sym )
/*************************************/
{
    overlay_ref     ovlref;

    if( IS_SYM_COMMUNAL( sym ) )
        return;
    if( (sym->u.d.ovlstate & OVL_VEC_MASK) != OVL_UNDECIDED )
        return;
    if( (sym->info & SYM_DEFINED) == 0 ) {
        if( (sym->u.d.ovlstate & OVL_REF) == 0 ) {
            sym->u.d.ovlref = wlink().CurrSect->ovlref;
            sym->u.d.ovlstate |= OVL_REF;
        } else if( wlink().FmtData.u.dos.distribute ) {
            sym->u.d.ovlref = LowestAncestor( sym->u.d.ovlref, wlink().CurrSect );
        }
    } else {
        if( sym->p.seg == nullptr )
            return;
        /* at this point, we know it has already been defined, but does */
        /* not have an overlay vector, and is not data */
        ovlref = sym->p.seg->u.leader->class_->section->ovlref;
        if( IsAncestor( ovlref, wlink().CurrSect ) )
            return;
        /* overlay vector necessary */
        OvlVectorize( sym );
    }
}

void OvlTryRefVector( symbol *sym )
/*********************************/
{
    if( wlink().FmtData.u.dos.distribute
      && (wlink().LinkState & LS_SEARCHING_LIBRARIES) ) {
        RefDistribSym( sym );
    } else {
        OvlRefVector( sym );
    }
}

void OvlUseVector( symbol *sym, extnode *newnode )
/************************************************/
{
    if( IS_SYM_COMMUNAL( sym ) )
        return;
    if( (sym->u.d.ovlstate & OVL_VEC_MASK) != OVL_MAKE_VECTOR )
        return;
    if( IsAncestor( sym->p.seg->u.leader->class_->section->ovlref, wlink().CurrSect ) ) {
        return;
    }
    /* use overlay vector for all calls */
    newnode->ovlref = sym->u.d.ovlref;
}

void OvlIndirectCall( symbol *sym )
/*********************************/
{
    overlay_ref ovlref;

    if( NO_VECTOR( sym ) )
        return;
    if( sym->info & SYM_DEFINED ) {
        if( sym->info & SYM_1_DISTRIB ) {
            DistribIndirectCall( sym );
        } else if( sym->p.seg != nullptr ) {
            ovlref = sym->p.seg->u.leader->class_->section->ovlref;
            if( ( ovlref != 0 )
              && sym->p.seg->iscode ) {
                OvlVectorize( sym );
            }
        }
    } else {
        // indicate a reference from the root for purposes of vector generation
        sym->u.d.ovlstate |= OVL_REF;
        sym->u.d.ovlref = 0;
    }
}

void OvlGetVecAddr( int vecnum, addr_type *addr )
/************************************************
 * return address of overlay vector in canonical form
 */
{
    *addr = ovl().vecAddr;
    if( wlink().FmtData.u.dos.ovl_short ) {
        /* short address -- fits into 16 bits */
        addr->off += ( vecnum - 1 ) * sizeof( svector ) + ( addr->seg << wlink().FmtData.SegShift );
        addr->seg = 0;
    } else {
        addr->off += ( vecnum - 1 ) * sizeof( lvector );
    }
}

bool OvlCheckOvlClass( const char *clname, bool *isovlclass )
/***********************************************************/
/* check if among overlay classes, and return true if it is code. */
{
    list_of_names       *cnamelist;
    bool                retval;

    retval = IsCodeClass( clname, strlen( clname ) );
    cnamelist = wlink().OvlClasses;
    if( cnamelist == nullptr ) {   /* check for anything ending in code */
        *isovlclass = retval;
        return( retval );
    } else {
        do {
            if( stricmp( clname, cnamelist->name ) == 0 ) {
                *isovlclass = true;
                return( retval );
            }
            cnamelist = cnamelist->next;
        } while( cnamelist != nullptr );
    }
    *isovlclass = false;
    return( false );
}

section *OvlCheckOvlSection( const char *clname )
/***********************************************/
{
    section     *sect;
    bool        dummy;

    sect = DBIGetSect( clname );
    if( sect == nullptr ) {
        if( OvlCheckOvlClass( clname, &dummy ) ) {
            sect = wlink().CurrSect;
        } else {
            sect = wlink().NonSect;
        }
    }
    return( sect );
}

static void PutOvlInfo( unsigned off, void *src, unsigned len )
/*************************************************************/
{
    PutInfo( ovl().ovlSegData->u1.vm_ptr + off - ovl().group->addr.off, src, len );
}

static void ShortVectors( symbol *loadsym )
/*****************************************/
{
    vecnode             *vectnode;
    unsigned            vectoff;
    svector             vectdata;
    unsigned_32         loadval;
    unsigned_32         temp;
    signed_32           diff;
    int                 vecnum;

    vectoff = ovl().vecAddr.off;
    temp = MK_REAL_ADDR( ovl().vecAddr.seg, ovl().vecAddr.off );
    /* get loader address */
    loadval = MK_REAL_ADDR( loadsym->addr.seg, loadsym->addr.off );
    /* fill in overlay vector */
    vectdata.call_op = 0xe8;
    vectdata.jmp_op = 0xe9;     // near jmp
    vecnum = 1;
    for( vectnode = ovl().vect.OvlVectors; vectnode != nullptr; vectnode = vectnode->next ) {
        temp = vectoff + offsetof( svector, ldr_addr ) + sizeof( unsigned_16 );
        diff = loadval - temp;
        if( ( diff < -32768 )
          || ( diff > 32767 ) ) {
            LnkMsg( ERR+MSG_VECT_RANGE, "sd", "short (1)", vecnum );
        }
        _HostU16toTarg( diff, vectdata.ldr_addr );
        loadsym = vectnode->sym;
        _HostU16toTarg( loadsym->p.seg->u.leader->class_->section->ovlref, vectdata.sec_num );
        temp = vectoff + offsetof( svector, target ) + sizeof( unsigned_16 );
        diff = MK_REAL_ADDR( loadsym->addr.seg, loadsym->addr.off ) - temp;
        if( ( diff < -32768 )
          || ( diff > 32767 ) ) {
            LnkMsg( ERR+MSG_VECT_RANGE, "sd", "short (2)", vecnum );
        }
        _HostU16toTarg( diff, vectdata.target );
        PutOvlInfo( vectoff, &vectdata, sizeof( vectdata ) );
        vectoff += sizeof( vectdata );
        ++vecnum;
    }
}

static void LongVectors( symbol *loadsym )
/****************************************/
{
    vecnode             *vectnode;
    unsigned            vectoff;
    lvector             vectdata;
    dos_addr            addr;
    signed_32           diff;
    unsigned_32         loadval;
    unsigned_32         temp;
    int                 vecnum;

    vectoff = ovl().vecAddr.off;
    /* fill in overlay vector */
    vectdata.u.v.call_op = 0xe8;
    vectdata.jmp_op = 0xea;     // far jmp
    loadval = loadsym->addr.off;
    addr.seg = ovl().group->addr.seg;
    vecnum = 1;
    for( vectnode = ovl().vect.OvlVectors; vectnode != nullptr; vectnode = vectnode->next ) {
        temp = vectoff + offsetof( lvector, u.v.ldr_addr ) + sizeof( unsigned_16 );
        diff = loadval - temp;
        if( ( diff < -32768 )
          || ( diff > 32767 ) ) {
            LnkMsg( ERR+MSG_VECT_RANGE, "sd", "long", vecnum );
        }
        _HostU16toTarg( diff, vectdata.u.v.ldr_addr );
        loadsym = vectnode->sym;
        _HostU16toTarg( loadsym->p.seg->u.leader->class_->section->ovlref, vectdata.u.v.sec_num );
        _HostU16toTarg( loadsym->addr.off, vectdata.target.off );
        if( wlink().FmtData.u.dos.dynamic ) {
            _HostU16toTarg( loadsym->addr.seg - loadsym->p.seg->u.leader->group->addr.seg, vectdata.target.seg );
        } else {
            _HostU16toTarg( loadsym->addr.seg, vectdata.target.seg );
            addr.off = vectoff + offsetof( lvector, target.seg );
            WriteReloc( ovl().group, addr.off, &addr, sizeof( dos_addr ) );
        }
        PutOvlInfo( vectoff, &vectdata, sizeof( vectdata ) );
        vectoff += sizeof( vectdata );
        vecnum++;
    }
}

void OvlEmitVectors( void )
/*************************/
{
    symbol      *symptr;
    dos_addr    addr;
    const char  *loader_name;
    bool        isshort;

    /* output relocation items for overlay table */
    addr.seg = ovl().group->addr.seg;
    addr.off = ovl().tabAddr.off + offsetof( ovltab_prolog, start.seg );
    WriteReloc( ovl().group, addr.off, &addr, sizeof( dos_addr ) );
    addr.off = ovl().tabAddr.off + offsetof( ovltab_prolog, delta );
    WriteReloc( ovl().group, addr.off, &addr, sizeof( dos_addr ) );

    isshort = false;
    if( wlink().FmtData.u.dos.dynamic ) {
        loader_name = _DynamicOvlldr;
    } else if( wlink().FmtData.u.dos.ovl_short ) {
        loader_name = _ShortOvlldr;
        isshort = true;
    } else {
        loader_name = _LongOvlldr;
    }
    symptr = RefISymbol( loader_name );
    if( (symptr->info & SYM_DEFINED) == 0 ) {
        LnkMsg( ERR+MSG_NO_OVERLAY_LOADER, nullptr );
    } else if( isshort ) {
        ShortVectors( symptr );
    } else {
        LongVectors( symptr );
    }
}

void OvlSetStartAddr( void )
/**************************/
{
    symbol      *sym;

    /* stuff overlay init routine address in header */
    ovl().stashAddr = wlink().StartInfo.addr;

    if( wlink().FmtData.u.dos.dynamic ) {
        sym = FindISymbol( _DynamicInitRtn );
    } else if( wlink().FmtData.u.dos.ovl_short ) {
        sym = FindISymbol( _ShortOvlInitRtn );
    } else {
        sym = FindISymbol( _LongOvlInitRtn );
    }
    if( sym == nullptr )
        return;
    if( sym->info & SYM_DEFINED ) {
        wlink().StartInfo.addr = sym->addr;
    }
}

void OvlPass1( void )
/*******************/
{
    symbol      *sym;

    WalkAreas( wlink().Root->areas, LoadObjFiles );

    /* define symbols for overlay table */
    ovl().vect.OverlayTable = DefISymbol( _OvltabName );
    ovl().vect.OverlayTableEnd = DefISymbol( _OvltabEndName );
    ovl().vect.OvlVecStart = DefISymbol( _OvlVecStartName );
    ovl().vect.OvlVecEnd = DefISymbol( _OvlVecEndName );

    wlink().OvlSeg = InitLeader( "" );
    wlink().OvlSeg->class_ = FindClass( wlink().Root, OvlMgrClass, BITS_16, true );
    wlink().OvlSeg->combine = COMBINE_INVALID;
    ovl().ovlSegData = AllocSegData();
    ovl().ovlSegData->bits = BITS_16;
    ovl().ovlSegData->u.leader = wlink().OvlSeg;

    ovl().vect.OverlayTable->p.seg = ovl().ovlSegData;
    ovl().vect.OverlayTableEnd->p.seg = ovl().ovlSegData;
    ovl().vect.OvlVecStart->p.seg = ovl().ovlSegData;
    ovl().vect.OvlVecEnd->p.seg = ovl().ovlSegData;

    /* generate reference for overlay loader */
    if( wlink().FmtData.u.dos.dynamic ) {
        RefISymbol( _DynamicInitRtn );
        sym = RefISymbol( _DynamicOvlldr );
    } else if( wlink().FmtData.u.dos.ovl_short ) {
        RefISymbol( _ShortOvlInitRtn );
        sym = RefISymbol( _ShortOvlldr );
    } else {
        RefISymbol( _LongOvlInitRtn );
        sym = RefISymbol( _LongOvlldr );
    }
    sym->u.d.ovlref = 0;
    sym->u.d.ovlstate |= OVL_REF;
}

static void EmitOvlEntry( section *sect, void *_off )
/***************************************************/
{
    ovltab_entry        entry;
    offset              len;
    unsigned_16         flags_anc;
    unsigned_16         start_para;
    unsigned            *off = static_cast<unsigned *>( _off );

    _HostU16toTarg( 0, entry.code_handle );
    flags_anc = sect->parent->ovlref;
    if( sect == wlink().NonSect ) {
        flags_anc |= OVE_FLAG_PRELOAD;/*  pre-load the data area */
    }
    _HostU16toTarg( sect->outfile->ovlfnoff, entry.fname );
    _HostU16toTarg( sect->relocs, entry.relocs );
    _HostU16toTarg( flags_anc, entry.flags_anc );
    start_para = sect->sect_addr.seg + ( sect->sect_addr.off >> wlink().FmtData.SegShift );
    _HostU16toTarg( start_para, entry.start_para );
    len = __ROUND_UP_SIZE_TO_PARA( sect->size );
    _HostU16toTarg( len, entry.num_paras );
    _HostU32toTarg( sect->u.file_loc, entry.disk_addr );
    PutOvlInfo( *off, &entry, sizeof( entry ) );
    *off += sizeof( ovltab_entry );
}

void OvlEmitTable( void )
/***********************/
/* generate overlay table */
{
    unsigned            off;
    ovltab_prolog       prolog;
    unsigned_16         u16;
    unsigned            len;
    outfilelist         *fnode;

    off = ovl().tabAddr.off;
    /*
     *  Generate prolog :
     */
    prolog.major = OVL_MAJOR_VERSION;
    prolog.minor = OVL_MINOR_VERSION;
    /*
     *  output start address for program
     *  reloc for this was emitted by OvlEmitVectors
     */
    _HostU16toTarg( ovl().stashAddr.off, prolog.start.off );
    _HostU16toTarg( ovl().stashAddr.seg, prolog.start.seg );
    /*
     *  this should give us the paragraph of the load module start
     *  reloc for this was emitted by OvlEmitVectors
     */
    _HostU16toTarg( 0, prolog.delta );
    _HostU16toTarg( wlink().OvlAreaSize, prolog.ovl_size );

    PutOvlInfo( off, &prolog, sizeof( prolog ) );
    /*
     *  Generate entries :
     */
    off += sizeof( prolog );
    ParmWalkAreas( wlink().Root->areas, EmitOvlEntry, &off );
    /*
     *  Generate epilog :
     */
    u16 = OVLTAB_TERMINATOR;
    PutOvlInfo( off, &u16, sizeof( unsigned_16 ) );
    off += sizeof( unsigned_16 );
    /* generate overlay filenames, including NULLCHARS */
    for( fnode = wlink().OutFiles; fnode != nullptr; fnode = fnode->next ) {
        len = strlen( fnode->fname ) + 1;
        PutOvlInfo( off, fnode->fname, len );
        off += len;
    }
}

void OvlPadOvlFiles( void )
/*************************/
// The overlay files must contain a complete paragraph at the end of the file
// for the overlay loader to be able to correctly read it.
{
    outfilelist     *fnode;
    size_t          pad;

    for( fnode = wlink().OutFiles; fnode != nullptr; fnode = fnode->next ) {
        pad = __ROUND_UP_SIZE_PARA( fnode->file_loc ) - fnode->file_loc;
        if( pad ) {
            PadBuffFile( fnode, pad );
        }
    }
}

void OvlSetTableLoc( group_entry *group, unsigned long loc )
/**********************************************************/
{
    if( group == ovl().group ) {
        wlink().OvlTabOffset = loc;
    }
}

#endif
