// build/x86link/directives/phar.cpp -- Phar Lap (`format phar ext|rex|seg`, its options and runtime directives): a port of
// wlink's cmdphar.c.
#include "build/x86link/directives/common.hpp"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include "build/x86link/directives/formats.hpp"
#include "exephar.h"

#ifdef _PHARLAP

namespace cb::x86link::directives {

void SetPharFmt() {
    wlink().LinkState &= ~LS_MAKE_RELOCS;   // do not generate relocations.
    wlink().FmtData.def_ext = E_PROTECT;
    wlink().FmtData.u.phar.mindata = 0;
    wlink().FmtData.u.phar.maxdata = 0xFFFFFFFF;
    wlink().FmtData.u.phar.breaksym = nullptr;
    wlink().FmtData.u.phar.stub = nullptr;
    wlink().FmtData.u.phar.minreal = 0;
    wlink().FmtData.u.phar.maxreal = 0;
    wlink().FmtData.u.phar.callbufs = 0;
    wlink().FmtData.u.phar.realbreak = 0;
    wlink().FmtData.u.phar.minibuf = 1;
    wlink().FmtData.u.phar.maxibuf = 4;
    wlink().FmtData.u.phar.nistack = 4;
    wlink().FmtData.u.phar.istksize = 1;
    wlink().FmtData.u.phar.extender_flags = 0;
    wlink().FmtData.u.phar.unpriv = false;
    ChkBase(_4K);
}

void FreePharFmt() {
    MemFree(wlink().FmtData.u.phar.breaksym);
    MemFree(wlink().FmtData.u.phar.stub);
}

namespace {

// ---- "OPtion" directive

bool ProcMinData() { return get_number(wlink().FmtData.u.phar.mindata); }

bool ProcMaxData() { return get_number(wlink().FmtData.u.phar.maxdata); }

constexpr Entry MainOptions[] = {
    {"MINData", ProcMinData, MK_PHAR_LAP},
    {"MAXData", ProcMaxData, MK_PHAR_LAP},
};

// ---- "RUntime" directive

bool ProcUnpriv() {
    wlink().FmtData.u.phar.unpriv = true;
    return true;
}

bool ProcPriv() { return true; }

bool ProcFlags() {
    std::uint32_t value = 0;
    bool ret = get_number(value);
    wlink().FmtData.u.phar.extender_flags = value;
    return ret;
}

bool ProcMinReal() {
    std::uint32_t value;
    bool ret = get_number(value);
    if (ret) {
        value >>= wlink().FmtData.SegShift;   // value specified in paragraphs
        if (value > 0xffff) {
            LnkMsg(WRN + LOC + LINE + MSG_VALUE_TOO_LARGE, "s", "MINREAL");
        } else {
            wlink().FmtData.u.phar.minreal = value;
        }
    }
    return ret;
}

bool ProcMaxReal() {
    std::uint32_t value;
    bool ret = get_number(value);
    if (ret) {
        value >>= wlink().FmtData.SegShift;   // value specified in paragraphs
        if (value > 0xffff) {
            LnkMsg(WRN + LOC + LINE + MSG_VALUE_TOO_LARGE, "s", "MAXREAL");
        } else {
            wlink().FmtData.u.phar.maxreal = value;
        }
    }
    return ret;
}

bool ProcRealBreak() {
    std::uint32_t value;
    if (!have_equals(Want::include_dot))
        return false;
    Number ok = number(value);
    if (wlink().FmtData.u.phar.breaksym != nullptr) {
        LnkMsg(WRN + LOC + LINE + MSG_MULT_REALBREAKS, nullptr);
        MemFree(wlink().FmtData.u.phar.breaksym);
        wlink().FmtData.u.phar.breaksym = nullptr;
    }
    if (ok != Number::ok) {   // must be a symbol name.
        std::string_view text = reader().text();
        wlink().FmtData.u.phar.breaksym = MemToStringSafe(text.data(), text.size());
    } else {
        wlink().FmtData.u.phar.realbreak = value;
    }
    return true;
}

bool ProcCallBufs() {
    std::uint32_t value;
    bool ret = get_number(value);
    if (ret) {
        value >>= 10;   // value specified in kilobytes
        if (value > 64) {
            LnkMsg(WRN + LOC + LINE + MSG_VALUE_TOO_LARGE, "s", "CALLBUFS");
        } else {
            wlink().FmtData.u.phar.callbufs = value;
        }
    }
    return ret;
}

bool ProcMiniBuf() {
    std::uint32_t value;
    bool ret = get_number(value);
    if (ret) {
        value >>= 10;   // value specified in kilobytes
        if (value > 64 || value < 1) {
            LnkMsg(WRN + LOC + LINE + MSG_VALUE_INCORRECT, "s", "MINIBUF");
        } else {
            wlink().FmtData.u.phar.minibuf = value;
        }
    }
    return ret;
}

bool ProcMaxiBuf() {
    std::uint32_t value;
    bool ret = get_number(value);
    if (ret) {
        value >>= 10;   // value specified in kilobytes
        if (value > 64 || value < 1) {
            LnkMsg(WRN + LOC + LINE + MSG_VALUE_INCORRECT, "s", "MAXIBUF");
        } else {
            wlink().FmtData.u.phar.maxibuf = value;
        }
    }
    return ret;
}

bool ProcNIStack() {
    std::uint32_t value;
    bool ret = get_number(value);
    if (ret) {
        if (value < 4 || value > 0xFFFF) {
            LnkMsg(WRN + LOC + LINE + MSG_VALUE_INCORRECT, "s", "NISTACK");
        } else {
            wlink().FmtData.u.phar.nistack = value;
        }
    }
    return ret;
}

bool ProcIStkSize() {
    std::uint32_t value;
    bool ret = get_number(value);
    if (ret) {
        value >>= 10;   // value specified in kilobytes
        if (value > 64 || value < 1) {
            LnkMsg(WRN + LOC + LINE + MSG_VALUE_INCORRECT, "s", "ISTKSIZE");
        } else {
            wlink().FmtData.u.phar.istksize = value;
        }
    }
    return ret;
}

constexpr Entry RunOptions[] = {
    {"MINReal", ProcMinReal, MK_PHAR_FLAT},
    {"MAXReal", ProcMaxReal, MK_PHAR_FLAT},
    {"REALBreak", ProcRealBreak, MK_PHAR_FLAT, Flag::have_realbreak},
    {"CALLBufs", ProcCallBufs, MK_PHAR_FLAT},
    {"MINIBuf", ProcMiniBuf, MK_PHAR_FLAT},
    {"MAXIBuf", ProcMaxiBuf, MK_PHAR_FLAT},
    {"NISTack", ProcNIStack, MK_PHAR_FLAT},
    {"ISTKsize", ProcIStkSize, MK_PHAR_FLAT},
    {"UNPRIVileged", ProcUnpriv, MK_PHAR_FLAT},
    {"PRIVileged", ProcPriv, MK_PHAR_FLAT},
    // WARNING: do not document the following directive -- for internal use only
    {"FLAGs", ProcFlags, MK_PHAR_FLAT},
};

// ---- "Format" directive

bool ProcPharFlat() { return true; }

bool ProcRex() {
    wlink().LinkState |= LS_MAKE_RELOCS;   // make relocations;
    wlink().FmtData.def_ext = E_REX;
    return true;
}

bool ProcPharSegmented() {
    wlink().LinkState |= LS_MAKE_RELOCS;   // make relocations;
    return true;
}

constexpr Entry PharLapFormats[] = {
    {"EXTended", ProcPharFlat, MK_PHAR_FLAT},
    {"REX", ProcRex, MK_PHAR_REX},
    {"SEGmented", ProcPharSegmented, MK_PHAR_MULTISEG},
};

}  // namespace

bool ProcPharOptions() { return process_one(MainOptions, Sep::none); }

bool ProcPharRuntime() { return process_one(RunOptions, Sep::none); }

bool ProcPharFormat() {
    process_one(PharLapFormats, Sep::none);
    return true;
}

}  // namespace cb::x86link::directives

#endif
