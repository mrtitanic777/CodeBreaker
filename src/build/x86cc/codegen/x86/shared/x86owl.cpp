/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2026      The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Minimal OWL object emitter for the 32-bit Intel backend.
*
****************************************************************************/

#include "_cgstd.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "coderep.h"
#include "data.h"
#include "cgauxcc.h"
#include "cgauxinf.h"
#include "cgmem.h"
#include "utils.h"
#include "cgswitch.h"
#include "model.h"
#include "owl.h"
#include "objio.h"
#include "onexit.h"
#include "opttell.h"
#include "x86objd.h"
#include "objout.h"
#include "x86obj.h"
#include "x86emit.h"
#include "cgsegids.h"
#include "feprotos.h"
#include "depinfo.h"
#include "backrun.hpp"

#if _TARGET & _TARG_80386

#define COMMENTV( a )       a, sizeof( a ) - 1
#define COFF_DRECTVE_DEFLIB "-defaultlib:"

typedef struct owl_x86_section {
    struct owl_x86_section *next;
    segment_id              segid;
    seg_attr                attr;
    owl_section_handle      handle;
    owl_symbol_handle       base_symbol;
} owl_x86_section;

typedef struct owl_x86_patch {
    owl_section_handle      section;
    owl_offset              where;
    patch_attr              attr;
} owl_x86_patch;

typedef struct owl_x86_abs_patch {
    struct owl_x86_abs_patch *next;
    owl_x86_patch             patch;
    long_offset               value;
    bool                      have_value;
    bool                      have_offset;
} owl_x86_abs_patch;

namespace {

/* the statics of this file, one set for each run of the compiler */
}  // namespace
struct X86owlState {
    owl_handle OwlHandle;
    owl_file_handle OwlFile;
    owl_x86_section *Sections;
    owl_x86_section *CurrSection;
    owl_x86_abs_patch *AbsPatches;
    segment_id CodeSectionId;
    segment_id DataSectionId;
    segment_id BackSectionId;
    unsigned PendingLine;
    char ObjName[1024];
};
namespace {

}  // namespace



static void owlUnsupported( BackRun& back, const char *what )
{
    FatalError( back, what );
}

static int putBytes( BackRun& back, owl_client_file file, const char *buffer, size_t len )
{
    /* unused parameters */ (void)file;
    PutObjBytes( back, buffer, len );
    return( 0 );
}

static owl_x86_section *findSection( BackRun& back, segment_id segid )
{
    owl_x86_section *section;

    for( section = back.x86owl->Sections; section != NULL; section = section->next ) {
        if( section->segid == segid )
            return( section );
    }
    return( NULL );
}

static void gatherName( BackRun& back, const char *name, char *buffer )
{
    strcpy( buffer, name );
}

static const char *symbolName( BackRun& back, cg_sym_handle sym )
{
    import_kind kind;
    fe_attr     attr;

    attr = FEAttr( *back.front, sym );
    kind = NORMAL;
    if( attr & FE_DLLIMPORT )
        kind = DLLIMPORT;
    DoOutObjectName( back, sym, (outputter_fn *)(gatherName), (outputter_data)(back.x86owl->ObjName), kind );
    return( back.x86owl->ObjName );
}

static const char *labelName( BackRun& back, label_handle label )
{
    cg_sym_handle sym;

    sym = AskForLblSym( back, label );
    if( AskIfRTLabel( back, label ) )
        return( AskRTName( back, static_cast<rt_class>(SymToRtIdx( sym )) ) );
    if( sym != NULL )
        return( symbolName( back, sym ) );
    return( NULL );
}

static owl_symbol_handle labelSymbol( BackRun& back, label_handle label )
{
    const char *name;
    char        generated[32];

    if( label->owl_symbol == NULL ) {
        name = labelName( back, label );
        if( name == NULL ) {
            sprintf( generated, "$L%p", label );
            name = generated;
        }
        label->owl_symbol = OWLSymbolInit( back, back.x86owl->OwlFile, name );
    }
    return static_cast<owl_symbol_handle>(( label->owl_symbol ));
}

