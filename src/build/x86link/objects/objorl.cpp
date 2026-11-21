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
* Description:  Object file processing routines specific to ORL.
*
****************************************************************************/



#include <string.h>
#include <array>
#include <string>
#include <string_view>
#include <vector>
#include "wio.h"
#include "build/x86link/link/linkstd.h"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state */
#include "build/x86link/link/specials.h"
#include "build/x86link/fixups/obj2supp.h"
#include "build/x86link/objects/objnode.h"
#include "build/x86link/base/objcache.h"
#include "build/x86link/objects/objio.h"
#include "build/x86link/directives/cmdline.h"
#include "build/x86link/debuginfo/dbgall.h"
#include "build/x86link/symbols/objpass1.h"
#include "build/x86link/symbols/objpass2.h"
#include "build/x86link/objects/objorl.h"
#include "build/x86link/base/strtab.h"
#include "build/x86link/base/carve.h"
#include "build/x86link/symbols/wcomdef.h"
#include "build/x86link/link/permdata.h"
#include "build/x86link/symbols/impexp.h"
#include "build/x86link/base/virtmem.h"
#include "build/x86link/formats/output.h"
#include "build/x86link/objects/objstrip.h"
#include "build/x86link/fixups/toc.h"
#include "i64.h"
#include "build/x86link/link/run_state.hpp"

#include <unordered_map>

#include "clibext.h"


struct orl_io_struct {
    const file_list *list;
    Link        *link;
};

namespace {

// ORL's allocator: wlink's MemAlloc, for the link ORL reads for
struct ORLAllocator {
    Link        *link = nullptr;
    void *operator()( size_t size ) const { return MemAlloc( *link, size ); }
};

// what objorl.cpp keeps for one run of the linker
struct ORLState {
    ORLAllocator            alloc;
    orl_handle              handle = nullptr;
    std::unordered_map<const file_list *, orl_io_struct> io;  // each file ORL reads: its handle (the address stays)
    long                    filePos = 0;
    long                    pos = 0;
    ORL_STRUCT( orl_reloc ) savedReloc{};
    char                    *impExternalName = nullptr;     // inside the section contents ORL holds
    char                    *impModName = nullptr;          // wlink's memory (MemAllocSafe / FileName), freed by MemFree
    const char              *firstCodeSymName = nullptr;
    const char              *firstDataSymName = nullptr;
    ordinal_t               impOrdinal = 0;
    std::vector<void *>     readCache;                      // data ORLRead handed out, given back by ClearCachedData
};

ORLState &orlState()
{
    return cb_wlink::RunState::part<ORLState>();
}

orl_io_struct *orlIo( Link& link, const file_list *file )
{
    return &orlState().io.try_emplace( file, orl_io_struct{ file, &link } ).first->second;
}

void *ORLRead( orl_io_struct *orlio, size_t len )
{
    Link& link = *orlio->link;
    ORLState &st = orlState();
    void *result = CachePermRead(link,  orlio->list, st.filePos + st.pos, len );

    st.pos += static_cast<long>( len );
    st.readCache.push_back( result );
    return result;
}

int ORLSeek( orl_io_struct *orlio, long pos, int where )
{
    ORLState &st = orlState();

    if( where == SEEK_SET ) {
        st.pos = pos;
    } else if( where == SEEK_CUR ) {
        st.pos += pos;
    } else {
        st.pos = orlio->list->infile->len - st.filePos - pos;
    }
    return 0;
}

}

void InitORLObj( Link& link )
/***************************/
{
    ORLState &st = orlState();

    st.alloc.link = &link;
    ORLSetFuncs( orl_cli_funcs, ORLRead, ORLSeek, st.alloc, MemFree );

    st.handle = ORLInit( &orl_cli_funcs );
    st.readCache.clear();
}

void FiniORLObj( void )
/*********************/
{
    ORLFini( orlState().handle );
}

namespace {

long ORLFileSeek( const file_list *file, long pos, int where )
{
    ORLState &st = orlState();

    if( where == SEEK_SET ) {
        st.filePos = pos;
        st.pos = 0;
    } else if( where == SEEK_CUR ) {
        st.filePos += pos;
    } else {
        st.filePos = file->infile->len - pos;
    }
    return st.filePos + st.pos;
}

orl_file_handle InitFile(Link& link)
{
    ORLState &st = orlState();
    orl_file_format     type;

    st.impExternalName = nullptr;
    st.impModName = nullptr;
    st.impOrdinal = 0;
    st.firstCodeSymName = nullptr;
    st.firstDataSymName = nullptr;
    if( link.ObjFileFormat == FILE_FMT_ELF ) {
        type = ORL_ELF;
    } else {
        type = ORL_COFF;
    }
    return ORLFileInit( st.handle, orlIo( link, link.CurrMod->u1.source ), type );
}

void ClearCachedData( const file_list *file )
{
    ORLState &st = orlState();

    while( !st.readCache.empty() ) {
        void *data = st.readCache.back();

        st.readCache.pop_back();
        CacheFree( file, data );
    }
}

}

