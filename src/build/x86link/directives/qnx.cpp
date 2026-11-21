// build/x86link/directives/qnx.cpp -- QNX (`format qnx [flat]`, its options and `segment` directive): a port of wlink's
// cmdqnx.c.
#include "build/x86link/directives/common.hpp"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state, wlink() */
#include "build/x86link/directives/formats.hpp"
#include "exeqnx.h"
#include "build/x86link/formats/qnx.h"
#include "build/x86link/objects/objfree.h"

#ifdef _QNX

namespace cb::x86link::directives {

void SetQNXFmt() {
    wlink().FmtData.def_ext = E_QNX;
    wlink().FmtData.u.qnx.flags = 0;
    wlink().FmtData.u.qnx.priv_level = 0;
    wlink().FmtData.u.qnx.seg_flags = nullptr;
    wlink().FmtData.u.qnx.heapsize = _4K;
    wlink().FmtData.u.qnx.gen_seg_relocs = true;
    wlink().FmtData.u.qnx.gen_linear_relocs = false;
    ChkBase(_4K);
}

void FreeQNXFmt() { FreeSegFlags(wlink().FmtData.u.qnx.seg_flags); }

void CmdQNXFini() {
    if (!wlink().FmtData.u.qnx.gen_linear_relocs && !wlink().FmtData.u.qnx.gen_seg_relocs) {
        wlink().LinkState &= ~LS_MAKE_RELOCS;
    }
}

namespace {

// ---- "OPtion" directive

bool ProcLongLived() {
    wlink().FmtData.u.qnx.flags |= _TCF_LONG_LIVED;
    return true;
}

bool ProcPrivilege() {
    std::uint16_t value = 0;
    if (!have_equals(Want::normal))
        return false;
    Number ret = number16(value);
    if (ret != Number::ok || value > 3) {
        LnkMsg(WRN + LOC + LINE + MSG_VALUE_INCORRECT, "s", "PRIVILEGE");
    } else {
        wlink().FmtData.u.qnx.priv_level = value;
    }
    return true;
}

bool ProcLinearRelocs() {
    wlink().FmtData.u.qnx.gen_linear_relocs = true;
    return true;
}

constexpr Entry MainOptions[] = {
    {"LOnglived", ProcLongLived, MK_QNX},
    {"PRIVilege", ProcPrivilege, MK_QNX},
    {"LInearrelocs", ProcLinearRelocs, MK_QNX},
};

// ---- "SEGment" directive

// All processing done for this already.
bool ProcQNXClass() { return true; }

bool ProcQNXExecuteonly() {
    wlink().FmtData.u.qnx.seg_flags->flags = (seg_flags_type)(QNX_EXEC_ONLY);
    return true;
}

bool ProcQNXExecuteread() {
    wlink().FmtData.u.qnx.seg_flags->flags = (seg_flags_type)(QNX_EXEC_READ);
    return true;
}

bool ProcQNXReadOnly() {
    wlink().FmtData.u.qnx.seg_flags->flags = (seg_flags_type)(QNX_READ_ONLY);
    return true;
}

bool ProcQNXReadWrite() {
    wlink().FmtData.u.qnx.seg_flags->flags = (seg_flags_type)(QNX_READ_WRITE);
    return true;
}

constexpr Entry QNXSegDesc[] = {
    {"Class", ProcQNXClass, MK_QNX},
};

constexpr Entry QNXSegModel[] = {
    {"EXECUTEOnly", ProcQNXExecuteonly, MK_QNX},
    {"EXECUTERead", ProcQNXExecuteread, MK_QNX},
    {"READOnly", ProcQNXReadOnly, MK_QNX},
    {"READWrite", ProcQNXReadWrite, MK_QNX},
};

bool getSegFlags() {
    reader().push_back();
    bool isclass = process_one(QNXSegDesc, Sep::none);
    if (!reader().get(Sep::none, Want::include_dot)) {
        return false;
    }
    seg_flags_struct* seg_flags = (seg_flags_struct*)(MemAllocSafe(sizeof(seg_flags_struct)));
    seg_flags->specified = SEG_NONE;
    seg_flags->flags = SEG_NONE;
    std::string_view text = reader().text();
    seg_flags->name = MemToStringSafe(text.data(), text.size());
    seg_flags->type = (isclass) ? SEGFLAG_CLASS : SEGFLAG_SEGMENT;
    seg_flags->next = wlink().FmtData.u.qnx.seg_flags;
    wlink().FmtData.u.qnx.seg_flags = seg_flags;
    return process_one(QNXSegModel, Sep::none);
}

// ---- "Format" directive

bool ProcQNXFlat() { return true; }

constexpr Entry QNXFormats[] = {
    {"FLat", ProcQNXFlat, MK_QNX_FLAT},
};

}  // namespace

bool ProcQNXOptions() { return process_one(MainOptions, Sep::none); }

bool ProcQNXNoRelocs() {
    wlink().FmtData.u.qnx.gen_seg_relocs = false;
    return true;
}

bool ProcQNXHeapSize() {
    std::uint32_t value = 0;
    if (!have_equals(Want::normal))
        return false;
    Number ret = number(value);
    if (ret != Number::ok || value == 0) {
        LnkMsg(WRN + LOC + LINE + MSG_VALUE_INCORRECT, "s", "HEAPSIZE");
    } else {
        wlink().FmtData.u.qnx.heapsize = value;
    }
    return true;
}

bool ProcQNXSegment() { return process_list(getSegFlags, Want::include_dot); }

bool ProcQNXFormat() {
    if (!process_one(QNXFormats, Sep::none)) {
        HintFormat(MK_QNX_16);   // set to 16-bit qnx mode
    }
    return true;
}

}  // namespace cb::x86link::directives

#endif