static owl_symbol_handle symbolHandle( BackRun& back, cg_sym_handle sym )
{
    return( labelSymbol( back, FEBack( *back.front, sym )->lbl ) );
}

static array_control *newPatchArray( BackRun& back )
{
    array_control *array;

    array = static_cast<array_control *>(CGAlloc( back, sizeof( *array ) ));
    array->array = NULL;
    array->alloc = 0;
    array->used = 0;
    array->entry = sizeof( owl_x86_patch );
    array->inc = 8;
    return( array );
}

static owl_x86_patch *appendPatch( BackRun& back, array_control *array )
{
    owl_x86_patch *patch;
    void          *old;
    unsigned      size;

    if( array->used == array->alloc ) {
        array->alloc += array->inc;
        size = array->alloc * array->entry;
        if( array->array == NULL ) {
            array->array = CGAlloc( back, size );
        } else {
            old = array->array;
            array->array = CGAlloc( back, size );
            memcpy( array->array, old, array->used * array->entry );
            CGFree( back, old );
        }
    }
    patch = (owl_x86_patch *)array->array + array->used;
    ++array->used;
    return( patch );
}

static void deletePatchArray( BackRun& back, array_control *array )
{
    if( array->array != NULL )
        CGFree( back, array->array );
    CGFree( back, array );
}

static void applyPatch( BackRun& back, const owl_x86_patch *patch, offset value )
{
    owl_offset  save;
    uint_32     dword;
    uint_16     word;
    byte        one;

    save = OWLTellLocation( patch->section );
    OWLSetLocation( patch->section, patch->where );
    if( patch->attr & LONG_PATCH ) {
        if( patch->attr & ADD_PATCH ) {
            OWLTellData( patch->section, patch->where, (char *)&dword, sizeof( dword ) );
            dword += value;
        } else {
            dword = value;
        }
        OWLEmitData( back, patch->section, (char *)&dword, sizeof( dword ) );
    } else if( patch->attr & WORD_PATCH ) {
        if( patch->attr & ADD_PATCH ) {
            OWLTellData( patch->section, patch->where, (char *)&word, sizeof( word ) );
            word += value;
        } else {
            word = value;
        }
        OWLEmitData( back, patch->section, (char *)&word, sizeof( word ) );
    } else {
        if( patch->attr & ADD_PATCH ) {
            OWLTellData( patch->section, patch->where, (char *)&one, sizeof( one ) );
            one += value;
        } else {
            one = value;
        }
        OWLEmitData( back, patch->section, (char *)&one, sizeof( one ) );
    }
    OWLSetLocation( patch->section, save );
}

static void freeAbsPatch( BackRun& back, owl_x86_abs_patch *patch )
{
    owl_x86_abs_patch **owner;

    for( owner = &back.x86owl->AbsPatches; *owner != patch; owner = &(*owner)->next ) {
    }
    *owner = patch->next;
    CGFree( back, patch );
}

static owl_reloc_type relocType( BackRun& back, fix_class class_, bool relative )
{
    if( class_ & (F_TLS | F_FAR16) ) {
        owlUnsupported( back, "OWL x86 MVP does not support TLS or far16 relocations" );
    }
    switch( FixClass( class_ ) ) {
    case F_OFFSET:
    case F_BIG_OFFSET:
        return( relative ? OWL_RELOC_BRANCH_REL : OWL_RELOC_WORD );
    default:
        owlUnsupported( back, "OWL x86 MVP supports only flat 32-bit offset relocations" );
        return( OWL_RELOC_ABSOLUTE );
    }
}

static bool inlineFunction( BackRun& back, cg_sym_handle sym )
{
    if( FEAttr( *back.front, sym ) & FE_PROC ) {
        if( FindAuxInfoSym( back, sym, FEINF_CALL_BYTES ) != NULL
          || ((call_class)(pointer_uint)FindAuxInfoSym( back, sym, FEINF_CALL_CLASS ) & FECALL_GEN_MAKE_CALL_INLINE) ) {
            return( true );
        }
    }
    return( false );
}

