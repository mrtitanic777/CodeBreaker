/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2025 The Open Watcom Contributors. All Rights Reserved.
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
* Description:  Code to handle pre-compiled header files.
*
****************************************************************************/


#include "cvars.h"
#include "wio.h"
#include "watcom.h"
#include "sopen.h"
#include "cmacadd.h"
#include "toggles.h"

#include "clibext.h"
#include "frontrun.hpp"


#define PH_BUF_SIZE     32768
#define PCH_SIGNATURE   (('H'<<24)|('C'<<16)|('P'<<8)|'W')     /* 'WPCH' */
#define PCH_VERSION     0x0128
#define PCH_VERSION_HOST ( ( 7L << 16 ) | PCH_VERSION )

#define PCHGetUInt(p)           ((unsigned)(pointer_uint)(p))
#define PCHSetUInt(v)           ((void *)(pointer_uint)((unsigned)(v)))


#define PCHAlign(p)             _RoundUp(p,sizeof(int))



#define PCH_FIRST_USER_INDEX    (PCH_FIRST_INDEX + MAX_BUILTIN_AUXINFO)


namespace {

/* the statics of this file, one set for each run of the compiler */
/* (struct PchdrState: pchdr_state.h) */

}  // namespace



typedef struct pheader {
    unsigned        signature;      //  'WPCH'
    unsigned        version;
    unsigned        size_of_header;
    unsigned        size_of_int;
    unsigned        pack_amount;    // PackAmount
    unsigned        gen_switches;   // GenSwitches
    unsigned        target_switches;// TargetSwitches
    pragma_toggles  toggles;        // PragmaToggles
    unsigned        size;
    unsigned        macro_size;
    unsigned        file_count;
    unsigned        rdir_count;
    unsigned        ialias_count;
    unsigned        incfile_count;
    unsigned        incline_count;  // IncLineCount
    unsigned        library_count;  // # of pragma library(s)
    unsigned        alias_count;    // # of pragma alias(s)
    unsigned        seg_count;
    unsigned        macro_count;
    unsigned        undef_macro_count;
    unsigned        type_count;
    unsigned        tag_count;
    unsigned        pragma_count;
    unsigned        pragma_entry_count;
    unsigned        symhash_count;
    unsigned        symbol_count;
    unsigned        specialsyms_count;
    unsigned        cwd_len;        // length of current working directory
    unsigned        msglevel_len;   // length of msg_level array
    unsigned        cpp_ignore_env;
    unsigned        ignore_default_dirs;
} pheader;

static int FixupDataStructures( FrontRun& front, char *p, pheader *pch );

void InitDebugTypes( FrontRun& front );

//========================================================================
//      This portion of the code creates the pre-compiled header.
//========================================================================

static void InitPHVars( FrontRun& front )
//*****************************
// Set vars to 0
//*****************************
{
    front.pchdr.PH_SymHashCount    = 0;
    front.pchdr.PH_FileCount       = 0;
    front.pchdr.PH_RDirCount       = 0;
    front.pchdr.PH_IAliasCount     = 0;
    front.pchdr.PH_IncFileCount    = 0;
    front.pchdr.PH_LibraryCount    = 0;
    front.pchdr.PH_AliasCount      = 0;
    front.pchdr.PH_SegCount        = 0;
    front.pchdr.PH_MacroCount      = 0;
    front.pchdr.PH_UndefMacroCount = 0;
    front.pchdr.PH_TypeCount       = 0;
    front.pchdr.PH_TagCount        = 0;
    front.pchdr.PH_PragmaCount     = 0;
    front.pchdr.PH_PragmaEntryCount = 0;
    front.pchdr.PH_size            = 0;
    front.pchdr.PH_MacroSize       = 0;
    front.pchdr.PH_cwd_len         = 0;
    front.pchdr.PH_computing_size  = 0;
}

static void CreatePHeader( FrontRun& front, const char *filename )
{
    front.pchdr.PH_handle = _sopen4( filename, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, SH_DENYRW, PMODE_RW );
    if( front.pchdr.PH_handle == -1 ) {
        longjmp( front.pchdr.PH_jmpbuf, 1 );
    }
}

static bool pch_write( FrontRun& front, const char *buf, size_t len )
{
    size_t     amt_towrite;

    if( front.pchdr.PH_Buffer != NULL ) {
        for( ; len != 0; ) {
            amt_towrite = len;
            if( amt_towrite > front.pchdr.PH_BufSize )
                amt_towrite = front.pchdr.PH_BufSize;
            memcpy( front.pchdr.PH_BufPtr, buf, amt_towrite );
            front.pchdr.PH_BufSize -= amt_towrite;
            front.pchdr.PH_BufPtr  += amt_towrite;
            buf += amt_towrite;
            len -= amt_towrite;
            if( front.pchdr.PH_BufSize == 0 ) {         // if buffer is full
                front.pchdr.PH_BufSize = PH_BUF_SIZE;
                front.pchdr.PH_BufPtr = front.pchdr.PH_Buffer;
                if( write( front.pchdr.PH_handle, front.pchdr.PH_Buffer, PH_BUF_SIZE ) != PH_BUF_SIZE ) {
                    return( true );
                }
            }
        }
    } else if( len > 0 ) {
        if( write( front.pchdr.PH_handle, buf, len ) != len ) {
            return( true );
        }
    }
    return( false );
}

static bool pch_align( FrontRun& front, size_t len )
{
    len = PCHAlign( len ) - len;
    if( len > 0 ) {
        char    padding[8] = {0};
        return( pch_write( front, padding, len ) );
    }
    return( false );
}

static bool PCHWrite( FrontRun& front, const void *bufptr, size_t len )
{
    bool    rc = false;

    if( len != 0 ) {
        if( front.pchdr.PH_computing_size ) {
            front.pchdr.PH_size += PCHAlign( len );
        } else {
            rc = pch_write( front, static_cast<const char *>(bufptr), len );
            rc |= pch_align( front, len );
        }
    }
    return( rc );
}

static bool PCHWriteUnalign( FrontRun& front, const void *bufptr, size_t len )
{
    if( len != 0 ) {
        if( front.pchdr.PH_computing_size ) {
            front.pchdr.PH_size += len;
        } else {
            return( pch_write( front, static_cast<const char *>(bufptr), len ) );
        }
    }
    return( false );
}

/* writes the bytes of a variable (its address and size) */
template< class Var >
static inline bool PCHWriteVar( FrontRun& front, Var& var ) { return PCHWriteUnalign( front, &var, sizeof( var ) ); }

static void FlushPHeader( FrontRun& front )
{
    unsigned    len;

    if( front.pchdr.PH_BufSize != PH_BUF_SIZE ) {   // if buffer has some stuff in it
        len = (unsigned)( PH_BUF_SIZE - front.pchdr.PH_BufSize );
        if( (unsigned)write( front.pchdr.PH_handle, front.pchdr.PH_Buffer, len ) != len ) {
            longjmp( front.pchdr.PH_jmpbuf, 1 );
        }
    }
}

static void ClosePHeader( FrontRun& front )
{
    close( front.pchdr.PH_handle );
    front.pchdr.PH_handle = -1;
}


