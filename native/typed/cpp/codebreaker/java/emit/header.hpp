// codebreaker/java/emit/header.hpp -- SHIM over cb/java/emit/header.hpp (docs/CPP_DESIGN.md 6.3); deleted with its
// last py:: consumer.
//
// Render a class's .h from the IR and a TypeResolver. The old class and resolver are converted to the new IR for the
// call (ir::to_new, types::TypeResolver::resolver()); the ledger events the new renderer records are appended to the
// old ledger. `two_phase` is the set of class names built in two phases (the pipeline sets it on the Python's
// resolver; here it is a parameter). The interface methods the MI-diamond forwards point at are converted too, so the
// converted class's forwards stay valid for the call.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/java/emit/header.hpp"
#include "codebreaker/java/ir.hpp"
#include "codebreaker/java/ledger.hpp"
#include "codebreaker/java/typemap.hpp"
#include "codebreaker/java/types.hpp"
#include "py.hpp"

namespace codebreaker::java::emit::header {

using ClassRef = py::ref<ir::JClass>;
using types::TypeResolver;

// What header.py reads of a transpile.CppStandard: its year and its label ('C++%02d' % (year % 100)).
struct CppStd {
    int64_t year = 0;
    py::str label;

    CppStd() = default;
    explicit CppStd(int64_t year_)
        : year(year_), label(cb::compat::to_py(cb::java::emit::header::CppStd::of_year(static_cast<int>(year_)).label)) {}
    CppStd(int64_t year_, const py::str& label_) : year(year_), label(label_) {}
};

// The banner: '// Auto-generated from <cls_name>.java\n// Target: <platform> / <C++ standard>'.
inline py::str _banner(const py::str& cls_name, const std::optional<CppStd>& cpp_std = std::nullopt,
                       const std::optional<py::str>& platform_label = std::nullopt) {
    std::optional<cb::java::emit::header::CppStd> std_;
    if (cpp_std) std_ = cb::java::emit::header::CppStd{static_cast<int>(cpp_std->year), cb::compat::from_py(cpp_std->label)};
    return cb::compat::to_py(cb::java::emit::header::banner(cb::compat::from_py(cls_name), std_,
                                                            platform_label ? cb::compat::from_py(*platform_label) : std::string()));
}

inline py::str render_header(const ClassRef& cls, const TypeResolver& resolver, ledger::Ledger* ledger = nullptr,
                             const std::optional<CppStd>& cpp_std = std::nullopt,
                             const std::optional<py::str>& platform_label = std::nullopt,
                             const std::optional<py::set<py::str>>& two_phase = std::nullopt) {
    return cb::compat::guard([&] {
        namespace cbh = cb::java::emit::header;
        cbh::Options options;
        if (cpp_std) options.cpp_std = cbh::CppStd{static_cast<int>(cpp_std->year), cb::compat::from_py(cpp_std->label)};
        if (platform_label) options.platform_label = cb::compat::from_py(*platform_label);
        if (two_phase) {
            for (const py::str& n : *two_phase) options.two_phase.insert(cb::compat::from_py(n));
        }
        for (const auto& pair : typemap::UNSIGNED_BYTE_SCALAR_FIELDS) {
            options.unsigned_byte_scalar_fields.insert({cb::compat::from_py(std::get<0>(pair)), cb::compat::from_py(std::get<1>(pair))});
        }
        cb::java::ir::JClass converted = ir::to_new(*cls);
        std::vector<cb::java::ir::Method> forwarded;   // what the MI forwards point at: alive for the call
        forwarded.reserve(static_cast<std::size_t>(cls->mi_forwards.size()));
        for (const auto& [method, base] : cls->mi_forwards) forwarded.push_back(ir::to_new(*method));
        std::size_t i = 0;
        for (const auto& [method, base] : cls->mi_forwards) converted.mi_forwards.push_back({&forwarded[i++], cb::compat::from_py(base)});

        cb::java::ledger::Ledger recorded;
        if (ledger != nullptr) options.ledger = &recorded;
        const std::string text = cbh::render_header(converted, resolver.resolver(), options);
        if (ledger != nullptr) {
            for (const cb::java::ledger::Event& e : recorded.events()) ledger->events.append(ledger::from_new(e));
        }
        return cb::compat::to_py(text);
    });
}

}  // namespace codebreaker::java::emit::header
