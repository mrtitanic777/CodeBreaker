// codebreaker/recompile/switches.hpp -- compat shim for cb/recompile/switches.hpp (docs/CPP_DESIGN.md 6.3). Conversion
// and forwarding; deleted with its last old-layer consumer (recompile/config, recompile/x360project).
//
// switches.py -- the switch tables of an Xbox 360 image, for XenonRecomp.
//
//     tables, report = switches::find(xex_or_image_path)
//     switches::write(path, tables)            // XenonRecomp's switch-table file
//
// The old port's scanner internals (Image, Scanner, decode, Expr ...) were never an interface; the new module keeps
// them in its unnamed namespace and takes a CodeImage instead of a path. Reading an image from a path is the
// formats modules' job; until powerpc_image and xex are refactored this shim does it with the old ones
// (COMPAT(py): remove when formats/powerpc_image is refactored, find() then takes a cb::...::CodeImage).
#pragma once

#include <algorithm>
#include <cstdint>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/fs.hpp"
#include "cb/recompile/switches.hpp"
#include "codebreaker/formats/powerpc_image.hpp"
#include "codebreaker/formats/xex.hpp"
#include "py.hpp"

namespace codebreaker::recompile::switches {

// One switch table found.
struct Table {
    int64_t bctr = 0;
    int64_t r = 0;                           // the index register
    py::str kind;                            // absolute | byte | short | XenonAnalyse
    py::list<int64_t> labels;
    int64_t default_ = 0;                    // `default` in the Python
    std::optional<int64_t> guard;            // address of the cmplwi, None for an unguarded table
    std::optional<int64_t> table;            // unguarded only
    std::optional<bool> inline_;             // unguarded only
    std::optional<std::tuple<int64_t, int64_t>> func;   // set by find: the .pdata function holding the bctr
};

// The counts find() answers with (a dict in the Python).
struct Report {
    int64_t tables = 0;
    py::dict<py::str, int64_t> kinds;
    py::list<std::tuple<int64_t, py::str>> unresolved;
    py::list<std::tuple<int64_t, std::tuple<int64_t, int64_t>>> outside;
    py::list<int64_t> pointer_calls;
};

namespace detail {

inline cb::recompile::switches::TableKind kind_of(const py::str& name) {
    namespace n = cb::recompile::switches;
    const std::string s = name.utf8();
    for (const n::TableKind k : {n::TableKind::Absolute, n::TableKind::Byte, n::TableKind::Short, n::TableKind::XenonAnalyse}) {
        if (n::kind_name(k) == s) return k;
    }
    throw py::ValueError("switches: no table kind " + s);
}

inline Table to_old(const cb::recompile::switches::SwitchTable& t) {
    using cb::compat::to_py;
    Table out;
    out.bctr = t.bctr;
    out.r = t.index_reg;
    out.kind = to_py(cb::recompile::switches::kind_name(t.kind));
    for (const std::uint32_t l : t.labels) out.labels.append(l);
    out.default_ = t.default_target;
    if (t.guard) {
        out.guard = static_cast<int64_t>(*t.guard);
    } else {
        out.table = t.table;
        out.inline_ = t.inline_table;
    }
    if (t.function) out.func = std::tuple<int64_t, int64_t>(t.function->begin, t.function->end);
    return out;
}

inline cb::recompile::switches::SwitchTable from_old(const Table& t) {
    cb::recompile::switches::SwitchTable out;
    out.bctr = static_cast<std::uint32_t>(t.bctr);
    out.index_reg = static_cast<std::uint32_t>(t.r);
    out.kind = kind_of(t.kind);
    for (const int64_t l : t.labels) out.labels.push_back(static_cast<std::uint32_t>(l));
    out.default_target = static_cast<std::uint32_t>(t.default_);
    if (t.guard) out.guard = static_cast<std::uint32_t>(*t.guard);
    out.table = static_cast<std::uint32_t>(t.table.value_or(0));
    out.inline_table = t.inline_.value_or(false);
    return out;
}

}  // namespace detail

// -> ([table], {counts}) for an image or .xex.
inline std::tuple<py::list<Table>, Report> find(const py::str& path) {
    namespace pi = codebreaker::formats::powerpc_image;
    return cb::compat::guard([&] {
        const py::ref<pi::Image> image = pi::read(codebreaker::formats::xex::is_xex(path) ? codebreaker::formats::xex::basefile(path) : path);
        cb::recompile::switches::CodeImage code;
        code.base = static_cast<std::uint32_t>(image->base);
        code.code_begin = static_cast<std::uint32_t>(std::get<0>(image->code));
        code.code_end = static_cast<std::uint32_t>(std::get<1>(image->code));
        code.data = cb::compat::from_py(image->bytes_at(image->base, std::get<1>(image->code) - image->base));
        for (const auto& [start, info] : image->extents.items()) {
            code.functions.push_back({static_cast<std::uint32_t>(start), static_cast<std::uint32_t>(start + std::get<0>(info))});
        }
        std::sort(code.functions.begin(), code.functions.end(), [](const auto& a, const auto& b) { return a.begin < b.begin; });
        const cb::recompile::switches::Found found = cb::recompile::switches::find(code);

        py::list<Table> tables;
        for (const auto& t : found.tables) tables.append(detail::to_old(t));
        Report report;
        report.tables = static_cast<int64_t>(found.report.tables);
        for (const auto& [kind, n] : found.report.kinds) report.kinds.set(py::str(std::string(cb::recompile::switches::kind_name(kind))), static_cast<int64_t>(n));
        for (const auto& u : found.report.unresolved) report.unresolved.append({static_cast<int64_t>(u.bctr), py::str(u.why)});
        for (const auto& o : found.report.outside) {
            report.outside.append({static_cast<int64_t>(o.bctr), std::tuple<int64_t, int64_t>(o.function.begin, o.function.end)});
        }
        for (const std::uint32_t a : found.report.pointer_calls) report.pointer_calls.append(a);
        return std::tuple<py::list<Table>, Report>(tables, report);
    });
}

// XenonRecomp's switch-table file.
inline void write(const py::str& path, const py::list<Table>& found) {
    cb::compat::guard([&] {
        std::vector<cb::recompile::switches::SwitchTable> tables;
        for (const Table& t : found) tables.push_back(detail::from_old(t));
        cb::recompile::switches::write(cb::fs::from_text(path.utf8()), tables);
    });
}

}  // namespace codebreaker::recompile::switches
