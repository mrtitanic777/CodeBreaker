// build/x86link/directives/dos.cpp -- DOS load files: overlays and sections (a port of wlink's cmddos.c).
#include <cstring>

#include "build/x86link/directives/common.hpp"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include "build/x86link/directives/formats.hpp"
#include "build/x86link/directives/state.hpp"
#include "build/x86link/directives/cmdline.h"
#include "build/x86link/debuginfo/dbgall.h"
#include "build/x86link/layout/distrib.h"
#include "build/x86link/layout/objcalc.h"
#include "build/x86link/layout/overlays.h"

#ifdef _EXE

namespace cb::x86link::directives {

namespace {

// this is an arbitrary non-zero value put in the sect->relocs field to signify that ProcBegin already made a new
// section, so ProcSection should not.
constexpr int SECT_ALREADY_MADE = 1;

byte& ovl_level() { return state().ovl_level; }   // (wlink's file-static OvlLevel)

void SetOvlClasses() {
    // make sure the overlay loader is always "overlayed".
    if (wlink().OvlClasses != nullptr) {
        auto* ovlmgr = static_cast<list_of_names*>(_PermAlloc(sizeof(list_of_names) + OVL_MGR_CL_LEN));
        std::memcpy(ovlmgr->name, OvlMgrClass, OVL_MGR_CL_LEN + 1);
        ovlmgr->next = wlink().OvlClasses;
        wlink().OvlClasses = ovlmgr;
    }
}

section* OvlNewSection() {
    wlink().OvlSectNum++;
    return NewSection();
}

void NewArea(section* sect) {
    // allocate a new area including this section
    auto* area = static_cast<areasect*>(_PermAlloc(sizeof(areasect)));
    area->next = nullptr;
    area->sections = sect;
    sect->parent = wlink().CurrSect;
    areasect** owner;
    for (owner = &wlink().CurrSect->areas; *owner != nullptr; owner = &(*owner)->next) {
    }
    *owner = area;
}

void MakeNonArea() {
    // make a new overlay area for non-overlay classes
    wlink().NonSect = OvlNewSection();   // No debug info in nonsect.
    NewArea(wlink().NonSect);
}

// ---- "OPtion" Directive

bool ProcDistribute() {
    wlink().FmtData.u.dos.distribute = true;
    return true;
}

bool ProcPadSections() {
    wlink().FmtData.u.dos.pad_sections = true;
    return true;
}

bool ProcSmall() {
    wlink().FmtData.u.dos.ovl_short = true;
    return true;
}

bool ProcDynamic() {
    wlink().FmtData.u.dos.dynamic = true;
    return true;
}

bool ProcNoIndirect() {
    wlink().FmtData.u.dos.noindirect = true;
    return true;
}

bool ProcFullHeader() {
    wlink().FmtData.u.dos.full_mz_hdr = true;
    return true;
}

bool ProcStandard() {
    wlink().FmtData.u.dos.dynamic = false;
    return true;
}

bool ProcArea() {
    // process the area size directive.
    unsigned_32 value;
    const bool ret = get_number(value);
    if (ret) {
        wlink().OvlAreaSize = (value + wlink().FmtData.SegMask) >> wlink().FmtData.SegShift;
    }
    return ret;
}

constexpr Entry MainOptions[] = {
    {"SMall", ProcSmall, MK_OVERLAYS},
    {"DIStribute", ProcDistribute, MK_OVERLAYS},
    {"DYNamic", ProcDynamic, MK_OVERLAYS},
    {"STANdard", ProcStandard, MK_OVERLAYS},
    {"NOIndirect", ProcNoIndirect, MK_OVERLAYS},
    {"ARea", ProcArea, MK_OVERLAYS},
    {"PADSections", ProcPadSections, MK_OVERLAYS},
    {"FULLHeader", ProcFullHeader, MK_DOS},
};

// ---- "OVerlay" Directive

bool AddClass() {
    const std::string_view text = reader().text();
    auto* ovlclass = static_cast<list_of_names*>(_PermAlloc(sizeof(list_of_names) + text.size()));
    std::memcpy(ovlclass->name, text.data(), text.size());
    ovlclass->name[text.size()] = '\0';
    ovlclass->next = wlink().OvlClasses;
    wlink().OvlClasses = ovlclass;
    return true;
}

// ---- "Begin" Directive

bool ProcInto() {
    // Process the INTO keyword.
    if (reader().get(Sep::none, Want::file_name)) {
        const std::string_view text = reader().text();
        wlink().CurrSect->outfile = NewOutFile(FileName(text.data(), text.size(), E_OVL, false));
        return true;
    }
    LnkMsg(WRN + LOC + LINE + MSG_DIRECTIVE_ERR, "s", "into");
    return false;
}

constexpr Entry SectOptions[] = {
    {"INto", ProcInto, MK_OVERLAYS},
};

bool ProcSection() {
    // process SECTION command
    if (ovl_level() == 0) {
        LnkMsg(WRN + LOC + LINE + MSG_NO_SECTION_IN_ROOT, nullptr);
    } else {
        MakeNewSection();
        process_one(SectOptions, Sep::none);   // check for INTO
        while (DoParseDirective()) {
            reader().forward();
        }
    }
    return true;
}

bool ProcAutoSection() {
    if (ovl_level() == 0) {
        LnkMsg(WRN + LOC + LINE + MSG_NO_SECTION_IN_ROOT, nullptr);
    } else {
        MakeNewSection();
        process_one(SectOptions, Sep::none);   // check for INTO
        cmd_flags() |= Flag::auto_section | Flag::section_there;
        while (DoParseDirective()) {
        }
        cmd_flags() &= ~Flag::auto_section;
    }
    return true;
}

bool ProcEnd() {
    // process the end of an overlay area
    if (ovl_level() > 0) {   // OvlLevel should always be > 0, but just in case..
        ovl_level()--;
    }
    if (wlink().CurrSect->relocs != 0) {   // this only happens if the user specifies
        wlink().CurrSect->relocs = 0;      // an overlay area with no files in it.
    }
    return false;   // cause loop to be exited in ProcBegin
}

constexpr Entry Sections[] = {
    {"Section", ProcSection, MK_OVERLAYS},
    {"AUTOSection", ProcAutoSection, MK_OVERLAYS},
    {"End", ProcEnd, MK_OVERLAYS},
};

// ---- "NOVector", "VECtor", "FORCEVector" Directives

bool AddNoVector() {
    const std::string_view text = reader().text();
    symbol* sym = SymOp(ST_CREATE_REFERENCE, text.data(), text.size());
    sym->u.d.ovlstate |= (OVL_FORCE | OVL_NO_VECTOR);
    return true;
}

bool AddVector() {
    const std::string_view text = reader().text();
    OvlVectorize(SymOp(ST_CREATE_REFERENCE, text.data(), text.size()));
    return true;
}

bool AddForceVector() {
    const std::string_view text = reader().text();
    symbol* sym = SymOp(ST_CREATE_REFERENCE, text.data(), text.size());
    OvlVectorize(sym);
    sym->u.d.ovlstate |= OVL_ALWAYS;
    return true;
}

// ---- "FORMat" Directive

bool ProcCom() {
    wlink().FmtData.def_ext = E_COM;
    return true;
}

constexpr Entry DosFormats[] = {
    {"COM", ProcCom, MK_COM},
};

}  // namespace

void SetDosFmt() {
    wlink().FmtData.def_ext = E_LOAD;
}

void FreeDosFmt() {
}

overlay_ref GetOvlRef() {
    // OvlSectNum value 0 is reserved for Root
    return (ovl_level() == 0) ? 0 : wlink().OvlSectNum - 1;
}

void CmdOvlFini() {
    if (ovl_level() != 0) {
        Ignite();
        LnkMsg(FTL + LOC + LINE + MSG_EXPECTING_END, nullptr);
    }
    if (wlink().FmtData.u.dos.dynamic && ((wlink().Root->areas == nullptr) || (wlink().Root->areas->next != nullptr))) {
        Ignite();
        LnkMsg(FTL + LOC + LINE + MSG_INCORRECT_NUM_AREAS, nullptr);
    }
    SetOvlClasses();
    MakeNonArea();
}

void MakeNewSection() {
    if (wlink().CurrSect->relocs != SECT_ALREADY_MADE) {
        section* sect = OvlNewSection();
        if (wlink().LinkFlags & LF_ANY_DBI_FLAG) {
            DBISectInit(sect);
        }
        if (any(cmd_flags() & Flag::auto_section)) {
            sect->outfile = wlink().CurrSect->outfile;
        }
        sect->parent = wlink().CurrSect->parent;
        wlink().CurrSect->next = sect;
        wlink().CurrSect = sect;
        state().current_file_list = &sect->files;
    } else {
        wlink().CurrSect->relocs = 0;
    }
}

bool ProcDosOptions() {
    return process_one(MainOptions, Sep::none);
}

bool ProcOverlay() {
    return process_list(AddClass, Want::include_dot);
}

bool ProcFixedLib() {
    cmd_flags() |= Flag::set_section;
    const bool ret = ProcLibrary();
    cmd_flags() &= ~Flag::set_section;
    return ret;
}

bool ProcBegin() {
    // process a new overlay area
    section* oldsect;
    file_list** oldflist;

    wlink().LinkState |= LS_FMT_SPECIFIED;   // she must want DOS mode.
    if ((ovl_level() > 0) && wlink().FmtData.u.dos.dynamic) {
        oldsect = nullptr;
        oldflist = nullptr;
        cmd_flags() &= ~Flag::auto_section;   // merge old area with this.
    } else {
        oldsect = wlink().CurrSect;
        oldflist = state().current_file_list;
        section* sect = OvlNewSection();
        if (wlink().LinkFlags & LF_ANY_DBI_FLAG) {
            DBISectInit(sect);
        }
        NewArea(sect);
        sect->relocs = SECT_ALREADY_MADE;
        wlink().CurrSect = sect;
        state().current_file_list = &sect->files;
    }
    ovl_level()++;
    while (process_one(Sections, Sep::none)) {
    }
    if ((ovl_level() == 0) || !wlink().FmtData.u.dos.dynamic) {
        state().current_file_list = oldflist;
        wlink().CurrSect = oldsect;
    }
    return true;
}

bool ProcNoVector() {
    return process_list(AddNoVector, Want::include_dot);
}

bool ProcVector() {
    return process_list(AddVector, Want::include_dot);
}

bool ProcForceVector() {
    return process_list(AddForceVector, Want::include_dot);
}

bool ProcDosFormat() {
    ovl_level() = 0;
    process_one(DosFormats, Sep::none);
    return true;
}

}  // namespace cb::x86link::directives

#endif
