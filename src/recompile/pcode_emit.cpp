// cb-unity: alone (file-local helpers named for what they do here)
// recompile/pcode_emit.cpp -- see pcode_emit.hpp.
#include "recompile/pcode_emit.hpp"
#include "recompile/pcode_emit_vmx.hpp"

#include <algorithm>
#include <cstring>
#include <format>
#include <set>
#include <stdexcept>
#include <string_view>

namespace cb::recompile::pcode_emit {

namespace {

namespace sl = decompile::sleigh;
using Op = sl::Op;
using Kind = Translator::Cell::Kind;

constexpr std::uint32_t kEieio = 0x7C0006AC;
constexpr std::uint32_t kDb16cyc = 0x7FFFFB78;   // or r31,r31,r31: the recompiler's PPC_SPIN_HINT()

// An instruction the writer declines, with the reason.
struct Decline {
    std::string why;
};

bool supported_size(unsigned s) { return s == 1 || s == 2 || s == 4 || s == 8 || s == 16; }

std::string utype(unsigned s) {
    switch (s) {
        case 1: return "uint8_t";
        case 2: return "uint16_t";
        case 4: return "uint32_t";
        case 8: return "uint64_t";
        case 16: return "__uint128_t";
    }
    throw Decline{std::format("a value of {} bytes", s)};
}

std::string stype(unsigned s) {
    switch (s) {
        case 1: return "int8_t";
        case 2: return "int16_t";
        case 4: return "int32_t";
        case 8: return "int64_t";
        case 16: return "__int128_t";
    }
    throw Decline{std::format("a value of {} bytes", s)};
}

// The type an operation of this size computes in (no promotion to int, no overflow of a narrow type).
std::string wtype(unsigned s) { return s <= 8 ? "uint64_t" : "__uint128_t"; }
std::string swtype(unsigned s) { return s <= 8 ? "int64_t" : "__int128_t"; }

std::string ftype(unsigned s) {
    if (s == 4) return "float";
    if (s == 8) return "double";
    throw Decline{std::format("a float of {} bytes", s)};
}

std::string constant(std::uint64_t value, unsigned size) {
    if (size == 16) return std::format("((__uint128_t)UINT64_C(0x{:X}))", value);
    if (size == 8) return std::format("UINT64_C(0x{:X})", value);
    return std::format("{}(0x{:X}u)", utype(size), value);
}

std::string all_ones(unsigned size) { return size >= 8 ? std::string("~UINT64_C(0)") : std::format("UINT64_C(0x{:X})", (std::uint64_t{1} << (8 * size)) - 1); }

// One instruction being written.
class Writer {
public:
    Writer(const sl::Reader& reader, const std::function<const Translator::Cell*(std::uint64_t)>& cell_at, const Names& names, std::uint32_t address)
        : reader_(reader), cell_at_(cell_at), names_(names), address_(address) {}

    Statements write(const sl::Lifted& lifted, std::uint32_t next_word) {
        const std::vector<sl::PcodeOp>& ops = lifted.ops;
        check_ops(ops, next_word);
        const std::vector<bool> live = liveness(ops);
        std::set<std::size_t> labels;
        for (std::size_t i = 0; i < ops.size(); ++i) {
            if ((ops[i].op == Op::Branch || ops[i].op == Op::CBranch) && to_next(ops[i].inputs[0])) {
                labels.insert(ops.size());
                continue;
            }
            if ((ops[i].op == Op::Branch || ops[i].op == Op::CBranch) && ops[i].inputs[0].kind == sl::Space::Constant) {
                const std::int64_t to = static_cast<std::int64_t>(i) + static_cast<std::int64_t>(ops[i].inputs[0].offset);
                if (to < 0 || to > static_cast<std::int64_t>(ops.size())) throw Decline{"a branch inside the instruction out of its operations"};
                labels.insert(static_cast<std::size_t>(to));
            }
        }
        fold_ = labels.empty();
        ops_count_ = ops.size();
        std::string body;
        for (std::size_t i = 0; i < ops.size(); ++i) {
            if (labels.count(i)) body += std::format("\t\tpcode_{:X}_{}:;\n", address_, i);
            if (!live[i]) continue;
            const std::string s = statement(ops[i], i);
            for (std::size_t at = 0; at < s.size();) {
                const std::size_t end = std::min(s.find('\n', at), s.size());
                body += "\t\t" + s.substr(at, end - at) + "\n";
                at = end + 1;
            }
        }
        if (labels.count(ops.size())) body += std::format("\t\tpcode_{:X}_{}:;\n", address_, ops.size());

        Statements out;
        out.fpu = fpu_;
        out.vmx = vmx_;
        std::string decls;
        for (const auto& [offset, size] : uniques_) decls += std::format("\t\t{} t_{:x};\n", utype(size), offset);
        for (const auto& [offset, size] : dropped_) decls += std::format("\t\t{} d_{:x};\n", utype(size), offset);
        for (const auto& [n, loaded] : cr_loaded_) {
            const std::string c = names_.cr(n);
            decls += loaded ? std::format("\t\tuint8_t cr_{0} = uint8_t(({1}.lt << 3) | ({1}.gt << 2) | ({1}.eq << 1) | {1}.so);\n", n, c)
                            : std::format("\t\tuint8_t cr_{};\n", n);
        }
        for (const unsigned n : cr_written_) {
            const std::string c = names_.cr(n);
            body += std::format("\t\t{0}.lt = (cr_{1} >> 3) & 1; {0}.gt = (cr_{1} >> 2) & 1; {0}.eq = (cr_{1} >> 1) & 1; {0}.so = cr_{1} & 1;\n", c, n);
        }
        if (decls.empty() && labels.empty()) {
            // (no block needed: the statements one level in)
            for (std::size_t at = 0; at < body.size();) {
                const std::size_t end = body.find('\n', at);
                out.text += body.substr(at + 1, end - at);   // (drop one of the two tabs)
                at = end + 1;
            }
        } else {
            out.text = "\t{\n" + decls + body + "\t}\n";
        }
        return out;
    }

private:
    // ---- what is declined, before anything is written
    void check_ops(const std::vector<sl::PcodeOp>& ops, std::uint32_t next_word) {
        bool vector = false, floating = false, store = false;
        std::set<std::uint64_t> products;   // the temporaries a FLOAT_MULT wrote
        for (const sl::PcodeOp& op : ops) {
            switch (op.op) {
                case Op::Branch:
                case Op::CBranch:
                    if (op.inputs[0].kind != sl::Space::Constant && !to_next(op.inputs[0])) {
                        if (!names_.jump) throw Decline{"a branch"};
                        flow_ = true;
                    }
                    break;
                case Op::BranchInd:
                    if (!names_.jump_indirect) throw Decline{"an indirect branch"};
                    flow_ = true;
                    break;
                case Op::Call:
                    if (!names_.call) throw Decline{"a call"};
                    flow_ = true;
                    break;
                case Op::CallInd:
                    if (!names_.call_indirect) throw Decline{"an indirect call"};
                    flow_ = true;
                    break;
                case Op::Return:
                    if (!names_.jump) throw Decline{"a return"};
                    flow_ = true;
                    break;
                case Op::CallOther:
                    if (!served(reader_.user_op_name(op.inputs[0].offset))) throw Decline{"the user operation " + reader_.user_op_name(op.inputs[0].offset)};
                    if (reader_.user_op_name(op.inputs[0].offset).starts_with("fused")) floating = true;
                    break;
                case Op::Store: store = true; break;
                default: break;
            }
            const unsigned code = static_cast<unsigned>(op.op);
            if (code >= static_cast<unsigned>(Op::FloatEqual) && code <= static_cast<unsigned>(Op::FloatRound)) floating = true;
            if (op.op == Op::FloatMult && op.output && op.output->kind == sl::Space::Unique) products.insert(op.output->offset);
            if ((op.op == Op::FloatAdd || op.op == Op::FloatSub) && std::any_of(op.inputs.begin(), op.inputs.end(), [&](const sl::Varnode& v) {
                    return v.kind == sl::Space::Unique && products.count(v.offset);
                })) {
                throw Decline{"a fused multiply-add"};
            }
            auto is_vector = [&](const sl::Varnode& v) {
                if (v.kind != sl::Space::Register) return false;
                const Translator::Cell* c = cell_at_(v.offset);
                return c != nullptr && c->kind == Kind::Vector;
            };
            if (op.output && is_vector(*op.output)) vector = true;
            for (const sl::Varnode& v : op.inputs) vector = vector || is_vector(v);
        }
        if (vector && floating) throw Decline{"floating point on a vector register"};
        mmio_ = store && next_word == kEieio;   // (a store an eieio follows is to memory-mapped I/O: PPC_MM_STORE_*)
        fpu_ = floating;
        // a condition field is a local of the block, stored back at its end: a way out before the end would lose it
        if (flow_) {
            for (const sl::PcodeOp& op : ops) {
                if (op.output && op.output->kind == sl::Space::Register) {
                    const Translator::Cell* c = cell_at_(op.output->offset);
                    if (c != nullptr && c->kind == Kind::Cr) throw Decline{"a condition field written with a way out"};
                }
            }
        }
    }