static void OutPutHeader( FrontRun& front )
{
    bool            rc;
    pheader         pch;

    pch.signature         = PCH_SIGNATURE;
    pch.version           = PCH_VERSION_HOST;
    pch.size_of_header    = sizeof( pheader );
    pch.size_of_int       = TARGET_INT;
    pch.pack_amount       = front.cvars.PackAmount;
    pch.gen_switches      = front.cvars.GenSwitches;
    pch.target_switches   = front.cvars.TargetSwitches;
    pch.toggles           = front.toggles.PragmaToggles;
    pch.size              = front.pchdr.PH_size - front.pchdr.PH_MacroSize;
    pch.macro_size        = front.pchdr.PH_MacroSize;
    pch.file_count        = front.pchdr.PH_FileCount;
    pch.rdir_count        = front.pchdr.PH_RDirCount;
    pch.ialias_count      = front.pchdr.PH_IAliasCount;
    pch.incfile_count     = front.pchdr.PH_IncFileCount;
    pch.incline_count     = front.cvars.IncLineCount;
    pch.library_count     = front.pchdr.PH_LibraryCount;
    pch.alias_count       = front.pchdr.PH_AliasCount;
    pch.seg_count         = front.pchdr.PH_SegCount;
    pch.macro_count       = front.pchdr.PH_MacroCount;
    pch.undef_macro_count = front.pchdr.PH_UndefMacroCount;
    pch.type_count        = front.pchdr.PH_TypeCount;
    pch.tag_count         = front.pchdr.PH_TagCount;
    pch.pragma_count      = front.pchdr.PH_PragmaCount;
    pch.pragma_entry_count = front.pchdr.PH_PragmaEntryCount;
    pch.symhash_count     = front.pchdr.PH_SymHashCount;
    pch.symbol_count      = SymGetNumSyms(front);
    pch.specialsyms_count = SymGetNumSpecialSyms(front);
    pch.cwd_len           = front.pchdr.PH_cwd_len;
    pch.msglevel_len      = sizeof( front.cerrs.msg_level );
    pch.cpp_ignore_env    = front.cvars.CompFlags.cpp_ignore_env;
    pch.ignore_default_dirs = front.cvars.CompFlags.ignore_default_dirs;

    rc = PCHWriteVar( front, pch );
    rc |= PCHWrite( front, front.pchdr.PH_Buffer + sizeof( pch ), pch.cwd_len );
    if( rc ) {
        longjmp( front.pchdr.PH_jmpbuf, rc );
    }
}

static void OutPutIncPathList( FrontRun& front )     // output include paths
{
    bool        rc;
    size_t      len;

    len = strlen( front.cvars.IncPathList ) + 1;
    rc = PCHWrite( front, front.cvars.IncPathList, len );
    if( rc ) {
        longjmp( front.pchdr.PH_jmpbuf, rc );
    }
}

static void OutPutIncFileList( FrontRun& front )   // output primary include files
{
    bool        rc;
    size_t      len;
    INCFILE     *ifile;
    INCFILE     *next_file;

    for( ifile = front.cvars.IncFileList; ifile != NULL; ifile = next_file ) {
        next_file = ifile->nextfile;
        len = offsetof( INCFILE, filename ) + strlen( ifile->filename ) + 1;
        ifile->nextfile = static_cast<struct incfile *>(PCHSetUInt( PCHAlign( len ) ));
        rc = PCHWrite( front, ifile, len );
        ifile->nextfile = next_file;
        if( rc ) {
            longjmp( front.pchdr.PH_jmpbuf, rc );
        }
        front.pchdr.PH_IncFileCount++;
    }
}

static void OutPutLibraries( FrontRun& front )
{
    bool                rc;
    size_t              len;
    library_list        *lib;
    library_list        *lib_next;

    for( lib = front.cvars.HeadLibs; lib != NULL; lib = lib_next ) {
        lib_next = lib->next;
        len = offsetof( library_list, libname ) + strlen( lib->libname ) + 1;
        lib->next = static_cast<struct library_list *>(PCHSetUInt( PCHAlign( len ) ));
        rc = PCHWrite( front, lib, len );
        lib->next = lib_next;
        if( rc ) {
            longjmp( front.pchdr.PH_jmpbuf, rc );
        }
        front.pchdr.PH_LibraryCount++;
    }
}

static void OutPutAliases( FrontRun& front )
{
    bool            rc;
    size_t          len;
    size_t          alias_len;
    size_t          subst_len;
    alias_list      *alias;
    alias_list      *alias_next;
    const char      *alias_name;
    const char      *alias_subst;

    for( alias = front.cvars.AliasHead; alias != NULL; alias = alias->next ) {
        alias_next = alias->next;
        alias_name = alias->name;
        alias_subst = alias->subst;
        len = offsetof( alias_list, names );
        if( alias_name != NULL ) {
            alias_len = strlen( alias_name ) + 1;
            alias->name = static_cast<const char *>(PCHSetUInt( alias_len ));
            len += alias_len;
        }
        if( alias_subst != NULL ) {
            subst_len =  strlen( alias_subst ) + 1;
            alias->subst = static_cast<const char *>(PCHSetUInt( subst_len ));
            len += subst_len;
        }
        alias->next = static_cast<struct alias_list *>(PCHSetUInt( PCHAlign( len ) ));
        rc = PCHWrite( front, alias, len );
        alias->subst = alias_subst;
        alias->name = alias_name;
        alias->next = alias_next;
        if( rc ) {
            longjmp( front.pchdr.PH_jmpbuf, rc );
        }
        front.pchdr.PH_AliasCount++;
    }
}

static void OutPutMsgLevels( FrontRun& front )
{
    bool        rc;

    rc = PCHWrite( front, front.cerrs.msg_level, sizeof( front.cerrs.msg_level ) );
    if( rc ) {
        longjmp( front.pchdr.PH_jmpbuf, rc );
    }
}

static void OutPutIncludes( FrontRun& front )
{
    FNAMEPTR    flist;
    FNAMEPTR    next_flist;
    size_t      len;
    bool        rc;

    // don't want to include the primary source file, skip first item
    for( flist = front.cvars.FNames->next; flist != NULL; flist = next_flist ) {
        next_flist = flist->next;
        len = offsetof( fname_list, name ) + strlen( flist->name ) + 1;
        flist->next = static_cast<struct fname_list *>(PCHSetUInt( PCHAlign( len ) ));
        rc = PCHWrite( front, flist, len );
        flist->next = next_flist;
        if( rc ) {
            longjmp( front.pchdr.PH_jmpbuf, rc );
        }
        front.pchdr.PH_FileCount++;
    }
}

static void OutPutRoDirList( FrontRun& front )
{
    RDIRPTR     dirlist;
    RDIRPTR     next_dirlist;
    size_t      len;
    bool        rc;

    for( dirlist = front.pchdr.PCHRDirNames; dirlist != NULL; dirlist = next_dirlist ) {
        next_dirlist = dirlist->next;
        len = offsetof( rdir_list, name ) + strlen( dirlist->name ) + 1;
        dirlist->next = static_cast<struct rdir_list *>(PCHSetUInt( PCHAlign( len ) ));
        rc = PCHWrite( front, dirlist, len );
        dirlist->next = next_dirlist;
        if( rc ) {
            longjmp( front.pchdr.PH_jmpbuf, rc );
        }
        front.pchdr.PH_RDirCount++;
    }
}

static void OutPutIncAliasList( FrontRun& front )
{
    IALIASPTR   aliaslist;
    IALIASPTR   next_aliaslist;
    char        *real_name;
    size_t      len;
    size_t      alias_len;
    bool        rc;

    for( aliaslist = front.cvars.IAliasNames; aliaslist != NULL; aliaslist = next_aliaslist ) {
        next_aliaslist = aliaslist->next;
        real_name = aliaslist->real_name;
        alias_len = aliaslist->real_name - aliaslist->alias_name;
        len = offsetof( ialias_list, alias_name ) + alias_len + strlen( aliaslist->real_name ) + 1;
        aliaslist->next = static_cast<struct ialias_list *>(PCHSetUInt( PCHAlign( len ) ));
        aliaslist->real_name = static_cast<char *>(PCHSetUInt( alias_len ));
        rc = PCHWrite( front, aliaslist, len );
        aliaslist->real_name = real_name;
        aliaslist->next = next_aliaslist;
        if( rc ) {
            longjmp( front.pchdr.PH_jmpbuf, rc );
        }
        front.pchdr.PH_IAliasCount++;
    }
}

static void OutPutSegInfo( FrontRun& front )
{
    textsegment     *tseg;
    textsegment     *tseg_next;
    bool            rc;
    size_t          len;

    for( tseg = front.cvars.TextSegList; tseg != NULL; tseg = tseg_next ) {
        tseg_next = tseg->next;             // save next pointer
        front.pchdr.PH_SegCount++;
        tseg->index = front.pchdr.PH_SegCount;
        len = offsetof( textsegment, segname ) + tseg->class_ + strlen( tseg->segname + tseg->class_ ) + 1;
        tseg->next = static_cast<struct textsegment *>(PCHSetUInt( PCHAlign( len ) ));
        rc = PCHWrite( front, tseg, len );
        tseg->next = tseg_next;             // restore next pointer
        if( rc ) {
            longjmp( front.pchdr.PH_jmpbuf, rc );
        }
    }
}