static void owlInitSegDefs( BackRun& back )
{
    owl_client_funcs funcs = { putBytes, NULL, NULL, CGAlloc, CGFree };
    owl_format       format;

    format = IsModel( back, CGSW_GEN_OBJ_ELF ) ? OWL_FORMAT_ELF : OWL_FORMAT_COFF;
    back.x86owl->OwlHandle = OWLInit( back, &funcs, OWL_CPU_X86 );
    back.x86owl->OwlFile = OWLFileInit( back, back.x86owl->OwlHandle, static_cast<const char *>(FEAuxInfo( *back.front, NULL, FEINF_SOURCE_NAME )), NULL, format, OWL_FILE_OBJECT );
    back.x86owl->Sections = NULL;
    back.x86owl->CurrSection = NULL;
    back.x86owl->AbsPatches = NULL;
    back.x86owl->CodeSectionId = BACKSEGS;
    back.x86owl->DataSectionId = BACKSEGS;
    back.x86owl->BackSectionId = BACKSEGS;
    back.x86owl->PendingLine = 0;
}

static bool owlFreeObjCache( BackRun& back ) { return( false ); }
static segment_id owlAskOP( BackRun& back ) { return( back.x86owl->CurrSection == NULL ? UNDEFSEG : back.x86owl->CurrSection->segid ); }

static void owlSetOP( BackRun& back, segment_id segid )
{
    if( segid == UNDEFSEG ) {
        back.x86owl->CurrSection = NULL;
    } else {
        back.x86owl->CurrSection = findSection( back, segid );
        if( back.x86owl->CurrSection == NULL )
            owlUnsupported( back, "OWL x86 selected an undefined section" );
    }
}

static segment_id owlChangeOP( BackRun& back, segment_id segid )
{
    segment_id old;

    old = owlAskOP(back);
    owlSetOP( back, segid );
    return( old );
}

static void owlDefSegment( BackRun& back, segment_id segid, seg_attr attr, const char *name, uint align, bool use_16 )
{
    owl_x86_section *section;
    owl_section_type type;
    char             base_name[32];

    if( use_16 )
        owlUnsupported( back, "OWL x86 MVP does not support 16-bit sections" );
    section = static_cast<owl_x86_section *>(CGAlloc( back, sizeof( *section ) ));
    section->next = back.x86owl->Sections;
    back.x86owl->Sections = section;
    section->segid = segid;
    section->attr = attr;
    if( attr & EXEC ) {
        type = (attr & COMDAT) ? OWL_SECTION_COMDAT_CODE : OWL_SECTION_CODE;
        if( back.x86owl->CodeSectionId == BACKSEGS )
            back.x86owl->CodeSectionId = segid;
    } else if( attr & INIT ) {
        type = (attr & COMDAT) ? OWL_SECTION_COMDAT_DATA : OWL_SECTION_DATA;
        if( attr & BACK )
            back.x86owl->DataSectionId = segid;
    } else {
        type = (attr & (COMDAT | COMMON)) ? OWL_SECTION_COMDAT_BSS : OWL_SECTION_BSS;
    }
    section->handle = OWLSectionInit( back, back.x86owl->OwlFile, name, type, align == 0 ? 1 : align );
    sprintf( base_name, "$S%d", segid );
    section->base_symbol = OWLSymbolInit( back, back.x86owl->OwlFile, base_name );
    /* OWL creates its own private OWL_TYPE_SECTION symbol.  A normal local
     * symbol gives the x86 emitter a public-API relocation target for offset
     * zero without corrupting OWL's section-symbol bookkeeping. */
    OWLEmitLabel( section->handle, section->base_symbol, OWL_TYPE_OBJECT, OWL_SYM_STATIC );
}

static void owlEmptyQueue( BackRun& back ) {}
static char owlGetMemModel( BackRun& back ) { return( 'f' ); }

static segment_id owlDbgSegDef( BackRun& back, const char *name, const char *class_name, int modifier )
{
    segment_id segid;

    /* unused parameters */ (void)class_name; (void)modifier;
    segid = --back.x86owl->BackSectionId;
    owlDefSegment( back, segid, INIT | GIVEN_NAME, name, 1, false );
    return( segid );
}

