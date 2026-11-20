// build/x86link/directives/rdos.cpp -- RDOS (`format rdos dev|bin|mboot`): a port of wlink's cmdrdv.c.
#include "build/x86link/directives/common.hpp"
#include "build/x86link/directives/formats.hpp"

#ifdef _RDOS

namespace cb::x86link::directives {

// set up the structures needed to be able to process something in RDOS mode.
void SetRdosFmt() {
    if (LinkState & LS_FMT_INITIALIZED)
        return;
    LinkState |= LS_FMT_INITIALIZED;
    FmtData.u.rdos.code_seg = 0;
    FmtData.u.rdos.data_seg = 0;
    FmtData.u.rdos.code_sel = 0;
    FmtData.u.rdos.data_sel = 0;
    FmtData.u.rdos.driver = 0;
    FmtData.u.rdos.mboot = 0;
}

void FreeRdosFmt() {}

namespace {

// ---- "OPtion" directive

bool ProcRdosCodeSel() { return get_number(FmtData.u.rdos.code_sel); }

bool ProcRdosDataSel() { return get_number(FmtData.u.rdos.data_sel); }

constexpr Entry MainOptions[] = {
    {"CODESelector", ProcRdosCodeSel, MK_RDOS},
    {"DATASelector", ProcRdosDataSel, MK_RDOS},
};

// ---- "Format" directive

bool ProcRdos16() { return true; }

constexpr Entry RdosSubFormats[] = {
    {"16", ProcRdos16, MK_RDOS_16},
};

bool ProcRdosDev() {
    if (!process_one(RdosSubFormats, Sep::none)) {
        HintFormat(MK_RDOS_32);   // set to 32-bit RDOS mode
    }
    FmtData.def_ext = E_RDV;
    FmtData.u.rdos.driver = 1;
    FmtData.u.rdos.mboot = 0;
    return true;
}

bool ProcRdosBin() {
    if (!process_one(RdosSubFormats, Sep::none)) {
        HintFormat(MK_RDOS_32);   // set to 32-bit RDOS mode
    }
    FmtData.def_ext = E_BIN;
    FmtData.u.rdos.driver = 0;
    FmtData.u.rdos.mboot = 0;
    return true;
}

bool ProcRdosMboot() {
    FmtData.def_ext = E_BIN;
    FmtData.u.rdos.driver = 0;
    FmtData.u.rdos.mboot = 1;
    return true;
}

constexpr Entry RdosFormats[] = {
    {"DEV", ProcRdosDev, MK_RDOS},
    {"BIN", ProcRdosBin, MK_RDOS},
    {"MBOOT", ProcRdosMboot, MK_RDOS_16},
};

}  // namespace

bool ProcRdosOptions() { return process_one(MainOptions, Sep::none); }

bool ProcRdosFormat() {
    LinkState |= LS_MAKE_RELOCS | LS_FMT_DECIDED;   // make relocations;
    process_one(RdosFormats, Sep::none);
    return true;
}

}  // namespace cb::x86link::directives

#endif