static void OutPutEnums( FrontRun& front, ENUMPTR ep, TAGPTR parent )
{
    bool        rc;
    size_t      len;

    for( ; ep != NULL; ep = ep->thread ) {
        len = offsetof( ENUMDEFN, name ) + strlen( ep->name ) + 1;
        ep->parent = static_cast<struct tag_entry *>(PCHSetUInt( PCHAlign( len ) ));
        rc = PCHWrite( front, ep, len );
        ep->parent = parent;
        if( rc ) {
            longjmp( front.pchdr.PH_jmpbuf, rc );
        }
    }
}

static void OutPutFields( FrontRun& front, FIELDPTR field )
{
    bool        rc;
    size_t      len;
    TYPEPTR     field_type;

    for( ; field != NULL; field = field->next_field ) {
        field_type = field->field_type;         // save pointer
        len = offsetof( FIELD_ENTRY, name ) + strlen( field->name ) + 1;
        field->next_field_same_hash = static_cast<struct field_entry *>(PCHSetUInt( PCHAlign( len ) ));
        field->field_type = static_cast<TYPEPTR>(PCHSetUInt( field_type->u1.type_index ));
        rc = PCHWrite( front, field, len );
        field->field_type = field_type;         // restore pointer
        if( rc ) {
            longjmp( front.pchdr.PH_jmpbuf, rc );
        }
    }
}

static void OutPutATag( FrontRun& front, TAGPTR tag )
{
    bool        rc;
    TYPEPTR     sym_type;
    TAGPTR      next_tag;
    size_t      len;

    next_tag = tag->next_tag;
    sym_type = tag->sym_type;
    len = offsetof( TAGDEFN, name ) + strlen( tag->name ) + 1;
    tag->next_tag = static_cast<struct tag_entry *>(PCHSetUInt( PCHAlign( len ) ));
    tag->sym_type = static_cast<TYPEPTR>(PCHSetUInt( sym_type->u1.type_index ));
    rc = PCHWrite( front, tag, len );
    tag->sym_type = sym_type;
    tag->next_tag = next_tag;
    if( rc ) {
        longjmp( front.pchdr.PH_jmpbuf, rc );
    }
    if( sym_type->decl_type == TYP_ENUM ) {
        OutPutEnums( front, tag->u.enum_list, tag );
    } else {
        OutPutFields( front, tag->u.field_list );
    }
}

static void OutPutTags( FrontRun& front )
{
    WalkTagList( front, OutPutATag );
}

static void SetTypeIndex( FrontRun& front, TYPEPTR typ )
{
    front.pchdr.PH_TypeCount++;
    typ->u1.type_index = front.pchdr.PH_TypeCount;
}

static void SetFuncTypeIndex( FrontRun& front, TYPEPTR typ, int index )
{
    /* unused parameters */ (void)index;

    SetTypeIndex( front, typ );
}

static void NumberTypes( FrontRun& front )
{
    front.pchdr.PH_TypeCount = 0;
    WalkTypeList( front, SetTypeIndex );
    WalkFuncTypeList( front, SetFuncTypeIndex );
}

static void SetTagIndex( FrontRun& front, TAGPTR tag )
{
    tag->refno = front.pchdr.PH_TagCount++;
}

static void SetDebugTag( FrontRun& front, TAGPTR tag )
{
    tag->refno = 0;
}

static void NumberTags( FrontRun& front )
{
    front.pchdr.PH_TagCount = 0;
    WalkTagList( front, SetTagIndex );
}

static void InitDebugTags( FrontRun& front )
{
    WalkTagList( front, SetDebugTag );
}

static bool WriteType( FrontRun& front, TYPEPTR typ )
{
    bool        rc;
    TYPEPTR     object;

    object = typ->object;
    if( object != NULL ) {
        typ->object = static_cast<struct type_definition *>(PCHSetUInt( object->u1.type_index ));
        rc = PCHWriteVar( front, *typ );
        typ->object = object;
    } else {
        rc = PCHWriteVar( front, *typ );
    }
    return( rc );
}

typedef struct type_indices {
    int     basetype_index[DATA_TYPE_SIZE];
    int     stringtype_index;
    int     constchartype_index;
} type_indices;

static void OutPutTypeIndexes( FrontRun& front )
{
    TYPEPTR             typ;
    bool                rc;
    int                 i;
    type_indices        typ_index;

    for( i = 0; i < DATA_TYPE_SIZE; i++ ) {
        typ = front.cvars.BaseTypes[i];
        if( typ == NULL ) {
            typ_index.basetype_index[i] = 0;
        } else {
            typ_index.basetype_index[i] = typ->u1.type_index;
        }
    }
    typ_index.stringtype_index = front.cvars.StringType->u1.type_index;
    typ_index.constchartype_index = front.cvars.ConstCharType->u1.type_index;
    rc = PCHWriteVar( front, typ_index );
    if( rc ) {
        longjmp( front.pchdr.PH_jmpbuf, rc );
    }
}

static void OutPutAType( FrontRun& front, TYPEPTR typ )
{
    TAGPTR          tag;
    bool            rc;
    array_info      *array;

    rc = false;
    switch( typ->decl_type ) {
    case TYP_ARRAY:
        array = typ->u.array;                   // save pointer
        typ->u.array = static_cast<array_info *>(PCHSetUInt( array->dimension ));  // replace with dim
        rc = WriteType( front, typ );
        typ->u.array = array;                   // restore pointer
        break;
    case TYP_STRUCT:
    case TYP_UNION:
    case TYP_ENUM:
        tag = typ->u.tag;                       // save tag
        typ->u.tag = static_cast<struct tag_entry *>(PCHSetUInt( tag->refno ));  // replace with index
        rc = WriteType( front, typ );
        typ->u.tag = tag;                       // restore tag pointer
        break;
    default:
        if( typ->decl_type != TYP_FUNCTION ) {
            rc = WriteType( front, typ );
        }
        break;
    }
    if( rc ) {
        longjmp( front.pchdr.PH_jmpbuf, rc );
    }
}

static void OutPutAFuncType( FrontRun& front, TYPEPTR typ, int index )
{
    TYPEPTR     *parm_types;
    bool        rc;

    parm_types = typ->u.fn.parms;           // save pointer
    typ->u.fn.parms = static_cast<struct type_definition **>(PCHSetUInt( index ));  // replace with index
    rc = WriteType( front, typ );
    typ->u.fn.parms = parm_types;           // restore pointer
    if( rc ) {
        longjmp( front.pchdr.PH_jmpbuf, rc );
    }
}

static void OutPutFuncParmList( FrontRun& front, TYPEPTR typ, int index )
{
    TYPEPTR     *parm_types;
    bool        rc;
    TYPEPTR     parm_type;

    /* unused parameters */ (void)index;

    if( typ->u.fn.parms != NULL ) {
        for( parm_types = typ->u.fn.parms; (parm_type = *parm_types) != NULL; parm_types++ ) {
            parm_type = static_cast<TYPEPTR>(PCHSetUInt( parm_type->u1.type_index ));
            rc = PCHWriteVar( front, parm_type );
            if( rc ) {
                longjmp( front.pchdr.PH_jmpbuf, rc );
            }
        }
    }
    parm_type = static_cast<TYPEPTR>(PCHSetUInt( -1 ));
    rc = PCHWriteVar( front, parm_type );
    if( rc ) {
        longjmp( front.pchdr.PH_jmpbuf, rc );
    }
}

static void OutPutTypes( FrontRun& front )
{
    NumberTypes(front);
    NumberTags(front);
    WalkTypeList( front, OutPutAType );
    WalkFuncTypeList( front, OutPutAFuncType );
    WalkFuncTypeList( front, OutPutFuncParmList );
    OutPutTypeIndexes(front);
}

