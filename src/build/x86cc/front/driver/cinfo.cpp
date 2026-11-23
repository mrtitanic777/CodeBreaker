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
* Description:  Segment management and FE callbacks.
*
****************************************************************************/


#include "cvars.h"
#include "cg.h"
#include "cgdefs.h"
#include "cgswitch.h"
#include "cgstd.h"
#include "cgaux.h"
#include "langenv.h"
#include "cgprotos.h"
#include "feprotos.h"
#include "caux.h"
#include "cfeinfo.h"

#include "clibext.h"
#include "frontrun.hpp"


#define FIRST_USER_SEGMENT      10000

inline bool IsConstant( type_modifiers decl_flags ) { return ((decl_flags & MASK_CV_QUALIFIERS) == FLAG_CONST); }

typedef struct user_seg {
    struct user_seg *next;
    char            *name;
    SYM_HANDLE      sym_handle;
    int             segtype;
    segment_id      segid;
    char            *class_name;
    hw_reg_set      pegged_register;
    bool            used;
} user_seg;

typedef struct seg_name {
    const char      *name;
    segment_id      segid;
    SYM_HANDLE      sym_handle;
} seg_name;

typedef struct spc_info {
    const char      *name;
    const char      *class_name;
    seg_type        segtype;
} spc_info;


static const spc_info InitFiniSegs[][3] = {
    {
        { TS_SEG_TIB, "DATA",           SEGTYPE_INITFINI },
        { TS_SEG_TI,  "DATA",           SEGTYPE_INITFINI },
        { TS_SEG_TIE, "DATA",           SEGTYPE_INITFINI }
    },
    {
        { TS_SEG_XIB, "DATA",           SEGTYPE_INITFINI },
        { TS_SEG_XI,  "DATA",           SEGTYPE_INITFINI },
        { TS_SEG_XIE, "DATA",           SEGTYPE_INITFINI }
    },
    {
        { TS_SEG_YIB, "DATA",           SEGTYPE_INITFINI },
        { TS_SEG_YI,  "DATA",           SEGTYPE_INITFINI },
        { TS_SEG_YIE, "DATA",           SEGTYPE_INITFINI }
    },
    {
        { TS_SEG_TLSB, TS_SEG_TLS_CLASS,SEGTYPE_INITFINITR },
        { TS_SEG_TLS,  TS_SEG_TLS_CLASS,SEGTYPE_INITFINITR },
        { TS_SEG_TLSE, TS_SEG_TLS_CLASS,SEGTYPE_INITFINITR }
    }
};


namespace {

/* the statics of this file, one set for each run of the compiler */
}  // namespace
struct CinfoState {
    seg_name Predefined_Segs[4] = {
        { "_CODE",      SEG_CODE,       SYM_NULL },
        { "_CONST",     SEG_CONST,      SYM_NULL },
        { "_DATA",      SEG_DATA,       SYM_NULL },
        { "_STACK",     SEG_STACK,      SYM_NULL },
    };
    user_seg *userSegments;
    segment_id userSegId;
    segment_id import_segid = SEG_NULL;   /* next segment # for import sym */
    segment_id import_near_segid = SEG_NULL;   /* data seg # for -nd option */
};
namespace {

}  // namespace



void AssignSeg( FrontRun& front, SYMPTR sym )
{
    SetFarHuge( front, sym, true );
    if( (sym->attribs.stg_class == SC_AUTO)
      || (sym->attribs.stg_class == SC_REGISTER)
      || (sym->attribs.stg_class == SC_TYPEDEF) ) {
        /*
         * if stack/register var, there is no segment
         */
        sym->u.var.segid = SEG_NULL;
    } else if( sym->attribs.stg_class != SC_EXTERN ) {  /* if not imported */
        if( (sym->flags & SYM_INITIALIZED) == 0 ) {
            if( sym->u.var.segid == SEG_NULL ) {
                SetSegment( front, sym );
            }
            if( sym->u.var.segid == SEG_DATA ) {
                sym->u.var.segid = SEG_BSS;
                front.cvars.CompFlags.bss_segment_used = true;
            }
            SetSegAlign( front, sym );
        }
    } else if( sym->mods & (FLAG_FAR | FLAG_HUGE) ) {
        sym->u.var.segid = front.cinfo->import_segid--;
    } else if( (front.cinfo->import_near_segid != SEG_NULL) && (sym->mods & FLAG_NEAR) ) {  // imported and near
        sym->u.var.segid = front.cinfo->import_near_segid;
    }
}


