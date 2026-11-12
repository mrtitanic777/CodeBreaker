// codebreaker/java/runtime.hpp -- SHIM over cb/java/runtime.hpp (docs/CPP_DESIGN.md 6.3); deleted with its last py:: consumer.
//
// The old module held the runtime's files and the stub classes' member types in module variables built at import. They
// are answered from one cb::java::runtime::Runtime, made on first use from the java data folder (_paths::_HERE) and made
// again if the host moves that folder; the dicts below are the Python-shaped views of it, converted once per Runtime.
// `_GENERATED` was the stub half of FILES; its readers look a header up in FILES first, so both views answer from the one
// file map.
#pragma once

#include <memory>
#include <optional>

#include "cb/compat/py_bridge.hpp"
#include "cb/java/runtime.hpp"
#include "codebreaker/java/_paths.hpp"
#include "codebreaker/java/emit/header.hpp"
#include "py.hpp"
#include "py_os.hpp"

namespace codebreaker::java::runtime {

// One STUB_MEMBERS entry: {'parent': base_or_None, 'methods': {method: cpp_ret_type}}.
struct StubMembers {
    std::optional<py::str> parent;
    py::dict<py::str, py::str> methods;
};

namespace shim_ {

struct Views {
    std::unique_ptr<cb::java::runtime::Runtime> runtime;
    std::string here;
    py::dict<py::str, StubMembers> members;
    py::dict<py::str, py::str> files;
};

inline Views& views() {
    static Views v;
    const std::string here = cb::compat::from_py(_paths::_HERE());
    if (!v.runtime || v.here != here) {
        v.runtime = cb::compat::guard([&] {
            return std::make_unique<cb::java::runtime::Runtime>(cb::java::JavaData(cb::fs::from_text(here)));
        });
        v.here = here;
        v.members = py::dict<py::str, StubMembers>();
        for (const auto& [name, rec] : v.runtime->stub_members()) {
            StubMembers m;
            m.parent = cb::compat::to_py(rec.parent);
            for (const auto& [method, type] : rec.methods) m.methods.set(cb::compat::to_py(method), cb::compat::to_py(type));
            v.members.set(cb::compat::to_py(name), m);
        }
        v.files = py::dict<py::str, py::str>();
        for (const auto& [name, text] : v.runtime->files()) v.files.set(cb::compat::to_py(name), cb::compat::to_py(text));
    }
    return v;
}

}  // namespace shim_

// Return-type registry for the emitted stub classes, so expression-type inference can type a chain THROUGH a stub method
// result -- e.g. `Runtime::getRuntime()->maxMemory()`.
inline py::dict<py::str, StubMembers>& STUB_MEMBERS() { return shim_::views().members; }

// filename -> content
inline py::dict<py::str, py::str>& FILES() { return shim_::views().files; }

// the generated stubs; every one of them is in FILES
inline py::dict<py::str, py::str>& _GENERATED() { return shim_::views().files; }

// The canonical headers that are not part of the emitted FILES map -- empty = all wired.
inline py::list<py::str> unwired_canonicals() {
    return cb::compat::guard([&] {
        shim_::views();
        return cb::compat::to_py(shim_::views().runtime->unwired_canonicals());
    });
}

// The full runtime file set with the standard's rewrites and the platform's banners and timers applied.
inline py::dict<py::str, py::str> generate_files(const std::optional<emit::header::CppStd>& cpp_std = std::nullopt,
                                                 const std::optional<py::str>& platform_id = std::nullopt) {
    return cb::compat::guard([&] {
        cb::java::runtime::GenerateOptions options;
        if (cpp_std) options.cpp_std = cb::java::emit::header::CppStd{static_cast<int>(cpp_std->year), cb::compat::from_py(cpp_std->label)};
        if (platform_id) options.platform = cb::compat::from_py(*platform_id);
        py::dict<py::str, py::str> out;
        for (const auto& [name, text] : shim_::views().runtime->generate_files(options)) out.set(cb::compat::to_py(name), cb::compat::to_py(text));
        return out;
    });
}

}  // namespace codebreaker::java::runtime