    // A register varnode's place: the cell holding it, or nullptr.
    const Translator::Cell* holder(const sl::Varnode& v) const {
        const Translator::Cell* c = cell_at_(v.offset);
        if (c == nullptr || v.offset + v.size > c->offset + c->size) return nullptr;
        return c;
    }

    // Whether writing `v` changes something the context keeps (a dropped register's write does not).
    bool kept(const sl::Varnode& v) const {
        if (v.kind != sl::Space::Register) return true;
        const Translator::Cell* c = holder(v);
        if (c == nullptr) {
            const std::string name = reader_.register_name(v.offset, v.size);
            throw Decline{"the register " + (name.empty() ? std::format("at {:#x}", v.offset) : name)};
        }
        if (c->kind == Kind::Lr && names_.lr().empty()) return false;
        return c->kind != Kind::Dropped;
    }

    // The operations whose results are used: what writes a kept register or memory, the branches inside the instruction, and what
    // computes their inputs.
    std::vector<bool> liveness(const std::vector<sl::PcodeOp>& ops) const {
        std::vector<bool> live(ops.size(), false);
        std::vector<std::pair<std::uint64_t, std::uint64_t>> needed;   // [begin, end) of the temporaries read
        std::vector<std::pair<std::uint64_t, std::uint64_t>> needed_dropped;   // ... and of the dropped registers read
        const auto overlaps = [&](const sl::Varnode& v) {
            return std::any_of(needed.begin(), needed.end(), [&](const auto& r) { return v.offset < r.second && r.first < v.offset + v.size; });
        };
        const auto dropped_read = [&](const sl::Varnode& v) {
            return std::any_of(needed_dropped.begin(), needed_dropped.end(), [&](const auto& r) { return v.offset < r.second && r.first < v.offset + v.size; });
        };
        const auto is_dropped = [&](const sl::Varnode& v) {
            if (v.kind != sl::Space::Register) return false;
            const Translator::Cell* c = holder(v);
            return c != nullptr && c->kind == Kind::Dropped;
        };
        for (bool changed = true; changed;) {
            changed = false;
            for (std::size_t k = ops.size(); k-- > 0;) {
                if (live[k]) continue;
                const sl::PcodeOp& op = ops[k];
                bool l = false;
                if (op.op == Op::Store || op.op == Op::Branch || op.op == Op::CBranch || op.op == Op::BranchInd || op.op == Op::Call ||
                    op.op == Op::CallInd || op.op == Op::Return || (op.op == Op::CallOther && (!op.output || effects(op)))) {
                    l = true;
                } else if (op.output) {
                    l = op.output->kind == sl::Space::Unique ? overlaps(*op.output) : is_dropped(*op.output) ? dropped_read(*op.output) : kept(*op.output);
                }
                if (!l) continue;
                live[k] = true;
                changed = true;
                for (const sl::Varnode& v : op.inputs) {
                    if (v.kind == sl::Space::Unique) needed.emplace_back(v.offset, v.offset + v.size);
                    if (is_dropped(v)) needed_dropped.emplace_back(v.offset, v.offset + v.size);
                }
            }
        }
        return live;
    }