void SetFarHuge( FrontRun& front, SYMPTR sym, bool report )
{
    TYPEPTR             typ;
    type_modifiers      attrib;
    target_size         size;

  #if _CPU != 8086
    /* unused parameters */ (void)report;
  #endif

  #if _CPU == 8086
    if( sym->attribs.declspec == DECLSPEC_DLLIMPORT
      || sym->attribs.declspec == DECLSPEC_DLLEXPORT ) {
        sym->mods |= FLAG_FAR;
    } else if( sym->mods & FLAG_EXPORT ) {
        sym->mods |= FLAG_FAR;
    }
  #endif
    size = SizeOfArg( front, sym->sym_type );
    if( front.cvars.TargetSwitches & CGSW_X86_BIG_DATA ) {
        attrib = sym->mods;
        if( (attrib & MASK_ALL_MEM_MODELS) == 0 ) {
            if( size == 0 ) {   /* unspecified array size */
                if( sym->attribs.stg_class == SC_EXTERN ) {
                    typ = sym->sym_type;
                    if( typ->decl_type == TYP_ARRAY ) {
                        attrib |= FLAG_FAR;
                    }
                }
            } else if( size > front.cvars.DataThreshold ) {
                attrib |= FLAG_FAR;
            } else if( front.cvars.CompFlags.strings_in_code_segment && (sym->mods & FLAG_CONST) ) {
                attrib |= FLAG_FAR;
            }
  #if _CPU == 8086
            if( (attrib & FLAG_FAR) && size > 0x10000 ) {
                attrib &= ~FLAG_FAR;
                attrib |= FLAG_HUGE;
            }
  #endif
            sym->mods = attrib;
        }
    }
  #if _CPU == 8086
   if( report && size > 0x10000 && (sym->mods & FLAG_HUGE) == 0 ) {
        SetErrLoc( front, &sym->src_loc );
        CErr1( front, ERR_VAR_TOO_LARGE );
        InitErrLoc(front);
   }
  #endif
}

static fe_attr FESymAttr( FrontRun& front, SYMPTR sym )
/************************************/
{
    fe_attr         attr;

    attr = static_cast<fe_attr>(0);
    switch( sym->attribs.stg_class ) {
    case SC_FORWARD:
    case SC_EXTERN:
        attr = FE_GLOBAL | FE_IMPORT | FE_STATIC;
        break;
    case SC_NONE:
        attr = FE_GLOBAL | FE_STATIC;
        break;
    case SC_STATIC:
        attr = FE_STATIC | FE_VISIBLE;
        if( sym->level != 0 ) {
            attr |= FE_INTERNAL;
        }
        break;
    }
    if( sym->flags & SYM_FUNCTION ) {
        attr |= FE_PROC | FE_STATIC;
        if( VarFunc( front, sym ) )
            attr |= FE_VARARGS;
        if( front.cvars.CompFlags.unique_functions ) {
            if( (attr & FE_GLOBAL) || (sym->flags & SYM_ADDR_TAKEN) ) {
                attr |= FE_UNIQUE;
            }
        }
    }
    if( sym->flags & SYM_USED_IN_PRAGMA ) {
        attr |= FE_MEMORY | FE_ADDR_TAKEN | FE_VOLATILE;
    }
    if( sym->flags & SYM_TRY_VOLATILE ) {
        attr |= FE_MEMORY | FE_VOLATILE;
    }
    if( sym->mods & FLAG_VOLATILE ) {
        attr |= FE_MEMORY | FE_VOLATILE;
    } else if( sym->mods & FLAG_CONST ) {
        attr |= FE_CONSTANT;
    }
    switch( sym->attribs.declspec ) {
    case DECLSPEC_DLLIMPORT:
        if( (attr & FE_IMPORT) || front.cvars.CompFlags.rent ) {
            attr |= FE_DLLIMPORT;
        }
        break;
    case DECLSPEC_DLLEXPORT:
        if( sym->attribs.stg_class == SC_NONE ) {
            attr |= FE_DLLEXPORT;
        }
        break;
    case DECLSPEC_THREAD:
        attr |= FE_THREAD_DATA;
        break;
    }
    if( sym->attribs.naked ) {
        attr |= FE_NAKED;
    }
    if( sym->attribs.rent ) {   //Override on function or r/w data
        attr |= FE_THREAD_DATA;
    }
    return( attr );
}