static void OutPutAuxInfo( FrontRun& front, aux_info *info )
{
    hw_reg_set          *regs;
    hw_reg_set          *info_parms;
    char                *info_objname;
    byte_seq            *info_code;
    bool                rc;
    size_t              len;
    size_t              objname_len;
    size_t              code_len;

    objname_len = 0;
    code_len = 0;
    info_parms = info->parms;
    info_objname = info->objname;
    info_code = info->code;
    if( info_parms != NULL ) {
        len = 0;
        for( regs = info_parms; ; regs++ ) {
            len += sizeof( hw_reg_set );
            if( HW_CEqual( *regs, HW_EMPTY ) ) {
                break;
            }
        }
        info->parms = static_cast<hw_reg_set *>(PCHSetUInt( len ));
    }
    if( info_objname != NULL ) {
        objname_len = strlen( info_objname ) + 1;
        info->objname = static_cast<char *>(PCHSetUInt( PCHAlign( objname_len ) ));
    }
    if( info_code != NULL ) {
        code_len = offsetof( byte_seq, data ) + info_code->length;
        info->code = static_cast<byte_seq *>(PCHSetUInt( PCHAlign( code_len ) ));
    }
    rc = PCHWriteVar( front, *info );
    if( info_parms != NULL ) {
        for( regs = info_parms; ; regs++ ) {
            rc |= PCHWriteVar( front, *regs );
            if( HW_CEqual( *regs, HW_EMPTY ) ) {
                break;
            }
        }
    }
    if( info_objname != NULL ) {
        rc |= PCHWrite( front, info_objname, objname_len );
    }
    if( info_code != NULL ) {
        rc |= PCHWrite( front, info_code, code_len );
    }
    info->parms = info_parms;
    info->objname = info_objname;
    info->code = info_code;
    if( rc ) {
        longjmp( front.pchdr.PH_jmpbuf, rc );
    }
}

static void OutPutPragmaInfo( FrontRun& front )
{
    aux_entry       *ent;
    aux_entry       *ent_next;
    aux_info        *ent_info;
    int             index;
    bool            rc;
    size_t          len;

    // write built-in aux_info
    for( index = PCH_FIRST_INDEX; index < PCH_FIRST_USER_INDEX; index++ ) {
        ent_info = front.callinfo.BuiltinAuxInfo + index - PCH_FIRST_INDEX;
        ent_info->index = index;
        OutPutAuxInfo( front, ent_info );      // write out the aux_info struct
    }
    // write user aux_info
    for( ent = front.pragdefn.AuxList; ent != NULL; ent = ent->next ) {
        ent->info->index = PCH_NULL_INDEX;
    }
    for( ent = front.pragdefn.AuxList; ent != NULL; ent = ent->next ) {
        ent_info = ent->info;
        if( ent_info->index == PCH_NULL_INDEX ) {
            ent_info->index = index++;
            OutPutAuxInfo( front, ent_info );      // write out the aux_info struct
        }
    }
    front.pchdr.PH_PragmaCount = index - PCH_FIRST_INDEX;
    front.pchdr.PH_PragmaEntryCount = 0;
    for( ent = front.pragdefn.AuxList; ent != NULL; ent = ent_next ) {
        ent_next = ent->next;                   // save pointer
        ent_info = ent->info;                   // save pointer
        len = offsetof( aux_entry, name ) + strlen( ent->name ) + 1;
        ent->next = static_cast<struct aux_entry *>(PCHSetUInt( PCHAlign( len ) ));
        ent->info = static_cast<aux_info *>(PCHSetUInt( ent_info->index - PCH_FIRST_INDEX ));
        rc = PCHWrite( front, ent, len );
        ent->info = ent_info;                   // restore pointer
        ent->next = ent_next;                   // restore pointer
        if( rc ) {
            longjmp( front.pchdr.PH_jmpbuf, rc );
        }
        front.pchdr.PH_PragmaEntryCount++;
    }
}

static void OutPutMacros( FrontRun& front )
{
    mac_hash_idx    hash;
    bool            rc;
    MEPTR           mentry;
    MEPTR           mentry_next_macro;

    front.pchdr.PH_MacroCount = 0;
    front.pchdr.PH_MacroSize = front.pchdr.PH_size;
    for( hash = 0; hash < MACRO_HASH_SIZE; hash++ ) {
        for( mentry = front.cvars.MacHash[hash]; mentry != NULL; mentry = mentry_next_macro ) {
            mentry_next_macro = mentry->next_macro;        // save pointer
            mentry->next_macro = static_cast<struct macro_entry *>(PCHSetUInt( hash ));
            rc = PCHWrite( front, mentry, mentry->macro_len );
            mentry->next_macro = mentry_next_macro;        // restore pointer
            if( rc ) {
                longjmp( front.pchdr.PH_jmpbuf, rc );
            }
            front.pchdr.PH_MacroCount++;
        }
    }
    /* write out undefined macro list. */
    front.pchdr.PH_UndefMacroCount = 0;
    for( mentry = front.cvars.UndefMacroList; mentry != NULL; mentry = mentry->next_macro ) {
        rc = PCHWrite( front, mentry, mentry->macro_len );
        if( rc ) {
            longjmp( front.pchdr.PH_jmpbuf, rc );
        }
        front.pchdr.PH_UndefMacroCount++;
    }
    front.pchdr.PH_MacroSize = front.pchdr.PH_size - front.pchdr.PH_MacroSize;
}

static void OutPutSymHashTable( FrontRun& front )
{
    SYM_HASHPTR     hsym;
    SYM_HASHPTR     hsym_next_sym;
    TYPEPTR         hsym_sym_typ;
    SYM_HASHPTR     sym_list;
    id_hash_idx     hash;
    bool            rc;
    size_t          len;

    for( hash = 0; hash < ID_HASH_SIZE; hash++ ) {
        // reverse the list
        sym_list = NULL;
        for( hsym = front.cvars.HashTab[hash]; hsym != NULL; hsym = hsym_next_sym ) {
            hsym_next_sym = hsym->next_sym;
            hsym->next_sym = sym_list;
            sym_list = hsym;
            front.pchdr.PH_SymHashCount++;
        }
        front.cvars.HashTab[hash] = NULL;
        rc = false;
        for( hsym = sym_list; hsym != NULL; hsym = hsym_next_sym ) {
            hsym_next_sym = hsym->next_sym;
            hsym->next_sym = static_cast<struct id_hash_entry *>(PCHSetUInt( hash ));
            hsym_sym_typ = hsym->sym_type;               // save type pointer
            if( hsym_sym_typ != NULL ) {
                hsym->sym_type = static_cast<TYPEPTR>(PCHSetUInt( hsym_sym_typ->u1.type_index )); // replace with index
            }
            len = offsetof( id_hash_entry, name ) + strlen( hsym->name ) + 1;
            rc |= PCHWrite( front, hsym, len );
            hsym->sym_type = hsym_sym_typ;               // restore type pointer
            hsym->next_sym = front.cvars.HashTab[hash];
            front.cvars.HashTab[hash] = hsym;
        }
        if( rc ) {
            longjmp( front.pchdr.PH_jmpbuf, rc );
        }
    }
}

static void OutPutSymbols( FrontRun& front )
{
    SYM_ENTRY   sym;
    SYM_HANDLE  sym_handle;
    bool        rc;

    if( front.pchdr.PH_computing_size ) {
        front.pchdr.PH_size += SymGetNumSyms(front) * sizeof( SYM_ENTRY );
    } else {
        for( sym_handle = SymGetFirst(); sym_handle != SYM_INVALID; sym_handle = SymGetNext( front, sym_handle ) ) {
            SymGet( front, &sym, sym_handle );
            if( sym.sym_type != NULL ) {
                sym.sym_type = static_cast<TYPEPTR>(PCHSetUInt( sym.sym_type->u1.type_index ));
            }
            if( sym.seginfo != NULL ) {
                sym.seginfo = static_cast<textsegment *>(PCHSetUInt( sym.seginfo->index ));
            }
            sym.name = NULL; // can't carry a name across
            rc = PCHWriteVar( front, sym );
            if( rc ) {
                longjmp( front.pchdr.PH_jmpbuf, rc );
            }
        }
    }
}

