// codebreaker/java/emit/source.hpp -- SHIM over cb/java/emit/source.hpp (docs/CPP_DESIGN.md 6.3); deleted with its last py::
// consumer.
//
// Render a class's .cpp from the IR and a TypeResolver. The old class and resolver are converted to the new IR for the call
// (ir::to_new, types::TypeResolver::resolver()); the ledger events the new renderer records are appended to the old
// ledger. The translator's back end is the old cpp03 `Bodies`, which answers by class name and old method, so the new
// back-end interface is answered by an adapter over it: a member of the converted class is found again by its position in
// the old class. `TREE_BODIES` is the module global the transpiler sets for a conversion; `two_phase` is the set of class
// names built in two phases (the pipeline sets it on the Python's resolver; here it is a parameter).
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/java/emit/source.hpp"
#include "codebreaker/java/emit/header.hpp"
#include "codebreaker/java/ir.hpp"
#include "codebreaker/java/ledger.hpp"
#include "codebreaker/java/typemap.hpp"
#include "codebreaker/java/types.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::java::emit::source {

using ClassRef = py::ref<ir::JClass>;
using MethodRef = py::ref<ir::Method>;
using types::TypeResolver;
using header::CppStd;

// (kind 'super'/'this'/None, its arguments or None, body lines) of a constructor.
struct CtorParts {
    std::optional<py::str> kind;
    std::optional<py::str> args;
    py::list<py::str> body;
};

// The translator's back end (translate.backends.cpp03.Bodies): every body, constructor and initialiser is written from the
// typed IR. Each lookup returns nullopt when it declined.
class Bodies {
  public:
    virtual ~Bodies() = default;
    // The C++ initialiser expression of field `name`.
    virtual std::optional<py::str> field_init(const py::str& cls_name, const py::str& name) = 0;
    // The C++ elements of static field `name`'s `new T[]{...}` initialiser.
    virtual std::optional<py::list<py::str>> array_items(const py::str& cls_name, const py::str& name) = 0;
    // `this->f = init;` for each instance field initialiser and the instance {} blocks, in source order.
    virtual std::optional<py::list<py::str>> instance_inits(const py::str& cls_name) = 0;
    // The constructor `old` (null: the implicit one) of class `cls_name`.
    virtual std::optional<CtorParts> ctor(const py::str& cls_name, const MethodRef& old, bool two_phase = false) = 0;
    // The C++ arguments of enum constant `const_`'s constructor call.
    virtual std::optional<py::list<py::str>> enum_args(const py::str& cls_name, const py::str& const_) = 0;
    // Each static {} block's lines, in source order.
    virtual std::optional<py::list<py::list<py::str>>> static_blocks(const py::str& cls_name) = 0;
    // Body lines for the method `old` of class `cls_name`.
    virtual std::optional<py::list<py::str>> body(const py::str& cls_name, const MethodRef& old) = 0;
};

// Set by the transpiler for the conversion; null: no back end (everything is a J2X-GAP).
inline Bodies* TREE_BODIES = nullptr;

namespace shim_ {

namespace cbs = cb::java::emit::source;

inline py::list<py::str> to_old_lines(const std::vector<std::string>& lines) { return cb::compat::to_py(lines); }
inline std::vector<std::string> from_old_lines(const py::list<py::str>& lines) { return cb::compat::from_py(lines); }

// The new back-end interface over the old one, for one class.
class Adapter : public cbs::BodySource {
  public:
    Adapter(Bodies& old, const ClassRef& cls) : old_(old), cls_(cls) {}