void    FEGenProc( FrontRun& front, CGSYM_HANDLE hdl, call_handle call )
/*****************************************************/
{
    SYM_HANDLE      sym_handle = (SYM_HANDLE)hdl;

    /* unused parameters */ (void)call;

    GenInLineFunc( front, sym_handle );
}


fe_attr FEAttr( FrontRun& front, CGSYM_HANDLE cgsym_handle )
/*****************************************/
{
    SYM_HANDLE sym_handle = (SYM_HANDLE)cgsym_handle;

    return( FESymAttr( front, SymGetPtr( front, sym_handle ) ) );
}

segment_id SymSegId( FrontRun& front, SYMPTR sym )
{
    fe_attr     attr;

    attr = FESymAttr( front, sym );
    if( attr & FE_PROC )
        return( SEG_CODE );
    if( !front.cvars.CompFlags.rent ) {
        if( attr & FE_CONSTANT ) {
            return( SEG_CONST2 );
        }
    }
    return( SEG_DATA );
}


void SetSegment( FrontRun& front, SYMPTR sym )
{
    segment_list        *seg;
    target_size         size;

  #if _CPU == 8086
    if( (sym->mods & FLAG_FAR) && front.cvars.CompFlags.zc_switch_used ) {
        if( IsConstant( sym->mods )
          || (sym->attribs.stg_class == SC_STATIC && (sym->flags & SYM_TEMP)) ) {
            sym->u.var.segid = SEG_CODE;
            return;
        }
    }
  #else /* _CPU == 386 */
    if( !front.cvars.CompFlags.rent ) {
        if( (sym->mods & FLAG_FAR) || (front.cvars.TargetSwitches & CGSW_X86_FLAT_MODEL) ) {
           if( IsConstant( sym->mods ) && front.cvars.CompFlags.zc_switch_used ) {
                sym->u.var.segid = SEG_CODE;
                return;
           }
           if( (sym->attribs.stg_class == SC_STATIC) && (sym->flags & SYM_TEMP) ) {
                sym->u.var.segid = SEG_CODE;
                return;
            }
         }
     }
  #endif
    if( sym->mods & (FLAG_FAR | FLAG_HUGE) ) {
        size = SizeOfArg( front, sym->sym_type );
        seg = NULL;
#if _CPU == 8086
        if( size < 0x10000 ) {
            for( seg = front.cvars.SegListHead; seg != NULL; seg = seg->next_segment ) {
                if( seg->size_left >= size ) {
                    break;
                }
            }
        }
#else
        seg = front.cvars.SegListHead;
#endif
        if( seg == NULL ) {
            if( front.cvars.SegListHead == NULL ) {
                front.cvars.SegListHead = static_cast<segment_list *>(CMemAlloc( front, sizeof( segment_list ) ));
                seg = front.cvars.SegListHead;
            } else {
                for( seg = front.cvars.SegListHead; seg->next_segment; ) {
                    seg = seg->next_segment;
                }
                seg->next_segment = static_cast<struct segment_list *>(CMemAlloc( front, sizeof( segment_list ) ));
                seg = seg->next_segment;
            }
            seg->segid = front.cvars.SegmentNum++;
#if _CPU == 8086
            if( size > 0xFFFF ) {
                while( size > 0x0FFFF ) {       /* while >= 64K */
                    ++front.cvars.SegmentNum;
                    size -= 0x10000;
                }
                seg->size_left = 0;
            } else {
                seg->size_left = 0x10000 - size;
            }
#endif
        } else {
            seg->size_left -= size;
        }
        sym->u.var.segid = seg->segid;
    } else {
        sym->u.var.segid = SymSegId( front, sym );
    }
}

static user_seg *AllocUserSeg( FrontRun& front, const char *segname, const char *class_name, seg_type segtype )
{
    user_seg    *useg;

    useg = static_cast<user_seg *>(CMemAlloc( front, sizeof( user_seg ) ));
    useg->next = NULL;
    useg->name = CMemStrdup( front, segname );
    useg->class_name = NULL;
    useg->segtype = segtype;
    if( class_name != NULL ) {
        useg->class_name = CMemStrdup( front, class_name );
    }
    useg->segid = front.cinfo->userSegId++;
    return( useg );
}

static const spc_info *InitFiniLookup( const char *name )
{
    int     i;
    int     j;

    for( j = 0; j < ArraySize( InitFiniSegs ); j++ ) {
        for( i = 0; i < ArraySize( InitFiniSegs[0] ); i++ ) {
            if( strcmp( InitFiniSegs[j][i].name, name ) == 0 ) {
                return( &InitFiniSegs[j][0] );
            }
        }
    }
    return( NULL );
}


