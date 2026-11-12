// codebreaker/translate/backends/module_file.hpp -- COMPAT SHIM over cb/translate/backends/module_file.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/translate/backends/module_file.{hpp,cpp}. This header keeps the old py:: API for source/module_writer:
// the facts and the C document as records of py:: values, `c` and `asm_`. No logic of its own: convert, forward, and turn a
// declined Result back into the old `Decline` exception. (`_counts` and `unit` were this module's own helpers, used by nothing
// else, and are not kept; `banner` and `asm_banner` are.)
#pragma once

#include <optional>
#include <string>
#include <tuple>

#include "cb/compat/py_bridge.hpp"
#include "cb/translate/backends/module_file.hpp"
#include "codebreaker/translate/backends/asm.hpp"
#include "py.hpp"

namespace codebreaker::translate::backends::module_file {

// from codebreaker.translate.backends import asm as _asm
namespace backend_asm = ::codebreaker::translate::backends::asm_;

// facts["whole"]: how the whole module built (present = a non-empty dict)
struct Whole {
    bool linked = false;
    int64_t exports_same = 0;
    int64_t exports_total = 0;
    py::str error;
    int64_t rejected = 0;
    int64_t absent = 0;
    py::str built;
};

// one of facts["rows"]: (address, name, title, what)
struct Row {
    py::str va;
    py::str name;
    py::str title;
    py::str what;
};

// the per-binary facts
struct Facts {
    py::str name;
    py::str stem;
    py::str date;
    py::str digest;
    py::list<py::str> order;
    py::dict<py::str, py::str> titles;
    py::dict<py::str, int64_t> counts;
    int64_t total = 0;
    py::str kind;
    std::optional<Whole> whole;
    int64_t real = 0;
    bool header = false;
    std::optional<py::str> how;
    py::list<Row> rows;
};

// One unit of the C document (a dict in the Python); an absent key is std::nullopt.
struct Unit {
    std::optional<py::str> node;
    std::optional<py::list<py::str>> comments;
    std::optional<py::str> text;
};

// The C document: a Module whose units are the functions.
struct Document {
    std::optional<py::str> node;
    py::list<Unit> units;
};

namespace detail_shim {
namespace nw = cb::translate::backends::module_file;

inline nw::Facts to_new(const Facts& f) {
    nw::Facts out;
    out.name = f.name.utf8();
    out.stem = f.stem.utf8();
    out.date = f.date.utf8();
    out.digest = f.digest.utf8();
    for (const py::str& c : f.order) out.statuses.push_back({f.titles.at(c).utf8(), static_cast<std::size_t>(f.counts.at(c))});
    out.total = static_cast<std::size_t>(f.total);
    const std::string kind = f.kind.utf8();
    out.lane = kind == "powerpc" ? nw::Lane::PowerPc : kind == "ne" ? nw::Lane::Ne : kind == "le" ? nw::Lane::Le
               : kind == "dos" ? nw::Lane::Dos : nw::Lane::Pe;
    if (f.whole) {
        const Whole& w = *f.whole;
        out.whole = nw::Whole{w.linked, static_cast<std::size_t>(w.exports_same), static_cast<std::size_t>(w.exports_total),
                              w.error.utf8(), static_cast<std::size_t>(w.rejected), static_cast<std::size_t>(w.absent),
                              w.built.utf8()};
    }
    out.real = static_cast<std::size_t>(f.real);
    out.header = f.header;
    if (f.how) out.how = f.how->utf8();
    for (const Row& r : f.rows) out.rows.push_back({r.va.utf8(), r.name.utf8(), r.title.utf8(), r.what.utf8()});
    return out;
}

// The old document is checked as the Python did: a Module whose units are functions and comments.
inline std::vector<nw::Part> to_new(const Document& d) {
    const std::string kind = d.node ? d.node->utf8() : "None";
    if (kind != "Module") throw backend_asm::Decline("document " + kind);
    std::vector<nw::Part> out;
    for (const Unit& u : d.units) {
        const std::string k = u.node ? u.node->utf8() : "None";
        if (k == "MethodDecl") {
            out.push_back({u.comments && u.comments->size() > 0 ? (*u.comments)[0].utf8() : std::string(), u.text ? u.text->utf8() : ""});
        } else if (k == "Comment") {
            out.push_back({u.text ? u.text->utf8() : "", std::nullopt});
        } else {
            throw backend_asm::Decline("document " + k);
        }
    }
    return out;
}
}  // namespace detail_shim

// The head of the C file: the status, how the module builds, what the labels mean.
inline py::list<py::str> banner(const Facts& facts) {
    return cb::compat::to_py(cb::translate::backends::module_file::banner(detail_shim::to_new(facts)));
}

// The module's C, as files: [(file name, text)] -- one, or parts of at most `split` functions each (the status in the first).
inline py::list<std::tuple<py::str, py::str>> c(const Document& doc, const Facts& facts, int64_t split = 0) {
    const auto files = cb::translate::backends::module_file::c(detail_shim::to_new(doc), detail_shim::to_new(facts),
                                                               static_cast<std::size_t>(split));
    py::list<std::tuple<py::str, py::str>> out;
    for (const auto& f : files) out.append(std::make_tuple(cb::compat::to_py(f.name), cb::compat::to_py(f.text)));
    return out;
}

// The head of a driver's or a real-mode program's assembly file, as comments.
inline py::list<py::str> asm_banner(const Facts& facts) {
    return cb::compat::to_py(cb::translate::backends::module_file::asm_banner(detail_shim::to_new(facts)));
}

// A driver's or a real-mode program's one assembly file: the status at its head as comments, then the module's source.
inline py::str asm_(const backend_asm::Document& doc, const Facts& facts) {
    const auto source = backend_asm::detail_shim::to_new(doc);
    auto text = cb::translate::backends::module_file::assembly_file(source.units, detail_shim::to_new(facts));
    if (!text.ok()) throw backend_asm::Decline(std::string(text.error().what()));
    return cb::compat::to_py(std::move(text).value());
}

}  // namespace codebreaker::translate::backends::module_file
