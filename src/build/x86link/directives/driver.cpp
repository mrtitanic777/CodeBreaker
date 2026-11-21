// build/x86link/directives/driver.cpp -- reading a link's directives from start to end, and what is decided after them:
// the command line and wlink.lnk read, the format narrowed to one and set up, the output files named, the library paths,
// the system blocks (a port of wlink's cmdline.c).
#include <cstring>

#include "build/x86link/directives/common.hpp"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include "build/x86link/directives/formats.hpp"
#include "build/x86link/directives/state.hpp"
#include "build/x86link/directives/cmdline.h"
#include "build/x86link/debuginfo/dbgall.h"
#include "build/x86link/base/fileio.h"
#include "build/x86link/driver/ideentry.h"
#include "build/x86link/symbols/library.h"
#include "build/x86link/formats/output.h"
#include "build/x86link/map/map.h"
#include "build/x86link/layout/overlays.h"
#include "pathlist.h"
#include "build/x86link/fixups/reloc.h"
#include "build/x86link/symbols/symtrace.h"
#include "build/x86link/link/sysblock.h"

namespace cb::x86link::directives {

namespace {

#ifdef BOOTSTRAP
constexpr const char* init_file_name = "bwlink.lnk";
#else
constexpr const char* init_file_name = "wlink.lnk";   // the default systems
#endif
constexpr const char* init_file_env = "WLINK_LNK";

// A format the directives can decide on: its libraries' environment variable, and its state set up and freed.
struct PossibleFormat {
    exe_format bits;
    const char* lib_var_name;
    void (*set)();
    void (*free)();
};

constexpr PossibleFormat possible_formats[] = {
#ifdef _EXE
    {MK_DOS, "LIBDOS", SetDosFmt, FreeDosFmt},
#endif
#ifdef _DOS16M
    {MK_DOS16M, "LIBDOS16M", SetD16MFmt, FreeD16MFmt},
#endif
#ifdef _QNX
    {MK_QNX, "LIBQNX", SetQNXFmt, FreeQNXFmt},
#endif
#ifdef _ELF
    {MK_ELF, "LIBELF", SetELFFmt, FreeELFFmt},
#endif
#ifdef _OS2
    {MK_WIN_NE, "LIBWIN", SetOS2Fmt, FreeOS2Fmt},
    {MK_OS2_NE, "LIBOS2", SetOS2Fmt, FreeOS2Fmt},
    {MK_OS2_LE, "LIBOS2FLAT", SetOS2Fmt, FreeOS2Fmt},
    {MK_OS2_LX, "LIBOS2FLAT", SetOS2Fmt, FreeOS2Fmt},
    {MK_PE, "LIBPE", SetOS2Fmt, FreeOS2Fmt},
    {MK_WIN_VXD, "LIBVXD", SetOS2Fmt, FreeOS2Fmt},
#endif
#ifdef _PHARLAP
    {MK_PHAR_LAP, "LIBPHAR", SetPharFmt, FreePharFmt},
#endif
#ifdef _NOVELL
    {MK_NOVELL, "LIBNOV", SetNovFmt, FreeNovFmt},
#endif
#ifdef _RDOS
    {MK_RDOS, "LIBRDOS", SetRdosFmt, FreeRdosFmt},
#endif
};

// The format the types still possible make (all of them within one entry's), or none.
const PossibleFormat* decided_format(Link& link) {
    for (const PossibleFormat& f : possible_formats) {
        if ((~f.bits & link.FmtData.type) == 0) return &f;
    }
    return nullptr;
}

// What parsing a command file needs set up (wlink's ResetCmdFile).
void reset_command(Link& link) {
    std::memset(&link.FmtData, 0, sizeof(link.FmtData));
    state().sys_blocks = nullptr;
    link.FmtData.def_ext = E_LOAD;
    exe_name() = nullptr;
    cmd_flags() = Flag::unnamed;
    link.ObjPath = nullptr;
    link.FmtData.base = NO_BASE_SPEC;
    link.FmtData.objalign = NO_BASE_SPEC;
    link.FmtData.type = MK_ALL;
    link.FmtData.def_seg_flags = SEG_PMODE_DPL_3;
    link.FmtData.Hshift = 12;   // (may want another value for some 32-bit segmented modes)
    link.FmtData.FillChar = 0;  // (the default fill for a segment's alignment)
    SetSegMask(link);
    link.CurrSect = link.Root;
    state().current_file_list = &link.Root->files;
    link.DBIFlag = static_cast<dbi_flags>(0);   // (only global information by default)
}

// The command asks for help: `?`, `-?` or `/?` first, or nothing at all (taken).
bool help_asked() {
    Reader& r = reader();
    const std::string_view rest = r.rest();
    std::size_t at = 0;
    while (at < rest.size() && rest[at] == ' ') ++at;
    auto c = [&](std::size_t i) { return at + i < rest.size() ? rest[at + i] : '\0'; };
    bool help = true;
    if (c(0) == '?') {
        ++at;
#if defined(__UNIX__)
    } else if (c(0) == '-' && c(1) == '?') {
#else
    } else if ((c(0) == '-' || c(0) == '/') && c(1) == '?') {
#endif
        at += 2;
    } else if (c(0) == '\0') {
    } else {
        help = false;
    }
    r.advance(at);
    return help;
}

// The help, for the format named after the `?` if there is one.
void help(Link& link) {
    Reader& r = reader();
    r.eat_white();
    const std::string_view rest = r.rest();
    if (rest.empty() || rest[0] == '?' || rest[0] == '\0' || !DoHelp(link)) DisplayOptions(link);
}

void parse_command() {
    Link& link = wlink();
    Reader& r = reader();
    while (!r.get(Sep::end, Want::include_dot)) {
        if (!DoParseDirectiveSuicide(link)) Syntax();
        r.forward();
    }
}

void init_format(Link& link, void (*set)()) {
    if (link.LinkState & LS_FMT_INITIALIZED) return;
    if (set != nullptr) set();
    link.LinkState |= LS_FMT_INITIALIZED;
}

void clean_system_list(bool burn) {
    for (sysblock** own = &state().sys_blocks; sysblock* sys = *own;) {
        const char* name = sys->name;
        if (burn || name == nullptr || (std::strcmp("286", name) != 0 && std::strcmp("386", name) != 0)) {
            *own = sys->next;
            if (name != nullptr) MemFree(sys->name);
            MemFree(sys);
        } else {
            own = &sys->next;
        }
    }
}

// Free the directives' own structures (wlink's Burn: split from Ignite for the IDE's options processor).
void burn(Link& link) {
    FreePaths(link);
    FreeOutFiles(link);
    MapFini(link);
    reader().finish();
}

}  // namespace

void Syntax() {
    Reader& r = reader();
    if (!r.has_token()) {
        LnkMsg(FTL + LOC + LINE + MSG_DIRECTIVE_ERR_BEGINNING, nullptr);
    } else {
        const std::string word = r.str();
        LnkMsg(FTL + LOC + LINE + MSG_DIRECTIVE_ERR, "s", word.c_str());
    }
}

void Ignite(Link& link) {
    BurnSystemList();
    burn(link);
}

}  // namespace cb::x86link::directives