static void OutPutEverything( FrontRun& front )
{
    front.pchdr.PH_SymHashCount = 0;
    front.pchdr.PH_FileCount = 0;
    front.pchdr.PH_RDirCount = 0;
    front.pchdr.PH_IAliasCount = 0;
    front.pchdr.PH_SegCount = 0;
    OutPutIncludes(front);
    OutPutRoDirList(front);
    OutPutIncAliasList(front);
    OutPutIncPathList(front);
    OutPutIncFileList(front);
    OutPutLibraries(front);
    OutPutAliases(front);
    OutPutSegInfo(front);
    OutPutTypes(front);
    OutPutTags(front);
    OutPutPragmaInfo(front);
    OutPutSymHashTable(front);
    OutPutSymbols(front);
    OutPutMsgLevels(front);
    OutPutMacros(front);
}

void InitBuildPreCompiledHeader( FrontRun& front )
//Save any before info when building pre compiled headers
{
    RDIRPTR     start;

    start = front.cvars.RDirNames;
    if( start != NULL ) {
        start = start->next;
    }
    front.pchdr.PCHRDirNames = start;

    front.pchdr.PCHIAliasNames = front.cvars.IAliasNames;
}

void BuildPreCompiledHeader( FrontRun& front, const char *filename )
{
    int         rc;
    char        *cwd;

    if( front.cvars.FirstStmt != 0 || DataQuadsAvailable(front) ) {
        PCHNote( front, PCHDR_NO_OBJECT );
        return;
    }
    InitPHVars(front);
    front.pchdr.PH_Buffer = static_cast<char *>(FEmalloc( front, PH_BUF_SIZE ));
    front.pchdr.PH_BufPtr = front.pchdr.PH_Buffer;
    front.pchdr.PH_BufSize = PH_BUF_SIZE;
    cwd = getcwd( front.pchdr.PH_Buffer + sizeof( pheader ), PH_BUF_SIZE - sizeof( pheader ) );
    rc = setjmp( front.pchdr.PH_jmpbuf );
    if( rc == 0 ) {
        CreatePHeader( front, filename );
        front.pchdr.PH_cwd_len = PCHAlign( strlen( cwd ) + 1 );
        front.pchdr.PH_size = front.pchdr.PH_cwd_len;
        front.pchdr.PH_computing_size = 1;
        OutPutEverything(front);
        front.pchdr.PH_computing_size = 0;
        OutPutHeader(front);
        OutPutEverything(front);
        FlushPHeader(front);
        ClosePHeader(front);
    } else {                            // error creating pre-compiled header
        if( front.pchdr.PH_handle != -1 ) {
            ClosePHeader(front);             // close the file
            remove( filename );         // delete the pre-compiled header
        }
    }
    InitDebugTypes(front);
    InitDebugTags(front);
    FEfree( front, front.pchdr.PH_Buffer );
    front.cvars.PCH_FileName = NULL;
    front.cvars.CompFlags.make_precompiled_header = false;
}

//========================================================================
//      This portion of the code loads the pre-compiled header and
//      rebuilds the data structures by replacing all the indices with
//      pointers, reconstructing hash tables, etc.
//========================================================================

static char *FixupIncFileList( FrontRun& front, char *p, unsigned incfile_count )
{
    INCFILE     *ifile;

    front.pchdr.PCHOldIncFileList = front.cvars.IncFileList;
    front.pchdr.PCHOldIncListValid = true;
    front.cvars.IncFileList = NULL;
    while( incfile_count-- > 0 ) {
        ifile = (INCFILE *)p;
        p += PCHGetUInt( ifile->nextfile );
        AddIncFile( front, ifile );
    }
    return( p );
}

static void RestoreIncFileList( FrontRun& front )
{
    if( front.pchdr.PCHOldIncListValid ) {
        front.cvars.IncFileList = front.pchdr.PCHOldIncFileList;
        front.pchdr.PCHOldIncFileList = NULL;
    }
}

static void FreeOldIncFileList( FrontRun& front )
{
    INCFILE *ilist;

    if( front.pchdr.PCHOldIncListValid ) {
        while( (ilist = front.pchdr.PCHOldIncFileList) != NULL ) {
            front.pchdr.PCHOldIncFileList = ilist->nextfile;
            CMemFree( front, ilist );
        }
        front.pchdr.PCHOldIncFileList = NULL;
        front.pchdr.PCHOldIncListValid = false;
    }
}

static char *FixupIncludes( FrontRun& front, char *p, unsigned file_count )
{
    FNAMEPTR    flist;
    FNAMEPTR    *lnk;

    for( lnk = &front.pchdr.FNameList; file_count-- > 0; lnk = &flist->next ) {
        flist = (FNAMEPTR)p;
        p += PCHGetUInt( flist->next );
        flist->fullpath = NULL;
        *lnk = flist;
    }
    *lnk = NULL;
    return( p );
}

static char *FixupRoDirList( FrontRun& front, char *p, unsigned list_count )
{
    RDIRPTR     dirlist;
    RDIRPTR     *lnk;

    for( lnk = &front.cvars.RDirNames; (dirlist = *lnk) != NULL; ) {
        lnk = &dirlist->next;
    }
    for( ; list_count-- > 0; lnk = &dirlist->next ) {
        dirlist = (RDIRPTR)p;
        p += PCHGetUInt( dirlist->next );
        *lnk = dirlist;
    }
    *lnk = NULL;
    return( p );
}

static char *FixupIncAliasList( FrontRun& front, char *p, unsigned list_count )
{
    IALIASPTR   aliaslist;
    IALIASPTR   *lnk;

    for( lnk = &front.cvars.IAliasNames; list_count-- > 0; lnk = &aliaslist->next ) {
        aliaslist = (IALIASPTR)p;
        p += PCHGetUInt( aliaslist->next );
        aliaslist->real_name = aliaslist->alias_name + PCHGetUInt( aliaslist->real_name );
        *lnk = aliaslist;
    }
    /* Tack pre-existing aliases onto the end (so that they're searched last) */
    *lnk = front.pchdr.PCHIAliasNames;
    return( p );
}

static bool VerifyIncludes( FrontRun& front, const char *filename )
{
    FNAMEPTR    flist;
    time_t      mtime;
    FNAMEPTR    fnew;
    struct stat statbuf;

    // skip the primary source file (first item)
    fnew = front.cvars.FNames->next;
    for( flist = front.pchdr.FNameList; flist != NULL; flist = flist->next ) {
        if( fnew != NULL ) {
            if( FNAMECMPSTR( fnew->name, flist->name ) != 0 ) {
                return( true );
            }
            fnew = fnew->next;
        } else if( filename != NULL ) {
            if( FNAMECMPSTR( filename, flist->name ) != 0 ) {
                return( true );
            }
            filename = NULL;
        }
        if( flist->rwflag ) {
            if( SrcFileInRDir( front, flist ) ) {
                flist->rwflag = false;
            }
        }
        if( flist->rwflag ) {
            mtime = 0;
            if( stat( flist->name, &statbuf ) == 0 ) {
                mtime = statbuf.st_mtime;
            }
            if( flist->mtime != mtime || mtime == 0 ) {
                PCHNote( front, PCHDR_INCFILE_CHANGED, flist->name  );
                return( true );
            }
        }
    }
    return( false );
}

static char *FixupLibraries( FrontRun& front, char *p, unsigned library_count )
{
    library_list    *lib;
    library_list    **lnk;

    for( lnk = &front.cvars.HeadLibs; library_count-- > 0; lnk = &lib->next ) {
        lib = (library_list *)p;
        p += PCHGetUInt( lib->next );
        *lnk = lib;
    }
    *lnk = NULL;
    return( p );
}

static char *FixupAliases( FrontRun& front, char *p, unsigned alias_count )
{
    alias_list  *alias;
    alias_list  **lnk;
    unsigned    len;

    for( lnk = &front.cvars.AliasHead; alias_count-- > 0; lnk = &alias->next ) {
        alias = (alias_list *)p;
        p += PCHGetUInt( alias->next );
        len = PCHGetUInt( alias->name );
        if( len != 0 ) {
            alias->name = alias->names;
        }
        if( PCHGetUInt( alias->subst ) != 0 ) {
            alias->subst = alias->names + len;
        }
        *lnk = alias;
    }
    *lnk = NULL;
    return( p );
}

