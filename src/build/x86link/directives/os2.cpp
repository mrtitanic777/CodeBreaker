// build/x86link/directives/os2.cpp -- OS/2, Windows, Windows NT and VxD directives: the options, `export`, `import`,
// `segment`, `commit`, `runtime` and `format` (a port of wlink's cmdos2.c).
#include <cstdint>
#include <string>
#include <string_view>

#include "build/x86link/directives/common.hpp"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state */
#include "build/x86link/directives/formats.hpp"
#include "build/x86link/directives/state.hpp"
#include "build/x86link/directives/cmdline.h"
#include "exepe.h"
#include "build/x86link/base/fileio.h"
#include "build/x86link/symbols/impexp.h"
#include "build/x86link/formats/os2.h"
#include "build/x86link/formats/pe.h"
#include "build/x86link/objects/objfree.h"
#include "build/x86link/symbols/objpass1.h"

#ifdef _OS2

namespace cb::x86link::directives {

void SetOS2Fmt(Link& link) {
    // set up the structures needed to be able to process something in OS/2 mode.
    if (link.LinkState & LS_FMT_INITIALIZED) return;
    link.LinkState |= LS_FMT_INITIALIZED;
    link.FmtData.def_ext = E_LOAD;
    link.FmtData.u.os2fam.flags = (seg_flags_type)(MULTIPLE_AUTO_DATA);
    link.FmtData.u.os2fam.heapsize = 0;
    link.FmtData.u.os2fam.segment_shift = 0;
    link.FmtData.u.os2fam.module_name = nullptr;
    link.FmtData.u.os2fam.stub_file_name = nullptr;
    link.FmtData.u.os2fam.seg_flags = nullptr;
    link.FmtData.description = nullptr;
    link.FmtData.u.os2fam.exports = nullptr;
    link.FmtData.u.os2fam.old_lib_name = nullptr;
    if (link.FmtData.type & MK_WIN_NE) {
        link.FmtData.def_seg_flags |= SEG_PRELOAD;
    }
    ChkBase(link, _64K);
}

void FreeOS2Fmt(Link& link) {
    MemFree(link.FmtData.u.os2fam.stub_file_name);
    MemFree(link.FmtData.u.os2fam.module_name);
    MemFree(link.FmtData.u.os2fam.old_lib_name);
    MemFree(link.FmtData.description);
    FreeImpNameTab(link);
    FreeExportList(link);
    FreeSegFlags(link.FmtData.u.os2fam.seg_flags);
}

namespace {

entry_export* ProcWlibDLLImportEntry(Link& link) {
    Reader& r = reader();
    std::string symname = r.str();
    std::string internal;
    bool have_internal = false;
    if (!r.get(Sep::dot_ext, Want::normal)) {
        return nullptr;
    }
    std::uint16_t ordinal = 0;
    if (r.get(Sep::dot_ext, Want::normal)) {
        if (number16(ordinal) != Number::ok) {
            if (r.text().size() > 0) {
                internal = symname;
                have_internal = true;
                symname = r.str();
            }
            if (r.get(Sep::dot_ext, Want::normal) && number16(ordinal) != Number::ok) {
                if (r.get(Sep::dot_ext, Want::normal)) {
                    number16(ordinal);
                }
            }
        }
    }
    entry_export* exp = AllocExport(link, symname.c_str(), symname.size());
    if (any(cmd_flags() & Flag::anonymous_export)) exp->isanonymous = true;
    if (have_internal) {
        exp->sym = SymOp(link, ST_CREATE_REFERENCE, internal.c_str(), internal.size());
    } else {
        exp->sym = SymOp(link, ST_CREATE_REFERENCE, symname.c_str(), symname.size());
    }
    exp->sym->info |= SYM_1_DCE_REF;    // make sure it isn't removed
    exp->ordinal = ordinal;
    if (ordinal == 0) {
        exp->isresident = true;         // no ord spec'd so must be resident
    }
    return exp;
}

bool GetWlibImports(Link& link) {
    // read in a wlib command file, get the import directives, and treat them as exports
    Reader& r = reader();
    const std::string_view name = r.text();
    char* fname = FileName(link, name.data(), name.size(), E_LBC, false);
    f_handle handle = QOpenR(link, fname);
    r.push_file(handle, fname);
    MemFree(fname);
    r.lock(true);       // make sure only this file parsed
    while (r.get(Sep::space, Want::normal)) {
        if (r.text().size() <= 2) continue;
        if (r.at(0) == '+' && r.at(1) == '+') {
            r.drop_front(2);
            if (r.at(0) == '\'') {
                r.push_back();
                if (!r.get(Sep::quote, Want::normal)) {
                    LnkMsg(link, ERR + LOC + LINE + MSG_BAD_WLIB_IMPORT, nullptr);
                    r.pop();    // get rid of this file
                    return true;
                }
            }
            entry_export* exp = ProcWlibDLLImportEntry(link);
            if (exp == nullptr) {
                LnkMsg(link, ERR + LOC + LINE + MSG_BAD_WLIB_IMPORT, nullptr);
                r.pop();        // get rid of this file
                return true;
            }
            AddToExportList(link, exp);
        }
    }
    r.lock(false);
    return true;
}

// ---- "OPtion" directive

bool ProcNoStub(Link& link) {
    link.FmtData.u.os2fam.no_stub = true;
    return true;
}

bool ProcSingle(Link& link) {
    if (any(cmd_flags() & Flag::auto_segment_flag)) {
        LnkMsg(link, WRN + LOC + LINE + MSG_AUTO_SEG_MULT_DEFD, nullptr);
    }
    link.FmtData.u.os2fam.flags |= SINGLE_AUTO_DATA;
    link.FmtData.u.os2fam.flags &= ~MULTIPLE_AUTO_DATA;
    return true;
}

bool ProcMultiple(Link& link) {
    if (any(cmd_flags() & Flag::auto_segment_flag)) {
        LnkMsg(link, WRN + LOC + LINE + MSG_AUTO_SEG_MULT_DEFD, nullptr);
    }
    link.FmtData.u.os2fam.flags &= ~SINGLE_AUTO_DATA;
    link.FmtData.u.os2fam.flags |= MULTIPLE_AUTO_DATA;
    return true;
}

bool ProcNone(Link& link) {
    if (any(cmd_flags() & Flag::auto_segment_flag)) {
        LnkMsg(link, WRN + LOC + LINE + MSG_AUTO_SEG_MULT_DEFD, nullptr);
    }
    link.FmtData.u.os2fam.flags &= ~(SINGLE_AUTO_DATA | MULTIPLE_AUTO_DATA);
    return true;
}

bool ProcOldLibrary(Link& link) {
    if (!have_equals(Want::file_name)) return false;
    const std::string_view t = reader().text();
    link.FmtData.u.os2fam.old_lib_name = FileName(link, t.data(), t.size(), E_DLL, false);
    return true;
}

bool ProcModName(Link& link) {
    Reader& r = reader();
    if (!have_equals(Want::include_dot)) return false;
    r.push_back();
    if (!r.get(Sep::none, Want::normal)) {
        r.get(Sep::none, Want::include_dot);
    }
    const std::string_view t = r.text();
    link.FmtData.u.os2fam.module_name = MemToStringSafe(link, t.data(), t.size());
    return true;
}

bool ProcNewFiles(Link& link) {
    link.FmtData.u.os2fam.flags |= LONG_FILENAMES;
    return true;
}

bool ProcProtMode(Link& link) {
    link.FmtData.u.os2fam.flags |= PROTMODE_ONLY;
    return true;
}

// CodeBreaker: the header says no application type (as KERNEL's does)
bool ProcNoAppType(Link& link) {
    link.FmtData.u.os2fam.flags |= NO_APP_TYPE;
    return true;
}

// CodeBreaker: the stub as it is, its header too (KRNL386's real-mode start steps back over a header of exactly 20h
// paragraphs) -- only its NE offset (3Ch) is filled in
bool ProcStubVerbatim(Link& link) {
    link.FmtData.u.os2fam.stub_verbatim = true;
    return true;
}

// CodeBreaker: a reference to a place in the module itself as the segment's number -- never through the entry table,
// movable segment or not -- and a far pointer to one as its offset, written in, with only its selector left to the
// loader (as KRNL386 has them: it relocates itself before there is an entry table to go through)
bool ProcSelectorFixups(Link& link) {
    link.FmtData.u.os2fam.selector_fixups = true;
    return true;
}

bool ProcNoStdCall(Link& link) {
    link.FmtData.u.pe.no_stdcall = true;
    return true;
}

bool ProcRWRelocCheck(Link& link) {
    // check for segment relocations pointing to read/write data segments
    link.FmtData.u.os2fam.chk_seg_relocs = true;
    return true;
}

bool ProcSelfRelative(Link& link) {
    link.FmtData.u.os2fam.gen_rel_relocs = true;
    return true;
}

bool ProcInternalRelocs(Link& link) {
    // in case someone wants internal relocs generated.
    link.FmtData.u.os2fam.gen_int_relocs = true;
    return true;
}

bool ProcToggleRelocsFlag(Link& link) {
    // Rational wants internal relocs generated, but wants the "no internal relocs" flag set
    link.FmtData.u.os2fam.toggle_relocs = true;
    return true;
}

bool ProcLinkVersion(Link& link) {
    Version vb;
    // set required limits, 0 = no limit
    vb.major = 255;
    vb.minor = 255;
    vb.revision = 0;
    vb.what = "LINKVERSION";
    if (get_version(link, vb, VersionPart::major | VersionPart::minor, false) != VersionPart::none) {
        link.FmtData.u.pe.lnk_specd = true;
        link.FmtData.u.pe.linkmajor = vb.major;
        link.FmtData.u.pe.linkminor = vb.minor;
        return true;
    }
    return false;
}

bool ProcOsVersion(Link& link) {
    Version vb;
    // set required limits, 0 = no limit
    vb.major = 0;
    vb.minor = 99;          // from old default of 100 max
    vb.revision = 0;
    vb.what = "OSVERSION";
    if (get_version(link, vb, VersionPart::major | VersionPart::minor, false) != VersionPart::none) {
        link.FmtData.u.pe.osv_specd = true;
        link.FmtData.u.pe.osmajor = vb.major;
        link.FmtData.u.pe.osminor = vb.minor;
        return true;
    }
    return false;       // error has occurred
}

bool ProcChecksum(Link& link) {
    link.FmtData.u.pe.checksumfile = true;
    return true;
}

bool ProcLargeAddressAware(Link& link) {
    link.FmtData.u.pe.largeaddressaware = true;
    link.FmtData.u.pe.nolargeaddressaware = false;
    return true;
}

bool ProcNoLargeAddressAware(Link& link) {
    link.FmtData.u.pe.nolargeaddressaware = true;
    link.FmtData.u.pe.largeaddressaware = false;
    return true;
}

bool ProcMixed1632(Link& link) {
    // Sometimes it's useful to mix 16-bit and 32-bit code/data into one segment, specially for OS/2 Device Drivers
    link.LinkFlags &= ~LF_FAR_CALLS_FLAG;    // must be turned off for mixed code
    link.FmtData.u.os2fam.mixed1632 = true;
    return true;
}

constexpr Entry MainOptions[] = {
    {"NOSTUB",              ProcNoStub,                 MK_NE | MK_OS2_FLAT | MK_PE | MK_WIN_VXD},
    {"ONEautodata",         ProcSingle,                 MK_NE | MK_OS2_FLAT,                        Flag::auto_segment_flag},
    {"MANYautodata",        ProcMultiple,               MK_NE | MK_OS2_FLAT,                        Flag::auto_segment_flag},
    {"NOAutodata",          ProcNone,                   MK_NE,                                      Flag::auto_segment_flag},
    {"OLDlibrary",          ProcOldLibrary,             MK_NE | MK_OS2_FLAT | MK_PE},
    {"MODName",             ProcModName,                MK_NE | MK_OS2_FLAT | MK_PE | MK_WIN_VXD},
    {"NEWFiles",            ProcNewFiles,               MK_OS2_NE},
    {"PROTmode",            ProcProtMode,               MK_NE},
    {"NOAPPType",           ProcNoAppType,              MK_NE},
    {"SELECTORFixups",      ProcSelectorFixups,         MK_NE},
    {"STUBVERBatim",        ProcStubVerbatim,           MK_NE},
    {"NOSTDCall",           ProcNoStdCall,              MK_PE},
    {"RWReloccheck",        ProcRWRelocCheck,           MK_WIN_NE},
    {"SELFrelative",        ProcSelfRelative,           MK_OS2_LX},
    {"INTernalrelocs",      ProcInternalRelocs,         MK_OS2_LX},
    {"TOGglerelocsflag",    ProcToggleRelocsFlag,       MK_OS2_LX},
    {"LINKVersion",         ProcLinkVersion,            MK_PE},
    {"OSVersion",           ProcOsVersion,              MK_PE},
    {"CHECKSUM",            ProcChecksum,               MK_PE},
    {"LARGEaddressaware",   ProcLargeAddressAware,      MK_PE},
    {"NOLARGEaddressaware", ProcNoLargeAddressAware,    MK_PE},
    {"MIXed1632",           ProcMixed1632,              MK_OS2_FLAT | MK_WIN_VXD},
};

// ---- "RESource" directive

bool AddResource(Link& link) {
    const std::string_view t = reader().text();
    char* str = MemToStringSafe(link, t.data(), t.size());
    DoAddResource(link, str);
    MemFree(str);
    return true;
}

// ---- "IMPort" directive

bool getimport(Link& link) {
    Reader& r = reader();
    const std::string intname = r.str();
    if (!r.get(Sep::none, Want::normal)) {
        return false;
    }
    const std::string modname = r.str();
    std::uint16_t ordinal = 0;
    length_name intn = {intname.size(), intname.c_str()};
    length_name modn = {modname.size(), modname.c_str()};
    if (r.get(Sep::period, Want::include_dot)) {
        const Number state = number16(ordinal);
        if (state == Number::too_big) {
            LnkMsg(link, LOC + LINE + MSG_IMPORT_ORD_INVALID + ERR, nullptr);
        } else if (state == Number::ok) {
            HandleImport(link, &intn, &modn, nullptr, ordinal);
        } else if (state == Number::not_a_number) {
            const std::string_view t = r.text();
            length_name extname = {t.size(), t.data()};
            HandleImport(link, &intn, &modn, &extname, IMP_BY_NAME);
        }
    } else {
        HandleImport(link, &intn, &modn, &intn, IMP_BY_NAME);
    }
    return true;
}

// ---- "EXPort" directive

bool ProcExpResident(Link& link) {
    link.FmtData.u.os2fam.exports->isresident = true;
    return true;
}

bool ProcPrivate(Link& link) {
    link.FmtData.u.os2fam.exports->isprivate = true;
    return true;
}

// CodeBreaker: NODATA, as in a .def file -- the export keeps its caller's data segment
bool ProcExpNoData(Link& link) {
    link.FmtData.u.os2fam.exports->isnodata = true;
    return true;
}

constexpr Entry Exp_Keywords[] = {
    {"RESident",    ProcExpResident,    MK_NE | MK_OS2_FLAT},
    {"NODATA",      ProcExpNoData,      MK_NE},
    {"PRIVATE",     ProcPrivate,        MK_NE | MK_OS2_FLAT | MK_PE},
};

bool getexport(Link& link) {
    Reader& r = reader();
    std::uint16_t val16;
    std::uint32_t val32;

    const std::string_view name = r.text();
    entry_export* exp = AllocExport(link, name.data(), name.size());
    if (any(cmd_flags() & Flag::anonymous_export)) exp->isanonymous = true;
    if (r.get(Sep::period, Want::include_dot)) {
        if (number(val32) != Number::ok) {
            LnkMsg(link, ERR + LOC + LINE + MSG_EXPORT_ORD_INVALID, nullptr);
            MemFree(exp);
            r.get(Sep::equals, Want::include_dot);
            return true;
        }
        exp->ordinal = val32;
    }
    if (r.get(Sep::equals, Want::include_dot)) {
        const std::string_view s = r.text();
        exp->sym = SymOp(link, ST_CREATE_REFERENCE, s.data(), s.size());
        if (r.get(Sep::equals, Want::include_dot)) {
            const std::string_view i = r.text();
            exp->impname = MemToStringSafe(link, i.data(), i.size());
        }
    } else {
        exp->sym = RefISymbol(link, exp->name.u.ptr);
    }
    exp->sym->info |= SYM_1_DCE_REF;        // make sure it is not removed
    if (exp->ordinal == 0) {
        exp->isresident = true;             // no ordinal spec'd so must be kept resident
    }
    exp->next = link.FmtData.u.os2fam.exports;   // put in the front of the list for
    link.FmtData.u.os2fam.exports = exp;         // now so ProcResidant can get to it.
    while (process_one(link, Exp_Keywords, Sep::none)) {
        /* nothing */
    }
    link.FmtData.u.os2fam.exports = exp->next;   // take it off the list
    exp->iopl_words = 0;
    if ((link.FmtData.type & (MK_WIN_NE | MK_PE)) == 0 && r.get(Sep::none, Want::include_dot)) {
        if (number16(val16) == Number::ok) {
            if (val16 > 63) {
                LnkMsg(link, LOC + LINE + MSG_TOO_MANY_IOPL_WORDS + ERR, nullptr);
            } else {
                if (val16 & 1) {
                    LnkMsg(link, WRN + LOC + LINE + MSG_IOPL_BYTES_ODD, nullptr);
                }
                // The linker takes bytes as input!
                exp->iopl_words = val16 / 2;
            }
        } else {
            r.push_back();      // reprocess the token.
        }
    }
    AddToExportList(link, exp);
    return true;
}

// ---- "SEGment" directive

bool ProcOS2Class(Link& link) {
    link.FmtData.u.os2fam.seg_flags->type = SEGFLAG_CLASS;
    return true;
}

bool ProcSegCode(Link& link) {
    link.FmtData.u.os2fam.seg_flags->type = SEGFLAG_CODE;
    return true;
}

bool ProcSegData(Link& link) {
    link.FmtData.u.os2fam.seg_flags->type = SEGFLAG_DATA;
    return true;
}

constexpr Entry SegTypeDesc[] = {
    {"CODE", ProcSegCode, MK_NE | MK_OS2_FLAT | MK_PE | MK_WIN_VXD},
    {"DATA", ProcSegData, MK_NE | MK_OS2_FLAT | MK_PE | MK_WIN_VXD},
};

bool ProcSegType(Link& link) {
    if (!process_one(link, SegTypeDesc, Sep::none)) {
        LnkMsg(link, WRN + LOC + LINE + MSG_INVALID_TYPE_DESC, nullptr);
    }
    return true;
}

bool ProcPreload(Link& link) {
    if (link.FmtData.u.os2fam.seg_flags->specified & SEG_PRELOAD) {
        LnkMsg(link, WRN + LOC + LINE + MSG_SEG_FLAG_MULT_DEFD, nullptr);
    }
    link.FmtData.u.os2fam.seg_flags->flags |= SEG_PRELOAD;
    link.FmtData.u.os2fam.seg_flags->specified |= SEG_PRELOAD;
    return true;
}

bool ProcLoadoncall(Link& link) {
    if (link.FmtData.u.os2fam.seg_flags->specified & SEG_PRELOAD) {
        LnkMsg(link, WRN + LOC + LINE + MSG_SEG_FLAG_MULT_DEFD, nullptr);
    }
    link.FmtData.u.os2fam.seg_flags->flags &= ~SEG_PRELOAD;
    link.FmtData.u.os2fam.seg_flags->specified |= SEG_PRELOAD;
    return true;
}

bool ProcIopl(Link& link) {
    if (link.FmtData.u.os2fam.seg_flags->specified & SEG_IOPL_SPECD) {
        LnkMsg(link, WRN + LOC + LINE + MSG_SEG_FLAG_MULT_DEFD, nullptr);
    }
    SET_SEG_PMODE_DPL(link.FmtData.u.os2fam.seg_flags->flags, SEG_PMODE_DPL_2);
    link.FmtData.u.os2fam.seg_flags->specified |= SEG_IOPL_SPECD;
    return true;
}

bool ProcNoIopl(Link& link) {
    if (link.FmtData.u.os2fam.seg_flags->specified & SEG_IOPL_SPECD) {
        LnkMsg(link, WRN + LOC + LINE + MSG_SEG_FLAG_MULT_DEFD, nullptr);
    }
    SET_SEG_PMODE_DPL(link.FmtData.u.os2fam.seg_flags->flags, SEG_PMODE_DPL_3);
    link.FmtData.u.os2fam.seg_flags->specified |= SEG_IOPL_SPECD;
    return true;
}

bool ProcExecuteonly(Link& link) {
    if (link.FmtData.u.os2fam.seg_flags->specified & SEG_RFLAG) {
        LnkMsg(link, WRN + LOC + LINE + MSG_SEG_FLAG_MULT_DEFD, nullptr);
    }
    link.FmtData.u.os2fam.seg_flags->flags |= SEG_READ_ONLY;
    link.FmtData.u.os2fam.seg_flags->specified |= SEG_READ_ONLY;
    return true;
}

bool ProcExecuteread(Link& link) {
    if (link.FmtData.u.os2fam.seg_flags->specified & SEG_RFLAG) {
        LnkMsg(link, WRN + LOC + LINE + MSG_SEG_FLAG_MULT_DEFD, nullptr);
    }
    link.FmtData.u.os2fam.seg_flags->flags &= ~SEG_READ_ONLY;
    link.FmtData.u.os2fam.seg_flags->specified |= SEG_READ_ONLY;
    return true;
}

bool ProcShared(Link& link) {
    if (link.FmtData.u.os2fam.seg_flags->specified & SEG_PURE) {
        LnkMsg(link, WRN + LOC + LINE + MSG_SEG_FLAG_MULT_DEFD, nullptr);
    }
    link.FmtData.u.os2fam.seg_flags->flags |= SEG_PURE;
    link.FmtData.u.os2fam.seg_flags->specified |= SEG_PURE;
    return true;
}

bool ProcNonShared(Link& link) {
    if (link.FmtData.u.os2fam.seg_flags->specified & SEG_PURE) {
        LnkMsg(link, WRN + LOC + LINE + MSG_SEG_FLAG_MULT_DEFD, nullptr);
    }
    link.FmtData.u.os2fam.seg_flags->flags &= ~SEG_PURE;
    link.FmtData.u.os2fam.seg_flags->specified |= SEG_PURE;
    return true;
}

bool ProcReadOnly(Link& link) {
    if (link.FmtData.u.os2fam.seg_flags->specified & SEG_RFLAG) {
        LnkMsg(link, WRN + LOC + LINE + MSG_SEG_FLAG_MULT_DEFD, nullptr);
    }
    link.FmtData.u.os2fam.seg_flags->flags |= SEG_READ_ONLY;
    link.FmtData.u.os2fam.seg_flags->specified |= SEG_READ_SPECD;
    return true;
}

bool ProcReadWrite(Link& link) {
    if (link.FmtData.u.os2fam.seg_flags->specified & SEG_RFLAG) {
        LnkMsg(link, WRN + LOC + LINE + MSG_SEG_FLAG_MULT_DEFD, nullptr);
    }
    link.FmtData.u.os2fam.seg_flags->flags &= ~SEG_READ_ONLY;
    link.FmtData.u.os2fam.seg_flags->specified |= SEG_READ_SPECD;
    return true;
}

bool ProcConforming(Link& link) {
    if (link.FmtData.u.os2fam.seg_flags->specified & SEG_CONFORMING) {
        LnkMsg(link, WRN + LOC + LINE + MSG_SEG_FLAG_MULT_DEFD, nullptr);
    }
    link.FmtData.u.os2fam.seg_flags->flags |= SEG_CONFORMING;
    link.FmtData.u.os2fam.seg_flags->specified |= SEG_CONFORMING;
    return true;
}

bool ProcNonConforming(Link& link) {
    if (link.FmtData.u.os2fam.seg_flags->specified & SEG_CONFORMING) {
        LnkMsg(link, WRN + LOC + LINE + MSG_SEG_FLAG_MULT_DEFD, nullptr);
    }
    link.FmtData.u.os2fam.seg_flags->flags &= ~SEG_CONFORMING;
    link.FmtData.u.os2fam.seg_flags->specified |= SEG_CONFORMING;
    return true;
}

bool ProcMovable(Link& link) {
    if (link.FmtData.u.os2fam.seg_flags->specified & SEG_MOVABLE) {
        LnkMsg(link, WRN + LOC + LINE + MSG_SEG_FLAG_MULT_DEFD, nullptr);
    }
    link.FmtData.u.os2fam.seg_flags->flags |= SEG_MOVABLE;
    link.FmtData.u.os2fam.seg_flags->specified |= SEG_MOVABLE;
    return true;
}

bool ProcFixed(Link& link) {
    if (link.FmtData.u.os2fam.seg_flags->specified & SEG_MOVABLE) {
        LnkMsg(link, WRN + LOC + LINE + MSG_SEG_FLAG_MULT_DEFD, nullptr);
    }
    link.FmtData.u.os2fam.seg_flags->flags &= ~SEG_MOVABLE;
    link.FmtData.u.os2fam.seg_flags->specified |= SEG_MOVABLE;
    return true;
}

bool ProcDiscardable(Link& link) {
    link.FmtData.u.os2fam.seg_flags->flags |= SEG_DISCARD;
    return true;
}

bool ProcNonDiscardable(Link& link) {
    link.FmtData.u.os2fam.seg_flags->flags &= ~SEG_DISCARD;
    return true;
}

bool ProcInvalid(Link& link) {
    link.FmtData.u.os2fam.seg_flags->flags |= SEG_INVALID;
    return true;
}

bool ProcContiguous(Link& link) {
    link.FmtData.u.os2fam.seg_flags->flags |= SEG_CONTIGUOUS;
    return true;
}

bool ProcOS2Dynamic(Link& link) {
    return true;
}

bool ProcPermanent(Link& link) {
    if (link.FmtData.u.os2fam.seg_flags->specified & SEG_RESIDENT) {
        LnkMsg(link, WRN + LOC + LINE + MSG_SEG_FLAG_MULT_DEFD, nullptr);
    }
    link.FmtData.u.os2fam.seg_flags->flags |= SEG_RESIDENT;
    link.FmtData.u.os2fam.seg_flags->specified |= SEG_RESIDENT;
    return true;
}

bool ProcNonPermanent(Link& link) {
    if (link.FmtData.u.os2fam.seg_flags->specified & SEG_RESIDENT) {
        LnkMsg(link, WRN + LOC + LINE + MSG_SEG_FLAG_MULT_DEFD, nullptr);
    }
    link.FmtData.u.os2fam.seg_flags->flags &= ~SEG_RESIDENT;
    link.FmtData.u.os2fam.seg_flags->specified |= SEG_RESIDENT;
    return true;
}

bool ProcPageable(Link& link) {
    if (link.FmtData.u.os2fam.seg_flags->specified & SEG_NOPAGE) {
        LnkMsg(link, WRN + LOC + LINE + MSG_SEG_FLAG_MULT_DEFD, nullptr);
    }
    link.FmtData.u.os2fam.seg_flags->flags &= ~SEG_NOPAGE;
    link.FmtData.u.os2fam.seg_flags->specified |= SEG_NOPAGE;
    return true;
}

bool ProcNonPageable(Link& link) {
    if (link.FmtData.u.os2fam.seg_flags->specified & SEG_NOPAGE) {
        LnkMsg(link, WRN + LOC + LINE + MSG_SEG_FLAG_MULT_DEFD, nullptr);
    }
    link.FmtData.u.os2fam.seg_flags->flags |= SEG_NOPAGE;
    link.FmtData.u.os2fam.seg_flags->specified |= SEG_NOPAGE;
    return true;
}

constexpr Entry SegDesc[] = {
    {"Class",   ProcOS2Class,   MK_NE | MK_OS2_FLAT | MK_PE | MK_WIN_VXD},
    {"TYpe",    ProcSegType,    MK_NE | MK_OS2_FLAT | MK_PE | MK_WIN_VXD},
};

constexpr Entry SegModel[] = {
    {"PReload",         ProcPreload,        MK_NE | MK_OS2_FLAT | MK_WIN_VXD},
    {"LOadoncall",      ProcLoadoncall,     MK_NE | MK_OS2_FLAT | MK_WIN_VXD},
    {"Iopl",            ProcIopl,           MK_OS2 | MK_WIN_VXD},
    {"NOIopl",          ProcNoIopl,         MK_OS2 | MK_WIN_VXD},
    {"EXECUTEOnly",     ProcExecuteonly,    MK_NE | MK_OS2_FLAT},
    {"EXECUTERead",     ProcExecuteread,    MK_NE | MK_OS2_FLAT},
    {"SHared",          ProcShared,         MK_NE | MK_OS2_FLAT | MK_PE | MK_WIN_VXD},
    {"NONShared",       ProcNonShared,      MK_NE | MK_OS2_FLAT | MK_PE | MK_WIN_VXD},
    {"READOnly",        ProcReadOnly,       MK_NE | MK_OS2_FLAT},
    {"READWrite",       ProcReadWrite,      MK_NE | MK_OS2_FLAT},
    {"CONforming",      ProcConforming,     MK_OS2 | MK_WIN_VXD},
    {"NONConforming",   ProcNonConforming,  MK_OS2 | MK_WIN_VXD},
    {"MOVeable",        ProcMovable,        MK_NE},
    {"FIXed",           ProcFixed,          MK_WIN_NE},
    {"DIScardable",     ProcDiscardable,    MK_WIN_NE | MK_WIN_VXD},
    {"NONDiscardable",  ProcNonDiscardable, MK_WIN_VXD},
    {"INValid",         ProcInvalid,        MK_OS2_FLAT},
    {"RESident",        ProcPermanent,      MK_OS2_FLAT | MK_WIN_VXD},
    {"CONTiguous",      ProcContiguous,     MK_OS2_FLAT},
    {"DYNamic",         ProcOS2Dynamic,     MK_OS2_FLAT},
    {"PERManent",       ProcPermanent,      MK_OS2_FLAT},
    {"NONPERManent",    ProcNonPermanent,   MK_OS2_FLAT},
    {"PAGEable",        ProcPageable,       MK_PE},
    {"NONPageable",     ProcNonPageable,    MK_PE},
};

bool getsegflags(Link& link) {
    Reader& r = reader();
    r.push_back();
    seg_flags_struct* seg_flags = (seg_flags_struct*)(MemAllocSafe(link, sizeof(seg_flags_struct)));
    seg_flags->specified = (seg_flags_type)(0);
    seg_flags->flags = link.FmtData.def_seg_flags;   // default value.
    seg_flags->name = nullptr;
    seg_flags->type = SEGFLAG_SEGMENT;
    seg_flags->next = link.FmtData.u.os2fam.seg_flags;
    link.FmtData.u.os2fam.seg_flags = seg_flags;
    process_one(link, SegDesc, Sep::none);            // look for an optional segdesc
    if (seg_flags->type != SEGFLAG_CODE && seg_flags->type != SEGFLAG_DATA) {
        if (!r.get(Sep::none, Want::include_dot)) {
            link.FmtData.u.os2fam.seg_flags = seg_flags->next;
            MemFree(seg_flags);
            return false;
        }
        const std::string_view t = r.text();
        seg_flags->name = MemToStringSafe(link, t.data(), t.size());
    }
    while (process_one(link, SegModel, Sep::none)) {
        /* nothing */
    }
    return true;
}

// ---- "COMmit" directive

bool ProcCommitStack(Link& link) {
    return get_number(link.FmtData.u.pe.stackcommit);
}

bool ProcCommitHeap(Link& link) {
    return get_number(link.FmtData.u.pe.heapcommit);
}

constexpr Entry CommitKeywords[] = {
    {"STack",   ProcCommitStack,    MK_PE},
    {"Heap",    ProcCommitHeap,     MK_PE},
};

bool AddCommit(Link& link) {
    reader().push_back();
    return process_one(link, CommitKeywords, Sep::none);
}

// ---- "RUntime" directive

void GetSubsystemVersion(Link& link) {
    Version vb;
    // set required limits, 0 = no limit
    vb.major = 0;
    vb.minor = 0;
    vb.revision = 0;
    vb.what = "SUBSYSTEM";
    if (get_version(link, vb, VersionPart::major | VersionPart::minor, false) != VersionPart::none) {
        link.FmtData.u.pe.sub_specd = true;
        link.FmtData.u.pe.submajor = vb.major;
        link.FmtData.u.pe.subminor = vb.minor;
    }
}

bool ProcRunNative(Link& link) {
    link.FmtData.u.pe.subsystem = PE_SS_NATIVE;
    GetSubsystemVersion(link);
    return true;
}

bool ProcRunWindows(Link& link) {
    link.FmtData.u.pe.subsystem = PE_SS_WINDOWS_GUI;
    GetSubsystemVersion(link);
    return true;
}

bool ProcRunConsole(Link& link) {
    link.FmtData.u.pe.subsystem = PE_SS_WINDOWS_CHAR;
    GetSubsystemVersion(link);
    return true;
}

bool ProcRunPosix(Link& link) {
    link.FmtData.u.pe.subsystem = PE_SS_POSIX_CHAR;
    GetSubsystemVersion(link);
    return true;
}

bool ProcRunOS2(Link& link) {
    link.FmtData.u.pe.subsystem = PE_SS_OS2_CHAR;
    GetSubsystemVersion(link);
    return true;
}

bool ProcRunDosstyle(Link& link) {
    link.FmtData.u.pe.subsystem = PE_SS_PL_DOSSTYLE;
    GetSubsystemVersion(link);
    return true;
}

bool ProcRunRDOS(Link& link) {
    link.FmtData.u.pe.subsystem = PE_SS_RDOS;
    GetSubsystemVersion(link);
    return true;
}

bool ProcRunEFIBoot(Link& link) {
    link.FmtData.def_ext = E_EFI;
    link.FmtData.u.pe.subsystem = PE_SS_EFI_BOOT;
    GetSubsystemVersion(link);
    return true;
}

constexpr Entry RunOptions[] = {
    {"NATive",   ProcRunNative,   MK_PE},
    {"WINdows",  ProcRunWindows,  MK_PE},
    {"CONsole",  ProcRunConsole,  MK_PE},
    {"POSix",    ProcRunPosix,    MK_PE},
    {"OS2",      ProcRunOS2,      MK_PE},
    {"DOSstyle", ProcRunDosstyle, MK_PE},
    {"RDOS",     ProcRunRDOS,     MK_PE},
    {"EFIBoot",  ProcRunEFIBoot,  MK_PE},
};

// ---- "Format" directive

bool ProcInitGlobal(Link& link) {
    link.FmtData.u.os2fam.flags &= ~INIT_INSTANCE_FLAG;
    return true;
}

bool ProcInitInstance(Link& link) {
    link.FmtData.u.os2fam.flags |= INIT_INSTANCE_FLAG;
    return true;
}

bool ProcInitThread(Link& link) {
    link.FmtData.u.os2fam.flags |= INIT_THREAD_FLAG;
    return true;
}

bool ProcTermGlobal(Link& link) {
    link.FmtData.u.os2fam.flags &= ~TERM_INSTANCE_FLAG;
    return true;
}

bool ProcTermInstance(Link& link) {
    link.FmtData.u.os2fam.flags |= TERM_INSTANCE_FLAG;
    return true;
}

bool ProcTermThread(Link& link) {
    link.FmtData.u.os2fam.flags |= TERM_THREAD_FLAG;
    return true;
}

constexpr Entry Init_Keywords[] = {
    {"INITGlobal",   ProcInitGlobal,    MK_NE | MK_OS2_FLAT | MK_PE},
    {"INITInstance", ProcInitInstance,  MK_NE | MK_OS2_FLAT | MK_PE},
    {"INITThread",   ProcInitThread,    MK_PE},
};

constexpr Entry Term_Keywords[] = {
    {"TERMGlobal",   ProcTermGlobal,    MK_OS2_FLAT | MK_PE},
    {"TERMInstance", ProcTermInstance,  MK_OS2_FLAT | MK_PE},
    {"TERMThread",   ProcTermThread,    MK_PE},
};

bool ProcOS2DLL(Link& link) {
    link.FmtData.def_ext = E_DLL;
    link.FmtData.dll = true;
    if (link.FmtData.type & MK_WIN_NE) {
        link.FmtData.u.os2fam.flags &= ~MULTIPLE_AUTO_DATA;
        link.FmtData.u.os2fam.flags |= SINGLE_AUTO_DATA;
        link.FmtData.def_seg_flags |= SEG_PURE | SEG_MOVABLE;
    }
    if (process_one(link, Init_Keywords, Sep::none)) {
        if (!process_one(link, Term_Keywords, Sep::none)) {
            if (link.FmtData.u.os2fam.flags & INIT_INSTANCE_FLAG) {
                link.FmtData.u.os2fam.flags |= TERM_INSTANCE_FLAG;
            }
        }
    }
    return true;
}

bool ProcLX(Link& link) {
    return true;
}

bool ProcLE(Link& link) {
    return true;
}

bool ProcTNT(Link& link) {
    link.FmtData.u.pe.tnt = true;
    return true;
}

constexpr Entry PESubFormats[] = {
    {"TNT", ProcTNT, MK_PE},
};

bool ProcPE(Link& link) {
    process_one(link, PESubFormats, Sep::none);
    link.FmtData.u.os2fam.heapsize = DEF_VALUE;      // another arbitrary non-zero default
    link.FmtData.u.os2fam.segment_shift = 9;         // 512 byte arbitrary rounding
    link.FmtData.u.pe.heapcommit = DEF_VALUE;        // arbitrary non-zero default.
    link.FmtData.u.pe.stackcommit = DEF_VALUE;
    return true;
}

bool ProcDynamicDriver(Link& link) {
    link.FmtData.u.os2fam.flags |= VIRT_DEVICE;
    return true;
}

bool ProcStaticDriver(Link& link) {
    link.FmtData.u.os2fam.flags |= PHYS_DEVICE;
    return true;
}

constexpr Entry VXDSubFormats[] = {
    {"DYNamic", ProcDynamicDriver,  MK_WIN_VXD},
    {"STATic",  ProcStaticDriver,   MK_WIN_VXD},
};

bool ProcVXD(Link& link) {
    process_one(link, VXDSubFormats, Sep::none);
    link.FmtData.dll = true;
    return true;
}

bool ProcMemory(Link& link) {
    link.FmtData.u.os2fam.flags |= CLEAN_MEMORY;
    return true;
}

bool ProcFont(Link& link) {
    link.FmtData.u.os2fam.flags |= PROPORTIONAL_FONT;
    return true;
}

bool ProcPM(Link& link) {
    link.FmtData.u.os2fam.flags |= PM_APPLICATION;
    return true;
}

bool ProcPMCompatible(Link& link) {
    link.FmtData.u.os2fam.flags |= PM_COMPATIBLE;
    return true;
}

bool ProcPMFullscreen(Link& link) {
    link.FmtData.u.os2fam.flags |= PM_NOT_COMPATIBLE;
    return true;
}

bool ProcPhysDevice(Link& link) {
    link.FmtData.def_ext = E_DLL;
    link.FmtData.dll = true;
    link.FmtData.u.os2fam.flags |= PHYS_DEVICE;
    return true;
}

bool ProcVirtDevice(Link& link) {
    link.FmtData.def_ext = E_DLL;
    link.FmtData.dll = true;
    link.FmtData.u.os2fam.flags |= VIRT_DEVICE;
    return true;
}

constexpr Entry WindowsFormatOptions[] = {
    {"MEMory",      ProcMemory,     MK_WIN_NE},
    {"FOnt",        ProcFont,       MK_WIN_NE},
    // VxD module type override: static(0x28000)/dynamic(0x38000) is a build-time property (not derivable from the DDB),
    // so allow it on `format windows vxd'. VIRTdevice -> dynamic (VXDLDR-loadable), PHYSdevice -> static. Default is
    // the DDB-driven choice (see loadflat.c), which is static for boot VxDs.
    {"VIRTdevice",  ProcVirtDevice, MK_WIN_VXD},
    {"PHYSdevice",  ProcPhysDevice, MK_WIN_VXD},
};

constexpr Entry WindowsSubFormats[] = {
    {"DLl", ProcOS2DLL, MK_WIN_NE | MK_PE},
};

constexpr Entry WindowsFormats[] = {
    {"NT",  ProcPE,     MK_PE},
    {"PE",  ProcPE,     MK_PE},
    {"VXD", ProcVXD,    MK_WIN_VXD},
};

constexpr Entry OS2SubFormats[] = {
    {"DLl",          ProcOS2DLL,        MK_OS2},
    {"PHYSdevice",   ProcPhysDevice,    MK_OS2_FLAT},
    {"VIRTdevice",   ProcVirtDevice,    MK_OS2_FLAT},
    {"PM",           ProcPM,            MK_OS2},
    {"PMCompatible", ProcPMCompatible,  MK_OS2},
    {"FULLscreen",   ProcPMFullscreen,  MK_OS2},
};

constexpr Entry OS2Formats[] = {
    {"FLat",    ProcLX, MK_OS2_LX},
    {"LE",      ProcLE, MK_OS2_LE},
    {"LX",      ProcLX, MK_OS2_LX},
};

}  // namespace

bool ProcOS2Options(Link& link) {
    return process_one(link, MainOptions, Sep::none);
}

bool ProcOS2NoRelocs(Link& link) {
    link.LinkState &= ~LS_MAKE_RELOCS;
    return true;
}

bool ProcOS2Alignment(Link& link) {
    // process Alignment option
    std::uint32_t value;
    if (!have_equals(Want::normal)) return false;
    const Number ret = number(value);
    if (ret != Number::ok || value == 0) {
        return false;
    }
    link.FmtData.u.os2fam.segment_shift = log2_32(value - 1) + 1;     // round up.
    return true;
}

bool ProcOS2HeapSize(Link& link) {
    std::uint32_t value;
    if (!have_equals(Want::normal)) return false;
    const Number ret = number(value);
    if (ret != Number::ok || value == 0) {
        LnkMsg(link, WRN + LOC + LINE + MSG_VALUE_INCORRECT, "s", "HEAPSIZE");
    } else {
        link.FmtData.u.os2fam.heapsize = value;
    }
    return true;
}

bool ProcResource(Link& link) {
    return process_list(link, AddResource, Want::file_name);
}

bool ProcOS2Import(Link& link) {
    return process_list(link, getimport, Want::normal);
}

bool ProcOS2Export(Link& link) {
    bool retval;
    if (reader().get(Sep::equals, Want::include_dot)) {
        retval = GetWlibImports(link);
    } else {
        retval = process_list(link, getexport, Want::normal);
    }
    return retval;
}

bool ProcAnonExport(Link& link) {
    cmd_flags() |= Flag::anonymous_export;
    const bool retval = ProcOS2Export(link);
    cmd_flags() &= ~Flag::anonymous_export;
    return retval;
}

bool ProcOS2Segment(Link& link) {
    return process_list(link, getsegflags, Want::include_dot);
}

bool ProcCommit(Link& link) {
    // set NT stack and heap commit sizes.
    return process_list(link, AddCommit, Want::include_dot);
}

bool ProcOS2Runtime(Link& link) {
    return process_one(link, RunOptions, Sep::none);
}

bool ProcOS2Format(Link& link) {
    // process the format os2 or format windows directives
    link.FmtData.def_ext = E_LOAD;
    process_one(link, OS2Formats, Sep::none);
    process_one(link, OS2SubFormats, Sep::none);
    if (link.FmtData.type & MK_OS2_LX) {
        if (link.FmtData.dll) {
            link.FmtData.u.os2fam.gen_int_relocs = true;
        }
    }
    if (link.FmtData.type & MK_OS2_NE) {     // if no 32-bit thing specd
        HintFormat(link, MK_OS2_NE);          // make sure 16-bit is what we get
        if (link.FmtData.dll) {
            link.FmtData.u.os2fam.flags &= ~MULTIPLE_AUTO_DATA;
            link.FmtData.u.os2fam.flags |= SINGLE_AUTO_DATA;
        }
    }
    return true;
}

bool ProcWindowsFormat(Link& link) {
    // process the format windows directives
    link.FmtData.def_ext = E_LOAD;
    process_one(link, WindowsFormats, Sep::none);
    process_one(link, WindowsSubFormats, Sep::none);
    if (link.FmtData.type & (MK_WIN_NE | MK_WIN_VXD)) {
        while (process_one(link, WindowsFormatOptions, Sep::none)) {
            /* nothing */
        }
    }
    return true;
}

}  // namespace cb::x86link::directives

#endif
