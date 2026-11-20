// build/x86link/directives/common.cpp -- the directives and options common to every format: file, library, name, option,
// debug, system blocks, order, output, sort, language (a port of wlink's cmdall.c, and NewOutFile of cmdutils.c).
#include "build/x86link/directives/common.hpp"

#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <string>

#include "build/x86link/directives/formats.hpp"
#include "build/x86link/directives/state.hpp"
#include "linkstd.h"
#include "alloc.h"
#include "strtab.h"
#include "dbgall.h"
#include "cmdline.h"
#include "symtrace.h"
#include "objio.h"
#include "loadfile.h"
#include "carve.h"
#include "permdata.h"
#include "pathlist.h"
#include "library.h"
#include "sysblock.h"
#include "linkutil.h"
#include "msg.h"
#include "wlnkmsg.h"

#include "clibext.h"

#define DEFAULT_INC_NAME "__wlink.ilk"

namespace cb::x86link::directives {

namespace {

CommonState& common_state() { return state().common; }

bool ProcDosSeg() {
    LinkState |= LS_DOSSEG_FLAG;
    DEBUG((DBG_OLD, "dosseg"));
    return true;
}

bool ProcName() {
    if (!reader().get(Sep::none, Want::file_name)) return false;
    cmd_flags() &= ~Flag::unnamed;
    if (exe_name() != nullptr) MemFree(exe_name());
    exe_name() = MemToStringSafe(reader().text().data(), reader().text().size());   // (just keep the name around for now)
    return true;
}

bool ProcDwarfDBI() {
    if (LinkFlags & (LF_ANY_DBI_FLAG & ~LF_DWARF_DBI_FLAG)) {
        LnkMsg(WRN + LOC + LINE + MSG_MULT_DBI_FORMATS, nullptr);
        return true;
    }
    LinkFlags |= LF_DWARF_DBI_FLAG;
    return true;
}

bool ProcWatcomDBI() {
    if (LinkFlags & (LF_ANY_DBI_FLAG & ~LF_OLD_DBI_FLAG)) {
        LnkMsg(WRN + LOC + LINE + MSG_MULT_DBI_FORMATS, nullptr);
        return true;
    }
    LinkFlags |= LF_OLD_DBI_FLAG;
    return true;
}

bool ProcCodeviewDBI() {
    if (LinkFlags & (LF_ANY_DBI_FLAG & ~LF_CV_DBI_FLAG)) {
        LnkMsg(WRN + LOC + LINE + MSG_MULT_DBI_FORMATS, nullptr);
        return true;
    }
    LinkFlags |= LF_CV_DBI_FLAG;
    return true;
}

bool ProcHllDBI() {
    if (LinkFlags & (LF_ANY_DBI_FLAG & ~LF_HLL_DBI_FLAG)) {
        LnkMsg(LOC + LINE + WRN + MSG_MULT_DBI_FORMATS, nullptr);
        return true;
    }
    LinkFlags |= LF_HLL_DBI_FLAG;
    return true;
}

bool ProcLine() {
    if ((LinkFlags & LF_ANY_DBI_FLAG) == 0) LinkFlags |= LF_DWARF_DBI_FLAG;
    DBIFlag |= DBI_LINE;
    return true;
}

bool ProcType() {
    if ((LinkFlags & LF_ANY_DBI_FLAG) == 0) LinkFlags |= LF_DWARF_DBI_FLAG;
    DBIFlag |= DBI_TYPE;
    return true;
}

bool ProcLocal() {
    if ((LinkFlags & LF_ANY_DBI_FLAG) == 0) LinkFlags |= LF_DWARF_DBI_FLAG;
    DBIFlag |= DBI_LOCAL;
    return true;
}

bool ProcAll() {
    if ((LinkFlags & LF_ANY_DBI_FLAG) == 0) LinkFlags |= LF_DWARF_DBI_FLAG;
    DBIFlag |= DBI_ALL;
    return true;
}

// an individual alias
bool AddAlias() {
    Reader& r = reader();
    const std::string name = r.str();
    if (!r.get(Sep::equals, Want::include_dot)) return false;
    MakeSymAlias(name.data(), name.size(), r.text().data(), r.text().size());
    return true;
}

bool ProcAlias() { return process_list(AddAlias, Want::include_dot); }

bool AddReference() {
    symbol* sym = SymOp(ST_CREATE_REFERENCE, reader().text().data(), reader().text().size());
    sym->info |= SYM_1_DCE_REF;   // make sure it stays around
    return true;
}

bool ProcReference() { return process_list(AddReference, Want::include_dot); }

bool ProcOSName() {
    if (reader().get(Sep::equals, Want::include_dot)) {
        if (FmtData.osname != nullptr) MemFree(FmtData.osname);
        FmtData.osname = MemToStringSafe(reader().text().data(), reader().text().size());
        return true;
    }
    return false;
}

// dead code elimination
bool ProcEliminate() {
    LinkFlags |= LF_STRIP_CODE;
    return true;
}

bool ProcMaxErrors() {
    if (!get_number(MaxErrors)) return false;
    LinkFlags |= LF_MAX_ERRORS_FLAG;
    return true;
}

bool ProcSymFile() {
    if (reader().get(Sep::equals, Want::file_name)) {
        if (SymFileName != nullptr) MemFree(SymFileName);
        SymFileName = FileName(reader().text().data(), reader().text().size(), E_SYM, false);
    }
    return true;
}

void* AddObjFile(const char* name, const char* membname, file_list** filelist) {
    file_list* file;
    member_list* member = nullptr;
    if (membname != nullptr) {
        member = (member_list*)(MemAllocSafe(sizeof(*member) - 1 + strlen(membname) + 1));
        member->flags_dbi = DBIFlag;
        member->flags_mod = (module_flags)(0);
        strcpy(member->name, membname);
        member->next = nullptr;
        for (file = CurrSect->files; file != nullptr; file = file->next) {
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
    file = AllocNewFile(member);
    if (member != nullptr) {
        file->infile = AllocUniqueFileEntry(name, UsrLibPath);
        file->infile->status |= INSTAT_LIBRARY;
    } else {
        file->infile = AllocFileEntry(name, ObjPath);
    }
    *filelist = file;
    return file;
}

bool AddLibFile() {
    char* membname;
    cmd_flags() &= ~Flag::member_added;
    char* ptr = file_name(&membname);
    if (membname != nullptr) {
        LnkMsg(WRN + LOC + LINE + MSG_NO_MEMB_IN_LIBFILE, nullptr);
        MemFree(membname);
        MemFree(ptr);
        return true;
    }
    CommonState& c = common_state();
    file_list* file = AllocNewFile(nullptr);
    file->infile = AllocFileEntry(ptr, UsrLibPath);
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

bool ProcLibFile() {
    if ((LinkFlags & (LF_DWARF_DBI_FLAG | LF_OLD_DBI_FLAG | LF_NOVELL_DBI_FLAG)) == 0) {
        cmd_flags() |= Flag::files_before_debug;
    }
    CommonState& c = common_state();
    if (c.last_lib_file == nullptr) c.last_lib_file = &Root->files;
    return process_list(AddLibFile, Want::file_name);
}

bool AddModFile() {
    char* membname;
    char* ptr = file_name(&membname);
    AddHTableElem(Root->modFilesHashed, ptr);
    LinkFlags |= LF_GOT_CHGD_FILES;
    if (membname != nullptr) MemFree(membname);
    return true;
}

bool AddFile() {
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
            MakeNewSection();
        }
    }
#endif
    char* ptr = file_name(&membname);
    if (membname == nullptr && exe_name() == nullptr) {
        exe_name() = MemToStringSafe(reader().text().data(), reader().text().size());
    }
    file = current;
    if (*current != nullptr) current = &(*current)->next;
    c.last_file.u.file = (file_list*)(AddObjFile(ptr, membname, current));
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

bool ProcFiles() {
    if ((LinkFlags & (LF_DWARF_DBI_FLAG | LF_OLD_DBI_FLAG | LF_NOVELL_DBI_FLAG)) == 0) {
        cmd_flags() |= Flag::files_before_debug;
    }
    return process_list(AddFile, Want::file_name);
}

bool ProcModFiles() { return process_list(AddModFile, Want::file_name); }

bool AddLib() {
    char* ptr = FileName(reader().text().data(), reader().text().size(), E_LIBRARY, false);
    file_list* file = AddObjLib(ptr, LIB_PRIORITY_MAX);
    file->flags_file |= STAT_USER_SPECD;
#ifdef _EXE
    if (any(cmd_flags() & Flag::set_section)) {
        file->flags_file |= STAT_LIB_FIXED;
        file->ovlref = GetOvlRef();
    }
#endif
    if (any(cmd_flags() & Flag::doing_optlib)) {
        file->infile->status |= INSTAT_NO_WARNING;
    }
    DEBUG((DBG_BASE, "library: %s", ptr));
    MemFree(ptr);
    return true;
}

bool ProcOptLib() {
    cmd_flags() |= Flag::doing_optlib;
    const bool retval = ProcLibrary();
    cmd_flags() &= ~Flag::doing_optlib;
    return retval;
}

bool ProcLibPath() {
    if (!reader().get(Sep::none, Want::file_name)) {
        LnkMsg(WRN + LOC + LINE + MSG_VALUE_INCORRECT, "s", "LIBPATH");
        return true;
    }
    AddLibPaths(reader().text().data(), reader().text().size(), true);   // true == add to front.
    return true;
}

bool ProcPath() {
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
    new_path->next = ObjPath;
    ObjPath = new_path;
    DEBUG((DBG_BASE, "path: %s", new_path->name));
    return true;
}

bool ProcMap() {
    MapFlags |= MAP_FLAG;
    if (reader().get(Sep::equals, Want::file_name)) {
        if (MapFName != nullptr) MemFree(MapFName);
        MapFName = FileName(reader().text().data(), reader().text().size(), E_MAP, false);
        DEBUG((DBG_OLD, "produce map file"));
    }
    return true;
}

bool ProcMapLines() {
    MapFlags |= MAP_LINES;
    return true;
}

bool ProcStack() {
    unsigned_32 value;
    LinkFlags |= LF_STK_SIZE_FLAG;
    const bool ret = get_number(value);
    if (ret) StackSize = value;
    return ret;
}

bool ProcNameLen() {
    unsigned_32 value;
    const bool ret = get_number(value);
    if (ret) {
        if (value == 0) {
            LnkMsg(WRN + LOC + LINE + MSG_VALUE_INCORRECT, "s", "NAMELEN");
        } else {
            NameLen = value;
        }
    }
    return ret;
}

bool ProcCase() {
    LinkFlags |= LF_CASE_FLAG;
    SetSymCase();
    DEBUG((DBG_OLD, "case"));
    return true;
}

bool ProcNoCaseExact() {
    LinkFlags &= ~LF_CASE_FLAG;
    SetSymCase();
    DEBUG((DBG_OLD, "nocase"));
    return true;
}

bool ProcNoExtension() {
    cmd_flags() |= Flag::no_extension;
    return true;
}

bool ProcNoCache() {
    LinkFlags &= ~LF_CACHE_FLAG;
    LinkFlags |= LF_NOCACHE_FLAG;
    return true;
}

bool ProcCache() {
    LinkFlags &= ~LF_NOCACHE_FLAG;
    LinkFlags |= LF_CACHE_FLAG;
    return true;
}

// disable an error message number
bool AddDisable() {
    unsigned_16 value = 0;
    if (number16(value) == Number::ok) {
        value = value % 1000;
        if (value <= MSG_MAX_ERR_MSG_NUM) {
            ClearBit(MsgFlags, value);
        } else {
            LnkMsg(WRN + LOC + LINE + MSG_INVALID_MSG_NUM, nullptr);
        }
    } else {
        LnkMsg(WRN + LOC + LINE + MSG_INVALID_MSG_NUM, nullptr);
    }
    return true;
}

bool ProcDisable() { return process_list(AddDisable, Want::normal); }

bool ProcNoDefLibs() { return true; }

bool ProcVerbose() {
    MapFlags |= MAP_VERBOSE;
    DEBUG((DBG_OLD, "verbose map file"));
    return true;
}

bool ProcUndefsOK() {
    LinkFlags |= LF_UNDEFS_ARE_OK;
    DEBUG((DBG_OLD, "undefined symbols are OK"));
    return true;
}

bool ProcNoUndefsOK() {
    LinkFlags &= ~LF_UNDEFS_ARE_OK;
    return true;
}

bool ProcRedefsOK() {
    LinkFlags |= LF_REDEFS_OK;
    return true;
}

bool ProcNoRedefs() {
    LinkFlags &= ~LF_REDEFS_OK;
    return true;
}

bool ProcCVPack() {
    LinkFlags |= LF_CVPACK_FLAG;
    return true;
}

bool ProcHLLPack() {
    LinkFlags |= LF_HLLPACK_FLAG;
    return true;
}

bool ProcIncremental() {
#if !defined(__DOS__)
    if (any(cmd_flags() & Flag::after_incremental)) {
        LnkMsg(ERR + LOC + LINE + MSG_INC_NEAR_START, nullptr);
    }
    LinkFlags |= LF_INC_LINK_FLAG;
    if (reader().get(Sep::equals, Want::file_name)) {
        IncFileName = FileName(reader().text().data(), reader().text().size(), E_ILK, false);
    } else if (exe_name() != nullptr) {
        IncFileName = FileName(exe_name(), strlen(exe_name()), E_ILK, true);
    } else {
        IncFileName = MemStrdupSafe(DEFAULT_INC_NAME);
    }
    ReadPermData();
#endif
    return true;
}

bool ProcQuiet() {
    LinkFlags |= LF_QUIET_FLAG;
    return true;
}

bool ProcMangledNames() {
    LinkFlags |= LF_DONT_UNMANGLE;
    return true;
}

bool ProcOpResource() {
    Reader& r = reader();
    if (r.get(Sep::equals, Want::file_name)) {
        FmtData.res_name_only = true;
        FmtData.resource = MemToStringSafe(r.text().data(), r.text().size());
    } else if (r.get(Sep::none, Want::include_dot)) {
        FmtData.resource = MemToStringSafe(r.text().data(), r.text().size());
    } else {
        return false;
    }
    return true;
}

bool ProcStatics() {
    MapFlags |= MAP_STATICS;
    return true;
}

bool ProcArtificial() {
    MapFlags |= MAP_ARTIFICIAL;
    return true;
}

bool ProcAlphabetical() {
    MapFlags |= MAP_ALPHA;
    return true;
}

bool ProcGlobal() {
    MapFlags |= MAP_GLOBAL;
    return true;
}

bool ProcJapanese() {
    cmd_flags() |= Flag::language_japanese;
    return true;
}

bool ProcChinese() {
    cmd_flags() |= Flag::language_chinese;
    return true;
}

bool ProcKorean() {
    cmd_flags() |= Flag::language_korean;
    return true;
}

bool ProcShowDead() {
    LinkFlags |= LF_SHOW_DEAD;
    return true;
}

bool ProcVFRemoval() {
    LinkFlags |= LF_VF_REMOVAL;
    return true;
}

bool ProcStart() {
    if (!reader().get(Sep::equals, Want::include_dot)) return false;
    StartInfo.user_specd = true;
    const std::string name = reader().str();
    SetStartSym(name.c_str());
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

bool ProcPackcode() {
    unsigned_32 value;
    if (GetPackValue(&value, "PACKCODE")) {
        PackCodeLimit = value;
        LinkFlags |= LF_PACKCODE_FLAG;
        return true;
    }
    return false;
}

bool ProcPackdata() {
    unsigned_32 value;
    if (GetPackValue(&value, "PACKDATA")) {
        PackDataLimit = value;
        LinkFlags |= LF_PACKDATA_FLAG;
        return true;
    }
    return false;
}

#if defined(_OS2) || defined(_EXE) || defined(_QNX)
// force the start of a new auto-group after the previous object file.
bool ProcNewSegment() {
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

bool ProcSysDelete() { return true; }

void CopyBlocks(void* copyp, const char* data, size_t size) {
    char** copyptr = (char**)(copyp);
    memcpy(*copyptr, data, size);
    *copyptr += size;
}

bool ProcSysEnd() { return true; }

bool ProcEndLink() { return true; }

constexpr Entry SysEndOptions[] = {
    {"End", ProcSysEnd, MK_ALL},
};

constexpr Entry EndLinkOpt[] = {
    {"ENDLink", ProcEndLink, MK_ALL},
};

// The text up to the table's keyword, token by token with a space after each (the quirk), kept as a system block.
void GetCommandBlock(sysblock** hdr, const char* name, Table endtab) {
    stringtable strtab;
    InitStringTable(&strtab, false);
    AddBufferStringTable(&strtab, &strtab, offsetof(sysblock, commands));
    while (!process_one(endtab, Sep::space)) {
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
        sys->name = MemStrdupSafe(name);
    } else {
        sys->name = nullptr;
    }
    AppendList((void**)hdr, sys);
}

char** getStubNamePtr() {
#ifdef _OS2
    if (HintFormat(MK_NE | MK_OS2_FLAT | MK_PE | MK_WIN_VXD)) {
        return &FmtData.u.os2fam.stub_file_name;
    }
#endif
#ifdef _DOS16M
    if (HintFormat(MK_DOS16M)) {
        return &FmtData.u.d16m.stub;
    }
#endif
#ifdef _PHARLAP
    if (HintFormat(MK_PHAR_LAP)) {
        FmtData.def_ext = E_LOAD;   // want .exe instead of .exp now
        return &FmtData.u.phar.stub;
    }
#endif
    return nullptr;
}

bool ProcStub() {
    if (!have_equals(Want::file_name)) return false;
    char** nameptr = getStubNamePtr();
    if (nameptr == nullptr) return false;
    char* name = FileName(reader().text().data(), reader().text().size(), E_LOAD, false);
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

bool ProcVersion() {
    Version vb;   // required limits, 0 = no limit
    vb.major = 100;
    vb.minor = 100;
    vb.revision = 100;
    vb.what = "VERSION";
    const VersionPart result = get_version(vb, VersionPart::major | VersionPart::minor | VersionPart::revision, true);
    if (result != VersionPart::none) {
        FmtData.major = vb.major;
        FmtData.minor = vb.minor;
        FmtData.revision = vb.revision;
        FmtData.ver_specified = true;
        return true;
    }
    return false;
}

bool ProcImplib() {
    FmtData.make_implib = true;
    FmtData.make_impfile = false;
    MemFree(FmtData.implibname);
    if (reader().get(Sep::equals, Want::file_name)) {
        FmtData.implibname = MemToStringSafe(reader().text().data(), reader().text().size());
    }
    return true;
}

bool ProcImpFile() {
    FmtData.make_implib = true;
    FmtData.make_impfile = true;
    MemFree(FmtData.implibname);
    if (reader().get(Sep::equals, Want::file_name)) {
        FmtData.implibname = MemToStringSafe(reader().text().data(), reader().text().size());
    }
    return true;
}

bool AddSymTrace() {
    AddTraceListSym(MemToStringSafe(reader().text().data(), reader().text().size()));
    return true;
}

bool ProcSymTrace() {
    LinkFlags |= LF_TRACE_FLAG;
    return process_list(AddSymTrace, Want::include_dot);
}

bool AddModTrace() {
    char* membname;
    char* fname = file_name(&membname);
    // AddTraceListMod consumes membname: not freed here
    AddTraceListMod(fname, membname);
    return true;
}

bool ProcModTrace() {
    LinkFlags |= LF_TRACE_FLAG;
    return process_list(AddModTrace, Want::file_name);
}

bool ProcFarCalls() {
    LinkFlags |= LF_FAR_CALLS_FLAG;
    DEBUG((DBG_OLD, "Far Calls optimization"));
    return true;
}

bool ProcNoFarCalls() {
    LinkFlags &= ~LF_FAR_CALLS_FLAG;
    DEBUG((DBG_OLD, "No Far Calls optimization"));
    return true;
}

#ifdef _RAW
bool ProcOutputRaw() {
    FmtData.output_raw = true;
    return true;
}

bool ProcOutputHex() {
    FmtData.output_hex = true;
    return true;
}

bool ProcOutputStart() {
    FmtData.output_start = true;
    return true;
}

bool ProcOutputOffset() {
    unsigned_32 value = 0;
    if (!reader().get(Sep::equals, Want::normal)) return false;
    const Number retval = number(value);
    if (retval == Number::ok && (value <= (0xFFFFUL << FmtData.SegShift) || HintFormat(~MK_SEGMENTED))) {
        FmtData.output_offset = value;
        return true;
    }
    LnkMsg(WRN + LOC + LINE + MSG_VALUE_INCORRECT, "s", "OUTPUT OFFSET");
    return false;
}

bool ProcOutputHshift() {
    unsigned_16 value = 0;
    if (!reader().get(Sep::equals, Want::normal)) return false;
    const Number ret = number16(value);
    if (ret == Number::ok && value < 16) {
        FmtData.HexSegShift = 16 - value;
        FmtData.output_hshift = true;
        return true;
    }
    LnkMsg(WRN + LOC + LINE + MSG_VALUE_TOO_LARGE, "s", "OUTPUT HSHIFT");
    return false;
}
#endif

bool ProcHshift() {
    unsigned_16 value = 0;
    if (!reader().get(Sep::equals, Want::normal)) return false;
    const Number ret = number16(value);
    if (ret == Number::ok && value < 16) {
        FmtData.Hshift = value;
        SetSegMask();
        return true;
    }
    LnkMsg(WRN + LOC + LINE + MSG_VALUE_TOO_LARGE, "s", "HSHIFT");
    return false;
}

bool ProcFillchar() {
    unsigned_16 value = 0;
    if (!reader().get(Sep::equals, Want::normal)) return false;
    const Number ret = number16(value);
    if (ret == Number::ok && value < 256) {
        FmtData.FillChar = value;
        return true;
    }
    LnkMsg(WRN + LOC + LINE + MSG_VALUE_TOO_LARGE, "s", "FILLCHAR");
    return false;
}

bool ProcOrdSegAdr() {
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

bool ProcOrdOfsAdr() {
    ORDER_CLASS* oc = common_state().current_order_class;
    unsigned_32 value = 0;
    if (!reader().get(Sep::equals, Want::normal)) return false;
    const Number retval = number(value);
    if (retval == Number::ok && (value <= 0xFFFFL || HintFormat(~MK_SEGMENTED))) {
        oc->Base.off = value;
        oc->FixedAddr = true;
        return true;
    }
    oc->FixedAddr = false;
    LnkMsg(WRN + LOC + LINE + MSG_VALUE_INCORRECT, "s", "ORDER CLNAME OFFSET");
    return false;
}

bool ProcOrdCopy() {
    if (!reader().get(Sep::none, Want::include_dot)) return false;
    ORDER_CLASS* oc = common_state().current_order_class;
    oc->SrcName = MemToStringSafe(reader().text().data(), reader().text().size());
    oc->Copy = true;
    return true;
}

bool ProcOrdNoEmit() {
    common_state().current_order_class->NoEmit = true;
    return true;
}

bool ProcOrdSegSegAdr() {
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

bool ProcOrdSegOfsAdr() {
    ORDER_SEGMENT* os = common_state().current_order_segment;
    unsigned_32 value = 0;
    if (!reader().get(Sep::equals, Want::normal)) return false;
    const Number retval = number(value);
    if (retval == Number::ok && (value <= 0xFFFFL || HintFormat(~MK_SEGMENTED))) {
        os->Base.off = value;
        os->FixedAddr = true;
        return true;
    }
    os->FixedAddr = false;
    LnkMsg(WRN + LOC + LINE + MSG_VALUE_INCORRECT, "s", "ORDER CLNAME SEGMENT OFFSET");
    return false;
}

bool ProcOrdSegNoEmit() {
    common_state().current_order_segment->NoEmit = true;
    return true;
}

bool ProcObjAlign() {
    unsigned_32 value = 0;
    if (!have_equals(Want::normal)) return false;
    const Number ret = number(value);
    if (ret != Number::ok || value == 0) return false;
    if (value < 16 || value > _256M || (value & (value - 1))) {   // value not a power of 2
        LnkMsg(WRN + LOC + LINE + MSG_VALUE_INCORRECT, "s", "OBJALIGN");
        value = _64K;
    }
    FmtData.objalign = value;
    ChkBase(value);
    return true;
}

bool ProcDescription() {
    if (!reader().get(Sep::none, Want::include_dot)) return false;
    if (FmtData.description != nullptr) MemFree(FmtData.description);
    FmtData.description = MemToStringSafe(reader().text().data(), reader().text().size());
    return true;
}

#if defined(_OS2) || defined(_NOVELL) || defined(_ELF)
bool ProcImport() {
#ifdef _OS2
    if (HintFormat(MK_NE | MK_OS2_FLAT | MK_PE)) return ProcOS2Import();
#endif
#ifdef _ELF
    if (HintFormat(MK_ELF)) return ProcELFImport();
#endif
#ifdef _NOVELL
    if (HintFormat(MK_NOVELL)) return ProcNovImport();
#endif
    return false;
}

bool ProcExport() {
#ifdef _OS2
    if (HintFormat(MK_NE | MK_OS2_FLAT | MK_PE | MK_WIN_VXD)) return ProcOS2Export();
#endif
#ifdef _ELF
    if (HintFormat(MK_ELF)) return ProcELFExport();
#endif
#ifdef _NOVELL
    if (HintFormat(MK_NOVELL)) return ProcNovExport();
#endif
    return false;
}
#endif

#if defined(_DOS16M) || defined(_QNX) || defined(_OS2) || defined(_ELF)
bool ProcNoRelocs() {
#ifdef _QNX
    if (HintFormat(MK_QNX)) return ProcQNXNoRelocs();
#endif
#ifdef _OS2
    if (HintFormat(MK_PE)) return ProcOS2NoRelocs();
#endif
#ifdef _DOS16M
    if (HintFormat(MK_DOS16M)) return Proc16MNoRelocs();
#endif
#ifdef _ELF
    if (HintFormat(MK_ELF)) return ProcELFNoRelocs();
#endif
    return true;
}
#endif

#if defined(_OS2) || defined(_QNX)
bool ProcSegment() {
#ifdef _OS2
    if (HintFormat(MK_NE | MK_OS2_FLAT | MK_PE | MK_WIN_VXD)) return ProcOS2Segment();
#endif
#ifdef _QNX
    if (HintFormat(MK_QNX)) return ProcQNXSegment();
#endif
    return true;
}
#endif

#if defined(_OS2) || defined(_ELF)
bool ProcAlignment() {
#ifdef _OS2
    if (HintFormat(MK_NE | MK_OS2_LX | MK_PE)) return ProcOS2Alignment();
#endif
#ifdef _ELF
    if (HintFormat(MK_ELF)) return ProcELFAlignment();
#endif
    return true;
}
#endif

#if defined(_OS2) || defined(_QNX)
bool ProcHeapSize() {
#if defined(_QNX) && defined(__QNX__)
    if (HintFormat(MK_QNX)) return ProcQNXHeapSize();
#endif
#ifdef _OS2
    if (HintFormat(MK_NE | MK_OS2_FLAT | MK_PE)) return ProcOS2HeapSize();
#endif
#if defined(_QNX) && !defined(__QNX__)
    if (HintFormat(MK_QNX)) return ProcQNXHeapSize();
#endif
    return true;
}
#endif

#if defined(_PHARLAP) || defined(_QNX) || defined(_OS2) || defined(_ELF) || defined(_RAW)
bool ProcOffset() {
    if (!get_number(FmtData.base)) return false;
#ifdef _PHARLAP
    if (FmtData.type & MK_PHAR_LAP) {
        ChkBase(_4K);
        return true;
    }
#endif
#ifdef _QNX
    if (FmtData.type & MK_QNX_FLAT) {
        ChkBase(_4K);
        return true;
    }
#endif
#ifdef _RAW
    if (FmtData.type & MK_RAW) {
        ChkBase(256);
        return true;
    }
#endif
    ChkBase(_64K);
    return true;
}
#endif

#if defined(_ELF) || defined(_NOVELL)
bool ProcModule() {
#ifdef _ELF
    if (HintFormat(MK_ELF)) return ProcELFModule();
#endif
#ifdef _NOVELL
    if (HintFormat(MK_NOVELL)) return ProcNovModule();
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
    LinkState |= LS_INTERNAL_DEBUG;
    return true;
}
#endif

constexpr Entry OrderSegOpts[] = {
    {"SEGAddr", ProcOrdSegSegAdr, MK_ALL},
    {"OFFset", ProcOrdSegOfsAdr, MK_ALL},
    {"NOEmit", ProcOrdSegNoEmit, MK_ALL},
};

bool ProcOrdSeg() {
    if (!reader().get(Sep::none, Want::include_dot)) return false;
    CommonState& c = common_state();
    c.current_order_segment = (ORDER_SEGMENT*)(MemAllocSafe(sizeof(ORDER_SEGMENT)));
    c.current_order_segment->NextSeg = c.current_order_class->SegList;
    c.current_order_class->SegList = c.current_order_segment;
    c.current_order_segment->Name = MemToStringSafe(reader().text().data(), reader().text().size());
    c.current_order_segment->FixedAddr = false;
    c.current_order_segment->NoEmit = false;
    while (process_one(OrderSegOpts, Sep::none)) {
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

bool ProcOrdClass() {
    if (!reader().get(Sep::none, Want::include_dot)) return false;
    CommonState& c = common_state();
    LinkState |= LS_SPEC_ORDER_FLAG;
    ORDER_CLASS* last = c.current_order_class;
    c.current_order_class = (ORDER_CLASS*)(MemAllocSafe(sizeof(ORDER_CLASS)));
    ORDER_CLASS* cur = c.current_order_class;
    if (last == nullptr) {
        CurrSect->orderlist = cur;
    } else {
        last->NextClass = cur;
    }
    cur->Name = MemToStringSafe(reader().text().data(), reader().text().size());
    cur->NextClass = nullptr;
    cur->Ring = nullptr;
    cur->SegList = nullptr;
    c.current_order_segment = nullptr;
    cur->FixedAddr = false;
    cur->Copy = false;
    cur->NoEmit = false;
    while (process_one(OrderClassOpts, Sep::none)) {
    }
    return true;
}

constexpr Entry OrderOpts[] = {
    {"CLName", ProcOrdClass, MK_ALL},
};

bool ProcOrder() {
    bool ret = false;
    common_state().current_order_class = CurrSect->orderlist;
    if (common_state().current_order_class != nullptr) {
        LnkMsg(WRN + LOC + LINE + MSG_DUP_DIRECTIVE, "s", "OPTION");
    }
    while (process_one(OrderOpts, Sep::none)) ret = true;
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

bool ProcOutput() {
    FmtData.output_offset = 0L;
    FmtData.output_hshift = false;
    FmtData.output_start = false;
    bool ret = false;
    while (process_one(OutputOpts, Sep::none)) ret = true;
    return ret;
}
#endif

#if defined(_PHARLAP) || defined(_DOS16M) || defined(_OS2) || defined(_ELF)
bool AddRunTime() {
    reader().push_back();   // reparse last token.
#ifdef _DOS16M
    if (Proc16MRuntime()) return true;
#endif
#ifdef _PHARLAP
    if (ProcPharRuntime()) return true;
#endif
#ifdef _OS2
    if (ProcOS2Runtime()) return true;
#endif
#ifdef _ELF
    if (ProcELFRuntime()) return true;
#endif
    return false;
}

bool ProcRuntime() { return process_list(AddRunTime, Want::include_dot); }
#endif

// save up list of commands to process later
bool ProcStartLink() {
    GetCommandBlock(&state().link_commands, nullptr, EndLinkOpt);
    return true;
}

// parse a system begin block and store it somewhere
bool ProcSysBegin() {
    if (!reader().get(Sep::none, Want::include_dot)) {
        LnkMsg(WRN + LOC + LINE + MSG_VALUE_INCORRECT, "s", "SYSTEM BEGIN");
        return true;
    }
    char* sysname = MemToStringSafe(reader().text().data(), reader().text().size());
    sysblock* sys = FindSysBlock(sysname);
    if (sys != nullptr) {
        LnkMsg(WRN + LOC + LINE + MSG_SYSTEM_ALREADY_DEFINED, "s", sys->name);
        while (!process_one(SysEndOptions, Sep::space)) {
            reader().forget_push_back();
            reader().forward();
        }
    } else {
        GetCommandBlock(&state().sys_blocks, sysname, SysEndOptions);
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
bool ProcSystem() {
    if (process_one(SysBeginOptions, Sep::none)) return true;
    const bool dodelete = process_one(SysDeleteOptions, Sep::none);
    if (dodelete) {
        if (!reader().get(Sep::none, Want::include_dot)) return false;
    } else {
        reader().forget_push_back();
    }
    char* sysname = MemToStringSafe(reader().text().data(), reader().text().size());
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

bool ProcLanguage() {
    cmd_flags() &= ~Flag::language_mask;
    return process_one(Languages, Sep::none);
}

constexpr Entry SortOptions[] = {
    {"ALPhabetical", ProcAlphabetical, MK_ALL},
    {"GLobal", ProcGlobal, MK_ALL},
};

bool ProcSort() {
    MapFlags |= MAP_SORT;
    if (!process_one(SortOptions, Sep::none)) return true;
    process_one(SortOptions, Sep::none);
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

bool ProcDebug() {
    if (any(cmd_flags() & Flag::files_before_debug)) {
        LnkMsg(WRN + LOC + LINE + MSG_DEBUG_AFTER_FILES, nullptr);
    }
    const bool gotmod = process_one(DbgMods, Sep::none);
    DBIFlag = (dbi_flags)(0);
    if (process_one(PosDbgMods, Sep::none)) {
        while (process_one(PosDbgMods, Sep::comma)) {
        }
    } else {
        DBIFlag |= DBI_ALL;
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

bool AddOption() {
    reader().push_back();
    if (process_one(MainOptions, Sep::none)) return true;
#ifdef _EXE
    if (ProcDosOptions()) return true;
#endif
#ifdef _ELF
    if (ProcELFOptions()) return true;
#endif
#ifdef _DOS16M
    if (Proc16MOptions()) return true;
#endif
#ifdef _NOVELL
    if (ProcNovOptions()) return true;
#endif
#ifdef _PHARLAP
    if (ProcPharOptions()) return true;
#endif
#ifdef _QNX
    if (ProcQNXOptions()) return true;
#endif
#ifdef _RDOS
    if (ProcRdosOptions()) return true;
#endif
#ifdef _OS2
    if (ProcOS2Options()) return true;
#endif
    return false;
}

bool ProcOptions() { return process_list(AddOption, Want::include_dot); }

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

bool ProcFormat() {
    if (LinkState & LS_FMT_SPECIFIED) {
        LnkMsg(FTL + LOC + LINE + MSG_MULTIPLE_MODES_FOUND, nullptr);
    }
    LinkState |= LS_FMT_SPECIFIED;
    return process_one_or_fail(Models, Sep::none);
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
bool ProcLibrary() {
    if ((LinkFlags & (LF_DWARF_DBI_FLAG | LF_OLD_DBI_FLAG | LF_NOVELL_DBI_FLAG)) == 0 && !in_system_block()) {
        cmd_flags() |= Flag::files_before_debug;
    }
    return process_list(AddLib, Want::file_name);
}

// Note: align must be a power of 2
void ChkBase(offset align) {
    if (FmtData.objalign != NO_BASE_SPEC && FmtData.objalign > align) {
        align = FmtData.objalign;
    }
    if (FmtData.base != NO_BASE_SPEC && (FmtData.base & (align - 1)) != 0) {
        LnkMsg(WRN + LOC + LINE + MSG_OFFSET_MUST_BE_ALIGNED, "l", align);
        FmtData.base = __ROUND_UP_SIZE(FmtData.base, align);
    }
}

bool DoParseDirectiveSubset() { return process_one_subset(Directives, Sep::none); }

bool DoParseDirective() { return process_one(Directives, Sep::none); }

bool DoParseDirectiveSuicide() { return process_one_or_fail(Directives, Sep::none); }

bool DoMatchDirective(std::string_view word) { return matches(Directives, word); }

outfilelist* NewOutFile(char* filename) {
    for (outfilelist* fnode = OutFiles; fnode != nullptr; fnode = fnode->next) {
        if (FNAMECMPSTR(filename, fnode->fname) == 0) {
            MemFree(filename);   // don't need this now.
            return fnode;
        }
    }
    // file name not already in list, so add a list entry.
    outfilelist* fnode = (outfilelist*)(MemAllocSafe(sizeof(outfilelist)));
    InitBuffFile(fnode, filename, true);
    fnode->next = OutFiles;
    OutFiles = fnode;
    return fnode;
}

}  // namespace cb::x86link::directives

// ---- called from outside the directive layer (wlink.cpp, cmdline.cpp)

void ResetCmdAll(void) {
    auto& c = cb::x86link::directives::common_state();
    c.last_file.u.file = nullptr;
    c.last_lib_file = nullptr;
    UsrLibPath = nullptr;
}

sysblock* FindSysBlock(const char* name) {
    for (sysblock* sys = cb::x86link::directives::state().sys_blocks; sys != nullptr; sys = sys->next) {
        if (sys->name != nullptr && stricmp(sys->name, name) == 0) return sys;
    }
    return nullptr;
}