static char *FixupSegInfo( FrontRun& front, char *p, unsigned seg_count )
{
    textsegment     *tseg;
    textsegment     **lnk;

    front.pchdr.TextSegArray = (textsegment **)CMemAlloc( front, ( seg_count + 1 ) * sizeof( textsegment * ) );
    front.pchdr.TextSegArray[0] = NULL;
    for( lnk = &tseg; seg_count-- > 0; lnk = &tseg->next ) {
        tseg = (textsegment *)p;
        p += PCHGetUInt( tseg->next );
        front.pchdr.TextSegArray[tseg->index + 1] = tseg;
        *lnk = tseg;
    }
    *lnk = NULL;
    return( p );
}

static char *FixupMacros( FrontRun& front, char *p, unsigned macro_count )
{
    int         i;
    MEPTR       mentry;

    while( macro_count-- > 0 ) {
        mentry = (MEPTR)p;
        p += PCHAlign( mentry->macro_len );
        i = PCHGetUInt( mentry->next_macro );       // get hash index
        mentry->next_macro = front.pchdr.PCHMacroHash[i];
        front.pchdr.PCHMacroHash[i] = mentry;
    }
    return( p );
}

static char *FixupUndefMacros( FrontRun& front, char *p, unsigned undef_macro_count )
{   // Read in as written out
    MEPTR       mentry;
    MEPTR       *lnk;

    for( lnk = &front.pchdr.PCHUndefMacroList; undef_macro_count-- > 0; lnk = &mentry->next_macro ) {
        mentry = (MEPTR)p;
        p += PCHAlign( mentry->macro_len );
        *lnk = mentry;
    }
    *lnk = NULL;
    return( p );
}

static int VerifyMacros( FrontRun& front, char *p, unsigned macro_count, unsigned undef_count )
{
    mac_hash_idx    hash;
    MEPTR           mpch;
    MEPTR           mcur;
    bool            macro_compare;

    front.pchdr.PCHMacroHash = (MEPTR *)CMemAlloc( front, MACRO_HASH_SIZE * sizeof( MEPTR ) );
    p = FixupMacros( front, p, macro_count );
    p = FixupUndefMacros( front, p, undef_count );
    for( hash = 0; hash < MACRO_HASH_SIZE; hash++ ) {
        MEPTR       prev_mpch;

        prev_mpch = NULL;
        for( mpch = front.pchdr.PCHMacroHash[hash]; mpch != NULL; mpch = mpch->next_macro ) {
            if( mpch->macro_flags & MFLAG_DEFINED_BEFORE_FIRST_INCLUDE ) {
                for( mcur = front.cvars.MacHash[hash]; mcur != NULL; mcur = mcur->next_macro ) {
                    if( strcmp( mcur->macro_name, mpch->macro_name ) == 0 ) {
                        macro_compare = MacroCompare( mpch, mcur );
                        if( mpch->macro_flags & MFLAG_REFERENCED ) {
                            if( macro_compare ) {
                                return( -1 );           /* abort: macros different */
                            }
                        } else if( macro_compare ) {    /* if different */
                            /* delete macro from pch, add new one */
                            if( prev_mpch == NULL ) {
                                front.pchdr.PCHMacroHash[hash] = mpch->next_macro;
                            } else {
                                prev_mpch->next_macro = mpch->next_macro;
                            }
                        }
                        break;
                    }
                }
                if( mcur == NULL ) {  /* macro not found in current compile */
                    if( mpch->macro_flags & MFLAG_REFERENCED ) {
                        return( -1 );   // abort: macro definition required
                    }
                    // delete macro from PCH list
                    if( prev_mpch == NULL ) {
                        front.pchdr.PCHMacroHash[hash] = mpch->next_macro;
                    } else {
                        prev_mpch->next_macro = mpch->next_macro;
                    }
                }
            }
            prev_mpch = mpch;
        }
    }
    // check if any user-defined macros have been defined that were not
    // defined when the pre-compiled header file was created
    //
    // for all macros in current compilation unit
    // - if it is present in the PCH header file set of macros
    // -- if not defined before first include
    // --- abort:
    // -- endif
    // - else macro NOT in PCH
    // -- if the macro is a user-defined macro
    // --- abort:
    // -- endif
    // - endif
    // endloop
    for( hash = 0; hash < MACRO_HASH_SIZE; hash++ ) {
        for( mcur = front.cvars.MacHash[hash]; mcur != NULL; mcur = mcur->next_macro ) {
            for( mpch = front.pchdr.PCHMacroHash[hash]; mpch != NULL; mpch = mpch->next_macro ) {
                if( strcmp( mpch->macro_name, mcur->macro_name ) == 0 ) {
                    break;
                }
            }
            if( mpch == NULL || (mpch->macro_flags & MFLAG_DEFINED_BEFORE_FIRST_INCLUDE) == 0 ) {
            // macro may either have been undef'd (mpch == NULL ) or undef'd and defined
                if( mcur->macro_flags & MFLAG_USER_DEFINED ) {  //compiler defined macros not saved on undefs
                    for( mpch = front.pchdr.PCHUndefMacroList; mpch != NULL; mpch = mpch->next_macro ) {
                        if( strcmp( mpch->macro_name, mcur->macro_name ) == 0 ) {
                            if( MacroCompare( mpch, mcur ) ) {
                                return( -1 );
                            } else {
                                break;
                            }
                        }
                    }
                    if( mpch == NULL ) {
                        // abort: macro wasn't defined before include
                       return( -1 );
                    }
                }
            }
        }
    }

    for( hash = 0; hash < MACRO_HASH_SIZE; hash++ ) {
        MEPTR       next_mcur;

        for( mcur = front.cvars.MacHash[hash]; mcur != NULL; mcur = next_mcur ) {
            for( mpch = front.pchdr.PCHMacroHash[hash]; mpch != NULL; mpch = mpch->next_macro ) {
                if( strcmp( mpch->macro_name, mcur->macro_name ) == 0 ) {
                    break;
                }
            }
            next_mcur = mcur->next_macro;
            if( mpch == NULL ) {                        // if this macro not found in PCH
                mcur->next_macro = front.pchdr.PCHMacroHash[hash];  // add it to PCH
                front.pchdr.PCHMacroHash[hash] = mcur;
            }
        }
    }
    memcpy( front.cvars.MacHash, front.pchdr.PCHMacroHash, MACRO_HASH_SIZE * sizeof( MEPTR ) );
    CMemFree( front, front.pchdr.PCHMacroHash );
    front.pchdr.PCHMacroHash = NULL;
    front.cvars.UndefMacroList = front.pchdr.PCHUndefMacroList;
    return( 0 );
}

static char *FixupSymHashTable( FrontRun& front, char *p, unsigned symhash_count )
{
    SYM_HASHPTR hsym;
    int         index;

    while( symhash_count-- > 0 ) {
        hsym = (SYM_HASHPTR)p;
        p += PCHAlign( offsetof( id_hash_entry, name ) + strlen( hsym->name ) + 1 );
        index = PCHGetUInt( hsym->next_sym );
        hsym->next_sym = front.cvars.HashTab[index];
        front.cvars.HashTab[index] = hsym;
        if( PCHGetUInt( hsym->sym_type ) != 0 ) {
            hsym->sym_type = front.pchdr.TypeArray + PCHGetUInt( hsym->sym_type );
        }
    }
    return( p );
}

static char *FixupSymbols( FrontRun& front, char *p, unsigned symbol_count )
{
    SYMPTR      symptr;
    SYM_ENTRY   sym;
    unsigned    handle;     // TODO: don't cheat!

    for( handle = 0; handle < symbol_count; handle++ ) {
        symptr = (SYMPTR)p;
        p += sizeof( SYM_ENTRY );
        if( PCHGetUInt( symptr->sym_type ) != 0 ) {
            symptr->sym_type = front.pchdr.TypeArray + PCHGetUInt( symptr->sym_type );
        }
        symptr->seginfo = front.pchdr.TextSegArray[PCHGetUInt( symptr->seginfo )];
        front.cvars.PCH_SymArray[handle] = symptr;
    }
    for( handle = 0; handle < (unsigned)(pointer_uint)front.cvars.SpecialSyms; handle++ ) {
        SymGet( front, &sym, (SYM_HANDLE)(pointer_uint)handle );  // Redo special syms
        symptr = front.cvars.PCH_SymArray[handle];
        *symptr = sym;
    }
    return( p );
}

