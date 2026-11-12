// codebreaker/analyze/cb_image.hpp -- COMPAT bridge: an old analyze::image::Image (over py::, decoded by the DLL) as the
// cb::analyze::Image the refactored analyses (dispatch, structures ...) work on. Deleted with analyze/image, which is what
// makes the cb Image directly (cb/analyze/image.hpp).
//
// The cb Image is built from what the old image holds -- its spans, function starts and limits, names, imports, relocations
// and roots -- and kept in the old image's memo, so each module is converted once. The call graph the structures pass needs
// is kept the same way. The old image has already discovered its functions; the cb one is not asked to discover again.
#pragma once

#include <memory>

#include "cb/analyze/call_graph.hpp"
#include "cb/analyze/convention_facts.hpp"
#include "cb/analyze/native.hpp"
#include "cb/compat/py_bridge.hpp"
#include "codebreaker/analyze/conventions.hpp"
#include "codebreaker/analyze/image.hpp"
#include "py.hpp"

namespace codebreaker::analyze::bridge {

namespace cba = ::cb::analyze;

namespace detail {

inline cba::PointerKind pointer_kind(const py::str& kind) {
    if (kind == py::str("far")) return cba::PointerKind::Far;
    if (kind == py::str("selector")) return cba::PointerKind::Selector;
    if (kind == py::str("offset")) return cba::PointerKind::Offset;
    if (kind == py::str("byte")) return cba::PointerKind::Byte;
    return cba::PointerKind::Abs32;
}

inline cba::ImageSpec spec_of(const image::Image& img) {
    const native::Image n = img.as_native();
    cba::ImageSpec spec;
    spec.kind = n.kind == py::str("ne") ? cba::ImageKind::Ne : n.kind == py::str("le") ? cba::ImageKind::Le : cba::ImageKind::Pe;
    spec.bits = static_cast<int>(n.bits);
    spec.flat = n.flat;
    for (const native::Span& s : n.spans) {
        cba::Span out;
        out.va = std::get<0>(s);
        out.data = ::cb::compat::from_py(std::get<1>(s));
        out.executable = std::get<2>(s);
        out.writable = std::get<3>(s);
        if (std::get<4>(s)) out.name = std::get<4>(s)->utf8();
        out.initialised = std::get<5>(s);
        spec.spans.push_back(std::move(out));
    }
    for (const auto& [start, limit] : n.limits.items()) spec.functions.emplace_back(start, limit);
    for (const auto& [start, name] : n.names.items()) spec.function_names[start] = name.utf8();
    for (const auto& [va, name] : n.imports.items()) spec.imports[va] = name.utf8();
    if (n.pointers) {
        spec.has_pointers = true;
        for (const auto& [site, p] : n.pointers->items()) {
            cba::Pointer out;
            out.kind = pointer_kind(std::get<0>(p));
            out.target = std::get<1>(p);
            spec.pointers.emplace_back(site, out);
        }
    }
    if (n.data_segment) spec.data_segment = *n.data_segment;
    for (int64_t r : n.roots) spec.roots.push_back(r);
    return spec;
}

}  // namespace detail

// The image as the refactored analyses see it (made once per old image).
inline cba::Image& cb_image_of(image::Image& img) {
    return *img.memo<cba::Image>(py::str("cb.image"), [](image::Image& i) { return std::make_shared<cba::Image>(detail::spec_of(i)); });
}

// The call graph of that image (made once).
inline const cba::call_graph::Graph& cb_graph_of(image::Image& img) {
    return *img.memo<cba::call_graph::Graph>(py::str("cb.call_graph"), [](image::Image& i) {
        return std::make_shared<cba::call_graph::Graph>(cba::call_graph::build(cb_image_of(i)));
    });
}

// What the calling-convention pass said of each function, as the refactored analyses take it.
inline cba::ConventionFacts convention_facts_of(image::Image& img) {
    cba::ConventionFacts out;
    for (const auto& [start, fact] : conventions::functions(img)->items()) {
        cba::ConventionFact f;
        if (fact.convention) f.convention = fact.convention->utf8();
        if (fact.procedure) f.procedure = fact.procedure->utf8();
        if (fact.removes) f.removes = static_cast<int>(*fact.removes);
        f.frame = fact.frame;
        for (const py::str& r : fact.reg_args) f.reg_args.push_back(r.utf8());
        out.emplace(start, std::move(f));
    }
    return out;
}

}  // namespace codebreaker::analyze::bridge
