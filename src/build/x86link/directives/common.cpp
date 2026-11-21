// build/x86link/directives/common.cpp -- the directives and options common to every format: file, library, name, option,
// debug, system blocks, order, output, sort, language (a port of wlink's cmdall.c, and NewOutFile of cmdutils.c).
#include "build/x86link/directives/common.hpp"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state, wlink() */

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <string>

#include "build/x86link/directives/formats.hpp"
#include "build/x86link/directives/state.hpp"
#include "build/x86link/link/linkstd.h"
#include "build/x86link/base/alloc.h"
#include "build/x86link/base/strtab.h"
#include "build/x86link/debuginfo/dbgall.h"
#include "build/x86link/directives/cmdline.h"
#include "build/x86link/symbols/symtrace.h"
#include "build/x86link/objects/objio.h"
#include "build/x86link/formats/output.h"
#include "build/x86link/base/carve.h"
#include "build/x86link/link/permdata.h"
#include "pathlist.h"
#include "build/x86link/symbols/library.h"
#include "build/x86link/link/sysblock.h"
#include "build/x86link/base/linkutil.h"
#include "build/x86link/messages/msg.h"
#include "build/x86link/messages/wlnkmsg.h"

#include "clibext.h"

#define DEFAULT_INC_NAME "__wlink.ilk"

