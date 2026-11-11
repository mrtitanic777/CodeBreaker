// codebreaker/build/definitions.hpp -- SHIM (docs/CPP_DESIGN.md 6.3) over cb/build/definitions.hpp for the ports that still
// use the old API (build/module). Staged under native/cb/compat/shims/ until the exe's build is wired for C++20.
//
// What the new module owns -- the three renderers (definition_file, export_directives, public_header) -- is forwarded here:
// conversion only. What it does not own yet is READING a module's interface (the NE and PE readers and the corpus's idea of a
// module's lane belong to formats/ne, formats/pe and base/corpus, not refactored): those seven functions are declared below
// and stay defined by the old definitions.cpp, which shrinks to just them -- COMPAT(py): remove when those modules are
// refactored and cb/build/definitions gains `read_interface`.
//
// Old API: Export, Import, is_library, module_name, description, exports, imports, definition_file, export_directives,
// public_header.
#pragma once

#include <functional>
#include <optional>
#include <tuple>
#include <variant>

#include "cb/build/definitions.hpp"
#include "cb/compat/py_bridge.hpp"

namespace codebreaker::build::definitions {

// One export. `address` is a virtual address (32-bit) or (segment, offset) (16-bit); `kind` is "code", "data" or "forward".
struct Export {
    int64_t ordinal;
    std::optional<py::str> name;
    std::variant<uint64_t, std::tuple<int64_t, int64_t>> address;
    py::str kind;
    bool resident;
    py::str target;
    bool shared_data;   // 16-bit: the loader gives it the module's data segment

    Export(int64_t ordinal_, const std::optional<py::str>& name_, const std::variant<uint64_t, std::tuple<int64_t, int64_t>>& address_,
           const py::str& kind_, bool resident_ = false, const py::str& target_ = py::str(""), bool shared_data_ = true)
        : ordinal(ordinal_), name(name_), address(address_), kind(kind_), resident(resident_), target(target_), shared_data(shared_data_) {}

