#include "emulate/native_shadow.hpp"

#include <algorithm>
#include <cstring>
#include <format>
#include <map>
#include <mutex>

#include "core/error.hpp"
#include "emulate/pcode.hpp"
#include "emulate/tracer/cbtrace.h"
#include "emulate/unicorn_uc.hpp"

namespace cb::emulate::native_shadow {

namespace {

constexpr std::uint64_t kNoNode = 0xFFFFFFFFFFFFFFFFULL;   // a node's absent child

// The bit of EFLAGS each flag register of the lifter is
const std::map<std::string, int, std::less<>> kFlagBits = {{"CF", 0}, {"PF", 2}, {"AF", 4}, {"ZF", 6}, {"SF", 7},
                                                            {"TF", 8}, {"IF", 9}, {"DF", 10}, {"OF", 11}};

// ---- the lifter the recorder asks -------------------------------------------------------------------------------------------

// The recorder's callback for an instruction it has not lifted before carries no context: one trampoline per slot asks the function
// its slot was last given, and a slot belongs to a lift mode (the answers depend on nothing else). This is the one place that state is
// kept; a slot's function is shared, so a shadow made while another runs does not pull it from under it.
class LiftBridge {
public:
    static LiftBridge& instance() {
        static LiftBridge bridge;
        return bridge;
    }

    // The trampoline for `mode`, asking `line` from now on. UsageError when more modes than slots are asked for.
    cbt_sh_lift attach(const std::string& mode, LineFn line) {
        const std::lock_guard<std::mutex> hold(lock_);
        std::size_t slot = kSlots;
        for (std::size_t i = 0; i < kSlots; ++i)
            if (slots_[i].line && slots_[i].mode == mode) slot = i;
        for (std::size_t i = 0; slot == kSlots && i < kSlots; ++i)
            if (!slots_[i].line) slot = i;
        if (slot == kSlots) throw UsageError("native_shadow: more lift modes than the recorder keeps callbacks for");
        slots_[slot].mode = mode;
        slots_[slot].line = std::make_shared<const LineFn>(std::move(line));
        return kTrampolines[slot];
    }

    // The answer a callback gives the recorder: pcodecheck's line, its length (more than `cap - 1`: ask again with room), or -2 with
    // the name of what was thrown.
    int ask(std::size_t slot, const std::uint8_t* code, std::int32_t n, std::uint64_t offset, char* out, std::int32_t cap) {
        std::shared_ptr<const LineFn> line;
        {
            const std::lock_guard<std::mutex> hold(lock_);
            line = slots_[slot].line;
        }
        try {
            const std::string text = (*line)(ByteView(code, static_cast<std::size_t>(n)), offset);
            if (static_cast<std::int64_t>(text.size()) < cap) std::memcpy(out, text.c_str(), text.size() + 1);
            return static_cast<int>(text.size());
        } catch (const Error& e) {
            return failed(e.kind(), out, cap);
        } catch (...) {
            return failed("Exception", out, cap);
        }
    }

private:
    static constexpr std::size_t kSlots = 4;
    struct Slot {
        std::string mode;
        std::shared_ptr<const LineFn> line;
    };

    static int failed(std::string_view name, char* out, std::int32_t cap) {
        const std::size_t n = std::min<std::size_t>(name.size(), static_cast<std::size_t>(cap - 1));
        std::memcpy(out, name.data(), n);
        out[n] = 0;
        return -2;
    }
    template <std::size_t N>
    static int trampoline(const std::uint8_t* code, std::int32_t n, std::uint64_t offset, char* out, std::int32_t cap) {
        return instance().ask(N, code, n, offset, out, cap);
    }
    static constexpr cbt_sh_lift kTrampolines[kSlots] = {&trampoline<0>, &trampoline<1>, &trampoline<2>, &trampoline<3>};

