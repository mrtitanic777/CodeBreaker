// codebreaker/source/pe_writer.cpp -- what is left of the old port of source/pe_writer now that the module lives in native/cb/source/pe_writer.
//
// COMPAT(py): the capstone decode of the old port's `Insn` and the DOS stub finder `_stub` are still read by codebreaker/source/dos_writer.cpp
// and by the ne_writer shim; remove this file when source/dos_writer is refactored (its own port then has no use of either).

#include "codebreaker/source/pe_writer.hpp"

#include <capstone/capstone.h>

#include "codebreaker/base/capstone_dll.hpp"
#include "codebreaker/emulate/capstone_x86.hpp"
#include "codebreaker/source/dos_writer.hpp"

namespace codebreaker::source::pe_writer {

namespace cs = codebreaker::emulate::capstone_x86;

namespace {
bool hex_digit_(char32_t c) {
    return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f');
}
}  // namespace
std::optional<Insn> decode(int64_t bits, const py::bytes& code, int64_t address) {
    const cs::Api& api = cs::api();
    csh handle = cs::handle(bits, true);
    cs_insn* insn = nullptr;
    const std::string& raw = code.raw();
    size_t n = api.disasm(handle, reinterpret_cast<const uint8_t*>(raw.data()), raw.size(), static_cast<uint64_t>(address), 1, &insn);
    if (n == 0) {
        return std::nullopt;
    }
    Insn out;
    out.address = static_cast<int64_t>(insn->address);
    out.size = insn->size;
    out.mnemonic = py::str(std::string(insn->mnemonic));
    out.op_str = py::str(std::string(insn->op_str));
    out.bytes = py::bytes(std::string(reinterpret_cast<const char*>(insn->bytes), insn->size));
    if (insn->detail) {
        const cs_x86& x = insn->detail->x86;
        out.imm_size = x.encoding.imm_size;
        out.imm_offset = x.encoding.imm_offset;
        out.disp_size = x.encoding.disp_size;
        out.disp_offset = x.encoding.disp_offset;
        for (int k = 0; k < 4; ++k) {
            out.prefix.append(x.prefix[k]);
        }
        for (int k = 0; k < insn->detail->groups_count; ++k) {
            out.groups.append(insn->detail->groups[k]);
        }
        out.op_count = x.op_count;
        if (x.op_count > 0) {
            out.op0_type = x.operands[0].type;
            out.op0_imm = x.operands[0].imm;
        }
    }
    api.free_(insn, n);
    return out;
}

bool is_branch(const py::str& m) {
    // ^(j\w+|call|loop\w*|jecxz|jcxz)$  (a mnemonic is all word characters)
    if (m == py::str("call") || m.startswith(py::str("loop"))) {
        return true;
    }
    return py::len(m) >= 2 && m.startswith(py::str("j"));
}

std::optional<int64_t> branch_target(const Insn& i) {
    if (is_branch(i.mnemonic) && i.imm_size != 0) {
        // ^0x[0-9a-f]+$
        const py::str& s = i.op_str;
        if (py::len(s) > 2 && s.startswith(py::str("0x"))) {
            bool ok = true;
            for (int64_t k = 2; k < py::len(s); ++k) {
                if (!hex_digit_(s.at_(k))) {
                    ok = false;
                    break;
                }
            }
            if (ok) {
                return py::int_parse(s, 16);
            }
        }
    }
    return std::nullopt;
}

std::optional<py::str> _stub(const py::bytes& d, int64_t e_lfanew, const py::str& outdir, const std::function<void(const py::str&)>& say) {
    auto [last, pages] = py::unpack_from<int64_t, int64_t>("<HH", d, 2);
    int64_t size = pages ? (pages - 1) * 512 + (last ? last : 512) : 0;
    if (!(0x80 < size && size <= e_lfanew)) {
        return std::nullopt;
    }
    py::str sd = py::os::path::join(outdir, py::str("stub"));
    py::os::makedirs(sd, 0777, true);
    py::str carved = py::os::path::join(sd, py::str("original"), py::str("STUB.EXE"));
    py::os::makedirs(py::os::path::dirname(carved), 0777, true);
    {
        py::file f = py::open(carved, py::str("wb"));
        f.write(d.slice(0, size));
        f.close();
    }
    dos_writer::Result r;
    try {
        r = dos_writer::reconstruct(carved, sd);
    } catch (const py::Exception& e) {   // (a stub the DOS lane cannot take: its bytes)
        say(py::str("the stub: ") + py::str(e.what()));
        return std::nullopt;
    }
    if (r.file.has_value() && py::truthy(*r.file) && r.file_same.value_or(false)) {
        say(py::percent("the stub: %d bytes, from its own source (%s)", size, r.source_text()));
        return py::os::path::abspath(*r.file);
    }
    say(py::str("the stub does not come back from its source; its bytes are kept"));
    return std::nullopt;
}

}  // namespace codebreaker::source::pe_writer
