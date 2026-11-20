#include "invert/early_c.hpp"

#include <algorithm>
#include <climits>
#include <format>
#include <limits>

#include "core/error.hpp"
#include "core/fs.hpp"
#include "core/text.hpp"
#include "invert/early_c_model.hpp"

namespace cb::invert::early_c {

namespace {

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

early_c_model::Hooks model_hooks(const Hooks& hooks) {
    early_c_model::Hooks h;
    if (hooks.switch_targets) {
        h.switch_targets = [&hooks](long long address, std::vector<long long>& out) {
            const auto targets = hooks.switch_targets(checked_offset<std::uint32_t>(address, "switch address"));
            if (!targets) return false;
            out.assign(targets->begin(), targets->end());
            return true;
        };
    }
    if (hooks.callee_pop_bytes) {
        h.callee_pop_bytes = [&hooks](long long address, const std::string& kind, const std::string& target, long long& pops) {
            const std::uint32_t n = hooks.callee_pop_bytes(call_site(address, kind, target));
            if (n == 0) return false;
            pops = n;
            return true;
        };
    }
    if (hooks.callee_register_args) {
        h.callee_registers = [&hooks](long long address, const std::string& kind, const std::string& target, std::vector<std::string>& regs) {
            const std::vector<Reg> args = hooks.callee_register_args(call_site(address, kind, target));
            if (args.empty()) return false;
            regs.clear();
            for (const Reg r : args) regs.push_back(register_name(r));
            return true;
        };
    }
    return h;
}

early_c_model::Settings model_settings(const Options& o) {
    early_c_model::Settings c;
    c.mark = o.mark;
    c.far_pointers = o.far_pointers;
    c.dead_registers = o.dead_registers;
    c.unsigned_compares = o.unsigned_compares;
    c.fp_casts = o.fp_casts;
    c.eager_register_variables = o.eager_register_variables;
    c.frame_after_bp = o.frame_after_bp;
    c.reread_register_params = o.reread_register_params;
    c.caller_cleans = o.caller_cleans;
    c.compare_switches = o.compare_switches;
    c.uncapped_structures = o.uncapped_structures;
    c.returns_value = o.returns_value ? (*o.returns_value ? 1 : 0) : -1;
    c.hooks = model_hooks(o.hooks);
    return c;
}

Findings findings_of(const early_c_model::Findings& last) {
    Findings f;
    f.saved_registers = last.saved;
    for (const auto& [reg, index] : last.register_params) {
        f.register_params.insert_or_assign(reg, checked_offset<std::uint32_t>(index, "register parameter index"));
    }
    for (const auto& [name, type] : last.fp_types) f.fp_types.insert_or_assign(name, type);
    f.fp_return = last.fp_return;
    f.unsigned_longs.insert(last.unsigned_longs.begin(), last.unsigned_longs.end());
    return f;
}

Recovered recover_with(bool structured, ByteView code, std::uint32_t base,
                       const std::unordered_map<std::uint32_t, std::string>& helper_operators, const Options& options) {
    const early_c_model::Settings settings = model_settings(options);
    const std::vector<std::uint8_t> bytes(code.begin(), code.end());
    std::map<long long, std::string> names;
    for (const auto& [address, name] : helper_operators) names.emplace(address, name);
    early_c_model::Findings last;
    const early_c_model::Recovery r = structured ? early_c_model::recover_cfg(bytes, base, names, settings, last)
                                                 : early_c_model::recover(bytes, base, names, settings, last);
    Recovered out;
    out.frame_size = checked_offset<std::uint32_t>(r.frame_size, "frame size");
    out.params = r.params;
    out.statements = r.statements;
    out.return_type = r.return_type;
    for (const long long off : r.locals) out.locals.push_back(checked_offset<early_c_frame::Offset>(off, "local offset"));
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
    const auto pairs = early_c_model::detect_copies(std::vector<std::string>(statements.begin(), statements.end()));
    std::vector<early_c_frame::Copy> out;
    for (const auto& [dest, src] : pairs) {
        out.push_back({checked_offset<early_c_frame::Offset>(dest, "copy destination"),
                       checked_offset<early_c_frame::Offset>(src, "copy source")});
    }
    return out;
}

std::vector<std::string> fold_ladders(std::span<const std::string> lines, bool mark) {
    return early_c_model::fold_ladders(std::vector<std::string>(lines.begin(), lines.end()), mark);
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
