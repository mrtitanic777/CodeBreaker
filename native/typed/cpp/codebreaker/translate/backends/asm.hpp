// codebreaker/translate/backends/asm.hpp -- typed C++ of codebreaker.translate.backends.asm
// (the namespace is asm_: "asm" is a C++ keyword).
//
// L2 to assembly language: a Module whose parts stay machine code, as the file its assembler reads.
// The L2 documents are dicts in the Python; the keys this back end reads are fixed, so a unit is a
// record (Unit) and a document is a record (Document).
#pragma once

#include <cstdint>
#include <optional>
#include <tuple>
#include <variant>

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
extern py::dict<py::str, py::str> COMMENT;

// One comment line: `;` alone for an empty one, else `; text`.
py::str comment(const py::str& text, const std::optional<py::str>& syntax = std::nullopt);

// The text of one unit, or None for a fragment whose text another holds.
std::optional<py::str> unit(const Unit& u, const std::optional<py::str>& syntax = std::nullopt);

// -> (the file, {unit index: the 1-based line it begins on})
std::tuple<py::str, py::dict<int64_t, int64_t>> lines(const Document& doc);

// The whole file the Module stands for.
py::str module(const Document& doc);

}  // namespace codebreaker::translate::backends::asm_
