// codebreaker/emulate/pcode.hpp -- SHIM over cb/emulate/pcode.hpp (docs/CPP_DESIGN.md 6.3): the old records and the
// process-wide lifter registry, answered by the new Lifter. Conversion and forwarding only; deleted with its last
// py:: consumer (emulate/shadow, native_shadow).
//
// The old process-wide `lifter(mode)` is kept as this header's function-local registry of one cb::LifterSet; the
// folder of the specifications and the pcodecheck setting come from the old decompiler/settings modules (COMPAT(py):
// remove when decompile/decompiler and base/cache are refactored).
#pragma once

#include <cstdint>
#include <map>
#include <memory>
#include <optional>
#include <tuple>
#include <variant>

#include "cb/compat/py_bridge.hpp"
#include "cb/emulate/pcode.hpp"
#include "codebreaker/base/cache.hpp"
#include "py_settings.hpp"
#include "py_stdlib.hpp"

namespace codebreaker::decompile::decompiler {
py::str _languages();   // COMPAT(py): remove when decompile/decompiler is refactored
}

namespace codebreaker::emulate::pcode {

// (space, offset, size)
struct Varnode {
    py::str space;      // "r" | "u" | "c" | "m"
    int64_t offset = 0;
    int64_t size = 0;
};
// One input of an op: a varnode, a user operation's name, or None.
using Input = std::variant<py::none_t, Varnode, py::str>;
// (OPCODE, out, inputs)
struct Op {
    py::str opcode;
    std::optional<Varnode> out;
    py::list<Input> inputs;
};

namespace shim {
inline py::str space_text(cb::emulate::pcode::Space s) {
    using S = cb::emulate::pcode::Space;
    return py::str(s == S::Register ? "r" : s == S::Unique ? "u" : s == S::Constant ? "c" : "m");
}
inline cb::emulate::pcode::Space space_of(const py::str& s) {
    using S = cb::emulate::pcode::Space;
    return s == py::str("r") ? S::Register : s == py::str("u") ? S::Unique : s == py::str("c") ? S::Constant : S::Ram;
}
inline Varnode varnode(const cb::emulate::pcode::Varnode& v) { return Varnode{space_text(v.space), static_cast<int64_t>(v.offset), v.size}; }
inline cb::emulate::pcode::Varnode varnode(const Varnode& v) {
    return {space_of(v.space), static_cast<uint64_t>(v.offset), static_cast<uint32_t>(v.size)};
}
inline Op op(const cb::emulate::pcode::Op& o) {
    Op out;
    out.opcode = py::str(o.opcode);
    if (o.out) out.out = varnode(*o.out);
    for (const auto& in : o.inputs) {
        if (const auto* v = std::get_if<cb::emulate::pcode::Varnode>(&in)) out.inputs.append(Input(varnode(*v)));
        else if (const auto* n = std::get_if<std::string>(&in)) out.inputs.append(Input(py::str(*n)));
        else out.inputs.append(Input(py::none_t{}));
    }
    return out;
}
inline cb::emulate::pcode::Op op(const Op& o) {
    cb::emulate::pcode::Op out;
    out.opcode = o.opcode.utf8();
    if (o.out) out.out = varnode(*o.out);
    for (const Input& in : o.inputs) {
        if (const auto* v = std::get_if<Varnode>(&in)) out.inputs.push_back(varnode(*v));
        else if (const auto* n = std::get_if<py::str>(&in)) out.inputs.push_back(n->utf8());
        else out.inputs.push_back(std::monostate{});
    }
    return out;
}
}  // namespace shim

// One instruction's p-code, and what the shadow needs to know of it without running it.
struct Lifted {
    int64_t length = 0;
    py::list<Op> ops;
    py::set<int64_t> reads;
    py::set<int64_t> writes;
    bool full = false;

    Lifted(int64_t length_, const py::list<Op>& ops_) : length(length_), ops(ops_) {
        std::vector<cb::emulate::pcode::Op> converted;
        for (const Op& o : ops_) converted.push_back(shim::op(o));
        const cb::emulate::pcode::Lifted l(static_cast<uint32_t>(length_), std::move(converted));
        for (uint64_t r : l.reads) reads.add(static_cast<int64_t>(r));
        for (uint64_t w : l.writes) writes.add(static_cast<int64_t>(w));
        full = l.full;
    }
    explicit Lifted(const cb::emulate::pcode::Lifted& l) : length(l.length), full(l.full) {
        for (const auto& o : l.ops) ops.append(shim::op(o));
        for (uint64_t r : l.reads) reads.add(static_cast<int64_t>(r));
        for (uint64_t w : l.writes) writes.add(static_cast<int64_t>(w));
    }
};

inline cb::emulate::pcode::Mode mode_of(const py::str& mode) {
    using M = cb::emulate::pcode::Mode;
    if (mode == py::str("x86-16-real")) return M::X86Real16;
    if (mode == py::str("x86-32-flat")) return M::X86Flat32;
    if (mode == py::str("ppc-be-64")) return M::PpcBe64;
    throw py::KeyError(py::repr(mode).utf8());
}

class Lifter {
  public:
    py::str mode;
    bool window = false;                                    // mode == "x86-16-real"
    py::dict<py::str, std::tuple<int64_t, int64_t>> regs;   // name -> (offset, size)

