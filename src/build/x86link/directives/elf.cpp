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

bool ProcExtraSections(Link& link) {
    std::uint32_t value;
    if (!get_number(value)) return false;
    link.FmtData.u.elf.extrasects = value;
    return true;
}

bool ProcExportAll(Link& link) {
    link.FmtData.u.elf.exportallsyms = true;
    return true;
}

constexpr Entry MainOptions[] = {
    {"EXTRASections", ProcExtraSections, MK_ELF},
    {"EXPORTAll", ProcExportAll, MK_ELF},
};

// ---- "IMPort" and "EXPort" directives

bool GetELFImport(Link& link) {
    const std::string_view t = reader().text();
    symbol* sym = SymOp(link, ST_CREATE_DEFINE_NOALIAS, t.data(), t.size());
    if (sym == nullptr) return true;
    SET_SYM_TYPE(sym, SYM_IMPORTED);
    sym->info |= SYM_1_DCE_REF;
    SetELFImportSymbol(sym);
    return true;
}

bool GetELFExport(Link& link) {
    const std::string_view t = reader().text();
    symbol* sym = SymOp(link, ST_CREATE_DEFINE_NOALIAS, t.data(), t.size());
    if (sym == nullptr) return true;
    SET_SYM_TYPE(sym, SYM_EXPORTED);
    sym->info |= SYM_1_DCE_REF;
    return true;   // (wlink's SetELFExportSymbol does nothing)
}

// ---- "MODule" directive

bool GetELFModule(Link& link) {
    const std::string_view t = reader().text();
    AddNameTable(link, t.data(), t.size(), false, &link.FmtData.u.elf.exp.module);
    return true;
}

// ---- "RUntime" directive

void ParseABITypeAndVersion(Link& link) {
    link.FmtData.u.elf.abitype = 0;
    link.FmtData.u.elf.abiversion = 0;
    Version v;
    v.major = 255;   // required limits, 0 = no limit
    v.minor = 255;
    v.revision = 0;
    v.what = "ABIVER";
    if (get_version(v, VersionPart::major | VersionPart::minor, false) != VersionPart::none) {
        link.FmtData.u.elf.abitype = v.major;
        link.FmtData.u.elf.abiversion = v.minor;
    }
}

void ParseABIVersion(Link& link, const std::string& message) {
    if (!reader().get(Sep::equals, Want::normal)) return;
    link.FmtData.u.elf.abiversion = 0;
    std::uint16_t version;
    if (number16(version) != Number::ok || version > 255) {
        LnkMsg(WRN + LOC + LINE + MSG_VALUE_INCORRECT, "s", message.c_str());
    } else {
        link.FmtData.u.elf.abiversion = version;
    }
}

bool ProcELFRNumber(Link& link) {
    ParseABITypeAndVersion(link);
    return true;
}

bool ProcELFRSVR4(Link& link) {
    link.FmtData.u.elf.abitype = ELFOSABI_NONE;
    ParseABIVersion(link, "SVR4");
    return true;
}

bool ProcELFRNetBSD(Link& link) {
    link.FmtData.u.elf.abitype = ELFOSABI_NETBSD;
    ParseABIVersion(link, "NETBSD");
    return true;
}

bool ProcELFRLinux(Link& link) {
    link.FmtData.u.elf.abitype = ELFOSABI_LINUX;
    ParseABIVersion(link, "LINUX");
    return true;
}

bool ProcELFRSolrs(Link& link) {
    link.FmtData.u.elf.abitype = ELFOSABI_SOLARIS;
    ParseABIVersion(link, "SOLARIS");
    return true;
}

bool ProcELFRFBSD(Link& link) {
    link.FmtData.u.elf.abitype = ELFOSABI_FREEBSD;
    ParseABIVersion(link, "FREEBSD");
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

bool ProcELFDLL(Link& link) {
    link.FmtData.dll = true;
    return true;
}

constexpr Entry ELFFormats[] = {
    {"DLl", ProcELFDLL, MK_ELF},
};

}  // namespace

void SetELFFmt() {
    Link& link = wlink();
    link.FmtData.def_ext = E_ELF;
    link.FmtData.u.elf.exp.export_ = nullptr;
    link.FmtData.u.elf.exp.module = nullptr;
    link.FmtData.u.elf.extrasects = 0;
    link.FmtData.u.elf.abitype = 0;
    link.FmtData.u.elf.abiversion = 0;
}

void FreeELFFmt() {
    // (the export and module lists are permanent memory now)
}

bool ProcELFOptions(Link& link) { return process_one(link, MainOptions, Sep::none); }

bool ProcELFAlignment(Link& link) {
    if (!have_equals(Want::normal)) return false;
    std::uint32_t value;
    if (number(value) != Number::ok || value == 0) return false;
    for (;;) {
        const std::uint32_t lessone = value & (value - 1);   // remove the low order bit
        if (lessone == 0) break;                             // until we are at a power of 2
        value = lessone;
    }
    link.FmtData.objalign = value;
    return true;
}

bool ProcELFNoRelocs(Link& link) {
    link.LinkState &= ~LS_MAKE_RELOCS;
    return true;
}

bool ProcELFImport(Link& link) { return process_list(link, GetELFImport, Want::include_dot); }

bool ProcELFExport(Link& link) { return process_list(link, GetELFExport, Want::include_dot); }

bool ProcELFModule(Link& link) { return process_list(link, GetELFModule, Want::include_dot); }

bool ProcELFRuntime(Link& link) { return process_one(link, RunOptions, Sep::none); }

bool ProcELFFormat(Link& link) {
    process_one(link, ELFFormats, Sep::none);
    return true;
}

}  // namespace cb::x86link::directives

// Called from objpass1.cpp (nothing to do for ELF).
void SetELFImportSymbol(symbol*) {}

#endif
