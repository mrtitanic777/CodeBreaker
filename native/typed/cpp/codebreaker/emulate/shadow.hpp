// codebreaker/emulate/shadow.hpp -- SHIM over cb/emulate/shadow.hpp (docs/CPP_DESIGN.md 6.3): the old surface (a shadow as an object with py:: members --
// path, trail, vars, lost -- and the x86 shadow factories) for the ports that still use it (verify/lanes/*). Conversion and forwarding only; deleted
// with its last py:: consumer.
//
// ShadowPPC wraps cb::emulate::shadow::PowerPcShadow. The old class's members are plain data the explorer reads after the run, so they are kept up
// to date after each step and call: only what the run made since the last time is converted (one old record per node of the run's pool, so
// the identity of a shared sub-expression holds between conditions). The x86 shadows (Shadow16, Shadow32, ShadowLE) are the recorder's, through the
// old native_shadow::Shadow shim; what the old module had besides (PowerPCView, the general Shadow class, _Stop) nobody outside used.
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>
#include <unordered_map>

#include "cb/compat/py_bridge.hpp"
#include "cb/emulate/shadow.hpp"
#include "codebreaker/core16_types.hpp"
#include "codebreaker/emulate/native_shadow.hpp"
#include "codebreaker/emulate/pcode.hpp"
#include "codebreaker/emulate/powerpc.hpp"
#include "codebreaker/emulate/windows16.hpp"
#include "py.hpp"

namespace codebreaker::emulate::shadow {

namespace nsh = ::cb::emulate::shadow;
using core16::Uc;

using native_shadow::PathEntry;
using native_shadow::Place;
using native_shadow::Step;
using native_shadow::Var;
using native_shadow::Vars;

// conditions kept per run; past it the run is followed no further
inline constexpr int64_t _CAP_PATH = static_cast<int64_t>(nsh::kPathCap);

// The lift mode (emulate/pcode.py) for a platform's cpu slot (its SLEIGH language), or None where there is no lift: "x86-16-real", "x86-32-flat",
// "ppc-be-64".  used by: verify.lanes.driver / powerpc / win32
inline std::optional<py::str> mode(const py::str& cpu) {
    const auto m = nsh::lift_mode(cpu.utf8());
    return m ? std::optional<py::str>(::cb::compat::to_py(::cb::emulate::pcode::mode_name(*m))) : std::nullopt;
}

// The shadow of one PowerPC run (emulate/powerpc.py run, `shadow`): a call out of the code given returns `ret` (64 bits) in r3 and changes nothing else.
// Arguments: r3-r10 (enter).
class ShadowPPC {
  public:
    ShadowPPC(const std::shared_ptr<pcode::Lifter>& lifter_, const std::function<bool(int64_t)>& inputs,
              const std::optional<std::tuple<int64_t, int64_t>>& = std::nullopt)   // (the span: the Python stored it and never read it)
        : lifter(lifter_), impl_(lifter_->impl(), [inputs](uint64_t address) { return inputs(static_cast<int64_t>(address)); }) {}
    ShadowPPC(const ShadowPPC&) = delete;
    ShadowPPC& operator=(const ShadowPPC&) = delete;

    std::shared_ptr<pcode::Lifter> lifter;
    py::list<PathEntry> path;   // the conditions the run's path went by
    py::list<Step> trail;       // every conditional jump, in order
    Vars vars;                  // the inputs the conditions are over
    py::set<py::str> lost;      // why the conditions may not be all there is (empty: they are)

    // The run is about to start: argument k, `bits` wide, is (where, value[, k]).
    void enter(const py::list<Place>& places, int64_t bits = 16) {
        std::vector<::cb::emulate::native_shadow::Place> converted;
        for (const Place& p : places) {
            ::cb::emulate::native_shadow::Place n;
            if (const auto* address = std::get_if<int64_t>(&p.where)) n.where = static_cast<uint64_t>(*address);
            else n.where = std::get<py::str>(p.where).utf8();
            n.value = static_cast<uint64_t>(p.value);
            if (p.k) {
                if (const auto* number = std::get_if<int64_t>(&*p.k)) n.input = *number;
                else n.input = std::get<py::str>(*p.k).utf8();
            }
            converted.push_back(std::move(n));
        }
        ::cb::compat::guard([&] { impl_.enter(converted, static_cast<int>(bits)); });
        sync();
    }
    // Before the instruction at `address` runs (the interpreter's machine, as the old layer holds it).
    void step(powerpc::Machine& m, int64_t address, int64_t = 0) {
        impl_.step(m.impl(), static_cast<uint32_t>(address));
        sync();
    }
    // A call the environment answered: r3 was set to `value`, an input when `symbolic_result`.
    void after_call(powerpc::Machine& m, bool symbolic_result, int64_t value, const std::optional<int64_t>& = std::nullopt, int64_t = 0,
                    const std::optional<py::str>& = std::nullopt, int64_t = 2) {
        impl_.after_call(m.impl(), symbolic_result, static_cast<uint64_t>(value));
        sync();
    }

