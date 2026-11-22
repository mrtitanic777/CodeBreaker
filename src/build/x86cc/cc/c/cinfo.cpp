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


#define FIRST_USER_SEGMENT      10000

#define CONSTANT( decl_flags )  ((decl_flags & MASK_CV_QUALIFIERS) == FLAG_CONST)

typedef struct user_seg {
    struct user_seg *next;
    char            *name;
    SYM_HANDLE      sym_handle;
    int             segtype;
    segment_id      segid;
    char            *class_name;
#if _INTEL_CPU
    hw_reg_set      pegged_register;
#endif
    bool_            used;
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

}  // namespace

static inline CinfoState& cinfo_state() { return cbrun_part<CinfoState>(); }


void AssignSeg( SYMPTR sym )
{
    SetFarHuge( sym, true );
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
                SetSegment( sym );
            }
            if( sym->u.var.segid == SEG_DATA ) {
                sym->u.var.segid = SEG_BSS;
                cvars_state().CompFlags.bss_segment_used = true;
            }
            SetSegAlign( sym );
        }
    } else if( sym->mods & (FLAG_FAR | FLAG_HUGE) ) {
        sym->u.var.segid = cinfo_state().import_segid--;
    } else if( (cinfo_state().import_near_segid != SEG_NULL) && (sym->mods & FLAG_NEAR) ) {  // imported and near
        sym->u.var.segid = cinfo_state().import_near_segid;
    }
}