    Lifter(const py::str& exe, const py::str& sla, const py::str& mode_) : mode(mode_) {
        window = mode == py::str("x86-16-real");
        try {
            impl_ = std::make_shared<cb::emulate::pcode::Lifter>(exe.utf8(), sla.utf8(), mode_of(mode));
        } catch (const cb::ToolError& e) {
            throw py::OSError(e.what());
        }
        adopt();
    }
    // (the registry's: a Lifter some LifterSet already started)
    Lifter(std::shared_ptr<cb::emulate::pcode::Lifter> impl, const py::str& mode_) : mode(mode_), impl_(std::move(impl)) {
        window = mode == py::str("x86-16-real");
        adopt();
    }

    // pcodecheck's line for the instruction at the start of `code` at `offset`.
    py::str line(const py::bytes& code, int64_t offset) {
        return cb::compat::to_py(impl_->line(cb::compat::from_py(code), static_cast<uint64_t>(offset)));
    }

    // The p-code of the instruction at the start of `code` at `offset`; null when it does not decode. The same
    // instruction gets the same answer.
    std::shared_ptr<const Lifted> lift(const py::bytes& code, int64_t offset) {
        const cb::emulate::pcode::Lifted* l = impl_->lift(cb::compat::from_py(code), static_cast<uint64_t>(offset));
        if (l == nullptr) return nullptr;
        auto it = made_.find(l);
        if (it == made_.end()) it = made_.emplace(l, std::make_shared<const Lifted>(*l)).first;
        return it->second;
    }

  private:
    void adopt() {
        for (const auto& [name, slot] : impl_->registers()) regs.set(py::str(name), std::make_tuple(static_cast<int64_t>(slot.offset), static_cast<int64_t>(slot.size)));
    }
    std::shared_ptr<cb::emulate::pcode::Lifter> impl_;
    std::map<const cb::emulate::pcode::Lifted*, std::shared_ptr<const Lifted>> made_;
};

// the specification each lift mode reads (native/decompiler/languages)
inline const py::dict<py::str, py::str> SLA = {{py::str("x86-16-real"), py::str("x86.sla")},
                                               {py::str("x86-32-flat"), py::str("x86.sla")},
                                               {py::str("ppc-be-64"), py::str("ppc_64_xenon_be.sla")}};

// CB_PCODECHECK, else decompiler_bin/pcodecheck.exe
inline py::str _exe() {
    auto bin = py::settings::get_opt_str(py::str("decompiler_bin"));
    return cb::compat::to_py(cb::emulate::pcode::PcodeConfig::from_environment(bin ? bin->utf8() : std::string(), {}).exe.string());
}

// The process's lifter for `mode`, started on first use; null when there is none to start.
inline std::shared_ptr<Lifter> lifter(const py::str& mode = py::str("x86-16-real")) {
    static std::map<std::string, std::shared_ptr<Lifter>> started;
    auto it = started.find(mode.utf8());
    if (it == started.end()) {
        cb::emulate::pcode::Mode m = mode_of(mode);
        std::shared_ptr<Lifter> made;
        cb::emulate::pcode::PcodeConfig config{_exe().utf8(), decompile::decompiler::_languages().utf8()};
        if (auto impl = cb::emulate::pcode::Lifter::try_start(config.exe, config.sla(m), m)) {
            made = std::make_shared<Lifter>(std::shared_ptr<cb::emulate::pcode::Lifter>(std::move(impl)), mode);
        }
        it = started.emplace(mode.utf8(), made).first;
    }
    return it->second;
}

// What the lifted p-code depends on (for keys of anything kept from it): the program and the specification.
inline std::tuple<py::str, py::str> stamp(const py::str& mode = py::str("x86-16-real")) {
    return {base::cache::tool(_exe()), base::cache::tool(py::os::path::join(decompile::decompiler::_languages(), SLA.at(mode)))};
}

}  // namespace codebreaker::emulate::pcode
