// build/x86link/directives/dos16m.cpp -- DOS/16M (a port of wlink's cmd16m.c).
#include <cstring>

#include "build/x86link/directives/common.hpp"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state */
#include "build/x86link/directives/formats.hpp"
#include "build/x86link/directives/state.hpp"
#include "build/x86link/directives/cmdline.h"
#include "exe16m.h"
#include "build/x86link/formats/dos16m.h"

#include "clibext.h"

#ifdef _DOS16M

namespace cb::x86link::directives {

namespace {

// ---- "OPtion" Directive

bool ProcBuffer(Link& link) {
    unsigned_32 value;
    if (!get_number(value)) return false;
    if (value < _1K || value > _32K) {
        LnkMsg(link, WRN + LOC + LINE + MSG_VALUE_INCORRECT, "s", "BUFFER");
    } else {
        link.FmtData.u.d16m.buffer = value;
    }
    return true;
}

bool ProcGDTSize(Link& link) {
    unsigned_32 value;
    if (!get_number(value)) return false;
    if ((value % 8) != 0) {
        LnkMsg(link, WRN + LOC + LINE + MSG_NOT_MULTIPLE_OF_8, "s", "GDTSIZE");
        value &= -8;
    }
    if (value > 0x10000) {
        LnkMsg(link, WRN + LOC + LINE + MSG_VALUE_TOO_LARGE, "s", "GDTSIZE");
    } else {
        link.FmtData.u.d16m.gdtsize = --value;
    }
    return true;
}

bool ProcRelocs(Link& link) {
    if (link.FmtData.u.d16m.flags & FORCE_NO_RELOCS) {
        LnkMsg(link, WRN + LOC + LINE + MSG_BOTH_RELOC_OPTIONS, nullptr);
    }
    link.LinkState |= LS_MAKE_RELOCS;
    return true;
}

bool ProcSelStart(Link& link) {
    unsigned_32 value;
    if (!get_number(value)) return false;
    if ((value % 8) != 0) {
        LnkMsg(link, WRN + LOC + LINE + MSG_NOT_MULTIPLE_OF_8, "s", "SELSTART");
        value &= -8;
    }
    if (value >= 0x10000 || value < D16M_USER_SEL) {
        LnkMsg(link, WRN + LOC + LINE + MSG_VALUE_INCORRECT, "s", "SELSTART");
    } else {
        link.FmtData.u.d16m.selstart = value;
    }
    return true;
}

bool ProcExtended(Link& link) {
    unsigned_32 value;
    if (!get_number(value)) return false;
    value >>= 10;   // value should be in K.
    if (value >= 0x10000) {
        LnkMsg(link, WRN + LOC + LINE + MSG_VALUE_TOO_LARGE, "s", "EXTENDED");
    } else {
        link.FmtData.u.d16m.extended = value;
    }
    return true;
}

bool ProcExpName(Link& link) {
    if (!have_equals(Want::file_name)) return false;
    if (link.FmtData.u.d16m.exp_name != nullptr) {
        MemFree(link.FmtData.u.d16m.exp_name);
    }
    const std::string_view text = reader().text();
    link.FmtData.u.d16m.exp_name = FileName(link, text.data(), text.size(), E_PROTECT, true);   // just keep the name around for now.
    return true;
}

bool ProcDataSize(Link& link) {
    unsigned_32 value;
    if (!get_number(value)) return false;
    if (value > _64K) {
        LnkMsg(link, WRN + LOC + LINE + MSG_VALUE_TOO_LARGE, "s", "DATASIZE");
    } else {
        link.FmtData.u.d16m.datasize = __ROUND_UP_SIZE_TO_PARA(value);
        link.FmtData.u.d16m.flags |= DATASIZE_SPECD;
    }
    return true;
}

constexpr Entry MainOptions[] = {
    {"BUFfer", ProcBuffer, MK_DOS16M},
    {"GDTsize", ProcGDTSize, MK_DOS16M},
    {"RELocs", ProcRelocs, MK_DOS16M},
    {"SELstart", ProcSelStart, MK_DOS16M},
    {"DATASize", ProcDataSize, MK_DOS16M},
    {"EXTended", ProcExtended, MK_DOS16M},
    {"EXPName", ProcExpName, MK_DOS16M},
};

// ---- "RUntime" Directive

bool ProcKeyboard(Link& link) {
    link.FmtData.u.d16m.options |= OPT_KEYBOARD;
    return true;
}

bool ProcOverload(Link& link) {
    link.FmtData.u.d16m.options |= OPT_OVERLOAD;
    return true;
}

bool ProcInt10(Link& link) {
    link.FmtData.u.d16m.options |= OPT_INT10;
    return true;
}

bool ProcInit00(Link& link) {
    link.FmtData.u.d16m.options |= OPT_INIT00;
    return true;
}

bool ProcInitFF(Link& link) {
    link.FmtData.u.d16m.options |= OPT_INITFF;
    return true;
}

bool ProcRotate(Link& link) {
    link.FmtData.u.d16m.options |= OPT_ROTATE;
    return true;
}

bool ProcSelectors(Link& link) {
    // force selectors to be assigned at load time.
    link.FmtData.u.d16m.options |= OPT_AUTO;
    return true;
}

bool ProcAuto(Link& link) {
    // force selectors to be assigned at load time, and force relocs as well.
    if (link.FmtData.u.d16m.flags & FORCE_NO_RELOCS) {
        LnkMsg(link, WRN + LOC + LINE + MSG_BOTH_RELOC_OPTIONS, nullptr);
    }
    link.FmtData.u.d16m.options |= OPT_AUTO;
    link.LinkState |= LS_MAKE_RELOCS;
    return true;
}

constexpr Entry RunOptions[] = {
    {"KEYboard", ProcKeyboard, MK_DOS16M},
    {"OVERload", ProcOverload, MK_DOS16M},
    {"INIT00", ProcInit00, MK_DOS16M},
    {"INITFF", ProcInitFF, MK_DOS16M},
    {"ROTate", ProcRotate, MK_DOS16M},
    {"AUTO", ProcAuto, MK_DOS16M},
    {"SELectors", ProcSelectors, MK_DOS16M},
    {"INT10", ProcInt10, MK_DOS16M},
};

// ---- "MEmory" Directive

bool ProcTryExtended(Link& link) {
    link.FmtData.u.d16m.strategy = MPreferExt;
    return true;
}

bool ProcTryLow(Link& link) {
    link.FmtData.u.d16m.strategy = MPreferLow;
    return true;
}

bool ProcForceExtended(Link& link) {
    link.FmtData.u.d16m.strategy = MForceExt;
    return true;
}

bool ProcForceLow(Link& link) {
    link.FmtData.u.d16m.strategy = MForceLow;
    return true;
}

constexpr Entry Strategies[] = {
    {"TRYExtended", ProcTryExtended, MK_DOS16M},
    {"TRYLow", ProcTryLow, MK_DOS16M},
    {"FORCEExtended", ProcForceExtended, MK_DOS16M},
    {"FORCELow", ProcForceLow, MK_DOS16M},
};

// ---- "TRansparent" Directive

bool ProcTStack(Link& link) {
    link.FmtData.u.d16m.flags |= TRANS_STACK;
    return true;
}

bool ProcTData(Link& link) {
    if (link.FmtData.u.d16m.flags & FORCE_NO_RELOCS) {
        LnkMsg(link, WRN + LOC + LINE + MSG_TRANS_RELOCS_NEEDED, nullptr);
    }
    link.FmtData.u.d16m.flags |= TRANS_DATA;
    link.LinkState |= LS_MAKE_RELOCS;
    return true;
}

constexpr Entry TransTypes[] = {
    {"STack", ProcTStack, MK_DOS16M},
    {"DAta", ProcTData, MK_DOS16M},
};

}  // namespace

void SetD16MFmt(Link& link) {
    link.LinkState &= ~LS_MAKE_RELOCS;   // assume none being produced.
    link.FmtData.def_ext = E_PROTECT;
    link.FmtData.u.d16m.options = 0;
    link.FmtData.u.d16m.flags = 0;
    link.FmtData.u.d16m.strategy = MNoStrategy;
    link.FmtData.u.d16m.buffer = 0;
    link.FmtData.u.d16m.gdtsize = 0xFFFF;   // 64 K - 1.
    link.FmtData.u.d16m.selstart = D16M_USER_SEL;
    link.FmtData.u.d16m.extended = 0x7FFF;
    link.FmtData.u.d16m.datasize = 0x1000;
    link.FmtData.u.d16m.exp_name = nullptr;
    link.FmtData.u.d16m.stub = nullptr;
}

void FreeD16MFmt(Link& link) {
    MemFree(link.FmtData.u.d16m.exp_name);
    MemFree(link.FmtData.u.d16m.stub);
}

void CmdD16MFini(Link& link) {
    if (link.FmtData.u.d16m.exp_name == nullptr && exe_name() != nullptr) {
        size_t base_name_len;
        const char* base_name = GetBaseName(exe_name(), strlen(exe_name()), &base_name_len);
        link.FmtData.u.d16m.exp_name = FileName(link, base_name, base_name_len, E_PROTECT, true);
    }
}

bool Proc16MOptions(Link& link) {
    return process_one(link, MainOptions, Sep::none);
}

bool Proc16MNoRelocs(Link& link) {
    if (link.LinkState & LS_MAKE_RELOCS) {
        LnkMsg(link, WRN + LOC + LINE + MSG_BOTH_RELOC_OPTIONS, nullptr);
    } else {
        link.FmtData.u.d16m.flags |= FORCE_NO_RELOCS;
    }
    return true;
}

bool Proc16MRuntime(Link& link) {
    return process_one(link, RunOptions, Sep::none);
}

bool Proc16MMemory(Link& link) {
    return process_one(link, Strategies, Sep::none);
}

bool Proc16MTransparent(Link& link) {
    if (link.FmtData.u.d16m.flags & TRANS_SPECD) {
        LnkMsg(link, WRN + LOC + LINE + MSG_OPTION_MULTIPLY_DEFD, "s", "transparent");
        return true;
    }
    return process_one(link, TransTypes, Sep::none);
}

bool Proc16MFormat(Link& link) {
    return true;
}

}  // namespace cb::x86link::directives

#endif
