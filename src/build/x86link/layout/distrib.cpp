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
* Description:  routines for distributing libraries over overlays
*
****************************************************************************/

#include <cstring>
#include "build/x86link/link/linkstd.h"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state */
#include "pcobj.h"
#include "build/x86link/symbols/objpass1.h"
#include "build/x86link/symbols/objpass2.h"
#include "build/x86link/objects/objfree.h"
#include "build/x86link/symbols/wcomdef.h"
#include "build/x86link/layout/overlays.h"
#include "build/x86link/base/ring.h"
#include "build/x86link/layout/distrib.h"
#include "build/x86link/link/specials.h"
#include "build/x86link/formats/dos16m.h"
#include "build/x86link/map/map.h"
#include "build/x86link/link/run_state.hpp"


#ifdef _EXE

#define INITIAL_MOD_ALLOC   32
#define INITIAL_ARC_ALLOC   32
#define MAX_NUM_MODULES     _8K

namespace {

// what distrib.cpp keeps for one run of the linker
struct DistribState {
    section         **SectOvlTab = nullptr;
    arcdata         *ArcList = nullptr;
    unsigned_32     ArcListMaxLen = 0;
    mod_entry       **ModTable = nullptr;
    unsigned_16     ModTableMaxLen = 0;
    unsigned_16     CurrModHandle = 0;
};

DistribState &distrib()
{
    return cb_wlink::RunState::part<DistribState>();
}

inline overlay_ref LowestAncestorMap( overlay_ref o, overlay_ref i )
{
    return LowestAncestor( o, distrib().SectOvlTab[i] );
}

}

/* forward declarations */

static void     ScanArcs(Link& link,  mod_entry *mod );

static void DistribNumASect(Link& link,  section *sect )
/******************************************/
{
    DistribState    &d = distrib();

    d.SectOvlTab[link.OvlSectNum++] = sect;
}

void DistribNumberSections(Link& link)
/********************************/
{
    DistribState    &d = distrib();

    d.SectOvlTab = static_cast<section **>( MemAllocSafe(link,  sizeof( section * ) * ( link.OvlSectNum + 1 ) ) );
    /* OvlSectNum value 0 is reserved for Root */
    /* Overlayed sections start at 1 */
    link.OvlSectNum = 0;
    DistribNumASect(link,  link.Root );
    WalkAreas( link.Root->areas, bind_link( link, DistribNumASect ) );
}

void ResetDistribSupp( void )
/***************************/
{
    DistribState    &d = distrib();

    d.ArcList = nullptr;
    d.ArcListMaxLen = 0;
    d.ModTable = nullptr;
    d.ModTableMaxLen = 0;
    d.SectOvlTab = nullptr;
}

void DistribInitMods(Link& link)
/**************************/
{
    DistribState    &d = distrib();

    d.ModTableMaxLen = INITIAL_MOD_ALLOC;
    d.ModTable = static_cast<mod_entry **>( MemAllocSafe(link,  INITIAL_MOD_ALLOC * sizeof( mod_entry * ) ) );
    d.CurrModHandle = 0;
    d.ArcListMaxLen = INITIAL_ARC_ALLOC;
    d.ArcList = static_cast<arcdata *>( MemAllocSafe(link,  offsetof( arcdata, arcs ) + INITIAL_ARC_ALLOC * sizeof( dist_arc ) ) );
    d.ArcList->numarcs = 0;
    ResetPass1Blocks();
}

void DistribAddMod(Link& link,  mod_entry *lp, overlay_ref ovlref )
/******************************************************
 * add this module to the table, and make the arclist field point to a
 * scratch buffer
 * NYI: segdata changes have completely broken distributing libraries.
 * fix this!
 */
{
    DistribState    &d = distrib();

    mod_entry   **new_;

    d.CurrModHandle++;
    if( d.CurrModHandle == d.ModTableMaxLen ) {
        if( d.ModTableMaxLen > MAX_NUM_MODULES ) {
            LnkMsg(link,  FTL+MSG_TOO_MANY_LIB_MODS, nullptr );
        }
        new_ = static_cast<mod_entry **>( MemAllocSafe(link,  sizeof( mod_entry * ) * d.ModTableMaxLen * 2 ) );
        memcpy( new_, d.ModTable, sizeof( mod_entry * ) * d.ModTableMaxLen );
        MemFree( d.ModTable );
        d.ModTable = new_;
        d.ModTableMaxLen *= 2;
    }
    d.ModTable[d.CurrModHandle] = lp;
    lp->u2.arclist = d.ArcList;
    d.ArcList->numarcs = 0;
    if( lp->flags_mod & MOD_FIXED ) {
        d.ArcList->ovlref = ovlref;
    } else {
        d.ArcList->ovlref = NO_ARCS_YET;
    }
}