namespace dx = cb::x86link::directives;

// ---- what the rest of wlink calls (cmdline.h)

void InitCmdFile() { dx::begin_link(); }   // (each run of the linker: nothing an earlier run read stays)

void SetSegMask(Link& link) {
    link.FmtData.SegShift = 16 - link.FmtData.Hshift;
    link.FmtData.SegMask = (1 << link.FmtData.SegShift) - 1;
}

char* GetNextLink() {
    dx::State& s = dx::state();
    char* cmd = nullptr;
    if (s.prev_command != nullptr) MemFree(s.prev_command);
    s.prev_command = nullptr;
    if (s.link_commands != nullptr) {
        s.prev_command = s.link_commands;
        s.link_commands = s.link_commands->next;
        cmd = s.prev_command->commands;
    }
    return cmd;
}

int DoBuffCmdParse(const char* cmd) {
    dx::reader().push({}, cmd != nullptr ? cmd : "", dx::SourceKind::command_line);
    return Spawn(dx::parse_command);
}

void FreePaths(Link& link) {
    FreeList(link.ObjPath);
    link.ObjPath = nullptr;
    if (char*& name = dx::exe_name(); name != nullptr) {
        MemFree(name);
        name = nullptr;
    }
}

void Burn(Link& link) { dx::burn(link); }