static void FixupTypeIndexes( FrontRun& front, type_indices *typ_index )
{
    DATA_TYPE   i;
    int         index;

    for( i = static_cast<DATA_TYPE>(0); i < DATA_TYPE_SIZE; i++ ) {
        index = typ_index->basetype_index[i];
        if( index != 0 ) {
            front.cvars.BaseTypes[i] = front.pchdr.TypeArray + index;
        }
    }
    front.cvars.VoidParmList[0] = front.cvars.BaseTypes[TYP_VOID];
    front.cvars.StringType = front.pchdr.TypeArray + typ_index->stringtype_index;
    front.cvars.ConstCharType = front.pchdr.TypeArray + typ_index->constchartype_index;
}

static char *FixupTypes( FrontRun& front, char *p, unsigned type_count )
{
    TYPEPTR         typ;
    parm_hash_idx   hash;
    array_info      *array;
    TYPEPTR         *parm_types;
    int             idx;

    InitTypeHashTables(front);
    typ = (TYPEPTR)p;
    front.pchdr.TypeArray = typ - 1;
    while( type_count-- > 0 ) {
        if( typ->decl_type == TYP_FUNCTION )
            break;
        if( PCHGetUInt( typ->object ) != 0 ) {
            typ->object = front.pchdr.TypeArray + PCHGetUInt( typ->object );
        }
        if( typ->decl_type == TYP_ARRAY ) {
            array = (array_info *)CMemAlloc( front, sizeof( array_info ) );
            array->dimension = PCHGetUInt( typ->u.array );
            array->unspecified_dim = ( array->dimension == 0 );
            typ->u.array = array;
        }
        AddTypeHash( front, typ );
        typ++;
    }
    parm_types = (TYPEPTR *)( typ + type_count );
    while( type_count-- > 0 ) {
        hash = (parm_hash_idx)PCHGetUInt( typ->u.fn.parms );
        typ->next_type = front.cvars.FuncTypeHead[hash];
        front.cvars.FuncTypeHead[hash] = typ;
        if( PCHGetUInt( typ->object ) != 0 ) {
            typ->object = front.pchdr.TypeArray + PCHGetUInt( typ->object );
        }
        if( PCHGetUInt( *parm_types ) == -1 ) {
            typ->u.fn.parms = NULL;
        } else {
            typ->u.fn.parms = parm_types;
            while( (idx = PCHGetUInt( *parm_types )) != -1 ) {
                *parm_types++ = front.pchdr.TypeArray + idx;
            }
        }
        *parm_types++ = NULL;
        typ++;
    }
    FixupTypeIndexes( front, (type_indices *)parm_types );
    return( (char *)parm_types + sizeof( type_indices ) );
}

static char *FixupEnums( FrontRun& front, char *p, TAGPTR parent )
{
    ENUMPTR     ep;
    ENUMPTR     *lnk;

    lnk = &ep;
    do {
        ep = (ENUMPTR)p;
        p += PCHGetUInt( ep->parent );
        ep->parent = parent;            // parent is union'ed with enum_len
        ep->next_enum = front.cvars.EnumTable[ep->hash];
        front.cvars.EnumTable[ep->hash] = ep;
        *lnk = ep;
        lnk = &ep->thread;
    } while( ep->thread != NULL );
    *lnk = NULL;
    return( p );
}

static char *FixupFields( FrontRun& front, char *p )
{
    FIELDPTR    field;
    FIELDPTR    *lnk;

    lnk = &field;
    do {
        field = (FIELDPTR)p;
        p += PCHGetUInt( field->next_field_same_hash );
        field->next_field_same_hash = NULL;
        field->field_type = front.pchdr.TypeArray + PCHGetUInt( field->field_type );
        *lnk = field;
        lnk = &field->next_field;
    } while( field->next_field != NULL );
    *lnk = NULL;
    return( p );
}

static void FixupTag( FrontRun& front, TYPEPTR typ )
{
    switch( typ->decl_type ) {
    case TYP_STRUCT:
    case TYP_UNION:
    case TYP_ENUM:
        typ->u.tag = front.pchdr.TagArray[PCHGetUInt( typ->u.tag )];
        break;
    default:
        break;
    }
}

static void FixupTagPointers( FrontRun& front )
{
    WalkTypeList( front, FixupTag );
}

static char *FixupTags( FrontRun& front, char *p, unsigned tag_count )
{
    TAGPTR      tag;
    TAGPTR      prevtag;
    TAGPTR      nexttag;
    TYPEPTR     typ;

    if( tag_count != 0 ) {
        front.pchdr.TagArray = (TAGPTR *)CMemAlloc( front, tag_count * sizeof( TAGPTR ) );
    }
    tag = NULL;
    for( prevtag = NULL; tag_count-- > 0; prevtag = tag ) {
        tag = (TAGPTR)p;
        p += PCHGetUInt( tag->next_tag );
        front.pchdr.TagArray[tag->refno] = tag;
        typ = front.pchdr.TypeArray + PCHGetUInt( tag->sym_type );
        tag->sym_type = typ;
        if( typ->decl_type == TYP_ENUM ) {
            if( tag->u.enum_list != NULL ) {
                tag->u.enum_list = (ENUMPTR)p;
                p = FixupEnums( front, p, tag );
            }
        } else {
            if( tag->u.field_list != NULL ) {
                tag->u.field_list = (FIELDPTR)p;
                p = FixupFields( front, p );
            }
        }
        tag->next_tag = prevtag;
    }
    for( tag = prevtag; tag != NULL; tag = nexttag ) {
        nexttag = tag->next_tag;
        tag->next_tag = front.cvars.TagHash[tag->hash];
        front.cvars.TagHash[tag->hash] = tag;
    }
    FixupTagPointers(front);
    return( p );
}

static char *FixupAuxInfo( char *p, aux_info *info )
{
    unsigned            len;

    p += sizeof( aux_info );
    len = PCHGetUInt( info->parms );
    if( len != 0 ) {
        info->parms = (hw_reg_set *)p;
        p += len;
    }
    len = PCHGetUInt( info->objname );
    if( len != 0 ) {
        info->objname = (char *)p;
        p += len;
    }
    len = PCHGetUInt( info->code );
    if( len != 0 ) {
        info->code = (byte_seq *)p;
        p += len;
    }
    return( p );
}

static char *FixupPragmaInfo( FrontRun& front, char *p, unsigned pragma_count, unsigned entry_count )
{
    aux_entry       *ent;
    aux_entry       **lnk;
    aux_info        *info;
    aux_info        **info_array;
    unsigned        index;

    info_array = (aux_info **)CMemAlloc( front, pragma_count * sizeof( aux_info * ) );
    for( index = 0; index < pragma_count; index++ ) {
        if( index < PCH_FIRST_USER_INDEX - PCH_FIRST_INDEX ) {
            info = front.callinfo.BuiltinAuxInfo + index;
            memcpy( info, p, sizeof( aux_info ) );
        } else {
            info = (aux_info *)p;
        }
        info_array[index] = info;
        p = FixupAuxInfo( p, info );
    }
    for( lnk = &front.pragdefn.AuxList; entry_count-- > 0; lnk = &ent->next ) {
        ent = (aux_entry *)p;
        p += PCHGetUInt( ent->next );
        ent->info = info_array[PCHGetUInt( ent->info )];
        *lnk = ent;
    }
    *lnk = NULL;
    CMemFree( front, info_array );
    return( p );
}

static char *FixupMsgLevels( FrontRun& front, char *p, unsigned len )
{
    memcpy( front.cerrs.msg_level, p, len );
    p += PCHAlign( len );
    return( p );
}

static void FixupFNames( FrontRun& front )
{
    FNAMEPTR    *lnk;
    FNAMEPTR    flist;

    for( lnk = &front.cvars.FNames; (flist = *lnk) != NULL; lnk = &flist->next ) {
        if( FNAMECMPSTR( flist->name, front.pchdr.FNameList->name ) == 0 ) {
            break;
        }
    }
    *lnk = front.pchdr.FNameList;
    for( flist = front.pchdr.FNameList; flist != NULL; flist = flist->next ) {
        flist->index_db = DBFILE_INVALID;
    }
}

