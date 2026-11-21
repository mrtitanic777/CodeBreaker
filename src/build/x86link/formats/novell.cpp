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
* Description:  Routines for creating Novell NetWare load files.
*
****************************************************************************/


#include <algorithm>
#include <array>
#include <cctype>
#include <cstddef>
#include <cstring>
#include <ctime>
#include <string_view>
#include "build/x86link/link/linkstd.h"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include "exenov.h"
#include "build/x86link/debuginfo/dbginfo.h"
#include "build/x86link/fixups/reloc.h"
#include "build/x86link/base/virtmem.h"
#include "build/x86link/base/fileio.h"
#include "build/x86link/debuginfo/dbgall.h"
#include "build/x86link/symbols/impexp.h"
#include "build/x86link/formats/output.h"
#include "build/x86link/formats/novell.h"
#include "wdbginfo.h"
#include "build/x86link/link/run_state.hpp"

#include "clibext.h"


#ifdef _NOVELL

namespace {

constexpr std::string_view  DUMMY_THREAD_NAME = " LONG";
constexpr std::size_t       EXT_NAME_MAX = 255;

// what the Novell load file keeps between the debug passes of one link
struct NovState {
    unsigned_32     DbgInfoCount;
    unsigned_32     DbgInfoLen;
    virt_mem        NovDbgInfo;
    virt_mem        CurrDbgLoc;
};

NovState &state()
{
    return cb_wlink::RunState::part<NovState>();
}

using ExtName = std::array<char, EXT_NAME_MAX + 1>;

}

void ResetLoadNov( void )
/***********************/
{
    NovState &st = state();

    st.DbgInfoCount = 0;
    st.DbgInfoLen = 0;
}

static unsigned_32 WriteNovRelocs(Link& link,  fixed_header *header )
/*******************************************************/
// write the relocations.
{
    DumpRelocList(link,  link.Root->reloclist );
    header->numberOfRelocationFixups = link.Root->relocs;
    return( static_cast<unsigned_32>( link.Root->relocs ) * sizeof( nov_reloc_item ) );
}

static size_t create_sym_extname( symbol *sym, ExtName &ext_name )
/****************************************************************/
{
    size_t      len;

    len = 0;
    /*
     * netware prefix support
     * (wlink copies a fixed count in each loop, not stopping at the end of the
     * string; kept as it was, the prefix filling the buffer up to the '@')
     */
    if( sym->prefix != nullptr ) {
        std::copy_n( sym->prefix, EXT_NAME_MAX - 1, ext_name.begin() );
        len = EXT_NAME_MAX - 1;
        ext_name[len++] = '@';
    }
    if( len < EXT_NAME_MAX ) {
        std::copy_n( sym->name.u.ptr, EXT_NAME_MAX - len, ext_name.begin() + len );
        len = EXT_NAME_MAX;
    }
    ext_name[len] = '\0';
    return( len );
}

static unsigned_32 WriteNovImports(Link& link,  fixed_header *header )
/********************************************************/
{
    nov_import      *nov_imp;
    unsigned_32     count;
    size_t          len;
    unsigned_32     wrote;
    unsigned_32     refs;
    virt_mem        *vmem_array;
    symbol          *sym;
    unsigned_32     size;

    wrote = count = 0;
    for( sym = link.HeadSym; sym != nullptr; sym = sym->next ) {
        if( !IS_SYM_IMPORTED( sym ) )
            continue;
        /* so SymFini doesn't try to free it */
        if( sym->p.import_nov == DUMMY_IMPORT_PTR )
            sym->p.import_nov = nullptr;
        nov_imp = sym->p.import_nov;
        if( nov_imp != nullptr ) {
            ExtName ext_name;

            len = create_sym_extname( sym, ext_name );
            wrote += WriteLoadU8Name(link,  ext_name.data(), len, false );

            if( nov_imp->contents <= MAX_IMP_INTERNAL ) {
                refs = nov_imp->contents;
            } else {        // imports are in virtual memory.
                refs = nov_imp->u.v.num_relocs;
            }
            WriteLoadU32(link,  refs );
            size = refs * sizeof( refs );
            wrote += sizeof( unsigned_32 ) + size;
            if( nov_imp->contents <= MAX_IMP_INTERNAL ) {
                WriteLoad(link,  nov_imp->u.r.relocs, size );
            } else {        // imports are in virtual memory.
                vmem_array = nov_imp->u.v.vm_ptr;
                for( ; size > IMP_VIRT_ALLOC_SIZE; size -= IMP_VIRT_ALLOC_SIZE ) {
                    WriteInfoLoad( *vmem_array++, IMP_VIRT_ALLOC_SIZE );
                }
                WriteInfoLoad( *vmem_array, size );
            }
            count++;
        }
    }
    header->numberOfExternalReferences = count;
    return( wrote );
}

