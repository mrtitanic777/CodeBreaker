// codebreaker/exact/emu/xenix.hpp -- compat shim for cb/exact/emu/xenix.hpp (docs/CPP_DESIGN.md 6.3). No logic: conversion
// and forwarding. Deleted with its last old-layer consumers (exact/xenix_c, exact/emu/dpmi). Only what a consumer uses is
// kept: the old command line (`main`), `load_unicorn` (the new code finds unicorn.dll beside the program, so it is a no-op) and
// the Python module's constants.
#pragma once

#include <functional>
#include <optional>
#include <variant>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/exact/emu/xenix.hpp"
#include "py.hpp"
#include "py_os.hpp"

// Unicorn's register numbers, which the old hooks name registers by
#pragma warning(push, 0)
#include <unicorn/unicorn.h>
#pragma warning(pop)

namespace codebreaker::exact::emu::xenix {

// unicorn.UcError: the engine refused (a memory access outside what is mapped ...).
struct UcError : py::Exception {
    explicit UcError(const std::string& m) : py::Exception(m) {}
    const char* type() const override { return "UcError"; }
};

// A system call as traced: (number, BX, CX, SI, DI).
using Call = std::tuple<int64_t, int64_t, int64_t, int64_t, int64_t>;

class Result {
  public:
    py::bytes console;                  // output of fd 1 and 2
    std::optional<int64_t> status;      // exit status (None: it did not exit)
    py::list<Call> calls;               // the traced system calls
    std::optional<py::str> error;       // why the run stopped short, or None

    Result() = default;
};

// The emulated machine, as an old hook sees it. Register ids are Unicorn's own (UC_X86_REG_*); memory is the machine's
// linear space.
class Machine {
  public:
    virtual ~Machine() = default;
    virtual int64_t reg_read(int64_t reg) = 0;
    virtual void reg_write(int64_t reg, int64_t value) = 0;
    virtual py::bytes mem_read(int64_t address, int64_t size) = 0;
    virtual void mem_write(int64_t address, const py::bytes& data) = 0;
    virtual void emu_stop() = 0;
};

// A function called with the machine and the two segments each time the instruction at its address is about to be done.
using Hook = std::function<void(Machine&, int64_t cs, int64_t ds)>;

// An argument of the program: text (encoded as UTF-8) or bytes.
using Arg = std::variant<py::str, py::bytes>;

// (the new code finds unicorn.dll itself)
inline void load_unicorn(const py::str&) {}

namespace detail {

namespace nx = cb::exact::emu::xenix;

inline nx::Reg new_register(int64_t id) {
    switch (id) {
        case UC_X86_REG_AX: return nx::Reg::Ax;
        case UC_X86_REG_BX: return nx::Reg::Bx;
        case UC_X86_REG_CX: return nx::Reg::Cx;
        case UC_X86_REG_DX: return nx::Reg::Dx;
        case UC_X86_REG_SI: return nx::Reg::Si;
        case UC_X86_REG_DI: return nx::Reg::Di;
        case UC_X86_REG_BP: return nx::Reg::Bp;
        case UC_X86_REG_SP: return nx::Reg::Sp;
        case UC_X86_REG_IP: return nx::Reg::Ip;
        case UC_X86_REG_CS: return nx::Reg::Cs;
        case UC_X86_REG_DS: return nx::Reg::Ds;
        case UC_X86_REG_ES: return nx::Reg::Es;
        case UC_X86_REG_SS: return nx::Reg::Ss;
        case UC_X86_REG_EFLAGS: return nx::Reg::Flags;
        default: break;
    }
    throw UcError("Invalid register id (UC_ERR_ARG)");
}

// The old Machine over a new one.
class OldMachine : public Machine {
  public:
    explicit OldMachine(nx::Machine& m) : m_(m) {}
    int64_t reg_read(int64_t reg) override { return guarded([&] { return static_cast<int64_t>(m_.reg(new_register(reg))); }); }
    void reg_write(int64_t reg, int64_t value) override {
        guarded([&] { m_.set_reg(new_register(reg), cb::compat::int_from_py<std::uint32_t>(value & 0xFFFFFFFF)); return 0; });
    }
    py::bytes mem_read(int64_t address, int64_t size) override {
        return guarded([&] { return cb::compat::to_py(cb::ByteView(m_.read(cb::compat::int_from_py<std::uint32_t>(address), cb::compat::int_from_py<std::size_t>(size)))); });
    }
    void mem_write(int64_t address, const py::bytes& data) override {
        guarded([&] { m_.write(cb::compat::int_from_py<std::uint32_t>(address), cb::compat::from_py(data)); return 0; });
    }
    void emu_stop() override { guarded([&] { m_.stop(); return 0; }); }

  private:
    template <class F>
    static auto guarded(F&& f) {
        try {
            return f();
        } catch (const cb::EmulationError& e) {
            throw UcError(e.what());
        }
    }
    nx::Machine& m_;
};

}  // namespace detail

// -> Result (console output of fd 1 and 2, exit status)
inline Result run(const py::str& root, const py::str& work, const py::str& prog, const py::list<Arg>& args, const py::bytes& stdin_ = py::bytes(),
                  bool trace = false, int64_t limit = 400000000, const py::dict<int64_t, Hook>& hooks = py::dict<int64_t, Hook>()) {
    using cb::compat::from_py;
    using cb::compat::int_from_py;
    namespace nx = cb::exact::emu::xenix;
    return cb::compat::guard([&] {
        nx::Spec spec;
        spec.root = cb::fs::from_text(from_py(root));
        spec.work = cb::fs::from_text(from_py(work));
        spec.program = from_py(prog);
        for (const Arg& a : args) {
            if (std::holds_alternative<py::str>(a)) spec.args.push_back(from_py(std::get<py::str>(a)));
            else spec.args.push_back(std::get<py::bytes>(a).raw());
        }
        spec.stdin_data = from_py(stdin_);
        spec.trace = trace;
        spec.limit = int_from_py<std::uint64_t>(limit);
        for (const auto& kv : hooks.items()) {
            const Hook old_hook = std::get<1>(kv);
            spec.hooks[int_from_py<std::uint32_t>(std::get<0>(kv))] = [old_hook](nx::Machine& m, std::uint16_t cs, std::uint16_t ds) {
                detail::OldMachine adapter(m);
                try {
                    old_hook(adapter, cs, ds);
                } catch (const UcError& e) {
                    throw cb::EmulationError(e.what());   // (what the old run caught as the engine's own error)
                }
            };
        }
        const nx::Result r = nx::run(spec);
        Result out;
        out.console = cb::compat::to_py(cb::ByteView(r.console));
        if (r.status) out.status = *r.status;
        for (const nx::SysCall& c : r.calls) out.calls.append(Call(c.number, c.bx, c.cx, c.si, c.di));
        if (r.error) out.error = cb::compat::to_py(*r.error);
        return out;
    });
}

}  // namespace codebreaker::exact::emu::xenix