static bool ValidHeader( FrontRun& front, pheader *pch )
{
    if( (pch->signature == PCH_SIGNATURE)
      && (pch->version == PCH_VERSION_HOST)
      && (pch->size_of_header == sizeof( pheader ))
      && (pch->size_of_int == TARGET_INT)
      && (pch->specialsyms_count == SymGetNumSpecialSyms(front))
      && (pch->pack_amount == front.cvars.PackAmount)
      && (pch->msglevel_len == sizeof( front.cerrs.msg_level )) ) {
        return( true );
    }
    return( false );                // indicate unusable pre-compiled header
}

static int LoadPreCompiledHeader( FrontRun& front, char *p, pheader *pch )
{
    int         rc;

    rc = FixupDataStructures( front, p, pch );
    CMemFree( front, front.pchdr.TagArray );
    CMemFree( front, front.pchdr.TextSegArray );
    CMemFree( front, front.pchdr.PCHMacroHash );
    return( rc );
}

static int FixupDataStructures( FrontRun& front, char *p, pheader *pch )
{
    p = FixupLibraries( front, p, pch->library_count );
    p = FixupAliases( front, p, pch->alias_count );
    p = FixupSegInfo( front, p, pch->seg_count );
    p = FixupTypes( front, p, pch->type_count );
    p = FixupTags( front, p, pch->tag_count );
    p = FixupPragmaInfo( front, p, pch->pragma_count, pch->pragma_entry_count );
    p = FixupSymHashTable( front, p, pch->symhash_count );
    p = FixupSymbols( front, p, pch->symbol_count );
    p = FixupMsgLevels( front, p, pch->msglevel_len );

    front.cvars.PCH_MaxSymHandle = pch->symbol_count;
    SetNextSymHandle( front, pch->symbol_count - 1 );
    front.cvars.IncLineCount = pch->incline_count;
    front.toggles.PragmaToggles = pch->toggles;
    FixupFNames(front);
    InitDebugTypes(front);
    InitDebugTags(front);
    return( 0 );
}

static bool SameCWD( const char *p )
{
    char        *cwd;
    int         same;
    char        buf[_MAX_PATH];

    cwd = getcwd( buf, _MAX_PATH );
    same = FNAMECMPSTR( cwd, p );
    return( same == 0 );
}

void FreePreCompiledHeader( FrontRun& front )
{
    FEfree( front, front.cvars.PCH_Start );
    FEfree( front, front.cvars.PCH_Macros );
    FEfree( front, front.cvars.PCH_SymArray );
}

static void AbortPreCompiledHeader( FrontRun& front )
{
    FreePreCompiledHeader(front);
    CMemFree( front, front.pchdr.TagArray );
    CMemFree( front, front.pchdr.TextSegArray );
    CMemFree( front, front.pchdr.PCHMacroHash );
    front.cvars.PCH_Start = NULL;
    front.cvars.PCH_End = NULL;
    front.cvars.PCH_Macros = NULL;
    front.cvars.PCH_SymArray = NULL;
    front.cvars.PCH_MaxSymHandle = 0;
    front.pchdr.TagArray = NULL;
    front.pchdr.TextSegArray = NULL;
    front.pchdr.FNameList = NULL;
    front.pchdr.PCHMacroHash = NULL;
    front.cvars.IAliasNames = front.pchdr.PCHIAliasNames;
    RestoreIncFileList(front);
    front.cvars.CompFlags.make_precompiled_header = true;   // force new PCH to be created
}

//========================================================================
//      This portion of the code checks to see if we can use the
//      existing pre-compiled header.
//      - all the predefined macros have the same definition
//      - all the include files are the same and have not been modified
//========================================================================

bool UsePreCompiledHeader( FrontRun& front, const char *filename )
{
    int                 handle;
    unsigned            size;
    size_t              len;
    char                *p;
    pheader             pch;

    handle = _sopen3( front.cvars.PCH_FileName, O_RDONLY | O_BINARY, SH_DENYWR );
    if( handle == -1 ) {
        front.cvars.CompFlags.make_precompiled_header = true;
        return( false );
    }
    front.cvars.PCH_Start = NULL;
    front.pchdr.TextSegArray = NULL;
    front.pchdr.TagArray = NULL;
    front.pchdr.PCHMacroHash = NULL;
    front.pchdr.PCHOldIncFileList = NULL;
    front.pchdr.PCHOldIncListValid = false;
    size = read( handle, &pch, sizeof( pheader ) );
    if( size != sizeof( pheader ) ) {
        close( handle );
        PCHNote( front, PCHDR_READ_ERROR );
        AbortPreCompiledHeader(front);
        return( false );
    }
    if( !ValidHeader( front, &pch ) ) {
        close( handle );
        PCHNote( front, PCHDR_INVALID_HEADER );
        AbortPreCompiledHeader(front);
        return( false );
    }
    if( pch.gen_switches != front.cvars.GenSwitches || pch.target_switches != front.cvars.TargetSwitches ) {
        close( handle );
        PCHNote( front, PCHDR_DIFFERENT_OPTIONS );
        AbortPreCompiledHeader(front);
        return( false );
    }
    p = static_cast<char *>(FEmalloc( front, pch.size ));                   // allocate big memory block
    front.cvars.PCH_Start = p;
    front.cvars.PCH_End = p + pch.size;
    front.pchdr.PH_size = read( handle, p, pch.size );      // read rest of the file
    front.cvars.PCH_Macros = static_cast<char *>(FEmalloc( front, pch.macro_size ));
    size = read( handle, front.cvars.PCH_Macros, pch.macro_size );
    close( handle );
    front.cvars.PCH_SymArray = (SYMPTR *)FEmalloc( front, pch.symbol_count * sizeof( SYMPTR ) );
    if( front.pchdr.PH_size != pch.size || size != pch.macro_size ) {
        PCHNote( front, PCHDR_READ_ERROR );
        AbortPreCompiledHeader(front);
        return( false );
    }
    if( !SameCWD( p ) ) {
        PCHNote( front, PCHDR_DIFFERENT_CWD );
        AbortPreCompiledHeader(front);
        return( false );
    }
    if( front.cvars.CompFlags.cpp_ignore_env != pch.cpp_ignore_env || front.cvars.CompFlags.ignore_default_dirs != pch.ignore_default_dirs ) {
        PCHNote( front, PCHDR_INCFILE_DIFFERENT );
        AbortPreCompiledHeader(front);
        return( false );
    }
    p = FixupIncludes( front, p + pch.cwd_len, pch.file_count );
    p = FixupRoDirList( front, p, pch.rdir_count );
    p = FixupIncAliasList( front, p, pch.ialias_count );
    if( VerifyIncludes( front, filename ) ) {
        PCHNote( front, PCHDR_INCFILE_DIFFERENT );
        AbortPreCompiledHeader(front);
        return( false );
    }
    len = strlen( p ) + 1;              // get length of saved IncPathList
    if( FNAMECMPSTR( p, front.cvars.IncPathList ) != 0 ) {
        PCHNote( front, PCHDR_INCPATH_CHANGED );
        AbortPreCompiledHeader(front);
        return( false );
    }
    p += PCHAlign( len );
    p = FixupIncFileList( front, p, pch.incfile_count );
    if( VerifyMacros( front, front.cvars.PCH_Macros, pch.macro_count, pch.undef_macro_count ) != 0 ) {
        PCHNote( front, PCHDR_MACRO_CHANGED );
        AbortPreCompiledHeader(front);
        return( false );
    }
    LoadPreCompiledHeader( front, p, &pch );
    FreeOldIncFileList(front);
    front.cvars.PCH_FileName = NULL;
    return( true );
}

static void SetDebugType( FrontRun& front, TYPEPTR typ )
{
    typ->u1.debug_type = DBG_NIL_TYPE;
}

static void SetFuncDebugType( FrontRun& front, TYPEPTR typ, int index )
{
    /* unused parameters */ (void)index;

    typ->u1.debug_type = DBG_NIL_TYPE;
}

void InitDebugTypes( FrontRun& front )
{
    WalkTypeList( front, SetDebugType );
    WalkFuncTypeList( front, SetFuncDebugType );
}