static unsigned_32 WriteNovExports(Link& link,  fixed_header *header )
/********************************************************/
{
    obj_name_list   *export_;
    symbol          *sym;
    unsigned_32     count;
    unsigned_32     wrote;
    unsigned_32     off;
    size_t          len;

    count = wrote = 0;
    for( export_ = link.FmtData.u.nov.exp.export_; export_ != nullptr; export_ = export_->next ) {
        sym = SymOp(link,  ST_FIND, export_->name.u.ptr, export_->len );
        if( ( sym == nullptr )
          || (sym->info & SYM_DEFINED) == 0 ) {
            LnkMsg( WRN+MSG_EXP_SYM_NOT_FOUND, "s", export_->name.u.ptr );
        } else if( !IS_SYM_IMPORTED( sym ) ) {
            ExtName ext_name;

            len = create_sym_extname( sym, ext_name );
            wrote += WriteLoadU8Name(link,  ext_name.data(), len, false );

            off = sym->addr.off;
            if( sym->addr.seg == CODE_SEGMENT ) {
                off |= NOV_EXP_ISCODE;
            }
            WriteLoadU32(link,  off );
            wrote += sizeof( unsigned_32 );
            count++;

            AddImpLibEntry(link,  sym->name.u.ptr, ext_name.data(), 0, true );
        }
    }
    header->numberOfPublics = count;
    return( wrote );
}

static unsigned_32 WriteNovModules(Link& link,  fixed_header *header )
/********************************************************/
{
    obj_name_list   *module;
    unsigned_32     count;
    unsigned_32     wrote;

    count = wrote = 0;
    for( module = link.FmtData.u.nov.exp.module; module != nullptr; module = module->next ) {
        wrote += WriteLoadU8Name(link,  module->name.u.ptr, module->len, false );
        count++;
    }
    header->numberOfModuleDependencies = count;
    return( wrote );
}

void NovDBIAddGlobal(Link& link,  void *_sym )
/********************************/
{
    symbol *sym = static_cast<symbol *>( _sym );

    if( !IS_SYM_A_REF( sym )
      && !IS_SYM_ALIAS( sym )
      && ( sym->p.seg != nullptr )
      && (sym->info & SYM_DEAD) == 0
      && !sym->p.seg->isabs
      && (sym->info & SYM_STATIC) == 0
      && (link.FmtData.u.nov.flags & DO_NOV_EXPORTS) == 0 ) {
        state().DbgInfoLen += sizeof( nov_dbg_info ) + std::strlen( sym->name.u.ptr );
    }
}

void NovDBIAddrStart( void )
/**************************/
{
    NovState &st = state();

    if( st.DbgInfoLen != 0 ) {
        st.NovDbgInfo = AllocStg( st.DbgInfoLen );
        st.CurrDbgLoc = st.NovDbgInfo;
    }
}

void NovDBIGenGlobal(Link& link,  symbol *sym )
/*********************************/
{
    NovState        &st = state();
    nov_dbg_info    info;
    size_t          len;

    if( ( st.DbgInfoLen != 0 )
      && ( (link.FmtData.u.nov.flags & DO_NOV_REF_ONLY) == 0
      || (sym->info & SYM_REFERENCED) ) ) {
        st.DbgInfoCount++;
        if( sym->addr.seg == DATA_SEGMENT ) {
            info.type = DBG_DATA;
        } else {
            info.type = DBG_CODE;
        }
        info.offset = sym->addr.off;
        len = std::strlen( sym->name.u.ptr );
        if( len > 255 )
            len = 255;
        info.namelen = static_cast<unsigned char>( len );
        PutInfo( st.CurrDbgLoc, &info, sizeof( nov_dbg_info ) );
        st.CurrDbgLoc += sizeof( nov_dbg_info );
        PutInfo( st.CurrDbgLoc, sym->name.u.ptr, info.namelen );
        st.CurrDbgLoc += info.namelen;
    }
}