static void owlObjInit( BackRun& back )
{
    if( IsModel( back, CGSW_GEN_DBG_TYPES | CGSW_GEN_DBG_LOCALS | CGSW_GEN_DBG_NUMBERS ) )
        owlUnsupported( back, "debug information is not implemented by the OWL x86 MVP" );
    OpenObj(back);
}

static offset owlAskLocation( BackRun& back ) { return( OWLTellLocation( back.x86owl->CurrSection->handle ) ); }
static void owlChkDbgSegSize( BackRun& back, offset max, bool typing ) { (void)max; (void)typing; }

static bool owlUseImportForm( BackRun& back, fe_attr attr )
{
    if( attr & (FE_GLOBAL | FE_IMPORT) )
        return( true );
    return( (attr & FE_INTERNAL) == 0 );
}

static bool owlSegIsNear( BackRun& back, segment_id segid ) { return( findSection( back, segid ) != NULL ); }
static bool owlSegIsBlank( BackRun& back, segment_id segid )
{
    owl_section_type type = OWLTellSectionType( findSection( back, segid )->handle );
    return( type == OWL_SECTION_BSS || type == OWL_SECTION_COMDAT_BSS );
}
static bool owlSegIsPrivate( BackRun& back, segment_id segid )
{
    owl_x86_section *section = findSection( back, segid );
    return( section == NULL || (section->attr & (PRIVATE | EXEC)) != 0 );
}
static bool owlSegIsROM( BackRun& back, segment_id segid )
{
    owl_x86_section *section = findSection( back, segid );
    return( section != NULL && (section->attr & ROM) != 0 );
}
static segment_id owlAskBackSeg( BackRun& back ) { return( back.x86owl->DataSectionId ); }
static segment_id owlAskCodeSeg( BackRun& back ) { return( back.x86owl->CodeSectionId ); }
static bool owlHaveCodeSeg( BackRun& back ) { return( back.x86owl->CodeSectionId != BACKSEGS ); }
static segment_id owlAskAltCodeSeg( BackRun& back ) { return( back.x86owl->CodeSectionId ); }
static segment_id owlAskCode16Seg( BackRun& back ) { owlUnsupported( back, "OWL x86 has no 16-bit code section" ); return( UNDEFSEG ); }
static void owlOutSelect( BackRun& back, bool starts ) { (void)starts; }
static void owlSetUpObj( BackRun& back, bool is_data ) { (void)is_data; }
static void owlFlushOP( BackRun& back, segment_id segid ) { (void)segid; }

static void owlDefaultLibs( BackRun& back )
{
    owl_section_handle comments;
    char               *lib;
    char               *name;

    comments = NULL;
    lib = NULL;
    for( ;; ) {
        lib = static_cast<char *>(FEAuxInfo( *back.front, lib, FEINF_NEXT_LIBRARY ));
        if( lib == NULL )
            break;
        name = (char *)FEAuxInfo( *back.front, lib, FEINF_LIBRARY_NAME );
        if( name == NULL || *name == '\0' )
            continue;
        if( comments == NULL )
            comments = OWLSectionInit( back, back.x86owl->OwlFile, ".drectve", OWL_SECTION_INFO, 1 );
        OWLEmitData( back, comments, COMMENTV( COFF_DRECTVE_DEFLIB ) );
        OWLEmitData( back, comments, name, strlen( name ) );
        OWLEmitData( back, comments, " ", 1 );
    }
    if( comments != NULL )
        OWLEmitData( back, comments, "", 1 );
}

static void stringOut( BackRun& back, const char *name, const char **data )
{
    *data = name;
}