static segment_id AddSeg( FrontRun& front, const char *segname, const char *class_name, int segtype )
{
    int             i;
    user_seg        *useg;
    user_seg        **lnk;
    hw_reg_set      reg;
    const char      *p;

    for( i = 0; i < ArraySize( front.cinfo->Predefined_Segs ); i++ ) {
        if( strcmp( segname, front.cinfo->Predefined_Segs[i].name ) == 0 ) {
            return( front.cinfo->Predefined_Segs[i].segid );
        }
    }
    HW_CAsgn( reg, HW_EMPTY );
    for( p = segname; *p != '\0'; ++p ) {
        if( *p == ':' ) {
            char        buffer[REG_BUFF_SIZE];
            const char  *p1;
            size_t      len;

            p1 = SkipUnderscorePrefix( front, segname );
            if( p1 == NULL ) {
                /*
                 * error missing underscore prefix
                 */
                PragRegNameErr( front, segname );
                p1 = segname;
            }
            len = 0;
            while( p1 != p && len < ( sizeof( buffer ) - 1 ) ) {
                buffer[len++] = *p1++;
            }
            buffer[len] = '\0';
            reg = PragRegName( front, buffer );
            segname = p + 1;
            break;
        }
    }
    for( lnk = &front.cinfo->userSegments; (useg = *lnk) != NULL; lnk = &useg->next ) {
        if( strcmp( segname, useg->name ) == 0 ) {
            return( useg->segid ); /* was return( segment ) */
        }
    }
    useg = AllocUserSeg( front, segname, class_name, static_cast<seg_type>(segtype) );
    useg->next = *lnk;
    useg->pegged_register = reg;
    *lnk = useg;
    return( useg->segid );
}


segment_id AddSegName( FrontRun& front, const char *segname, const char *class_name, int segtype )
{
    const spc_info  *initfini;
    segment_id      segid;
    int             i;

    initfini = InitFiniLookup( segname );
    if( initfini != NULL ) {
        for( i = 0; i < ArraySize( InitFiniSegs[0] ); i++ ) {
            AddSeg( front, initfini[i].name, initfini[i].class_name, initfini[i].segtype );
        }
    }
    segid = AddSeg( front, segname, class_name, segtype );
    return( segid );
}


segment_id DefThreadSeg( FrontRun& front )
{
    segment_id  segid;

    segid = AddSegName( front, TS_SEG_TLS, "DATA", SEGTYPE_INITFINI );
    return( segid );
}


void SetFuncSegment( FrontRun& front, SYMPTR sym, segment_id segid )
{
    user_seg    *useg;

    for( useg = front.cinfo->userSegments; useg != NULL; useg = useg->next ) {
        if( useg->segid == segid ) {
            sym->seginfo = LkSegName( front, useg->name, "CODE" );
            break;
        }
    }
}


char *SegClassName( FrontRun& front, segment_id segid )
{
    user_seg        *useg;
    textsegment     *tseg;
    char            *classname;
    size_t          len;

    if( segid == SEG_CODE ) {
        return( front.cvars.CodeClassName );
    }
    for( useg = front.cinfo->userSegments; useg != NULL; useg = useg->next ) {
        if( useg->segid == segid && useg->class_name != NULL ) {
            classname = useg->class_name;
            if( classname[0] == '\0' ) {
                if( useg->segtype == SEGTYPE_BASED ) {
                    classname = const_cast<char *>("FAR_DATA");
                } else {
                    len = strlen( useg->name );
                    if( len >= 4 ) {
                        if( stricmp( useg->name + len - 4, "DATA" ) == 0 ) {
                            classname = const_cast<char *>("FAR_DATA");
                        } else if( stricmp( useg->name + len - 4, "TEXT" ) == 0 ) {
                            classname = const_cast<char *>("CODE");
                        }
                    }
                }
            }
            return( classname );
        }
    }
    for( tseg = front.cvars.TextSegList; tseg != NULL; tseg = tseg->next ) {
        if( tseg->segid == segid )  {
            /*
             * class name appears after the segment name
             */
            classname = tseg->segname + tseg->class_;
            /*
             * if class name not specified, then use the default
             */
            if( *classname == '\0' )
                classname = front.cvars.CodeClassName;
            return( classname );
        }
    }
    if( front.cvars.DataSegName != NULL && *front.cvars.DataSegName != '\0' ) {
        return const_cast<char *>(( "FAR_DATA" ));
    }
    return( NULL );
}