static unsigned_32 WriteNovDBI(Link& link,  fixed_header *header )
/****************************************************/
{
    obj_name_list   *export_;
    symbol          *sym;
    unsigned_32     count;
    unsigned_32     wrote;
    nov_dbg_info    info;
    NovState        &st = state();

    if( st.DbgInfoLen > 0 ) {
        WriteInfoLoad( st.NovDbgInfo, st.CurrDbgLoc - st.NovDbgInfo );
        header->numberOfDebugRecords = st.DbgInfoCount;
        return( st.CurrDbgLoc - st.NovDbgInfo );
    } else if( link.FmtData.u.nov.flags & DO_NOV_EXPORTS ) {
        count = wrote = 0;
        for( export_ = link.FmtData.u.nov.exp.export_; export_ != nullptr; export_ = export_->next ) {
            sym = SymOp(link,  ST_FIND, export_->name.u.ptr, export_->len );
            if( ( sym != nullptr )
              && !IS_SYM_IMPORTED( sym ) ) {
                if( sym->addr.seg == DATA_SEGMENT ) {
                    info.type = DBG_DATA;
                } else {
                    info.type = DBG_CODE;
                }
                info.offset = sym->addr.off;
                WriteLoad(link,  &info, offsetof( nov_dbg_info, namelen ) );
                wrote += offsetof( nov_dbg_info, namelen ) + WriteLoadU8Name(link,  export_->name.u.ptr, export_->len, false );
                count++;
            }
        }
        header->numberOfDebugRecords = count;
        return( wrote );
    }
    header->numberOfDebugRecords = 0;
    return( 0 );
}

static unsigned_32 WriteMessages(Link& link,  extended_nlm_header *header )
/*************************************************************/
/* write out the messages file */
{
    f_handle    handle;
    std::array<unsigned_32, 2>  buf;

    header->messageFileLength = 0;
    if( link.FmtData.u.nov.messages != nullptr ) {
        handle = QOpenR(link,  link.FmtData.u.nov.messages );
        QRead( handle, link.TokBuff, MSG_FILE_SIGNATURE_LENGTH, link.FmtData.u.nov.messages );
        if( std::memcmp( link.TokBuff, MSG_FILE_SIGNATURE, MSG_FILE_SIGNATURE_LENGTH ) != 0 ) {
            LnkMsg( WRN+MSG_INV_MESSAGE_FILE, "s", link.FmtData.u.nov.messages );
        } else {
            QSeek( handle, LANGUAGE_ID_OFFSET, link.FmtData.u.nov.messages );
            QRead( handle, buf.data(), sizeof( buf ), link.FmtData.u.nov.messages );
            header->languageID = buf[0];
            header->messageCount = buf[1];
            QSeek( handle, 0, link.FmtData.u.nov.messages );
            header->messageFileLength = CopyToLoad(link,  handle, link.FmtData.u.nov.messages );
        }
        QClose( handle, link.FmtData.u.nov.messages );
    }
    return( header->messageFileLength );
}

static unsigned_32 WriteSharedNLM(Link& link,  extended_nlm_header *header, unsigned_32 file_size )
/*************************************************************************************/
{
    f_handle            handle;
    fixed_header        *sharehdr;
    unsigned_32         size;

    size = 0;
    if( link.FmtData.u.nov.sharednlm != nullptr ) {
        handle = QOpenR(link,  link.FmtData.u.nov.sharednlm );
        QRead( handle, link.TokBuff, sizeof( fixed_header ), link.FmtData.u.nov.sharednlm );
        if( std::memcmp( link.TokBuff, NLM_SIGNATURE, NLM_SIGNATURE_LENGTH ) != 0 ) {
            LnkMsg( WRN+MSG_INV_SHARED_NLM_FILE, "s", link.FmtData.u.nov.sharednlm );
        } else {
            sharehdr = reinterpret_cast<fixed_header *>( link.TokBuff );
            header->sharedCodeOffset = sharehdr->codeImageOffset + file_size;
            header->sharedCodeLength = sharehdr->codeImageSize;
            header->sharedDataOffset = sharehdr->dataImageOffset + file_size;
            header->sharedDataLength = sharehdr->dataImageSize;
            header->sharedRelocationFixupOffset
                        = sharehdr->relocationFixupOffset + file_size;
            header->sharedRelocationFixupCount
                        = sharehdr->numberOfRelocationFixups;
            header->sharedExternalReferenceOffset
                        = sharehdr->externalReferencesOffset + file_size;
            header->sharedExternalReferenceCount
                        = sharehdr->numberOfExternalReferences;
            header->sharedPublicsOffset = sharehdr->publicsOffset + file_size;
            header->sharedPublicsCount = sharehdr->numberOfPublics;
            header->sharedDebugRecordOffset
                        = sharehdr->debugInfoOffset + file_size;
            header->sharedDebugRecordCount = sharehdr->numberOfDebugRecords;
            header->sharedInitializationOffset = sharehdr->codeStartOffset;
            header->sharedExitProcedureOffset = sharehdr->exitProcedureOffset;
            QSeek( handle, 0, link.FmtData.u.nov.sharednlm );
            size = CopyToLoad(link,  handle, link.FmtData.u.nov.sharednlm );
        }
        QClose( handle, link.FmtData.u.nov.sharednlm );
    }
    return( size );
}

