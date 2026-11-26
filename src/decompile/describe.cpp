// decompile/describe.cpp -- the 16-bit decoder, the Description records, the address spaces (see describe.hpp).
// cb-unity: alone

#include "decompile/describe.hpp"

#include <cstdlib>

#include "core/capstone_api.hpp"
#include "core/fs.hpp"
#include "core/intmath.hpp"
#include "decompile/describe_internal.hpp"
#include "platforms/decoder.hpp"
#include "symbols/prototypes.hpp"

namespace cb::decompile::describe {

std::optional<std::string_view> language_of_machine(std::uint16_t machine) {
    if (machine == formats::pe::kMachineI386) return kArchLe;
    if (machine == formats::pe::kMachineX64) return "x86:LE:64:default:windows";   // (Visual C++'s x64 convention: x86-64-win.cspec)
    if (machine == 0x01F2) return "PowerPC:BE:64:Xenon:homed";   // the Xbox 360's
    return std::nullopt;
}

std::optional<Declared> declared_of(const symbols::prototypes::Prototype* prototype) {
    if (prototype == nullptr) return std::nullopt;
    Declared out;
    out.returns = prototype->returns;
    for (const symbols::prototypes::Parameter& p : prototype->parameters) {
        if (p.type == "...") {
            out.varargs = true;
            break;
        }
        out.parameters.emplace_back(p.type, p.name);
    }
    return out;
}

SegOff where(Va va) {
    return SegOff{static_cast<std::uint32_t>(floor_div<Va>((va >> 16) - kFirstSelector, 8) + 1), static_cast<std::uint32_t>(va & 0xFFFF)};
}

Policy Policy::from_environment() {
    auto is = [](const char* name, std::string_view value) {
        auto v = fs::get_env(name);
        return v && *v == value;
    };
    Policy p;
    p.caller_args = !is("CB_CALLER_ARGS", "0");
    p.caller_results = !is("CB_CALLER_RESULTS", "0");
    p.dx_shared_bodies = !is("CB_DX_SHARED", "0");
    p.dx_unspecified = !is("CB_DX_UNSPECIFIED", "0");
    p.fastcall = !is("CB_FASTCALL", "0");
    p.inline_helpers = !is("CB_INLINE_HELPERS", "0");
    auto no_overrides = fs::get_env("CB_NO_CALL_OVERRIDES");
    p.call_overrides = !no_overrides || no_overrides->empty();
    return p;
}

std::string Knowledge::driver_service_label(std::uint32_t device, std::uint32_t service) {
    return std::format("vxd_{:x}_{:x}", device, service);
}

// ---- the decoder ------------------------------------------------------------------------------------------------------
namespace {

// The Capstone engines this thread has made (an engine is not shared between threads).
const platforms::Decoder& detail_decoder16(bool detail) {
    thread_local platforms::DecoderSet set;
    return set.get(platforms::Arch::X86, 16, detail);
}

Insn insn_of(const platforms::Decoder& md, const cs_insn& c, bool detail) {
    Insn out;
    out.address = static_cast<std::int64_t>(c.address);
    out.size = c.size;
    out.mn = c.mnemonic;
    out.op_str = c.op_str;
    out.bytes.assign(c.bytes, c.bytes + c.size);
    if (!detail || !c.detail) return out;
    const cs_x86& x = c.detail->x86;
    for (int k = 0; k < x.op_count; ++k) {
        const cs_x86_op& o = x.operands[k];
        Operand op;
        op.kind = o.type == X86_OP_REG ? OperandKind::Register
                  : o.type == X86_OP_IMM ? OperandKind::Immediate
                  : o.type == X86_OP_MEM ? OperandKind::Memory
                                         : OperandKind::Invalid;
        op.reg = static_cast<std::uint32_t>(o.reg);
        op.imm = o.imm;
        op.segment = static_cast<std::uint32_t>(o.mem.segment);
        op.base = static_cast<std::uint32_t>(o.mem.base);
        op.index = static_cast<std::uint32_t>(o.mem.index);
        op.disp = o.mem.disp;
        op.size = o.size;
        out.operands.push_back(op);
    }
    for (int k = 0; k < c.detail->groups_count; ++k)
        if (c.detail->groups[k] < 32) out.groups |= 1u << c.detail->groups[k];
    if (auto access = md.register_access(c)) {
        out.regs_ok = true;
        out.regs_read = std::move(access->read);
        out.regs_write = std::move(access->written);
    }
    return out;
}

}  // namespace

std::vector<Insn> decode16(ByteView code, std::int64_t at, bool detail, std::size_t count) {
    const platforms::Decoder& md = detail_decoder16(detail);
    std::vector<Insn> out;
    md.each(code, static_cast<std::uint64_t>(at), count, [&](const cs_insn& c) {
        out.push_back(insn_of(md, c, detail));
        return true;
    });
    return out;
}

std::string register_name16(std::uint32_t reg) {
    const char* s = detail_decoder16(true).register_name(reg);
    return s ? s : "";
}

const Insn* Code16::one(std::int64_t at) const {
    auto it = seen_.find(at);
    if (it != seen_.end()) return it->second.get();
    std::vector<Insn> got = decode(at, at + 16, false, 1);
    std::unique_ptr<Insn> ins = got.empty() ? nullptr : std::make_unique<Insn>(std::move(got[0]));
    const Insn* raw = ins.get();
    seen_.emplace(at, std::move(ins));
    return raw;
}

std::vector<Insn> Code16::decode(std::int64_t a, std::int64_t b, bool detail, std::size_t count) const {
    const std::int64_t n = size();
    a = std::clamp<std::int64_t>(a, 0, n);
    b = std::clamp<std::int64_t>(b, a, n);
    return decode16(ByteView(bytes_).subspan(static_cast<std::size_t>(a), static_cast<std::size_t>(b - a)), a, detail, count);
}

// ---- Description --------------------------------------------------------------------------------------------------------
void Description::write(const std::optional<std::filesystem::path>& to) {
    if (to && !to->empty()) path = *to;
    std::string out;
    for (const std::string& l : lines) {
        out += l;
        out += '\n';
    }
    if (lines.empty()) out = "\n";
    fs::write_text(path, out);
}

std::vector<std::vector<std::string>> Description::records(const std::vector<std::string_view>& kinds) const {
    std::vector<std::vector<std::string>> out;
    for (const std::string& ln : lines) {
        std::vector<std::string> parts;
        for (std::string_view p : text::split(ln, '\t')) parts.emplace_back(p);
        if (std::find(kinds.begin(), kinds.end(), parts[0]) != kinds.end()) out.push_back(std::move(parts));
    }
    return out;
}

bool NeDescription::inside(Va va) const {
    const SegOff w = where(va);
    const auto it = code.find(w.seg);
    return floor_mod<Va>((va >> 16) - kFirstSelector, 8) == 0 && static_cast<std::int64_t>(w.off) < (it == code.end() ? 0 : it->second);
}

Code16& NeDescription::loaded(std::int64_t seg) const {
    auto& slot = loaded_[seg];
    if (!slot) {
        auto it = segment_bytes.find(seg);
        slot = std::make_unique<Code16>(it == segment_bytes.end() ? Bytes{} : it->second);
    }
    return *slot;
}

Code16& NeDescription::relocated(std::int64_t seg) const {
    auto& slot = relocated_[seg];
    if (!slot) {
        auto it = images.find(seg);
        slot = std::make_unique<Code16>(it == images.end() ? Bytes{} : it->second);
    }
    return *slot;
}

}  // namespace cb::decompile::describe
