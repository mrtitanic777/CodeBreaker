#include "cb/invert/early_c.hpp"

#include <algorithm>
#include <climits>
#include <format>
#include <limits>

#include "cb/core/capstone_api.hpp"
#include "cb/core/error.hpp"
#include "cb/core/fs.hpp"
#include "cb/core/text.hpp"
#include "early_c.h"   // native/invert: cbinv::ec_recover, ec_recover_cfg, ec_detect_copies, ec_fold_ladders
#include "flow16.h"    // cbinv::PyError

namespace cb::invert::early_c {

namespace {

constexpr std::string_view kReader = "early_c";

// The model decodes with Capstone through the DLL's path; loaded once.
void ensure_capstone() {
    static const bool loaded = [] {
        const std::filesystem::path path = disasm::capstone::library().path();
        std::string error;
        if (!cbinv::cs_load(path.c_str(), error)) throw IoError(path, 0, error);
        return true;
    }();
    (void)loaded;
}

// What the model raised where the Python raised an exception (kinds 1 IndexError, 2 ValueError, 3 RuntimeError, 4 KeyError,
// 5 AssertionError, 6 TypeError, 7 AttributeError), by meaning.
[[noreturn]] void raise_model_error(const cbinv::PyError& e) {
    switch (e.kind) {
        case 1:
        case 2:
        case 4:
            throw FormatError(kReader, 0, e.msg);   // the bytes are not code of the modelled compiler
        case 3:
            throw UnsupportedError(kReader, 0, e.msg);
        default:
            throw InternalError("early_c model: " + e.msg);
    }
}

template <class F>
auto guarded(F&& f) -> decltype(f()) {
    try {
        return f();
    } catch (const cbinv::PyError& e) {
        raise_model_error(e);
    }
}

template <class T>
T checked_offset(long long v, const char* what) {
    if (v < 0 || v > static_cast<long long>(std::numeric_limits<T>::max())) {
        throw InternalError(std::format("early_c model: {} {} is outside the frame", what, v));
    }
    return static_cast<T>(v);
}

std::string register_name(Reg r) {
    switch (r) {
        case Reg::Ax: return "ax";
        case Reg::Bx: return "bx";
        case Reg::Cx: return "cx";
        case Reg::Dx: return "dx";
    }
    throw InternalError("early_c: unknown register");
}

// The model hands a call's kind and target as text: "near" with an offset or "", "far" with "segment,offset" or "".
CallSite call_site(long long address, const std::string& kind, const std::string& target) {
    const std::uint32_t at = checked_offset<std::uint32_t>(address, "call address");
    if (kind == "far") {
        FarCall far{at, std::nullopt};
        const auto comma = target.find(',');
        if (comma != std::string::npos) {
            const auto seg = text::parse_int<std::uint16_t>(target.substr(0, comma));
            const auto off = text::parse_int<std::uint16_t>(target.substr(comma + 1));
            if (seg && off) far.target = Far16{*seg, *off};
        }
        return far;
    }
    return NearCall{at, text::parse_int<std::uint32_t>(target)};
}

cbinv::EcHooks model_hooks(const Hooks& hooks) {
    cbinv::EcHooks h;
    if (hooks.switch_targets) {
        h.sw = [&hooks](long long address, std::vector<long long>& out) -> int {
            const auto targets = hooks.switch_targets(checked_offset<std::uint32_t>(address, "switch address"));
            if (!targets) return 0;
            out.assign(targets->begin(), targets->end());
            return 1;
        };
    }
    if (hooks.callee_pop_bytes) {
        h.callinfo = [&hooks](long long address, const std::string& kind, const std::string& target, long long& pops) -> int {
            const std::uint32_t n = hooks.callee_pop_bytes(call_site(address, kind, target));
            if (n == 0) return 0;
            pops = n;
            return 1;
        };
    }
    if (hooks.callee_register_args) {
        h.callregs = [&hooks](long long address, const std::string& kind, const std::string& target,
                              std::vector<std::string>& regs) -> int {
            const std::vector<Reg> args = hooks.callee_register_args(call_site(address, kind, target));
            if (args.empty()) return 0;
            regs.clear();
            for (const Reg r : args) regs.push_back(register_name(r));
            return 1;
        };
    }
    return h;
}

cbinv::EcCfg model_config(const Options& o) {
    cbinv::EcCfg c;
    c.mark = o.mark;
    c.farptr = o.far_pointers;
    c.deadregs = o.dead_registers;
    c.unsigned_cast = o.unsigned_compares;
    c.fpcasts = o.fp_casts;
    c.regvars_eager = o.eager_register_variables;
    c.frame_after_bp = o.frame_after_bp;
    c.regparam_reread = o.reread_register_params;
    c.masm_cret = o.caller_cleans;
    c.cmpswitch = o.compare_switches;
    c.nocap = o.uncapped_structures;
    c.retval = o.returns_value ? (*o.returns_value ? 1 : 0) : -1;
    // The registers written by an instruction are listed in the model's own order, ax bx cx dx si di es: the model
    // places each name in a small table by its hash, and the names' "hashes" 0..6 make that order the table's.
    for (int k = 0; k < 7; ++k) c.hash[k] = k;
    c.hooks = model_hooks(o.hooks);
    return c;
}

Findings findings_of(const cbinv::EcLast& last) {
    Findings f;
    f.saved_registers = last.saved;
    for (const auto& [reg, index] : last.regparam) {
        f.register_params.insert_or_assign(reg, checked_offset<std::uint32_t>(index, "register parameter index"));
    }
    for (const auto& [name, type] : last.fptypes) f.fp_types.insert_or_assign(name, type);
    f.fp_return = last.fpret;
    f.unsigned_longs.insert(last.unsigned_longs.begin(), last.unsigned_longs.end());
    return f;
}

Recovered recover_with(bool structured, ByteView code, std::uint32_t base,
                       const std::unordered_map<std::uint32_t, std::string>& helper_operators, const Options& options) {
    const cbinv::EcCfg config = model_config(options);
    const std::vector<std::uint8_t> bytes(code.begin(), code.end());
    std::map<long long, std::string> names;
    for (const auto& [address, name] : helper_operators) names.emplace(address, name);
    cbinv::EcLast last;
    ensure_capstone();
    const cbinv::EcResult r = guarded([&] {
        return structured ? cbinv::ec_recover_cfg(bytes, base, names, config, last)
                          : cbinv::ec_recover(bytes, base, names, config, last);
    });
    Recovered out;
    out.frame_size = checked_offset<std::uint32_t>(r.fs, "frame size");
    out.params = r.params;
    out.statements = r.stmts;
    out.return_type = r.pt;
    for (const long long off : r.loc) out.locals.push_back(checked_offset<early_c_frame::Offset>(off, "local offset"));
    out.findings = findings_of(last);
    return out;
}

}  // namespace

Options Options::from_environment() {
    Options o;
    auto set = [](std::string_view name) { return fs::get_env(name).value_or("") == "1"; };
    auto present = [](std::string_view name) { return !fs::get_env(name).value_or("").empty(); };
    o.far_pointers = set("MC2INV_FAR");
    o.dead_registers = set("MC2INV_DEADREGS");
    o.unsigned_compares = set("MC2INV_UNSIGNED");
    o.caller_cleans = present("MASM_CRET");
    o.uncapped_structures = present("CFG_NOCAP");
    return o;
}

Recovered recover(ByteView code, std::uint32_t base, const std::unordered_map<std::uint32_t, std::string>& helper_operators,
                  const Options& options) {
    return recover_with(false, code, base, helper_operators, options);
}

Recovered recover_cfg(ByteView code, std::uint32_t base, const std::unordered_map<std::uint32_t, std::string>& helper_operators,
                      const Options& options) {
    return recover_with(true, code, base, helper_operators, options);
}

std::vector<early_c_frame::Copy> detect_copies(std::span<const std::string> statements) {
    const auto pairs = guarded([&] { return cbinv::ec_detect_copies(std::vector<std::string>(statements.begin(), statements.end())); });
    std::vector<early_c_frame::Copy> out;
    for (const auto& [dest, src] : pairs) {
        out.push_back({checked_offset<early_c_frame::Offset>(dest, "copy destination"),
                       checked_offset<early_c_frame::Offset>(src, "copy source")});
    }
    return out;
}

std::vector<std::string> fold_ladders(std::span<const std::string> lines, bool mark) {
    return guarded([&] { return cbinv::ec_fold_ladders(std::vector<std::string>(lines.begin(), lines.end()), mark); });
}

std::string local_decls(std::uint32_t frame_size, std::span<const early_c_frame::Offset> locals,
                        const std::set<early_c_frame::Offset>& longs, std::span<const early_c_frame::Copy> copies) {
    std::string out;
    for (const early_c_frame::Decl& d : early_c_frame::emit_local_decls(locals, frame_size, longs, copies)) {
        out += "    " + early_c_frame::format_decl(d) + "\n";
    }
    return out;
}

std::string to_c(std::string_view name, std::uint32_t frame_size, std::span<const std::string> params,
                 std::span<const std::string> statements, std::string_view return_type,
                 std::span<const early_c_frame::Offset> locals, const std::set<early_c_frame::Offset>& longs) {
    const std::string joined = text::join(params, ", ");
    std::string out = std::format("{}({})\n", name, joined);
    if (!params.empty()) out += std::format("{} {};\n", return_type, joined);
    out += "{\n";
    const std::vector<early_c_frame::Copy> copies = detect_copies(statements);
    out += local_decls(frame_size, locals, longs, copies);
    std::vector<std::string> body;
    for (const std::string& s : statements) body.push_back("    " + s);
    out += text::join(body, "\n");
    out += "\n}\n";
    return out;
}

}  // namespace cb::invert::early_c