static void owlAliasNames( BackRun& back )
{
    pointer             alias;
    const char          *alias_name;
    const char          *subst_name;
    owl_symbol_handle   owl_alias;
    owl_symbol_handle   owl_subst;

    alias = NULL;
    for( ;; ) {
        alias = FEAuxInfo( *back.front, alias, FEINF_NEXT_ALIAS );
        if( alias == NULL )
            break;
        alias_name = static_cast<const char *>(FEAuxInfo( *back.front, alias, FEINF_ALIAS_NAME ));
        if( alias_name == NULL ) {
            DoOutObjectName( back, static_cast<cg_sym_handle>(FEAuxInfo( *back.front, alias, FEINF_ALIAS_SYMBOL )), (outputter_fn *)(stringOut), (outputter_data)(&alias_name), NORMAL );
        }
        subst_name = static_cast<const char *>(FEAuxInfo( *back.front, alias, FEINF_ALIAS_SUBST_NAME ));
        if( subst_name == NULL ) {
            DoOutObjectName( back, static_cast<cg_sym_handle>(FEAuxInfo( *back.front, alias, FEINF_ALIAS_SUBST_SYMBOL )), (outputter_fn *)(stringOut), (outputter_data)(&subst_name), NORMAL );
        }
        owl_alias = OWLSymbolInit( back, back.x86owl->OwlFile, alias_name );
        owl_subst = OWLSymbolInit( back, back.x86owl->OwlFile, subst_name );
        OWLWeakExt( back.x86owl->OwlFile, owl_alias, owl_subst, OWL_WKSYM_ALIAS );
    }
}

static void owlEmitImports( BackRun& back )
{
    void        *auto_import;
    const char  *name;

    auto_import = NULL;
    for( ;; ) {
        auto_import = FEAuxInfo( *back.front, auto_import, FEINF_NEXT_IMPORT );
        if( auto_import == NULL )
            break;
        OWLEmitImport( back, back.x86owl->OwlFile, static_cast<const char *>(FEAuxInfo( *back.front, auto_import, FEINF_IMPORT_NAME )) );
    }
    auto_import = NULL;
    for( ;; ) {
        auto_import = FEAuxInfo( *back.front, auto_import, FEINF_NEXT_IMPORT_S );
        if( auto_import == NULL )
            break;
        DoOutObjectName( back, static_cast<cg_sym_handle>(FEAuxInfo( *back.front, auto_import, FEINF_IMPORT_NAME_S )), (outputter_fn *)(stringOut), (outputter_data)(&name), NORMAL );
        OWLEmitImport( back, back.x86owl->OwlFile, name );
    }
}

static void owlEmitDependencyInfo( BackRun& back )
{
    owl_section_handle section;
    void               *depend;
    char               *name;
    DepInfo            info;

    section = NULL;
    depend = NULL;
    for( ;; ) {
        depend = FEAuxInfo( *back.front, depend, FEINF_NEXT_DEPENDENCY );
        if( depend == NULL )
            break;
        if( section == NULL )
            section = OWLSectionInit( back, back.x86owl->OwlFile, ".depend", OWL_SECTION_INFO, 16 );
        name = (char *)FEAuxInfo( *back.front, depend, FEINF_DEPENDENCY_NAME );
        info.time = *(time_t *)FEAuxInfo( *back.front, depend, FEINF_DEPENDENCY_TIMESTAMP );
        info.len = strlen( name ) + 1;
        OWLEmitData( back, section, (char *)&info, offsetof( DepInfo, name ) );
        OWLEmitData( back, section, name, info.len );
    }
    if( section != NULL ) {
        info.time = 0;
        info.len = 0;
        OWLEmitData( back, section, (char *)&info, offsetof( DepInfo, name ) );
    }
}

static void owlObjFini( BackRun& back )
{
    owl_x86_section *section;
    owl_x86_section *next;
    owl_x86_abs_patch *patch;

    while( (patch = back.x86owl->AbsPatches) != NULL ) {
        back.x86owl->AbsPatches = patch->next;
        applyPatch( back, &patch->patch, patch->value );
        CGFree( back, patch );
    }
    owlDefaultLibs(back);
    owlAliasNames(back);
    owlEmitImports(back);
    owlEmitDependencyInfo(back);
    OWLFileFini( back, back.x86owl->OwlFile );
    OWLFini( back, back.x86owl->OwlHandle );
    for( section = back.x86owl->Sections; section != NULL; section = next ) {
        next = section->next;
        CGFree( back, section );
    }
    CloseObj(back);
}

static bool owlSymIsExported( BackRun& back, cg_sym_handle sym )
{
    return( sym != NULL && ((FEAttr( *back.front, sym ) & FE_DLLEXPORT) != 0
        || ((call_class)(pointer_uint)FindAuxInfoSym( back, sym, FEINF_CALL_CLASS ) & FECALL_GEN_DLL_EXPORT) != 0) );
}