    std::mutex lock_;
    Slot slots_[kSlots];
};

// ---- reading the recorder's tables back -------------------------------------------------------------------------------------

constexpr std::size_t kNodeWords = 8, kPathWords = 4, kTrailWords = 2, kVarWords = 7;

// `n` records of `words` words: record i is the span.
std::span<const std::uint64_t> record(std::span<const std::uint64_t> table, std::size_t words, std::size_t i) {
    return table.subspan(i * words, words);
}

[[noreturn]] void lies(std::string_view table, std::size_t index, const std::string& what) {
    throw FormatError("native_shadow", index, std::format("{} {}: {}", table, index, what));
}

std::size_t checked_count(std::span<const std::uint64_t> table, std::size_t words, std::string_view name) {
    if (table.size() % words != 0) lies(name, table.size(), std::format("the table is not whole records of {} words", words));
    return table.size() / words;
}

std::vector<std::string_view> split_strings(std::string_view all, std::int64_t count) {
    std::vector<std::string_view> strings;
    if (all.empty()) return strings;
    std::size_t at = 0;
    for (;;) {
        const std::size_t end = all.find('\0', at);
        strings.push_back(all.substr(at, end == std::string_view::npos ? std::string_view::npos : end - at));
        if (end == std::string_view::npos) break;
        at = end + 1;
    }
    if (static_cast<std::int64_t>(strings.size()) > count) strings.resize(static_cast<std::size_t>(std::max<std::int64_t>(count, 0)));
    return strings;
}

symbolic::Op op_at(std::uint64_t word, std::size_t node) {
    if (word > static_cast<std::uint64_t>(symbolic::Op::True)) lies("node", node, std::format("the operation {} is not one", word));
    return static_cast<symbolic::Op>(word);
}

// A node of the recorder's table as the pool's expression of it, over the ones made before it.
const symbolic::Expr* make_node(symbolic::ExprPool& pool, const std::vector<const symbolic::Expr*>& nodes, std::span<const std::uint64_t> w,
                                std::size_t index, const std::vector<std::string_view>& strings) {
    using symbolic::Op;
    const Op op = op_at(w[0], index);
    const std::uint64_t bits64 = w[1];
    if (bits64 > 0xFFFFFFFFULL) lies("node", index, "a width that is not one");
    const auto bits = static_cast<std::uint32_t>(bits64);
    auto child = [&](std::uint64_t k) -> const symbolic::Expr* {
        if (k == kNoNode || k >= nodes.size()) lies("node", index, std::format("it is made of node {}, which is not before it", k));
        return nodes[k];
    };
    switch (op) {
        case Op::Var:
            if (w[6] >= strings.size()) lies("node", index, "its name is not in the strings");
            return pool.var(strings[w[6]], bits);
        case Op::Const:
            if (w[7] != 0) lies("node", index, "a constant wider than 64 bits");
            return pool.constant(w[6], bits);   // (a 64-bit constant keeps its bit pattern)
        case Op::Add: case Op::Sub: case Op::And: case Op::Or: case Op::Xor: case Op::Mul: case Op::UDiv: case Op::URem: case Op::SDiv:
        case Op::SRem: case Op::Shl: case Op::LShr: case Op::AShr:
            return pool.binary(op, child(w[2]), child(w[3]));
        case Op::Neg: case Op::Not:
            return pool.unary(op, child(w[2]));
        case Op::ZExt: return pool.zext(child(w[2]), bits);
        case Op::SExt: return pool.sext(child(w[2]), bits);
        case Op::Extract: return pool.extract(child(w[2]), static_cast<std::uint32_t>(w[5]), bits);
        case Op::Concat: return pool.concat(child(w[2]), child(w[3]));
        case Op::Ite: return pool.ite(child(w[2]), child(w[3]), child(w[4]));
        case Op::PopCount: return pool.popcount(child(w[2]), bits);
        case Op::Bool: return pool.to_bool(child(w[2]));
        case Op::Nz: return pool.nonzero(child(w[2]));
        case Op::Eq: case Op::ULt: case Op::ULe: case Op::SLt: case Op::SLe:
            return pool.compare(op, child(w[2]), child(w[3]));
        case Op::BNot: return pool.negate(child(w[2]));
        case Op::BAnd: case Op::BOr: case Op::BXor: return pool.logic(op, child(w[2]), child(w[3]));
        case Op::True: return pool.true_condition();
    }
    lies("node", index, "the operation is not handled");
}

}  // namespace

std::shared_ptr<const Run> decode_run(const RawRun& raw) {
    auto run = std::make_shared<Run>();
    const std::vector<std::string_view> strings = split_strings(raw.strings, raw.string_count);
    auto string_at = [&](std::uint64_t i, std::string_view table, std::size_t row) -> std::string {
        if (i >= strings.size()) lies(table, row, std::format("the string {} is not there", i));
        return std::string(strings[i]);
    };
    std::vector<const symbolic::Expr*> nodes;
    const std::size_t node_count = checked_count(raw.nodes, kNodeWords, "nodes");
    nodes.reserve(node_count);
    for (std::size_t i = 0; i < node_count; ++i) nodes.push_back(make_node(run->pool, nodes, record(raw.nodes, kNodeWords, i), i, strings));

    const std::size_t path_count = checked_count(raw.path, kPathWords, "path");
    for (std::size_t i = 0; i < path_count; ++i) {
        const auto w = record(raw.path, kPathWords, i);
        if (w[3] > static_cast<std::uint64_t>(PathKind::Pin)) lies("path", i, "the kind is not one");
        if (w[2] >= nodes.size()) lies("path", i, "the condition is not a node");
        run->path.push_back({w[0], w[1] != 0, nodes[w[2]], static_cast<PathKind>(w[3])});
    }
    const std::size_t trail_count = checked_count(raw.trail, kTrailWords, "trail");
    for (std::size_t i = 0; i < trail_count; ++i) {
        const auto w = record(raw.trail, kTrailWords, i);
        run->trail.push_back({w[0], w[1] != 0});
    }
    const std::size_t var_count = checked_count(raw.vars, kVarWords, "vars");
    for (std::size_t i = 0; i < var_count; ++i) {
        const auto w = record(raw.vars, kVarWords, i);
        if (w[1] > static_cast<std::uint64_t>(VarKind::Carry)) lies("vars", i, "the kind is not one");
        Var v;
        v.kind = static_cast<VarKind>(w[1]);
        if (w[2] == 1) v.where = w[3];
        else if (w[2] == 2) v.where = string_at(w[3], "vars", i);
        v.bits = static_cast<std::uint32_t>(w[4]);
        v.value = w[5];
        run->vars.insert_or_assign(string_at(w[0], "vars", i), std::move(v));
    }
    for (std::size_t i = 0; i < raw.lost.size(); ++i) run->lost.insert(string_at(raw.lost[i], "lost", i));
    run->pins = raw.pins;
    run->steps = raw.steps;
    run->off = raw.off != 0;
    return run;
}

Setup Setup::for_lifter(Kind kind, pcode::Lifter& lifter, std::vector<std::pair<std::uint64_t, std::uint64_t>> inputs,
                        std::optional<std::pair<std::uint64_t, std::uint64_t>> span, bool real_mode) {
    Setup s;
    s.kind = kind;
    s.mode = std::string(pcode::mode_name(lifter.mode()));
    for (const auto& [name, slot] : lifter.registers()) s.registers.push_back({name, slot.offset, slot.size});
    s.line = [&lifter](ByteView code, std::uint64_t offset) { return lifter.line(code, offset); };
    s.inputs = std::move(inputs);
    s.span = span;
    s.real_mode = real_mode;
    return s;
}

// ---- the shadow -----------------------------------------------------------------------------------------------------------------

Shadow::Shadow(Setup setup) : setup_(std::move(setup)) {
    const std::size_t n = setup_.registers.size();
    std::vector<const char*> names;
    std::vector<std::int64_t> offsets, sizes;
    std::vector<std::int32_t> uc, flag;
    for (const RegisterSpec& r : setup_.registers) {
        names.push_back(r.name.c_str());
        offsets.push_back(static_cast<std::int64_t>(r.offset));
        sizes.push_back(r.size);
        const auto bit = kFlagBits.find(r.name);
        flag.push_back(bit != kFlagBits.end() ? bit->second : -1);
        const std::optional<int> id = bit != kFlagBits.end() ? std::nullopt : unicorn_uc::x86_register(r.name);
        uc.push_back(id ? *id : -1);
    }
    std::vector<std::uint64_t> inputs;
    for (const auto& [low, high] : setup_.inputs) {
        inputs.push_back(low);
        inputs.push_back(high);
    }
    // (the recorder's arrays are at least one long)
    if (n == 0) {
        names.push_back(nullptr);
        offsets.push_back(0);
        sizes.push_back(0);
        uc.push_back(0);
        flag.push_back(0);
    }
    if (inputs.empty()) inputs.push_back(0);

    cbt_sh_config config{};
    config.kind = static_cast<std::int32_t>(setup_.kind);
    config.mode = setup_.mode.c_str();
    config.nregs = static_cast<std::int32_t>(n);
    config.reg_names = names.data();
    config.reg_off = offsets.data();
    config.reg_size = sizes.data();
    config.reg_uc = uc.data();
    config.reg_flag = flag.data();
    config.eflags_id = *unicorn_uc::x86_register("EFLAGS");
    config.inputs = inputs.data();
    config.ninputs = static_cast<std::int64_t>(setup_.inputs.size());
    config.gdt_addr = setup_.gdt_addr;
    config.real_mode = setup_.real_mode ? 1 : 0;
    config.lift = LiftBridge::instance().attach(setup_.mode, setup_.line);
    shadow_ = cbt_sh_new(&config);   // (it copies what it is given)
    CB_ASSERT(shadow_ != nullptr, "the recorder made no shadow");
}

Shadow::~Shadow() {
    if (shadow_ != nullptr) cbt_sh_free(shadow_);
}

void* Shadow::handle() {
    read_.reset();   // (the recorder steps it from here on)
    return shadow_;
}

void Shadow::enter(std::span<const Place> places, std::int32_t bits) {
    read_.reset();
    std::int64_t position = 0;
    for (const Place& item : places) {
        const std::variant<std::int64_t, std::string> input = item.input ? *item.input : std::variant<std::int64_t, std::string>(position);
        ++position;
        std::string name;
        std::int32_t kind = 0;
        std::int64_t number = 0;
        std::optional<std::string> register_name;
        if (const auto* n = std::get_if<std::int64_t>(&input)) {
            number = *n;
            name = "a" + std::to_string(number);
        } else {
            name = std::get<std::string>(input);
            kind = 1;
            register_name = name;
        }
        const char* where_text = register_name ? register_name->c_str() : nullptr;
        if (const auto* reg = std::get_if<std::string>(&item.where)) {
            const auto spec = std::find_if(setup_.registers.begin(), setup_.registers.end(), [&](const RegisterSpec& r) { return r.name == *reg; });
            if (spec == setup_.registers.end()) throw UsageError("the lifter has no register called " + *reg);
            cbt_sh_enter(shadow_, name.c_str(), kind, number, where_text, item.value, bits, 1, spec->offset, spec->size);
        } else {
            cbt_sh_enter(shadow_, name.c_str(), kind, number, where_text, item.value, bits, 0, std::get<std::uint64_t>(item.where), 0);
        }
    }
}

void Shadow::step(core16::Uc& machine, std::uint64_t address, std::uint64_t cs_base) {
    read_.reset();
    cbt_sh_step(shadow_, machine.handle(), address, cs_base);
}

void Shadow::after_call(core16::Uc& machine, const CallResult& result) {
    read_.reset();
    cbt_sh_after_call(shadow_, machine.handle(), result.symbolic ? 1 : 0, result.value, result.args_at ? 1 : 0, result.args_at.value_or(0),
                      result.args, result.why.empty() ? nullptr : result.why.c_str(), result.word, result.carry);
}

void Shadow::written(std::uint64_t at, std::uint64_t size) {
    read_.reset();
    cbt_sh_written(shadow_, at, size);
}

void Shadow::forget(std::span<const std::string> names) {
    read_.reset();
    for (const std::string& name : names) cbt_sh_forget(shadow_, name.c_str());
}

std::uint64_t Shadow::segbase(core16::Uc& machine, std::uint64_t selector) { return cbt_sh_segbase(shadow_, machine.handle(), selector); }

std::shared_ptr<const Run> Shadow::run() {
    if (read_) return read_;
    const cbt_sh_result& r = *cbt_sh_export(shadow_);
    auto table = [](const std::uint64_t* p, std::int64_t records, std::size_t words) {
        return std::span<const std::uint64_t>(p, static_cast<std::size_t>(records) * words);
    };
    RawRun raw;
    raw.nodes = table(r.nodes, r.nnodes, kNodeWords);
    raw.path = table(r.path, r.npath, kPathWords);
    raw.trail = table(r.trail, r.ntrail, kTrailWords);
    raw.vars = table(r.vars, r.nvars, kVarWords);
    raw.lost = table(r.lost, r.nlost, 1);
    raw.strings = std::string_view(r.strings, static_cast<std::size_t>(r.strings_len));
    raw.string_count = r.nstrings;
    raw.pins = r.pins;
    raw.steps = r.steps;
    raw.off = r.off;
    read_ = decode_run(raw);
    return read_;
}

}  // namespace cb::emulate::native_shadow
