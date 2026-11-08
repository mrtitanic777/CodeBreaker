// codebreaker/translate/frontends/clang/ctypes.hpp -- typed C++ port of
// codebreaker.translate.frontends.clang.ctypes.
//
// ctypes.py -- clang's type spellings ("const char *", "int (*)[4]", "geo::Vec &") as IR types, and the
// sizes and layouts the source's compiler gives them.
//
// clang's JSON AST writes an expression's or declaration's type as text (`qualType`), so the front end
// reads C's declarator grammar back: specifiers, then an abstract declarator (pointers bind looser than
// array and function suffixes; a parenthesised part applies last). Typedef names, struct/union/class tags
// and C++ class names are resolved through the `names` callback the lowering supplies, so a type is
// always concrete: `uint8_t` is u8, `Pair` is the record it names.
//
// The data model is the target's: `Model::for_triple("x86_64-pc-windows-msvc")` is LLP64 (long is 4 bytes,
// pointers 8, plain char signed, long double = double), which is what the judge's clang build runs.
//
// Typing notes:
//   * The Python's Type(kind, name, args, elem, bound, **info) keeps the keyword details in `info`; here
//     they are an ir::Dict of ir::Val (None, bool, int, float, str, a Type, a list or a dict of Vals).
//   * Model.long is long_. Model.json() returns that Dict. Python ints are int64_t.
//   * Parser and Layouts hold the Python's callbacks as std::function: `names(name, args, tag)` returns the
//     Type a name means, or a null TypeP for None (tag is None, 'struct', 'union', 'class' or 'enum');
//     `record(type)` returns a 'record' Type, or null.
//   * The Python's private tables _QUALS, _BUILTIN, _INT, _UINT, _PRIM_SIZE and _TOKEN, and _anon_key(), are
//     kept in the .cpp (as QUALS, BUILTIN, INT_NAMES, UINT_NAMES, PRIM_SIZE, token_pattern()) except
//     _anon_key, which is anon_key_text.
//   * A token handed to int() that is not ASCII digits (the Python reads any Unicode digit) and an array
//     size past int64 are outside this port: the first is a ValueError, the second an OverflowError.
//   * Parser::t is empty, not None, outside a parse().
#pragma once

#include <functional>
#include <optional>
#include <tuple>

#include "codebreaker/translate/ir.hpp"

namespace codebreaker::translate::frontends::clang::ctypes {

namespace ir = codebreaker::translate::ir;

// ------------------------------------------------------------------------------------------ data models
// Widths the target gives the types C leaves open.
struct Model {
    py::str triple;
    int64_t pointer;
    int64_t long_;
    int64_t wchar;
    bool char_signed;
    int64_t long_double;

    Model(const py::str& triple, int64_t pointer, int64_t long_, int64_t wchar, bool char_signed = true,
          int64_t long_double = 8);

    static Model for_triple(const py::str& triple);

    ir::Dict json(const py::str& language, const py::str& std_) const;
};

// ['unsigned', 'long', 'long'] -> ir.Type, by the model.
ir::TypeP builtin(const py::list<py::str>& words, const Model& model);

// ------------------------------------------------------------------------------------------ the parser
py::list<py::str> tokens(const py::str& s);

// '(unnamed struct at C:\x\t.c:3:9)' -> '3_9' (the line and column clang named it by). (Python: _anon_key)
py::str anon_key_text(const py::str& text);

py::str anon_key(int64_t line, int64_t col);

// parse(text) -> ir.Type. `names(name, args, tag)` resolves a typedef / record / class name: it returns
// an ir.Type (a typedef's meaning, or Type('class', qname, args)), or null if unknown.
class Parser {
  public:
    using Names = std::function<ir::TypeP(const py::str& name, const py::list<ir::TypeP>& args,
                                          const std::optional<py::str>& tag)>;

    Parser(const Model& model, const Names& names);

    ir::TypeP parse(const py::str& text);
    std::optional<py::str> peek(int64_t k = 0) const;
    std::optional<py::str> take();
    std::optional<py::str> take(const char* want);
    ir::TypeP type_();
    ir::TypeP specifiers();
    std::tuple<py::str, py::list<ir::TypeP>> qualified_name();
    py::list<ir::TypeP> template_args();
    ir::TypeP declarator(ir::TypeP t);
    ir::TypeP suffixes(ir::TypeP t);

    Model model;
    Names names;
    py::dict<py::str, ir::TypeP> cache;
    py::list<py::str> t;
    int64_t i = 0;
};

ir::TypeP fn_type(const ir::TypeP& ret, const py::list<ir::TypeP>& params, bool variadic = false,
                  const std::optional<py::list<std::optional<py::str>>>& names = std::nullopt);

// A function type as a pointer to it; an array type as a pointer to its element.
ir::TypeP decay(const ir::TypeP& t);

// Every pointer to a function as an 'fnptr', all the way down.
ir::TypeP normalise(const ir::TypeP& t);

// ------------------------------------------------------------------------------------------ sizes
// sizeof / alignof by the model; records through `record(name)` -> ir 'record' Type (or null).
class Layouts {
  public:
    using Record = std::function<ir::TypeP(const ir::TypeP&)>;

    Layouts(const Model& model, const Record& record);

    int64_t size(const ir::TypeP& t) const;
    int64_t align(const ir::TypeP& t) const;
    std::tuple<int64_t, int64_t> size_align(const ir::TypeP& t) const;

    // A record's alignment: its widest field's (L2 keeps offsets and size, not the alignment).
    int64_t record_align(const ir::TypeP& r) const;

    // fields [(name, Type)] -> ('record' Type, align): C's rules -- each field at the next multiple
    // of its alignment, the whole padded to the largest alignment.
    std::tuple<ir::TypeP, int64_t> lay_out(const py::str& name,
                                           const py::list<std::tuple<py::str, ir::TypeP>>& fields,
                                           bool union_ = false) const;

    Model model;
    Record record;
};

}  // namespace codebreaker::translate::frontends::clang::ctypes
