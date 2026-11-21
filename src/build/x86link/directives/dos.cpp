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

void SetOvlClasses(Link& link) {
    // make sure the overlay loader is always "overlayed".
    if (link.OvlClasses != nullptr) {
        auto* ovlmgr = static_cast<list_of_names*>(_PermAlloc(sizeof(list_of_names) + OVL_MGR_CL_LEN));
        std::memcpy(ovlmgr->name, OvlMgrClass, OVL_MGR_CL_LEN + 1);
        ovlmgr->next = link.OvlClasses;
        link.OvlClasses = ovlmgr;
    }
}

section* OvlNewSection(Link& link) {
    link.OvlSectNum++;
    return NewSection();
}

void NewArea(Link& link, section* sect) {
    // allocate a new area including this section
    auto* area = static_cast<areasect*>(_PermAlloc(sizeof(areasect)));
    area->next = nullptr;
    area->sections = sect;
    sect->parent = link.CurrSect;
    areasect** owner;
    for (owner = &link.CurrSect->areas; *owner != nullptr; owner = &(*owner)->next) {
    }
    *owner = area;
}

void MakeNonArea(Link& link) {
    // make a new overlay area for non-overlay classes
    link.NonSect = OvlNewSection(link);   // No debug info in nonsect.
    NewArea(link, link.NonSect);
}

// ---- "OPtion" Directive

bool ProcDistribute(Link& link) {
    link.FmtData.u.dos.distribute = true;
    return true;
}

bool ProcPadSections(Link& link) {
    link.FmtData.u.dos.pad_sections = true;
    return true;
}

bool ProcSmall(Link& link) {
    link.FmtData.u.dos.ovl_short = true;
    return true;
}

bool ProcDynamic(Link& link) {
    link.FmtData.u.dos.dynamic = true;
    return true;
}

bool ProcNoIndirect(Link& link) {
    link.FmtData.u.dos.noindirect = true;
    return true;
}

bool ProcFullHeader(Link& link) {
    link.FmtData.u.dos.full_mz_hdr = true;
    return true;
}

bool ProcStandard(Link& link) {
    link.FmtData.u.dos.dynamic = false;
    return true;
}