    // ---- reading and writing varnodes
    std::string cell_value(const Translator::Cell& c) {
        switch (c.kind) {
            case Kind::Gpr: return names_.r(c.index) + ".u64";
            case Kind::Fpr: return names_.f(c.index) + ".u64";
            case Kind::Ctr: return names_.ctr() + ".u64";
            case Kind::Lr: return names_.lr();
            case Kind::Vector: return "__builtin_bit_cast(__uint128_t, " + names_.v(c.index) + ")";
            case Kind::Cr:
                cr_loaded_.emplace(c.index, true);   // (read before it is written: loaded at the start of the block)
                return std::format("cr_{}", c.index);
            case Kind::XerBit: return names_.xer() + "." + c.member;
            // (a context without MSR reads the value it starts with, as ctx.msr does: the title's code only puts it back)
            case Kind::Msr: return names_.msr && !names_.msr().empty() ? "uint64_t(" + names_.msr() + ")" : std::string("UINT64_C(0x200A000)");
            case Kind::TimeBase: return c.index == 0 ? std::string("uint64_t(PPC_QUERY_TIMEBASE())") : std::string("uint64_t(PPC_QUERY_TIMEBASE() >> 32)");
            case Kind::XerWord: return std::format("uint64_t(({0}.so << 31) | ({0}.ov << 30) | ({0}.ca << 29))", names_.xer());
            case Kind::Dropped: break;
        }
        throw Decline{"a read of " + c.member + ", which the context keeps no value of"};
    }

    std::string read(const sl::Varnode& v) {
        if (!supported_size(v.size)) throw Decline{std::format("a value of {} bytes", v.size)};
        switch (v.kind) {
            case sl::Space::Constant: return constant(v.offset, v.size);
            case sl::Space::Unique: {
                if (const std::optional<std::uint64_t> k = known(v)) return constant(*k, v.size);
                const auto it = containing(uniques_, v);
                if (it == uniques_.end()) throw Decline{"a temporary read before it is written"};
                const unsigned shift = static_cast<unsigned>(8 * ((it->first + it->second) - (v.offset + v.size)));
                if (it->first == v.offset && it->second == v.size) return std::format("t_{:x}", v.offset);
                return std::format("{}(t_{:x} >> {})", utype(v.size), it->first, shift);
            }
            case sl::Space::Register: {
                const Translator::Cell* c = holder(v);
                if (c == nullptr) {
                    const std::string name = reader_.register_name(v.offset, v.size);
                    throw Decline{"the register " + (name.empty() ? std::format("at {:#x}", v.offset) : name)};
                }
                if (c->kind == Kind::Lr && names_.lr().empty()) throw Decline{"a read of LR, which the context does not keep"};
                if (c->kind == Kind::Dropped) {
                    const auto it = dropped_.find(v.offset);
                    if (it == dropped_.end() || it->second != v.size) throw Decline{"a read of " + c->member + ", which the context keeps no value of"};
                    return std::format("d_{:x}", v.offset);
                }
                const std::string full = cell_value(*c);
                const unsigned shift = static_cast<unsigned>(8 * ((c->offset + c->size) - (v.offset + v.size)));
                if (shift == 0 && v.size == c->size) return full;
                return std::format("{}(({}) >> {})", utype(v.size), full, shift);
            }
            case sl::Space::Memory: break;
        }
        throw Decline{"a varnode in memory used as a value"};
    }

    // The statement that puts `value` (of the varnode's size) in `v`.
    std::string write(const sl::Varnode& v, const std::string& value) {
        if (!supported_size(v.size)) throw Decline{std::format("a value of {} bytes", v.size)};
        if (v.kind == sl::Space::Unique) {
            for (const auto& [o, size] : uniques_) {
                if (o != v.offset && o < v.offset + v.size && v.offset < o + size) throw Decline{"temporaries that overlap"};
            }
            for (auto k = consts_.begin(); k != consts_.end();) {
                k = k->first < v.offset + v.size && v.offset < k->first + k->second.second ? consts_.erase(k) : std::next(k);
            }
            const auto [it, added] = uniques_.emplace(v.offset, v.size);
            if (!added && it->second != v.size) throw Decline{"a temporary written at two sizes"};
            return std::format("t_{:x} = {};", v.offset, value);
        }
        if (v.kind != sl::Space::Register) throw Decline{"an output that is no register or temporary"};
        if (const Translator::Cell* c = holder(v); c != nullptr && c->kind == Kind::Dropped) {
            const auto [it, added] = dropped_.emplace(v.offset, v.size);
            if (!added && it->second != v.size) throw Decline{"a dropped register written at two sizes"};
            return std::format("d_{:x} = {};", v.offset, value);
        }
        if (!kept(v)) return {};
        const Translator::Cell& c = *holder(v);
        const unsigned shift = static_cast<unsigned>(8 * ((c.offset + c.size) - (v.offset + v.size)));
        const bool whole = shift == 0 && v.size == c.size;
        switch (c.kind) {
            case Kind::Gpr:
            case Kind::Fpr:
            case Kind::Ctr:
            case Kind::Lr: {
                const std::string place = cell_value(c);
                if (whole) return std::format("{} = {};", place, value);
                return std::format("{0} = ({0} & ~({1} << {2})) | (uint64_t({3}) << {2});", place, all_ones(v.size), shift, value);
            }
            case Kind::Vector: {
                const std::string place = names_.v(c.index);
                if (whole) return std::format("{} = __builtin_bit_cast(PPCVRegister, (__uint128_t)({}));", place, value);
                const std::string ones = v.size >= 16 ? "~(__uint128_t)0" : std::format("(((__uint128_t)1 << {}) - 1)", 8 * v.size);
                return std::format("{0} = __builtin_bit_cast(PPCVRegister, (__builtin_bit_cast(__uint128_t, {0}) & ~({1} << {2})) | ((__uint128_t)({3}) << {2}));",
                                   place, ones, shift, value);
            }
            case Kind::Cr:
                cr_loaded_.emplace(c.index, false);   // (written first: no load)
                cr_written_.insert(c.index);
                return std::format("cr_{} = {};", c.index, value);
            case Kind::XerBit: return std::format("{}.{} = {};", names_.xer(), c.member, value);
            case Kind::Msr:
                if (!whole) throw Decline{"a write of part of MSR"};
                if (!names_.msr || names_.msr().empty()) return {};   // (the context keeps no MSR)
                return std::format("{} = uint32_t({});", names_.msr(), value);
            case Kind::TimeBase: throw Decline{"a write of the time base"};
            case Kind::XerWord:
                if (!whole) throw Decline{"a write of part of XER"};
                return std::format("{{ const uint64_t x = {1}; {0}.so = (x >> 31) & 1; {0}.ov = (x >> 30) & 1; {0}.ca = (x >> 29) & 1; }}", names_.xer(), value);
            case Kind::Dropped: break;
        }
        return {};
    }

    std::string sext(const std::string& value, unsigned from, unsigned to) const { return std::format("{}({}({}))", swtype(to), stype(from), value); }

