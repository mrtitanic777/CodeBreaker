// build/x86link/directives/rdos.cpp -- RDOS (`format rdos dev|bin|mboot`): a port of wlink's cmdrdv.c.
#include "build/x86link/directives/common.hpp"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include "build/x86link/directives/formats.hpp"

#ifdef _RDOS

namespace cb::x86link::directives {

// set up the structures needed to be able to process something in RDOS mode.
void SetRdosFmt() {
    Link& link = wlink();
    if (link.LinkState & LS_FMT_INITIALIZED)
        return;
    link.LinkState |= LS_FMT_INITIALIZED;
    link.FmtData.u.rdos.code_seg = 0;
    link.FmtData.u.rdos.data_seg = 0;
    link.FmtData.u.rdos.code_sel = 0;
    link.FmtData.u.rdos.data_sel = 0;
    link.FmtData.u.rdos.driver = 0;
    link.FmtData.u.rdos.mboot = 0;
}

void FreeRdosFmt() {}

namespace {

// ---- "OPtion" directive

bool ProcRdosCodeSel(Link& link) { return get_number(link.FmtData.u.rdos.code_sel); }

bool ProcRdosDataSel(Link& link) { return get_number(link.FmtData.u.rdos.data_sel); }

constexpr Entry MainOptions[] = {
    {"CODESelector", ProcRdosCodeSel, MK_RDOS},
    {"DATASelector", ProcRdosDataSel, MK_RDOS},
};

// ---- "Format" directive

bool ProcRdos16(Link& link) { return true; }

constexpr Entry RdosSubFormats[] = {
    {"16", ProcRdos16, MK_RDOS_16},
};

bool ProcRdosDev(Link& link) {
    if (!process_one(link, RdosSubFormats, Sep::none)) {
        HintFormat(link, MK_RDOS_32);   // set to 32-bit RDOS mode
    }
    link.FmtData.def_ext = E_RDV;
    link.FmtData.u.rdos.driver = 1;
    link.FmtData.u.rdos.mboot = 0;
    return true;
}

bool ProcRdosBin(Link& link) {
    if (!process_one(link, RdosSubFormats, Sep::none)) {
        HintFormat(link, MK_RDOS_32);   // set to 32-bit RDOS mode
    }
    link.FmtData.def_ext = E_BIN;
    link.FmtData.u.rdos.driver = 0;
    link.FmtData.u.rdos.mboot = 0;
    return true;
}

bool ProcRdosMboot(Link& link) {
    link.FmtData.def_ext = E_BIN;
    link.FmtData.u.rdos.driver = 0;
    link.FmtData.u.rdos.mboot = 1;
    return true;
}

constexpr Entry RdosFormats[] = {
    {"DEV", ProcRdosDev, MK_RDOS},
    {"BIN", ProcRdosBin, MK_RDOS},
    {"MBOOT", ProcRdosMboot, MK_RDOS_16},
};

}  // namespace

bool ProcRdosOptions(Link& link) { return process_one(link, MainOptions, Sep::none); }

bool ProcRdosFormat(Link& link) {
    link.LinkState |= LS_MAKE_RELOCS | LS_FMT_DECIDED;   // make relocations;
    process_one(link, RdosFormats, Sep::none);
    return true;
}

}  // namespace cb::x86link::directives

#endif