void SetFarHuge( SYMPTR sym, bool_ report )
{
#if _INTEL_CPU
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
    size = SizeOfArg( sym->sym_type );
    if( cvars_state().TargetSwitches & CGSW_X86_BIG_DATA ) {
        attrib = sym->mods;
        if( (attrib & MASK_ALL_MEM_MODELS) == 0 ) {
            if( size == 0 ) {   /* unspecified array size */
                if( sym->attribs.stg_class == SC_EXTERN ) {
                    typ = sym->sym_type;
                    if( typ->decl_type == TYP_ARRAY ) {
                        attrib |= FLAG_FAR;
                    }
                }
            } else if( size > cvars_state().DataThreshold ) {
                attrib |= FLAG_FAR;
            } else if( cvars_state().CompFlags.strings_in_code_segment && (sym->mods & FLAG_CONST) ) {
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
        SetErrLoc( &sym->src_loc );
        CErr1( ERR_VAR_TOO_LARGE );
        InitErrLoc();
   }
  #endif
#else /* _RISC_CPU */

    /* unused parameters */ (void)sym; (void)report;

#endif
}

static fe_attr FESymAttr( SYMPTR sym )
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
        if( VarFunc( sym ) )
            attr |= FE_VARARGS;
        if( cvars_state().CompFlags.unique_functions ) {
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
        if( (attr & FE_IMPORT) || cvars_state().CompFlags.rent ) {
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


void    FEGenProc( CGSYM_HANDLE hdl, call_handle call )
/*****************************************************/
{
    SYM_HANDLE      sym_handle = (SYM_HANDLE)hdl;

    /* unused parameters */ (void)call;

    GenInLineFunc( sym_handle );
}


fe_attr FEAttr( CGSYM_HANDLE cgsym_handle )
/*****************************************/
{
    SYM_HANDLE sym_handle = (SYM_HANDLE)cgsym_handle;

    return( FESymAttr( SymGetPtr( sym_handle ) ) );
}

segment_id SymSegId( SYMPTR sym )
{
    fe_attr     attr;

    attr = FESymAttr( sym );
    if( attr & FE_PROC )
        return( SEG_CODE );
    if( !cvars_state().CompFlags.rent ) {
        if( attr & FE_CONSTANT ) {
            return( SEG_CONST2 );
        }
    }
    return( SEG_DATA );
}


void SetSegment( SYMPTR sym )
{
    segment_list        *seg;
    target_size         size;

#if _INTEL_CPU
  #if _CPU == 8086
    if( (sym->mods & FLAG_FAR) && cvars_state().CompFlags.zc_switch_used ) {
        if( CONSTANT( sym->mods )
          || (sym->attribs.stg_class == SC_STATIC && (sym->flags & SYM_TEMP)) ) {
            sym->u.var.segid = SEG_CODE;
            return;
        }
    }
  #else /* _CPU == 386 */
    if( !cvars_state().CompFlags.rent ) {
        if( (sym->mods & FLAG_FAR) || (cvars_state().TargetSwitches & CGSW_X86_FLAT_MODEL) ) {
           if( CONSTANT( sym->mods ) && cvars_state().CompFlags.zc_switch_used ) {
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
#endif
    if( sym->mods & (FLAG_FAR | FLAG_HUGE) ) {
        size = SizeOfArg( sym->sym_type );
        seg = NULL;
#if _CPU == 8086
        if( size < 0x10000 ) {
            for( seg = cvars_state().SegListHead; seg != NULL; seg = seg->next_segment ) {
                if( seg->size_left >= size ) {
                    break;
                }
            }
        }
#else
        seg = cvars_state().SegListHead;
#endif
        if( seg == NULL ) {
            if( cvars_state().SegListHead == NULL ) {
                cvars_state().SegListHead = static_cast<segment_list *>(CMemAlloc( sizeof( segment_list ) ));
                seg = cvars_state().SegListHead;
            } else {
                for( seg = cvars_state().SegListHead; seg->next_segment; ) {
                    seg = seg->next_segment;
                }
                seg->next_segment = static_cast<struct segment_list *>(CMemAlloc( sizeof( segment_list ) ));
                seg = seg->next_segment;
            }
            seg->segid = cvars_state().SegmentNum++;
#if _CPU == 8086
            if( size > 0xFFFF ) {
                while( size > 0x0FFFF ) {       /* while >= 64K */
                    ++cvars_state().SegmentNum;
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
        sym->u.var.segid = SymSegId( sym );
    }
}

static user_seg *AllocUserSeg( const char *segname, const char *class_name, seg_type segtype )
{
    user_seg    *useg;

    useg = static_cast<user_seg *>(CMemAlloc( sizeof( user_seg ) ));
    useg->next = NULL;
    useg->name = CMemStrdup( segname );
    useg->class_name = NULL;
    useg->segtype = segtype;
    if( class_name != NULL ) {
        useg->class_name = CMemStrdup( class_name );
    }
    useg->segid = cinfo_state().userSegId++;
    return( useg );
}

static const spc_info *InitFiniLookup( const char *name )
{
    int     i;
    int     j;

    for( j = 0; j < ARRAY_SIZE( InitFiniSegs ); j++ ) {
        for( i = 0; i < ARRAY_SIZE( InitFiniSegs[0] ); i++ ) {
            if( strcmp( InitFiniSegs[j][i].name, name ) == 0 ) {
                return( &InitFiniSegs[j][0] );
            }
        }
    }
    return( NULL );
}


static segment_id AddSeg( const char *segname, const char *class_name, int segtype )
{
    int             i;
    user_seg        *useg;
    user_seg        **lnk;
#if _INTEL_CPU
    hw_reg_set      reg;
    const char      *p;
#endif

    for( i = 0; i < ARRAY_SIZE( cinfo_state().Predefined_Segs ); i++ ) {
        if( strcmp( segname, cinfo_state().Predefined_Segs[i].name ) == 0 ) {
            return( cinfo_state().Predefined_Segs[i].segid );
        }
    }
#if _INTEL_CPU
    HW_CAsgn( reg, HW_EMPTY );
    for( p = segname; *p != '\0'; ++p ) {
        if( *p == ':' ) {
            char        buffer[REG_BUFF_SIZE];
            const char  *p1;
            size_t      len;

            p1 = SkipUnderscorePrefix( segname );
            if( p1 == NULL ) {
                /*
                 * error missing underscore prefix
                 */
                PragRegNameErr( segname );
                p1 = segname;
            }
            len = 0;
            while( p1 != p && len < ( sizeof( buffer ) - 1 ) ) {
                buffer[len++] = *p1++;
            }
            buffer[len] = '\0';
            reg = PragRegName( buffer );
            segname = p + 1;
            break;
        }
    }
#endif
    for( lnk = &cinfo_state().userSegments; (useg = *lnk) != NULL; lnk = &useg->next ) {
        if( strcmp( segname, useg->name ) == 0 ) {
            return( useg->segid ); /* was return( segment ) */
        }
    }
    useg = AllocUserSeg( segname, class_name, static_cast<seg_type>(segtype) );
    useg->next = *lnk;
#if _INTEL_CPU
    useg->pegged_register = reg;
#endif
    *lnk = useg;
    return( useg->segid );
}


segment_id AddSegName( const char *segname, const char *class_name, int segtype )
{
    const spc_info  *initfini;
    segment_id      segid;
    int             i;

    initfini = InitFiniLookup( segname );
    if( initfini != NULL ) {
        for( i = 0; i < ARRAY_SIZE( InitFiniSegs[0] ); i++ ) {
            AddSeg( initfini[i].name, initfini[i].class_name, initfini[i].segtype );
        }
    }
    segid = AddSeg( segname, class_name, segtype );
    return( segid );
}


segment_id DefThreadSeg( void )
{
    segment_id  segid;

    segid = AddSegName( TS_SEG_TLS, "DATA", SEGTYPE_INITFINI );
    return( segid );
}


void SetFuncSegment( SYMPTR sym, segment_id segid )
{
    user_seg    *useg;

    for( useg = cinfo_state().userSegments; useg != NULL; useg = useg->next ) {
        if( useg->segid == segid ) {
            sym->seginfo = LkSegName( useg->name, "CODE" );
            break;
        }
    }
}


char *SegClassName( segment_id segid )
{
    user_seg        *useg;
    textsegment     *tseg;
    char            *classname;
    size_t          len;

    if( segid == SEG_CODE ) {
        return( cvars_state().CodeClassName );
    }
    for( useg = cinfo_state().userSegments; useg != NULL; useg = useg->next ) {
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
    for( tseg = cvars_state().TextSegList; tseg != NULL; tseg = tseg->next ) {
        if( tseg->segid == segid )  {
            /*
             * class name appears after the segment name
             */
            classname = tseg->segname + tseg->class_;
            /*
             * if class name not specified, then use the default
             */
            if( *classname == '\0' )
                classname = cvars_state().CodeClassName;
            return( classname );
        }
    }
    if( cvars_state().DataSegName != NULL && *cvars_state().DataSegName != '\0' ) {
        return const_cast<char *>(( "FAR_DATA" ));
    }
    return( NULL );
}


void SetSegSymHandle( SYM_HANDLE sym_handle, segment_id segid )
{
    int     i;

    for( i = 0; i < ARRAY_SIZE( cinfo_state().Predefined_Segs ); i++ ) {
        if( cinfo_state().Predefined_Segs[i].segid == segid ) {
            cinfo_state().Predefined_Segs[i].sym_handle = sym_handle;
            break;
        }
    }
}


SYM_HANDLE SegSymHandle( segment_id segid )
{
    int             i;
    user_seg        *useg;

    for( i = 0; i < ARRAY_SIZE( cinfo_state().Predefined_Segs ); i++ ) {
        if( cinfo_state().Predefined_Segs[i].segid == segid ) {
            return( cinfo_state().Predefined_Segs[i].sym_handle );
        }
    }
    for( useg = cinfo_state().userSegments; useg != NULL; useg = useg->next ) {
        if( useg->segid == segid ) {
            if( useg->sym_handle == SYM_NULL ) {
                useg->sym_handle = SegSymbol( useg->name, SEG_NULL );
            }
            return( useg->sym_handle );
        }
    }
    return( SYM_NULL );
}

#if _INTEL_CPU
hw_reg_set *SegPeggedReg( segment_id segid )
{
    user_seg    *useg;

    for( useg = cinfo_state().userSegments; useg != NULL; useg = useg->next ) {
        if( useg->segid == segid ) {
            return( &useg->pegged_register );
        }
    }
    return( NULL );
}
#endif

static align_type SegAlign( align_type suggested )
/************************************************/
{
    align_type     align;

    align = suggested;
    if( cvars_state().CompFlags.unaligned_segs ) {
        align = 1;
    }
    return( align );
}

void    SetSegs( void )
/*********************/
{
    segment_id      segid;
    user_seg        *useg;
    textsegment     *tseg;
    char            *name;
    const char      *module_name;
    align_type      optsize_segalign;

    cvars_state().CompFlags.low_on_memory_printed = false;
    optsize_segalign = ( cvars_state().OptSize == 0 ) ? (align_type)BETypeLength( TY_INTEGER ) : 1;

    if( *cvars_state().TextSegName == '\0' ) {
        BEDefSeg( SEG_CODE, GLOBAL | INIT | EXEC, TS_SEG_CODE, SegAlign( optsize_segalign ) );
    } else {
        BEDefSeg( SEG_CODE, GLOBAL | INIT | EXEC | GIVEN_NAME, cvars_state().TextSegName, SegAlign( optsize_segalign ) );
    }
    BEDefSeg( SEG_CONST, BACK|INIT|ROM, TS_SEG_CONST, SegAlign( cvars_state().SegAlignment[SEG_CONST] ) );
    BEDefSeg( SEG_CONST2, INIT | ROM, TS_SEG_CONST2, SegAlign( cvars_state().SegAlignment[SEG_CONST2] ) );
    BEDefSeg( SEG_DATA,  GLOBAL | INIT, TS_SEG_DATA, SegAlign( cvars_state().SegAlignment[SEG_DATA] ) );
    if( cvars_state().CompFlags.ec_switch_used ) {
        BEDefSeg( SEG_YIB,      GLOBAL | INIT,  TS_SEG_YIB, 2 );
        BEDefSeg( SEG_YI,       GLOBAL | INIT,  TS_SEG_YI,  2 );
        BEDefSeg( SEG_YIE,      GLOBAL | INIT,  TS_SEG_YIE, 2 );
    }
    if( cvars_state().CompFlags.bss_segment_used ) {
        BEDefSeg( SEG_BSS, GLOBAL, TS_SEG_BSS, SegAlign( cvars_state().SegAlignment[SEG_BSS] ) );
    }
    if( cvars_state().CompFlags.far_strings ) {
        cvars_state().FarStringSegId = cvars_state().SegmentNum++;
    }
    module_name = FEModuleName();
    name = static_cast<char *>(CMemAlloc( strlen( module_name ) + 10 + sizeof( "_DATA" ) ));
    for( segid = FIRST_PRIVATE_SEGMENT; segid < cvars_state().SegmentNum; ++segid ) {
        sprintf( name, "%s%d_DATA", module_name, segid );
        BEDefSeg( segid, INIT | PRIVATE, name, SegAlign( 16 ) );
    }
    CMemFree( name );
    for( useg = cinfo_state().userSegments; useg != NULL ; useg = useg->next ) {
        segid = useg->segid;
        switch( useg->segtype ) {
#if 0
        case SEGTYPE_CODE:
            BEDefSeg( segid, INIT | GLOBAL | EXEC, useg->name, 1 );
            break;
#endif
        case SEGTYPE_DATA:  /* created through #pragma data_seg */
            BEDefSeg( segid, INIT | GLOBAL, useg->name, SegAlign( TARGET_INT ) );
            break;
        case SEGTYPE_BASED:
            BEDefSeg( segid, INIT | PRIVATE | GLOBAL, useg->name, SegAlign( TARGET_INT ) );
            break;
        case SEGTYPE_INITFINI:
            BEDefSeg( segid, INIT | GLOBAL, useg->name, SegAlign( 1 ) );
            break;
        case SEGTYPE_INITFINITR:
            BEDefSeg( segid, INIT | GLOBAL| THREAD_LOCAL, useg->name, SegAlign( 1 ) );
            break;
        }
    }
    for( tseg = cvars_state().TextSegList; tseg != NULL; tseg = tseg->next ) {
        tseg->segid = ++cvars_state().SegmentNum;
        BEDefSeg( tseg->segid,  GLOBAL | INIT | EXEC | GIVEN_NAME, tseg->segname, SegAlign( optsize_segalign ) );
    }
}

void EmitSegLabels( void )
{
    segment_id          segid;
    user_seg            *useg;
    BACK_HANDLE         bck;
    SYM_ENTRY           sym;

    segid = FIRST_USER_SEGMENT;
    for( useg = cinfo_state().userSegments; useg != NULL; useg = useg->next ) {
        if( useg->sym_handle != SYM_NULL ) {
            SymGet( &sym, useg->sym_handle );
            bck = BENewBack( (CGSYM_HANDLE)useg->sym_handle );
            sym.u1.backinfo = bck;
            SymReplace( &sym, useg->sym_handle );
            BESetSeg( segid );
            DGLabel( bck );
        }
        ++segid;
    }
}


void FiniSegLabels( void )
{
    user_seg        *useg;
    SYM_ENTRY       sym;

    for( useg = cinfo_state().userSegments; useg != NULL; useg = useg->next ) {
        if( useg->sym_handle != SYM_NULL ) {
            SymGet( &sym, useg->sym_handle );
            BEFiniBack( sym.u1.backinfo );
        }
    }
}


void FiniSegBacks( void )
{
    user_seg        *useg;
    SYM_ENTRY       sym;

    for( useg = cinfo_state().userSegments; useg != NULL; useg = useg->next ) {
        if( useg->sym_handle != SYM_NULL ) {
            SymGet( &sym, useg->sym_handle );
            BEFreeBack( sym.u1.backinfo );
        }
    }
}


const char *FEName( CGSYM_HANDLE cgsym_handle )
/********** return unmangled names ***********/
{
    SYM_HANDLE  sym_handle;
    SYMPTR      sym;

    sym_handle = (SYM_HANDLE)cgsym_handle;
    if( sym_handle == SYM_NULL )
        return( "*** NULL ***" );
    sym = SymGetPtr( sym_handle );
    return( sym->name );
}


void FEMessage( fe_msg femsg, CGPOINTER parm )
/********************************************/
{
    char    msgtxt[80];
    char    msgbuf[MAX_MSG_LEN];

    switch( femsg ) {
    case FEMSG_SYMBOL_TOO_LONG:
        {
            SYM_ENTRY   *sym;

            sym = SymGetPtr( (SYM_HANDLE)parm );
            SetErrLoc( &sym->src_loc );
            CWarn2p( ERR_SYMBOL_NAME_TOO_LONG, sym->name );
            InitErrLoc();
        }
        break;
    case FEMSG_BLIP:
        if( cvars_state().GlobalCompFlags.ide_console_output ) {
            if( !cvars_state().CompFlags.quiet_mode ) {
                ConBlip();
            }
        }
        break;
    case FEMSG_INFO:
    case FEMSG_INFO_FILE:
    case FEMSG_INFO_PROC:
        if( cvars_state().GlobalCompFlags.ide_console_output ) {
            if( !cvars_state().CompFlags.quiet_mode ) {
                NoteMsg( static_cast<const char *>(parm) );
            }
        }
        break;
    case FEMSG_CODE_SIZE:
        if( cvars_state().GlobalCompFlags.ide_console_output ) {
            if( !cvars_state().CompFlags.quiet_mode ) {
                CGetMsg( msgtxt, PHRASE_CODE_SIZE );
                sprintf( msgbuf, "%s: %u", msgtxt, (unsigned)(pointer_uint)parm );
                NoteMsg( msgbuf );
            }
        }
        break;
    case FEMSG_DATA_SIZE:
        break;
    case FEMSG_ERROR:
        CErr2p( ERR_INTERNAL_ERROR_MSG, static_cast<const char *>(parm) );
        break;
    case FEMSG_FATAL:
        CErr2p( ERR_FATAL_ERROR, static_cast<const char *>(parm) );
        CloseFiles();       /* get rid of temp file */
        MyExit( 1 );        /* exit to DOS do not pass GO */
        break;
    case FEMSG_BAD_PARM_REGISTER:
        /*
         * this will be issued after a call to CGInitCall or CGProcDecl
         */
        CErr2( ERR_BAD_PARM_REGISTER, (int)(pointer_uint)parm );
        break;
    case FEMSG_BAD_RETURN_REGISTER:
        CErr2p( ERR_BAD_RETURN_REGISTER, FEName( (CGSYM_HANDLE)parm ) );
        break;
    case FEMSG_SCHEDULER_DIED:
    case FEMSG_REGALLOC_DIED:
    case FEMSG_SCOREBOARD_DIED:
        if( (cvars_state().GenSwitches & CGSW_GEN_NO_OPTIMIZATION) == 0 ) {
            if( cvars_state().LastFuncOutOfMem != parm ) {
                CInfoMsg( INFO_NOT_ENOUGH_MEMORY_TO_FULLY_OPTIMIZE, FEName( (CGSYM_HANDLE)parm ) );
                cvars_state().LastFuncOutOfMem = parm;
            }
        }
        break;
    case FEMSG_PEEPHOLE_FLUSHED:
        if( (cvars_state().GenSwitches & CGSW_GEN_NO_OPTIMIZATION) == 0 ) {
            if( !cvars_state().CompFlags.low_on_memory_printed ) {
                CInfoMsg( INFO_NOT_ENOUGH_MEMORY_TO_MAINTAIN_PEEPHOLE);
                cvars_state().CompFlags.low_on_memory_printed = true;
            }
        }
        break;
    case FEMSG_BACK_END_ERROR:
        CErr2( ERR_BACK_END_ERROR, (int)(pointer_uint)parm );
        break;
    case FEMSG_BAD_SAVE:
        CErr2p( ERR_BAD_SAVE, FEName( (CGSYM_HANDLE)parm ) );
        break;
    case FEMSG_BAD_LINKAGE:
        CErr2p( ERR_BAD_LINKAGE, FEName( (CGSYM_HANDLE)parm ) );
        break;
    case FEMSG_NO_SEG_REGS:
        CErr2p( ERR_NO_SEG_REGS, FEName( (CGSYM_HANDLE)parm ) );
        break;
    case FEMSG_BAD_PEG_REG:
        CErr2p( ERR_BAD_PEG_REG, FEName( (CGSYM_HANDLE)parm ) );
        break;
    default:
        break;
    }
}


const char *FEModuleName( void )
/******************************/
{
    if( cvars_state().ModuleName != NULL && cvars_state().ModuleName[0] != '\0' )
        return( cvars_state().ModuleName );
    return( cvars_state().SrcFName );
}


int FETrue( void )
/****************/
{
    return( 1 );
}


segment_id FESegID( CGSYM_HANDLE cgsym_handle )
/*********************************************/
{
    SYM_HANDLE  sym_handle = (SYM_HANDLE)cgsym_handle;
    segment_id  segid;
    SYMPTR      sym;

    sym = SymGetPtr( sym_handle );
#if _CPU == 370
    {
        segid = SymSegId( sym );
    }
#else
    {
        fe_attr     attr;

        attr = FESymAttr( sym );
        if( attr & FE_PROC ) {
            /*
             * in large code models, should return different segment #
             * for every imported routine.
             */
            segid = SEG_CODE;
            if( sym->seginfo != NULL ) {
                segid = sym->seginfo->segid;
    #if _INTEL_CPU
            } else if( attr & FE_IMPORT ) {
                if( (sym->mods & FLAG_FAR) || (cvars_state().TargetSwitches & CGSW_X86_BIG_CODE) ) {
                    if( sym->flags & SYM_ADDR_TAKEN ) {
                        segid = cinfo_state().import_segid--;
                    }
                }
    #endif
            }
        } else if( sym->u.var.segid != SEG_NULL ) {
            segid = sym->u.var.segid;
        } else if( attr & FE_GLOBAL ) {
            segid = SEG_DATA;
        } else {
            segid = SEG_CONST;
        }
    }
#endif
    return( segid );
}


BACK_HANDLE FEBack( CGSYM_HANDLE cgsym_handle )
/*********************************************/
{
    SYM_HANDLE          sym_handle = (SYM_HANDLE)cgsym_handle;
    BACK_HANDLE         bck;
    SYMPTR              symptr;
    SYM_ENTRY           sym;

    symptr = SymGetPtr( sym_handle );
    bck = symptr->u1.backinfo;
    if( bck == NULL ) {
        bck = BENewBack( (CGSYM_HANDLE)sym_handle );
        SymGet( &sym, sym_handle );
        sym.u1.backinfo = bck;
        SymReplace( &sym, sym_handle );
    }
    return( bck );
}


int FELexLevel( CGSYM_HANDLE cgsym_handle )
/*****************************************/
{
    /* unused parameters */ (void)cgsym_handle;

    return( 0 );
}


#if _CPU == _AXP
cg_type FEParmType( CGSYM_HANDLE func, CGSYM_HANDLE parm, cg_type tipe )
/**********************************************************************/
{
    /* unused parameters */ (void)func; (void)parm;

    switch( tipe ) {
    case TY_INT_1:
    case TY_INT_2:
    case TY_INT_4:
    case TY_UINT_4:
    case TY_INTEGER:
    case TY_UNSIGNED:
    case TY_POINTER:
    case TY_CODE_PTR:
    case TY_NEAR_POINTER:
    case TY_NEAR_CODE_PTR:
        return( TY_INT_8 );
    case TY_UINT_1:
    case TY_UINT_2:
        return( TY_UINT_8 );
    case TY_DOUBLE:
    case TY_SINGLE:
    case TY_LONG_DOUBLE:
        return( TY_DOUBLE );
    default:
        return( tipe );
    }
}
#else
cg_type FEParmType( CGSYM_HANDLE func, CGSYM_HANDLE parm, cg_type tipe )
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
            sym = SymGetPtr( sym_handle );
            if( sym->mods & FLAG_FAR16 ) {
                return( TY_INT_2 );
            }
        }
#endif
        tipe = TY_INTEGER;
    }
    return( tipe );
}
#endif


int FEStackChk( CGSYM_HANDLE cgsym_handle )
/*****************************************/
{
    SYM_HANDLE  sym_handle = (SYM_HANDLE)cgsym_handle;
    SYMPTR      sym;

    sym = SymGetPtr( sym_handle );
    return( (sym->flags & SYM_CHECK_STACK) != 0 );
}

void SegInit( void )
{
    segment_id  segid;

    cinfo_state().userSegments = NULL;
    cinfo_state().userSegId = FIRST_USER_SEGMENT;
    for( segid = 0; segid < FIRST_PRIVATE_SEGMENT; segid++ ) {
        cvars_state().SegAlignment[segid] = TARGET_INT;
    }
}

void SegFini( void )
{
    user_seg    *useg;

    while( (useg = cinfo_state().userSegments) != NULL ) {
        cinfo_state().userSegments = useg->next;
        CMemFree( useg );
    }
}

void SetSegAlign( SYMPTR sym )
{
    align_type          align;
    segment_id          segid;

    segid = sym->u.var.segid;
    if( segid < FIRST_PRIVATE_SEGMENT && cvars_state().OptSize == 0 ) {
        align = GetTypeAlignment( sym->sym_type );
        if( cvars_state().SegAlignment[segid] < align ) {
            cvars_state().SegAlignment[segid] = align;
        }
    }
}

void ImportNearSegIdInit( void )
{
    cinfo_state().import_near_segid = -1;
}

void ImportSegIdInit( void )
{
    if( cinfo_state().import_near_segid != SEG_NULL ) {
        cinfo_state().import_segid = cinfo_state().import_near_segid - 1;
    } else {
        cinfo_state().import_segid = -1;
    }
}