    std::string fvalue(const std::string& bits, unsigned size) const { return std::format("__builtin_bit_cast({}, {}({}))", ftype(size), utype(size), bits); }
    std::string fbits(const std::string& value, unsigned size) const { return std::format("__builtin_bit_cast({}, {}({}))", utype(size), ftype(size), value); }

    // ---- one operation
    std::string statement(const sl::PcodeOp& op, std::size_t index) {
        const auto in = [&](std::size_t k) { return read(op.inputs.at(k)); };
        const auto isz = [&](std::size_t k) { return op.inputs.at(k).size; };
        const auto label = [&]() {
            return std::format("pcode_{:X}_{}", address_, static_cast<std::int64_t>(index) + static_cast<std::int64_t>(op.inputs[0].offset));
        };
        if ((op.op == Op::Branch || op.op == Op::CBranch) && to_next(op.inputs[0])) {
            const std::string end = std::format("pcode_{:X}_{}", address_, ops_count_);
            return op.op == Op::Branch ? "goto " + end + ";" : std::format("if ({}) goto {};", in(1), end);
        }
        const bool to_address = (op.op == Op::Branch || op.op == Op::CBranch) && op.inputs[0].kind != sl::Space::Constant;
        if (op.op == Op::Branch && !to_address) return "goto " + label() + ";";
        if (op.op == Op::CBranch && !to_address) return std::format("if ({}) goto {};", in(1), label());
        if (op.op == Op::Branch) return recompiler_text(names_.jump(target(op.inputs[0])));
        if (op.op == Op::CBranch) return std::format("if ({}) {{\n{}}}", in(1), recompiler_text(names_.jump(target(op.inputs[0])), 1));
        if (op.op == Op::Call) return recompiler_text(names_.call(target(op.inputs[0])));
        if (op.op == Op::CallInd) return recompiler_text(names_.call_indirect(std::format("uint32_t({})", in(0))));
        if (op.op == Op::BranchInd) {
            // a branch through LR is a return (the description's bclr forms other than blr branch through it, as the processor does);
            // in the recompiled program the caller's code follows the call, so the branch is `return`
            if (const sl::Varnode& v = op.inputs.at(0); v.kind == sl::Space::Register) {
                if (const Translator::Cell* c = holder(v); c != nullptr && c->kind == Kind::Lr) return "return;";
            }
            return recompiler_text(names_.jump_indirect(std::format("uint32_t({})", in(0))));
        }
        if (op.op == Op::Return) return "return;";
        if (op.op == Op::Store) {
            if (const std::string space = reader_.space_name(static_cast<unsigned>(op.inputs.at(0).offset)); space != "ram") throw Decline{"a store to the space " + space};
            const std::string address = in(1);
            const sl::Varnode& v = op.inputs.at(2);
            const std::string value = in(2);
            switch (v.size) {
                case 1: case 2: case 4: case 8: return std::format("PPC_{}STORE_U{}({}, {});", mmio_ ? "MM_" : "", 8 * v.size, address, value);
                case 16:
                    if (mmio_) throw Decline{"a 16-byte store to memory-mapped I/O"};
                    return std::format("{{ const __uint128_t q = {1}; PPC_STORE_U64({0}, uint64_t(q >> 64)); PPC_STORE_U64({0} + 8, uint64_t(q)); }}", address, value);
            }
            throw Decline{std::format("a store of {} bytes", v.size)};
        }
        if (op.op == Op::CallOther) {
            const std::string name = reader_.user_op_name(op.inputs[0].offset);
            if (vmx::known(name)) {
                std::vector<vmx::Arg> args;
                for (std::size_t k = 1; k < op.inputs.size(); ++k) args.push_back(vmx::Arg{read(op.inputs[k]), known(op.inputs[k]), op.inputs[k].size});
                const std::string current = op.output && op.output->kind == sl::Space::Register ? read(*op.output) : std::string();
                std::string why;
                const std::optional<vmx::Written> w = vmx::emit(name, args, current, &why);
                if (!w) throw Decline{why};
                vmx_ = vmx_ || w->vmx;
                if (w->statement) return w->text;
                if (!op.output) throw Decline{"the user operation " + name + " with no output"};
                return write(*op.output, w->text);
            }
            const auto arg = [&](std::size_t k) { return in(k + 1); };
            const auto fp = [&](std::size_t k) { return fvalue(arg(k), isz(k + 1)); };
            const auto fused = [&](const char* helper, bool negate) {
                return write(*op.output, fbits(std::format("{}({}, {}, {}{})", helper, fp(0), fp(1), fp(2), negate ? ", true" : ""), op.output->size));
            };
            if (name == "fusedMultiplyAdd") return fused("ppc_fmadd", false);
            if (name == "fusedMultiplySubtract") return fused("ppc_fmsub", false);
            if (name == "fusedNegativeMultiplyAdd") return fused("ppc_fmadd", true);
            if (name == "fusedNegativeMultiplySubtract") return fused("ppc_fmsub", true);
            if (name == "fusedMultiplyAddSingle") return fused("ppc_fmadds", false);
            if (name == "fusedMultiplySubtractSingle") return fused("ppc_fmsubs", false);
            if (name == "fusedNegativeMultiplyAddSingle") return fused("ppc_fmadds", true);
            if (name == "fusedNegativeMultiplySubtractSingle") return fused("ppc_fmsubs", true);
            // the reservation: the value lwarx loaded, and a compare and exchange against it
            const std::string reserved = names_.reserved ? names_.reserved() : std::string();
            if (reserved.empty() && name.find("Reserve") != std::string::npos) throw Decline{"no reservation"};
            if (name == "loadWordAndReserve") {
                return write(*op.output, std::format("({0}.u32 = *(uint32_t*)PPC_HOST({1}), __builtin_bswap32({0}.u32))", reserved, arg(0)));
            }
            if (name == "loadDoublewordAndReserve") {
                return write(*op.output, std::format("({0}.u64 = *(uint64_t*)PPC_HOST({1}), __builtin_bswap64({0}.u64))", reserved, arg(0)));
            }
            if (name == "storeWordConditional" || name == "storeDoublewordConditional") {
                if (names_.reserved == nullptr || names_.reserved().empty()) throw Decline{"no reservation"};
                const bool word = name == "storeWordConditional";
                return write(*op.output, std::format("uint8_t(__sync_bool_compare_and_swap(reinterpret_cast<uint{0}_t*>(PPC_HOST({1})), {2}.{3}, __builtin_bswap{0}({4})))",
                                                     word ? 32 : 64, arg(0), names_.reserved(), word ? "s32" : "s64", word ? "uint32_t(" + arg(1) + ")" : arg(1)));
            }
            if (name == "dataCacheBlockClearToZero") return std::format("memset(PPC_HOST(uint32_t({}) & ~31u), 0, 32);", arg(0));
            // the FPSCR as the context keeps it (the host's rounding and flush modes)
            if (name == "moveFromFPSCR") return write(*op.output, "uint64_t(ctx.fpscr.loadFromHost())");
            if (name == "moveToFPSCR") return std::format("ctx.fpscr.storeFromGuest(uint32_t({}));", arg(0));
            return {};   // (an operation that changes nothing a title sees: a cache hint, a barrier, a trap XenonRecomp's titles never take)
        }
        if (!op.output) throw Decline{std::string("an operation with no output: ") + sl::op_name(op.op)};
        if (fold_ && op.output->kind == sl::Space::Unique) {
            const sl::Varnode& o = *op.output;
            const bool declared = std::any_of(uniques_.begin(), uniques_.end(), [&](const auto& u) { return u.first < o.offset + o.size && o.offset < u.first + u.second; });
            if (const std::optional<std::uint64_t> k = declared ? std::nullopt : fold(op)) {
                for (auto c = consts_.begin(); c != consts_.end();) c = c->first < o.offset + o.size && o.offset < c->first + c->second.second ? consts_.erase(c) : std::next(c);
                consts_[o.offset] = {*k, o.size};
                return {};
            }
        }
        const unsigned os = op.output->size;
        const std::string T = utype(os), W = wtype(os);
        std::string e;
        switch (op.op) {
            case Op::Copy: e = in(0); break;
            case Op::Load: {
                const std::string address = in(1);
                const std::string space = reader_.space_name(static_cast<unsigned>(op.inputs.at(0).offset));
                if (space == "const") {   // (a constant space's cell holds its own address: what a subtable exports as *[const])
                    e = std::format("{}({})", T, address);
                    break;
                }
                if (space != "ram") throw Decline{"a load from the space " + space};
                switch (os) {
                    case 1: case 2: case 4: case 8: e = std::format("{}(PPC_LOAD_U{}({}))", T, 8 * os, address); break;
                    case 16: e = std::format("(((__uint128_t)PPC_LOAD_U64({0}) << 64) | PPC_LOAD_U64({0} + 8))", address); break;
                    default: throw Decline{std::format("a load of {} bytes", os)};
                }
                break;
            }
            case Op::IntZext: e = std::format("{}({})", T, in(0)); break;
            case Op::IntSext: e = std::format("{}({})", T, sext(in(0), isz(0), os)); break;
            case Op::IntAdd: e = std::format("{}({}({}) + {})", T, W, in(0), in(1)); break;
            case Op::IntSub: e = std::format("{}({}({}) - {})", T, W, in(0), in(1)); break;
            case Op::IntMult: e = std::format("{}({}({}) * {})", T, W, in(0), in(1)); break;
            case Op::IntAnd: e = std::format("{}({} & {})", T, in(0), in(1)); break;
            case Op::IntOr: e = std::format("{}({} | {})", T, in(0), in(1)); break;
            case Op::IntXor: e = std::format("{}({} ^ {})", T, in(0), in(1)); break;
            case Op::IntNegate: e = std::format("{}(~{}({}))", T, W, in(0)); break;
            case Op::Int2Comp: e = std::format("{}({}(0) - {})", T, W, in(0)); break;
            case Op::IntLeft:
                if (const std::optional<std::uint64_t> k = known(op.inputs.at(1))) {
                    e = *k >= 8 * os ? std::format("{}(0)", T) : std::format("{}({}({}) << {})", T, W, in(0), *k);
                    break;
                }
                e = std::format("{}(uint64_t({}) >= {} ? 0 : {}({}) << uint64_t({}))", T, in(1), 8 * os, W, in(0), in(1));
                break;
            case Op::IntRight:
                if (const std::optional<std::uint64_t> k = known(op.inputs.at(1))) {
                    e = *k >= 8 * isz(0) ? std::format("{}(0)", T) : std::format("{}({}({}) >> {})", T, W, in(0), *k);
                    break;
                }
                e = std::format("{}(uint64_t({}) >= {} ? 0 : {}({}) >> uint64_t({}))", T, in(1), 8 * isz(0), W, in(0), in(1));
                break;
            case Op::IntSRight: {
                const std::string s = sext(in(0), isz(0), isz(0));
                if (const std::optional<std::uint64_t> k = known(op.inputs.at(1))) {
                    e = std::format("{}({} >> {})", T, s, std::min<std::uint64_t>(*k, 8 * isz(0) - 1));
                    break;
                }
                e = std::format("{}({} < 0 && uint64_t({}) >= {} ? {}(-1) : uint64_t({}) >= {} ? 0 : {} >> uint64_t({}))", T, s, in(1), 8 * isz(0), swtype(isz(0)), in(1),
                                8 * isz(0), s, in(1));
                break;
            }
            case Op::IntDiv: e = std::format("{}({} == 0 ? 0 : {}({}) / {})", T, in(1), W, in(0), in(1)); break;
            case Op::IntRem: e = std::format("{}({} == 0 ? 0 : {}({}) % {})", T, in(1), W, in(0), in(1)); break;
            case Op::IntSDiv:
            case Op::IntSRem: {
                const std::string a = sext(in(0), isz(0), isz(0)), b = sext(in(1), isz(1), isz(1));
                const std::string guard = isz(0) >= 8 ? std::format(" || ({} == -1 && {} == {}(1) << {})", b, a, swtype(isz(0)), 8 * isz(0) - 1) : std::string();
                e = std::format("{}({} == 0{} ? 0 : {} {} {})", T, b, guard, a, op.op == Op::IntSDiv ? "/" : "%", b);
                break;
            }
            case Op::IntEqual: e = std::format("uint8_t({} == {})", in(0), in(1)); break;
            case Op::IntNotEqual: e = std::format("uint8_t({} != {})", in(0), in(1)); break;
            case Op::IntLess: e = std::format("uint8_t({} < {})", in(0), in(1)); break;
            case Op::IntLessEqual: e = std::format("uint8_t({} <= {})", in(0), in(1)); break;
            case Op::IntSLess: e = std::format("uint8_t({} < {})", sext(in(0), isz(0), isz(0)), sext(in(1), isz(1), isz(1))); break;
            case Op::IntSLessEqual: e = std::format("uint8_t({} <= {})", sext(in(0), isz(0), isz(0)), sext(in(1), isz(1), isz(1))); break;
            case Op::IntCarry: e = std::format("uint8_t({}({}({}) + {}) < {})", utype(isz(0)), wtype(isz(0)), in(0), in(1), in(0)); break;
            case Op::IntSCarry: {
                const std::string a = in(0), b = in(1), r = std::format("{}({}({}) + {})", utype(isz(0)), wtype(isz(0)), a, b);
                e = std::format("uint8_t((((({0} ^ {2}) & ({1} ^ {2}))) >> {3}) & 1)", a, b, r, 8 * isz(0) - 1);
                break;
            }
            case Op::IntSBorrow: {
                const std::string a = in(0), b = in(1), r = std::format("{}({}({}) - {})", utype(isz(0)), wtype(isz(0)), a, b);
                e = std::format("uint8_t((((({0} ^ {1}) & ({0} ^ {2}))) >> {3}) & 1)", a, b, r, 8 * isz(0) - 1);
                break;
            }
            case Op::BoolNegate: e = std::format("uint8_t(!{})", in(0)); break;
            case Op::BoolAnd: e = std::format("uint8_t({} & {})", in(0), in(1)); break;
            case Op::BoolOr: e = std::format("uint8_t({} | {})", in(0), in(1)); break;
            case Op::BoolXor: e = std::format("uint8_t({} ^ {})", in(0), in(1)); break;
            case Op::Piece: e = std::format("{}(({}({}) << {}) | {})", T, W, in(0), 8 * isz(1), in(1)); break;
            case Op::Subpiece: e = std::format("{}({}({}) >> {})", T, wtype(isz(0)), in(0), 8 * op.inputs.at(1).offset); break;
            case Op::PopCount:
                if (isz(0) > 8) e = std::format("{0}(__builtin_popcountll(uint64_t({1})) + __builtin_popcountll(uint64_t({1} >> 64)))", T, in(0));
                else e = std::format("{}(__builtin_popcountll(uint64_t({})))", T, in(0));
                break;
            case Op::LzCount:
                if (isz(0) > 8) throw Decline{"a leading-zero count of 16 bytes"};
                e = std::format("{0}({1} == 0 ? {2} : __builtin_clzll(uint64_t({1})) - {3})", T, in(0), 8 * isz(0), 64 - 8 * isz(0));
                break;
            // ---- floating point: the values are the bits
            case Op::FloatAdd: e = fbits(std::format("{} + {}", fvalue(in(0), isz(0)), fvalue(in(1), isz(1))), os); break;
            case Op::FloatSub: e = fbits(std::format("{} - {}", fvalue(in(0), isz(0)), fvalue(in(1), isz(1))), os); break;
            case Op::FloatMult: e = fbits(std::format("{} * {}", fvalue(in(0), isz(0)), fvalue(in(1), isz(1))), os); break;
            case Op::FloatDiv: e = fbits(std::format("{} / {}", fvalue(in(0), isz(0)), fvalue(in(1), isz(1))), os); break;
            case Op::FloatNeg: e = fbits(std::format("-{}", fvalue(in(0), isz(0))), os); break;
            case Op::FloatAbs: e = fbits(std::format("__builtin_fabs({})", fvalue(in(0), isz(0))), os); break;
            case Op::FloatSqrt: e = fbits(std::format("({})__builtin_sqrt({})", ftype(os), fvalue(in(0), isz(0))), os); break;
            case Op::FloatEqual: e = std::format("uint8_t({} == {})", fvalue(in(0), isz(0)), fvalue(in(1), isz(1))); break;
            case Op::FloatNotEqual: e = std::format("uint8_t({} != {})", fvalue(in(0), isz(0)), fvalue(in(1), isz(1))); break;
            case Op::FloatLess: e = std::format("uint8_t({} < {})", fvalue(in(0), isz(0)), fvalue(in(1), isz(1))); break;
            case Op::FloatLessEqual: e = std::format("uint8_t({} <= {})", fvalue(in(0), isz(0)), fvalue(in(1), isz(1))); break;
            case Op::FloatNan: e = std::format("uint8_t(__builtin_isnan({}))", fvalue(in(0), isz(0))); break;
            case Op::FloatInt2Float: e = fbits(std::format("{}({})", ftype(os), sext(in(0), isz(0), isz(0))), os); break;
            case Op::FloatFloat2Float: e = fbits(std::format("{}({})", ftype(os), fvalue(in(0), isz(0))), os); break;
            case Op::FloatTrunc: {
                // toward zero, saturated, a NaN the most negative value: as fctiwz and fctidz give it (a plain cast is undefined outside the range)
                if (os != 4 && os != 8) throw Decline{std::format("a truncation to {} bytes", os)};
                const std::string S = stype(os), limit = os == 4 ? "2147483648.0" : "9223372036854775808.0";
                e = std::format("{0}([](double x) {{ return x != x || x < -{2} ? {1}(({3}(1) << {4})) : x >= {2} ? {1}(~({3}(1) << {4})) : {1}(x); }}(double({5})))",
                                T, S, limit, "uint64_t", 8 * os - 1, fvalue(in(0), isz(0)));
                break;
            }
            case Op::FloatCeil: e = fbits(std::format("({})__builtin_ceil({})", ftype(os), fvalue(in(0), isz(0))), os); break;
            case Op::FloatFloor: e = fbits(std::format("({})__builtin_floor({})", ftype(os), fvalue(in(0), isz(0))), os); break;
            case Op::FloatRound: e = fbits(std::format("({})__builtin_round({})", ftype(os), fvalue(in(0), isz(0))), os); break;
            default: throw Decline{std::string("the operation ") + sl::op_name(op.op)};
        }
        return write(*op.output, e);
    }

