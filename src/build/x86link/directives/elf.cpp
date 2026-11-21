// build/x86link/directives/elf.cpp -- ELF's directives: `format elf dll`, `option` for it, `import`, `export`, `module`
// and `runtime` (the ABI) (a port of wlink's cmdelf.c).
#include "build/x86link/directives/formats.hpp"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state, wlink() */

#ifdef _ELF

#include <string>
#include <string_view>

#include "build/x86link/directives/common.hpp"
#include "build/x86link/directives/state.hpp"
#include "exeelf.h"
#include "build/x86link/base/linkutil.h"
#include "build/x86link/formats/elf.h"
#include "build/x86link/messages/msg.h"
#include "build/x86link/link/syms.h"
#include "build/x86link/messages/wlnkmsg.h"

// (defined after the namespace: objpass1.cpp calls it)
void SetELFImportSymbol(symbol* sym);

namespace cb::x86link::directives {

namespace {

// ---- "OPtion" directive

bool ProcExtraSections() {
    std::uint32_t value;
    if (!get_number(value)) return false;
    wlink().FmtData.u.elf.extrasects = value;
    return true;
}

bool ProcExportAll() {
    wlink().FmtData.u.elf.exportallsyms = true;
    return true;
}

constexpr Entry MainOptions[] = {
    {"EXTRASections", ProcExtraSections, MK_ELF},
    {"EXPORTAll", ProcExportAll, MK_ELF},
};

// ---- "IMPort" and "EXPort" directives

bool GetELFImport() {
    const std::string_view t = reader().text();
    symbol* sym = SymOp(ST_CREATE_DEFINE_NOALIAS, t.data(), t.size());
    if (sym == nullptr) return true;
    SET_SYM_TYPE(sym, SYM_IMPORTED);
    sym->info |= SYM_1_DCE_REF;
    SetELFImportSymbol(sym);
    return true;
}

bool GetELFExport() {
    const std::string_view t = reader().text();
    symbol* sym = SymOp(ST_CREATE_DEFINE_NOALIAS, t.data(), t.size());
    if (sym == nullptr) return true;
    SET_SYM_TYPE(sym, SYM_EXPORTED);
    sym->info |= SYM_1_DCE_REF;
    return true;   // (wlink's SetELFExportSymbol does nothing)
}

// ---- "MODule" directive

bool GetELFModule() {
    const std::string_view t = reader().text();
    AddNameTable(t.data(), t.size(), false, &wlink().FmtData.u.elf.exp.module);
    return true;
}

// ---- "RUntime" directive

void ParseABITypeAndVersion() {
    wlink().FmtData.u.elf.abitype = 0;
    wlink().FmtData.u.elf.abiversion = 0;
    Version v;
    v.major = 255;   // required limits, 0 = no limit
    v.minor = 255;
    v.revision = 0;
    v.what = "ABIVER";
    if (get_version(v, VersionPart::major | VersionPart::minor, false) != VersionPart::none) {
        wlink().FmtData.u.elf.abitype = v.major;
        wlink().FmtData.u.elf.abiversion = v.minor;
    }
}

void ParseABIVersion(const std::string& message) {
    if (!reader().get(Sep::equals, Want::normal)) return;
    wlink().FmtData.u.elf.abiversion = 0;
    std::uint16_t version;
    if (number16(version) != Number::ok || version > 255) {
        LnkMsg(WRN + LOC + LINE + MSG_VALUE_INCORRECT, "s", message.c_str());
    } else {
        wlink().FmtData.u.elf.abiversion = version;
    }
}

bool ProcELFRNumber() {
    ParseABITypeAndVersion();
    return true;
}

bool ProcELFRSVR4() {
    wlink().FmtData.u.elf.abitype = ELFOSABI_NONE;
    ParseABIVersion("SVR4");
    return true;
}

bool ProcELFRNetBSD() {
    wlink().FmtData.u.elf.abitype = ELFOSABI_NETBSD;
    ParseABIVersion("NETBSD");
    return true;
}

bool ProcELFRLinux() {
    wlink().FmtData.u.elf.abitype = ELFOSABI_LINUX;
    ParseABIVersion("LINUX");
    return true;
}

bool ProcELFRSolrs() {
    wlink().FmtData.u.elf.abitype = ELFOSABI_SOLARIS;
    ParseABIVersion("SOLARIS");
    return true;
}

bool ProcELFRFBSD() {
    wlink().FmtData.u.elf.abitype = ELFOSABI_FREEBSD;
    ParseABIVersion("FREEBSD");
    return true;
}

constexpr Entry RunOptions[] = {
    {"ABIver", ProcELFRNumber, MK_ELF},
    {"SVR4", ProcELFRSVR4, MK_ELF},
    {"NETbsd", ProcELFRNetBSD, MK_ELF},
    {"LINux", ProcELFRLinux, MK_ELF},
    {"FREebsd", ProcELFRFBSD, MK_ELF},
    {"SOLaris", ProcELFRSolrs, MK_ELF},
};

// ---- "Format" directive

bool ProcELFDLL() {
    wlink().FmtData.dll = true;
    return true;
}

constexpr Entry ELFFormats[] = {
    {"DLl", ProcELFDLL, MK_ELF},
};

}  // namespace

void SetELFFmt() {
    wlink().FmtData.def_ext = E_ELF;
    wlink().FmtData.u.elf.exp.export_ = nullptr;
    wlink().FmtData.u.elf.exp.module = nullptr;
    wlink().FmtData.u.elf.extrasects = 0;
    wlink().FmtData.u.elf.abitype = 0;
    wlink().FmtData.u.elf.abiversion = 0;
}

void FreeELFFmt() {
    // (the export and module lists are permanent memory now)
}

bool ProcELFOptions() { return process_one(MainOptions, Sep::none); }

bool ProcELFAlignment() {
    if (!have_equals(Want::normal)) return false;
    std::uint32_t value;
    if (number(value) != Number::ok || value == 0) return false;
    for (;;) {
        const std::uint32_t lessone = value & (value - 1);   // remove the low order bit
        if (lessone == 0) break;                             // until we are at a power of 2
        value = lessone;
    }
    wlink().FmtData.objalign = value;
    return true;
}

bool ProcELFNoRelocs() {
    wlink().LinkState &= ~LS_MAKE_RELOCS;
    return true;
}

bool ProcELFImport() { return process_list(GetELFImport, Want::include_dot); }

bool ProcELFExport() { return process_list(GetELFExport, Want::include_dot); }

bool ProcELFModule() { return process_list(GetELFModule, Want::include_dot); }

bool ProcELFRuntime() { return process_one(RunOptions, Sep::none); }

bool ProcELFFormat() {
    process_one(ELFFormats, Sep::none);
    return true;
}

}  // namespace cb::x86link::directives

// Called from objpass1.cpp (nothing to do for ELF).
void SetELFImportSymbol(symbol*) {}

#endif