void SetSegSymHandle( FrontRun& front, SYM_HANDLE sym_handle, segment_id segid )
{
    int     i;

    for( i = 0; i < ArraySize( front.cinfo->Predefined_Segs ); i++ ) {
        if( front.cinfo->Predefined_Segs[i].segid == segid ) {
            front.cinfo->Predefined_Segs[i].sym_handle = sym_handle;
            break;
        }
    }
}


SYM_HANDLE SegSymHandle( FrontRun& front, segment_id segid )
{
    int             i;
    user_seg        *useg;

    for( i = 0; i < ArraySize( front.cinfo->Predefined_Segs ); i++ ) {
        if( front.cinfo->Predefined_Segs[i].segid == segid ) {
            return( front.cinfo->Predefined_Segs[i].sym_handle );
        }
    }
    for( useg = front.cinfo->userSegments; useg != NULL; useg = useg->next ) {
        if( useg->segid == segid ) {
            if( useg->sym_handle == SYM_NULL ) {
                useg->sym_handle = SegSymbol( front, useg->name, SEG_NULL );
            }
            return( useg->sym_handle );
        }
    }
    return( SYM_NULL );
}

hw_reg_set *SegPeggedReg( FrontRun& front, segment_id segid )
{
    user_seg    *useg;

    for( useg = front.cinfo->userSegments; useg != NULL; useg = useg->next ) {
        if( useg->segid == segid ) {
            return( &useg->pegged_register );
        }
    }
    return( NULL );
}

static align_type SegAlign( FrontRun& front, align_type suggested )
/************************************************/
{
    align_type     align;

    align = suggested;
    if( front.cvars.CompFlags.unaligned_segs ) {
        align = 1;
    }
    return( align );
}

void    SetSegs( FrontRun& front )
/*********************/
{
    segment_id      segid;
    user_seg        *useg;
    textsegment     *tseg;
    char            *name;
    const char      *module_name;
    align_type      optsize_segalign;

    front.cvars.CompFlags.low_on_memory_printed = false;
    optsize_segalign = ( front.cvars.OptSize == 0 ) ? (align_type)BETypeLength( *front.back, TY_INTEGER ) : 1;

    if( *front.cvars.TextSegName == '\0' ) {
        BEDefSeg( *front.back, SEG_CODE, GLOBAL | INIT | EXEC, TS_SEG_CODE, SegAlign( front, optsize_segalign ) );
    } else {
        BEDefSeg( *front.back, SEG_CODE, GLOBAL | INIT | EXEC | GIVEN_NAME, front.cvars.TextSegName, SegAlign( front, optsize_segalign ) );
    }
    BEDefSeg( *front.back, SEG_CONST, BACK|INIT|ROM, TS_SEG_CONST, SegAlign( front, front.cvars.SegAlignment[SEG_CONST] ) );
    BEDefSeg( *front.back, SEG_CONST2, INIT | ROM, TS_SEG_CONST2, SegAlign( front, front.cvars.SegAlignment[SEG_CONST2] ) );
    BEDefSeg( *front.back, SEG_DATA,  GLOBAL | INIT, TS_SEG_DATA, SegAlign( front, front.cvars.SegAlignment[SEG_DATA] ) );
    if( front.cvars.CompFlags.ec_switch_used ) {
        BEDefSeg( *front.back, SEG_YIB,      GLOBAL | INIT,  TS_SEG_YIB, 2 );
        BEDefSeg( *front.back, SEG_YI,       GLOBAL | INIT,  TS_SEG_YI,  2 );
        BEDefSeg( *front.back, SEG_YIE,      GLOBAL | INIT,  TS_SEG_YIE, 2 );
    }
    if( front.cvars.CompFlags.bss_segment_used ) {
        BEDefSeg( *front.back, SEG_BSS, GLOBAL, TS_SEG_BSS, SegAlign( front, front.cvars.SegAlignment[SEG_BSS] ) );
    }
    if( front.cvars.CompFlags.far_strings ) {
        front.cvars.FarStringSegId = front.cvars.SegmentNum++;
    }
    module_name = FEModuleName(front);
    name = static_cast<char *>(CMemAlloc( front, strlen( module_name ) + 10 + sizeof( "_DATA" ) ));
    for( segid = FIRST_PRIVATE_SEGMENT; segid < front.cvars.SegmentNum; ++segid ) {
        sprintf( name, "%s%d_DATA", module_name, segid );
        BEDefSeg( *front.back, segid, INIT | PRIVATE, name, SegAlign( front, 16 ) );
    }
    CMemFree( front, name );
    for( useg = front.cinfo->userSegments; useg != NULL ; useg = useg->next ) {
        segid = useg->segid;
        switch( useg->segtype ) {
        case SEGTYPE_DATA:  /* created through #pragma data_seg */
            BEDefSeg( *front.back, segid, INIT | GLOBAL, useg->name, SegAlign( front, TARGET_INT ) );
            break;
        case SEGTYPE_BASED:
            BEDefSeg( *front.back, segid, INIT | PRIVATE | GLOBAL, useg->name, SegAlign( front, TARGET_INT ) );
            break;
        case SEGTYPE_INITFINI:
            BEDefSeg( *front.back, segid, INIT | GLOBAL, useg->name, SegAlign( front, 1 ) );
            break;
        case SEGTYPE_INITFINITR:
            BEDefSeg( *front.back, segid, INIT | GLOBAL| THREAD_LOCAL, useg->name, SegAlign( front, 1 ) );
            break;
        }
    }
    for( tseg = front.cvars.TextSegList; tseg != NULL; tseg = tseg->next ) {
        tseg->segid = ++front.cvars.SegmentNum;
        BEDefSeg( *front.back, tseg->segid,  GLOBAL | INIT | EXEC | GIVEN_NAME, tseg->segname, SegAlign( front, optsize_segalign ) );
    }
}

