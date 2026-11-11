// codebreaker/exact/readable.hpp -- compat shim for cb/exact/readable.hpp (docs/CPP_DESIGN.md 6.3). No logic:
// conversion and forwarding. Deleted with its last old-layer consumer.
//
// readable.py -- the sources with the names SYMBOLS has put in for the engine's address-names, proven to change nothing.
//
//     python -m codebreaker exact PROJECT readable [--write]
#pragma once

#include <cstdint>
#include <filesystem>
#include <memory>
#include <optional>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/core/log.hpp"
#include "cb/exact/readable.hpp"
#include "py.hpp"

namespace codebreaker::exact::readable {

// An address-name's place: ("f", address of a function) or ("g", offset of a variable).
using Place = std::tuple<py::str, int64_t>;
// {address-name: name}
using Names = py::dict<Place, py::str>;
// project.symbols(): {name: (type "C" | "D" | "S" | "?", address)}
using Symbols = py::dict<py::str, std::tuple<py::str, int64_t>>;
// {(file, function): verdict}
using Counts = py::dict<std::tuple<py::str, py::str>, py::str>;

// One row of the ledger (ledger.run): file, function, address (None: no place), length, verdict, notes, toolchains.
struct Row {
    py::str file;
    py::str name;
    std::optional<int64_t> address;
    int64_t length = 0;
    py::str verdict;
    py::list<py::str> notes;
    py::list<py::str> by;
};

// The part of exact.project.Project and exact.ledger that main() uses.
class Project {
public:
    virtual ~Project() = default;
    virtual py::str dir() const = 0;                                      // prj.dir
    virtual py::str src() const = 0;                                      // prj.src
    virtual Symbols symbols() const = 0;                                  // prj.symbols()
    virtual py::list<std::tuple<int64_t, py::str>> sources() const = 0;   // prj.sources(): [(address, path)]
    virtual py::list<Row> ledger_run() const = 0;                         // ledger.run(prj)
    // projmod.Project(prj.dir, override={"src": src})
    virtual std::unique_ptr<Project> with_src(const py::str& src) const = 0;
};

namespace detail {

inline cb::exact::SymbolTable table_of(const Symbols& symbols) {
    cb::exact::SymbolTable t;
    for (const auto& kv : symbols.items()) {
        const std::string kind = std::get<0>(std::get<1>(kv)).utf8();
        t.insert_or_assign(std::get<0>(kv).utf8(),
                           cb::exact::SymPlace{kind == "C" ? cb::exact::SymKind::Code : kind == "D" ? cb::exact::SymKind::Data
                                               : kind == "S" ? cb::exact::SymKind::Segment : cb::exact::SymKind::Unknown,
                                               std::get<1>(std::get<1>(kv))});
    }
    return t;
}

// The new interface over an old Project (it owns the twin projects with_src makes).
class View final : public cb::exact::readable::Project {
public:
    explicit View(const readable::Project& p) : p_(&p) { init(); }
    explicit View(std::unique_ptr<readable::Project> owned) : owned_(std::move(owned)), p_(owned_.get()) { init(); }
    const cb::exact::SymbolTable& symbols() const override { return symbols_; }
    std::vector<cb::exact::readable::SourceFile> sources() const override {
        std::vector<cb::exact::readable::SourceFile> out;
        for (const auto& s : p_->sources()) out.push_back({cb::compat::int_from_py<std::uint32_t>(std::get<0>(s)), std::filesystem::path(std::get<1>(s).utf8())});
        return out;
    }
    const std::filesystem::path& source_dir() const override { return src_; }
    std::vector<cb::exact::readable::LedgerRow> ledger() const override {
        std::vector<cb::exact::readable::LedgerRow> out;
        for (const Row& r : p_->ledger_run()) out.push_back({r.file.utf8(), r.name.utf8(), r.verdict.utf8()});
        return out;
    }
    std::unique_ptr<cb::exact::readable::Project> with_source_dir(const std::filesystem::path& dir) const override {
        return std::make_unique<View>(p_->with_src(py::str(dir.generic_string())));
    }

private:
    void init() {
        symbols_ = table_of(p_->symbols());
        src_ = std::filesystem::path(p_->src().utf8());
    }
    std::unique_ptr<readable::Project> owned_;
    const readable::Project* p_;
    cb::exact::SymbolTable symbols_;
    std::filesystem::path src_;
};

}  // namespace detail

// {address-name: name} - from SYMBOLS, for the places it has a real name for
inline Names mapping(const Symbols& symbols) {
    Names out;
    for (const auto& [place, name] : cb::exact::readable::mapping(detail::table_of(symbols)))
        out.set(Place(py::str(place.kind == cb::exact::readable::Place::Kind::Function ? "f" : "g"), place.address), py::str(name));
    return out;
}
inline Names mapping(const Project& prj) { return mapping(prj.symbols()); }

// the text with each address-name that has a name renamed - unless the name is in the text already, as something else
inline py::str rename(const py::str& text, const Names& names) {
    cb::exact::readable::NameMap map;
    for (const auto& kv : names.items()) {
        const Place& p = std::get<0>(kv);
        map.emplace(cb::exact::readable::Place{std::get<0>(p).utf8() == "f" ? cb::exact::readable::Place::Kind::Function : cb::exact::readable::Place::Kind::Variable,
                                               cb::compat::int_from_py<std::uint32_t>(std::get<1>(p))},
                    std::get<1>(kv).utf8());
    }
    return py::str(cb::exact::readable::rename(text.utf8(), map));
}

// {(file, function): verdict} of the ledger's rows
inline Counts counts(const py::list<Row>& rows) {
    Counts out;
    for (const Row& r : rows) out.set(std::make_tuple(r.file, r.name), r.verdict);
    return out;
}
inline Counts counts(const Project& prj) { return counts(prj.ledger_run()); }

// The command: 0 kept (or nothing to do), 1 not kept / no names. Prints its report to stdout.
inline int64_t main(const Project& prj, const py::list<py::str>& argv) {
    cb::exact::readable::Options options;
    for (const py::str& a : argv)
        if (a == py::str("--write")) options.write = true;
    const detail::View view(prj);
    const int64_t code = cb::compat::guard([&] { return cb::exact::readable::run(view, options); });
    cb::flush_out();   // (the report is in order with what the old layer prints)
    return code;
}

}  // namespace codebreaker::exact::readable
