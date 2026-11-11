// codebreaker/translate/backends/asm.hpp -- COMPAT SHIM over cb/translate/backends/asm.hpp (docs/CPP_DESIGN.md 6.3).
//
// The module is native/cb/translate/backends/asm.{hpp,cpp} (namespace cb::translate::backends::assembly). This header
// keeps the old py:: API for the writers that still build a Document of dict-shaped units (port/asm16, the driver,
// image and PE writers, module_file). No logic of its own: convert, forward, and turn a declined Result back into
// the old `Decline` exception. (The namespace is asm_: "asm" is a C++ keyword.)
//
// `COMMENT`, the registry of comment leads by syntax, stays here as the old global it was: the new module takes the table
// as an argument, and this header hands it the current contents.
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>
#include <variant>

#include "cb/compat/py_bridge.hpp"
#include "cb/translate/backends/asm.hpp"
#include "py.hpp"

namespace codebreaker::translate::backends::asm_ {

// A node this back end does not write.
struct Decline : py::Exception {
    Decline() : py::Exception() {}
    explicit Decline(const std::string& m) : py::Exception(m) {}
    explicit Decline(const char* m) : py::Exception(m) {}
    explicit Decline(const py::str& m) : py::Exception(m.utf8()) {}
    const char* type() const override { return "Decline"; }
};

// One unit of a Module (a dict in the Python). A key that is absent is std::nullopt (or false for
// `within`, which is only ever tested for truth). All keys the back end reads are strings.
struct Unit {
    std::optional<py::str> node;
    std::optional<py::str> asm_;      // "asm" in the dict
    bool within = false;              // truthiness of the "within" key
    std::optional<py::str> name;
    std::optional<py::str> text;
    std::optional<py::str> syntax;
    bool empty() const { return !node && !asm_ && !within && !name && !text && !syntax; }   // the empty dict
};

// A document: `units` is a list of units in the Python, or (the one case the Python tolerates) a single
// unit dict, which is taken as a list of one -- std::variant says which it was.
struct Document {
    std::optional<py::str> node;
    std::variant<py::list<Unit>, Unit> units{py::list<Unit>()};
};

// (syntax -> the comment's lead where it is not ';')
inline py::dict<py::str, py::str> COMMENT;

namespace detail_shim {
namespace nw = cb::translate::backends::assembly;

inline nw::CommentLeads leads() {
    nw::CommentLeads out;
    for (const auto& kv : COMMENT.items()) out[std::get<0>(kv).utf8()] = std::get<1>(kv).utf8();
    return out;
}

// `syntax`, when given, is the one the comment is written in (as unit(u, syntax) was asked); else the unit's own
inline nw::Unit to_new(const Unit& u, const std::optional<py::str>& syntax = std::nullopt) {
    const std::string kind = u.node ? u.node->utf8() : "None";
    if (kind == "MachineFragment") {
        nw::MachineFragment f;
        f.name = u.name ? u.name->utf8() : "";
        if (u.asm_) f.text = u.asm_->utf8();
        f.within = u.within;
        return f;
    }
    if (kind == "Comment") {
        const std::optional<py::str>& written = syntax ? syntax : u.syntax;
        return nw::Comment{u.text ? u.text->utf8() : "", written ? written->utf8() : ""};
    }
    return nw::OtherUnit{kind};
}

inline nw::Document to_new(const Document& d) {
    nw::Document out;
    out.kind = d.node ? d.node->utf8() : "None";
    if (const auto* many = std::get_if<py::list<Unit>>(&d.units)) {
        for (const Unit& u : *many) out.units.push_back(to_new(u, u.syntax));
    } else if (const Unit& one = std::get<Unit>(d.units); !one.empty()) {
        out.units.push_back(to_new(one, one.syntax));
    }
    return out;
}

template <class T>
T unwrap(cb::Result<T> r) {
    if (!r.ok()) throw Decline(std::string(r.error().what()));
    return std::move(r).value();
}
}  // namespace detail_shim

// One comment line: `;` alone for an empty one, else `; text`.
inline py::str comment(const py::str& text, const std::optional<py::str>& syntax = std::nullopt) {
    return cb::compat::to_py(cb::translate::backends::assembly::comment(text.utf8(), syntax ? syntax->utf8() : "", detail_shim::leads()));
}

// The text of one unit, or None for a fragment whose text another holds.
inline std::optional<py::str> unit(const Unit& u, const std::optional<py::str>& syntax = std::nullopt) {
    return cb::compat::to_py(
        detail_shim::unwrap(cb::translate::backends::assembly::unit(detail_shim::to_new(u, syntax), detail_shim::leads())));
}

// -> (the file, {unit index: the 1-based line it begins on})
inline std::tuple<py::str, py::dict<int64_t, int64_t>> lines(const Document& doc) {
    const auto a = detail_shim::unwrap(cb::translate::backends::assembly::assemble(detail_shim::to_new(doc), detail_shim::leads()));
    py::dict<int64_t, int64_t> first;
    for (std::size_t k = 0; k < a.first_line.size(); ++k)
        if (a.first_line[k]) first.set(static_cast<int64_t>(k), static_cast<int64_t>(*a.first_line[k]));
    return {cb::compat::to_py(a.text), first};
}

// The whole file the Module stands for.
inline py::str module(const Document& doc) {
    return cb::compat::to_py(detail_shim::unwrap(cb::translate::backends::assembly::module(detail_shim::to_new(doc), detail_shim::leads())));
}

}  // namespace codebreaker::translate::backends::asm_