static void owlOutDLLExport( BackRun& back, uint words, cg_sym_handle sym )
{
    /* unused parameters */ (void)words;
    OWLEmitExport( back.x86owl->OwlFile, symbolHandle( back, sym ) );
}

static void owlOutLabel( BackRun& back, label_handle label )
{
    array_control      *patches;
    owl_x86_patch      *patch;
    cg_sym_handle      sym;
    owl_sym_type       type;
    owl_sym_linkage    linkage;
    unsigned           i;
    offset             location;

    location = owlAskLocation(back);
    TellAddress( back, label, location );
    sym = AskForLblSym( back, label );
    type = OWL_TYPE_OBJECT;
    linkage = OWL_SYM_STATIC;
    if( sym != NULL ) {
        if( FEAttr( *back.front, sym ) & FE_PROC )
            type = OWL_TYPE_FUNCTION;
        if( FEAttr( *back.front, sym ) & FE_GLOBAL )
            linkage = OWL_SYM_GLOBAL;
    }
    OWLEmitLabel( back.x86owl->CurrSection->handle, labelSymbol( back, label ), type, linkage );
    if( owlSymIsExported( back, sym ) )
        OWLEmitExport( back.x86owl->OwlFile, labelSymbol( back, label ) );
    patches = static_cast<array_control *>(AskLblPatches( back, label ));
    for( i = 0; i < patches->used; ++i ) {
        patch = (owl_x86_patch *)patches->array + i;
        applyPatch( back, patch, location );
    }
    deletePatchArray( back, patches );
    TellDonePatches( back, label );
}

static void owlAbsPatch( BackRun& back, abspatch_handle handle, offset value )
{
    owl_x86_abs_patch *patch = (owl_x86_abs_patch *)handle;
    if( patch->have_offset ) {
        applyPatch( back, &patch->patch, value );
        freeAbsPatch( back, patch );
    } else {
        patch->value = value;
        patch->have_value = true;
    }
}

static void *owlInitPatches( BackRun& back ) { return( newPatchArray(back) ); }
static void owlSetLocation( BackRun& back, offset location ) { OWLSetLocation( back.x86owl->CurrSection->handle, location ); }
static void owlSetBigLocation( BackRun& back, long_offset location ) { OWLSetLocation( back.x86owl->CurrSection->handle, location ); }
static void owlIncLocation( BackRun& back, offset amount ) { OWLSetLocation( back.x86owl->CurrSection->handle, owlAskLocation(back) + amount ); }
static void owlOutFPPatch( BackRun& back, fp_patches patch ) { (void)patch; owlUnsupported( back, "floating-point patch records require OMF" ); }

static void owlOutPatch( BackRun& back, label_handle label, patch_attr attr )
{
    owl_x86_patch *patch = appendPatch( back, static_cast<array_control *>(AskLblPatches( back, label )) );
    patch->section = back.x86owl->CurrSection->handle;
    patch->where = owlAskLocation(back);
    patch->attr = attr;
}

static abs_patch *owlNewAbsPatch( BackRun& back )
{
    owl_x86_abs_patch *patch = static_cast<owl_x86_abs_patch *>(CGAlloc( back, sizeof( *patch ) ));
    memset( patch, 0, sizeof( *patch ) );
    patch->next = back.x86owl->AbsPatches;
    back.x86owl->AbsPatches = patch;
    return( (abs_patch *)patch );
}

static void owlOutDataByte( BackRun& back, byte value ) { OWLEmitData( back, back.x86owl->CurrSection->handle, (char *)&value, sizeof( value ) ); }
static void owlOutDataShort( BackRun& back, uint_16 value ) { OWLEmitData( back, back.x86owl->CurrSection->handle, (char *)&value, sizeof( value ) ); }
static void owlOutDataLong( BackRun& back, uint_32 value ) { OWLEmitData( back, back.x86owl->CurrSection->handle, (char *)&value, sizeof( value ) ); }