void InitArcList(Link& link,  mod_entry *mod )
/*********************************
 * set up the mod_entry arcdata field for dead code elimination
 */
{
    if( (link.FmtData.type & MK_OVERLAYS)
      && link.FmtData.u.dos.distribute
      && (link.LinkState & LS_SEARCHING_LIBRARIES) ) {
    } else {
        mod->u2.arclist = static_cast<arcdata *>( PermAlloc( link, offsetof( arcdata, arcs ) ) );
    }
}

static void MarkDead(Link& link,  void *_seg )
/********************************/
{
    segdata     *seg = static_cast<segdata *>( _seg );

    if( seg->isrefd )
        return;
    if( seg->isdead )
        return;

    if( seg->iscode ) {
        seg->isdead = true;
    } else {
        if( link.FmtData.type & MK_PE ) {
            char *segname = seg->u.leader->segname.u.ptr;
            if( ( strcmp( segname, CoffPDataSegName ) == 0 )
              || ( strcmp( segname, CoffReldataSegName ) == 0 ) ) {
                seg->isdead = true;
            }
        }
    }
}

static void KillUnrefedSyms(Link& link,  void *_sym )
/***************************************/
{
    symbol      *sym = static_cast<symbol *>( _sym );
    segdata     *seg;

    seg = sym->p.seg;
    if( ( seg != nullptr )
      && !IS_SYM_IMPORTED( sym )
       && !IS_SYM_ALIAS( sym )
       && seg->isdead ) {
        if( seg->u.leader->combine == COMBINE_COMMON ) {
            seg = static_cast<segdata *>( RingFirst( seg->u.leader->pieces ) );
            if( !seg->isdead ) {
                return;
            }
        }
        if( sym->e.def != nullptr ) {
            WeldSyms(link,  sym, sym->e.def );
        } else {
            sym->info |= SYM_DEAD;
        }
        if( link.LinkFlags & LF_SHOW_DEAD ) {
            if( link.MapFile != nullptr ) {
                WriteMapLnkMsg( link, MSG_SYMBOL_DEAD, "S", sym );
            }
        }
    }
}

static void DefineOvlSegments(Link& link,  mod_entry *mod )
/**********************************************
 * figure out which of the segments are live
 */
{
    Ring2Walk( mod->segs, bind_link( link, MarkDead ) );
    Ring2Walk( mod->publist, bind_link( link, KillUnrefedSyms ) );
}

void DistribSetSegments(Link& link)
/******************************
 * now that we know where everything is, do all the processing that has been
 * postponed until now.
 */
{
    DistribState    &d = distrib();

    if( (link.LinkFlags & LF_STRIP_CODE) == 0 )
        return;
    link.LinkState &= ~LS_CAN_REMOVE_SEGMENTS;
    link.ObjFormat |= OBJ_FMT_DEBUG_COMENT;
    if( (link.FmtData.type & MK_OVERLAYS)
      && link.FmtData.u.dos.distribute ) {
        MemFree( d.ArcList );
        d.ArcList = nullptr;
    }
    if( link.LinkFlags & LF_STRIP_CODE ) {
        WalkMods(link,  bind_link( link, DefineOvlSegments ) );
    }
#if 0           // NYI: distributing libraries completely broken.
    unsigned        index;
    mod_entry       *mod;
    overlay_ref     ovlref;
    mod_entry       **currmod;
    unsigned        num_segdefs;

    if( (link.FmtData.type & MK_OVERLAYS)
      && link.FmtData.u.dos.distribute ) {
        for( index = 1; index <= d.CurrModHandle; index++ ) {
            mod = d.ModTable[index];
            link.CurrMod = mod;
            ovlref = mod->u2.arclist->ovlref;
            if( ovlref == NO_ARCS_YET ) {       // only data referenced
                link.CurrSect = link.Root;
            } else {
                link.CurrSect = d.SectOvlTab[ovlref];
            }
            DefModSegments( mod );
            mod->x.next = nullptr;
            for( currmod = &link.CurrSect->u.dist_mods; *currmod != nullptr; ) {
                currmod = &((*currmod)->x.next);
            }
            *currmod = mod;
            mod->n.sect = link.CurrSect;
        }
    }
    FixGroupProblems();
    FindRedefs();
    if( (link.FmtData.type & MK_OVERLAYS)
      && link.FmtData.u.dos.distribute ) {
        MemFree( d.SectOvlTab );
        d.SectOvlTab = nullptr;
    }
#endif
    if( (link.FmtData.type & MK_OVERLAYS)
      && link.FmtData.u.dos.distribute ) {
        MemFree( d.SectOvlTab );
        d.SectOvlTab = nullptr;
    }
    ReleasePass1Blocks();
}