    const sl::Reader& reader_;
    const std::function<const Translator::Cell*(std::uint64_t)>& cell_at_;
    const Names& names_;
    std::uint32_t address_;
    bool fpu_ = false;
    std::map<std::uint64_t, unsigned> uniques_;   // the temporaries written: offset -> size
    std::map<std::uint64_t, unsigned> dropped_;   // the dropped registers written and read again in the instruction: offset -> size
    bool flow_ = false;                            // it has a way out (a branch, call or return to an address)
    std::size_t ops_count_ = 0;

    // A branch to the instruction after this one: no way out, only the end of this instruction.
    bool to_next(const sl::Varnode& v) const { return v.kind == sl::Space::Memory && v.offset == static_cast<std::uint64_t>(address_) + 4; }
    bool mmio_ = false;                            // its store is to memory-mapped I/O
    bool vmx_ = false;                             // it computes with the vector unit's floating point

    // The user operations the writer serves.
    static bool served(const std::string& name) {
        static const std::set<std::string, std::less<>> names = {
            "fusedMultiplyAdd", "fusedMultiplySubtract", "fusedNegativeMultiplyAdd", "fusedNegativeMultiplySubtract", "fusedMultiplyAddSingle",
            "fusedMultiplySubtractSingle", "fusedNegativeMultiplyAddSingle", "fusedNegativeMultiplySubtractSingle", "loadWordAndReserve",
            "loadDoublewordAndReserve", "storeWordConditional", "storeDoublewordConditional", "dataCacheBlockClearToZero", "moveFromFPSCR", "moveToFPSCR",
            // no effect a title sees (XenonRecomp writes them as nothing too)
            "sync", "enforceInOrderExecutionIO", "dataCacheBlockTouch", "dataCacheBlockTouchForStore", "dataCacheBlockFlush", "dataCacheBlockStore",
            "instructionCacheBlockInvalidate", "trapWord", "trapDoubleWord", "trapDoubleWordImmediate", "dataStreamStop", "dataStreamStopAll",
            "dataStreamTouch", "dataStreamTouchSoon", "dataStreamTouchForStore", "dataStreamTouchForStoreTransient"};
        return names.contains(name) || vmx::known(name);
    }
    // Whether a user operation with an output also changes memory (a conditional store).
    bool effects(const sl::PcodeOp& op) const {
        const std::string name = reader_.user_op_name(op.inputs[0].offset);
        return name.starts_with("store") || name.find("Reserve") != std::string::npos || name == "moveFromFPSCR";
    }