void EmitSegLabels( FrontRun& front )
{
    segment_id          segid;
    user_seg            *useg;
    BACK_HANDLE         bck;
    SYM_ENTRY           sym;

    segid = FIRST_USER_SEGMENT;
    for( useg = front.cinfo->userSegments; useg != NULL; useg = useg->next ) {
        if( useg->sym_handle != SYM_NULL ) {
            SymGet( front, &sym, useg->sym_handle );
            bck = BENewBack( *front.back, (CGSYM_HANDLE)useg->sym_handle );
            sym.u1.backinfo = bck;
            SymReplace( front, &sym, useg->sym_handle );
            BESetSeg( *front.back, segid );
            DGLabel( *front.back, bck );
        }
        ++segid;
    }
}


void FiniSegLabels( FrontRun& front )
{
    user_seg        *useg;
    SYM_ENTRY       sym;

    for( useg = front.cinfo->userSegments; useg != NULL; useg = useg->next ) {
        if( useg->sym_handle != SYM_NULL ) {
            SymGet( front, &sym, useg->sym_handle );
            BEFiniBack( *front.back, sym.u1.backinfo );
        }
    }
}


void FiniSegBacks( FrontRun& front )
{
    user_seg        *useg;
    SYM_ENTRY       sym;

    for( useg = front.cinfo->userSegments; useg != NULL; useg = useg->next ) {
        if( useg->sym_handle != SYM_NULL ) {
            SymGet( front, &sym, useg->sym_handle );
            BEFreeBack( *front.back, sym.u1.backinfo );
        }
    }
}


const char *FEName( FrontRun& front, CGSYM_HANDLE cgsym_handle )
/********** return unmangled names ***********/
{
    SYM_HANDLE  sym_handle;
    SYMPTR      sym;

    sym_handle = (SYM_HANDLE)cgsym_handle;
    if( sym_handle == SYM_NULL )
        return( "*** NULL ***" );
    sym = SymGetPtr( front, sym_handle );
    return( sym->name );
}


