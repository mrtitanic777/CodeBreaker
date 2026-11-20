// build/x86link/directives/dos16m.cpp -- DOS/16M (a port of wlink's cmd16m.c).
#include <cstring>

#include "build/x86link/directives/common.hpp"
#include "build/x86link/directives/formats.hpp"
#include "build/x86link/directives/state.hpp"
#include "cmdline.h"
#include "exe16m.h"
#include "load16m.h"

#include "clibext.h"

#ifdef _DOS16M

namespace cb::x86link::directives {

namespace {

// ---- "OPtion" Directive

bool ProcBuffer() {
    unsigned_32 value;
    if (!get_number(value)) return false;
    if (value < _1K || value > _32K) {
        LnkMsg(WRN + LOC + LINE + MSG_VALUE_INCORRECT, "s", "BUFFER");
    } else {
        FmtData.u.d16m.buffer = value;
    }
    return true;
}

bool ProcGDTSize() {
    unsigned_32 value;
    if (!get_number(value)) return false;
    if ((value % 8) != 0) {
        LnkMsg(WRN + LOC + LINE + MSG_NOT_MULTIPLE_OF_8, "s", "GDTSIZE");
        value &= -8;
    }
    if (value > 0x10000) {
        LnkMsg(WRN + LOC + LINE + MSG_VALUE_TOO_LARGE, "s", "GDTSIZE");
    } else {
        FmtData.u.d16m.gdtsize = --value;
    }
    return true;
}

bool ProcRelocs() {
    if (FmtData.u.d16m.flags & FORCE_NO_RELOCS) {
        LnkMsg(WRN + LOC + LINE + MSG_BOTH_RELOC_OPTIONS, nullptr);
    }
    LinkState |= LS_MAKE_RELOCS;
    return true;
}

bool ProcSelStart() {
    unsigned_32 value;
    if (!get_number(value)) return false;
    if ((value % 8) != 0) {
        LnkMsg(WRN + LOC + LINE + MSG_NOT_MULTIPLE_OF_8, "s", "SELSTART");
        value &= -8;
    }
    if (value >= 0x10000 || value < D16M_USER_SEL) {
        LnkMsg(WRN + LOC + LINE + MSG_VALUE_INCORRECT, "s", "SELSTART");
    } else {
        FmtData.u.d16m.selstart = value;
    }
    return true;
}

bool ProcExtended() {
    unsigned_32 value;
    if (!get_number(value)) return false;
    value >>= 10;   // value should be in K.
    if (value >= 0x10000) {
        LnkMsg(WRN + LOC + LINE + MSG_VALUE_TOO_LARGE, "s", "EXTENDED");
    } else {
        FmtData.u.d16m.extended = value;
    }
    return true;
}

bool ProcExpName() {
    if (!have_equals(Want::file_name)) return false;
    if (FmtData.u.d16m.exp_name != nullptr) {
        MemFree(FmtData.u.d16m.exp_name);
    }
    const std::string_view text = reader().text();
    FmtData.u.d16m.exp_name = FileName(text.data(), text.size(), E_PROTECT, true);   // just keep the name around for now.
    return true;
}

bool ProcDataSize() {
    unsigned_32 value;
    if (!get_number(value)) return false;
    if (value > _64K) {
        LnkMsg(WRN + LOC + LINE + MSG_VALUE_TOO_LARGE, "s", "DATASIZE");
    } else {
        FmtData.u.d16m.datasize = __ROUND_UP_SIZE_TO_PARA(value);
        FmtData.u.d16m.flags |= DATASIZE_SPECD;
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

bool ProcKeyboard() {
    FmtData.u.d16m.options |= OPT_KEYBOARD;
    return true;
}

bool ProcOverload() {
    FmtData.u.d16m.options |= OPT_OVERLOAD;
    return true;
}

bool ProcInt10() {
    FmtData.u.d16m.options |= OPT_INT10;
    return true;
}

bool ProcInit00() {
    FmtData.u.d16m.options |= OPT_INIT00;
    return true;
}

bool ProcInitFF() {
    FmtData.u.d16m.options |= OPT_INITFF;
    return true;
}

bool ProcRotate() {
    FmtData.u.d16m.options |= OPT_ROTATE;
    return true;
}

bool ProcSelectors() {
    // force selectors to be assigned at load time.
    FmtData.u.d16m.options |= OPT_AUTO;
    return true;
}

bool ProcAuto() {
    // force selectors to be assigned at load time, and force relocs as well.
    if (FmtData.u.d16m.flags & FORCE_NO_RELOCS) {
        LnkMsg(WRN + LOC + LINE + MSG_BOTH_RELOC_OPTIONS, nullptr);
    }
    FmtData.u.d16m.options |= OPT_AUTO;
    LinkState |= LS_MAKE_RELOCS;
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

bool ProcTryExtended() {
    FmtData.u.d16m.strategy = MPreferExt;
    return true;
}

bool ProcTryLow() {
    FmtData.u.d16m.strategy = MPreferLow;
    return true;
}

bool ProcForceExtended() {
    FmtData.u.d16m.strategy = MForceExt;
    return true;
}

bool ProcForceLow() {
    FmtData.u.d16m.strategy = MForceLow;
    return true;
}

constexpr Entry Strategies[] = {
    {"TRYExtended", ProcTryExtended, MK_DOS16M},
    {"TRYLow", ProcTryLow, MK_DOS16M},
    {"FORCEExtended", ProcForceExtended, MK_DOS16M},
    {"FORCELow", ProcForceLow, MK_DOS16M},
};

// ---- "TRansparent" Directive

bool ProcTStack() {
    FmtData.u.d16m.flags |= TRANS_STACK;
    return true;
}

bool ProcTData() {
    if (FmtData.u.d16m.flags & FORCE_NO_RELOCS) {
        LnkMsg(WRN + LOC + LINE + MSG_TRANS_RELOCS_NEEDED, nullptr);
    }
    FmtData.u.d16m.flags |= TRANS_DATA;
    LinkState |= LS_MAKE_RELOCS;
    return true;
}

constexpr Entry TransTypes[] = {
    {"STack", ProcTStack, MK_DOS16M},
    {"DAta", ProcTData, MK_DOS16M},
};

}  // namespace

void SetD16MFmt() {
    LinkState &= ~LS_MAKE_RELOCS;   // assume none being produced.
    FmtData.def_ext = E_PROTECT;
    FmtData.u.d16m.options = 0;
    FmtData.u.d16m.flags = 0;
    FmtData.u.d16m.strategy = MNoStrategy;
    FmtData.u.d16m.buffer = 0;
    FmtData.u.d16m.gdtsize = 0xFFFF;   // 64 K - 1.
    FmtData.u.d16m.selstart = D16M_USER_SEL;
    FmtData.u.d16m.extended = 0x7FFF;
    FmtData.u.d16m.datasize = 0x1000;
    FmtData.u.d16m.exp_name = nullptr;
    FmtData.u.d16m.stub = nullptr;
}

void FreeD16MFmt() {
    MemFree(FmtData.u.d16m.exp_name);
    MemFree(FmtData.u.d16m.stub);
}

void CmdD16MFini() {
    if (FmtData.u.d16m.exp_name == nullptr && exe_name() != nullptr) {
        size_t base_name_len;
        const char* base_name = GetBaseName(exe_name(), strlen(exe_name()), &base_name_len);
        FmtData.u.d16m.exp_name = FileName(base_name, base_name_len, E_PROTECT, true);
    }
}

bool Proc16MOptions() {
    return process_one(MainOptions, Sep::none);
}

bool Proc16MNoRelocs() {
    if (LinkState & LS_MAKE_RELOCS) {
        LnkMsg(WRN + LOC + LINE + MSG_BOTH_RELOC_OPTIONS, nullptr);
    } else {
        FmtData.u.d16m.flags |= FORCE_NO_RELOCS;
    }
    return true;
}

bool Proc16MRuntime() {
    return process_one(RunOptions, Sep::none);
}

bool Proc16MMemory() {
    return process_one(Strategies, Sep::none);
}

bool Proc16MTransparent() {
    if (FmtData.u.d16m.flags & TRANS_SPECD) {
        LnkMsg(WRN + LOC + LINE + MSG_OPTION_MULTIPLY_DEFD, "s", "transparent");
        return true;
    }
    return process_one(TransTypes, Sep::none);
}

bool Proc16MFormat() {
    return true;
}

}  // namespace cb::x86link::directives

#endif
