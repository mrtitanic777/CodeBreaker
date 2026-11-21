// build/x86link/directives/qnx.cpp -- QNX (`format qnx [flat]`, its options and `segment` directive): a port of wlink's
// cmdqnx.c.
#include "build/x86link/directives/common.hpp"
#include "build/x86link/link/link.hpp"   /* CodeBreaker: the link's state */
#include "build/x86link/directives/formats.hpp"
#include "exeqnx.h"
#include "build/x86link/formats/qnx.h"
#include "build/x86link/objects/objfree.h"

#ifdef _QNX

namespace cb::x86link::directives {

void SetQNXFmt(Link& link) {
    link.FmtData.def_ext = E_QNX;
    link.FmtData.u.qnx.flags = 0;
    link.FmtData.u.qnx.priv_level = 0;
    link.FmtData.u.qnx.seg_flags = nullptr;
    link.FmtData.u.qnx.heapsize = _4K;
    link.FmtData.u.qnx.gen_seg_relocs = true;
    link.FmtData.u.qnx.gen_linear_relocs = false;
    ChkBase(link, _4K);
}

void FreeQNXFmt(Link& link) { FreeSegFlags(link.FmtData.u.qnx.seg_flags); }

void CmdQNXFini(Link& link) {
    if (!link.FmtData.u.qnx.gen_linear_relocs && !link.FmtData.u.qnx.gen_seg_relocs) {
        link.LinkState &= ~LS_MAKE_RELOCS;
    }
}

namespace {

// ---- "OPtion" directive

bool ProcLongLived(Link& link) {
    link.FmtData.u.qnx.flags |= _TCF_LONG_LIVED;
    return true;
}

bool ProcPrivilege(Link& link) {
    std::uint16_t value = 0;
    if (!have_equals(Want::normal))
        return false;
    Number ret = number16(value);
    if (ret != Number::ok || value > 3) {
        LnkMsg(link, WRN + LOC + LINE + MSG_VALUE_INCORRECT, "s", "PRIVILEGE");
    } else {
        link.FmtData.u.qnx.priv_level = value;
    }
    return true;
}

bool ProcLinearRelocs(Link& link) {
    link.FmtData.u.qnx.gen_linear_relocs = true;
    return true;
}

constexpr Entry MainOptions[] = {
    {"LOnglived", ProcLongLived, MK_QNX},
    {"PRIVilege", ProcPrivilege, MK_QNX},
    {"LInearrelocs", ProcLinearRelocs, MK_QNX},
};

// ---- "SEGment" directive

// All processing done for this already.
bool ProcQNXClass(Link& link) { return true; }

bool ProcQNXExecuteonly(Link& link) {
    link.FmtData.u.qnx.seg_flags->flags = (seg_flags_type)(QNX_EXEC_ONLY);
    return true;
}

bool ProcQNXExecuteread(Link& link) {
    link.FmtData.u.qnx.seg_flags->flags = (seg_flags_type)(QNX_EXEC_READ);
    return true;
}

bool ProcQNXReadOnly(Link& link) {
    link.FmtData.u.qnx.seg_flags->flags = (seg_flags_type)(QNX_READ_ONLY);
    return true;
}

bool ProcQNXReadWrite(Link& link) {
    link.FmtData.u.qnx.seg_flags->flags = (seg_flags_type)(QNX_READ_WRITE);
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

bool getSegFlags(Link& link) {
    reader().push_back();
    bool isclass = process_one(link, QNXSegDesc, Sep::none);
    if (!reader().get(Sep::none, Want::include_dot)) {
        return false;
    }
    seg_flags_struct* seg_flags = (seg_flags_struct*)(MemAllocSafe(link, sizeof(seg_flags_struct)));
    seg_flags->specified = SEG_NONE;
    seg_flags->flags = SEG_NONE;
    std::string_view text = reader().text();
    seg_flags->name = MemToStringSafe(link, text.data(), text.size());
    seg_flags->type = (isclass) ? SEGFLAG_CLASS : SEGFLAG_SEGMENT;
    seg_flags->next = link.FmtData.u.qnx.seg_flags;
    link.FmtData.u.qnx.seg_flags = seg_flags;
    return process_one(link, QNXSegModel, Sep::none);
}

// ---- "Format" directive

bool ProcQNXFlat(Link& link) { return true; }

constexpr Entry QNXFormats[] = {
    {"FLat", ProcQNXFlat, MK_QNX_FLAT},
};

}  // namespace

bool ProcQNXOptions(Link& link) { return process_one(link, MainOptions, Sep::none); }

bool ProcQNXNoRelocs(Link& link) {
    link.FmtData.u.qnx.gen_seg_relocs = false;
    return true;
}

bool ProcQNXHeapSize(Link& link) {
    std::uint32_t value = 0;
    if (!have_equals(Want::normal))
        return false;
    Number ret = number(value);
    if (ret != Number::ok || value == 0) {
        LnkMsg(link, WRN + LOC + LINE + MSG_VALUE_INCORRECT, "s", "HEAPSIZE");
    } else {
        link.FmtData.u.qnx.heapsize = value;
    }
    return true;
}

bool ProcQNXSegment(Link& link) { return process_list(link, getSegFlags, Want::include_dot); }

bool ProcQNXFormat(Link& link) {
    if (!process_one(link, QNXFormats, Sep::none)) {
        HintFormat(link, MK_QNX_16);   // set to 16-bit qnx mode
    }
    return true;
}

}  // namespace cb::x86link::directives

#endif
