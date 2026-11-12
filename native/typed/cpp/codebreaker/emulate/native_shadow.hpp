// codebreaker/emulate/native_shadow.hpp -- COMPAT SHIM (docs/CPP_DESIGN.md 6.3). The module now lives in native/cb/emulate/native_shadow.{hpp,cpp}; this
// header keeps the old py:: surface for the ports and the dynamic exe that still use it: the Shadow class over the old pcode::Lifter and
// old Uc, and the records a run's shadow hands back (PathEntry, Var, Step, Place; shared with emulate/shadow.hpp's PowerPC shadow).
// Conversion and forwarding only.
//
// Like the old port, a shadow is made only once the recorder's environment is set (native_trace::_load(): the Unicorn and Capstone DLLs the
// recorder is pointed at), since the recorder it steps calls those. The old Uc handed to a step is asked for its engine's handle only.
// The conditions are old symbolic::Expr records made from the pool's nodes (one record per node of one read-back, so identity holds).
// COMPAT(py): remove with the last of emulate/shadow, emulator, windows16, driver.
#pragma once

#include <cstdint>
#include <functional>
#include <memory>
#include <optional>
#include <tuple>
#include <unordered_map>
#include <variant>

#include "cb/compat/py_bridge.hpp"
#include "cb/emulate/native_shadow.hpp"
#include "codebreaker/core16_types.hpp"
#include "codebreaker/emulate/native_trace.hpp"
#include "codebreaker/emulate/pcode.hpp"
#include "codebreaker/emulate/symbolic.hpp"
#include "py.hpp"

namespace codebreaker::emulate::native_shadow {

namespace ns = ::cb::emulate::native_shadow;

using core16::Uc;

// shadow.path: one condition the run's path went by.  kind: "jump" (a conditional jump: taken or not), "inner" (one
// inside an instruction's p-code: a string instruction's count), "pin" (an address or selector held at its value).
struct PathEntry {
    int64_t site = 0;
    bool taken = false;
    symbolic::ExprP cond;
    py::str kind;
};
// shadow.trail: (site, taken) of every conditional jump, in order
using Step = std::tuple<int64_t, bool>;
// shadow.vars[name] = (kind, where, bits, value): kind "arg" (where: the argument's index), "reg" (where: the
// register's name), "mem" (where: the address), "ret" (what an unmodelled call returned; where None) or "carry".
struct Var {
    py::str kind;
    std::variant<py::none_t, int64_t, py::str> where;   // (None, an index/address, or a register name)
    int64_t bits = 0;
    int64_t value = 0;
};
using Vars = py::dict<py::str, Var>;

// One argument place for enter(): Python's `(where, value)` or `(where, value, k)`.
struct Place {
    std::variant<int64_t, py::str> where;
    int64_t value = 0;
    std::optional<std::variant<int64_t, py::str>> k;
};

// The shadow for this process: the DLL loads here (require() raises with why_not() when it does not).
inline bool enabled() { return native_trace::_load() != nullptr; }
inline std::optional<py::str> why_not() { return native_trace::why_not(); }

class Shadow;
// The recorder's handle of a shadow it can step itself, or None. Marks the shadow's cached export stale.
inline void* handle_of(Shadow* shadow);

namespace shim_detail {

// An engine that is only its handle: what a shadow asks of the machine it is stepped on.
class HandleOnly final : public ::cb::core16::Uc {
  public:
    explicit HandleOnly(void* handle) : handle_(handle) {}
    void* handle() const override { return handle_; }
    void mem_map(std::uint64_t, std::uint64_t) override { unsupported(); }
    void mem_unmap(std::uint64_t, std::uint64_t) override { unsupported(); }
    ::cb::Bytes mem_read(std::uint64_t, std::uint64_t) override { unsupported(); }
    void mem_write(std::uint64_t, ::cb::ByteView) override { unsupported(); }
    std::uint64_t reg_read(int) override { unsupported(); }
    double reg_read_float(int) override { unsupported(); }
    void reg_write(int, std::uint64_t) override { unsupported(); }
    void reg_write_descriptor(int, const Descriptor&) override { unsupported(); }
    std::unique_ptr<Context> context_save() override { unsupported(); }
    void context_restore(const Context&) override { unsupported(); }
    void ctl_flush_tb() override { unsupported(); }
    void emu_start(std::uint64_t, std::uint64_t, std::uint64_t, std::uint64_t) override { unsupported(); }
    void emu_stop() override { unsupported(); }
    void hook_add_code(std::function<void(::cb::core16::Uc&, std::uint64_t, std::uint32_t)>, std::uint64_t, std::uint64_t) override { unsupported(); }
    void hook_add_insn_in(std::function<std::uint32_t(::cb::core16::Uc&, std::uint32_t, std::uint32_t)>) override { unsupported(); }
    void hook_add_insn_out(std::function<void(::cb::core16::Uc&, std::uint32_t, std::uint32_t, std::uint32_t)>) override { unsupported(); }

