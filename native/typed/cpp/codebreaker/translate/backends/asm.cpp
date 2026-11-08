// codebreaker/translate/backends/asm.cpp -- typed C++ of codebreaker.translate.backends.asm.
//
// asm.py -- L2 to assembly language: a Module whose parts stay machine code, as the file its assembler reads.
//
//     text = asm.module(doc)                  // the whole file
//     text, first = asm.lines(doc)            // ... and the line each unit begins on (1-based), by unit
//
// The back end of every lane whose output is assembly (docs/IR_UNIFICATION.md step 4): the whole-file
// NASM writers (source/ne_writer.py, pe_writer.py, le_writer.py), the MASM writers (driver_writer.py,
// dos_writer.py), the console and home-computer writers through source/image_writer.py, the 16-bit
// port's assembly (port/asm16.py), and the head of a driver's source file (backends/module_file.py).
// Each of those works out what to write -- which bytes are code, which instructions its assembler
// gives back as they were, what each label is -- and builds the module as one L2 tree as it goes
// (l2.Assembly): each part of the file a MachineFragment (a section, a segment, a function; its
// origin, its name, the assembler it is written for). This writes the tree as the file:
//
//     MachineFragment     its `asm`, as it is (a fragment `within` another has its text there)
//     Comment             a comment line in the assembler's spelling (`; text`)
//
// The parts are joined by a line break, nothing added and nothing taken away, so the file is the
// text the writer laid out, whatever the assembler. A unit of any other kind is not assembly: it
// is declined (Decline), and the lane that holds it writes it with the back end of its language
// (backends/c.py for C).

#include "codebreaker/translate/backends/asm.hpp"

namespace codebreaker::translate::backends::asm_ {

py::dict<py::str, py::str> COMMENT;

namespace {
// "%s" of a node that may be None
py::str node_text(const std::optional<py::str>& node) { return node ? *node : py::str("None"); }
}  // namespace

py::str comment(const py::str& text, const std::optional<py::str>& syntax) {
    py::str lead = COMMENT.get((syntax && !syntax->empty()) ? *syntax : py::str(""), py::str(";"));
    return lead + (!text.empty() ? py::str(" ") + text : py::str(""));
}

std::optional<py::str> unit(const Unit& u, const std::optional<py::str>& syntax) {
    const std::optional<py::str>& kind = u.node;
    if (kind && *kind == py::str("MachineFragment")) {
        if (!u.asm_) {
            if (u.within) {
                return std::nullopt;
            }
            throw Decline(py::percent(py::str("fragment %s has no text"), (u.name && !u.name->empty()) ? *u.name : py::str("?")));
        }
        return *u.asm_;
    }
    if (kind && *kind == py::str("Comment")) {
        return comment(u.text ? *u.text : py::str(""), syntax);
    }
    throw Decline(py::percent(py::str("document %s"), node_text(kind)));
}

std::tuple<py::str, py::dict<int64_t, int64_t>> lines(const Document& doc) {
    if (!(doc.node && *doc.node == py::str("Module"))) {
        throw Decline(py::percent(py::str("document %s"), node_text(doc.node)));
    }
    // `units or []`; a single unit dict is a list of one
    py::list<Unit> units;
    if (const auto* many = std::get_if<py::list<Unit>>(&doc.units)) {
        units = *many;
    } else {
        const Unit& one = std::get<Unit>(doc.units);
        if (!one.empty()) {
            units.append(one);
        }
    }
    py::list<py::str> parts;
    py::dict<int64_t, int64_t> first;
    int64_t line = 1;
    int64_t k = 0;
    for (const Unit& u : units) {
        std::optional<py::str> text = unit(u, u.syntax);
        if (!text) {
            ++k;
            continue;
        }
        first.set(k, line);
        parts.append(*text);
        line += text->count(py::str("\n")) + 1;
        ++k;
    }
    return {py::join(py::str("\n"), parts), first};
}

py::str module(const Document& doc) {
    return std::get<0>(lines(doc));
}

}  // namespace codebreaker::translate::backends::asm_