orl_file_format FileTypeORL( Link& link, const file_list *file, unsigned long loc )
/**********************************************************************
 * return true if this is can be handled by ORL
 */
{
    orl_file_format     type;

    ORLFileSeek( file, loc, SEEK_SET );
    type = ORLFileIdentify( orlState().handle, orlIo( link, file ) );
    ClearCachedData( file );
    return type;
}

namespace {

void FiniFile( orl_file_handle filehdl, const file_list *file )
{
    ORLState &st = orlState();

    ORLFileFini( filehdl );
    ClearCachedData( file );
    if( st.impModName != nullptr ) {
        MemFree( st.impModName );
        st.impModName = nullptr;
    }
}

}

void ORLSkipObj( Link& link, const file_list *file, unsigned long *loc )
/***********************************************************
 * skip the object file.
 * NYI: add an entry point in ORL for a more efficient way of doing this.
 */
{
    orl_file_handle     filehdl;

    ORLFileSeek( file, *loc, SEEK_SET );
    filehdl = InitFile(link);               /* assumes that entire file is read! */
    *loc = ORLFileSeek( file, 0, SEEK_CUR );
    FiniFile( filehdl, file );
}

namespace {

bool CheckFlags(Link& link,  orl_file_handle filehdl )
{
    orl_machine_type    machtype;
    stateflag           typemask;
    stateflag           test;
    orl_file_flags      flags;

    machtype = ORLFileGetMachineType( filehdl );
    switch( machtype ) {
    case ORL_MACHINE_TYPE_I386:
        typemask = LS_HAVE_X86_CODE;
        break;
    case ORL_MACHINE_TYPE_AMD64:
        typemask = LS_HAVE_X64_CODE;
        break;
    case ORL_MACHINE_TYPE_ALPHA:
        typemask = LS_HAVE_ALPHA_CODE;
        break;
    case ORL_MACHINE_TYPE_PPC601:
        typemask = LS_HAVE_PPC_CODE;
        break;
    case ORL_MACHINE_TYPE_R3000:
    case ORL_MACHINE_TYPE_R4000: /* TODO */
        typemask = LS_HAVE_MIPS_CODE;
        break;
    case ORL_MACHINE_TYPE_NONE:
        typemask = static_cast<stateflag>( 0 );
        break;
    default:
        typemask = LS_HAVE_MACHTYPE_MASK;   /* trigger the error */
        break;
    }
    test = (typemask | link.LinkState) & LS_HAVE_MACHTYPE_MASK;
    test &= test - 1;           /* turn off one bit */
    if( test != 0 ) {           /* multiple bits were turned on. */
        LnkMsg(link,  WRN+MSG_MACHTYPE_DIFFERENT, "s", link.CurrMod->u1.source->infile->name.u.ptr );
    } else {
        link.LinkState |= typemask;
    }
    if( ORLFileGetType( filehdl ) != ORL_FILE_TYPE_OBJECT ) {
        BadObject(link);
        return false;
    }
    flags = ORLFileGetFlags( filehdl );
    if( flags & ORL_FILE_FLAG_64BIT_MACHINE ) {
        Set64BitMode(link);
    } else if( flags & ORL_FILE_FLAG_16BIT_MACHINE ) {
        Set16BitMode(link);
    } else {
        Set32BitMode(link);
    }
    return true;
}

// section type is ignored
orl_return NullFunc( Link&, orl_sec_handle )
{
    return ORL_OKAY;
}

orl_return ExportCallback( const char *name, void *cookie )
{
    Link& link = *static_cast<Link *>( cookie );
    length_name lname;

    lname.name = name;
    lname.len = strlen( name );
    HandleExport(link,  &lname, &lname, 0, 0 );
    return ORL_OKAY;
}

orl_return EntryCallback( const char *name, void *cookie )
{
    Link& link = *static_cast<Link *>( cookie );
    if( !link.StartInfo.user_specd ) {
        if( link.ObjFileFormat == FILE_FMT_COFF
          && (link.LinkState & LS_HAVE_X86_CODE) ) {
            /*
             * simple hack for 32-bit MS COFF module to get real startup symbol name
             * entry directive contains unmangled symbol name
             * prepend undrescore should be OK
             */
            const std::string coff_symbol_name = std::string( "_" ) + name;

            SetStartSym(link,  coff_symbol_name.c_str() );
        } else {
            SetStartSym(link,  name );
        }
    }
    return ORL_OKAY;
}

orl_return DeflibCallback( const char *name, void *cookie )
{
    Link& link = *static_cast<Link *>( cookie );
    AddCommentLib(link,  name, strlen( name ), static_cast<lib_priority>( LIB_PRIORITY_MAX - 2 ) );
    return ORL_OKAY;
}

// handle extra object file information records
orl_return P1Note( Link& link, orl_sec_handle sec )
{
    orl_note_callbacks cb;

    cb.export_fn = ExportCallback;
    cb.deflib_fn = DeflibCallback;
    cb.entry_fn = EntryCallback;
    cb.scantab_fn = nullptr;
    ORLNoteSecScan( sec, &cb, &link );
    return ORL_OKAY;
}

// NYI
orl_return Unsupported( Link&, orl_sec_handle )
{
    return ORL_OKAY;
}

void AllocSeg(Link& link,  void *_snode, void * )
{
    segnode             *snode = static_cast<segnode *>( _snode );
    segdata             *sdata;
    const char          *clname;
    const char          *sname;
    group_entry         *group;
    bool                isdbi;

    sdata = snode->entry;
    if( sdata == nullptr )
        return;
    sname = sdata->u.name.u.ptr;
    if( link.CurrMod->flags_mod & MOD_IMPORT_LIB ) {
        if( sdata->isidata
          || sdata->iscode ) {
            if( sdata->iscode ) {
                snode->info |= SEGINF_CODE;
            }
            snode->info |= SEGINF_DEAD;
            snode->entry = nullptr;
            FreeSegData(link,  sdata );
            return;
        }
    }
    isdbi = false;
    if( strnicmp( CoffDebugPrefix, sdata->u.name.u.ptr, sizeof( CoffDebugPrefix ) - 1 ) == 0 ) {
        if( link.CurrMod->flags_mod & MOD_IMPORT_LIB ) {
            snode->info |= SEGINF_DEAD;
            snode->entry = nullptr;
            FreeSegData(link,  sdata );
            return;
        }
        isdbi = true;
        if( stricmp( CoffDebugSymName, sdata->u.name.u.ptr ) == 0 ) {
            clname = _MSLocalClass;
        } else if( stricmp( CoffDebugTypeName, sdata->u.name.u.ptr ) == 0 ) {
            clname = _MSTypeClass;
        } else {
            clname = _DwarfClass;
        }
    } else if( strnicmp( TLSSegPrefix, sdata->u.name.u.ptr, sizeof( TLSSegPrefix ) - 1 ) == 0 ) {
        clname = TLSClassName;
    } else if( sdata->iscode ) {
        clname = CodeClassName;
    } else if( sdata->isuninit ) {
        clname = BSSClassName;
    } else {
        clname = DataClassName;
        if( memcmp( sname, CoffPDataSegName, sizeof( CoffPDataSegName ) ) == 0 ) {
            sdata->ispdata = true;
        } else if( memcmp( sname, CoffReldataSegName, sizeof( CoffReldataSegName ) ) == 0 ) {
            sdata->isreldata = true;
        }
    }
    AllocateSegment(link,  snode, clname );
    if( clname == TLSClassName ) {
        group = GetGroup(link,  TLSGrpName );
        AddToGroup(link,  group, snode->entry->u.leader );
    } else if( !sdata->iscode
      && !isdbi ) {
        group = GetGroup(link,  DataGrpName );
        AddToGroup(link,  group, snode->entry->u.leader );
    }
    if( sdata->isuninit ) {
        snode->contents = nullptr;
    } else {
        snode->entry->u.leader->info |= SEGINF_LXDATA_SEEN;
        if( !sdata->isdead ) {
            ORLSecGetContents( snode->handle, &snode->contents );
            if( !sdata->iscdat
              && ( snode->contents != nullptr ) ) {
                PutInfo( sdata->u1.vm_ptr, snode->contents, sdata->length );
            }
        }
    }
}

void DefNosymComdats(Link& link,  void *_snode, void * )
{
    segnode             *snode = static_cast<segnode *>( _snode );
    segdata             *sdata;

    sdata = snode->entry;
    if( sdata == nullptr
      || (snode->info & SEGINF_DEAD) )
        return;
    if( sdata->iscdat
      && !sdata->hascdatsym
      && ( snode->contents != nullptr )) {
        sdata->u1.vm_ptr = AllocStg(link,  sdata->length );
        PutInfo( sdata->u1.vm_ptr, snode->contents, sdata->length );
    }
}

// declare the "segment"
orl_return DeclareSegment( Link& link, orl_sec_handle sec )
{
    ORLState                &st = orlState();
    segdata                 *sdata;
    segnode                 *snode;
    const char              *name;
    unsigned char           *contents;
    size_t                  len;
    orl_sec_flags           flags;
    orl_sec_type            type;
    size_t                  numlines;
    unsigned                segidx;

    type = ORLSecGetType( sec );
    if( type != ORL_SEC_TYPE_NO_BITS
      && type != ORL_SEC_TYPE_PROG_BITS ) {
         return ORL_OKAY;
    }
    flags = ORLSecGetFlags( sec );
    name = ORLSecGetName( sec );
    sdata = AllocSegData(link);
    segidx = ORLCvtSecHdlToIdx( sec );
    snode = static_cast<segnode *>( AllocNodeIdx(link,  link.SegNodes, segidx ) );
    snode->entry = sdata;
    snode->handle = sec;
    sdata->iscdat = ( (flags & ORL_SEC_FLAG_COMDAT) != 0 );
    len = sizeof( CoffIDataSegName ) - 1;
    if( strnicmp( CoffIDataSegName, name, len ) == 0 ) {
        SeenDLLRecord(link);
        link.CurrMod->flags_mod |= MOD_IMPORT_LIB;
        if( name[len] == '$'
          && name[len + 1] == '6' ) {
            /*
             * .idata$6 it is the segment containg the name
             */
            ORLSecGetContents( sec, reinterpret_cast<unsigned_8 **>( &st.impExternalName ) );
            st.impExternalName += 2;
        } else if( name[len] == '$'
          && name[len + 1] == '4' ) {
            /*
             * .idata$4 it is an import by ordinal
             */
            ORLSecGetContents( sec, &contents );
            if( flags & ORL_SEC_FLAG_USE_64 ) {
                uint_64 val;

                memcpy( &val, contents, sizeof( val ) );
                st.impOrdinal = static_cast<ordinal_t>( val );
            } else {
                uint_32 val;

                memcpy( &val, contents, sizeof( val ) );
                st.impOrdinal = static_cast<ordinal_t>( val );
            }
        }
        sdata->isdead = true;
        sdata->isidata = true;
    }
    sdata->combine = COMBINE_ADD;
    sdata->align = ( flags & ORL_SEC_FLAG_NO_PADDING ) ? 0 : ORLSecGetAlignment( sec );
    sdata->bits = ( flags & ORL_FILE_FLAG_64BIT_MACHINE ) ? BITS_64 : BITS_32;
    sdata->length = ORLSecGetSize( sec );
    sdata->u.name.u.ptr = const_cast<char *>( name );
    if( flags & ORL_SEC_FLAG_EXEC ) {
        sdata->iscode = true;
    } else if( flags & ORL_SEC_FLAG_UNINITIALIZED_DATA ) {
        sdata->isuninit = true;
#ifdef DEVBUILD
    } else {
        const std::string_view sv( name );

        if( sv.size() >= 3
          && strnicmp( name + sv.size() - 3, "bss", 3 ) == 0 ) {
            LnkMsg( ERR+MSG_INTERNAL, "s", "Initialized BSS found" );
        }
#endif
    }
    numlines = ORLSecGetNumLines( sec );
    if( numlines > 0 ) {
        DBIAddLines(link,  sdata, static_cast<const void *>( ORLSecGetLines( sec ) ), numlines * ORL_STRUCT_SIZEOF( orl_linnum ), true );
    }
    return ORL_OKAY;
}

segnode *FindSegNode(Link& link,  orl_sec_handle sechdl )
{
    orl_table_index     idx;

    if( sechdl == nullptr )
        return nullptr;
    idx = ORLCvtSecHdlToIdx( sechdl );
    if( idx == 0 ) {
        return nullptr;
    } else {
        return static_cast<segnode *>( FindNode( link.SegNodes, idx ) );
    }
}

constexpr size_t PREFIX_LEN = sizeof( ImportSymPrefix ) - 1;

void ImpProcSymbol(Link& link,  segnode *snode, orl_symbol_type type, const char *name, size_t namelen )
{
    ORLState &st = orlState();

    if( type & ORL_SYM_TYPE_UNDEFINED ) {
        if( namelen > sizeof( CoffImportRefName ) - 1 ) {
            namelen -= sizeof( CoffImportRefName ) - 1;
            if( strnicmp( name + namelen, CoffImportRefName, sizeof( CoffImportRefName ) - 1 ) == 0 ) {
                st.impModName = static_cast<char *>( MemAllocSafe(link,  namelen + 5 ) );
                memcpy( st.impModName, name, namelen );
                if( strnicmp( link.CurrMod->name.u.ptr + strlen( link.CurrMod->name.u.ptr ) - 4, ".drv", 4 ) == 0 ) { /* KLUDGE!! */
                    memcpy( st.impModName + namelen, ".drv", 5 );
                } else {
                    memcpy( st.impModName + namelen, ".dll", 5 );
                }
            }
        }
    } else if( snode != nullptr
      && (snode->info & SEGINF_CODE) ) {
        if( st.firstCodeSymName == nullptr ) {
            st.firstCodeSymName = name;
        }
    } else {
        if( st.firstDataSymName == nullptr
          && memcmp( name, ImportSymPrefix, PREFIX_LEN ) == 0 ) {
            st.firstDataSymName = name + PREFIX_LEN;
        }
    }
}

void DefineComdatSym(Link& link,  segnode *snode, symbol *sym, unsigned_32 value )
{
    unsigned    select;
    sym_info    sym_type;
    segdata     *sdata;

    sdata = snode->entry;
    sdata->hascdatsym = true;
    select = sdata->select;
    if( select == 0 ) {
        sym_type = SYM_CDAT_SEL_ANY;
    } else {
        sym_type = static_cast<sym_info>( (select - 1) << SYM_CDAT_SEL_SHIFT );
    }
    DefineComdat(link,  sdata, sym, value, sym_type, snode->contents );
}

orl_return ProcSymbol( Link& link, orl_symbol_handle symhdl )
{
    orl_symbol_type     type;
    const char          *name;
    unsigned_64         val64;
    orl_sec_handle      sechdl;
    symbol              *sym;
    size_t              namelen;
    sym_flags           symop;
    extnode             *newnode;
    segnode             *snode;
    bool                isweak;
    orl_symbol_handle   assocsymhdl;
    symbol              *assocsym;
    orl_symbol_binding  binding;

    sechdl = ORLSymbolGetSecHandle( symhdl );
    snode = FindSegNode(link,  sechdl );
    type = ORLSymbolGetType( symhdl );
    name = ORLSymbolGetName( symhdl );
    if( type & ORL_SYM_TYPE_FILE ) {
        return ORL_OKAY;
    }
    if( type & ORL_SYM_TYPE_DEBUG )
        return ORL_OKAY;
    if( (type & (ORL_SYM_TYPE_OBJECT | ORL_SYM_TYPE_FUNCTION))
      || ((type & (ORL_SYM_TYPE_NOTYPE | ORL_SYM_TYPE_UNDEFINED))
      && name != nullptr ) ) {
        namelen = strlen( name );
        if( namelen == 0 ) {
            BadObject(link);
        }
        if( link.CurrMod->flags_mod & MOD_IMPORT_LIB ) {
            ImpProcSymbol(link,  snode, type, name, namelen );
            return ORL_OKAY;
        }
        newnode = static_cast<extnode *>( AllocNode(link,  link.ExtNodes ) );
        newnode->handle = symhdl;
        binding = ORLSymbolGetBinding( symhdl );
        symop = ST_CREATE;
        if( binding == ORL_SYM_BINDING_LOCAL ) {
            symop |= ST_STATIC | ST_NONUNIQUE;
        }
        if( (type & ORL_SYM_TYPE_UNDEFINED)
          && binding != ORL_SYM_BINDING_ALIAS ) {
            symop |= ST_REFERENCE;
        } else {
            symop |= ST_NOALIAS;
        }
        sym = SymOp(link,  symop, name, namelen );
        CheckIfTocSym( sym );
        if( type & ORL_SYM_TYPE_COMMON ) {
            ORLSymbolGetValue( symhdl, &val64 );
            sym = MakeCommunalSym(link,  sym, U64Low( val64 ), false, BITS_32 );
        } else if( type & ORL_SYM_TYPE_UNDEFINED ) {
            DefineReference(link,  sym );
            isweak = false;
            switch( binding ) {
            case ORL_SYM_BINDING_WEAK:
                isweak = true;
                [[fallthrough]];
            case ORL_SYM_BINDING_ALIAS:
            case ORL_SYM_BINDING_LAZY:
                assocsymhdl = ORLSymbolGetAssociated( symhdl );
                name = ORLSymbolGetName( assocsymhdl );
                namelen = strlen( name );
                if( binding == ORL_SYM_BINDING_ALIAS ) {
                    MakeSymAlias(link,  sym->name.u.ptr, strlen( sym->name.u.ptr ), name, namelen );
                } else {
                    assocsym = SymOp(link,  ST_CREATE_REFERENCE, name, namelen );
                    DefineLazyExtdef(link,  sym, assocsym, isweak );
                    newnode->isweak = true;
                }
                break;
            default:
                break;
            }
        } else {
            newnode->isdefd = true;
            ORLSymbolGetValue( symhdl, &val64 );
            if( (type & ORL_SYM_TYPE_COMMON)
              && (type & ORL_SYM_TYPE_OBJECT)
              && sechdl == nullptr ) {
                sym = MakeCommunalSym(link,  sym, U64Low( val64 ), false, BITS_32 );
            } else if( snode != nullptr
              && snode->entry != nullptr
              && snode->entry->iscdat ) {
                DefineComdatSym(link,  snode, sym, U64Low( val64 ) );
            } else {
                sym->info |= SYM_DEFINED;
                DefineSymbol(link,  sym, snode, U64Low( val64 ), 0 );
            }
        }
        newnode->entry = sym;
    } else if( (type & ORL_SYM_TYPE_SECTION)
      && (type & ORL_SYM_CDAT_MASK)
      && snode != nullptr
      && (snode->info & SEGINF_DEAD) == 0 ) {
        snode->entry->select = (type & ORL_SYM_CDAT_MASK) >> ORL_SYM_CDAT_SHIFT;
    }
    return ORL_OKAY;
}

orl_return SymTable( Link& link, orl_sec_handle sec )
{
    return ORLSymbolSecScan( sec, bind_link( link, ProcSymbol ) );
}

orl_return DoReloc( Link& link, orl_reloc reloc )
{
    fix_type    type;
    frame_spec  frame;
    target_spec target;
    offset      addend;
    segnode     *snode;
    segnode     *sym_snode;
    extnode     *ext;
    bool        istoc;

    istoc = false;
    type = static_cast<fix_type>( 0 );
    switch( reloc->type ) {
    case ORL_RELOC_TYPE_PAIR:
        return ORL_OKAY;
    case ORL_RELOC_TYPE_ABSOLUTE:
        type = FIX_OFFSET_32 | FIX_ABS;
        break;
    case ORL_RELOC_TYPE_WORD_16:
        type = FIX_OFFSET_16;
        break;
    case ORL_RELOC_TYPE_WORD_26:
        type = FIX_OFFSET_26 | FIX_SHIFT;
        break;
    case ORL_RELOC_TYPE_TOCREL_14:  /* relative ref to 14-bit offset from TOC base. */
        type = FIX_SHIFT;
        [[fallthrough]];
    case ORL_RELOC_TYPE_TOCREL_16:  /* relative ref to 16-bit offset from TOC base. */
    case ORL_RELOC_TYPE_GOT_16:     /* relative ref to 16-bit offset from TOC base. */
        type |= FIX_TOC | FIX_OFFSET_16;
        istoc = true;
        break;
    case ORL_RELOC_TYPE_TOCVREL_14: /* relative ref to 14-bit offset from TOC base. */
        type = FIX_SHIFT;
        [[fallthrough]];
    case ORL_RELOC_TYPE_TOCVREL_16: /* relative ref to 16-bit offset from TOC base. */
        type |= FIX_TOCV | FIX_OFFSET_16;
        break;
    case ORL_RELOC_TYPE_IFGLUE:
        type = FIX_IFGLUE | FIX_OFFSET_32;
        break;
    case ORL_RELOC_TYPE_IMGLUE:
        /* NYI: do we need this? */
        return ORL_OKAY;
    case ORL_RELOC_TYPE_JUMP:
        type = FIX_OFFSET_32 | FIX_REL;
        break;
    case ORL_RELOC_TYPE_REL_21_SH:
        type = FIX_OFFSET_21 | FIX_REL | FIX_SHIFT;
        break;
    case ORL_RELOC_TYPE_REL_24:
        type = FIX_OFFSET_24 | FIX_REL;
        break;
    case ORL_RELOC_TYPE_REL_32:
        type = FIX_OFFSET_32 | FIX_REL;
        break;
    case ORL_RELOC_TYPE_REL_32_NOADJ:
        type = FIX_OFFSET_32 | FIX_REL | FIX_NOADJ;
        break;
    case ORL_RELOC_TYPE_SEGMENT:
        type = FIX_BASE;
        break;
    case ORL_RELOC_TYPE_WORD_32_NB:
        type = FIX_OFFSET_32 | FIX_NO_BASE;
        break;
    case ORL_RELOC_TYPE_SEC_REL:
        type = FIX_OFFSET_32 | FIX_SEC_REL;
        break;
    case ORL_RELOC_TYPE_SECTION:
        type = FIX_BASE;
        break;
    case ORL_RELOC_TYPE_WORD_32:
        type = FIX_OFFSET_32;
        break;
    case ORL_RELOC_TYPE_GOT_32:     /* relative ref to 32-bit offset from TOC base. */
        type = FIX_OFFSET_32 | FIX_TOC;
        break;
    case ORL_RELOC_TYPE_HALF_HI:
    case ORL_RELOC_TYPE_HALF_HA:
        orlState().savedReloc = *reloc;
        return ORL_OKAY;
    case ORL_RELOC_TYPE_HALF_LO: {
        ORLState &st = orlState();

        if( st.savedReloc.type == ORL_RELOC_TYPE_NONE ) {  /* we recursed */
            type = FIX_OFFSET_16 | FIX_SIGNED;
        } else {
            st.savedReloc.type = ORL_RELOC_TYPE_NONE;      /* flag recursion */
            DoReloc( link, reloc );
            reloc = &st.savedReloc;
            type = FIX_HIGH_OFFSET_16;
        }
        break;
    }
    case ORL_RELOC_TYPE_NONE:
    default:
        LnkMsg(link,  ERR+LOC+MSG_BAD_RELOC_TYPE, nullptr );
        break;
    }
    snode = FindSegNode(link,  reloc->section );
    if( snode != nullptr
      && (snode->info & SEGINF_DEAD) == 0
      && snode->entry != nullptr
      && !snode->entry->isdead ) {
        addend = 0;
        SetCurrSeg(link,  snode->entry, 0, snode->contents );
        frame.type = FIX_FRAME_TARG;
        ext = FindExtHandle(link,  reloc->symbol );
        if( ext == nullptr ) {
            sym_snode = FindSegNode(link,  ORLSymbolGetSecHandle( reloc->symbol ) );
            if( sym_snode == nullptr ) {
                return ORL_OKAY;
            } else {
                unsigned_64 val64;

                ORLSymbolGetValue( reloc->symbol, &val64 );
                addend = U64Low( val64 );
                target.u.sdata = sym_snode->entry;
                target.type = FIX_TARGET_SEG;
                if( istoc ) {
                    AddSdataOffToToc(link,  sym_snode->entry, addend );
                }
            }
        } else {
            target.u.sym = ext->entry;
            target.type = FIX_TARGET_EXT;
            if( istoc ) {
                AddSymToToc(link,  target.u.sym );
            }
        }
        StoreFixup(link,  reloc->offset, type, &frame, &target, addend );
    }
    return ORL_OKAY;
}

orl_return P1Relocs( Link& link, orl_sec_handle sec )
{
    return ORLRelocSecScan( sec, bind_link( link, DoReloc ) );
}

void HandleImportSymbol(Link& link,  const char *name )
{
    ORLState &st = orlState();
    length_name intname;
    length_name modname;

    intname.name = name;
    intname.len = strlen( name );
    if( st.impModName == nullptr ) {
        st.impModName = FileName(link, link.CurrMod->name.u.ptr, strlen( link.CurrMod->name.u.ptr ), E_DLL, false );
    }
    modname.name = st.impModName;
    modname.len = strlen( st.impModName );
    if( st.impExternalName == nullptr ) {
        if( st.impOrdinal == 0 ) {
            st.impOrdinal = IMP_BY_NAME;
            HandleImport(link,  &intname, &modname, &intname, st.impOrdinal );
        } else {
            st.impOrdinal &= 0x7FFFFFFF;           /* get rid of that high bit */
            HandleImport(link,  &intname, &modname, nullptr, st.impOrdinal );
        }
    } else {
        length_name     extname;

        extname.name = st.impExternalName;
        extname.len = strlen( st.impExternalName );
        HandleImport(link,  &intname, &modname, &extname, IMP_BY_NAME );
    }
    MemFree( st.impModName );
    st.impModName = nullptr;
}

void ScanImported(Link& link)
{
    ORLState &st = orlState();

    if( link.CurrMod->flags_mod & MOD_IMPORT_LIB ) {
        if( st.firstCodeSymName != nullptr ) {
            HandleImportSymbol(link,  st.firstCodeSymName );
        } else if( st.firstDataSymName != nullptr ) {
            HandleImportSymbol(link,  st.firstDataSymName );
        }
    }
}

using SectionRoutine = orl_return (*)( Link&, orl_sec_handle );

constexpr std::array<SectionRoutine, 12> SegmentJumpTable = {
    NullFunc,           /* ORL_SEC_TYPE_NONE */
    DeclareSegment,     /* ORL_SEC_TYPE_NO_BITS (bss) */
    DeclareSegment,     /* ORL_SEC_TYPE_PROG_BITS */
    NullFunc,           /* ORL_SEC_TYPE_SYM_TABLE */
    NullFunc,           /* ORL_SEC_TYPE_DYN_SYM_TABLE */
    NullFunc,           /* ORL_SEC_TYPE_STR_TABLE */
    NullFunc,           /* ORL_SEC_TYPE_RELOCS */
    NullFunc,           /* ORL_SEC_TYPE_RELOCS_EXPAND */
    NullFunc,           /* ORL_SEC_TYPE_HASH */
    NullFunc,           /* ORL_SEC_TYPE_DYNAMIC */
    NullFunc,           /* ORL_SEC_TYPE_NOTE */
    NullFunc,           /* ORL_SEC_TYPE_LINK_INFO */
};

constexpr std::array<SectionRoutine, 12> SymbolJumpTable = {
    NullFunc,           /* ORL_SEC_TYPE_NONE */
    NullFunc,           /* ORL_SEC_TYPE_NO_BITS (bss) */
    NullFunc,           /* ORL_SEC_TYPE_PROG_BITS */
    SymTable,           /* ORL_SEC_TYPE_SYM_TABLE */
    NullFunc,           /* ORL_SEC_TYPE_DYN_SYM_TABLE */
    NullFunc,           /* ORL_SEC_TYPE_STR_TABLE */
    NullFunc,           /* ORL_SEC_TYPE_RELOCS */
    NullFunc,           /* ORL_SEC_TYPE_RELOCS_EXPAND */
    NullFunc,           /* ORL_SEC_TYPE_HASH */
    NullFunc,           /* ORL_SEC_TYPE_DYNAMIC */
    NullFunc,           /* ORL_SEC_TYPE_NOTE */
    NullFunc            /* ORL_SEC_TYPE_LINK_INFO */
};

constexpr std::array<SectionRoutine, 12> P1SpecificJumpTable = {
    NullFunc,           /* ORL_SEC_TYPE_NONE */
    NullFunc,           /* ORL_SEC_TYPE_NO_BITS (bss) */
    NullFunc,           /* ORL_SEC_TYPE_PROG_BITS */
    NullFunc,           /* ORL_SEC_TYPE_SYM_TABLE */
    Unsupported,        /* ORL_SEC_TYPE_DYN_SYM_TABLE */
    NullFunc,           /* ORL_SEC_TYPE_STR_TABLE */
    P1Relocs,           /* ORL_SEC_TYPE_RELOCS */
    P1Relocs,           /* ORL_SEC_TYPE_RELOCS_EXPAND */
    NullFunc,           /* ORL_SEC_TYPE_HASH */
    Unsupported,        /* ORL_SEC_TYPE_DYNAMIC */
    P1Note,             /* ORL_SEC_TYPE_NOTE */
    NullFunc            /* ORL_SEC_TYPE_LINK_INFO */
};

orl_return ProcSegments( Link& link, orl_sec_handle hdl )
{
    return SegmentJumpTable[ORLSecGetType( hdl )]( link, hdl );
}

orl_return ProcSymbols( Link& link, orl_sec_handle hdl )
{
    return SymbolJumpTable[ORLSecGetType( hdl )]( link, hdl );
}

orl_return ProcP1Specific( Link& link, orl_sec_handle hdl )
{
    return P1SpecificJumpTable[ORLSecGetType( hdl )]( link, hdl );
}

}