void DoCmdFile(Link& link, const char* fname) {
    dx::Reader& r = dx::reader();
    dx::reset_command(link);
    // the command: the text the linker was handed (as the command line), or none
    if (fname == nullptr || *fname == '\0') {
        r.push({}, {}, dx::SourceKind::command_line);
    } else {
        r.push({}, fname, dx::SourceKind::environment);
    }
    if (dx::help_asked()) {
        dx::help(link);
        dx::Ignite(link);
        Suicide();
    }
    // wlink.lnk (its systems) first: WLINK_LNK names it, else it is found on the path
    f_handle file = NIL_FHANDLE;
    const char* namelnk = GetEnvString(dx::init_file_env);
    if (namelnk != nullptr) file = FindPath(link, namelnk, nullptr);
    if (file == NIL_FHANDLE) {
        namelnk = dx::init_file_name;
        file = FindPath(link, namelnk, nullptr);
    }
    if (file != NIL_FHANDLE) r.push_file(file, namelnk);
    if (Spawn(dx::parse_command)) {
        dx::Ignite(link);
        Suicide();
    }
    GetExtraCommands();
    if ((link.LinkState & LS_FMT_DECIDED) == 0) {   // the formats this host decides on by itself
#if defined(__QNX__)
        HintFormat(MK_OS2 | MK_QNX);
#elif defined(__LINUX__) || defined(__BSD__)
        HintFormat(MK_OS2 | MK_ELF);
#elif defined(__NT__)
        HintFormat(link, MK_OS2_LX | MK_NE | MK_PE | MK_DOS_EXE | MK_WIN_VXD);
#elif defined(__RDOS__)
        HintFormat(MK_RDOS | MK_PE | MK_DOS_EXE);
#else
        HintFormat(MK_OS2 | MK_DOS_EXE | MK_PHAR_SIMPLE);
#endif
    } else {   // one type of those still possible: the lowest
        for (auto possible = static_cast<exe_format>(1); possible != 0; possible = static_cast<exe_format>(possible * 2)) {
            if (link.FmtData.type & possible) {
                link.FmtData.type = possible;
                break;
            }
        }
    }
    if ((link.FmtData.type & (MK_NOVELL | MK_DOS)) && (link.LinkFlags & LF_INC_LINK_FLAG)) {
        LnkMsg(FTL + MSG_FORMAT_BAD_OPTION, "s", "incremental");
    }
#ifdef _NOVELL
    if (link.FmtData.type & MK_NOVELL) dx::CmdNovFini();
#endif
#ifdef _DOS16M
    if (link.FmtData.type & MK_DOS16M) dx::CmdD16MFini(link);
#endif
#ifdef _EXE
    if (link.FmtData.type & MK_OVERLAYS) {
        dx::CmdOvlFini(link);
        AddObjLib(link, "wovl.lib", LIB_PRIORITY_MIN);   // (overlays use wovl.lib)
    }
#endif
    char* name = dx::exe_name();
    if (name == nullptr || !dx::any(dx::cmd_flags() & dx::Flag::have_files)) {
        dx::Ignite(link);
        LnkMsg(FTL + MSG_NO_FILES_FOUND, nullptr);
    }
    const size_t namelen = std::strlen(name);
    if (namelen > 0) {
        if (link.MapFName == nullptr && (link.MapFlags & MAP_FLAG)) link.MapFName = FileName(name, namelen, E_MAP, true);
        const bool separate_symbols = dx::any(dx::cmd_flags() & dx::Flag::separate_symbols);
        if (link.SymFileName == nullptr &&
            (separate_symbols || ((link.FmtData.type & MK_COM) && (link.LinkFlags & LF_ANY_DBI_FLAG)) ||
             ((link.FmtData.type & MK_ELF) && (link.LinkFlags & (LF_OLD_DBI_FLAG | LF_CV_DBI_FLAG))) ||
             ((link.FmtData.type & MK_RAW) && (link.LinkFlags & LF_ANY_DBI_FLAG)))) {
            link.SymFileName = FileName(name, namelen, E_SYM, true);
        }
        if (link.FmtData.make_implib && link.FmtData.implibname == nullptr) {
            link.FmtData.implibname = FileName(name, namelen, link.FmtData.make_impfile ? E_LBC : E_LIBRARY, true);
        }
    }
    if (link.MapFName == nullptr) link.MapFlags = static_cast<mapflag>(0);   // (no map: none of its options)
    CheckTraces(link);
    r.finish();
    PruneSystemList();
#ifdef _EXE
    if (link.FmtData.type & MK_OVERLAYS) OvlNumberSections(link);
#endif
    DBIInit(link);
}