    // A branch's or call's target address.
    static std::uint32_t target(const sl::Varnode& v) {
        if (v.kind != sl::Space::Memory) throw Decline{"a way out to a place that is no address"};
        return static_cast<std::uint32_t>(v.offset);
    }

    // The recompiler's lines (each starting with a tab, ending with a newline) as statements of this writer, `extra` tabs in.
    static std::string recompiler_text(const std::string& text, unsigned extra = 0) {
        std::string out;
        for (std::size_t at = 0; at < text.size();) {
            std::size_t end = text.find('\n', at);
            if (end == std::string::npos) end = text.size();
            std::string_view line(text.data() + at, end - at);
            if (!line.empty() && line.front() == '\t') line.remove_prefix(1);
            if (!line.empty()) {
                out.append(extra, '\t');
                out.append(line);
                out += '\n';
            }
            at = end + 1;
        }
        if (!out.empty() && extra == 0) out.pop_back();   // (the last line's newline is the writer's)
        return out;
    }
    std::map<std::uint64_t, std::pair<std::uint64_t, unsigned>> consts_;   // the temporaries whose value is known: offset -> (value, size)
    bool fold_ = false;                                // straight-line p-code: constants are folded as they are written
    std::map<unsigned, bool> cr_loaded_;               // the condition fields used: number -> read before written (loaded first)
    std::set<unsigned> cr_written_;                    // those written (stored back at the end of the block)