unsigned long ORLPass1(Link& link)
/*****************************
 * do pass 1 for an object file handled by ORL.
 */
{
    orl_file_handle     filehdl;

    link.LinkState |= LS_DOSSEG_FLAG;
    PermStartMod( link.CurrMod );
    filehdl = InitFile(link);
    if( filehdl == nullptr ) {
        LnkMsg(link,  FTL+MSG_BAD_OBJECT, "s", link.CurrMod->u1.source->infile->name.u.ptr );
        link.CurrMod->u1.source->infile->status |= INSTAT_IOERR;
        return static_cast<unsigned long>( -1 );
    }
    if( CheckFlags(link,  filehdl ) ) {
        if( (link.LinkState & LS_HAVE_PPC_CODE)
          && !link.FmtData.toc_initialized ) {
            InitToc(link);
            link.FmtData.toc_initialized = true;
        }
        if( link.LinkFlags & LF_DWARF_DBI_FLAG ) {
            link.CurrMod->flags_mod |= MOD_FLATTEN_DBI;
        }
        link.CurrMod->flags_mod |= MOD_NEED_PASS_2;
        ORLFileScan( filehdl, nullptr, bind_link( link, ProcSegments ) );
        IterateNodelist( link.SegNodes, bind_link( link, AllocSeg ), nullptr );
        ORLFileScan( filehdl, nullptr, bind_link( link, ProcSymbols ) );
        ScanImported(link);
        ORLFileScan( filehdl, nullptr, bind_link( link, ProcP1Specific ) );
        IterateNodelist( link.SegNodes, bind_link( link, DefNosymComdats ), nullptr );
    }
    FiniFile( filehdl, link.CurrMod->u1.source );
    return ORLFileSeek( link.CurrMod->u1.source, 0, SEEK_CUR );
}