static void GetProcOffsets(Link& link,  fixed_header *header )
/************************************************/
{
    symbol      *sym;
    const char  *name;

    header->checkUnloadProcedureOffset = 0;
    if( link.FmtData.u.nov.checkfn != nullptr ) {
        sym = FindISymbol(link,  link.FmtData.u.nov.checkfn );
        if( sym == nullptr ) {
            LnkMsg( WRN+MSG_CHECK_NOT_FOUND, nullptr );
        } else {
            header->checkUnloadProcedureOffset = sym->addr.off;
        }
    }
    if( link.StartInfo.type == START_IS_SYM ) {
        name = link.StartInfo.targ.sym->name.u.ptr;
    } else {
        name = DEFAULT_PRELUDE_FN_CLIB;
    }
    sym = FindISymbol(link,  name );
    if( ( sym == nullptr )
      || (sym->info & SYM_DEFINED) == 0 ) {
        LnkMsg( ERR + MSG_START_PROC_NOT_FOUND, nullptr );
    } else {
        header->codeStartOffset = sym->addr.off;
    }
    if( link.FmtData.u.nov.exitfn != nullptr ) {
        name = link.FmtData.u.nov.exitfn;
    } else {
        name = DEFAULT_EXIT_FN_CLIB;
    }
    sym = FindISymbol(link,  name );
    if( sym == nullptr ) {
        LnkMsg( ERR + MSG_EXIT_PROC_NOT_FOUND, nullptr );
    } else {
        header->exitProcedureOffset = sym->addr.off;
    }
}

static unsigned_32 WriteNovImage(Link& link,  unsigned_32 file_pos, bool docode )
/*******************************************************************/
// Write a Novell image
{
    group_entry         *group;
    outfilelist         *fnode;
    bool                repos;
    bool                iscode;

    /* write groups.*/
    fnode = link.Root->outfile;
    for( group = link.Groups; group != nullptr; group = group->next ) {
        if( group->addr.seg == CODE_SEGMENT ) {
            iscode = true;
        } else {
            iscode = false;
        }
        if( iscode == docode ) {  // logical XNOR would be better, but...
            repos = WriteGroup(link,  group );
            if( repos ) {
                SeekLoad(link,  fnode->file_loc );
            }
        }
    }
    return( fnode->file_loc - file_pos );
}

static unsigned_32 WriteNovData(Link& link,  unsigned_32 file_pos, fixed_header *header )
/***************************************************************************/
// write both the code image and the data image.
{
    unsigned_32     codesize;

    DEBUG(( DBG_BASE, "Writing data" ));
    OrderGroups(link,  CompareProtSegments );
    link.CurrSect = link.Root;        // needed for WriteInfo.
    link.Root->outfile->file_loc = file_pos;
    link.Root->u.file_loc = file_pos;
    link.Root->sect_addr.off = link.Groups->addr.off;
    link.Root->sect_addr.seg = CODE_SEGMENT;
    codesize = WriteNovImage(link,  file_pos, true );         // true = do code.
    header->codeImageSize = codesize;
    file_pos += codesize;
    header->dataImageOffset = file_pos;
    link.Root->outfile->file_loc = file_pos;
    link.Root->u.file_loc = file_pos;
    link.Root->sect_addr.off = link.Groups->addr.off;
    link.Root->sect_addr.seg = DATA_SEGMENT;
    header->dataImageSize = WriteNovImage(link,  file_pos, false );   // do data.
    return( codesize + header->dataImageSize );
}


