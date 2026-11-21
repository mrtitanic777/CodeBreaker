// build/x86link/directives/phar.cpp -- Phar Lap (`format phar ext|rex|seg`, its options and runtime directives): a port of
// wlink's cmdphar.c.
#include "build/x86link/directives/common.hpp"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include "build/x86link/directives/formats.hpp"
#include "exephar.h"

#ifdef _PHARLAP

namespace cb::x86link::directives {

void SetPharFmt(Link& link) {
    link.LinkState &= ~LS_MAKE_RELOCS;   // do not generate relocations.
    link.FmtData.def_ext = E_PROTECT;
    link.FmtData.u.phar.mindata = 0;
    link.FmtData.u.phar.maxdata = 0xFFFFFFFF;
    link.FmtData.u.phar.breaksym = nullptr;
    link.FmtData.u.phar.stub = nullptr;
    link.FmtData.u.phar.minreal = 0;
    link.FmtData.u.phar.maxreal = 0;
    link.FmtData.u.phar.callbufs = 0;
    link.FmtData.u.phar.realbreak = 0;
    link.FmtData.u.phar.minibuf = 1;
    link.FmtData.u.phar.maxibuf = 4;
    link.FmtData.u.phar.nistack = 4;
    link.FmtData.u.phar.istksize = 1;
    link.FmtData.u.phar.extender_flags = 0;
    link.FmtData.u.phar.unpriv = false;
    ChkBase(link, _4K);
}

void FreePharFmt(Link& link) {
    MemFree(link.FmtData.u.phar.breaksym);
    MemFree(link.FmtData.u.phar.stub);
}

namespace {

// ---- "OPtion" directive

bool ProcMinData(Link& link) { return get_number(link.FmtData.u.phar.mindata); }

bool ProcMaxData(Link& link) { return get_number(link.FmtData.u.phar.maxdata); }

constexpr Entry MainOptions[] = {
    {"MINData", ProcMinData, MK_PHAR_LAP},
    {"MAXData", ProcMaxData, MK_PHAR_LAP},
};

// ---- "RUntime" directive

bool ProcUnpriv(Link& link) {
    link.FmtData.u.phar.unpriv = true;
    return true;
}

bool ProcPriv(Link& link) { return true; }

bool ProcFlags(Link& link) {
    std::uint32_t value = 0;
    bool ret = get_number(value);
    link.FmtData.u.phar.extender_flags = value;
    return ret;
}

bool ProcMinReal(Link& link) {
    std::uint32_t value;
    bool ret = get_number(value);
    if (ret) {
        value >>= link.FmtData.SegShift;   // value specified in paragraphs
        if (value > 0xffff) {
            LnkMsg(WRN + LOC + LINE + MSG_VALUE_TOO_LARGE, "s", "MINREAL");
        } else {
            link.FmtData.u.phar.minreal = value;
        }
    }
    return ret;
}

bool ProcMaxReal(Link& link) {
    std::uint32_t value;
    bool ret = get_number(value);
    if (ret) {
        value >>= link.FmtData.SegShift;   // value specified in paragraphs
        if (value > 0xffff) {
            LnkMsg(WRN + LOC + LINE + MSG_VALUE_TOO_LARGE, "s", "MAXREAL");
        } else {
            link.FmtData.u.phar.maxreal = value;
        }
    }
    return ret;
}

bool ProcRealBreak(Link& link) {
    std::uint32_t value;
    if (!have_equals(Want::include_dot))
        return false;
    Number ok = number(value);
    if (link.FmtData.u.phar.breaksym != nullptr) {
        LnkMsg(WRN + LOC + LINE + MSG_MULT_REALBREAKS, nullptr);
        MemFree(link.FmtData.u.phar.breaksym);
        link.FmtData.u.phar.breaksym = nullptr;
    }
    if (ok != Number::ok) {   // must be a symbol name.
        std::string_view text = reader().text();
        link.FmtData.u.phar.breaksym = MemToStringSafe(link, text.data(), text.size());
    } else {
        link.FmtData.u.phar.realbreak = value;
    }
    return true;
}

bool ProcCallBufs(Link& link) {
    std::uint32_t value;
    bool ret = get_number(value);
    if (ret) {
        value >>= 10;   // value specified in kilobytes
        if (value > 64) {
            LnkMsg(WRN + LOC + LINE + MSG_VALUE_TOO_LARGE, "s", "CALLBUFS");
        } else {
            link.FmtData.u.phar.callbufs = value;
        }
    }
    return ret;
}

bool ProcMiniBuf(Link& link) {
    std::uint32_t value;
    bool ret = get_number(value);
    if (ret) {
        value >>= 10;   // value specified in kilobytes
        if (value > 64 || value < 1) {
            LnkMsg(WRN + LOC + LINE + MSG_VALUE_INCORRECT, "s", "MINIBUF");
        } else {
            link.FmtData.u.phar.minibuf = value;
        }
    }
    return ret;
}

bool ProcMaxiBuf(Link& link) {
    std::uint32_t value;
    bool ret = get_number(value);
    if (ret) {
        value >>= 10;   // value specified in kilobytes
        if (value > 64 || value < 1) {
            LnkMsg(WRN + LOC + LINE + MSG_VALUE_INCORRECT, "s", "MAXIBUF");
        } else {
            link.FmtData.u.phar.maxibuf = value;
        }
    }
    return ret;
}

bool ProcNIStack(Link& link) {
    std::uint32_t value;
    bool ret = get_number(value);
    if (ret) {
        if (value < 4 || value > 0xFFFF) {
            LnkMsg(WRN + LOC + LINE + MSG_VALUE_INCORRECT, "s", "NISTACK");
        } else {
            link.FmtData.u.phar.nistack = value;
        }
    }
    return ret;
}

bool ProcIStkSize(Link& link) {
    std::uint32_t value;
    bool ret = get_number(value);
    if (ret) {
        value >>= 10;   // value specified in kilobytes
        if (value > 64 || value < 1) {
            LnkMsg(WRN + LOC + LINE + MSG_VALUE_INCORRECT, "s", "ISTKSIZE");
        } else {
            link.FmtData.u.phar.istksize = value;
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

bool ProcPharFlat(Link& link) { return true; }

bool ProcRex(Link& link) {
    link.LinkState |= LS_MAKE_RELOCS;   // make relocations;
    link.FmtData.def_ext = E_REX;
    return true;
}

bool ProcPharSegmented(Link& link) {
    link.LinkState |= LS_MAKE_RELOCS;   // make relocations;
    return true;
}

constexpr Entry PharLapFormats[] = {
    {"EXTended", ProcPharFlat, MK_PHAR_FLAT},
    {"REX", ProcRex, MK_PHAR_REX},
    {"SEGmented", ProcPharSegmented, MK_PHAR_MULTISEG},
};

}  // namespace

bool ProcPharOptions(Link& link) { return process_one(link, MainOptions, Sep::none); }

bool ProcPharRuntime(Link& link) { return process_one(link, RunOptions, Sep::none); }

bool ProcPharFormat(Link& link) {
    process_one(link, PharLapFormats, Sep::none);
    return true;
}

}  // namespace cb::x86link::directives

#endif