void FEMessage( FrontRun& front, fe_msg femsg, CGPOINTER parm )
/********************************************/
{
    char    msgtxt[80];
    char    msgbuf[MAX_MSG_LEN];

    switch( femsg ) {
    case FEMSG_SYMBOL_TOO_LONG:
        {
            SYM_ENTRY   *sym;

            sym = SymGetPtr( front, (SYM_HANDLE)parm );
            SetErrLoc( front, &sym->src_loc );
            CWarn2p( front, ERR_SYMBOL_NAME_TOO_LONG, sym->name );
            InitErrLoc(front);
        }
        break;
    case FEMSG_BLIP:
        if( front.cvars.GlobalCompFlags.ide_console_output ) {
            if( !front.cvars.CompFlags.quiet_mode ) {
                ConBlip(front);
            }
        }
        break;
    case FEMSG_INFO:
    case FEMSG_INFO_FILE:
    case FEMSG_INFO_PROC:
        if( front.cvars.GlobalCompFlags.ide_console_output ) {
            if( !front.cvars.CompFlags.quiet_mode ) {
                NoteMsg( front, static_cast<const char *>(parm) );
            }
        }
        break;
    case FEMSG_CODE_SIZE:
        if( front.cvars.GlobalCompFlags.ide_console_output ) {
            if( !front.cvars.CompFlags.quiet_mode ) {
                CGetMsg( front, msgtxt, PHRASE_CODE_SIZE );
                sprintf( msgbuf, "%s: %u", msgtxt, (unsigned)(pointer_uint)parm );
                NoteMsg( front, msgbuf );
            }
        }
        break;
    case FEMSG_DATA_SIZE:
        break;
    case FEMSG_ERROR:
        CErr2p( front, ERR_INTERNAL_ERROR_MSG, static_cast<const char *>(parm) );
        break;
    case FEMSG_FATAL:
        CErr2p( front, ERR_FATAL_ERROR, static_cast<const char *>(parm) );
        CloseFiles(front);       /* get rid of temp file */
        MyExit( front, 1 );        /* exit to DOS do not pass GO */
        break;
    case FEMSG_BAD_PARM_REGISTER:
        /*
         * this will be issued after a call to CGInitCall or CGProcDecl
         */
        CErr2( front, ERR_BAD_PARM_REGISTER, (int)(pointer_uint)parm );
        break;
    case FEMSG_BAD_RETURN_REGISTER:
        CErr2p( front, ERR_BAD_RETURN_REGISTER, FEName( front, (CGSYM_HANDLE)parm ) );
        break;
    case FEMSG_SCHEDULER_DIED:
    case FEMSG_REGALLOC_DIED:
    case FEMSG_SCOREBOARD_DIED:
        if( (front.cvars.GenSwitches & CGSW_GEN_NO_OPTIMIZATION) == 0 ) {
            if( front.cvars.LastFuncOutOfMem != parm ) {
                CInfoMsg( front, INFO_NOT_ENOUGH_MEMORY_TO_FULLY_OPTIMIZE, FEName( front, (CGSYM_HANDLE)parm ) );
                front.cvars.LastFuncOutOfMem = parm;
            }
        }
        break;
    case FEMSG_PEEPHOLE_FLUSHED:
        if( (front.cvars.GenSwitches & CGSW_GEN_NO_OPTIMIZATION) == 0 ) {
            if( !front.cvars.CompFlags.low_on_memory_printed ) {
                CInfoMsg( front, INFO_NOT_ENOUGH_MEMORY_TO_MAINTAIN_PEEPHOLE);
                front.cvars.CompFlags.low_on_memory_printed = true;
            }
        }
        break;
    case FEMSG_BACK_END_ERROR:
        CErr2( front, ERR_BACK_END_ERROR, (int)(pointer_uint)parm );
        break;
    case FEMSG_BAD_SAVE:
        CErr2p( front, ERR_BAD_SAVE, FEName( front, (CGSYM_HANDLE)parm ) );
        break;
    case FEMSG_BAD_LINKAGE:
        CErr2p( front, ERR_BAD_LINKAGE, FEName( front, (CGSYM_HANDLE)parm ) );
        break;
    case FEMSG_NO_SEG_REGS:
        CErr2p( front, ERR_NO_SEG_REGS, FEName( front, (CGSYM_HANDLE)parm ) );
        break;
    case FEMSG_BAD_PEG_REG:
        CErr2p( front, ERR_BAD_PEG_REG, FEName( front, (CGSYM_HANDLE)parm ) );
        break;
    default:
        break;
    }
}


const char *FEModuleName( FrontRun& front )
/******************************/
{
    if( front.cvars.ModuleName != NULL && front.cvars.ModuleName[0] != '\0' )
        return( front.cvars.ModuleName );
    return( front.cvars.SrcFName );
}


int FETrue( void )
/****************/
{
    return( 1 );
}