  private:
    // The members, up to what the run has made.
    void sync() {
        const ::cb::emulate::native_shadow::Run& run = impl_.run();
        for (; paths_ < run.path.size(); ++paths_) {
            const auto& e = run.path[paths_];
            PathEntry o;
            o.site = static_cast<int64_t>(e.site);
            o.taken = e.taken;
            o.cond = native_shadow::shim_detail::old_expr(e.condition, made_);
            o.kind = py::str(native_shadow::shim_detail::path_kind_text(e.kind));
            path.append(o);
        }
        for (; trails_ < run.trail.size(); ++trails_) trail.append(Step(static_cast<int64_t>(run.trail[trails_].site), run.trail[trails_].taken));
        for (auto it = run.vars.begin() + static_cast<std::ptrdiff_t>(vars_); it != run.vars.end(); ++it, ++vars_) {
            const auto& v = it->second;
            Var o;
            o.kind = py::str(native_shadow::shim_detail::var_kind_text(v.kind));
            if (const auto* number = std::get_if<uint64_t>(&v.where)) o.where = static_cast<int64_t>(*number);
            else if (const auto* text = std::get_if<std::string>(&v.where)) o.where = py::str(*text);
            else o.where = py::none_t{};
            o.bits = v.bits;
            o.value = static_cast<int64_t>(v.value);
            vars.set(py::str(it->first), o);
        }
        if (run.lost.size() != losses_) {
            for (const std::string& s : run.lost) lost.add(py::str(s));
            losses_ = run.lost.size();
        }
    }

    nsh::PowerPcShadow impl_;
    size_t paths_ = 0, trails_ = 0, vars_ = 0, losses_ = 0;
    std::unordered_map<const ::cb::emulate::symbolic::Expr*, symbolic::ExprP> made_;
};

// A selector's base in real mode (Shadow16's `segbase` for a real-mode run): sel * 16.
inline int64_t real_mode(Uc&, int64_t sel) { return static_cast<int64_t>(nsh::real_mode_base(static_cast<uint32_t>(sel))); }

// A selector's base for Shadow16: windows16::selector_base (the GDT's) or real_mode -- the only two.
using SegBase = int64_t (*)(Uc&, int64_t);

// --- the x86 shadows: the recorder's, stepped by the recorder of the run they are handed to. `inputs`: [(low, high)] -- the memory bytes that are
// inputs of the run until written.
using Inputs = py::list<std::tuple<int64_t, int64_t>>;
using Span = std::optional<std::tuple<int64_t, int64_t>>;
// A 16-bit run's (windows16::trace_function16): `segbase` -- windows16::selector_base or real_mode (ValueError otherwise).
inline std::shared_ptr<native_shadow::Shadow> Shadow16(const std::shared_ptr<pcode::Lifter>& lifter, SegBase segbase, const Inputs& inputs,
                                                       const Span& span = std::nullopt) {
    if (segbase != &windows16::selector_base && segbase != &real_mode) throw py::ValueError("Shadow16: a selector's base is the GDT's or real mode's");
    native_shadow::require();
    return std::make_shared<native_shadow::Shadow>(16, lifter, inputs, span, segbase == &real_mode);
}
// A flat 32-bit run's (emulator::trace_function).
inline std::shared_ptr<native_shadow::Shadow> Shadow32(const std::shared_ptr<pcode::Lifter>& lifter, const Inputs& inputs, const Span& span = std::nullopt) {
    native_shadow::require();
    return std::make_shared<native_shadow::Shadow>(32, lifter, inputs, span);
}
// A driver's flat ring-0 run's (driver.run): a service call also sets the carry flag.
inline std::shared_ptr<native_shadow::Shadow> ShadowLE(const std::shared_ptr<pcode::Lifter>& lifter, const Inputs& inputs, const Span& span = std::nullopt) {
    native_shadow::require();
    return std::make_shared<native_shadow::Shadow>(33, lifter, inputs, span);
}

}  // namespace codebreaker::emulate::shadow
