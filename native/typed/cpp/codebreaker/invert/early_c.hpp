// codebreaker/invert/early_c.hpp -- SHIM over cb/invert/early_c.hpp (docs/CPP_DESIGN.md 6.3); deleted with its last
// py:: consumer. The old module's settings and hooks are variables read at each call: the shim passes them to the new
// functions as an Options. REGISTER_HASHES is kept for source compatibility and ignored (the new module lists registers in a
// fixed order); flow16_CMPSWITCH is flow16's CMPSWITCH.
#pragma once

#include <array>
#include <functional>
#include <optional>
#include <tuple>
#include <variant>

#include "cb/compat/py_bridge.hpp"
#include "cb/invert/early_c.hpp"
#include "py_os.hpp"

namespace codebreaker::invert::early_c {

namespace ne = cb::invert::early_c;
namespace nf = cb::invert::early_c_frame;

// The target is None, or an int (near) or a tuple of ints (far: here a list).
using CallTarget = std::optional<std::variant<int64_t, py::list<int64_t>>>;
using CallinfoHook = std::function<std::optional<int64_t>(int64_t, const py::str&, const CallTarget&)>;
using CallregsHook = std::function<std::optional<py::list<py::str>>(int64_t, const py::str&, const CallTarget&)>;
using SwitchHook = std::function<std::optional<py::list<int64_t>>(int64_t)>;

// An empty std::function is the Python's None.
inline CallinfoHook CALLINFO;
inline CallregsHook CALLREGS;
inline SwitchHook SWITCH;

inline bool MARK = false;
inline bool FARPTR = ne::Options::from_environment().far_pointers;
inline bool DEADREGS = ne::Options::from_environment().dead_registers;
inline bool UNSIGNED_CAST = ne::Options::from_environment().unsigned_compares;
inline bool FPCASTS = false;
inline std::optional<bool> RETVAL;

// What recover() last found (the Python's LAST dict: only the keys it has).
struct Last {
    std::optional<py::list<py::str>> saved;
    std::optional<py::dict<py::str, int64_t>> regparam;
    std::optional<py::dict<py::str, py::str>> fptypes;
    std::optional<py::str> fpret;
    std::optional<py::set<py::str>> unsigned_longs;
};
inline Last LAST;

inline bool REGVARS_EAGER = false;
inline bool FRAME_AFTER_BP = false;
inline bool REGPARAM_REREAD = true;
inline bool flow16_CMPSWITCH = false;
inline std::array<int64_t, 7> REGISTER_HASHES = {0, 0, 0, 0, 0, 0, 0};   // (ignored)

// (framesize, params, statements, return type, local offsets)
using Recovered = std::tuple<int64_t, py::list<py::str>, py::list<py::str>, py::str, py::list<int64_t>>;

namespace detail {

inline CallTarget target_of(const ne::CallSite& call, std::string& kind) {
    if (const auto* near = std::get_if<ne::NearCall>(&call)) {
        kind = "near";
        if (!near->target) return std::nullopt;
        return CallTarget(std::variant<int64_t, py::list<int64_t>>(static_cast<int64_t>(*near->target)));
    }
    const auto& far = std::get<ne::FarCall>(call);
    kind = "far";
    if (!far.target) return std::nullopt;
    py::list<int64_t> parts;
    parts.append(far.target->seg);
    parts.append(far.target->off);
    return CallTarget(std::variant<int64_t, py::list<int64_t>>(parts));
}

inline int64_t address_of(const ne::CallSite& call) {
    return std::visit([](const auto& c) { return static_cast<int64_t>(c.address); }, call);
}

inline ne::Options options_now() {
    ne::Options o = ne::Options::from_environment();   // MASM_CRET and CFG_NOCAP are read at each call
    o.mark = MARK;
    o.far_pointers = FARPTR;
    o.dead_registers = DEADREGS;
    o.unsigned_compares = UNSIGNED_CAST;
    o.fp_casts = FPCASTS;
    o.eager_register_variables = REGVARS_EAGER;
    o.frame_after_bp = FRAME_AFTER_BP;
    o.reread_register_params = REGPARAM_REREAD;
    o.compare_switches = flow16_CMPSWITCH;
    o.returns_value = RETVAL;
    if (SWITCH) {
        o.hooks.switch_targets = [](std::uint32_t address) -> std::optional<std::vector<std::uint32_t>> {
            const auto answer = SWITCH(static_cast<int64_t>(address));
            if (!answer) return std::nullopt;
            std::vector<std::uint32_t> out;
            for (const int64_t t : *answer) out.push_back(cb::compat::int_from_py<std::uint32_t>(t));
            return out;
        };
    }
    if (CALLINFO) {
        o.hooks.callee_pop_bytes = [](const ne::CallSite& call) -> std::uint32_t {
            std::string kind;
            const CallTarget target = target_of(call, kind);
            const auto answer = CALLINFO(address_of(call), cb::compat::to_py(kind), target);
            return answer && *answer > 0 ? cb::compat::int_from_py<std::uint32_t>(*answer) : 0;   // None and 0 are "no answer"
        };
    }
    if (CALLREGS) {
        o.hooks.callee_register_args = [](const ne::CallSite& call) -> std::vector<ne::Reg> {
            std::string kind;
            const CallTarget target = target_of(call, kind);
            const auto answer = CALLREGS(address_of(call), cb::compat::to_py(kind), target);
            std::vector<ne::Reg> regs;
            if (!answer) return regs;
            for (const py::str& name : *answer) {
                const std::string n = cb::compat::from_py(name);
                if (n == "ax") regs.push_back(ne::Reg::Ax);
                else if (n == "bx") regs.push_back(ne::Reg::Bx);
                else if (n == "cx") regs.push_back(ne::Reg::Cx);
                else if (n == "dx") regs.push_back(ne::Reg::Dx);
                else throw cb::UsageError("early_c: '" + n + "' is not a scratch register a callee reads");
            }
            return regs;
        };
    }
    return o;
}

inline Recovered finish(const ne::Recovered& r) {
    LAST = Last();
    LAST.saved = cb::compat::to_py(r.findings.saved_registers);
    LAST.regparam = cb::compat::to_py(r.findings.register_params);
    if (!r.findings.fp_types.empty()) LAST.fptypes = cb::compat::to_py(r.findings.fp_types);
    if (!r.findings.fp_return.empty()) LAST.fpret = cb::compat::to_py(r.findings.fp_return);
    if (!r.findings.unsigned_longs.empty()) LAST.unsigned_longs = cb::compat::to_py(r.findings.unsigned_longs);
    return Recovered(r.frame_size, cb::compat::to_py(r.params), cb::compat::to_py(r.statements), cb::compat::to_py(r.return_type),
                     cb::compat::to_py(r.locals));
}

inline std::unordered_map<std::uint32_t, std::string> names_of(const std::optional<py::dict<int64_t, py::str>>& cxmap) {
    std::unordered_map<std::uint32_t, std::string> out;
    if (cxmap) {
        for (const auto& kv : cxmap->items()) out[cb::compat::int_from_py<std::uint32_t>(std::get<0>(kv))] = cb::compat::from_py(std::get<1>(kv));
    }
    return out;
}

inline std::vector<nf::Offset> offsets_of(const std::optional<py::list<int64_t>>& locals) {
    std::vector<nf::Offset> out;
    if (locals) {
        for (const int64_t o : *locals) out.push_back(cb::compat::int_from_py<nf::Offset>(o));
    }
    return out;
}

inline std::set<nf::Offset> longs_of(const std::optional<py::set<int64_t>>& longset) {
    std::set<nf::Offset> out;
    if (longset) {
        for (const int64_t o : *longset) out.insert(cb::compat::int_from_py<nf::Offset>(o));
    }
    return out;
}

}  // namespace detail

// (framesize, params, statements, return type, local offsets) by the linear block walker.
inline Recovered recover(const py::bytes& code, int64_t base = 0, const std::optional<py::dict<int64_t, py::str>>& cxmap = std::nullopt) {
    return cb::compat::guard([&] {
        return detail::finish(ne::recover(cb::compat::from_py(code), cb::compat::int_from_py<std::uint32_t>(base), detail::names_of(cxmap),
                                          detail::options_now()));
    });
}

// The same by the structured control-flow graph (flow16), with the text passes after it.
inline Recovered recover_cfg(const py::bytes& code, int64_t base = 0, const std::optional<py::dict<int64_t, py::str>>& cxmap = std::nullopt) {
    return cb::compat::guard([&] {
        return detail::finish(ne::recover_cfg(cb::compat::from_py(code), cb::compat::int_from_py<std::uint32_t>(base), detail::names_of(cxmap),
                                              detail::options_now()));
    });
}

// Live local-to-local copies `vX = vY;` in straight-line stmts, as (dest_off, src_off) pairs.
inline py::list<std::tuple<int64_t, int64_t>> detect_copies(const py::list<py::str>& stmts) {
    return cb::compat::guard([&] {
        py::list<std::tuple<int64_t, int64_t>> out;
        for (const auto& c : ne::detect_copies(cb::compat::from_py(stmts))) out.append(std::make_tuple<int64_t, int64_t>(c.dest, c.src));
        return out;
    });
}

// A 32-bit compare's ladder folded into one comparison.
inline py::list<py::str> _fold_ladders(const py::list<py::str>& lines) {
    return cb::compat::guard([&] { return cb::compat::to_py(ne::fold_ladders(cb::compat::from_py(lines), MARK)); });
}

// The declarations of the locals as a string block.
inline py::str local_decls(int64_t fs, const std::optional<py::list<int64_t>>& locals, const std::optional<py::set<int64_t>>& longset = std::nullopt,
                           const std::optional<py::list<std::tuple<int64_t, int64_t>>>& copies = std::nullopt) {
    return cb::compat::guard([&] {
        std::vector<nf::Copy> cps;
        if (copies) {
            for (const auto& [dest, src] : *copies) {
                cps.push_back({cb::compat::int_from_py<nf::Offset>(dest), cb::compat::int_from_py<nf::Offset>(src)});
            }
        }
        return cb::compat::to_py(ne::local_decls(cb::compat::int_from_py<std::uint32_t>(fs), detail::offsets_of(locals), detail::longs_of(longset), cps));
    });
}

inline py::str to_c(const py::str& name, int64_t fs, const py::list<py::str>& params, const py::list<py::str>& stmts, const py::str& ptype = py::str("int"),
                    const std::optional<py::list<int64_t>>& locals = std::nullopt, const std::optional<py::set<int64_t>>& longset = std::nullopt) {
    return cb::compat::guard([&] {
        return cb::compat::to_py(ne::to_c(cb::compat::from_py(name), cb::compat::int_from_py<std::uint32_t>(fs), cb::compat::from_py(params),
                                          cb::compat::from_py(stmts), cb::compat::from_py(ptype), detail::offsets_of(locals), detail::longs_of(longset)));
    });
}

// argv as sys.argv: [script, image.bin, hexoff, len]; prints the recovered function.
inline void main(const py::list<py::str>& argv) {
    py::bytes img = py::open_rb(argv[1]).read();
    const int64_t off = py::int_from_str(argv[2], 16);
    const int64_t ln = py::int_from_str(argv[3]);
    auto [fs, params, stmts, pt, loc] = recover(img.slice(off, off + ln));
    char name[32];
    std::snprintf(name, sizeof name, "f%05llx", static_cast<unsigned long long>(off));
    py::print(to_c(py::str(name), fs, params, stmts, pt, loc));
}

}  // namespace codebreaker::invert::early_c