void SetFormat(Link& link) {
    if (dx::any(dx::cmd_flags() & dx::Flag::no_extension)) {
        link.FmtData.def_ext = E_NONE;
#ifdef _RAW
    } else if (link.FmtData.output_hex) {   // (hex or raw output: their extension)
        link.FmtData.def_ext = E_HEX;
    } else if (link.FmtData.output_raw) {
        link.FmtData.def_ext = E_BIN;
#endif
    }
    char*& name = dx::exe_name();
    char* fname = FileName(name, std::strlen(name), link.FmtData.def_ext, dx::any(dx::cmd_flags() & dx::Flag::unnamed));
    MemFree(name);
    link.Root->outfile = dx::NewOutFile(link, fname);
    name = nullptr;
#ifdef _EXE
    if (link.FmtData.type & MK_OVERLAYS) OvlFillOutFilePtrs(link);   // (every output file not named)
#endif
    if (link.MapFile != nullptr) {
        WriteMapLnkMsg(MSG_EXE_NAME, "s", link.Root->outfile->fname);
        WriteMapLnkMsg(MSG_CREATE_EXE, "f");
    }
#ifdef _QNX
    if (link.FmtData.type & MK_QNX) dx::CmdQNXFini(link);
#endif
    SetRelocSize(link);
}

void AddFmtLibPaths(Link& link) {
    if ((link.LinkState & LS_FMT_DECIDED) == 0) return;
    if (const dx::PossibleFormat* f = dx::decided_format(link)) AddLibPathsToEnd(link, GetEnvString(f->lib_var_name));
}

bool HintFormat(Link& link, exe_format hint) {
    if ((hint & link.FmtData.type) == 0) return false;
    link.FmtData.type &= hint;
    if (link.LinkState & LS_FMT_DECIDED) return true;
    const dx::PossibleFormat* f = dx::decided_format(link);
    if (f == nullptr) {
#ifdef _OS2
        // Windows, OS/2 1.x and 2.x, PE and VxD want the same structure
        if ((~(MK_NE | MK_OS2_FLAT | MK_PE | MK_WIN_VXD) & link.FmtData.type) == 0) dx::init_format(link, dx::SetOS2Fmt);
#endif
        return true;
    }
    dx::init_format(link, f->set);
    link.LinkState |= LS_FMT_DECIDED;
    if (link.LinkState & LS_SEARCHING_LIBRARIES) AddFmtLibPaths(link);
    return true;
}