void FreeDistribSupp(Link& link)
/**************************/
{
    DistribState    &d = distrib();

    unsigned    index;

    for( index = 1; index <= d.CurrModHandle; index++ ) {
        FreeAMod(link,  d.ModTable[index] );
    }
    MemFree( d.ModTable );
    MemFree( d.ArcList );
    MemFree( d.SectOvlTab );
    ReleasePass1Blocks();
}

void DistribProcMods(Link& link)
/**************************/
{
    DistribState    &d = distrib();

    unsigned_16 index;
    mod_entry   *mod;

    for( index = 1; index <= d.CurrModHandle; index++ ) {
        mod = d.ModTable[index];
        link.CurrSect = mod->u.sect;
        PModule(link,  mod );
    }
}

overlay_ref LowestAncestor( overlay_ref ovlref, section *sect )
/**************************************************************
 * find the lowest common ancestor of the two overlay values by marking all of
 * the ancestors of the first overlay, and then looking for marked ancestors
 * of the other overlay
 */
{
    DistribState    &d = distrib();

    section     *list;

    for( list = sect; list != nullptr; list = list->parent ) {
        // set visited flag
        list->next = reinterpret_cast<SECTION *>( reinterpret_cast<pointer_int>( list->next ) | 1 );
    }
    for( list = d.SectOvlTab[ovlref]; list != nullptr; list = list->parent ) {
        // check visited flag
        if( reinterpret_cast<pointer_int>( list->next ) & 1 ) {
            break;
        }
    }
    for( ; sect != nullptr; sect = sect->parent ) {
        // reset visited flag
        sect->next = reinterpret_cast<SECTION *>( reinterpret_cast<pointer_int>( sect->next ) & ~pointer_int( 1 ) );
    }
    return( list->ovlref );
}

static bool NewRefVector(Link& link,  symbol *sym, overlay_ref ovlref, overlay_ref sym_ovlref )
/**********************************************************************************
 * sometimes there can be an overlay vector generated to a routine specified
 * in an .OBJ file caused by a call from a library routine. this checks for
 * this case.
 */
{
    if( ( sym->p.seg == nullptr )
      || ( (sym->u.d.ovlstate & OVL_VEC_MASK) != OVL_UNDECIDED ) ) {
        return( true );
    }
    /*
     * at this point, we know it has already been defined, but does not have an
     * overlay vector, and is not data
     */
    if( LowestAncestorMap( sym_ovlref, ovlref ) != sym_ovlref ) {
        OvlVectorize(link,  sym );
        return( true );
    }
    return( false );
}

void DefDistribSym(Link& link,  symbol *sym )
/********************************
 * move current module based on where this symbol has been referenced from,
 * and make the symbol point to the current module. All symbols which get
 * passed to this routine are in an overlay class.
 */
{
    DistribState    &d = distrib();

    arcdata     *arclist;
    segdata     *seg;

    if( sym->info & SYM_REFERENCED ) {
        arclist = link.CurrMod->u2.arclist;
        if( link.CurrMod->flags_mod & MOD_FIXED ) {
            seg = sym->p.seg;
            if( seg->iscode ) {      // if code..
                NewRefVector(link,  sym, sym->u.d.ovlref, arclist->ovlref );
            } else if( (sym->u.d.ovlstate & OVL_FORCE) == 0 ) {
                // don't generate a vector.
                sym->u.d.ovlstate |= OVL_FORCE | OVL_NO_VECTOR;
            }
        } else {
            if( arclist->ovlref == NO_ARCS_YET ) {
                arclist->ovlref = sym->u.d.ovlref;
            } else {
                arclist->ovlref = LowestAncestorMap( arclist->ovlref, sym->u.d.ovlref );
            }
        }
    }
    sym->u.d.modnum = d.CurrModHandle;
}