    // <export NAME @ORDINAL KIND>  (the Python's __repr__)
    py::str repr() const {
        return py::percent("<export %s @%d %s>", name && !name->empty() ? *name : py::str("(no name)"), ordinal, kind);
    }
};

using Import = std::tuple<py::str, std::variant<py::str, int64_t>>;   // (MODULE, name or ordinal)

// ---- COMPAT(py): defined by the old definitions.cpp (it needs formats/ne, formats/pe and base/corpus) -------------------
bool is_library(const py::str& path);
py::str module_name(const py::str& path);
py::str description(const py::str& path);
py::list<Export> exports(const py::str& path);
py::set<Import> imports(const py::str& path);
bool is_ne(const py::str& path);                              // the corpus's lane for the file is "ne"
std::tuple<int64_t, int64_t> ne_heap_and_stack(const py::str& path);   // a 16-bit module's header: heap, stack

// ---- conversions ---------------------------------------------------------------------------------------------------------
namespace detail {

namespace nd = cb::build::definitions;

inline nd::Export to_new(const Export& e) {
    nd::Export out;
    out.ordinal = cb::compat::int_from_py<std::uint32_t>(e.ordinal);
    if (e.name) out.name = e.name->utf8();
    if (const auto* va = std::get_if<uint64_t>(&e.address)) {
        out.address = *va;
    } else {
        const auto& [seg, off] = std::get<std::tuple<int64_t, int64_t>>(e.address);
        out.address = cb::Far16{cb::compat::int_from_py<std::uint16_t>(seg), cb::compat::int_from_py<std::uint16_t>(off)};
    }
    out.kind = e.kind == py::str("data") ? nd::ExportKind::Data : e.kind == py::str("forward") ? nd::ExportKind::Forward : nd::ExportKind::Code;
    out.resident = e.resident;
    out.forward_target = e.target.utf8();
    out.shared_data = e.shared_data;
    return out;
}

inline Export to_old(const nd::Export& e) {
    std::optional<py::str> name;
    if (e.name) name = py::str(*e.name);
    std::variant<uint64_t, std::tuple<int64_t, int64_t>> address;
    if (const auto* va = std::get_if<std::uint64_t>(&e.address)) {
        address = *va;
    } else {
        const cb::Far16 f = std::get<cb::Far16>(e.address);
        address = std::tuple<int64_t, int64_t>{f.seg, f.off};
    }
    const char* kind = e.kind == nd::ExportKind::Data ? "data" : e.kind == nd::ExportKind::Forward ? "forward" : "code";
    return Export(e.ordinal, name, address, py::str(kind), e.resident, py::str(e.forward_target), e.shared_data);
}

// The facts the renderers need, read through the old readers.
inline nd::Interface interface_of(const py::str& path) {
    nd::Interface iface;
    iface.kind = is_ne(path) ? nd::ModuleKind::Win16 : nd::ModuleKind::Win32;
    iface.library = is_library(path);
    iface.name = module_name(path).utf8();
    iface.description = description(path).utf8();
    if (iface.kind == nd::ModuleKind::Win16) {
        const auto [heap, stack] = ne_heap_and_stack(path);
        iface.heap = cb::compat::int_from_py<std::uint32_t>(heap);
        iface.stack = cb::compat::int_from_py<std::uint32_t>(stack);
    }
    for (const Export& e : exports(path)) iface.exports.push_back(to_new(e));
    for (const Import& i : imports(path)) {
        nd::Import in;
        in.module = std::get<0>(i).utf8();
        const auto& what = std::get<1>(i);
        if (const auto* name = std::get_if<py::str>(&what)) in.what = name->utf8();
        else in.what = cb::compat::int_from_py<std::uint32_t>(std::get<int64_t>(what));
        iface.imports.push_back(std::move(in));
    }
    return iface;
}

inline std::string file_name_of(const py::str& path) {
    const std::string p = path.utf8();
    const std::size_t slash = p.find_last_of("/\\");
    return slash == std::string::npos ? p : p.substr(slash + 1);
}

}  // namespace detail

// The module's interface as a module-definition file.
inline py::str definition_file(const py::str& path) {
    return cb::compat::guard([&] {
        return py::str(cb::build::definitions::definition_file(detail::interface_of(path), detail::file_name_of(path)));
    });
}

// Linker `export` lines for every export whose address has a symbol in the build. -> ([line], [(Export, why it could not be exported)])
inline std::tuple<py::list<py::str>, py::list<std::tuple<Export, py::str>>> export_directives(
    const py::str& path, const std::function<std::optional<py::str>(const Export&)>& symbol_at) {
    return cb::compat::guard([&] {
        const auto d = cb::build::definitions::export_directives(
            detail::interface_of(path), [&](const cb::build::definitions::Export& e) -> std::optional<std::string> {
                const std::optional<py::str> s = symbol_at(detail::to_old(e));
                if (!s) return std::nullopt;
                return s->utf8();
            });
        py::list<py::str> lines;
        for (const std::string& l : d.lines) lines.append(py::str(l));
        py::list<std::tuple<Export, py::str>> missing;
        for (const auto& m : d.missing) missing.append(std::make_tuple(detail::to_old(m.what), py::str(m.why)));
        return std::make_tuple(lines, missing);
    });
}

// A C header declaring the module's exported functions.
inline py::str public_header(const py::str& path, const std::function<std::optional<py::str>(const Export&)>& declaration_at) {
    return cb::compat::guard([&] {
        return py::str(cb::build::definitions::public_header(
            detail::interface_of(path), detail::file_name_of(path), [&](const cb::build::definitions::Export& e) -> std::optional<std::string> {
                const std::optional<py::str> s = declaration_at(detail::to_old(e));
                if (!s) return std::nullopt;
                return s->utf8();
            }));
    });
}

}  // namespace codebreaker::build::definitions