static void NovNameWrite(Link& link,  const char *name )
/******************************************/
// write a name to the loadfile in the typical novell fashion
{
    if( name != nullptr ) {
        WriteLoadU8Name(link,  name, std::strlen( name ), false );
    } else {
        WriteLoadU8(link,  0 );
    }
    WriteLoadU8(link,  '\0' );    // terminating null character
}

void FiniNovellLoadFile(Link& link)
/*****************************/
{
    unsigned_32         file_size;
    fixed_header        nov_header;
    fixed_hdr_2         second_header;
    fixed_hdr_3         third_header;
    extended_nlm_header ext_header;
    unsigned_32         temp;
    unsigned_32         image_size;
    const char          *startname;
    const char          *src;
    size_t              len;
    size_t              len1;
    const std::tm       *currtime;
    std::time_t         thetime;
    const char          *pPeriod;
    std::array<char, NOV_MAX_MODNAME_LEN + 1>   module_name;
    bool                name_trunc;
    const char          *desc;

    nov_header = fixed_header{};
    /*
     * get module name (output file base name without the path)
     */
    startname = GetBaseName( link.Root->outfile->fname, 0, &len1 );
    pPeriod = startname + len1;
    /*
     * truncate the module name to 8.3 (NOV_MAX_MODNAME_LEN) if necessary
     */
    name_trunc = false;
    if( len1 > NOV_MAX_NAME_LEN ) {
        len1 = NOV_MAX_NAME_LEN;
        name_trunc = true;
    }
    len = 0;
    src = startname;
    while( len < len1 ) {
        module_name[len++] = static_cast<char>( std::toupper( static_cast<unsigned char>( *src++ ) ) );
    }
    if( pPeriod[0] == '.' ) {
        len1 = std::strlen( pPeriod );
        if( len1 > NOV_MAX_EXT_LEN + 1 ) {   /* +1 include period */
            len1 = NOV_MAX_EXT_LEN + 1;
            name_trunc = true;
        }
        len1 += len;
        src = pPeriod;
        while( len < len1 ) {
            module_name[len++] = static_cast<char>( std::toupper( static_cast<unsigned char>( *src++ ) ) );
        }
    }
    module_name[len] = '\0';
    if( name_trunc ) {
        LnkMsg( WRN+MSG_INTERNAL_MOD_NAME_DIFF_FROM_FILE, "s", module_name.data() );
    }
    /* setup description */
    if( link.FmtData.description == nullptr ) {
        desc = module_name.data();
        nov_header.descriptionLength = len;
    } else {
        nov_header.descriptionLength = std::strlen( link.FmtData.description );
        if( nov_header.descriptionLength > MAX_DESCRIPTION_LENGTH ) {
            nov_header.descriptionLength = MAX_DESCRIPTION_LENGTH;
            link.FmtData.description[MAX_DESCRIPTION_LENGTH] = '\0';
            LnkMsg( WRN+MSG_VALUE_TOO_LARGE, "s", "DESCRIPTION" );
        }
        desc = link.FmtData.description;
    }

    file_size = nov_header.descriptionLength + sizeof( fixed_header )
                + sizeof( extended_nlm_header ) + 2 * sizeof( unsigned_32 )
                + NOV_MAX_MODNAME_LEN;
    if( link.FmtData.u.nov.screenname != nullptr ) {
        file_size += std::strlen( link.FmtData.u.nov.screenname );
    }
    if( link.FmtData.u.nov.threadname != nullptr ) {
        file_size += std::strlen( link.FmtData.u.nov.threadname );
    } else {
        file_size += len;
    }
    if( link.FmtData.ver_specified ) {
        if( ( link.FmtData.major != 0 )
          || ( link.FmtData.minor != 0 ) ) {
            file_size += sizeof( fixed_hdr_2 );
        }
    }
    if( link.FmtData.u.nov.copyright != nullptr ) {
        file_size += sizeof( fixed_hdr_3 ) + std::strlen( link.FmtData.u.nov.copyright );
    }
    SeekLoad(link,  file_size );
    nov_header.codeImageOffset = file_size;
    image_size = WriteNovData(link,  file_size, &nov_header );
    len1 = MemorySize(link) - image_size;
    if( len1 > 0 ) {       // write out BSS.
        PadLoad(link,  len1 );
        nov_header.dataImageSize += len1;
        image_size += len1;
    }
    file_size += image_size;
    nov_header.relocationFixupOffset = file_size;
    file_size += WriteNovRelocs(link,  &nov_header );
    nov_header.externalReferencesOffset = file_size;
    file_size += WriteNovImports(link,  &nov_header );
    nov_header.publicsOffset = file_size;
    file_size += WriteNovExports(link,  &nov_header );
    nov_header.moduleDependencyOffset = file_size;
    file_size += WriteNovModules(link,  &nov_header );
    nov_header.customDataOffset = file_size;
    temp = AppendToLoadFile(link,  link.FmtData.u.nov.customdata );
    nov_header.customDataSize = temp;
    file_size += temp;
    ext_header = extended_nlm_header{};
    std::memcpy( ext_header.stamp, EXTENDED_NLM_SIGNATURE, EXTENDED_NLM_SIGNATURE_LENGTH );
    ext_header.messageFileOffset = file_size;
    file_size += WriteMessages(link,  &ext_header );
    ext_header.helpFileOffset = file_size;
    temp = AppendToLoadFile(link,  link.FmtData.u.nov.help );
    ext_header.helpFileLength = temp;
    file_size += temp;
    ext_header.RPCDataOffset = file_size;
    temp = AppendToLoadFile(link,  link.FmtData.u.nov.rpcdata );
    ext_header.RPCDataLength = temp;
    file_size += temp;
    file_size += WriteSharedNLM(link,  &ext_header, file_size );
    nov_header.debugInfoOffset = file_size;
    file_size += WriteNovDBI(link,  &nov_header );
    DBIWrite(link);
    std::memcpy( nov_header.signature, NLM_SIGNATURE, NLM_SIGNATURE_LENGTH );
    nov_header.version = NLM_VERSION;
    nov_header.moduleName[0] = static_cast<char>( len );
    std::copy_n( module_name.begin(), len, nov_header.moduleName + 1 );
    nov_header.uninitializedDataSize = 0; // MemorySize() - image_size;
    GetProcOffsets(link,  &nov_header );
    nov_header.moduleType = link.FmtData.u.nov.moduletype;
    nov_header.flags = link.FmtData.u.nov.exeflags;
    SeekLoad(link,  0L );
    WriteLoad(link,  &nov_header, sizeof( nov_header ) );
    WriteLoad(link,  desc, nov_header.descriptionLength + 1 );
    WriteLoadU32(link,  link.StackSize );
    WriteLoadU32(link,  0 );          // reserved.
    WriteLoad(link,  DUMMY_THREAD_NAME.data(), OLD_THREAD_NAME_LENGTH );
    NovNameWrite(link,  link.FmtData.u.nov.screenname );
    if( link.FmtData.u.nov.threadname != nullptr ) {
        NovNameWrite(link,  link.FmtData.u.nov.threadname );
    } else {
        NovNameWrite(link,  module_name.data() );      // use module name as a default
    }
    if( link.FmtData.ver_specified ) {
        if( ( link.FmtData.major != 0 )
          || ( link.FmtData.minor != 0 ) ) {
            std::memcpy( second_header.versionSignature, VERSION_SIGNATURE, VERSION_SIGNATURE_LENGTH );
            second_header.majorVersion = link.FmtData.major;
            second_header.minorVersion = link.FmtData.minor;
            second_header.revision = link.FmtData.revision;
            thetime = std::time( nullptr );
            currtime = std::localtime( &thetime );
            second_header.year = currtime->tm_year + 1900;
            second_header.month = currtime->tm_mon + 1;
            second_header.day = currtime->tm_mday;
            WriteLoad(link,  &second_header, sizeof( second_header ) );
        }
    }
    if( link.FmtData.u.nov.copyright != nullptr ) {
        std::memcpy( third_header.copyrightSignature, COPYRIGHT_SIGNATURE, COPYRIGHT_SIGNATURE_LENGTH);
        WriteLoad(link,  &third_header, sizeof( third_header ) );
        NovNameWrite(link,  link.FmtData.u.nov.copyright );
    }
    WriteLoad(link,  &ext_header, sizeof( ext_header ) );
}