    // A varnode's value when it is known while writing: a constant, or a temporary folded to one.
    std::optional<std::uint64_t> known(const sl::Varnode& v) const {
        if (v.kind == sl::Space::Constant) return v.size >= 8 ? v.offset : v.offset & ((std::uint64_t{1} << (8 * v.size)) - 1);
        if (v.kind == sl::Space::Unique && fold_) {
            const auto k = containing(consts_, v);
            if (k == consts_.end() || v.size > 8) return std::nullopt;
            const unsigned base_size = k->second.second;
            if (base_size > 8) return std::nullopt;
            const unsigned shift = static_cast<unsigned>(8 * ((k->first + base_size) - (v.offset + v.size)));
            const std::uint64_t value = shift >= 64 ? 0 : k->second.first >> shift;
            return v.size >= 8 ? value : value & ((std::uint64_t{1} << (8 * v.size)) - 1);
        }
        return std::nullopt;
    }

    // The written temporary holding all of `v` (its own place or a bigger one around it), or end().
    template <typename Map>
    static typename Map::const_iterator containing(const Map& m, const sl::Varnode& v) {
        auto it = m.upper_bound(v.offset);
        if (it == m.begin()) return m.end();
        --it;
        const unsigned size = size_of(it->second);
        return v.offset + v.size <= it->first + size ? it : m.end();
    }
    static unsigned size_of(unsigned size) { return size; }
    static unsigned size_of(const std::pair<std::uint64_t, unsigned>& c) { return c.second; }