    std::optional<std::string> field_init(const cb::java::ir::JClass& cls, const cb::java::ir::Field& f) override {
        const auto r = old_.field_init(cb::compat::to_py(cls.name), cb::compat::to_py(f.name));
        return r ? std::optional<std::string>(cb::compat::from_py(*r)) : std::nullopt;
    }
    std::optional<std::vector<std::string>> array_items(const cb::java::ir::JClass& cls, const cb::java::ir::Field& f) override {
        const auto r = old_.array_items(cb::compat::to_py(cls.name), cb::compat::to_py(f.name));
        return r ? std::optional<std::vector<std::string>>(from_old_lines(*r)) : std::nullopt;
    }
    std::optional<std::vector<std::string>> instance_inits(const cb::java::ir::JClass& cls) override {
        const auto r = old_.instance_inits(cb::compat::to_py(cls.name));
        return r ? std::optional<std::vector<std::string>>(from_old_lines(*r)) : std::nullopt;
    }
    std::optional<cbs::CtorBody> ctor(const cb::java::ir::JClass& cls, const cb::java::ir::Method* m, bool two_phase) override {
        const auto r = old_.ctor(cb::compat::to_py(cls.name), m != nullptr ? method_of(cls, *m) : MethodRef(), two_phase);
        if (!r) return std::nullopt;
        cbs::CtorBody out;
        if (r->kind && *r->kind == py::str("super")) out.call = cbs::CtorCall::Super;
        else if (r->kind && *r->kind == py::str("this")) out.call = cbs::CtorCall::This;
        if (r->args) out.super_args = cb::compat::from_py(*r->args);
        out.lines = from_old_lines(r->body);
        return out;
    }
    std::optional<std::vector<std::string>> enum_args(const cb::java::ir::JClass& cls, const cb::java::ir::EnumConst& c) override {
        const auto r = old_.enum_args(cb::compat::to_py(cls.name), cb::compat::to_py(c.name));
        return r ? std::optional<std::vector<std::string>>(from_old_lines(*r)) : std::nullopt;
    }
    std::optional<std::vector<std::vector<std::string>>> static_blocks(const cb::java::ir::JClass& cls) override {
        const auto r = old_.static_blocks(cb::compat::to_py(cls.name));
        if (!r) return std::nullopt;
        std::vector<std::vector<std::string>> blocks;
        for (const py::list<py::str>& b : r->vec()) blocks.push_back(from_old_lines(b));
        return blocks;
    }
    std::optional<std::vector<std::string>> body(const cb::java::ir::JClass& cls, const cb::java::ir::Method& m) override {
        const auto r = old_.body(cb::compat::to_py(cls.name), method_of(cls, m));
        return r ? std::optional<std::vector<std::string>>(from_old_lines(*r)) : std::nullopt;
    }

  private:
    // The old method at the converted method's position in the class (to_new keeps the order).
    MethodRef method_of(const cb::java::ir::JClass& cls, const cb::java::ir::Method& m) const {
        return cls_->methods[static_cast<int64_t>(&m - cls.methods.data())];
    }

    Bodies& old_;
    ClassRef cls_;
};

}  // namespace shim_

inline std::optional<py::str> render_source(const ClassRef& cls, const TypeResolver& resolver, ledger::Ledger* ledger = nullptr,
                                            const std::optional<CppStd>& cpp_std = std::nullopt,
                                            const std::optional<py::str>& platform_label = std::nullopt,
                                            const std::optional<py::set<py::str>>& two_phase = std::nullopt) {
    return cb::compat::guard([&]() -> std::optional<py::str> {
        namespace cbs = cb::java::emit::source;
        cbs::Options options;
        if (cpp_std) options.cpp_std = cb::java::emit::header::CppStd{static_cast<int>(cpp_std->year), cb::compat::from_py(cpp_std->label)};
        if (platform_label) options.platform_label = cb::compat::from_py(*platform_label);
        if (two_phase) {
            for (const py::str& n : *two_phase) options.two_phase.insert(cb::compat::from_py(n));
        }
        cb::java::ir::JClass converted = ir::to_new(*cls);
        std::vector<cb::java::ir::Method> forwarded;   // what the MI forwards point at: alive for the call
        forwarded.reserve(static_cast<std::size_t>(cls->mi_forwards.size()));
        for (const auto& [method, base] : cls->mi_forwards) forwarded.push_back(ir::to_new(*method));
        std::size_t i = 0;
        for (const auto& [method, base] : cls->mi_forwards) converted.mi_forwards.push_back({&forwarded[i++], cb::compat::from_py(base)});

        std::optional<shim_::Adapter> adapter;
        if (TREE_BODIES != nullptr) {
            adapter.emplace(*TREE_BODIES, cls);
            options.bodies = &*adapter;
        }
        cb::java::ledger::Ledger recorded;
        if (ledger != nullptr) options.ledger = &recorded;
        const cbs::Rendered r = cbs::render_source(converted, resolver.resolver(), options);
        if (ledger != nullptr) {
            for (const cb::java::ledger::Event& e : recorded.events()) ledger->events.append(ledger::from_new(e));
        }
        cls->has_init_statics = r.has_init_statics;
        return r.text ? std::optional<py::str>(cb::compat::to_py(*r.text)) : std::nullopt;
    });
}

}  // namespace codebreaker::java::emit::source