static void owlOutAbsPatch( BackRun& back, abs_patch *handle, patch_attr attr )
{
    owl_x86_abs_patch *patch = (owl_x86_abs_patch *)handle;
    long_offset value = 0;
    if( patch->have_value ) {
        value = patch->value;
        freeAbsPatch( back, patch );
    } else {
        patch->patch.section = back.x86owl->CurrSection->handle;
        patch->patch.where = owlAskLocation(back);
        patch->patch.attr = attr;
        patch->have_offset = true;
    }
    if( attr & LONG_PATCH ) owlOutDataLong( back, value );
    else if( attr & WORD_PATCH ) owlOutDataShort( back, value );
    else owlOutDataByte( back, value );
}

static void owlOutReloc( BackRun& back, segment_id segid, fix_class class_, bool relative )
{
    owl_x86_section *target = findSection( back, segid );
    if( target == NULL )
        owlUnsupported( back, "OWL x86 relocation targets an undefined section" );
    OWLEmitReloc( back, back.x86owl->CurrSection->handle, owlAskLocation(back), target->base_symbol, relocType( back, class_, relative ) );
}

static void owlOutSpecialCommon( BackRun& back, import_handle handle, fix_class class_, bool relative )
{
    (void)handle; (void)class_; (void)relative;
    owlUnsupported( back, "conditional/common import handles are not implemented by the OWL x86 MVP" );
}

static void owlOutImport( BackRun& back, cg_sym_handle sym, fix_class class_, bool relative )
{
    OWLEmitReloc( back, back.x86owl->CurrSection->handle, owlAskLocation(back), symbolHandle( back, sym ), relocType( back, class_, relative ) );
}

static void owlOutRTImportRel( BackRun& back, rt_class index, fix_class class_, bool relative )
{
    owl_symbol_handle symbol = OWLSymbolInit( back, back.x86owl->OwlFile, AskRTName( back, index ) );
    OWLEmitReloc( back, back.x86owl->CurrSection->handle, owlAskLocation(back), symbol, relocType( back, class_, relative ) );
}

static void owlOutRTImport( BackRun& back, rt_class index, fix_class class_ )
{
    owlOutRTImportRel( back, index, class_, FixClass( class_ ) == F_OFFSET || FixClass( class_ ) == F_LDR_OFFSET );
}

static void owlOutBckExport( BackRun& back, const char *name, bool is_export )
{
    owl_symbol_handle symbol = OWLSymbolInit( back, back.x86owl->OwlFile, name );
    OWLEmitLabel( back.x86owl->CurrSection->handle, symbol, OWL_TYPE_OBJECT, OWL_SYM_GLOBAL );
    if( is_export )
        OWLEmitExport( back.x86owl->OwlFile, symbol );
}

static void owlOutBckImport( BackRun& back, const char *name, back_handle bck, fix_class class_ )
{
    owl_symbol_handle symbol;
    (void)bck;
    symbol = OWLSymbolInit( back, back.x86owl->OwlFile, name );
    OWLEmitReloc( back, back.x86owl->CurrSection->handle, owlAskLocation(back), symbol, relocType( back, class_, false ) );
}

static void owlOutLineNum( BackRun& back, cg_linenum line, bool label_line ) { (void)label_line; back.x86owl->PendingLine = line; }
static unsigned owlSavePendingLine( BackRun& back, unsigned line ) { unsigned old = back.x86owl->PendingLine; back.x86owl->PendingLine = line; return( old ); }
static void owlOutBytes( BackRun& back, unsigned len, const byte *src ) { if( len != 0 ) OWLEmitData( back, back.x86owl->CurrSection->handle, (const char *)src, len ); }

static void owlOutIterated( BackRun& back, byte pattern, offset len )
{
    byte buffer[64];
    unsigned amount;
    memset( buffer, pattern, sizeof( buffer ) );
    while( len != 0 ) {
        amount = len > sizeof( buffer ) ? sizeof( buffer ) : len;
        OWLEmitData( back, back.x86owl->CurrSection->handle, (char *)buffer, amount );
        len -= amount;
    }
}