namespace cb::x86link::directives {

namespace {

CommonState& common_state() { return state().common; }

bool ProcDosSeg(Link& link) {
    link.LinkState |= LS_DOSSEG_FLAG;
    DEBUG((DBG_OLD, "dosseg"));
    return true;
}

bool ProcName(Link& link) {
    if (!reader().get(Sep::none, Want::file_name)) return false;
    cmd_flags() &= ~Flag::unnamed;
    if (exe_name() != nullptr) MemFree(exe_name());
    exe_name() = MemToStringSafe(link, reader().text().data(), reader().text().size());   // (just keep the name around for now)
    return true;
}

bool ProcDwarfDBI(Link& link) {
    if (link.LinkFlags & (LF_ANY_DBI_FLAG & ~LF_DWARF_DBI_FLAG)) {
        LnkMsg(WRN + LOC + LINE + MSG_MULT_DBI_FORMATS, nullptr);
        return true;
    }
    link.LinkFlags |= LF_DWARF_DBI_FLAG;
    return true;
}

bool ProcWatcomDBI(Link& link) {
    if (link.LinkFlags & (LF_ANY_DBI_FLAG & ~LF_OLD_DBI_FLAG)) {
        LnkMsg(WRN + LOC + LINE + MSG_MULT_DBI_FORMATS, nullptr);
        return true;
    }
    link.LinkFlags |= LF_OLD_DBI_FLAG;
    return true;
}

bool ProcCodeviewDBI(Link& link) {
    if (link.LinkFlags & (LF_ANY_DBI_FLAG & ~LF_CV_DBI_FLAG)) {
        LnkMsg(WRN + LOC + LINE + MSG_MULT_DBI_FORMATS, nullptr);
        return true;
    }
    link.LinkFlags |= LF_CV_DBI_FLAG;
    return true;
}

bool ProcHllDBI(Link& link) {
    if (link.LinkFlags & (LF_ANY_DBI_FLAG & ~LF_HLL_DBI_FLAG)) {
        LnkMsg(LOC + LINE + WRN + MSG_MULT_DBI_FORMATS, nullptr);
        return true;
    }
    link.LinkFlags |= LF_HLL_DBI_FLAG;
    return true;
}

bool ProcLine(Link& link) {
    if ((link.LinkFlags & LF_ANY_DBI_FLAG) == 0) link.LinkFlags |= LF_DWARF_DBI_FLAG;
    link.DBIFlag |= DBI_LINE;
    return true;
}

bool ProcType(Link& link) {
    if ((link.LinkFlags & LF_ANY_DBI_FLAG) == 0) link.LinkFlags |= LF_DWARF_DBI_FLAG;
    link.DBIFlag |= DBI_TYPE;
    return true;
}

bool ProcLocal(Link& link) {
    if ((link.LinkFlags & LF_ANY_DBI_FLAG) == 0) link.LinkFlags |= LF_DWARF_DBI_FLAG;
    link.DBIFlag |= DBI_LOCAL;
    return true;
}

bool ProcAll(Link& link) {
    if ((link.LinkFlags & LF_ANY_DBI_FLAG) == 0) link.LinkFlags |= LF_DWARF_DBI_FLAG;
    link.DBIFlag |= DBI_ALL;
    return true;
}

// an individual alias
bool AddAlias(Link& link) {
    Reader& r = reader();
    const std::string name = r.str();
    if (!r.get(Sep::equals, Want::include_dot)) return false;
    MakeSymAlias(link, name.data(), name.size(), r.text().data(), r.text().size());
    return true;
}

bool ProcAlias(Link& link) { return process_list(link, AddAlias, Want::include_dot); }

bool AddReference(Link& link) {
    symbol* sym = SymOp(link, ST_CREATE_REFERENCE, reader().text().data(), reader().text().size());
    sym->info |= SYM_1_DCE_REF;   // make sure it stays around
    return true;
}

bool ProcReference(Link& link) { return process_list(link, AddReference, Want::include_dot); }

bool ProcOSName(Link& link) {
    if (reader().get(Sep::equals, Want::include_dot)) {
        if (link.FmtData.osname != nullptr) MemFree(link.FmtData.osname);
        link.FmtData.osname = MemToStringSafe(link, reader().text().data(), reader().text().size());
        return true;
    }
    return false;
}

// dead code elimination
bool ProcEliminate(Link& link) {
    link.LinkFlags |= LF_STRIP_CODE;
    return true;
}

bool ProcMaxErrors(Link& link) {
    if (!get_number(link.MaxErrors)) return false;
    link.LinkFlags |= LF_MAX_ERRORS_FLAG;
    return true;
}

bool ProcSymFile(Link& link) {
    if (reader().get(Sep::equals, Want::file_name)) {
        if (link.SymFileName != nullptr) MemFree(link.SymFileName);
        link.SymFileName = FileName(link, reader().text().data(), reader().text().size(), E_SYM, false);
    }
    return true;
}

void* AddObjFile(Link& link, const char* name, const char* membname, file_list** filelist) {
    file_list* file;
    member_list* member = nullptr;
    if (membname != nullptr) {
        member = (member_list*)(MemAllocSafe(sizeof(*member) - 1 + strlen(membname) + 1));
        member->flags_dbi = link.DBIFlag;
        member->flags_mod = (module_flags)(0);
        strcpy(member->name, membname);
        member->next = nullptr;
        for (file = link.CurrSect->files; file != nullptr; file = file->next) {
            if (FNAMECMPSTR(file->infile->name.u.ptr, name) == 0) {
                cmd_flags() |= Flag::member_added;
                if (file->u.member != nullptr) {
                    AppendList((void**)&file->u.member, member);
                    return member;
                } else {
                    MemFree(member);   // user did a stupid thing.
                    return file->u.member;
                }
            }
        }
    }
    file = AllocNewFile(link, member);
    if (member != nullptr) {
        file->infile = AllocUniqueFileEntry(link, name, link.UsrLibPath);
        file->infile->status |= INSTAT_LIBRARY;
    } else {
        file->infile = AllocFileEntry(link, name, link.ObjPath);
    }
    *filelist = file;
    return file;
}

bool AddLibFile(Link& link) {
    char* membname;
    cmd_flags() &= ~Flag::member_added;
    char* ptr = file_name(link, &membname);
    if (membname != nullptr) {
        LnkMsg(WRN + LOC + LINE + MSG_NO_MEMB_IN_LIBFILE, nullptr);
        MemFree(membname);
        MemFree(ptr);
        return true;
    }
    CommonState& c = common_state();
    file_list* file = AllocNewFile(link, nullptr);
    file->infile = AllocFileEntry(link, ptr, link.UsrLibPath);
    file->next = *c.last_lib_file;
    *c.last_lib_file = file;
    c.last_lib_file = &file->next;
    if (*c.last_lib_file == nullptr) {   // no file directives found yet
        state().current_file_list = c.last_lib_file;
    }
    file->infile->status |= INSTAT_USE_LIBPATH;
    MemFree(ptr);
    return true;
}

bool ProcLibFile(Link& link) {
    if ((link.LinkFlags & (LF_DWARF_DBI_FLAG | LF_OLD_DBI_FLAG | LF_NOVELL_DBI_FLAG)) == 0) {
        cmd_flags() |= Flag::files_before_debug;
    }
    CommonState& c = common_state();
    if (c.last_lib_file == nullptr) c.last_lib_file = &link.Root->files;
    return process_list(link, AddLibFile, Want::file_name);
}

bool AddModFile(Link& link) {
    char* membname;
    char* ptr = file_name(link, &membname);
    AddHTableElem(link.Root->modFilesHashed, ptr);
    link.LinkFlags |= LF_GOT_CHGD_FILES;
    if (membname != nullptr) MemFree(membname);
    return true;
}

bool AddFile(Link& link) {
    char* membname;
    file_list** file;
    CommonState& c = common_state();
    file_list**& current = state().current_file_list;

    cmd_flags() &= ~Flag::member_added;
#ifdef _EXE
    if (any(cmd_flags() & Flag::auto_section)) {
        if (any(cmd_flags() & Flag::section_there)) {   // is section there already?
            cmd_flags() &= ~Flag::section_there;
        } else {
            MakeNewSection(link);
        }
    }
#endif
    char* ptr = file_name(link, &membname);
    if (membname == nullptr && exe_name() == nullptr) {
        exe_name() = MemToStringSafe(link, reader().text().data(), reader().text().size());
    }
    file = current;
    if (*current != nullptr) current = &(*current)->next;
    c.last_file.u.file = (file_list*)(AddObjFile(link, ptr, membname, current));
    if (any(cmd_flags() & Flag::member_added)) {
        current = file;                 // go back to previous entry.
    } else if (membname != nullptr) {      // 1st member added
        c.last_file.u.member = c.last_file.u.file->u.member;
        cmd_flags() |= Flag::member_added;
    }
    MemFree(membname);
    MemFree(ptr);
    return true;
}

bool ProcFiles(Link& link) {
    if ((link.LinkFlags & (LF_DWARF_DBI_FLAG | LF_OLD_DBI_FLAG | LF_NOVELL_DBI_FLAG)) == 0) {
        cmd_flags() |= Flag::files_before_debug;
    }
    return process_list(link, AddFile, Want::file_name);
}

bool ProcModFiles(Link& link) { return process_list(link, AddModFile, Want::file_name); }

bool AddLib(Link& link) {
    char* ptr = FileName(link, reader().text().data(), reader().text().size(), E_LIBRARY, false);
    file_list* file = AddObjLib(link, ptr, LIB_PRIORITY_MAX);
    file->flags_file |= STAT_USER_SPECD;
#ifdef _EXE
    if (any(cmd_flags() & Flag::set_section)) {
        file->flags_file |= STAT_LIB_FIXED;
        file->ovlref = GetOvlRef(link);
    }
#endif
    if (any(cmd_flags() & Flag::doing_optlib)) {
        file->infile->status |= INSTAT_NO_WARNING;
    }
    DEBUG((DBG_BASE, "library: %s", ptr));
    MemFree(ptr);
    return true;
}

bool ProcOptLib(Link& link) {
    cmd_flags() |= Flag::doing_optlib;
    const bool retval = ProcLibrary(link);
    cmd_flags() &= ~Flag::doing_optlib;
    return retval;
}

bool ProcLibPath(Link& link) {
    if (!reader().get(Sep::none, Want::file_name)) {
        LnkMsg(WRN + LOC + LINE + MSG_VALUE_INCORRECT, "s", "LIBPATH");
        return true;
    }
    AddLibPaths(link, reader().text().data(), reader().text().size(), true);   // true == add to front.
    return true;
}

bool ProcPath(Link& link) {
    if (!reader().get(Sep::none, Want::file_name)) return false;
    const std::string_view text = reader().text();
    path_entry* new_path = (path_entry*)(MemAllocSafe(sizeof(path_entry) + text.size()));
    const char* p_in = text.data();
    const char* end = p_in + text.size();
    char* p = new_path->name;
    while (p_in != end) {
        if (p != new_path->name) *p++ = PATH_LIST_SEP;
        p_in = GetPathElement(p_in, end, &p);
    }
    *p = '\0';
    new_path->next = link.ObjPath;
    link.ObjPath = new_path;
    DEBUG((DBG_BASE, "path: %s", new_path->name));
    return true;
}

bool ProcMap(Link& link) {
    link.MapFlags |= MAP_FLAG;
    if (reader().get(Sep::equals, Want::file_name)) {
        if (link.MapFName != nullptr) MemFree(link.MapFName);
        link.MapFName = FileName(link, reader().text().data(), reader().text().size(), E_MAP, false);
        DEBUG((DBG_OLD, "produce map file"));
    }
    return true;
}

bool ProcMapLines(Link& link) {
    link.MapFlags |= MAP_LINES;
    return true;
}

bool ProcStack(Link& link) {
    unsigned_32 value;
    link.LinkFlags |= LF_STK_SIZE_FLAG;
    const bool ret = get_number(value);
    if (ret) link.StackSize = value;
    return ret;
}

bool ProcNameLen(Link& link) {
    unsigned_32 value;
    const bool ret = get_number(value);
    if (ret) {
        if (value == 0) {
            LnkMsg(WRN + LOC + LINE + MSG_VALUE_INCORRECT, "s", "NAMELEN");
        } else {
            link.NameLen = value;
        }
    }
    return ret;
}

bool ProcCase(Link& link) {
    link.LinkFlags |= LF_CASE_FLAG;
    SetSymCase(link);
    DEBUG((DBG_OLD, "case"));
    return true;
}

bool ProcNoCaseExact(Link& link) {
    link.LinkFlags &= ~LF_CASE_FLAG;
    SetSymCase(link);
    DEBUG((DBG_OLD, "nocase"));
    return true;
}

bool ProcNoExtension(Link& link) {
    cmd_flags() |= Flag::no_extension;
    return true;
}

bool ProcNoCache(Link& link) {
    link.LinkFlags &= ~LF_CACHE_FLAG;
    link.LinkFlags |= LF_NOCACHE_FLAG;
    return true;
}

bool ProcCache(Link& link) {
    link.LinkFlags &= ~LF_NOCACHE_FLAG;
    link.LinkFlags |= LF_CACHE_FLAG;
    return true;
}

// disable an error message number
bool AddDisable(Link& link) {
    unsigned_16 value = 0;
    if (number16(value) == Number::ok) {
        value = value % 1000;
        if (value <= MSG_MAX_ERR_MSG_NUM) {
            ClearBit(link.MsgFlags, value);
        } else {
            LnkMsg(WRN + LOC + LINE + MSG_INVALID_MSG_NUM, nullptr);
        }
    } else {
        LnkMsg(WRN + LOC + LINE + MSG_INVALID_MSG_NUM, nullptr);
    }
    return true;
}

bool ProcDisable(Link& link) { return process_list(link, AddDisable, Want::normal); }

bool ProcNoDefLibs(Link& link) { return true; }

bool ProcVerbose(Link& link) {
    link.MapFlags |= MAP_VERBOSE;
    DEBUG((DBG_OLD, "verbose map file"));
    return true;
}

bool ProcUndefsOK(Link& link) {
    link.LinkFlags |= LF_UNDEFS_ARE_OK;
    DEBUG((DBG_OLD, "undefined symbols are OK"));
    return true;
}

bool ProcNoUndefsOK(Link& link) {
    link.LinkFlags &= ~LF_UNDEFS_ARE_OK;
    return true;
}

bool ProcRedefsOK(Link& link) {
    link.LinkFlags |= LF_REDEFS_OK;
    return true;
}

bool ProcNoRedefs(Link& link) {
    link.LinkFlags &= ~LF_REDEFS_OK;
    return true;
}

bool ProcCVPack(Link& link) {
    link.LinkFlags |= LF_CVPACK_FLAG;
    return true;
}

bool ProcHLLPack(Link& link) {
    link.LinkFlags |= LF_HLLPACK_FLAG;
    return true;
}

bool ProcIncremental(Link& link) {
#if !defined(__DOS__)
    if (any(cmd_flags() & Flag::after_incremental)) {
        LnkMsg(ERR + LOC + LINE + MSG_INC_NEAR_START, nullptr);
    }
    link.LinkFlags |= LF_INC_LINK_FLAG;
    if (reader().get(Sep::equals, Want::file_name)) {
        link.IncFileName = FileName(link, reader().text().data(), reader().text().size(), E_ILK, false);
    } else if (exe_name() != nullptr) {
        link.IncFileName = FileName(link, exe_name(), strlen(exe_name()), E_ILK, true);
    } else {
        link.IncFileName = MemStrdupSafe(link, DEFAULT_INC_NAME);
    }
    ReadPermData(link);
#endif
    return true;
}

bool ProcQuiet(Link& link) {
    link.LinkFlags |= LF_QUIET_FLAG;
    return true;
}

bool ProcMangledNames(Link& link) {
    link.LinkFlags |= LF_DONT_UNMANGLE;
    return true;
}

bool ProcOpResource(Link& link) {
    Reader& r = reader();
    if (r.get(Sep::equals, Want::file_name)) {
        link.FmtData.res_name_only = true;
        link.FmtData.resource = MemToStringSafe(link, r.text().data(), r.text().size());
    } else if (r.get(Sep::none, Want::include_dot)) {
        link.FmtData.resource = MemToStringSafe(link, r.text().data(), r.text().size());
    } else {
        return false;
    }
    return true;
}

bool ProcStatics(Link& link) {
    link.MapFlags |= MAP_STATICS;
    return true;
}

bool ProcArtificial(Link& link) {
    link.MapFlags |= MAP_ARTIFICIAL;
    return true;
}

bool ProcAlphabetical(Link& link) {
    link.MapFlags |= MAP_ALPHA;
    return true;
}

bool ProcGlobal(Link& link) {
    link.MapFlags |= MAP_GLOBAL;
    return true;
}

bool ProcJapanese(Link& link) {
    cmd_flags() |= Flag::language_japanese;
    return true;
}

bool ProcChinese(Link& link) {
    cmd_flags() |= Flag::language_chinese;
    return true;
}

bool ProcKorean(Link& link) {
    cmd_flags() |= Flag::language_korean;
    return true;
}

bool ProcShowDead(Link& link) {
    link.LinkFlags |= LF_SHOW_DEAD;
    return true;
}

bool ProcVFRemoval(Link& link) {
    link.LinkFlags |= LF_VF_REMOVAL;
    return true;
}

bool ProcStart(Link& link) {
    if (!reader().get(Sep::equals, Want::include_dot)) return false;
    link.StartInfo.user_specd = true;
    const std::string name = reader().str();
    SetStartSym(link, name.c_str());
    return true;
}

bool GetPackValue(unsigned_32* value, const char* message) {
    if (!have_equals(Want::normal)) return false;
    if (number(*value) != Number::ok) {
        LnkMsg(WRN + LOC + LINE + MSG_VALUE_INCORRECT, "s", message);
        return true;
    }
    if (*value == 0) *value = 1;
    return true;
}

bool ProcPackcode(Link& link) {
    unsigned_32 value;
    if (GetPackValue(&value, "PACKCODE")) {
        link.PackCodeLimit = value;
        link.LinkFlags |= LF_PACKCODE_FLAG;
        return true;
    }
    return false;
}

bool ProcPackdata(Link& link) {
    unsigned_32 value;
    if (GetPackValue(&value, "PACKDATA")) {
        link.PackDataLimit = value;
        link.LinkFlags |= LF_PACKDATA_FLAG;
        return true;
    }
    return false;
}

#if defined(_OS2) || defined(_EXE) || defined(_QNX)
// force the start of a new auto-group after the previous object file.
bool ProcNewSegment(Link& link) {
    CommonState& c = common_state();
    if (c.last_file.u.file == nullptr) {
        LnkMsg(WRN + LOC + LINE + MSG_NEWSEG_BEFORE_OBJ, nullptr);
    } else {
        if (any(cmd_flags() & Flag::member_added)) {
            c.last_file.u.member->flags_mod |= MOD_LAST_SEG;
        } else {
            c.last_file.u.file->flags_file |= STAT_LAST_SEG;
        }
    }
    return true;
}
#endif

sysblock* FindSystemBlock(const char* name) {
    sysblock* tmpblk = FindSysBlock(name);
    if (tmpblk == nullptr) {
        const size_t len = strlen(name);
        for (sysblock* sys = state().sys_blocks; sys != nullptr; sys = sys->next) {
            if (sys->name != nullptr && strnicmp(sys->name, name, len) == 0) {
                if (tmpblk == nullptr) {
                    tmpblk = sys;
                } else {
                    return nullptr;
                }
            }
        }
    }
    return tmpblk;
}

bool ProcSysDelete(Link& link) { return true; }

void CopyBlocks(void* copyp, const char* data, size_t size) {
    char** copyptr = (char**)(copyp);
    memcpy(*copyptr, data, size);
    *copyptr += size;
}

bool ProcSysEnd(Link& link) { return true; }

bool ProcEndLink(Link& link) { return true; }

constexpr Entry SysEndOptions[] = {
    {"End", ProcSysEnd, MK_ALL},
};

constexpr Entry EndLinkOpt[] = {
    {"ENDLink", ProcEndLink, MK_ALL},
};

// The text up to the table's keyword, token by token with a space after each (the quirk), kept as a system block.
void GetCommandBlock(Link& link, sysblock** hdr, const char* name, Table endtab) {
    stringtable strtab;
    InitStringTable(&strtab, false);
    AddBufferStringTable(&strtab, &strtab, offsetof(sysblock, commands));
    while (!process_one(link, endtab, Sep::space)) {
        reader().forget_push_back();
        AddBufferStringTable(&strtab, reader().text().data(), reader().text().size());
        AddCharStringTable(&strtab, ' ');
        reader().forward();
    }
    AddCharStringTable(&strtab, '\0');
    char* copyptr = (char*)(MemAllocSafe(GetStringTableSize(&strtab)));
    sysblock* sys = (sysblock*)copyptr;
    WriteStringTable(&strtab, CopyBlocks, &copyptr);
    FiniStringTable(&strtab);
    if (name != nullptr) {
        sys->name = MemStrdupSafe(link, name);
    } else {
        sys->name = nullptr;
    }
    AppendList((void**)hdr, sys);
}

char** getStubNamePtr(Link& link) {
#ifdef _OS2
    if (HintFormat(link, MK_NE | MK_OS2_FLAT | MK_PE | MK_WIN_VXD)) {
        return &link.FmtData.u.os2fam.stub_file_name;
    }
#endif
#ifdef _DOS16M
    if (HintFormat(link, MK_DOS16M)) {
        return &link.FmtData.u.d16m.stub;
    }
#endif
#ifdef _PHARLAP
    if (HintFormat(link, MK_PHAR_LAP)) {
        link.FmtData.def_ext = E_LOAD;   // want .exe instead of .exp now
        return &link.FmtData.u.phar.stub;
    }
#endif
    return nullptr;
}

bool ProcStub(Link& link) {
    if (!have_equals(Want::file_name)) return false;
    char** nameptr = getStubNamePtr(link);
    if (nameptr == nullptr) return false;
    char* name = FileName(link, reader().text().data(), reader().text().size(), E_LOAD, false);
    if (*nameptr == nullptr) {
        *nameptr = name;
    } else if (in_system_block()) {
        // if we're in a system block, we don't want to override a user specification
        MemFree(name);
    } else {
        MemFree(*nameptr);
        *nameptr = name;
    }
    return true;
}

bool ProcVersion(Link& link) {
    Version vb;   // required limits, 0 = no limit
    vb.major = 100;
    vb.minor = 100;
    vb.revision = 100;
    vb.what = "VERSION";
    const VersionPart result = get_version(vb, VersionPart::major | VersionPart::minor | VersionPart::revision, true);
    if (result != VersionPart::none) {
        link.FmtData.major = vb.major;
        link.FmtData.minor = vb.minor;
        link.FmtData.revision = vb.revision;
        link.FmtData.ver_specified = true;
        return true;
    }
    return false;
}

bool ProcImplib(Link& link) {
    link.FmtData.make_implib = true;
    link.FmtData.make_impfile = false;
    MemFree(link.FmtData.implibname);
    if (reader().get(Sep::equals, Want::file_name)) {
        link.FmtData.implibname = MemToStringSafe(link, reader().text().data(), reader().text().size());
    }
    return true;
}

bool ProcImpFile(Link& link) {
    link.FmtData.make_implib = true;
    link.FmtData.make_impfile = true;
    MemFree(link.FmtData.implibname);
    if (reader().get(Sep::equals, Want::file_name)) {
        link.FmtData.implibname = MemToStringSafe(link, reader().text().data(), reader().text().size());
    }
    return true;
}

bool AddSymTrace(Link& link) {
    AddTraceListSym(MemToStringSafe(link, reader().text().data(), reader().text().size()));
    return true;
}

bool ProcSymTrace(Link& link) {
    link.LinkFlags |= LF_TRACE_FLAG;
    return process_list(link, AddSymTrace, Want::include_dot);
}

bool AddModTrace(Link& link) {
    char* membname;
    char* fname = file_name(link, &membname);
    // AddTraceListMod consumes membname: not freed here
    AddTraceListMod(fname, membname);
    return true;
}

bool ProcModTrace(Link& link) {
    link.LinkFlags |= LF_TRACE_FLAG;
    return process_list(link, AddModTrace, Want::file_name);
}

bool ProcFarCalls(Link& link) {
    link.LinkFlags |= LF_FAR_CALLS_FLAG;
    DEBUG((DBG_OLD, "Far Calls optimization"));
    return true;
}

bool ProcNoFarCalls(Link& link) {
    link.LinkFlags &= ~LF_FAR_CALLS_FLAG;
    DEBUG((DBG_OLD, "No Far Calls optimization"));
    return true;
}

#ifdef _RAW
bool ProcOutputRaw(Link& link) {
    link.FmtData.output_raw = true;
    return true;
}

bool ProcOutputHex(Link& link) {
    link.FmtData.output_hex = true;
    return true;
}

bool ProcOutputStart(Link& link) {
    link.FmtData.output_start = true;
    return true;
}

bool ProcOutputOffset(Link& link) {
    unsigned_32 value = 0;
    if (!reader().get(Sep::equals, Want::normal)) return false;
    const Number retval = number(value);
    if (retval == Number::ok && (value <= (0xFFFFUL << link.FmtData.SegShift) || HintFormat(link, ~MK_SEGMENTED))) {
        link.FmtData.output_offset = value;
        return true;
    }
    LnkMsg(WRN + LOC + LINE + MSG_VALUE_INCORRECT, "s", "OUTPUT OFFSET");
    return false;
}

bool ProcOutputHshift(Link& link) {
    unsigned_16 value = 0;
    if (!reader().get(Sep::equals, Want::normal)) return false;
    const Number ret = number16(value);
    if (ret == Number::ok && value < 16) {
        link.FmtData.HexSegShift = 16 - value;
        link.FmtData.output_hshift = true;
        return true;
    }
    LnkMsg(WRN + LOC + LINE + MSG_VALUE_TOO_LARGE, "s", "OUTPUT HSHIFT");
    return false;
}
#endif

bool ProcHshift(Link& link) {
    unsigned_16 value = 0;
    if (!reader().get(Sep::equals, Want::normal)) return false;
    const Number ret = number16(value);
    if (ret == Number::ok && value < 16) {
        link.FmtData.Hshift = value;
        SetSegMask(link);
        return true;
    }
    LnkMsg(WRN + LOC + LINE + MSG_VALUE_TOO_LARGE, "s", "HSHIFT");
    return false;
}

bool ProcFillchar(Link& link) {
    unsigned_16 value = 0;
    if (!reader().get(Sep::equals, Want::normal)) return false;
    const Number ret = number16(value);
    if (ret == Number::ok && value < 256) {
        link.FmtData.FillChar = value;
        return true;
    }
    LnkMsg(WRN + LOC + LINE + MSG_VALUE_TOO_LARGE, "s", "FILLCHAR");
    return false;
}

bool ProcOrdSegAdr(Link& link) {
    ORDER_CLASS* oc = common_state().current_order_class;
    unsigned_16 value = 0;
    if (!reader().get(Sep::equals, Want::normal)) return false;
    if (number16(value) == Number::ok) {
        oc->Base.seg = value;
        oc->FixedAddr = true;
        return true;
    }
    oc->FixedAddr = false;
    LnkMsg(WRN + LOC + LINE + MSG_ADDR_INFO_TOO_LARGE, nullptr);
    return false;
}

bool ProcOrdOfsAdr(Link& link) {
    ORDER_CLASS* oc = common_state().current_order_class;
    unsigned_32 value = 0;
    if (!reader().get(Sep::equals, Want::normal)) return false;
    const Number retval = number(value);
    if (retval == Number::ok && (value <= 0xFFFFL || HintFormat(link, ~MK_SEGMENTED))) {
        oc->Base.off = value;
        oc->FixedAddr = true;
        return true;
    }
    oc->FixedAddr = false;
    LnkMsg(WRN + LOC + LINE + MSG_VALUE_INCORRECT, "s", "ORDER CLNAME OFFSET");
    return false;
}

bool ProcOrdCopy(Link& link) {
    if (!reader().get(Sep::none, Want::include_dot)) return false;
    ORDER_CLASS* oc = common_state().current_order_class;
    oc->SrcName = MemToStringSafe(link, reader().text().data(), reader().text().size());
    oc->Copy = true;
    return true;
}

bool ProcOrdNoEmit(Link& link) {
    common_state().current_order_class->NoEmit = true;
    return true;
}

bool ProcOrdSegSegAdr(Link& link) {
    ORDER_SEGMENT* os = common_state().current_order_segment;
    unsigned_16 value = 0;
    if (!reader().get(Sep::equals, Want::normal)) return false;
    if (number16(value) == Number::ok) {
        os->Base.seg = value;
        os->FixedAddr = true;
        return true;
    }
    os->FixedAddr = false;
    LnkMsg(WRN + LOC + LINE + MSG_ADDR_INFO_TOO_LARGE, nullptr);
    return false;
}

bool ProcOrdSegOfsAdr(Link& link) {
    ORDER_SEGMENT* os = common_state().current_order_segment;
    unsigned_32 value = 0;
    if (!reader().get(Sep::equals, Want::normal)) return false;
    const Number retval = number(value);
    if (retval == Number::ok && (value <= 0xFFFFL || HintFormat(link, ~MK_SEGMENTED))) {
        os->Base.off = value;
        os->FixedAddr = true;
        return true;
    }
    os->FixedAddr = false;
    LnkMsg(WRN + LOC + LINE + MSG_VALUE_INCORRECT, "s", "ORDER CLNAME SEGMENT OFFSET");
    return false;
}

bool ProcOrdSegNoEmit(Link& link) {
    common_state().current_order_segment->NoEmit = true;
    return true;
}

bool ProcObjAlign(Link& link) {
    unsigned_32 value = 0;
    if (!have_equals(Want::normal)) return false;
    const Number ret = number(value);
    if (ret != Number::ok || value == 0) return false;
    if (value < 16 || value > _256M || (value & (value - 1))) {   // value not a power of 2
        LnkMsg(WRN + LOC + LINE + MSG_VALUE_INCORRECT, "s", "OBJALIGN");
        value = _64K;
    }
    link.FmtData.objalign = value;
    ChkBase(link, value);
    return true;
}

bool ProcDescription(Link& link) {
    if (!reader().get(Sep::none, Want::include_dot)) return false;
    if (link.FmtData.description != nullptr) MemFree(link.FmtData.description);
    link.FmtData.description = MemToStringSafe(link, reader().text().data(), reader().text().size());
    return true;
}

#if defined(_OS2) || defined(_NOVELL) || defined(_ELF)
bool ProcImport(Link& link) {
#ifdef _OS2
    if (HintFormat(link, MK_NE | MK_OS2_FLAT | MK_PE)) return ProcOS2Import(link);
#endif
#ifdef _ELF
    if (HintFormat(link, MK_ELF)) return ProcELFImport(link);
#endif
#ifdef _NOVELL
    if (HintFormat(link, MK_NOVELL)) return ProcNovImport(link);
#endif
    return false;
}

bool ProcExport(Link& link) {
#ifdef _OS2
    if (HintFormat(link, MK_NE | MK_OS2_FLAT | MK_PE | MK_WIN_VXD)) return ProcOS2Export(link);
#endif
#ifdef _ELF
    if (HintFormat(link, MK_ELF)) return ProcELFExport(link);
#endif
#ifdef _NOVELL
    if (HintFormat(link, MK_NOVELL)) return ProcNovExport(link);
#endif
    return false;
}
#endif

#if defined(_DOS16M) || defined(_QNX) || defined(_OS2) || defined(_ELF)
bool ProcNoRelocs(Link& link) {
#ifdef _QNX
    if (HintFormat(link, MK_QNX)) return ProcQNXNoRelocs(link);
#endif
#ifdef _OS2
    if (HintFormat(link, MK_PE)) return ProcOS2NoRelocs(link);
#endif
#ifdef _DOS16M
    if (HintFormat(link, MK_DOS16M)) return Proc16MNoRelocs(link);
#endif
#ifdef _ELF
    if (HintFormat(link, MK_ELF)) return ProcELFNoRelocs(link);
#endif
    return true;
}
#endif

#if defined(_OS2) || defined(_QNX)
bool ProcSegment(Link& link) {
#ifdef _OS2
    if (HintFormat(link, MK_NE | MK_OS2_FLAT | MK_PE | MK_WIN_VXD)) return ProcOS2Segment(link);
#endif
#ifdef _QNX
    if (HintFormat(link, MK_QNX)) return ProcQNXSegment(link);
#endif
    return true;
}
#endif

#if defined(_OS2) || defined(_ELF)
bool ProcAlignment(Link& link) {
#ifdef _OS2
    if (HintFormat(link, MK_NE | MK_OS2_LX | MK_PE)) return ProcOS2Alignment(link);
#endif
#ifdef _ELF
    if (HintFormat(link, MK_ELF)) return ProcELFAlignment(link);
#endif
    return true;
}
#endif

#if defined(_OS2) || defined(_QNX)
bool ProcHeapSize(Link& link) {
#if defined(_QNX) && defined(__QNX__)
    if (HintFormat(MK_QNX)) return ProcQNXHeapSize();
#endif
#ifdef _OS2
    if (HintFormat(link, MK_NE | MK_OS2_FLAT | MK_PE)) return ProcOS2HeapSize(link);
#endif
#if defined(_QNX) && !defined(__QNX__)
    if (HintFormat(link, MK_QNX)) return ProcQNXHeapSize(link);
#endif
    return true;
}
#endif

#if defined(_PHARLAP) || defined(_QNX) || defined(_OS2) || defined(_ELF) || defined(_RAW)
bool ProcOffset(Link& link) {
    if (!get_number(link.FmtData.base)) return false;
#ifdef _PHARLAP
    if (link.FmtData.type & MK_PHAR_LAP) {
        ChkBase(link, _4K);
        return true;
    }
#endif
#ifdef _QNX
    if (link.FmtData.type & MK_QNX_FLAT) {
        ChkBase(link, _4K);
        return true;
    }
#endif
#ifdef _RAW
    if (link.FmtData.type & MK_RAW) {
        ChkBase(link, 256);
        return true;
    }
#endif
    ChkBase(link, _64K);
    return true;
}
#endif

#if defined(_ELF) || defined(_NOVELL)
bool ProcModule(Link& link) {
#ifdef _ELF
    if (HintFormat(link, MK_ELF)) return ProcELFModule(link);
#endif
#ifdef _NOVELL
    if (HintFormat(link, MK_NOVELL)) return ProcNovModule(link);
#endif
    return false;
}
#endif

#ifdef DEVBUILD
bool ProcXDbg() {
    if (reader().get(Sep::equals, Want::include_dot)) {
        if (reader().text().size() > 6) return false;
        const std::string value = reader().str();
        Debug = strtoul(value.c_str(), nullptr, 0);
        DEBUG((DBG_BASE, "debugging info type = %x", Debug));
        return true;
    }
    return false;
}

bool ProcIntDbg() {
    wlink().LinkState |= LS_INTERNAL_DEBUG;
    return true;
}
#endif

constexpr Entry OrderSegOpts[] = {
    {"SEGAddr", ProcOrdSegSegAdr, MK_ALL},
    {"OFFset", ProcOrdSegOfsAdr, MK_ALL},
    {"NOEmit", ProcOrdSegNoEmit, MK_ALL},
};

bool ProcOrdSeg(Link& link) {
    if (!reader().get(Sep::none, Want::include_dot)) return false;
    CommonState& c = common_state();
    c.current_order_segment = (ORDER_SEGMENT*)(MemAllocSafe(sizeof(ORDER_SEGMENT)));
    c.current_order_segment->NextSeg = c.current_order_class->SegList;
    c.current_order_class->SegList = c.current_order_segment;
    c.current_order_segment->Name = MemToStringSafe(link, reader().text().data(), reader().text().size());
    c.current_order_segment->FixedAddr = false;
    c.current_order_segment->NoEmit = false;
    while (process_one(link, OrderSegOpts, Sep::none)) {
    }
    return true;
}

constexpr Entry OrderClassOpts[] = {
    {"SEGAddr", ProcOrdSegAdr, MK_ALL},
    {"OFFset", ProcOrdOfsAdr, MK_ALL},
    {"COpy", ProcOrdCopy, MK_ALL},
    {"NOEmit", ProcOrdNoEmit, MK_ALL},
    {"SEGMent", ProcOrdSeg, MK_ALL},
};

bool ProcOrdClass(Link& link) {
    if (!reader().get(Sep::none, Want::include_dot)) return false;
    CommonState& c = common_state();
    link.LinkState |= LS_SPEC_ORDER_FLAG;
    ORDER_CLASS* last = c.current_order_class;
    c.current_order_class = (ORDER_CLASS*)(MemAllocSafe(sizeof(ORDER_CLASS)));
    ORDER_CLASS* cur = c.current_order_class;
    if (last == nullptr) {
        link.CurrSect->orderlist = cur;
    } else {
        last->NextClass = cur;
    }
    cur->Name = MemToStringSafe(link, reader().text().data(), reader().text().size());
    cur->NextClass = nullptr;
    cur->Ring = nullptr;
    cur->SegList = nullptr;
    c.current_order_segment = nullptr;
    cur->FixedAddr = false;
    cur->Copy = false;
    cur->NoEmit = false;
    while (process_one(link, OrderClassOpts, Sep::none)) {
    }
    return true;
}

constexpr Entry OrderOpts[] = {
    {"CLName", ProcOrdClass, MK_ALL},
};

bool ProcOrder(Link& link) {
    bool ret = false;
    common_state().current_order_class = link.CurrSect->orderlist;
    if (common_state().current_order_class != nullptr) {
        LnkMsg(WRN + LOC + LINE + MSG_DUP_DIRECTIVE, "s", "OPTION");
    }
    while (process_one(link, OrderOpts, Sep::none)) ret = true;
    return ret;
}

#ifdef _RAW
constexpr Entry OutputOpts[] = {
    {"RAW", ProcOutputRaw, MK_ALL},
    {"HEX", ProcOutputHex, MK_ALL},
    {"OFFset", ProcOutputOffset, MK_ALL},
    {"HSHIFT", ProcOutputHshift, MK_ALL},
    {"STartrec", ProcOutputStart, MK_ALL},
};

bool ProcOutput(Link& link) {
    link.FmtData.output_offset = 0L;
    link.FmtData.output_hshift = false;
    link.FmtData.output_start = false;
    bool ret = false;
    while (process_one(link, OutputOpts, Sep::none)) ret = true;
    return ret;
}
#endif

#if defined(_PHARLAP) || defined(_DOS16M) || defined(_OS2) || defined(_ELF)
bool AddRunTime(Link& link) {
    reader().push_back();   // reparse last token.
#ifdef _DOS16M
    if (Proc16MRuntime(link)) return true;
#endif
#ifdef _PHARLAP
    if (ProcPharRuntime(link)) return true;
#endif
#ifdef _OS2
    if (ProcOS2Runtime(link)) return true;
#endif
#ifdef _ELF
    if (ProcELFRuntime(link)) return true;
#endif
    return false;
}

bool ProcRuntime(Link& link) { return process_list(link, AddRunTime, Want::include_dot); }
#endif

// save up list of commands to process later
bool ProcStartLink(Link& link) {
    GetCommandBlock(link, &state().link_commands, nullptr, EndLinkOpt);
    return true;
}

// parse a system begin block and store it somewhere
bool ProcSysBegin(Link& link) {
    if (!reader().get(Sep::none, Want::include_dot)) {
        LnkMsg(WRN + LOC + LINE + MSG_VALUE_INCORRECT, "s", "SYSTEM BEGIN");
        return true;
    }
    char* sysname = MemToStringSafe(link, reader().text().data(), reader().text().size());
    sysblock* sys = FindSysBlock(sysname);
    if (sys != nullptr) {
        LnkMsg(WRN + LOC + LINE + MSG_SYSTEM_ALREADY_DEFINED, "s", sys->name);
        while (!process_one(link, SysEndOptions, Sep::space)) {
            reader().forget_push_back();
            reader().forward();
        }
    } else {
        GetCommandBlock(link, &state().sys_blocks, sysname, SysEndOptions);
    }
    MemFree(sysname);
    return true;
}

constexpr Entry SysBeginOptions[] = {
    {"Begin", ProcSysBegin, MK_ALL},
};

constexpr Entry SysDeleteOptions[] = {
    {"DELete", ProcSysDelete, MK_ALL},
};

// the system directive
bool ProcSystem(Link& link) {
    if (process_one(link, SysBeginOptions, Sep::none)) return true;
    const bool dodelete = process_one(link, SysDeleteOptions, Sep::none);
    if (dodelete) {
        if (!reader().get(Sep::none, Want::include_dot)) return false;
    } else {
        reader().forget_push_back();
    }
    char* sysname = MemToStringSafe(link, reader().text().data(), reader().text().size());
    sysblock* sys = FindSystemBlock(sysname);
    if (sys == nullptr) {
        LnkMsg(WRN + LOC + LINE + MSG_SYSTEM_UNDEFINED, "s", sysname);
    } else {
        if (dodelete) {
            sysblock** prev;
            for (prev = &state().sys_blocks; *prev != sys;) prev = &((*prev)->next);
            *prev = sys->next;
            if (sys->name != nullptr) MemFree(sys->name);
            MemFree(sys);
        } else {
            reader().forward();
            reader().push(sys->name, sys->commands, SourceKind::system);
        }
    }
    MemFree(sysname);
    return true;
}

constexpr Entry Languages[] = {
    {"JApanese", ProcJapanese, MK_ALL},
    {"CHinese", ProcChinese, MK_ALL},
    {"KOrean", ProcKorean, MK_ALL},
};

bool ProcLanguage(Link& link) {
    cmd_flags() &= ~Flag::language_mask;
    return process_one(link, Languages, Sep::none);
}

constexpr Entry SortOptions[] = {
    {"ALPhabetical", ProcAlphabetical, MK_ALL},
    {"GLobal", ProcGlobal, MK_ALL},
};

bool ProcSort(Link& link) {
    link.MapFlags |= MAP_SORT;
    if (!process_one(link, SortOptions, Sep::none)) return true;
    process_one(link, SortOptions, Sep::none);
    return true;
}

constexpr Entry PosDbgMods[] = {
    {"LInes", ProcLine, MK_ALL},
    {"Types", ProcType, MK_ALL},
    {"LOcals", ProcLocal, MK_ALL},
    {"All", ProcAll, MK_ALL},
#ifdef _NOVELL
    {"ONLyexports", ProcNovExportsDBI, MK_NOVELL},
#endif
};

constexpr Entry DbgMods[] = {
    {"Watcom", ProcWatcomDBI, MK_ALL},
    {"Dwarf", ProcDwarfDBI, MK_ALL},
    {"Codeview", ProcCodeviewDBI, MK_ALL},
    {"Hll", ProcHllDBI, MK_ALL},
#ifdef _NOVELL
    {"Novell", ProcNovDBI, MK_NOVELL},
#endif
};

bool ProcDebug(Link& link) {
    if (any(cmd_flags() & Flag::files_before_debug)) {
        LnkMsg(WRN + LOC + LINE + MSG_DEBUG_AFTER_FILES, nullptr);
    }
    const bool gotmod = process_one(link, DbgMods, Sep::none);
    link.DBIFlag = (dbi_flags)(0);
    if (process_one(link, PosDbgMods, Sep::none)) {
        while (process_one(link, PosDbgMods, Sep::comma)) {
        }
    } else {
        link.DBIFlag |= DBI_ALL;
        if (!gotmod) return false;
    }
    return true;
}

constexpr Entry MainOptions[] = {
    {"Map", ProcMap, MK_ALL},
    {"MAPLines", ProcMapLines, MK_ALL},
    {"STack", ProcStack, MK_ALL},
    {"NODefaultlibs", ProcNoDefLibs, MK_ALL, Flag::no_default_libs},
    {"Quiet", ProcQuiet, MK_ALL},
    {"Dosseg", ProcDosSeg, MK_ALL},
    {"Caseexact", ProcCase, MK_ALL},
    {"Verbose", ProcVerbose, MK_ALL},
    {"Undefsok", ProcUndefsOK, MK_ALL},
    {"NOUndefsok", ProcNoUndefsOK, MK_ALL},
    {"NAMELen", ProcNameLen, MK_ALL},
    {"SYMFile", ProcSymFile, MK_ALL, Flag::separate_symbols},
    {"OSName", ProcOSName, MK_ALL},
    {"ELiminate", ProcEliminate, MK_ALL},
    {"FARcalls", ProcFarCalls, MK_ALL},
    {"MAXErrors", ProcMaxErrors, MK_ALL},
    {"NOCASEexact", ProcNoCaseExact, MK_ALL},
    {"NOCAChe", ProcNoCache, MK_ALL},
    {"NOEXTension", ProcNoExtension, MK_ALL},
    {"NOFARcalls", ProcNoFarCalls, MK_ALL},
    {"CAChe", ProcCache, MK_ALL},
    {"MANGlednames", ProcMangledNames, MK_ALL},
    {"OBJAlign", ProcObjAlign, MK_ALL},
    {"RESource", ProcOpResource, MK_ALL},
    {"STATics", ProcStatics, MK_ALL},
    {"START", ProcStart, MK_ALL},
    {"ARTificial", ProcArtificial, MK_ALL},
    {"SHOwdead", ProcShowDead, MK_ALL},
    {"VFRemoval", ProcVFRemoval, MK_ALL},
    {"REDefsok", ProcRedefsOK, MK_ALL},
    {"NOREDefsok", ProcNoRedefs, MK_ALL},
    {"CVPack", ProcCVPack, MK_ALL},
    {"HLLPack", ProcHLLPack, MK_ALL},
    {"INCremental", ProcIncremental, MK_ALL},
    {"FILLchar", ProcFillchar, MK_ALL},
#if defined(_OS2) || defined(_EXE) || defined(_DOS16M) || defined(_QNX)
    {"PACKCode", ProcPackcode, (MK_NE | MK_DOS | MK_QNX | MK_DOS16M)},
    {"PACKData", ProcPackdata, (MK_NE | MK_DOS | MK_QNX | MK_DOS16M)},
#endif
#if defined(_OS2) || defined(_ELF)
    {"Alignment", ProcAlignment, (MK_NE | MK_OS2_LX | MK_PE | MK_ELF)},
#endif
#if defined(_OS2) || defined(_PHARLAP) || defined(_DOS16M)
    {"STUB", ProcStub, (MK_NE | MK_OS2_FLAT | MK_PE | MK_WIN_VXD | MK_PHAR_LAP | MK_DOS16M)},
#endif
#if defined(_OS2) || defined(_NOVELL)
    {"DEscription", ProcDescription, MK_NE | MK_OS2_FLAT | MK_PE | MK_WIN_VXD | MK_NOVELL},
#endif
#if defined(_OS2) || defined(_QNX)
    {"Heapsize", ProcHeapSize, (MK_NE | MK_OS2_FLAT | MK_QNX | MK_PE)},
#endif
#if defined(_PHARLAP) || defined(_QNX) || defined(_OS2) || defined(_RAW)
    {"OFFset", ProcOffset, MK_PHAR_FLAT | MK_OS2_FLAT | MK_WIN_VXD | MK_PE | MK_QNX_FLAT | MK_ELF | MK_RAW},
#endif
#if defined(_OS2) || defined(_NOVELL)
    {"VERSion", ProcVersion, MK_NOVELL | MK_OS2_FLAT | MK_WIN_VXD | MK_PE | MK_WIN_NE},
    {"IMPLib", ProcImplib, MK_NOVELL | MK_NE | MK_OS2_FLAT | MK_PE},
    {"IMPFile", ProcImpFile, MK_NOVELL | MK_NE | MK_OS2_FLAT | MK_PE},
#endif
#if defined(_DOS16M) || defined(_QNX) || defined(_OS2) || defined(_ELF)
    {"NORelocs", ProcNoRelocs, (MK_QNX | MK_DOS16M | MK_PE | MK_ELF)},
#endif
    {"HSHIFT", ProcHshift, (MK_DOS | MK_ALLOW_16)},
};

bool AddOption(Link& link) {
    reader().push_back();
    if (process_one(link, MainOptions, Sep::none)) return true;
#ifdef _EXE
    if (ProcDosOptions(link)) return true;
#endif
#ifdef _ELF
    if (ProcELFOptions(link)) return true;
#endif
#ifdef _DOS16M
    if (Proc16MOptions(link)) return true;
#endif
#ifdef _NOVELL
    if (ProcNovOptions(link)) return true;
#endif
#ifdef _PHARLAP
    if (ProcPharOptions(link)) return true;
#endif
#ifdef _QNX
    if (ProcQNXOptions(link)) return true;
#endif
#ifdef _RDOS
    if (ProcRdosOptions(link)) return true;
#endif
#ifdef _OS2
    if (ProcOS2Options(link)) return true;
#endif
    return false;
}

bool ProcOptions(Link& link) { return process_list(link, AddOption, Want::include_dot); }

constexpr Entry Models[] = {
#ifdef _EXE
    {"Dos", ProcDosFormat, MK_DOS},
#endif
#ifdef _OS2
    {"OS2", ProcOS2Format, MK_OS2},
    {"WINdows", ProcWindowsFormat, MK_WIN_NE | MK_PE | MK_WIN_VXD},
#endif
#ifdef _PHARLAP
    {"PHARlap", ProcPharFormat, MK_PHAR_LAP},
#endif
#ifdef _NOVELL
    {"NOVell", ProcNovFormat, MK_NOVELL},
#endif
#ifdef _QNX
    {"QNX", ProcQNXFormat, MK_QNX},
#endif
#ifdef _DOS16M
    {"DOS16M", Proc16MFormat, MK_DOS16M},
#endif
#ifdef _ELF
    {"ELF", ProcELFFormat, MK_ELF},
#endif
#ifdef _RDOS
    {"RDos", ProcRdosFormat, MK_RDOS},
#endif
#ifdef _RAW
    {"Raw", ProcRawFormat, MK_RAW},
#endif
};

bool ProcFormat(Link& link) {
    if (link.LinkState & LS_FMT_SPECIFIED) {
        LnkMsg(FTL + LOC + LINE + MSG_MULTIPLE_MODES_FOUND, nullptr);
    }
    link.LinkState |= LS_FMT_SPECIFIED;
    return process_one_or_fail(link, Models, Sep::none);
}

// directives with Flag::subset are the only ones that are harmless to run after the files have been processed in pass 1
constexpr Entry Directives[] = {
    {"File", ProcFiles, MK_ALL, Flag::have_files},
    {"MODFile", ProcModFiles, MK_ALL},
    {"Library", ProcLibrary, MK_ALL, Flag::subset},
    {"Name", ProcName, MK_ALL, Flag::subset},
    {"OPtion", ProcOptions, MK_ALL, Flag::subset},
    {"Debug", ProcDebug, MK_ALL},
    {"SYStem", ProcSystem, MK_ALL},
    {"LIBPath", ProcLibPath, MK_ALL, Flag::subset},
    {"LIBFile", ProcLibFile, MK_ALL, Flag::have_files},
    {"Path", ProcPath, MK_ALL},
    {"FORMat", ProcFormat, MK_ALL, Flag::subset},
    {"MODTrace", ProcModTrace, MK_ALL},
    {"SYMTrace", ProcSymTrace, MK_ALL, Flag::after_incremental},
    {"Alias", ProcAlias, MK_ALL, Flag::after_incremental},
    {"REFerence", ProcReference, MK_ALL, Flag::after_incremental},
    {"DISAble", ProcDisable, MK_ALL, Flag::subset},
    {"SOrt", ProcSort, MK_ALL, Flag::subset},
    {"LANGuage", ProcLanguage, MK_ALL},
    {"STARTLink", ProcStartLink, MK_ALL},
    {"OPTLIB", ProcOptLib, MK_ALL},
    {"ORDer", ProcOrder, MK_ALL, Flag::subset},
#ifdef _RAW
    {"OUTput", ProcOutput, MK_ALL, Flag::subset},
#endif
#ifdef _OS2
    {"RESource", ProcResource, MK_PE},
    {"COMmit", ProcCommit, MK_PE},
    {"ANONymousexport", ProcAnonExport, MK_NE | MK_OS2_FLAT, Flag::after_incremental},
#endif
#if defined(_NOVELL) || defined(_OS2) || defined(_ELF)
    {"IMPort", ProcImport, (MK_NOVELL | MK_ELF | MK_NE | MK_OS2_FLAT | MK_PE), Flag::after_incremental},
    {"EXPort", ProcExport, (MK_NOVELL | MK_ELF | MK_NE | MK_OS2_FLAT | MK_PE | MK_WIN_VXD), Flag::after_incremental},
#endif
#if defined(_OS2) || defined(_QNX)
    {"SEGment", ProcSegment, (MK_QNX | MK_NE | MK_OS2_FLAT | MK_PE | MK_WIN_VXD), Flag::subset},
#endif
#ifdef _EXE
    {"OVerlay", ProcOverlay, MK_OVERLAYS},
    {"Begin", ProcBegin, MK_OVERLAYS},
    {"FIXedlib", ProcFixedLib, MK_OVERLAYS},
    {"NOVector", ProcNoVector, MK_OVERLAYS, Flag::after_incremental},
    {"VEctor", ProcVector, MK_OVERLAYS, Flag::after_incremental},
    {"FORCEVEctor", ProcForceVector, MK_OVERLAYS, Flag::after_incremental},
#endif
#if defined(_PHARLAP) || defined(_DOS16M) || defined(_OS2) || defined(_ELF)
    {"RUntime", ProcRuntime, (MK_PHAR_LAP | MK_DOS16M | MK_PE | MK_ELF), Flag::subset},
#endif
#if defined(_NOVELL) || defined(_ELF)
    {"MODUle", ProcModule, MK_NOVELL | MK_ELF},
#endif
#ifdef _DOS16M
    {"MEMory", Proc16MMemory, MK_DOS16M, Flag::subset},
    {"TRansparent", Proc16MTransparent, MK_DOS16M, Flag::subset},
#endif
#if defined(_OS2) || defined(_EXE) || defined(_QNX)
    {"NEWsegment", ProcNewSegment, (MK_NE | MK_DOS | MK_QNX)},
#endif
#ifdef DEVBUILD
    {"Xdbg", ProcXDbg, MK_ALL},
    {"INTDBG", ProcIntDbg, MK_ALL},
#endif
};

}  // namespace

// `library` (DOS's `fixedlib` runs it too)
bool ProcLibrary(Link& link) {
    if ((link.LinkFlags & (LF_DWARF_DBI_FLAG | LF_OLD_DBI_FLAG | LF_NOVELL_DBI_FLAG)) == 0 && !in_system_block()) {
        cmd_flags() |= Flag::files_before_debug;
    }
    return process_list(link, AddLib, Want::file_name);
}

// Note: align must be a power of 2
void ChkBase(Link& link, offset align) {
    if (link.FmtData.objalign != NO_BASE_SPEC && link.FmtData.objalign > align) {
        align = link.FmtData.objalign;
    }
    if (link.FmtData.base != NO_BASE_SPEC && (link.FmtData.base & (align - 1)) != 0) {
        LnkMsg(WRN + LOC + LINE + MSG_OFFSET_MUST_BE_ALIGNED, "l", align);
        link.FmtData.base = __ROUND_UP_SIZE(link.FmtData.base, align);
    }
}

bool DoParseDirectiveSubset(Link& link) { return process_one_subset(link, Directives, Sep::none); }

bool DoParseDirective(Link& link) { return process_one(link, Directives, Sep::none); }

bool DoParseDirectiveSuicide(Link& link) { return process_one_or_fail(link, Directives, Sep::none); }

bool DoMatchDirective(std::string_view word) { return matches(Directives, word); }

outfilelist* NewOutFile(Link& link, char* filename) {
    for (outfilelist* fnode = link.OutFiles; fnode != nullptr; fnode = fnode->next) {
        if (FNAMECMPSTR(filename, fnode->fname) == 0) {
            MemFree(filename);   // don't need this now.
            return fnode;
        }
    }
    // file name not already in list, so add a list entry.
    outfilelist* fnode = (outfilelist*)(MemAllocSafe(sizeof(outfilelist)));
    InitBuffFile(fnode, filename, true);
    fnode->next = link.OutFiles;
    link.OutFiles = fnode;
    return fnode;
}

}  // namespace cb::x86link::directives

// ---- called from outside the directive layer (wlink.cpp, cmdline.cpp)

void ResetCmdAll(Link& link) {
    auto& c = cb::x86link::directives::common_state();
    c.last_file.u.file = nullptr;
    c.last_lib_file = nullptr;
    link.UsrLibPath = nullptr;
}

sysblock* FindSysBlock(const char* name) {
    for (sysblock* sys = cb::x86link::directives::state().sys_blocks; sys != nullptr; sys = sys->next) {
        if (sys->name != nullptr && stricmp(sys->name, name) == 0) return sys;
    }
    return nullptr;
}
