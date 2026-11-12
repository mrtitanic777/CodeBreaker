// cb/translate/frontends/clang/lower.hpp -- clang's typed AST (`-ast-dump=json`) into the translator's IR.
//
// What clang resolved is what the IR says: every implicit conversion is an explicit Cast (marked implicit), every name is
// the declaration clang bound it to, overloads and template instantiations are the ones clang chose. Only the declarations
// of the program's own files are lowered (not those of the headers it includes); a call into a library keeps the header
// that declared it as the symbol's owner.
//
//     Arena arena;
//     clang::Options options{.main_path = "C:/p/prog.c"};
//     clang::Lowered out = clang::lower_text(arena, dump_text, options);   // ParseError for text that is not JSON
//     ir::Node* unit = out.unit;                                            // an ir::Unit, in `arena`
//     for (const clang::Refusal& r : out.refused) ...                       // what became ir::Unsupported, and why
//
// C's model in the IR:
//   * a translation unit is one Unit: its structs/unions/classes (ClassDecl, with their byte layout for C/C++ records), its
//     functions (MethodDecl kind "function"), its globals (FieldDecl, sym kind "global"); C enum constants are int globals
//     (mods const), a C++ `enum class` a ClassDecl kind "enum";
//   * pointers are ptr types; `*p` and `&x` are Unary "*" and "&"; `p->f` is Select on `*p`;
//   * a C++ reference is a ptr with `ref`: its reads are Unary "*", a binding Unary "&";
//   * `this` is a pointer (This typed ptr to the class), so `this->x` is Select on `*this`;
//   * a string literal's value is its bytes, one character per byte (latin-1, written as UTF-8); its type char[N];
//   * sizeof/alignof are Lit values by the target's layout, `text` keeping the spelling;
//   * an aggregate initialiser is NewArray (arrays) or New with memory "stack" and the field values in order (structs);
//     missing trailing elements are zero, as in C;
//   * a static local is a Local with mods ["static"].
// Anything not handled becomes ir::Unsupported (its kind), never a guess; a declaration or an expression the dump does not
// describe the way clang writes it (a member missing, a value of the wrong kind) becomes one too, with the reason as `raw`.
#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include "cb/core/arena.hpp"
#include "cb/core/json_fwd.hpp"
#include "cb/translate/ir.hpp"

namespace cb::translate::frontends::clang {

struct Options {
    std::string main_path;                       // the source file the dump was made of
    // The files whose declarations are lowered. Not given: the main file and the files beside it (its own headers).
    std::optional<std::vector<std::string>> own;
    std::string language = "c";                  // "c", or "c++" ("cpp", "cxx")
    std::optional<std::string> standard;
    std::string triple = "x86_64-pc-windows-msvc";
};

// A construct that became ir::Unsupported: where, which kind of construct, and why.
struct Refusal {
    std::uint32_t line = 0;
    std::optional<std::string> kind;
    std::optional<std::string> why;
};

struct Lowered {
    ir::Node* unit = nullptr;       // an ir::Unit, its nodes made in the arena the caller gave
    std::vector<Refusal> refused;   // in the order a walk of the unit meets them
};

// One translation unit's dump -> its Unit.
Lowered lower(Arena& arena, const Json& tree, const Options& options);
// The same from a dump's text. ParseError when the text is not JSON.
Lowered lower_text(Arena& arena, std::string_view dump_text, const Options& options);

// A C string literal as clang spells it (`"a\n"`, `u8".."`, `L".."`) -> its bytes, one character per byte (written as
// UTF-8 of the latin-1 character); a wide literal's UTF-16 code units, one character each. ParseError for a literal that
// is not one (a UTF-32 literal, a bad escape).
std::string string_literal_value(std::string_view spelling);

// The spelling of a character constant in a Lit's `text` ('a', '\n'), or nothing for a value that has none.
std::optional<std::string> char_literal_text(std::int64_t value);

// A canonical spelling of a type: the key template instances and records are found by.
std::string spell(const ir::TypeP& type);

// An integer constant reduced to its type's width, as C does (a type that is no integer keeps the value).
std::int64_t wrap_to_type(std::int64_t value, const ir::TypeP& type);

}  // namespace cb::translate::frontends::clang