bool ProcArea(Link& link) {
    // process the area size directive.
    unsigned_32 value;
    const bool ret = get_number(value);
    if (ret) {
        link.OvlAreaSize = (value + link.FmtData.SegMask) >> link.FmtData.SegShift;
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

bool AddClass(Link& link) {
    const std::string_view text = reader().text();
    auto* ovlclass = static_cast<list_of_names*>(_PermAlloc(sizeof(list_of_names) + text.size()));
    std::memcpy(ovlclass->name, text.data(), text.size());
    ovlclass->name[text.size()] = '\0';
    ovlclass->next = link.OvlClasses;
    link.OvlClasses = ovlclass;
    return true;
}

// ---- "Begin" Directive

bool ProcInto(Link& link) {
    // Process the INTO keyword.
    if (reader().get(Sep::none, Want::file_name)) {
        const std::string_view text = reader().text();
        link.CurrSect->outfile = NewOutFile(link, FileName(text.data(), text.size(), E_OVL, false));
        return true;
    }
    LnkMsg(WRN + LOC + LINE + MSG_DIRECTIVE_ERR, "s", "into");
    return false;
}

constexpr Entry SectOptions[] = {
    {"INto", ProcInto, MK_OVERLAYS},
};

bool ProcSection(Link& link) {
    // process SECTION command
    if (ovl_level() == 0) {
        LnkMsg(WRN + LOC + LINE + MSG_NO_SECTION_IN_ROOT, nullptr);
    } else {
        MakeNewSection(link);
        process_one(link, SectOptions, Sep::none);   // check for INTO
        while (DoParseDirective(link)) {
            reader().forward();
        }
    }
    return true;
}

bool ProcAutoSection(Link& link) {
    if (ovl_level() == 0) {
        LnkMsg(WRN + LOC + LINE + MSG_NO_SECTION_IN_ROOT, nullptr);
    } else {
        MakeNewSection(link);
        process_one(link, SectOptions, Sep::none);   // check for INTO
        cmd_flags() |= Flag::auto_section | Flag::section_there;
        while (DoParseDirective(link)) {
        }
        cmd_flags() &= ~Flag::auto_section;
    }
    return true;
}

bool ProcEnd(Link& link) {
    // process the end of an overlay area
    if (ovl_level() > 0) {   // OvlLevel should always be > 0, but just in case..
        ovl_level()--;
    }
    if (link.CurrSect->relocs != 0) {   // this only happens if the user specifies
        link.CurrSect->relocs = 0;      // an overlay area with no files in it.
    }
    return false;   // cause loop to be exited in ProcBegin
}

constexpr Entry Sections[] = {
    {"Section", ProcSection, MK_OVERLAYS},
    {"AUTOSection", ProcAutoSection, MK_OVERLAYS},
    {"End", ProcEnd, MK_OVERLAYS},
};

// ---- "NOVector", "VECtor", "FORCEVector" Directives

bool AddNoVector(Link& link) {
    const std::string_view text = reader().text();
    symbol* sym = SymOp(link, ST_CREATE_REFERENCE, text.data(), text.size());
    sym->u.d.ovlstate |= (OVL_FORCE | OVL_NO_VECTOR);
    return true;
}

bool AddVector(Link& link) {
    const std::string_view text = reader().text();
    OvlVectorize(SymOp(link, ST_CREATE_REFERENCE, text.data(), text.size()));
    return true;
}

bool AddForceVector(Link& link) {
    const std::string_view text = reader().text();
    symbol* sym = SymOp(link, ST_CREATE_REFERENCE, text.data(), text.size());
    OvlVectorize(sym);
    sym->u.d.ovlstate |= OVL_ALWAYS;
    return true;
}

// ---- "FORMat" Directive

bool ProcCom(Link& link) {
    link.FmtData.def_ext = E_COM;
    return true;
}

constexpr Entry DosFormats[] = {
    {"COM", ProcCom, MK_COM},
};

}  // namespace

void SetDosFmt(Link& link) {
    link.FmtData.def_ext = E_LOAD;
}

void FreeDosFmt(Link& link) {
}

overlay_ref GetOvlRef(Link& link) {
    // OvlSectNum value 0 is reserved for Root
    return (ovl_level() == 0) ? 0 : link.OvlSectNum - 1;
}

void CmdOvlFini(Link& link) {
    if (ovl_level() != 0) {
        Ignite(link);
        LnkMsg(FTL + LOC + LINE + MSG_EXPECTING_END, nullptr);
    }
    if (link.FmtData.u.dos.dynamic && ((link.Root->areas == nullptr) || (link.Root->areas->next != nullptr))) {
        Ignite(link);
        LnkMsg(FTL + LOC + LINE + MSG_INCORRECT_NUM_AREAS, nullptr);
    }
    SetOvlClasses(link);
    MakeNonArea(link);
}

void MakeNewSection(Link& link) {
    if (link.CurrSect->relocs != SECT_ALREADY_MADE) {
        section* sect = OvlNewSection(link);
        if (link.LinkFlags & LF_ANY_DBI_FLAG) {
            DBISectInit(link, sect);
        }
        if (any(cmd_flags() & Flag::auto_section)) {
            sect->outfile = link.CurrSect->outfile;
        }
        sect->parent = link.CurrSect->parent;
        link.CurrSect->next = sect;
        link.CurrSect = sect;
        state().current_file_list = &sect->files;
    } else {
        link.CurrSect->relocs = 0;
    }
}

bool ProcDosOptions(Link& link) {
    return process_one(link, MainOptions, Sep::none);
}

bool ProcOverlay(Link& link) {
    return process_list(link, AddClass, Want::include_dot);
}

bool ProcFixedLib(Link& link) {
    cmd_flags() |= Flag::set_section;
    const bool ret = ProcLibrary(link);
    cmd_flags() &= ~Flag::set_section;
    return ret;
}

bool ProcBegin(Link& link) {
    // process a new overlay area
    section* oldsect;
    file_list** oldflist;

    link.LinkState |= LS_FMT_SPECIFIED;   // she must want DOS mode.
    if ((ovl_level() > 0) && link.FmtData.u.dos.dynamic) {
        oldsect = nullptr;
        oldflist = nullptr;
        cmd_flags() &= ~Flag::auto_section;   // merge old area with this.
    } else {
        oldsect = link.CurrSect;
        oldflist = state().current_file_list;
        section* sect = OvlNewSection(link);
        if (link.LinkFlags & LF_ANY_DBI_FLAG) {
            DBISectInit(link, sect);
        }
        NewArea(link, sect);
        sect->relocs = SECT_ALREADY_MADE;
        link.CurrSect = sect;
        state().current_file_list = &sect->files;
    }
    ovl_level()++;
    while (process_one(link, Sections, Sep::none)) {
    }
    if ((ovl_level() == 0) || !link.FmtData.u.dos.dynamic) {
        state().current_file_list = oldflist;
        link.CurrSect = oldsect;
    }
    return true;
}

bool ProcNoVector(Link& link) {
    return process_list(link, AddNoVector, Want::include_dot);
}

bool ProcVector(Link& link) {
    return process_list(link, AddVector, Want::include_dot);
}

bool ProcForceVector(Link& link) {
    return process_list(link, AddForceVector, Want::include_dot);
}

bool ProcDosFormat(Link& link) {
    ovl_level() = 0;
    process_one(link, DosFormats, Sep::none);
    return true;
}

}  // namespace cb::x86link::directives

#endif
