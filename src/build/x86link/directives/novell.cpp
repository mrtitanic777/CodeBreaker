// build/x86link/directives/novell.cpp -- Netware's directives: `format novell nlm|lan|dsk|nam|<n> 'description'`, `option`
// for it, `import`, `export` and `module` with their `(PREFIX)`, and `debug novell` (a port of wlink's cmdnov.c).
#include "build/x86link/directives/formats.hpp"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state, wlink() */

#ifdef _NOVELL

#include <cctype>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <string>
#include <string_view>

#include "build/x86link/base/alloc.h"
#include "build/x86link/directives/common.hpp"
#include "build/x86link/directives/state.hpp"
#include "exenov.h"
#include "build/x86link/base/linkutil.h"
#include "build/x86link/formats/novell.h"
#include "build/x86link/messages/msg.h"
#include "build/x86link/link/syms.h"
#include "build/x86link/messages/wlnkmsg.h"

// (defined after the namespace: objpass1.cpp calls it)
void SetNovImportSymbol(symbol* sym);

namespace cb::x86link::directives {

namespace {

// The current token as wlink's memory (wlink's getstring).
char* token_string() {
    const std::string_view t = reader().text();
    return MemToStringSafe(t.data(), t.size());
}

bool is_space(char c) { return std::isspace(static_cast<unsigned char>(c)) != 0; }

// The token is `(PREFIX)`.
bool IsNetWarePrefix(std::string_view token) {
    return !token.empty() && token.front() == '(' && token.back() == ')';
}

// A symbol is `name`, `prefix@name`, `@name`, or `name@<digits>` (a stdcall name: all of it is the name).
struct SplitSymbol {
    std::string_view name;
    std::string_view prefix;   // empty: none
};

bool NetWareSplitSymbol(std::string_view token, SplitSymbol& out) {
    if (token.empty()) return false;
    out = SplitSymbol{};
    std::size_t at = 0;
    for (; at < token.size(); ++at) {
        if (token[at] == '@' || token[at] == '\0') break;
    }
    if (at == token.size() || token[at] == '\0') {   // (no `@`)
        out.name = token;
        return true;
    }
    // (an `@`: a stdcall designator when a digit follows it, else a prefix)
    if (std::isdigit(static_cast<unsigned char>(reader().at(at + 1)))) {
        out.name = token;
        return true;
    }
    if (at != 0) out.prefix = token.substr(0, at);
    out.name = token.substr(at + 1);
    return true;
}

// `(PREFIX)` becomes the prefix of the symbols after it (the command file keeps it); no token clears it.
bool SetCurrentPrefix(std::string_view str) {
    Source* file = reader().current();
    file->symbol_prefix.clear();   // (always deleted)
    if (str.empty()) return true;
    // (it is "(.....)")
    str.remove_prefix(1);   // the opening parenthesis
    while (!str.empty() && is_space(str.front())) str.remove_prefix(1);
    if (str.empty()) return false;
    str.remove_suffix(1);   // the closing parenthesis
    if (str.empty()) return false;
    while (!str.empty() && is_space(str.back())) str.remove_suffix(1);
    if (str.empty()) return false;
    file->symbol_prefix = std::string(str);
    return true;
}

// Trouble! In files, import and export specifiers may or may not have a trailing comma so we look ahead to the next
// characters and see if there is a comma next (after whitespace) and if there is then we don't set this flag else we do;
// this also affects `IMPORT x, (PREFIX), y, (PREFIX), x`.
bool DoWeNeedToSkipASeparator(bool CheckDirectives) {
    const std::string_view next = reader().rest();
    auto ch = [&](std::size_t i) { return i < next.size() ? next[i] : '\0'; };
    if (ch(0) == '\0') return false;
    std::size_t parse = 0;
    while (ch(parse) != '\0' && is_space(ch(parse))) ++parse;
    if (ch(parse) == '\0') return false;
    if (ch(parse) == ',') return false;
    // (skip cr-lf)
    if (ch(parse) == '\n' || ch(parse) == '\r') ++parse;
    if (ch(parse) == '\n' || ch(parse) == '\r') ++parse;
    // always skip to the next token if the next available token is not a comma (this allows individual tokens without
    // commas, which isn't a big deal)
    if (ch(parse) != '\0' && ch(parse) != ',') {
        // (if the next token is not a comma, it must not be a directive before the skip is allowed)
        if (CheckDirectives) {
            std::size_t len = 0;
            while (ch(parse + len) != '\0' && !is_space(ch(parse + len))) ++len;
            if (DoMatchDirective(next.substr(parse, len))) return false;
        }
        return true;
    }
    return false;
}

bool GetNovSymbolImportExport(bool import) {
    Reader& r = reader();
    // we need to trap import/export prefixes here. Unfortunately the prefix context is not followed by a valid separator
    // so the get in process_list at the end of the loop terminates the loop after we return from this call
    if (IsNetWarePrefix(r.text())) {
        const bool result = SetCurrentPrefix(r.text());
        if (result) r.skip_next_separator(DoWeNeedToSkipASeparator(false));
        return result;
    }
    SplitSymbol split;
    if (!NetWareSplitSymbol(r.text(), split)) return false;
    const std::string_view name = split.name;
    std::string_view prefix = split.prefix;
    if (prefix.empty() && !r.current()->symbol_prefix.empty()) prefix = r.current()->symbol_prefix;
    const char* prefix_text = prefix.empty() ? nullptr : prefix.data();
    symbol* sym;
    if (import) {
        sym = SymOpNWPfx(ST_CREATE_DEFINE_NOALIAS, name.data(), name.size(), prefix_text, prefix.size());
        if (sym == nullptr || sym->p.import_nov != nullptr) return true;
        SET_SYM_TYPE(sym, SYM_IMPORTED);
        sym->info |= SYM_1_DCE_REF;   // make sure we don't try to get rid of these
        SetNovImportSymbol(sym);
    } else {
        sym = SymOpNWPfx(ST_CREATE_REFERENCE, name.data(), name.size(), prefix_text, prefix.size());
        sym->info |= SYM_1_DCE_REF | SYM_EXPORTED;
        AddNameTable(name.data(), name.size(), true, &wlink().FmtData.u.nov.exp.export_);
    }
    r.skip_next_separator(DoWeNeedToSkipASeparator(true));
    return true;
}

// ---- "OPtion" directive

bool ProcScreenName() {
    Reader& r = reader();
    if (!r.get(Sep::none, Want::include_dot)) return false;
    if (r.text().size() > MAX_SCREEN_NAME_LENGTH) {
        LnkMsg(WRN + LOC + LINE + MSG_VALUE_TOO_LARGE, "s", "SCREENNAME");
    } else {
        if (wlink().FmtData.u.nov.screenname != nullptr) MemFree(wlink().FmtData.u.nov.screenname);   // assume second is correct
        wlink().FmtData.u.nov.screenname = token_string();
    }
    return true;
}

bool ProcCheck() {
    if (!reader().get(Sep::equals, Want::include_dot)) return false;
    wlink().FmtData.u.nov.checkfn = token_string();
    return true;
}

bool ProcMultiLoad() {
    wlink().FmtData.u.nov.exeflags |= NOV_MULTIPLE;
    return true;
}

bool ProcAutoUnload() {
    wlink().FmtData.u.nov.exeflags |= NOV_AUTOUNLOAD;
    return true;
}

bool ProcReentrant() {
    wlink().FmtData.u.nov.exeflags |= NOV_REENTRANT;
    return true;
}

bool ProcSynch() {
    wlink().FmtData.u.nov.exeflags |= NOV_SYNCHRONIZE;
    return true;
}

bool ProcPseudoPreemption() {
    wlink().FmtData.u.nov.exeflags |= NOV_PSEUDOPREEMPTION;
    return true;
}

bool ProcNLMFlags() {
    std::uint32_t value;
    if (!get_number(value)) return false;
    wlink().FmtData.u.nov.exeflags |= value;
    return true;
}

bool ProcCustom() {
    if (!reader().get(Sep::equals, Want::file_name)) return false;
    wlink().FmtData.u.nov.customdata = token_string();   // no default extension
    return true;
}

bool ProcMessages() {
    if (!reader().get(Sep::equals, Want::file_name)) return false;
    wlink().FmtData.u.nov.messages = token_string();   // no default extension
    return true;
}

bool ProcHelp() {
    if (!reader().get(Sep::equals, Want::file_name)) return false;
    wlink().FmtData.u.nov.help = token_string();   // no default extension
    return true;
}

bool ProcXDCData() {
    if (!reader().get(Sep::equals, Want::file_name)) return false;
    wlink().FmtData.u.nov.rpcdata = token_string();   // no default extension
    return true;
}

bool ProcSharelib() {
    Reader& r = reader();
    if (!r.get(Sep::equals, Want::file_name)) return false;
    const std::string_view t = r.text();
    wlink().FmtData.u.nov.sharednlm = FileName(t.data(), t.size(), E_NLM, false);
    return true;
}

bool ProcExit() {
    if (!reader().get(Sep::equals, Want::include_dot)) return false;
    wlink().FmtData.u.nov.exitfn = token_string();
    return true;
}

bool ProcThreadName() {
    Reader& r = reader();
    if (!r.get(Sep::none, Want::include_dot)) {
        return false;
    } else if (r.text().size() > MAX_THREAD_NAME_LENGTH) {
        LnkMsg(WRN + LOC + LINE + MSG_VALUE_TOO_LARGE, "s", "THREADNAME");
    } else {
        wlink().FmtData.u.nov.threadname = token_string();
    }
    return true;
}

constexpr char COPYRIGHT_START[] = "Copyright 1991";
constexpr char DEFAULT_COPYRIGHT[] = "Copyright 1991 Novell, Inc. All rights reserved";
constexpr std::size_t DEFAULT_COPYRIGHT_LENGTH = sizeof(DEFAULT_COPYRIGHT) - 1;
constexpr std::size_t YEAR_OFFSET = sizeof(COPYRIGHT_START) - 1;

bool ProcCopyright() {
    Reader& r = reader();
    if (!r.get(Sep::equals, Want::include_dot) && !r.get(Sep::none, Want::include_dot)) {
        if (wlink().FmtData.u.nov.copyright != nullptr) MemFree(wlink().FmtData.u.nov.copyright);   // assume second is correct
        wlink().FmtData.u.nov.copyright = static_cast<char*>(MemAllocSafe(DEFAULT_COPYRIGHT_LENGTH + 1));
        std::memcpy(wlink().FmtData.u.nov.copyright, DEFAULT_COPYRIGHT, DEFAULT_COPYRIGHT_LENGTH + 1);
        // (the year is written backwards from YEAR_OFFSET, as wlink does)
        char* copy_year = wlink().FmtData.u.nov.copyright + YEAR_OFFSET;
        const std::time_t thetime = std::time(nullptr);
        const std::tm* currtime = std::localtime(&thetime);
        for (unsigned year = currtime->tm_year + 1900; year > 0; year /= 10) {
            *copy_year = static_cast<char>('0' + (year % 10));
            copy_year--;
        }
    } else {
        if (r.text().size() > MAX_COPYRIGHT_LENGTH) {
            LnkMsg(WRN + LOC + LINE + MSG_VALUE_TOO_LARGE, "s", "COPYRIGHT");
        } else {
            if (wlink().FmtData.u.nov.copyright != nullptr) MemFree(wlink().FmtData.u.nov.copyright);   // assume second is correct
            wlink().FmtData.u.nov.copyright = token_string();
        }
    }
    return true;
}

bool ProcOSDomain() {
    wlink().FmtData.u.nov.exeflags |= NOV_OS_DOMAIN;
    return true;
}

constexpr Entry MainOptions[] = {
    {"SCReenname", ProcScreenName, MK_NOVELL},
    {"CHeck", ProcCheck, MK_NOVELL},
    {"MULTILoad", ProcMultiLoad, MK_NOVELL},
    {"AUTOUNload", ProcAutoUnload, MK_NOVELL},
    {"REentrant", ProcReentrant, MK_NOVELL},
    {"SYnchronize", ProcSynch, MK_NOVELL},
    {"CUSTom", ProcCustom, MK_NOVELL},
    {"EXit", ProcExit, MK_NOVELL},
    {"THReadname", ProcThreadName, MK_NOVELL},
    {"PSeudopreemption", ProcPseudoPreemption, MK_NOVELL},
    {"COPYRight", ProcCopyright, MK_NOVELL},
    {"MESsages", ProcMessages, MK_NOVELL},
    {"HElp", ProcHelp, MK_NOVELL},
    {"XDCdata", ProcXDCData, MK_NOVELL},
    {"SHArelib", ProcSharelib, MK_NOVELL},
    {"OSDomain", ProcOSDomain, MK_NOVELL},
    {"NLMFlags", ProcNLMFlags, MK_NOVELL},
};

// ---- "IMPort", "EXPort" and "MODule" directives

bool GetNovImport() { return GetNovSymbolImportExport(true); }

bool GetNovExport() { return GetNovSymbolImportExport(false); }

bool GetNovModule() {
    const std::string_view t = reader().text();
    AddNameTable(t.data(), t.size(), false, &wlink().FmtData.u.nov.exp.module);
    return true;
}

// ---- "Debug" directive

bool ProcNovDBIExports() {
    wlink().FmtData.u.nov.flags |= DO_NOV_EXPORTS;
    return true;
}

bool ProcNovDBIReferenced() {
    wlink().FmtData.u.nov.flags |= DO_NOV_REF_ONLY;
    return true;
}

constexpr Entry NovDBIOptions[] = {
    {"ONLyexports", ProcNovDBIExports, MK_NOVELL},
    {"REFerenced", ProcNovDBIReferenced, MK_NOVELL},
};

// ---- "Format" directive

bool ProcModuleTypeN(int n) {
    wlink().FmtData.def_ext = E_NLM;
    wlink().FmtData.u.nov.moduletype = n;
    return true;
}

bool ProcNLM() {
    wlink().FmtData.def_ext = E_NLM;
    wlink().FmtData.u.nov.moduletype = 0;
    return true;
}

bool ProcLAN() {
    wlink().FmtData.def_ext = E_LAN;
    wlink().FmtData.u.nov.moduletype = 1;
    return true;
}

bool ProcDSK() {
    wlink().FmtData.def_ext = E_DSK;
    wlink().FmtData.u.nov.moduletype = 2;
    return true;
}

bool ProcNAM() {
    wlink().FmtData.def_ext = E_NAM;
    wlink().FmtData.u.nov.moduletype = 3;
    return true;
}

bool ProcModuleType4() {
    wlink().FmtData.def_ext = E_NLM;
    wlink().FmtData.u.nov.moduletype = 4;
    return true;
}

bool ProcModuleType5() {
    wlink().FmtData.def_ext = E_NOV_MSL;
    wlink().FmtData.u.nov.moduletype = 5;
    return true;
}

bool ProcModuleType6() {
    wlink().FmtData.def_ext = E_NLM;
    wlink().FmtData.u.nov.moduletype = 6;
    return true;
}

bool ProcModuleType7() {
    wlink().FmtData.def_ext = E_NLM;
    wlink().FmtData.u.nov.moduletype = 7;
    return true;
}

bool ProcModuleType8() {
    wlink().FmtData.def_ext = E_NOV_HAM;
    wlink().FmtData.u.nov.moduletype = 8;
    return true;
}

bool ProcModuleType9() {
    wlink().FmtData.def_ext = E_NOV_CDM;
    wlink().FmtData.u.nov.moduletype = 9;
    return true;
}

// (module types 10 through 12 are reserved)
constexpr Entry NovFormats[] = {
    {"NLM", ProcNLM, MK_NOVELL},   // 0
    {"LAN", ProcLAN, MK_NOVELL},   // 1
    {"DSK", ProcDSK, MK_NOVELL},   // 2
    {"NAM", ProcNAM, MK_NOVELL},   // 3
    {"0", ProcNLM, MK_NOVELL},     // 0 again
    {"1", ProcLAN, MK_NOVELL},     // etc
    {"2", ProcDSK, MK_NOVELL},
    {"3", ProcNAM, MK_NOVELL},
    {"4", ProcModuleType4, MK_NOVELL},
    {"5", ProcModuleType5, MK_NOVELL},
    {"6", ProcModuleType6, MK_NOVELL},
    {"7", ProcModuleType7, MK_NOVELL},
    {"8", ProcModuleType8, MK_NOVELL},
    {"9", ProcModuleType9, MK_NOVELL},
};

}  // namespace

void SetNovFmt() { wlink().FmtData.def_ext = E_NLM; }

void FreeNovFmt() {
    MemFree(wlink().FmtData.description);
    MemFree(wlink().FmtData.u.nov.screenname);
    MemFree(wlink().FmtData.u.nov.checkfn);
    MemFree(wlink().FmtData.u.nov.exitfn);
    MemFree(wlink().FmtData.u.nov.customdata);
    MemFree(wlink().FmtData.u.nov.threadname);
    MemFree(wlink().FmtData.u.nov.copyright);
    MemFree(wlink().FmtData.u.nov.messages);
    MemFree(wlink().FmtData.u.nov.help);
    MemFree(wlink().FmtData.u.nov.rpcdata);
    MemFree(wlink().FmtData.u.nov.sharednlm);
    // (the export and module lists are permanent memory now)
}

void CmdNovFini() {}

bool ProcNovOptions() { return process_one(MainOptions, Sep::none); }

bool ProcNovImport() {
    SetCurrentPrefix({});
    return process_list(GetNovImport, Want::include_dot, reader().current());
}

bool ProcNovExport() {
    SetCurrentPrefix({});
    return process_list(GetNovExport, Want::include_dot, reader().current());
}

bool ProcNovModule() { return process_list(GetNovModule, Want::include_dot); }

bool ProcNovExportsDBI() {
    wlink().DBIFlag |= DBI_ONLY_EXPORTS;
    wlink().FmtData.u.nov.flags |= DO_WATCOM_EXPORTS;
    return true;
}

bool ProcNovDBI() {
    wlink().LinkFlags |= LF_NOVELL_DBI_FLAG;
    if (process_one(NovDBIOptions, Sep::none)) {
        while (process_one(NovDBIOptions, Sep::comma)) {
        }
    }
    return true;
}

bool ProcNovFormat() {
    Reader& r = reader();
    if (!process_one(NovFormats, Sep::none)) {   // get file type
        const int nType = std::atoi(r.str().c_str());
        if (nType > 0) {
            r.get(Sep::none, Want::include_dot);
            ProcModuleTypeN(nType);
        } else {
            ProcNLM();
        }
    }
    if (r.get(Sep::quote, Want::include_dot)) {   // get description
        wlink().FmtData.description = token_string();
    }
    return true;
}

}  // namespace cb::x86link::directives

// Called from objpass1.cpp.
void SetNovImportSymbol(symbol* sym) {
    sym->p.import_nov = static_cast<nov_import*>(DUMMY_IMPORT_PTR);
}

#endif