segment_id FESegID( FrontRun& front, CGSYM_HANDLE cgsym_handle )
/*********************************************/
{
    SYM_HANDLE  sym_handle = (SYM_HANDLE)cgsym_handle;
    segment_id  segid;
    SYMPTR      sym;

    sym = SymGetPtr( front, sym_handle );
    {
        fe_attr     attr;

        attr = FESymAttr( front, sym );
        if( attr & FE_PROC ) {
            /*
             * in large code models, should return different segment #
             * for every imported routine.
             */
            segid = SEG_CODE;
            if( sym->seginfo != NULL ) {
                segid = sym->seginfo->segid;
            } else if( attr & FE_IMPORT ) {
                if( (sym->mods & FLAG_FAR) || (front.cvars.TargetSwitches & CGSW_X86_BIG_CODE) ) {
                    if( sym->flags & SYM_ADDR_TAKEN ) {
                        segid = front.cinfo->import_segid--;
                    }
                }
            }
        } else if( sym->u.var.segid != SEG_NULL ) {
            segid = sym->u.var.segid;
        } else if( attr & FE_GLOBAL ) {
            segid = SEG_DATA;
        } else {
            segid = SEG_CONST;
        }
    }
    return( segid );
}


BACK_HANDLE FEBack( FrontRun& front, CGSYM_HANDLE cgsym_handle )
/*********************************************/
{
    SYM_HANDLE          sym_handle = (SYM_HANDLE)cgsym_handle;
    BACK_HANDLE         bck;
    SYMPTR              symptr;
    SYM_ENTRY           sym;

    symptr = SymGetPtr( front, sym_handle );
    bck = symptr->u1.backinfo;
    if( bck == NULL ) {
        bck = BENewBack( *front.back, (CGSYM_HANDLE)sym_handle );
        SymGet( front, &sym, sym_handle );
        sym.u1.backinfo = bck;
        SymReplace( front, &sym, sym_handle );
    }
    return( bck );
}


int FELexLevel( CGSYM_HANDLE cgsym_handle )
/*****************************************/
{
    /* unused parameters */ (void)cgsym_handle;

    return( 0 );
}


cg_type FEParmType( FrontRun& front, CGSYM_HANDLE func, CGSYM_HANDLE parm, cg_type tipe )
/**********************************************************************/
{
#if _CPU == 386
    SYM_HANDLE  sym_handle = (SYM_HANDLE)func;
    SYMPTR      sym;
#else
    /* unused parameters */ (void)func;
#endif
    /* unused parameters */ (void)parm;

    switch( tipe ) {
#if _CPU == 386 || _RISC_CPU
    case TY_UINT_2:
    case TY_INT_2:
#endif
    case TY_INT_1:
    case TY_UINT_1:
#if _CPU == 386
        if( sym_handle != SYM_NULL ) {
            sym = SymGetPtr( front, sym_handle );
            if( sym->mods & FLAG_FAR16 ) {
                return( TY_INT_2 );
            }
        }
#endif
        tipe = TY_INTEGER;
    }
    return( tipe );
}


int FEStackChk( FrontRun& front, CGSYM_HANDLE cgsym_handle )
/*****************************************/
{
    SYM_HANDLE  sym_handle = (SYM_HANDLE)cgsym_handle;
    SYMPTR      sym;

    sym = SymGetPtr( front, sym_handle );
    return( (sym->flags & SYM_CHECK_STACK) != 0 );
}

void SegInit( FrontRun& front )
{
    segment_id  segid;

    front.cinfo->userSegments = NULL;
    front.cinfo->userSegId = FIRST_USER_SEGMENT;
    for( segid = 0; segid < FIRST_PRIVATE_SEGMENT; segid++ ) {
        front.cvars.SegAlignment[segid] = TARGET_INT;
    }
}

void SegFini( FrontRun& front )
{
    user_seg    *useg;

    while( (useg = front.cinfo->userSegments) != NULL ) {
        front.cinfo->userSegments = useg->next;
        CMemFree( front, useg );
    }
}

void SetSegAlign( FrontRun& front, SYMPTR sym )
{
    align_type          align;
    segment_id          segid;

    segid = sym->u.var.segid;
    if( segid < FIRST_PRIVATE_SEGMENT && front.cvars.OptSize == 0 ) {
        align = GetTypeAlignment( front, sym->sym_type );
        if( front.cvars.SegAlignment[segid] < align ) {
            front.cvars.SegAlignment[segid] = align;
        }
    }
}

void ImportNearSegIdInit( FrontRun& front )
{
    front.cinfo->import_near_segid = -1;
}

void ImportSegIdInit( FrontRun& front )
{
    if( front.cinfo->import_near_segid != SEG_NULL ) {
        front.cinfo->import_segid = front.cinfo->import_near_segid - 1;
    } else {
        front.cinfo->import_segid = -1;
    }
}

/* the run makes and frees its copy of this file's variables (run/ files: see the run header) */
CinfoState* cinfo_make() { return new CinfoState(); }
void cinfo_free(CinfoState* part) { delete part; }