  private:
    [[noreturn]] static void unsupported() { throw py::NotImplementedError("a shadow steps on its engine's handle only"); }
    void* handle_;
};

// The old record of a node of a read-back (made once per node, so identity is kept between conditions that share one).
inline symbolic::ExprP old_expr(const ::cb::emulate::symbolic::Expr* n, std::unordered_map<const ::cb::emulate::symbolic::Expr*, symbolic::ExprP>& made) {
    namespace cbs = ::cb::emulate::symbolic;
    const auto it = made.find(n);
    if (it != made.end()) return it->second;
    auto e = std::make_shared<symbolic::Expr>();
    e->op = py::str(std::string(cbs::to_text(n->op)));
    e->bits = n->bits;
    if (n->op == cbs::Op::Var) {
        e->name = py::str(std::string(n->name));
    } else if (n->op == cbs::Op::Const) {
        e->value = static_cast<int64_t>(n->value);
    } else {
        for (const cbs::Expr* a : n->arg)
            if (a != nullptr) e->args.push_back(old_expr(a, made));
        if (n->op == cbs::Op::Extract) e->lo = n->lo;
    }
    made.emplace(n, e);
    return e;
}

inline const char* path_kind_text(ns::PathKind k) { return k == ns::PathKind::Jump ? "jump" : k == ns::PathKind::Inner ? "inner" : "pin"; }
inline const char* var_kind_text(ns::VarKind k) {
    switch (k) {
        case ns::VarKind::Arg: return "arg";
        case ns::VarKind::Reg: return "reg";
        case ns::VarKind::Mem: return "mem";
        case ns::VarKind::Ret: return "ret";
        case ns::VarKind::Carry: return "carry";
    }
    return "arg";
}

}  // namespace shim_detail

// One run's shadow in the recorder. kind 16 (a 16-bit run; selectors' bases read from the GDT), 32 (flat), 33 (a driver's
// flat ring-0 run: a service call also sets the carry flag to `carry`).
class Shadow {
  public:
    // lifter: emulate/pcode.py's for the processor; inputs: [(low, high)] -- the memory bytes that are inputs of the
    // run until written; span: (low, high) of the function's own code (its jumps are the path's)
    Shadow(int64_t kind_, std::shared_ptr<pcode::Lifter> lifter_, const py::list<std::tuple<int64_t, int64_t>>& inputs,
           const std::optional<std::tuple<int64_t, int64_t>>& span_ = std::nullopt, bool real_mode = false)
        : lifter(lifter_), span(span_), kind(kind_) {
        if (!native_trace::_load()) throw py::AttributeError("'NoneType' object has no attribute 'cbt_sh_new'");
        ns::Setup setup;
        setup.kind = static_cast<ns::Kind>(kind_);
        setup.mode = lifter_->mode.utf8();
        for (const auto& [name, place] : lifter_->regs.items())
            setup.registers.push_back({name.utf8(), static_cast<uint64_t>(std::get<0>(place)), static_cast<uint32_t>(std::get<1>(place))});
        setup.line = [lifter_](::cb::ByteView code, uint64_t offset) { return lifter_->line(::cb::compat::to_py(code), static_cast<int64_t>(offset)).utf8(); };
        for (const auto& lo_hi : inputs) setup.inputs.emplace_back(static_cast<uint64_t>(std::get<0>(lo_hi)), static_cast<uint64_t>(std::get<1>(lo_hi)));
        if (span_) setup.span = std::make_pair(static_cast<uint64_t>(std::get<0>(*span_)), static_cast<uint64_t>(std::get<1>(*span_)));
        setup.real_mode = real_mode;
        setup.gdt_addr = 0x1000;   // (windows16::GDT_ADDR)
        impl_ = ::cb::compat::guard([&] { return std::make_unique<ns::Shadow>(std::move(setup)); });
        handle = impl_->handle();
    }
    Shadow(const Shadow&) = delete;
    Shadow& operator=(const Shadow&) = delete;

    std::shared_ptr<pcode::Lifter> lifter;
    std::optional<std::tuple<int64_t, int64_t>> span;
    int64_t kind = 0;
    void* handle = nullptr;   // the recorder's handle (Python: self.handle)

