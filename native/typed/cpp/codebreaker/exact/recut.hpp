// codebreaker/exact/recut.hpp -- compat shim for cb/exact/recut.hpp (docs/CPP_DESIGN.md 6.3). Conversion and forwarding
// only; deleted with its last old-layer consumer (exact/project, exact/link).
//
//     recut::recut(project, raw) -> the object module's bytes (unchanged when its relocations are not the program's)
//
// `Project` is exact.project.Project as recut reads it: symbols(), the packed original's relocations, the EXE header and
// the file the relocation table is read from.
#pragma once

#include <cstdint>
#include <optional>

#include "cb/compat/py_bridge.hpp"
#include "cb/exact/recut.hpp"
#include "codebreaker/exact/compare.hpp"
#include "py.hpp"

namespace codebreaker::exact::recut {

// exact.project.Project, as recut.py reads it
class Project {
  public:
    virtual ~Project() = default;
    virtual compare::Symbols symbols() const = 0;                       // prj.symbols(): name -> (kind, address)
    // prj.packed.relocs when getattr(prj, "packed", None) is truthy, else nullopt
    virtual std::optional<py::list<int64_t>> packed_relocs() const = 0;
    virtual py::list<int64_t> header() const = 0;                       // prj.header (empty or None: no header)
    virtual py::bytes file() const = 0;                                 // prj.file: the original program's file
};

namespace detail {

// What the new module reads, built from the old project; `table`, `file` and `symbols` own what `original` points at.
struct Converted {
    cb::exact::SymbolTable symbols;
    cb::Bytes file;
    cb::exact::recut::Original original;
    explicit Converted(const Project& prj)
        : symbols(make_symbols(prj)), file(cb::compat::from_py(prj.file())), original{symbols, std::nullopt, std::nullopt, file} {
        using cb::compat::int_from_py;
        if (const auto packed = prj.packed_relocs()) {
            std::vector<std::uint32_t> relocs;
            for (int64_t r : *packed) relocs.push_back(int_from_py<std::uint32_t>(r));
            original.packed_relocs = std::move(relocs);
        }
        const py::list<int64_t> h = prj.header();
        if (!h.empty()) original.table = cb::exact::recut::RelocTable{int_from_py<std::uint32_t>(h[3]), int_from_py<std::uint32_t>(h[12])};
    }

  private:
    static cb::exact::SymbolTable make_symbols(const Project& prj) {
        cb::exact::SymbolTable table;
        for (const auto& kv : prj.symbols().items()) {
            const std::string k = std::get<0>(std::get<1>(kv)).utf8();
            const cb::exact::SymKind kind = k == "C"   ? cb::exact::SymKind::Code
                                            : k == "D" ? cb::exact::SymKind::Data
                                            : k == "S" ? cb::exact::SymKind::Segment
                                                       : cb::exact::SymKind::Unknown;
            table.insert_or_assign(std::get<0>(kv).utf8(), cb::exact::SymPlace{kind, std::get<1>(std::get<1>(kv))});
        }
        return table;
    }
};

}  // namespace detail

// the program's relocations, image addresses in the order of its table (EXEPACK's when it is packed)
inline py::list<int64_t> order(const Project& prj) {
    return cb::compat::guard([&] {
        const detail::Converted c(prj);
        return cb::compat::to_py(cb::exact::recut::relocation_order(c.original));
    });
}

// the object module's bytes (raw itself when its relocations are not the program's)
inline py::bytes recut(const Project& prj, const py::bytes& raw) {
    return cb::compat::guard([&] {
        const detail::Converted c(prj);
        const cb::Bytes made = cb::exact::recut::recut(c.original, cb::compat::from_py(raw));
        return cb::compat::to_py(cb::ByteView(made));
    });
}

}  // namespace codebreaker::exact::recut