void AddNovImpReloc(Link& link,  symbol *sym, unsigned_32 offset, bool isrelative, bool isdata )
/**********************************************************************************/
// add a relocation to the import record.
{
    nov_import      *nov_imp;
    nov_import      *tmp;
    virt_mem        vmem_ptr;
    unsigned        vblock;     // which virt_mem block
    unsigned        voff;       // offset into a virt_mem block

    if( !isrelative ) {
         offset |= NOV_IMP_NONRELATIVE;
    }
    if( !isdata ) {
        offset |= NOV_IMP_ISCODE;
    }
    nov_imp = sym->p.import_nov;
    if( nov_imp == DUMMY_IMPORT_PTR ) {
        nov_imp = static_cast<nov_import *>( MemAllocSafe( sizeof( nov_import ) ) );
        sym->p.import_nov = nov_imp;
        nov_imp->contents = 0;
        nov_imp->u.r.relocs[nov_imp->contents++] = offset;
    } else if( nov_imp->contents < MAX_IMP_INTERNAL ) {
        if( nov_imp->contents == 1 ) {
            tmp = static_cast<nov_import *>( MemAllocSafe( ( MAX_IMP_INTERNAL - 1 ) * sizeof( unsigned_32 ) + sizeof( nov_import ) ) );
            std::memcpy( tmp, nov_imp, sizeof( nov_import ) );
            MemFree( nov_imp );
            nov_imp = tmp;
            sym->p.import_nov = nov_imp;
        }
        nov_imp->u.r.relocs[nov_imp->contents++] = offset;
    } else if( nov_imp->contents == MAX_IMP_INTERNAL ) { // set up virt.mem
        vmem_ptr = AllocStg( IMP_VIRT_ALLOC_SIZE );
        PutInfo( vmem_ptr, nov_imp->u.r.relocs, MAX_IMP_INTERNAL * sizeof( unsigned_32 ) );
        PutInfo( vmem_ptr + MAX_IMP_INTERNAL * sizeof( unsigned_32 ), &offset, sizeof( unsigned_32 ) );
        nov_imp->contents++;
        nov_imp->u.v.num_relocs = nov_imp->contents;
        nov_imp->u.v.vm_ptr[0] = vmem_ptr;
    } else {    // nov_imp->contents > MAX_IMP_INTERNAL
        vblock = nov_imp->u.v.num_relocs / IMP_NUM_VIRT;
        voff = nov_imp->u.v.num_relocs % IMP_NUM_VIRT;
        if( voff == 0 ) {
            if( vblock >= static_cast<unsigned>( nov_imp->contents - MAX_IMP_INTERNAL ) * MAX_IMP_VIRT ) {
                tmp = static_cast<nov_import *>( MemAllocSafe( sizeof( nov_import ) - sizeof( unsigned_32 ) + vblock * sizeof( unsigned_32 ) * 2 ) );
                std::memcpy( tmp, nov_imp, sizeof( nov_import ) - sizeof( unsigned_32 ) + vblock * sizeof( unsigned_32 ) );
                MemFree( nov_imp );
                nov_imp = tmp;
                nov_imp->contents++;
                sym->p.import_nov = nov_imp;
            }
            nov_imp->u.v.vm_ptr[vblock] = AllocStg( IMP_VIRT_ALLOC_SIZE );
        }
        PutInfo( nov_imp->u.v.vm_ptr[vblock] + voff * sizeof( unsigned_32 ), &offset, sizeof( unsigned_32 ) );
        nov_imp->u.v.num_relocs++;
    }
}

void FindExportedSyms(Link& link)
/***************************/
{
    obj_name_list   *export_;
    symbol          *sym;
    debug_info      *dinfo;

    dinfo = static_cast<debug_info *>( link.CurrSect->dbg_info );
    if( (link.FmtData.u.nov.flags & DO_WATCOM_EXPORTS)
      && ( dinfo != nullptr ) ) {
        for( export_ = link.FmtData.u.nov.exp.export_; export_ != nullptr; export_ = export_->next ) {
            sym = SymOp(link,  ST_FIND, export_->name.u.ptr, export_->len );
            if( ( sym != nullptr )
              && !IS_SYM_IMPORTED( sym ) ) {
                dinfo->global.curr.u.vm_offs += sizeof( v3_gbl_info ) + std::strlen( sym->name.u.ptr );
            }
        }
    }
}

#endif