void DecideFormat(Link& link) {
    if (link.LinkState & LS_FMT_DECIDED) return;
    exe_format possible = link.FmtData.type;
    exe_format allowed = MK_OS2_NE | MK_OS2_LX;
    if ((link.LinkState & LS_FMT_SEEN_IMPORT_CMT) == 0) allowed = ~allowed;
    if ((possible & allowed) != 0) possible &= allowed;
    HintFormat(link, possible);
    if ((link.LinkState & LS_FMT_DECIDED) == 0) {
        char rc_buff[RESOURCE_MAX_SIZE];
        Msg_Get(MSG_FORMAT_NOT_DECIDED, rc_buff);
        LnkMsg(FTL + MSG_INTERNAL, "s", rc_buff);
    }
}

void FreeFormatStuff(Link& link) {
    if ((link.LinkState & LS_FMT_DECIDED) == 0) return;
    if (const dx::PossibleFormat* f = dx::decided_format(link); f != nullptr && f->free != nullptr) f->free();
}

void AddCommentLib(Link& link, const char* comment, size_t len, lib_priority priority) {
    if (dx::any(dx::cmd_flags() & dx::Flag::no_default_libs)) return;
    char* ptr = FileName(comment, len, E_LIBRARY, false);
    file_list* file = AddObjLib(link, ptr, priority);
    CheckLibTrace(file);
    DEBUG((DBG_BASE, "library: %s", ptr));
    MemFree(ptr);
}

void AddLibPaths(Link& link, const char* path_list, size_t len, bool add_to_front) {
    auto* newpath = static_cast<path_entry*>(MemAllocSafe(sizeof(path_entry) + len));
    const char* end = path_list + len;
    char* p = newpath->name;
    while (path_list != end) {
        if (p != newpath->name) *p++ = PATH_LIST_SEP;
        path_list = GetPathElement(path_list, end, &p);
    }
    *p = '\0';
    if (add_to_front) {
        newpath->next = link.UsrLibPath;
        link.UsrLibPath = newpath;
    } else {
        AppendList(reinterpret_cast<void**>(&link.UsrLibPath), newpath);
    }
    if (link.UsrLibPath == newpath) {   // the first: every file that searches the library path now searches it
        for (const file_list* file = link.ObjLibFiles; file != nullptr; file = file->next) file->infile->path_list = link.UsrLibPath;
        for (const file_list* file = link.Root->files; file != nullptr; file = file->next) {
            if (file->infile->status & INSTAT_USE_LIBPATH) file->infile->path_list = link.UsrLibPath;
        }
    }
}

void AddLibPathsToEnd(Link& link, const char* path_list) {
    if (path_list != nullptr && *path_list != '\0') AddLibPaths(link, path_list, std::strlen(path_list), false);
}

void AddLibPathsToEndList(Link& link, const char* path_list) {
    if (path_list == nullptr || *path_list == '\0') return;
    const size_t len = std::strlen(path_list);
    auto* newpath = static_cast<path_entry*>(MemAllocSafe(sizeof(path_entry) + len));
    std::strcpy(newpath->name, path_list);
    AppendList(reinterpret_cast<void**>(&link.UsrLibPath), newpath);
}

void ExecSystem(Link& link, const char* name) {
    // A system block run by name, once, after the directives were read: on an empty command line
    sysblock* sys = FindSysBlock(name);
    if (sys == nullptr) return;
    dx::Reader& r = dx::reader();
    r.push({}, {}, dx::SourceKind::environment);   // (the "command line")
    r.end_command();                                // (nothing on it)
    r.push(sys->name, sys->commands, dx::SourceKind::system);
    MemFree(sys->name);
    sys->name = nullptr;
    while (!r.get(dx::Sep::end, dx::Want::include_dot)) {
        if (!dx::DoParseDirectiveSubset(link)) {
            LnkMsg(WRN + LOC + LINE + MSG_ERROR_IN_SYSTEM_BLOCK, nullptr);
            r.pop();
            break;
        }
    }
    BurnSystemList();
    r.finish();
}

void PruneSystemList() { dx::clean_system_list(false); }   // (all but the "286" and "386" blocks)
void BurnSystemList() { dx::clean_system_list(true); }