static void AddArc(Link& link,  dist_arc arc )
/*********************************
 * add an arc to the arclist for the current module
 */
{
    DistribState    &d = distrib();

    arcdata     *arclist;

    arclist = link.CurrMod->u2.arclist;
    if( arclist->numarcs >= d.ArcListMaxLen ) {
        arclist = static_cast<arcdata *>( MemAllocSafe(link,  offsetof( arcdata, arcs ) + 2 * d.ArcListMaxLen * sizeof( dist_arc ) ) );
        memcpy( arclist, d.ArcList, offsetof( arcdata, arcs ) + d.ArcListMaxLen * sizeof( dist_arc ) );
        MemFree( d.ArcList );
        link.CurrMod->u2.arclist = arclist;
        d.ArcList = arclist;
        d.ArcListMaxLen *= 2;
    }
    arclist->arcs[arclist->numarcs] = arc;
    arclist->numarcs++;
}

static bool NotAnArc(Link& link,  dist_arc arc )
/***********************************
 * return true if this is not an arc in the current module
 */
{
    unsigned    index;
    arcdata     *arclist;

    arclist = link.CurrMod->u2.arclist;
    for( index = arclist->numarcs; index-- > 0; ) {
        if( arclist->arcs[index].test == arc.test ) {
            return( false );
        }
    }
    return( true );
}

void RefDistribSym(Link& link,  symbol *sym )
/********************************
 * add an arc to the reference graph if it is not already in the graph
 */
{
    DistribState    &d = distrib();

    mod_entry   *mod;
    segdata     *seg;
    dist_arc    arc;

    arc.sym = sym;
    if( sym->info & SYM_DEFINED ) {
        if( sym->info & SYM_1_DISTRIB ) {
            mod = d.ModTable[sym->u.d.modnum];
            if( mod->flags_mod & MOD_FIXED ) {        // add reference, as long
                seg = sym->p.seg;                   // as it is a code ref.
                if( seg->iscode ) {
                    AddArc(link,  arc );
                }
            } else {
                arc.test = sym->u.d.modnum;
                if( NotAnArc(link,  arc )
                  && ( sym->u.d.modnum != d.CurrModHandle ) ) {
                    AddArc(link,  arc );
                }
            }
        } else if( (sym->u.d.ovlstate & OVL_VEC_MASK) == OVL_UNDECIDED ) {
            if( NotAnArc(link,  arc ) ) {
                AddArc(link,  arc );
            }
        }
    } else {   // just a reference, so it has to be added to the call graph.
        AddArc(link,  arc );
    }
}

static void DoRefGraph(Link& link,  overlay_ref ovlref, mod_entry *mod )
/***********************************************************
 * checks to see if the mod has changed position, and if it has, check all
 * of the routines that mod references
 */
{
    arcdata     *arclist;
    overlay_ref anc_ovlref;

    arclist = mod->u2.arclist;
    /*
     * this next line is necessary to break cycles in the graph.
     */
    if( (mod->flags_mod & MOD_VISITED)
      && ( ovlref == arclist->ovlref )
      || (mod->flags_mod & MOD_FIXED) )
        return;
    if( arclist->ovlref == NO_ARCS_YET ) {
        arclist->ovlref = 0;
        anc_ovlref = 0;
        ScanArcs(link,  mod );
    } else {
        anc_ovlref = LowestAncestorMap( ovlref, arclist->ovlref );
        if( anc_ovlref != arclist->ovlref ) {
            arclist->ovlref = anc_ovlref;
            ScanArcs(link,  mod );
        }
    }
    if( anc_ovlref == 0 ) {   // it's at the root, so pull it out of the graph
        arclist->numarcs = 0;
    }
}