    // --- what the runner calls ---------------------------------------------------------------------------
    // As shadow.Shadow.enter: argument k, `bits` wide, is (where, value[, k]).
    void enter(const py::list<Place>& places, int64_t bits = 16) {
        std::vector<ns::Place> converted;
        for (const Place& p : places) {
            ns::Place n;
            if (const auto* address = std::get_if<int64_t>(&p.where)) n.where = static_cast<uint64_t>(*address);
            else n.where = std::get<py::str>(p.where).utf8();
            n.value = static_cast<uint64_t>(p.value);
            if (p.k) {
                if (const auto* number = std::get_if<int64_t>(&*p.k)) n.input = *number;
                else n.input = std::get<py::str>(*p.k).utf8();
            }
            converted.push_back(std::move(n));
        }
        ::cb::compat::guard([&] { impl_->enter(converted, static_cast<int32_t>(bits)); });
    }
    void step(Uc& m, int64_t address, int64_t cs_base = 0) {
        shim_detail::HandleOnly engine(m.handle());
        ::cb::compat::guard([&] { impl_->step(engine, static_cast<uint64_t>(address), static_cast<uint64_t>(cs_base)); });
    }
    void after_call(Uc& m, bool symbolic_result, int64_t value, const std::optional<int64_t>& args_at = std::nullopt, int64_t nargs = 0,
                    const std::optional<py::str>& why = std::nullopt, int64_t word = 2, int64_t carry = 0) {
        shim_detail::HandleOnly engine(m.handle());
        ns::CallResult result;
        result.symbolic = symbolic_result;
        result.value = static_cast<uint64_t>(value);
        if (args_at) result.args_at = static_cast<uint64_t>(*args_at);
        result.args = nargs;
        if (why) result.why = why->utf8();
        result.word = static_cast<int32_t>(word);
        result.carry = static_cast<uint64_t>(carry);
        ::cb::compat::guard([&] { impl_->after_call(engine, result); });
    }
    void written(int64_t at, int64_t size) { impl_->written(static_cast<uint64_t>(at), static_cast<uint64_t>(size)); }
    void forget(const py::list<py::str>& names) {
        const std::vector<std::string> converted = ::cb::compat::from_py(names);
        impl_->forget(converted);
    }
    int64_t segbase(Uc& m, int64_t sel) {
        shim_detail::HandleOnly engine(m.handle());
        return static_cast<int64_t>(impl_->segbase(engine, static_cast<uint64_t>(sel)));
    }

    // --- what the run made (read back from the recorder on first use after the run) -------------------------
    py::list<PathEntry> path() { return export_().path; }
    py::list<Step> trail() { return export_().trail; }
    Vars vars() { return export_().vars; }
    py::set<py::str> lost() { return export_().lost; }    // why the conditions may not be all there is (empty: they are)
    int64_t pins() { return export_().pins; }
    int64_t steps() { return export_().steps; }
    bool off() { return export_().off; }

  private:
    friend void* handle_of(Shadow* shadow);
    struct Export {
        py::list<PathEntry> path;
        py::list<Step> trail;
        Vars vars;
        py::set<py::str> lost;
        int64_t pins = 0, steps = 0;
        bool off = false;
    };
    const Export& export_() {
        const std::shared_ptr<const ns::Run> run = ::cb::compat::guard([&] { return impl_->run(); });
        if (got_ && run == run_) return *got_;
        auto x = std::make_shared<Export>();
        std::unordered_map<const ::cb::emulate::symbolic::Expr*, symbolic::ExprP> made;
        for (const ns::PathEntry& e : run->path) {
            PathEntry o;
            o.site = static_cast<int64_t>(e.site);
            o.taken = e.taken;
            o.cond = shim_detail::old_expr(e.condition, made);
            o.kind = py::str(shim_detail::path_kind_text(e.kind));
            x->path.append(o);
        }
        for (const ns::Step& s : run->trail) x->trail.append(Step(static_cast<int64_t>(s.site), s.taken));
        for (const auto& [name, v] : run->vars) {
            Var o;
            o.kind = py::str(shim_detail::var_kind_text(v.kind));
            if (const auto* number = std::get_if<uint64_t>(&v.where)) o.where = static_cast<int64_t>(*number);
            else if (const auto* text = std::get_if<std::string>(&v.where)) o.where = py::str(*text);
            else o.where = py::none_t{};
            o.bits = v.bits;
            o.value = static_cast<int64_t>(v.value);
            x->vars.set(py::str(name), o);
        }
        for (const std::string& s : run->lost) x->lost.add(py::str(s));
        x->pins = run->pins;
        x->steps = run->steps;
        x->off = run->off;
        run_ = run;
        got_ = x;
        return *got_;
    }

    std::unique_ptr<ns::Shadow> impl_;
    std::shared_ptr<const ns::Run> run_;   // what got_ was made from (its nodes stay while the records are in use)
    std::shared_ptr<Export> got_;
};

inline void* handle_of(Shadow* shadow) {
    if (shadow == nullptr) return nullptr;
    shadow->got_ = nullptr;                      // (the recorder steps it from here on)
    return shadow->impl_->handle();
}

// The recorder is C++ only (the Python hooks it replaced are gone): raise (RuntimeError) when the DLL cannot be used here, saying why.
inline void require() {
    if (!enabled()) {
        std::optional<py::str> why = why_not();
        throw py::RuntimeError(py::str("the run recorder is native (native/cb/emulate/tracer, cbtrace.dll): ") + (why ? *why : py::str("None")));
    }
}

}  // namespace codebreaker::emulate::native_shadow