    // The value of an integer operation of at most 8 bytes whose inputs are known, or nullopt.
    std::optional<std::uint64_t> fold(const sl::PcodeOp& op) const {
        const unsigned os = op.output->size;
        if (os > 8) return std::nullopt;
        std::uint64_t in[2] = {};
        for (std::size_t k = 0; k < op.inputs.size(); ++k) {
            if (k >= 2 || op.inputs[k].size > 8) return std::nullopt;
            const std::optional<std::uint64_t> v = known(op.inputs[k]);
            if (!v) return std::nullopt;
            in[k] = *v;
        }
        const auto mask = [](std::uint64_t v, unsigned size) { return size >= 8 ? v : v & ((std::uint64_t{1} << (8 * size)) - 1); };
        const unsigned is = op.inputs.empty() ? 0 : op.inputs[0].size;
        const auto sx = [&](std::uint64_t v) {
            const unsigned bits = 8 * is;
            return bits >= 64 ? v : ((v >> (bits - 1)) & 1) != 0 ? v | ~((std::uint64_t{1} << bits) - 1) : v;
        };
        std::uint64_t r = 0;
        switch (op.op) {
            case Op::Copy:
            case Op::IntZext: r = in[0]; break;
            case Op::IntSext: r = sx(in[0]); break;
            case Op::IntAdd: r = in[0] + in[1]; break;
            case Op::IntSub: r = in[0] - in[1]; break;
            case Op::IntMult: r = in[0] * in[1]; break;
            case Op::IntAnd: r = in[0] & in[1]; break;
            case Op::IntOr: r = in[0] | in[1]; break;
            case Op::IntXor: r = in[0] ^ in[1]; break;
            case Op::IntNegate: r = ~in[0]; break;
            case Op::Int2Comp: r = 0 - in[0]; break;
            case Op::IntLeft: r = in[1] >= 64 ? 0 : in[0] << in[1]; break;
            case Op::IntRight: r = in[1] >= 64 ? 0 : in[0] >> in[1]; break;
            case Op::IntEqual: r = in[0] == in[1]; break;
            case Op::IntNotEqual: r = in[0] != in[1]; break;
            case Op::IntLess: r = in[0] < in[1]; break;
            case Op::BoolNegate: r = !in[0]; break;
            case Op::Subpiece: r = in[1] >= 8 ? 0 : in[0] >> (8 * in[1]); break;
            default: return std::nullopt;
        }
        return mask(r, os);
    }
};

}  // namespace

Translator::Translator(const std::filesystem::path& sla) : reader_(sla, {}) {
    const auto add = [&](const std::string& name, Kind kind, unsigned index, std::string member = {}) {
        const std::optional<sl::Varnode> v = reader_.register_named(name);
        if (!v) throw std::runtime_error(sla.string() + ": no register " + name);
        cells_.push_back(Cell{kind, v->offset, v->size, index, member.empty() && kind == Kind::Dropped ? name : std::move(member)});
    };
    for (unsigned n = 0; n < 32; ++n) add(std::format("r{}", n), Kind::Gpr, n);
    for (unsigned n = 0; n < 32; ++n) add(std::format("f{}", n), Kind::Fpr, n);
    for (unsigned n = 32; n < 64; ++n) add(std::format("vs{}", n), Kind::Vector, n - 32);   // AltiVec's vr0..vr31
    for (unsigned n = 32; n < 128; ++n) add(std::format("v{}", n), Kind::Vector, n);       // VMX128's own
    for (unsigned n = 0; n < 8; ++n) add(std::format("cr{}", n), Kind::Cr, n);
    add("CTR", Kind::Ctr, 0);
    add("LR", Kind::Lr, 0);
    add("xer_so", Kind::XerBit, 0, "so");
    add("xer_ov", Kind::XerBit, 0, "ov");
    add("xer_ca", Kind::XerBit, 0, "ca");
    add("MSR", Kind::Msr, 0);
    add("XER", Kind::XerWord, 0);
    add("TBLr", Kind::TimeBase, 0);
    add("TBUr", Kind::TimeBase, 1);
    // written by the description, kept by no context: their writes are dropped, a read declines the instruction. (The FPSCR's
    // controls -- the enables, NI and the rounding mode, fp_ve to fp_drn -- are not among them: the context keeps them in ctx.fpscr, so
    // an instruction that writes them, mtfsf, mtfsfi, mtfsb0/1, is XenonRecomp's.)
    for (const char* name : {"xer_ov32", "xer_ca32", "r2Save", "fp_fx", "fp_fex", "fp_vx", "fp_ox", "fp_ux", "fp_zx", "fp_xx", "fp_vxsnan", "fp_vxisi", "fp_vxidi",
                             "fp_vxzdz", "fp_vximz", "fp_vxvc", "fp_fr", "fp_fi", "fp_c", "fp_cc0", "fp_cc1", "fp_cc2", "fp_cc3", "fp_reserve1", "fp_vxsoft", "fp_vxsqrt",
                             "fp_vxcvi"}) {
        add(name, Kind::Dropped, 0);
    }
    std::sort(cells_.begin(), cells_.end(), [](const Cell& a, const Cell& b) { return a.offset < b.offset; });
}

std::string Translator::mnemonic(std::uint32_t address, std::uint32_t word) const {
    const std::uint8_t bytes[4] = {static_cast<std::uint8_t>(word >> 24), static_cast<std::uint8_t>(word >> 16), static_cast<std::uint8_t>(word >> 8),
                                   static_cast<std::uint8_t>(word)};
    return reader_.at(bytes, sizeof bytes, address, address).mnemonic;
}

const Translator::Cell* Translator::cell_at(std::uint64_t offset) const {
    auto it = std::upper_bound(cells_.begin(), cells_.end(), offset, [](std::uint64_t o, const Cell& c) { return o < c.offset; });
    if (it == cells_.begin()) return nullptr;
    --it;
    return offset < it->offset + it->size ? &*it : nullptr;
}

std::optional<Statements> Translator::instruction(std::uint32_t address, std::uint32_t word, std::uint32_t next_word, const Names& names, std::string* why) const {
    if (word == kDb16cyc) return Statements{"\tPPC_SPIN_HINT();\n", false};   // (or r31,r31,r31 changes nothing; the hint lets the runtime yield)
    const std::uint8_t bytes[4] = {static_cast<std::uint8_t>(word >> 24), static_cast<std::uint8_t>(word >> 16), static_cast<std::uint8_t>(word >> 8),
                                   static_cast<std::uint8_t>(word)};
    const sl::Lifted lifted = reader_.pcode(bytes, sizeof bytes, address, address);
    if (lifted.length == 0) {
        // primary opcode 0 is no PowerPC instruction (a word of data the function's bounds take in, or POWER's attn): nothing to do,
        // as XenonRecomp writes it
        if ((word >> 26) == 0) return Statements{"\t// (no instruction: primary opcode 0)\n", false, false};
        if (why) *why = std::format("no instruction of the description ({:08X})", word);
        return std::nullopt;
    }
    const std::function<const Cell*(std::uint64_t)> cells = [this](std::uint64_t offset) { return cell_at(offset); };
    try {
        const bool flow = std::any_of(lifted.ops.begin(), lifted.ops.end(), [address](const sl::PcodeOp& op) {
            return op.op == Op::Call || op.op == Op::CallInd || op.op == Op::BranchInd || op.op == Op::Return ||
                   ((op.op == Op::Branch || op.op == Op::CBranch) && op.inputs[0].kind != sl::Space::Constant &&
                    op.inputs[0].offset != static_cast<std::uint64_t>(address) + 4);
        });
        if (flow && names.jump) {
            // (the recompiler's ways out change its state -- a switch table taken, the flush mode forgotten -- so they are asked only
            // once the instruction is known to be written)
            Names dry = names;
            dry.jump = dry.call = [](std::uint32_t) { return std::string("\t;\n"); };
            dry.call_indirect = dry.jump_indirect = [](const std::string&) { return std::string("\t;\n"); };
            Writer(reader_, cells, dry, address).write(lifted, next_word);
        }
        Writer writer(reader_, cells, names, address);
        return writer.write(lifted, next_word);
    } catch (const Decline& d) {
        if (why) *why = d.why;
        return std::nullopt;
    }
}

}  // namespace cb::recompile::pcode_emit