static bool owlNeedBaseSet( BackRun& back ) { return( false ); }
static offset owlAskMaxSize( BackRun& back ) { return( OWLTellSize( back.x86owl->CurrSection->handle ) ); }
static long_offset owlAskBigLocation( BackRun& back ) { return( OWLTellLocation( back.x86owl->CurrSection->handle ) ); }
static long_offset owlAskBigMaxSize( BackRun& back ) { return( OWLTellSize( back.x86owl->CurrSection->handle ) ); }
static void owlTellNewLabel( BackRun& back, cg_sym_handle sym ) { (void)sym; }

static void owlTellNewProc( BackRun& back, cg_sym_handle proc )
{
    segment_id segid = FESegID( *back.front, proc );
    if( findSection( back, segid ) != NULL ) {
        back.x86owl->CodeSectionId = segid;
        owlSetOP( back, segid );
    }
}

static void owlTellVirtFuncRef( BackRun& back, void *cookie ) { (void)cookie; owlUnsupported( back, "virtual-function linker directives are not implemented by the OWL x86 MVP" ); }

static segment_id owlAskSegID( BackRun& back, pointer handle, cg_class class_ )
{
    switch( class_ ) {
    case CG_FE:
        if( inlineFunction( back, (cg_sym_handle)handle ) ) return( back.x86owl->CodeSectionId );
        return( FESegID( *back.front, (cg_sym_handle)handle ) );
    case CG_BACK: return( ((back_handle)handle)->segid );
    case CG_TBL:
    case CG_VTB:
    case CG_CLB: return( back.x86owl->CodeSectionId );
    default: return( back.x86owl->DataSectionId );
    }
}

static bool owlNameIsCode( BackRun& back, pointer handle, cg_class class_ )
{
    switch( class_ ) {
    case CG_FE: return( (FEAttr( *back.front, static_cast<cg_sym_handle>(handle) ) & FE_PROC) != 0 );
    case CG_TBL:
    case CG_VTB:
    case CG_CLB: return( true );
    default: return( false );
    }
}

static bool owlNameIsROM( BackRun& back, pointer handle, cg_class class_ ) { return( owlSegIsROM( back, owlAskSegID( back, handle, class_ ) ) ); }

const x86_obj_emitter X86OwlEmitter = {
    owlInitSegDefs,
    owlFreeObjCache,
    owlAskOP,
    owlChangeOP,
    owlSetOP,
    owlDefSegment,
    owlEmptyQueue,
    owlGetMemModel,
    owlDbgSegDef,
    owlObjInit,
    owlAskLocation,
    owlChkDbgSegSize,
    owlUseImportForm,
    owlSegIsNear,
    owlSegIsBlank,
    owlSegIsPrivate,
    owlSegIsROM,
    owlAskBackSeg,
    owlAskCodeSeg,
    owlHaveCodeSeg,
    owlAskAltCodeSeg,
    owlAskCode16Seg,
    owlOutSelect,
    owlSetUpObj,
    owlFlushOP,
    owlObjFini,
    owlOutDLLExport,
    owlOutLabel,
    owlAbsPatch,
    owlInitPatches,
    owlSetBigLocation,
    owlIncLocation,
    owlSetLocation,
    owlOutFPPatch,
    owlOutPatch,
    owlNewAbsPatch,
    owlOutDataByte,
    owlOutDataShort,
    owlOutDataLong,
    owlOutAbsPatch,
    owlOutReloc,
    owlOutSpecialCommon,
    owlOutImport,
    owlOutRTImportRel,
    owlOutRTImport,
    owlOutBckExport,
    owlOutBckImport,
    owlOutLineNum,
    owlSavePendingLine,
    owlOutBytes,
    owlOutIterated,
    owlNeedBaseSet,
    owlAskMaxSize,
    owlAskBigLocation,
    owlAskBigMaxSize,
    owlTellNewLabel,
    owlTellNewProc,
    owlTellVirtFuncRef,
    owlAskSegID,
    owlNameIsCode,
    owlNameIsROM,
    owlSymIsExported
};

#endif

/* the run makes and frees its copy of this file's variables (run/ files: see the run header) */
#if _TARGET & _TARG_80386
X86owlState* x86owl_make() { return new X86owlState(); }
void x86owl_free(X86owlState* part) { delete part; }
#endif