static void DeleteArc( arcdata *arclist, unsigned_16 index )
/***********************************************************
 * delete an arc from the specified arc list
 */
{
    arclist->numarcs--;
    if( arclist->numarcs > 0 ) {
        arclist->arcs[index] = arclist->arcs[arclist->numarcs];
    }
}

static void ScanArcs(Link& link,  mod_entry *mod )
/*************************************
 * go through all modules referenced by mod, and see if they need to change
 * position because of the position of mod
 */
{
    DistribState    &d = distrib();

    arcdata     *arclist;
    symbol      *sym;
    mod_entry   *refmod;
    unsigned_16 index;
    overlay_ref ovlref;
    dist_arc    currarc;

    mod->flags_mod |= MOD_VISITED;
    arclist = mod->u2.arclist;
    ovlref = arclist->ovlref;
    if( ovlref != NO_ARCS_YET ) {
        for( index = arclist->numarcs; index-- > 0; ) {
            currarc = arclist->arcs[index];
            if( currarc.test <= MAX_NUM_MODULES ) {     // GIANT KLUDGE!
                DoRefGraph(link,  ovlref, d.ModTable[currarc.mod] );
            } else {
                sym = currarc.sym;
                if( sym->info & SYM_DEFINED ) {
                    if( sym->info & SYM_1_DISTRIB ) {
                        currarc.test = sym->u.d.modnum;
                        refmod = d.ModTable[currarc.mod];
                        if( refmod->flags_mod & MOD_FIXED ) {
                            if( NewRefVector(link,  sym, ovlref, refmod->u2.arclist->ovlref ) ) {
                                DeleteArc( arclist, index );
                            }
                        } else {
                            DoRefGraph(link,  ovlref, refmod );
                            if( !NotAnArc(link,  currarc ) ) {
                                DeleteArc( arclist, index );
                            } else {
                                arclist->arcs[index] = currarc;
                            }
                        }
                    } else {
                        if( ( sym->p.seg == nullptr )
                          || NewRefVector(link,  sym, ovlref, sym->p.seg->u.leader->class_->section->ovlref ) ) {
                            DeleteArc( arclist, index );
                        }
                    }
                } else {
                    if( (sym->u.d.ovlstate & OVL_REF) == 0 ) {
                        sym->u.d.ovlref = ovlref;
                        sym->u.d.ovlstate |= OVL_REF;
                    } else {
                        sym->u.d.ovlref = LowestAncestorMap( ovlref, sym->u.d.ovlref );
                    }
                }
            } /* if( a module ) */
        } /* for( arcs left ) */
    } /* if( an ovlnum defined ) */
    mod->flags_mod &= ~MOD_VISITED;
}

void DistribFinishMod(Link& link,  mod_entry *mod )
/**************************************
 * check the position of the modules referenced by mod, and then make a
 * more permanent copy of the arclist for this module.
 */
{
    arcdata     *arclist;
    unsigned    allocsize;

    ScanArcs(link,  mod );
    if( mod->flags_mod & MOD_FIXED ) {    // no need to scan a fixed module more than once
        mod->u2.arclist->numarcs = 0;
    }
    allocsize = offsetof( arcdata, arcs ) + mod->u2.arclist->numarcs * sizeof( dist_arc );
    arclist = static_cast<arcdata *>( Pass1Alloc(link,  allocsize ) );
    memcpy( arclist, mod->u2.arclist, allocsize );
    mod->u2.arclist = arclist;
}

void DistribIndirectCall(Link& link,  symbol *sym )
/**************************************
 * handle indirect calls and their effect on distributed libs.
 */
{
    DistribState    &d = distrib();

    arcdata         *arclist;
    overlay_ref     save_ovlref;

    arclist = link.CurrMod->u2.arclist;
    save_ovlref = arclist->ovlref;
    arclist->ovlref = 0;                // make sure current module isn't
    link.CurrMod->flags_mod |= MOD_VISITED;    // visited
    DoRefGraph(link,  0, d.ModTable[sym->u.d.modnum] );
    link.CurrMod->flags_mod &= ~MOD_VISITED;
    arclist->ovlref = save_ovlref;
    sym->u.d.ovlstate |= OVL_REF;
    sym->u.d.ovlref = 0;                // make sure current symbol put in root.
}

#endif
